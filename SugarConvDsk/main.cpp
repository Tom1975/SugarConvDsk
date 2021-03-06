//////////////////////////////////////////////////////////
// SugarConvDsk
//
// Convert disk format supported by Sugarbox
//
// (c) T.Guillemin 2016 -
//
//
//////////////////////////////////////////////////////////

#ifdef __MORPHOS__

#include "stdafx.h"
#include "DiskGen.h"
#include "DiskBuilder.h"
#include "FormatTypeCTRAW.h"
#include "FormatTypeDSK.h"
#include "FormatTypeEDSK.h"
#include "FormatTypeHFE.h"
#include "FormatTypeIPF.h"
#include "FormatTypeRAW.h"
#include "FormatTypeSCP.h"

#include <proto/dos.h>

#define PROGRAM_NAME      "SugarConvDsk"
#define PROGRAM_VERSION   "1.0"
#define PROGRAM_DATE      "20/05/2018"
#define PROGRAM_COPYRIGHT "� 2016-2018 Thomas Guillemin, MorphOS port by Philippe Rimauro"

// Regular AmigaDOS CLI arguments template for ReadArgs()
#define PROG_ARG_TEMPLATE "FROM/A,A=SIDEA/S,B=SIDEB/S,TO,CAPS=IPF/S,SUPERCARDPRO=SCP/S,HXC=HFE/S,EXTENDEDDSK=EDSK/S,QUIET/S"

struct ProgArgs
{
   STRPTR  pa_SrcFileName;    // FROM/A
   IPTR    pa_SrcSideA;       // A=SIDEA/S
   IPTR    pa_SrcSideB;       // B=SIDEB/S
   STRPTR  pa_DstFileName;    // TO
   IPTR    pa_DstIPF;         // CAPS=IPF/S
   IPTR    pa_DstSCP;         // SUPERCARDPRO=SCP/S
   IPTR    pa_DstHFE;         // HXC=HFE/S
   IPTR    pa_DstExtendedDSK; // EXTENDEDDSK=EDSK/S
   IPTR    pa_Quiet;          // QUIET/S
};

// Regular Amiga compliant version string
const UBYTE __attribute__((section(".text"))) ver_version[] = { "\0$VER: " PROGRAM_NAME " " PROGRAM_VERSION " (" PROGRAM_DATE ") " PROGRAM_COPYRIGHT "" };

#define QuietPrintf(quiet, format, ...) do { if(!(quiet)) { Printf((format), ##__VA_ARGS__); } } while(0)
#define SideSwitchesToEnum(a,b) (((a)&&(b))?IDisk::FACE_BOTH:((a)?IDisk::FACE_1:((b)?IDisk::FACE_2:IDisk::FACE_BOTH)))

