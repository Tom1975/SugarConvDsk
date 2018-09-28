
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

#include "gtest/gtest.h"

#include "DiskBuilder.h"
#include "IDisk.h"
#include "FileAccess.h"

namespace fs = std::experimental::filesystem;

const fs::path out_dir = "out";
const fs::path expected_dir = "Expected";
const fs::path dump_dir = "Ressources//Dumps";

/////////////////////////////////////////////////////////////
// Helper functions
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

bool CompareConversion (const char* in_file, const char* out_file, const char* conversion_options, std::string& error_string)
{
	fs::remove( fs::path(out_file));
	std::stringstream str;
	str << "SugarConvDsk \"" << in_file << "\" "<< conversion_options << " \"" << out_file << "\"";
	
	if (system(str.str().c_str()) != 0)
	{
		error_string = "SugarConvDsk return an error";
		return false;
	}

	// Check generated file
	DiskBuilder disk_builder;
	IDisk * d1, *d2;
	FormatType *type;

	if (!disk_builder.CanLoad(in_file, type))
	{
		error_string = "Cannot load input file";
		return false;
	}

	if (disk_builder.LoadDisk(in_file, d1) != 0)
	{
		error_string = "Error while loading input file";
		return false;
	}
	if (disk_builder.LoadDisk(out_file, d2) != 0)
	{
		error_string = "Error while loading output file";
		return false;
	}
	if (d1->CompareToDisk(d2, false) != 0)
	{
		error_string = "Input and Output are not equivalent";
		return false;
	}
	delete d1;
	delete d2;
	return true;
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
// Check conversion : Whole to IPF
TEST(SugarConvDsk, Conversion_2IPF)
{
	bool test_ok = true;
	std::vector<std::string> file_list;
	std::stringstream str;
	std::string error_string;

	DiskBuilder disk_builder;
	std::vector<FormatType*> list_formats = disk_builder.GetFormatsList(DiskBuilder::WRITE);

	GetDirectoryContent(dump_dir.string().c_str(), file_list);
	
	const char* ext = "IPF";
	for (auto&it : file_list)
	{
		const std::string filename_to_test = fs::path(it).filename().string();
		const std::string out_filename = filename_to_test + "." + ext;

		const fs::path in_file = fs::path(dump_dir) / filename_to_test;
		const fs::path out_file = fs::path(out_dir) / out_filename;

		std::string options = std::string("-o=") + ext;

		if (!CompareConversion(in_file.string().c_str(), out_file.string().c_str(), options.c_str(), error_string))
		{
			test_ok = false;
			str << "Error : " << in_file.string() << " -> "<< ext <<" : "<< error_string << "\n";
		}
	}
	ASSERT_TRUE(test_ok) << str.str();
}

TEST(SugarConvDsk, DSK_HFE)
{
	const std::string filename_to_test = "Arkanoid - Revenge of Doh (1988)(Imagine).dsk";
	const fs::path in_file = fs::path(dump_dir) / filename_to_test;
	std::string out_file = filename_to_test + ".HFE";
	const fs::path out_path = fs::path(out_dir) / out_file;
	
	std::string error_string;
	if (!CompareConversion(in_file.string().c_str(), out_path.string().c_str(), "-o=HFE", error_string))
	{
		FAIL() << error_string;
	}
}

// Check conversion : EDsk to HFE
TEST(SugarConvDsk, EDsk_HFE)
{
	const std::string filename_to_test = "Barbarian (1987)(Palace Software)(Disk 1 of 2)[a].dsk";
	const fs::path in_file = fs::path(dump_dir) / filename_to_test;
	std::string out_file = filename_to_test + ".HFE";
	const fs::path out_path = fs::path(out_dir) / out_file;

	std::string error_string;
	if (!CompareConversion(in_file.string().c_str(), out_path.string().c_str(), "-o=HFE", error_string))
	{
		FAIL() << error_string;
	}
}


