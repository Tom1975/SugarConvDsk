//////////////////////////////////////////////////////////
// SugarConvTape
//
// Convert disk format supported by Sugarbox
//
// (c) T.Guillemin 2016 - 
//
//
//////////////////////////////////////////////////////////

#include "windows.h"
#include "tchar.h"
#include "stdio.h"
#include "Shlwapi.h"
#include <strsafe.h>
#include "DiskGen.h"

bool bSuDir = false;
IDisk::tFormatSupport outSupport = IDisk::IPF;
IDisk::tFaceSelection bFaceToConvert = IDisk::FACE_BOTH;
CDiskGen diskGen;
char pFilter[MAX_PATH] = { 0 };

int ConversionFile (char * pSource, char * pDestination)
{
   int retValue = 0;
   char destination[MAX_PATH] = { 0 };
   ////////////////////////////////////
   // Conversion
   


   ////////////////////////////////////
   // Destination
   if (pDestination == nullptr)
   {
      // take source, remove file spec
      strcpy_s(destination, 260, pSource);
      ::PathRemoveExtensionA(destination);
      pDestination = destination;
   }

   char *pSrc, *pDest;
   int tstrLen;
   pDest = pDestination;
   pSrc = pSource;
   pSource = strdup(pSource);
   tstrLen = strlen(pDest);
   printf("Loading %s", pSource);
   // Load the disk
   if ( diskGen.LoadDisk( pSrc ) != 0)
   {
      printf (" !! Error loading disk !!\n");

      return -1;
   }
   else
   {
      printf (" ok - ");
   }

   // raw ?
   if (diskGen.GetDisk()->GetFormat() == IDisk::RAW)
   {
      retValue = 1;
   }
   // Filter side
   IDisk::tFaceSelection pSide = diskGen.FilterSide (bFaceToConvert);
   switch (pSide)
   {
   case IDisk::tFaceSelection::FACE_1 : printf ("Side 0 is kept - ");break;
   case IDisk::tFaceSelection::FACE_2 : printf ("Side 1 is kept - ");break;
   case IDisk::tFaceSelection::FACE_BOTH : printf ("Both Sides are kept - ");break;
   default:
      printf (" !! Error selecting side\n");

      return -1;
      break;
   }

   // Save with the correct format
   
   diskGen.WriteDisk (pDest, outSupport);
   printf("Saving %S \n", diskGen.GetCurrentLoadedDisk());

   // use tstrTo up to tstrLen characters as needed...

   return retValue;
}

int ConversionDirectory (char * pSource, char * pDestination)
{
   
   printf("DIRECTORY => %s\n", pSource);

   // Run throug the directory, then 
   WIN32_FIND_DATAA ffd;
   HANDLE hFind;
   char szDir[MAX_PATH];
   char szSource[MAX_PATH];

   StringCchCopyA(szDir, MAX_PATH, pSource);

   if (strlen(pFilter) == 0)
      StringCchCatA(szDir, MAX_PATH, "\\*");
   else
   {
      StringCchCatA(szDir, MAX_PATH, "\\");
      StringCchCatA(szDir, MAX_PATH, pFilter);
   }

   // -r and subdirs ?
   hFind = FindFirstFileA(szDir, &ffd);
   
   if (INVALID_HANDLE_VALUE == hFind) 
   {
      return -1;
   } 
   do
   {
      if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
         if (strcmp ( ".", ffd.cFileName) == 0 
            || strcmp("..", ffd.cFileName) == 0)
         {
            continue;
         }
         /*if (bSuDir)
         {
            strcpy(szSource, pSource);
            strcat(szSource, "\\");
            strcat(szSource, ffd.cFileName);

            ConversionDirectory (szSource, pDestination );
         }*/
      }
      else
      {
         // Remove extension
         char outputFile[MAX_PATH];
         if (pDestination != NULL)
            strcpy_s ( outputFile, 260, pDestination );
         else
            strcpy_s ( outputFile, 260, ".");
         strcat_s(outputFile, 260, "\\");
         strcat_s ( outputFile, 260, ffd.cFileName );
         ::PathRemoveExtensionA (outputFile);

         strcpy_s(szSource, 260, pSource);
         strcat_s(szSource, 260, "\\");
         strcat_s(szSource, 260, ffd.cFileName);

         if (ConversionFile(szSource, outputFile) == 1)
         {
            //End of this directory
            return 0;
         }

      }
   }
   while (FindNextFileA(hFind, &ffd) != 0);

   if (bSuDir)
   {
      StringCchCopyA(szDir, MAX_PATH, pSource);
      StringCchCatA(szDir, MAX_PATH, "\\*");
      hFind = FindFirstFileA(szDir, &ffd);

      if (INVALID_HANDLE_VALUE != hFind)
      do
      {
         if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
         {
            if (strcmp(".", ffd.cFileName) == 0
               || strcmp("..", ffd.cFileName) == 0)
            {
               continue;
            }
            strcpy_s(szSource, 260, pSource);
            strcat_s(szSource, 260, "\\");
            strcat_s(szSource, 260, ffd.cFileName);

            ConversionDirectory (szSource, pDestination );
         }
      } while (FindNextFileA(hFind, &ffd) != 0);

   }

   return 0;
}