int main(void)
{
   IDisk::FaceSelection srcSide;
   DiskGen diskGen;
   LONG rc = RETURN_OK;

   struct RDArgs *rdProgArgs = NULL;
   struct ProgArgs progArgs = { NULL, FALSE, FALSE, NULL, FALSE, FALSE, FALSE, FALSE, FALSE };

   if ((rdProgArgs = ReadArgs(PROG_ARG_TEMPLATE, (LONG *)&progArgs, NULL)) == NULL)
   {
      rc = IoErr();
      goto out;
   }

   // Manage default values
   // Note: pa_DstFileName is mandatory (/A) and already handled by ReadArgs()

   // Default destination name is source name (new extension will be appened automatically)
   if (progArgs.pa_DstFileName == NULL)
   {
      progArgs.pa_DstFileName = progArgs.pa_SrcFileName;
   }

   // Convert to IPF by default
   if (!progArgs.pa_DstIPF
      && !progArgs.pa_DstSCP
      && !progArgs.pa_DstHFE
      && !progArgs.pa_DstExtendedDSK)
   {
      progArgs.pa_DstIPF = TRUE;
   }

   // Note: handle both sides by default
   srcSide = SideSwitchesToEnum(progArgs.pa_SrcSideA, progArgs.pa_SrcSideB);

   QuietPrintf(progArgs.pa_Quiet, "Converting %s...\n", progArgs.pa_SrcFileName);

   // Load the disk
   if (diskGen.LoadDisk(progArgs.pa_SrcFileName) != 0)
   {
      rc = ERROR_OBJECT_WRONG_TYPE;
      goto out;
   }

   // Filter side
   switch (diskGen.FilterSide(srcSide))
   {
   case IDisk::FaceSelection::FACE_1:
   case IDisk::FaceSelection::FACE_2:
   case IDisk::FaceSelection::FACE_BOTH:
      break;
   default:
      rc = ERROR_SEEK_ERROR;
      goto out;
   }

   // Save with the correct format
   if (progArgs.pa_DstIPF)
   {
      FormatTypeIPF formatType;

      if (formatType.SaveDisk(progArgs.pa_DstFileName, diskGen.GetDisk()) == FormatType::OK)
      {
         QuietPrintf(progArgs.pa_Quiet, "IPF file `%s` saved.\n", diskGen.GetCurrentLoadedDisk());
      }
      else
      {
         QuietPrintf(progArgs.pa_Quiet, "IPF file `%s` could not be saved.\n", diskGen.GetCurrentLoadedDisk());
         rc = RETURN_WARN;
      }
   }
   if (progArgs.pa_DstSCP)
   {
      FormatTypeSCP formatType;

      if (formatType.SaveDisk(progArgs.pa_DstFileName, diskGen.GetDisk()) == FormatType::OK)
      {
         QuietPrintf(progArgs.pa_Quiet, "SCP file `%s` saved.\n", diskGen.GetCurrentLoadedDisk());
      }
      else
      {
         QuietPrintf(progArgs.pa_Quiet, "SCP file `%s` could not be saved.\n", diskGen.GetCurrentLoadedDisk());
         rc = RETURN_WARN;
        }
    }
   if (progArgs.pa_DstHFE)
   {
      FormatTypeHFE formatType;

      if (formatType.SaveDisk(progArgs.pa_DstFileName, diskGen.GetDisk()) == FormatType::OK)
      {
         QuietPrintf(progArgs.pa_Quiet, "HFE file `%s` saved.\n", diskGen.GetCurrentLoadedDisk());
      }
      else
      {
         QuietPrintf(progArgs.pa_Quiet, "HFE file `%s` could not be saved.\n", diskGen.GetCurrentLoadedDisk());
         rc = RETURN_WARN;
      }
   }
   if (progArgs.pa_DstExtendedDSK)
   {
      FormatTypeEDSK formatType;

      if (formatType.SaveDisk(progArgs.pa_DstFileName, diskGen.GetDisk()) == FormatType::OK)
      {
         QuietPrintf(progArgs.pa_Quiet, "Extended-DSK file `%s` saved.\n", diskGen.GetCurrentLoadedDisk());
      }
      else
      {
         QuietPrintf(progArgs.pa_Quiet, "Extended-DSK file `%s` could not be saved.\n", diskGen.GetCurrentLoadedDisk());
         rc = RETURN_WARN;
      }
   }

out:
   if (rc != RETURN_OK)
   {
      PrintFault(rc, PROGRAM_NAME);
   }

   // Free up everything!
   if (rdProgArgs)
   {
      FreeArgs(rdProgArgs);
   }

   return rc;
}

#else

#include "stdafx.h"
#include "DiskGen.h"
#include "DiskBuilder.h"
#include "FileAccess.h"


#ifdef __unix
#define stricmp strcasecmp
#define strnicmp strncasecmp
#endif // __unix

#include <string.h>
#include <algorithm>

static DiskBuilder disk_builder;
DiskGen disk_gen;
std::vector<FormatType*> out_format_list;
std::vector<FormatType*> in_format_list;

