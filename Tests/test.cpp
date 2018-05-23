
#ifdef _WIN32

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#endif

#include <iostream>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>

#include "tchar.h"
#include "windows.h"
#include "gtest/gtest.h"


namespace fs = std::experimental::filesystem;

const std::string out_dir = "out";
const std::string expected_dir = "Expected";
const std::string dump_dir = "Ressources//Dumps";

bool CompareFiles(const std::string& p1, const std::string& p2) {
	std::ifstream f1(p1, std::ifstream::binary | std::ifstream::ate);
	std::ifstream f2(p2, std::ifstream::binary | std::ifstream::ate);

	if (f1.fail() || f2.fail()) {
		return false; //file problem
	}

	if (f1.tellg() != f2.tellg()) {
		return false; //size mismatch
	}

	//seek back to beginning and use std::equal to compare contents
	f1.seekg(0, std::ifstream::beg);
	f2.seekg(0, std::ifstream::beg);
	return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
		std::istreambuf_iterator<char>(),
		std::istreambuf_iterator<char>(f2.rdbuf()));
}


/////////////////////////////////////////////////////////////
// Check that SugarConvDsk without any parameter display the usage text
TEST(SugarConvDsk, Display_Help)
{
	const fs::path out_file = "SugarConvHelp.txt";

	fs::path out_path = fs::path(out_dir) / out_file;
	fs::path expected_path = fs::path(expected_dir) / out_file;
	fs::remove(out_path);

	std::stringstream str;
	str << "SugarConvDsk >> " << out_path.string();

	system( str.str().c_str());

	ASSERT_EQ(true, CompareFiles(out_path.string(), expected_path.string())) << "Usage is not what's expected !";
}

/////////////////////////////////////////////////////////////
// Check conversion : Dsk to IPF
TEST(SugarConvDsk, Dsk_IPF)
{
	const fs::path in_file = fs::path(dump_dir)/ "After Burner (UK) (1988) (UK retail version) (CPM) [Original] (Weak Sectors).dsk";


}

