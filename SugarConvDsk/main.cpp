//////////////////////////////////////////////////////////
// SugarConvDsk
//
// Convert disk format supported by Sugarbox
//
// (c) T.Guillemin 2016 -
//
//
//////////////////////////////////////////////////////////
#include "DiskBuilder.h"

static DiskBuilder disk_builder;
std::vector<FormatType*> out_format_list;
std::vector<FormatType*> in_format_list;

#ifdef __MORPHOS__

#include "stdafx.h"
#include "DiskGen.h"

#include <proto/dos.h>

#define __TEXTSEGMENT__ __attribute__((section(".text")))
#define __UNUSED__ __attribute__ ((__unused__))

#define PROGRAM_NAME      "SugarConvDsk"
#define PROGRAM_VERSION   "0.1"
#define PROGRAM_DATE      "18/02/2018"
#define PROGRAM_COPYRIGHT "© 2018 Thomas Guillemin, MorphOS port by Philippe Rimauro"
#define PROG_ARG_TEMPLATE "FROM/A,TO,IPF/S,SCP/S,HFE/S,DSK/S,SIDEA/S,SIDEB/S,QUIET/S"

struct ProgArgs
{
    STRPTR  pa_SrcFileName;
    STRPTR  pa_DstFileName;
    IPTR    pa_DstIPF;
    IPTR    pa_DstSCP;
    IPTR    pa_DstHFE;
    IPTR    pa_DstDSK;
    IPTR    pa_SrcSideA;
    IPTR    pa_SrcSideB;
    IPTR    pa_Quiet;
};

const UBYTE __TEXTSEGMENT__ ver_version[] = { "\0$VER: " PROGRAM_NAME " " PROGRAM_VERSION " (" PROGRAM_DATE ") " PROGRAM_COPYRIGHT "" };
const ULONG __abox__ = 1;

#define QuietPrintf(quiet, format, ...) do { if(!(quiet)) { Printf((format), ##__VA_ARGS__); } } while(0)
#define SideSwitchesToEnum(a,b) (((a)&&(b))?IDisk::FACE_BOTH:((a)?IDisk::FACE_1:((b)?IDisk::FACE_2:IDisk::FACE_BOTH)))

int main(int argc, char *argv[])
{
    IDisk::tFaceSelection srcSide;
    IDisk::tFormatSupport dstFormat;
    CDiskGen diskGen;
    LONG rc = RETURN_OK;

    struct RDArgs *rdProgArgs = NULL;
    struct ProgArgs progArgs = { 0 };

    if((rdProgArgs = ReadArgs(PROG_ARG_TEMPLATE, (LONG *)&progArgs, NULL)) == NULL)
    {
        PrintFault(IoErr(), PROGRAM_NAME);
        rc = RETURN_ERROR;
        goto out;
    }

    // Manage default values
// Note: pa_DstFileName already handled by ReadArgs (mandatory /A)

    // Default destination name is source name (new extension will be appened automatically)
    if(progArgs.pa_DstFileName == NULL)
    {
        progArgs.pa_DstFileName = progArgs.pa_SrcFileName;
    }

    // Convert to IPF by default
    if(!progArgs.pa_DstIPF && !progArgs.pa_DstSCP && !progArgs.pa_DstHFE && !progArgs.pa_DstDSK)
    {
        progArgs.pa_DstIPF = TRUE;
    }

    // Note: handle both sides by default
    srcSide = SideSwitchesToEnum(progArgs.pa_SrcSideA, progArgs.pa_SrcSideB);

    QuietPrintf(progArgs.pa_Quiet, "Converting %s...\n", progArgs.pa_SrcFileName);

    // Load the disk
    if(diskGen.LoadDisk(progArgs.pa_SrcFileName) != 0)
    {
        PrintFault(ERROR_OBJECT_WRONG_TYPE, PROGRAM_NAME);
        rc = RETURN_FAIL;
        goto out;
    }

    // Filter side
    switch(diskGen.FilterSide(srcSide))
    {
    case IDisk::tFaceSelection::FACE_1:
    case IDisk::tFaceSelection::FACE_2:
    case IDisk::tFaceSelection::FACE_BOTH:
        break;
    default:
        PrintFault(ERROR_SEEK_ERROR, PROGRAM_NAME);
        rc = RETURN_ERROR;
        goto out;
    }

    // Save with the correct format
    if(progArgs.pa_DstIPF)
    {
        diskGen.WriteDisk(progArgs.pa_DstFileName, IDisk::IPF);
        QuietPrintf(progArgs.pa_Quiet, "IPF file saved.\n", diskGen.GetCurrentLoadedDisk());
    }
    if(progArgs.pa_DstSCP)
    {
        diskGen.WriteDisk(progArgs.pa_DstFileName, IDisk::SCP);
        QuietPrintf(progArgs.pa_Quiet, "SCP file saved.\n", diskGen.GetCurrentLoadedDisk());
    }
    if(progArgs.pa_DstHFE)
    {
        diskGen.WriteDisk(progArgs.pa_DstFileName, IDisk::HFE);
        QuietPrintf(progArgs.pa_Quiet, "HFE file saved.\n", diskGen.GetCurrentLoadedDisk());
    }
    if(progArgs.pa_DstDSK)
    {
        diskGen.WriteDisk(progArgs.pa_DstFileName, IDisk::DSK);
        QuietPrintf(progArgs.pa_Quiet, "DSK file saved.\n", diskGen.GetCurrentLoadedDisk());
    }

out:
    // Free up everything!
    if(rdProgArgs)
    {
        FreeArgs(rdProgArgs);
    }

    return rc;
}