bool sub_dir = false;
FormatType* out_support;
IDisk::FaceSelection face_to_convert = IDisk::FACE_BOTH;
char filter[MAX_PATH] = {0};
char * second_side = nullptr;

int ConversionFile(fs::path& source, fs::path& destination, bool use_second_side)
{
   DiskBuilder disk_builder;
   int return_value = 0;

   ////////////////////////////////////
   // Destination
   if (destination.empty())
   {
      // take source, remove file spec
      destination = source;
      destination.replace_extension("");
   }

   printf("Loading %s", source.generic_string().c_str());

   FormatType* format;
   if (!disk_builder.CanLoad(source.string().c_str(), format) )
   {
      printf(" !! Error loading disk : Format unknown !!\n");
      return -1;
   }

   // Load the disk - todo : add progression 
   IDisk* new_disk = nullptr;
   if (disk_builder.LoadDisk(source.string().c_str(), new_disk, nullptr) != 0)
   {
      printf(" !! Error loading disk !!\n");
      return -1;
   }
   else
   {
      printf(" ok - ");
   }

   // Second side foring ?
   IDisk* new_disk_side_2 = nullptr;
   if (use_second_side && strlen(second_side) > 0)
   {
      if (!disk_builder.CanLoad(second_side, format))
      {
         printf(" !! Error loading disk 2 : Format unknown !!\n");
         return -1;
      }

      // Load the disk - todo : add progression 
      if (disk_builder.LoadDisk(second_side, new_disk_side_2, nullptr) != 0)
      {
         printf(" !! Error loading disk 2 !!\n");
         return -1;
      }
      else
      {
         printf(" ok - ");
      }
   }

   // raw ?
   if (strcmp(format->GetFormatName(), "KRYOFLUX") == 0)
   {
      return_value = 1;
   }

   // Filter side
   IDisk::FaceSelection pSide = new_disk->FilterSide(face_to_convert);
   switch (pSide)
   {
   case IDisk::FaceSelection::FACE_1: printf("Side 0 is kept - ");
      break;
   case IDisk::FaceSelection::FACE_2: printf("Side 1 is kept - ");
      break;
   case IDisk::FaceSelection::FACE_BOTH: printf("Both Sides are kept - ");
      break;
   default:
      printf(" !! Error selecting side\n");
      return -1;
   }

   // Combine the 2 dumps, if necessary
   if (new_disk_side_2 != nullptr)
   {
      // Combine both disk      
      new_disk->CombineWithDisk(new_disk_side_2);
      delete new_disk_side_2;
   }


   // Save with the correct format
   out_support->SaveDisk(destination.string().c_str(), new_disk);
   printf("Saving %s \n", new_disk->GetCurrentLoadedDisk());

   return return_value;
}

int ConversionDirectory(fs::path& source, fs::path& destination)
{
   printf("DIRECTORY => %s\n", source.string().c_str());

   // -r and subdirs ?
   for (auto& p : fs::directory_iterator(source))
   {
      if (fs::is_regular_file(p.status()))
      {
         // Filter ? - TODO
         if ( strlen(filter) == 0 || MatchTextWithWildcards(p.path().filename().string(), filter))
         {
            fs::path input_file = p.path();
            fs::path output_file = destination;
            output_file.replace_filename(p.path().filename());
            output_file.replace_extension("");

            if (ConversionFile(input_file, output_file, false) == 1)
            {
               //End of this directory
               return 0;
            }
         }
      }
   }

   // Now, works on subdirs
   if (sub_dir)
   {
      for (auto& p : fs::directory_iterator(source))
      {
         if (fs::is_directory(p.status()))
         {
            fs::path input_file = p.path();

            ConversionDirectory(input_file, destination);

            if (ConversionFile(input_file, destination, false) == 1)
            {
               //End of this directory
               return 0;
            }
         }
      }
   }

   return 0;
}


