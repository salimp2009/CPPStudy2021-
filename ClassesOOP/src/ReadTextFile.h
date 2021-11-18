#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "spdlog/fmt/fmt.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"


inline void ReadFromFile()
{
	constexpr auto filename = "src/test.txt";
	std::ifstream inFile;
	inFile.open(filename);

	if (!inFile)
	{
		std::cerr << "File Not Found!!\n";
		return;
	}

	//std::cout <<"Filename: "<< filename <<", Is File Open: "<<inFile.is_open()<< '\n';
	fmt::print("Filename: {0}, is File Open: {1}\n ", filename, inFile.is_open());

	char line[10];


	while (!inFile.eof())
	{
		inFile >> line;
		fmt::print("File content: {}\n", line);
	}

	
	inFile.close();
}

inline void ReadFromFile_Poem()
{
	constexpr auto filename = "src/poem.txt";
	std::ifstream inFile;
	inFile.open(filename);

	if (!inFile)
	{
		std::cerr << "File Not Found!!\n";
		return;
	}

	fmt::print("Filename: {0}, is File Open: {1}\n", filename, inFile.is_open());

	std::string line;

	std::printf("\nReading with .getline()\n\n");

	while (std::getline(inFile, line))
	{
		fmt::print("{}\n", line);
	}

	/* since we reached the end of file cleaer .eof; end of file flag and go back to beginning*/
	inFile.clear();
	inFile.seekg(0);

	std::printf("\nReading with .get()\n\n");

	char readChar;
	while (inFile.get(readChar))
	{
		std::printf("%c", readChar);
	}

	std::printf("\n");

	inFile.close();
}

int processResponse(std::string_view response, std::string_view key)
{
	int score{ 0 };

	/* search will stop if all the elements of the subrange dont match therefore we return true all the time 
	  from the lambda so it can the rest of elements in the response if it matchs in the same place in key
	  so every time it is a match we increase the score; a hacky way to search algorithm but it works
	  Alternative is to use classic for-loop
	*/
	auto pos = std::search(response.begin(), response.end(), key.begin(), key.end(), [&score](auto&& resp, auto&& key)
		{
			if (resp == key) ++score;
			return true;
		});

	return score;
}

inline void StudentAnswers_Check()
{
	constexpr auto filename = "src/StudentAnswers.txt";
	std::ifstream inFile;
	inFile.open(filename);

	if (!inFile)
	{
		std::cerr << "File Not Found!!\n";
		return;
	}

	std::string answer;
	inFile >> answer;
	fmt::print("Correct answer: {}\n\nStudent Answers:\n", answer);

	std::string name;
	std::string response;
	std::size_t totalStudents{ 0 };
	std::uint32_t totalSum{ 0 };
	while (inFile>>name>>response)
	{
		++totalStudents;
		int score = processResponse(response, answer);
		totalSum += score;
		fmt::print("name: {0}, score: {1}, response: {2}\n", name, score, response);
	}	

	double averageScore{ 0.0 };
	if (totalStudents != 0)
	{
		averageScore = static_cast<double>(totalSum) / totalStudents;
	}

	fmt::print("avverage score: {0}, total students: {1}, totalSum: {2}\n", averageScore, totalStudents, totalSum);

}


