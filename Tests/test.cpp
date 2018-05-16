
#ifdef _WIN32

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#endif

#include <iostream>

#include "tchar.h"
#include "windows.h"
#include "gtest/gtest.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// LOADING TEST : These test check that loading will give same result from a file and from a buffer
///
/// - For a disk (with every supported format)
/// - For a tape (with every supported format)


/////////////////////////////////////////////////////////////
// Check that LoadDiskFromBuffer have the same generated disk than LoadDisk

TEST(Base, Dummy)
{
}