void printUsage()
{
   printf("Usage : SugarConvDsk source [destination] [-s=side] [-o=outputformat] [-r] [-f=filter]\n");
   printf("\n");
   printf("    -s=side : Select side of the disk to convert.\n");
   printf("       Side can be 1 or 2\n");
   printf("       If omitted, both side are written (if relevant for the format\n");
   printf("\n");
   printf("    -o=outputformat : Select output format. Can take the following values:\n");
   printf("        dsk : edsk format\n");
   printf("        hfe : HFE format\n");
   printf("        ipf : IPF format\n");
   printf("        scp : SuperCard pro format\n");
   printf("    If this parameter is not used, default output format is IPF\n");
   printf("\n");
   printf("    source : The source file can be in the following format : \n");
   printf("        Directory : In this case, every file int the given directory are converted to the 'output' directory\n");
   printf("        SCP       : Supercard Pro\n");
   printf("        DSK/EDSK  : dsk and extended-dsk\n");
   printf("        CT-Raw    : SPS CT-raw format (usually *.raw or *.ctr)\n");
   printf("        HFE       : HFE Format used for the HXC or Gotek\n");
   printf("        IPF       :  Interchangeable Preservation Format from SPS\n");
   printf("        RAW       :  Kryoflux RAW format\n");
   printf("\n");
   printf("    destination : output file. If source file is a directory, destination is used as an output directory\n");
   printf("\n");
   printf("    -r : If the source file is a directory, convert recursively the given directory.\n");
   printf("    -f=filter : If the source file is a directory, set a filter for the files to convert.\n");

}


int main(int argc, char** argv)
{
   int retValue = 0;
   ////////////////////////////////////
   // Sanity check :
   bool bCorrect = (argc > 1);

   char* pSource = nullptr;
   char* pDestination = nullptr;


   for (int i = 1; i < argc && bCorrect ; i++)
   {
      if (argv[i][0] == '-')
      {
         // Argument 
         if (stricmp(argv[i], "-s=1") == 0)
         {
            bFaceToConvert = IDisk::FACE_1;
         }
         else if (stricmp(argv[i], "-s=2") == 0)
         {
            bFaceToConvert = IDisk::FACE_2;
         }
         else if (stricmp(argv[i], "-r") == 0)
         {
            bSuDir = true;
         }
         else if (strnicmp(argv[i], "-f=", 3) == 0)
         {
            sscanf(argv[i], "-f=%s", pFilter);
            
         }
         else if (stricmp(argv[i], "-o=dsk") == 0) outSupport = IDisk::DSK;
         else if (stricmp(argv[i], "-o=scp") == 0) outSupport = IDisk::SCP;
         else if (stricmp(argv[i], "-o=hfe") == 0) outSupport = IDisk::HFE;
         else if (stricmp(argv[i], "-o=ipf") == 0) outSupport = IDisk::IPF;
         else
         {
            // Unknown option 
            bCorrect = false;
         }
      }
      else
      {
         // Source / destination
         if (pSource == NULL)
         {
            pSource = argv[i];
         }
         else if (pDestination == NULL)
         {
            pDestination = argv[i];
         }
         else
         {
            // Too much parameter !
            bCorrect = false;
         }
      }
   }

   if (!bCorrect)
   {
      printUsage();
      return -1;
   }
   /*if (pDestination == nullptr)
   {
      pDestination = ".\\";
   }*/
   
   WIN32_FILE_ATTRIBUTE_DATA info;
   GetFileAttributesExA(pSource, GetFileExInfoStandard, &info);

   if (( info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
   {
      return ConversionDirectory (pSource, pDestination);
   }
   else
   {
      return ConversionFile (pSource, pDestination);
   }
}

// Convert filename

