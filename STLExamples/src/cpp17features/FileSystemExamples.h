#pragma once
#include "STLpch.h"

#include <filesystem>
#include <fstream>
#include <regex>

inline void FileSystem_Basics(/*int argc, char* argv[]*/)
{
	std::printf("\n-------------FileSystem C++17-------------\n");
	/* if used from VS use this; if used with command line comment these 2 line and uncomment function inpuntsde*/
	int argc = 2;

	//const char* argv = "src/testfolder";
	//const char* argv = "src/STLtest.cpp";
	const char* argv = ".";
	/* if no path / directory entered warn!!*; this is if used with command line !!*/
	if (argc < 2)
	{
		fmt::print("Please enter a path; Usage: {} <path> \n", argv[0]);
		return;
	}

	std::filesystem::path p{argv}; // p is the file path given
	if (is_regular_file(p))
	{
		fmt::print("{0} exists with {1} bytes\n",p, file_size(p));
	}

	else if (is_directory(p))
	{
		fmt::print("{0} is a directory containing;\n", p);
		for (const auto& e : std::filesystem::directory_iterator{ p })
		{
			/* e.path() is normally neccessary but if used for output Windows put extra slashes
				then .string() will work but more expensive!
			*/
			fmt::print("----{} \n", e.path().string());
		
		}
	}
	else if (exists(p))
	{
		fmt::print("{0} is a special file\n", p.string());
	}
	else
	{
		fmt::print("path does not exist {}!\n", p.string());
	}

}

inline void CheckPath_SwitchFile(/*int argc, char* argv[]*/)
{
	std::printf("\n-----CheckPath SwitchFile-----\n");
	/* if used from VS use this; if used with command line comment these 2 line and uncomment function inpuntsde*/
	int argc = 2;


	//const char* argv = "src/testfolder";
	//onst char* argv = "src/STLtest.cpp";
	const char* argv = ".";
		/* if no path / directory entered warn!!*; this is if used with command line !!*/
		if (argc < 2)
		{
			fmt::print("Please enter a path; Usage: {} <path> \n", argv[0]);
			return;
		}


		switch (std::filesystem::path p{ argv }; status(p).type())
		{
			case std::filesystem::file_type::not_found:
			{
				/* for windows not use see "\\" in the output use p.string()*/
				fmt::print("path does not exist!: {}\n", p);
				return;
			}

			case std::filesystem::file_type::regular:
			{
				fmt::print("{0} exists with {1} bytes\n", p, file_size(p));
				return;
			}

			case std::filesystem::file_type::directory:
			{
				fmt::print("{0} is a directory containing;\n", p);
				for (const auto& e : std::filesystem::directory_iterator{ p })
				{
					fmt::print("---- {0} \n", e.path().string());
				}
				return;
			}
		}
		fmt::print("path is a special file {}!\n", argv);
}

inline void CreateDirectory_Files()
{
	
	std::filesystem::path testDir{ "temp/test"};
	/* std::filesystem:: not used before the function it uses ADL(argument dependent lookup) to find the function
	   the type of testDir is a "class path" defined under std::filesystem::path; if it creates problems on other platforms use std::filesystem::
	*/
	/* there is al create_directory to create one directory if it is subdirectory the root specified must exist*/
	create_directories(testDir);
	
	/* create file in the created directories */
	/* the type of testFile is class path*/
	auto testFile = testDir / "data.txt";
	std::ofstream dataFile{ testFile };
	if (!dataFile)
	{
		fmt::print("Cannot open {}", testFile.string());
		return;
	}


	dataFile << "The answer is some great number 4256!\n";

#if !_MSC_VER
	try {
	/* this create runtime error since Windows requires admin access */
	/* create a symbolic link from tmp/slink to tmp/test*/
	std::filesystem::create_directory_symlink("test", testDir.parent_path() / "slink");
	/* Alternative t above but the one is better in general!!*/
	//create_symlink("test", testDir.parent_path() / "slink");
	}
	catch (const std::filesystem::filesystem_error& e)
	{
		fmt::print("error: {0}\n ---- path1: \"{1}\"\n", e.what(), e.path1().string() );
	}

#endif

	fmt::print("{0}\n", std::filesystem::current_path().string());

	for (const auto& e : std::filesystem::recursive_directory_iterator("."))
	{
		fmt::print("{0}\n", e.path().lexically_normal().string());
		//fmt::print("{0}\n", e.path().filename().string());
	}

	dataFile.close();
	//std::filesystem::remove_all(testDir);
	
}

[[nodiscard]] std::string GetFileContents(const std::filesystem::path& filePath)
{
	std::ifstream inFile{ filePath, std::ios::in | std::ios::binary };

	if (!inFile)
	{
		//fmt::print("Cannot open: {0}", filePath.string());
		return { "Cannot open " + filePath.string()};
	}
	
	const auto fSize = std::filesystem::file_size(filePath);

	if (fSize > std::numeric_limits<std::size_t>::max())
	{
		//fmt::print("Cannot open: {0}", filePath.string());
		return { "File too big to fit in size_t" + filePath.string() };
	}

	std::string str(static_cast<std::size_t>(fSize), 0);

	inFile.read(str.data(), str.size());

	if (!inFile)
	{
		return {"Cannot read all the data from: " + filePath.string()};
	}

	return str;
}


inline void ReadFileContents_FromPath()
{
	std::printf("\n-----ReadFileContents_FromPath-----\n");

	
	const auto str = GetFileContents("temp/test/data.txt");

	fmt::print("File size: {0} and the contents are;\n{1}", str.size(), str);
	std::shared_ptr<int> shrPtr;
}


struct FileEntry
{
	std::filesystem::path mPath;
	uintmax_t mSize{ 0 };

	static FileEntry Create(const std::filesystem::path& filePath)
	{
		return FileEntry{ filePath, std::filesystem::file_size(filePath) };
	}

	friend bool operator <(const FileEntry& a, const FileEntry& b) noexcept
	{
		return a.mSize < b.mSize;
	}
};


std::vector<FileEntry> CollectFiles(const std::filesystem::path& inPath)
{
	namespace fs = std::filesystem;
	
	/* Optimized Version to code below*/
	std::vector<FileEntry> files;
	if (fs::exists(inPath) && fs::is_directory(inPath))
	{
		for (const auto& entry : fs::recursive_directory_iterator{ inPath })
		{
			if (entry.is_regular_file())
			{
				files.emplace_back(entry, entry.file_size());
			}
		}
		//std::vector<std::filesystem::path> paths;
		//std::filesystem::recursive_directory_iterator dirpos{ inPath };
		//std::copy_if(begin(dirpos), end(dirpos), std::back_inserter(paths),
		//	[](const fs::directory_entry& entry) { return entry.is_regular_file(); }
		//);
	}

	//std::vector<FileEntry> files{ paths.size() };
	//std::transform(paths.cbegin(), paths.cend(), files.begin(), FileEntry::Create);
	return files;
}

inline void FilterFilesUsingRegex()
{
	std::printf("\n-----FilterFiles UsingRegex-----\n");

	const std::filesystem::path pathToshow{ "." };
	const std::regex reg{ ".*txt" };
	
	auto files = CollectFiles(pathToshow);

	std::sort(files.begin(), files.end());

	for (auto& entry : files)
	{
		const auto strFileName = entry.mPath.filename().string();
		if (std::regex_match(strFileName, reg))
		{
			fmt::print("filename: {0}, size: {1}\n", strFileName, entry.mSize);
		}
	}
}