void PrintUsage()
{
   printf("Usage : SugarConvDsk source [destination] [-s=side] [-second=side_2_path][-o=outputformat] [-r] [-f=filter]\n");
   printf("    or  SugarConvDsk -cat=user [-sort] [-l] [-c] source    \n");
   printf("\n");
   printf("    -s=side : Select side of the disk to convert.\n");
   printf("       Side can be 1 or 2\n");
   printf("       If omitted, both side are written (if relevant for the format\n");
   printf("\n");
   printf("    -o=outputformat : Select output format. Can take the following values:\n");
   //
   for (auto it = out_format_list.begin(); it != out_format_list.end(); ++it)
   {
      printf("        %s : %s format\n", (*it)->GetFormatName(), (*it)->GetFormatDescriptor());
   }

   printf("    If this parameter is not used, default output format is IPF\n");
   printf("\n");
   printf("   -second=side_2_path : If source is a single dump, side 1 of side_2_path will be the 2nd side of the output (it will replace any side of the regular output)\n");
   printf("    source : The source file can be in the following format : \n");

   for (auto it = in_format_list.begin(); it != in_format_list.end(); ++it)
   {
      printf("        %s : %s format\n", (*it)->GetFormatName(), (*it)->GetFormatDescriptor());
   }
   printf(
      "        If the source file is a directory : In this case, every file int the given directory are converted to the 'output' directory\n");

   printf("\n");
   printf("    destination : output file. If source file is a directory, destination is used as an output directory\n");
   printf("\n");
   printf("    -r : If the source file is a directory, convert recursively the given directory.\n");
   printf("    -f=filter : If the source file is a directory, set a filter for the files to convert.\n");
   printf("    -cat=user : List the directory to standard output (no conversion is done if used). 'user' is used, unless ALLUSER is specified\n");
   printf("    -sort : This flag will wort the files in alphabetical order.\n");
   printf("    - l : Add flags H for hidden files, and R for read - only files.List isdisplayed with one file per line.\n");
   printf("    - c : Display file on the same line, separated with semicolumn. No result if used with -l option\n");
   printf("    - col=x,y : specift how to display result : x column, with y space between each column. No result if used with -l or -c option\n");
}


