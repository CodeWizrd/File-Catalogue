#include "InputParser.h"

using namespace std;
using namespace clparser;

//----< Default Constructor for InputParser >--------------------------------------------------------
inputParser::inputParser()
{
	duplicate = false; recursive = false; search = false;
	recursiveOption.assign("/s"); recursiveOptionC.assign("/S");
	duplicateOption.assign("/d"); duplicateOptionC.assign("/D");
	searchOption.assign("/f"); searchOptionC.assign("/F");
	filepatterns.push_back("*.*");
}

//----< Default Destructor for InputParser >---------------------------------------------------------
inputParser::~inputParser()
{

}

//----< Pasre the command line arguments >-----------------------------------------------------------
void inputParser::parseInput(int argc, char*argv[]) {
	argCount = argc;
	for (int i = 1; i < argCount; i++)
		args[i] = argv[i];
	cout << endl;
	extractOptions();
	path = extractPath();
	extractFilePatterns();
}

//----< Extracts the path in which search is to be conducted >---------------------------------------
string inputParser::extractPath() {
	pair<string, string> p;
	p = stripOptions(args[1]);
	(p.first == args[1]) ? args[1].clear() : args[1].shrink_to_fit();
	args[1] = p.second;
	if (p.first == p.second) args[1].clear();
	args[1].shrink_to_fit();
	return p.first;
}

//----< Checks cases where multiple command line arguments clubbed together >------------------------
int inputParser::hasOptions(string arg) {
	bool option_flag = false;
	int idx = -1, idx2 = -1, idx3 = -1, idx4 = -1, idx5, idx6, minidx = 10000;
	idx = arg.find(duplicateOption);
	idx2 = arg.find(duplicateOptionC);
	idx3 = arg.find(recursiveOption);
	idx4 = arg.find(recursiveOptionC);
	idx5 = arg.find(searchOption);
	idx6 = arg.find(searchOptionC);
	(idx != string::npos || idx2 != string::npos) ? minidx = min(minidx, (int)max(idx, idx2)) : minidx = minidx;
	(idx3 != string::npos || idx4 != string::npos) ? minidx = min(minidx, (int)max(idx3, idx4)) : minidx = minidx;
	(idx5 != string::npos || idx6 != string::npos) ? minidx = min(minidx, (int)max(idx3, idx4)) : minidx = minidx;
	(idx2 != string::npos) ? idx = idx2 : idx = idx;
	(idx4 != string::npos) ? idx3 = idx4 : idx3 = idx3;
	(idx6 != string::npos) ? idx5 = idx6 : idx5 = idx5;
	(minidx == 10000) ? minidx = NPOS : minidx = minidx;
	return minidx;
}

//----< Seprates arguments that are clubbed together on the command line >---------------------------
pair<string, string> inputParser::stripOptions(string arg) {
	string arg1, arg2;
	pair<string, string> p;
	p.first = arg; p.second = arg;
	int idx = hasOptions(arg);
	if (idx != NPOS) 	{
		(idx != NPOS) ? arg1 = arg.substr(0, idx) : arg1 = arg1;
		(idx != NPOS) ? arg2 = arg.substr(idx + 2) : arg2 = arg2;
		(idx != NPOS) ? arg.erase(idx, 2) : arg.shrink_to_fit();
		arg2.shrink_to_fit();
		p.first = arg1; p.second = arg2;
	}
	p.second.shrink_to_fit();
	return p;
}

//----< Extracts the filename to be searched >--------------------------------------------------------
void inputParser::extractFilePatterns() {
	int i, j = 7;
	if (filepatterns.at(0) == "*.*")
	{
		filepatterns.pop_back();
	}
	bool flag = false;
	string ptrn, orig;
	pair<string, string> p;
	pair<string, string> q;
	(args[1].empty()) ? i = 2 : i = 1;
	for (; i < argCount; i++) 	{
		while (!args[i].empty())	{
			p = stripOptions(args[i]);
			args[i] = p.second;
			args[i].shrink_to_fit();
			break;
			i = argCount;
		}
		if (!args[i].empty())
			if (args[i] == p.second) 	{
			flag = true;
			filepatterns.push_back(p.second);
			}
	}
	if (!flag) filepatterns.push_back("*.*");
}

