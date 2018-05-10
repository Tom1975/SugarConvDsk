# SugarConvDsk

[![Build Status](https://travis-ci.com/Tom1975/SugarConvDsk.svg?branch=master)](https://travis-ci.com/Tom1975/SugarConvDsk)
[![Build status](https://ci.appveyor.com/api/projects/status/oq746hdj4wrn8rod?svg=true)](https://ci.appveyor.com/project/Tom1975/sugarconvdsk)

Convert any Amstrad CPC dump file format to other format

Formats readable are :
- DSK
- EDSK
- Kryoflux RAW
- SuperCard Pro (SCP)
- IPF
- CTRaw
- HFE

Format writtable are : 
- EDSK (but not really reliable)
- HFE
- IPF 
- SCP

This tool mainly convert any format into an internal format, then write it back to the specified one.
Internal format is bases on MFM, with some specific attributes for every bit, which can be weak or optional.
The internal format is also not revolution-dependant : If a dump use more than one revolution, to keep track of weak sector for example, a specific algorithm will rebuild a single track from these revolutions, adding the attributes needed to describe everything. Also, an error correction is done, to avoid being fooled by reading error.