int main(int argc, char** argv)
{
   ////////////////////////////////////
   // Init the output formats available
   out_format_list = disk_builder.GetFormatsList(DiskBuilder::WRITE);
   in_format_list = disk_builder.GetFormatsList(DiskBuilder::READ);

   ////////////////////////////////////
   // Sanity check :
   bool arguments_are_ok = (argc > 1);

   char* source = nullptr;
   const char* destination = nullptr;

   // CATALOG
   bool cat = false;
   int user = 0;  // default user
   bool sort = false;
   bool full_list = false;
   bool compact;
   unsigned int col_number = 1;
   unsigned int col_intersize = 4;

   for (int i = 1; i < argc && arguments_are_ok; i++)
   {
      if (argv[i][0] == '-')
      {
         // Argument
         if (stricmp(argv[i], "-s=1") == 0)
         {
            face_to_convert = IDisk::FACE_1;
         }
         else if (stricmp(argv[i], "-s=2") == 0)
         {
            face_to_convert = IDisk::FACE_2;
         }
         else if (stricmp(argv[i], "-r") == 0)
         {
            sub_dir = true;
         }
         else if (strnicmp(argv[i], "-f=", 3) == 0)
         {
            sscanf(argv[i], "-f=%s", filter);
         }
         else if (strnicmp(argv[i], "-second=", 8) == 0)
         {
            second_side = argv[i] + strlen("-second=");
            //sscanf(argv[i], "-second=%s", second_side);
         }
         else if (strnicmp(argv[i], "-cat=", 5) == 0)
         {
            // cat
            cat = true;
            // Get user : 
            char user_str[256];
            sscanf(argv[i], "-cat=%s", user_str);
            // all user ?
            if (strcmp(user_str, "ALLUSERS")==0)
               user = -1;
            else
            {
               char* end_str;
               user = strtol(user_str, &end_str, 16);
            }
         }
         else if (stricmp(argv[i], "-sort") == 0)
         {
            sort = true;
         }
         else if (stricmp(argv[i], "-l") == 0)
         {
            full_list = true;
         }
         else if (strnicmp(argv[i], "-col=", 5) == 0)
         {
            sscanf(argv[i], "-col=%i,%i", &col_number, &col_intersize);
         }         
         else if (stricmp(argv[i], "-c") == 0)
         {
            compact = true;
         }
         // Chek outsupport with supported formats
         else if (strlen(argv[i]) > 3 && argv[i][0] == '-' && argv[i][1] == 'o' && argv[i][2] == '=')
         {
            //for (auto it = out_format_list.begin(); it != out_format_list.end(); it++)
            for (auto & it : out_format_list )
            {
               if (strcmp((it)->GetFormatName(), &argv[i][3]) == 0)
               {
                  // Found !
                  out_support = it;
                  break;
               }
            }
            if (out_support == nullptr)
            {
               arguments_are_ok = false;
            }
         }
         else
         {
            // Unknown option
            arguments_are_ok = false;
         }
      }
      else
      {
         // Source / destination
         if (source == nullptr)
         {
            source = argv[i];
         }
         else if (destination == nullptr)
         {
            destination = argv[i];
         }
         else
         {
            // Too much parameter !
            arguments_are_ok = false;
         }
      }
   }

   if (!arguments_are_ok)
   {
      PrintUsage();
      return -1;
   }

   // Specific : 'cat' support
   if (cat)
   {
      // Print catalog to stdout
      IDisk* new_disk = nullptr;
      if (disk_builder.LoadDisk(source, new_disk, nullptr) != 0)
      {
         printf(" !! Error loading disk !!\n");
         return -1;
      }
      IDisk::AutorunType autorun_type;
      std::vector<std::string> file_list = new_disk->GetCAT(autorun_type, user);

      fs::path p (source);
      
      if (user == -1)
      {
         printf("Catalog of %s for all users \n", p.filename().string().c_str());
      }
      else
      {
         printf("Catalog of %s for USER %X\n", p.filename().string().c_str(), user);
      }
      
      // Sort files ?
      if (sort)
      {
         std::sort (file_list.begin(), file_list.end() );
      }

      std::string output;
      int column_counter = 0;
      char buffer[256] = { 0 };
      for (auto&i : file_list)
      {
         bool hidden = (i[9] & 0x80) == 0x80;
         bool readonly = (i[8] & 0x80) == 0x80;
         i[8] &= 0x7F;
         i[9] &= 0x7F;
         i.insert(8, 1, '.');

         if (full_list)
         {
            sprintf(buffer, "%s %c %c\n", i.c_str(), hidden ? 'H' : '-', readonly ? 'R' : '-');
         }
         else
         {
            if (compact)
            {
               sprintf(buffer, "%s%c", i.c_str(), ';');
            }
            else
            { 
               // col and interlines
               column_counter++;
               if (column_counter == col_number)
               {
                  sprintf(buffer, "%s\n", i.c_str());
                  column_counter = 0;
               }
               else
               {
                  sprintf(buffer, "%s%*s", i.c_str(), col_intersize, "");
               }
            }
         }
         output += buffer;
      }
      printf(output.c_str());

      return 0;
   }
   else
   {
      if (out_support == nullptr)
      {
         if (!out_format_list.empty())
         {
            // take first one !
            out_support = out_format_list.at(0);
         }
         else
         {
            printf("*** ERROR : No output format available !\n");
            return -1;
         }
      }

      if (destination == nullptr)
      {
         destination = "";
      }

      fs::path source_path(source);
      fs::path destination_path(destination);

      if (fs::is_regular_file(fs::status(source)))
      {
         return ConversionFile(source_path, destination_path, (second_side != nullptr));
      }
      else if (fs::is_directory(fs::status(source)))
      {
         return ConversionDirectory(source_path, destination_path);
      }
   }
}

// Convert filename

#endif