//----< Information about whether /s, /d & /f options specified >-------------------------------------
void inputParser::extractOptions() {
	int i, idx = -1, idx2 = -1, idx3 = -1, idx4 = -1, idx5, idx6;
	bool search_changed = false;
	for (i = 1; i < argCount; i++) 	{
		idx = args[i].find(duplicateOption);
		idx2 = args[i].find(duplicateOptionC);
		idx3 = args[i].find(recursiveOption);
		idx4 = args[i].find(recursiveOptionC);
		idx5 = args[i].find(searchOption);
		idx6 = args[i].find(searchOptionC);
		(idx != string::npos || idx2 != string::npos) ? duplicate = true : duplicate = duplicate;
		(idx3 != string::npos || idx4 != string::npos) ? recursive = true : recursive = recursive;
		(idx5 != string::npos || idx6 != string::npos) ? search_changed = true : search_changed = false;
		if (search_changed) {
			search = true;
			searchpattern = args[i + 1];
			args[i + 1].clear(); args[i + 1].shrink_to_fit();
			args[i].erase(max(idx5, idx6), 2); args[i].shrink_to_fit();
		}
	}
}

//----< Start searching for specified file at specified path >----------------------------------------
void inputParser::searchForFilePattern() {
	cout << "In searchForFilePattern(): " << endl;
} 

//----< Returns File Pattern >------------------------------------------------------------------------
vector<string> inputParser::getFilePatterns()
{
	return filepatterns;
}

//----< Returns Path >--------------------------------------------------------------------------------
string inputParser::getPath()
{
	return path;
}

//----< Returns Search Text >--------------------------------------------------------------------------
string inputParser::getSearchText()
{
	return searchpattern;
}

//----< Find if option '/s' is specified >-------------------------------------------------------------
bool inputParser::isRecursive()
{
	return recursive;
}

//----< Find if option '/d' is specified >-------------------------------------------------------------
bool inputParser::displayDuplicates()
{
	return duplicate;
}

//----< Find if option '/f' is specified >-------------------------------------------------------------
bool inputParser::searchRequired()
{
	return search;
}

//----< Display the command line arguments >-------------------------------------------------------------
void inputParser::displayInput()
{
	filepatrnIter iter;
	cout << endl << "  Path to be searched: " << path << endl;
	cout << "  File patterns to be searched for: ";
	for (iter = filepatterns.begin(); iter != filepatterns.end(); ++iter)
	{
		cout << iter->c_str() << "   ";
	}
	cout << endl<< "  Is the search recursive? ";
	if (isRecursive()) cout << "Yes." << endl;
	else cout << "No." << endl;
	cout << "  Should duplicate file names be displayed? ";
	if (displayDuplicates()) cout << "Yes." << endl;
	else cout << "No." << endl;
	cout << "  Search Text Option enabled? ";
	if (searchRequired()) cout << "Yes." << "\n  ---Text to be searched: " << searchpattern << endl;
	else cout << "no" << endl;
	cout << endl;
}

#ifdef TEST_INPUTPARSER
int main(int argc, char *argv[])
{
	cout << "Parse Input: " << endl;
	parseInput(argc, argv);
	displayInput();

	if (isRecursive()) cout << "User wants recursive search the path" << endl;
	if (displayDuplicates)
	{
		cout << "User wants to see duplicate files as well" << endl;
	}
	if (searchRequired)
	{
		cout << "User has  specified a search text: " << getSearchString() << endl;
		getSearchText();
		getFilePatterns();
	}

	return 0;
}
#endif