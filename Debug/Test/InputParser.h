#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

namespace clparser
{
	class inputParser
	{
	public:
		inputParser();
		~inputParser();
		using filePatterns = vector < string >;
		using filepatrnIter = filePatterns::iterator;

		void parseInput(int argc, char *argv[]);
		void searchForFilePattern();
		vector<string> getFilePatterns();
		string getPath();
		string getSearchText();
		bool isRecursive();
		bool displayDuplicates();
		bool searchRequired();
		void displayInput();
	private:
		string path, searchpattern;
		filePatterns filepatterns;
		string args[100];
		string recursiveOption, duplicateOption, searchOption, recursiveOptionC, duplicateOptionC, searchOptionC;
		int argCount;
		const int NPOS = -1;
		bool recursive, duplicate, search;
		string extractPath();
		void extractFilePatterns();
		void extractOptions();
		pair<string, string> stripOptions(string);
		int hasOptions(string);
	};
}

#endif