#else

#include "stdafx.h"
#include "DiskGen.h"
#include "FileAccess.h"

#ifdef __unix
#define stricmp strcasecmp
#define strnicmp strncasecmp
#endif // __unix

#include <string.h>

bool sub_dir = false;
FormatType* out_support;
IDisk::FaceSelection face_to_convert = IDisk::FACE_BOTH;
DiskGen disk_gen;
char filter[MAX_PATH] = {0};

int ConversionFile(fs::path& source, fs::path& destination)
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
   if (!disk_builder.CanLoad(source.generic_string().c_str(), format) )
   {
      printf(" !! Error loading disk : Format unknown !!\n");
      return -1;
   }

   // Load the disk
   if (disk_gen.LoadDisk(source.generic_string().c_str() /*pSrc */) != 0)
   {
      printf(" !! Error loading disk !!\n");

      return -1;
   }
   else
   {
      printf(" ok - ");
   }

   // raw ?

   if (strcmp(format->GetFormatName(), "KRYOFLUX") == 0)
   {
      return_value = 1;
   }
   // Filter side
   IDisk::FaceSelection pSide = disk_gen.FilterSide(face_to_convert);
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

   // Save with the correct format
   out_support->SaveDisk(destination.generic_string().c_str(), disk_gen.GetDisk());
   printf("Saving %s \n", disk_gen.GetCurrentLoadedDisk());

   return return_value;
}

int ConversionDirectory(fs::path& source, fs::path& destination)
{
   printf("DIRECTORY => %s\n", source.generic_string().c_str());

   // -r and subdirs ?
   for (auto& p : fs::directory_iterator(source))
   {
      if (fs::is_regular_file(p.status()))
      {
         // Filter ? - TODO
         if ( strlen(filter) == 0 || MatchTextWithWildcards(p.path().filename().generic_string(), filter))
         {
            fs::path input_file = p.path();
            fs::path output_file = destination;
            output_file.replace_filename(p.path().filename());
            output_file.replace_extension("");

            if (ConversionFile(input_file, output_file) == 1)
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

            if (ConversionFile(input_file, destination) == 1)
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
   printf("Usage : SugarConvDsk source [destination] [-s=side] [-o=outputformat] [-r] [-f=filter]\n");
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
      return ConversionFile(source_path, destination_path);
   }
   else if (fs::is_directory(fs::status(source)))
   {
      return ConversionDirectory(source_path, destination_path);
   }
}

// Convert filename

#endif
