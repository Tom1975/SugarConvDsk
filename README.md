
# SugarConvDsk

[![Build Status](https://travis-ci.com/Tom1975/SugarConvDsk.svg?branch=master)](https://travis-ci.com/Tom1975/SugarConvDsk)
[![Build status](https://ci.appveyor.com/api/projects/status/oq746hdj4wrn8rod?svg=true)](https://ci.appveyor.com/project/Tom1975/sugarconvdsk)

Convert any Amstrad CPC dump file format to other format

Usage
=====
Usage : SugarConvDsk source [destination] [-s=side] [-o=outputformat] [-r] [-f=filter]

    -s=side : Select side of the disk to convert.
       Side can be 1 or 2
       If omitted, both side are written (if relevant for the format)

    -o=outputformat : Select output format. Can take the following values:
        EDSK : Extended Dsk format
        HFE : HFE format
        IPF : IPF format
        SCP : Supercard Pro format
    If this parameter is not used, default output format is EDSK

    source : The source file can be in the following format :
        CTRAW : CT-RAW format
        DSK : Dsk format
        EDSK : Extended Dsk format
        HFE : HFE format
        IPF : IPF format
        SCP : Supercard Pro format
        KRYOFLUX : Kryoflux RAW file format
		
        If the source file is a directory : In this case, every files in the given directory are converted to the 'output' directory

    destination : output file. If source file is a directory, destination is used as an output directory

    -r : If the source file is a directory, convert recursively the given directory.
    -f=filter : If the source file is a directory, set a filter for the files to convert.
	
	Example : SugarConvDsk d:\dump d:\result -o=IPF -r -f=[CPC]*.dsk 
	This will convert to IPF every file of the form "[CPC]*.dsk", from directory d:\result, and subdirectories.
	Result files will be saved in the d:\result directory.

This tool mainly convert any format into an internal format, then write it back to the specified one.
Internal format is bases on MFM, with some specific attributes for every bit, which can be weak or optional.
The internal format is also not revolution-dependant : If a dump use more than one revolution, to keep track of weak sector for example, a specific algorithm will rebuild a single track from these revolutions, adding the attributes needed to describe everything. Also, an error correction is done, to avoid being fooled by reading error.

Compilation
=
This tool can be compiled on various system.
Known working targets are :
 - Win32
 - Ubuntu

The tool use CMake as makefile generator, and ZLib as an external library.
Also, a modern compiler is required as it use some C++17 features (through experimental/filesystem for gcc7.2 and Visual 2017 15.6).

The code is a result of various and extended experimentation, so it may looks like a bit messy (as I tried to make it works before making it right)
