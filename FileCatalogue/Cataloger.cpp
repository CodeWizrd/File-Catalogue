#include "Cataloger.h"

using namespace std;
using namespace FileSystem;
using namespace nsDataStore;
using namespace catalog;

//----< Default constructor for Cataloger >----------------------------------------------------
Cataloger::Cataloger() {
	

}


//----< parse the command line arguments input by user >---------------------------------------
void Cataloger::parseInput(int argc, char* argv[])
{
	clparse.parseInput(argc, argv);
	filepatterns_ = clparse.getFilePatterns();
	path_ = clparse.getPath();
	if (clparse.searchRequired()) searchtext_ = clparse.getSearchText();
}

//----< display the command line arguments input by user >-------------------------------------
void Cataloger::displayInput()
{
	clparse.displayInput();
}

//----< scan the specified path for specified files to build the catalogue >--------------------
void Cataloger::beginScan()
{
	string temp_path, temp_filePattern;
	filePatterns files;
	filepatrnIter patternIter;
	filepatrnIter fileIter;
	for (patternIter = filepatterns_.begin(); patternIter != filepatterns_.end(); ++patternIter) 	{
		while (true) 	{
			if (Directory::exists(path_)) 		{
				Directory::setCurrentDirectory(path_);
				break;
			}
			else
				Directory::setCurrentDirectory("..");
		}
		temp_path = Directory::getCurrentDirectory();
		temp_filePattern = patternIter->c_str();
		files = Directory::getFiles(temp_path, temp_filePattern);
		int i = 1;
		for (fileIter = files.begin(); fileIter != files.end(); ++fileIter) {
			saveToDataStore((*fileIter), temp_path);
			i++;
		}
	}
	// Begin recursive search if /s option specified
	if (clparse.isRecursive())
	{
		filePatterns directories = Directory::getDirectories();
		filepatrnIter dirIter;

		for (dirIter = directories.begin(); dirIter != directories.end(); ++dirIter)
		{
			path_ = (*dirIter);
			if (path_ != "." && path_ != "..")
				beginScan();
		}
	}
}

//----< save the catalogue >--------------------------------------------------------------------
void Cataloger::saveToDataStore(const string& filename, const string& path)
{
	datastore.save(filename, path);
}

//----< Display the data store contents >--------------------------------------------------------
void Cataloger::displayDatastoreContent()
{
	datastore.displayStore();
}

//----< Display a summary of the data store contents >-------------------------------------------
void Cataloger::displayDatastoreSummary()
{
	datastore.displaySummary();
}

//----< Display list of stored duplcate files >---------------------------------------------------
void Cataloger::displayDatastoreDuplicates()
{
	datastore.displayDuplicates();
}

//----< extract file patterns for searching >------------------------------------------------------
void Cataloger::extractNewSearchPattern(const string& searchPattern)
{
	searchpatterns_.clear();
	size_t pos = 0;
	int idx = NPOS, nextString = NPOS, currentStart = 0;
	string temp_pattern;
	for (; pos < searchPattern.length(); pos++)
	{
		idx = searchPattern.find(".", pos);
		if (idx != NPOS)
		{
			pos = idx + 1;
			nextString = searchPattern.find(" ", idx);
			if (nextString != NPOS)
			{
				temp_pattern = searchPattern.substr(currentStart, nextString);
			}
			else temp_pattern = searchPattern.substr(currentStart, searchPattern.length());
			searchpatterns_.push_back(temp_pattern);
		}
	}
	if (searchpatterns_.size() == 0)
	{
		searchpatterns_ = filepatterns_;
	}
	if (searchpatterns_.size() == 0) searchpatterns_.push_back("*.*");
}

//----< search for text in built catalogue >-------------------------------------------------------
void Cataloger::searchText()
{
	map<string, list<set<string>::iterator>>::iterator iter;
	map<string, set<int>> results;
	string filename, path;
	if (searchtext_ == "") exit(0);

	for (iter = datastore.begin(); iter != datastore.end(); ++iter)
	{
		filename = iter->first;
		set<string> path = datastore.getPath(filename);
		set<string>::iterator pt;
		set<int> success;
		for (pt = path.begin(); pt != path.end(); ++pt)
		{
			string fullpath = (*pt) + "\\" + filename;
			File file(fullpath);
			file.open(File::in);
			if (file.isGood())
			{
				size_t pos = 0;
				int idx = NPOS;
				std::string fileContents = file.readAll(false);
				while (pos < fileContents.length())
				{
					idx = fileContents.find(searchtext_, pos);
					if (idx != NPOS)
					{
						pos = idx + searchtext_.length();
						success.insert(idx);
					}
					else
						pos++;
				}
				results[fullpath] = success;
			}
			file.close();
		}
	}
	searchresults = results;
}

//----< search for text in specific files in the built catalogue >---------------------------------
void Cataloger::searchText(string searchtext)
{
	searchtext_ = searchtext;
	map<string, list<set<string>::iterator>>::iterator iter;
	map<string, set<int>> results;
	string filename, path;

	for (iter = datastore.begin(); iter != datastore.end(); ++iter)
	{
		filename = iter->first;
		if (matchFnameForSearch(filename))
		{
			set<string> path = datastore.getPath(filename);
			set<string>::iterator pt;
			set<int> success;
			for (pt = path.begin(); pt != path.end(); ++pt)
			{
				string fullpath = (*pt) + "\\" + filename;
				File file(fullpath);
				file.open(File::in);
				if (file.isGood())
				{
					size_t pos = 0;
					int idx = NPOS;
					std::string fileContents = file.readAll(false);
					while (pos < fileContents.length())
					{
						idx = fileContents.find(searchtext_, pos);
						if (idx != NPOS)
						{
							pos = idx + searchtext_.length();
							success.insert(idx);
						}
						else 
							pos++; 
					}
					results[fullpath] = success;
				}
				file.close();
			}
		}
	}
	searchresults = results;
}

//----< Checks if file patterns entered as part of search text functionality ------------------------
//----  match the files present in the catalogue >---------------------------------------------------
bool Cataloger::matchFnameForSearch(const string& filename) {
	filepatrnIter searchIter;
	bool expression = false;
	string subpatterns[10], fType, fName;
	int idx = NPOS, idx2 = NPOS, i = 0, start = 0, found, count = 0;
	if (searchpatterns_.at(0) == "") return true;
	for (searchIter = searchpatterns_.begin(); searchIter != searchpatterns_.end() && i < 10; ++searchIter) 	{
		string pattern = searchIter->c_str();
		idx = pattern.find("*.");
		if (idx != NPOS) 		{
			expression = true;
			if (idx == 0) 			{
				subpatterns[0] = pattern.substr(idx + 1);
				i = 0;
			}
			else 			{
				subpatterns[0] = pattern.substr(0, idx);
				i = 1;
			}
			idx2 = pattern.find("*");
			if (idx2 == NPOS) 	subpatterns[1] = pattern.substr(idx + 1);
			else 	subpatterns[1] = pattern.substr(idx + 1);
		}
		if (!expression)
			found = filename.find(pattern);
			if (found != NPOS) return true;
		else 		{
			for (int j = 0; j <= i; j++) 			{
				found = filename.find(subpatterns[j]);
				(found != NPOS) ? count++ : count = count;
			}
			if (!(count < i) && count != 0) 			{
				count = 0;
				return true;
			}	
		}
	}
	return false;
}

//----< Find if option '/s' is specified >------------------------------------------------------------
bool Cataloger::isRecursive()
{
	return clparse.isRecursive();
}

//----< Find if option '/d' is specified >-------------------------------------------------------------
bool Cataloger::displayDuplicates()
{
	return clparse.displayDuplicates();
}

//----< Return the search results >-------------------------------------------------------------------
string Cataloger::displaySearchResults()
{

	searchResults::iterator resultIter;
	set<int>::iterator setIter;
	string positions;
	string result;
	for (resultIter = searchresults.begin(); resultIter != searchresults.end(); ++resultIter)
	{
		for (setIter = (resultIter->second).begin(); setIter != (resultIter->second).end(); ++setIter)
			positions = positions + to_string(*setIter) + ",\t ";
		positions = positions.substr(0, positions.size() - 2);
		if (positions.size() > 0)
		{
			result = result + "File: " + resultIter->first + "\nPosition(s): \t" + positions + "\n\n";
			positions.clear();
		}
	}
	if (result.size() <= 0)
		result = "Text is not present in any of the files searched\n\n";
	return result;
}

//----< Find if option '/f' is specified >-------------------------------------------------------------
bool Cataloger::searchRequired()
{
	return clparse.searchRequired();
}

//----< Return the search text >-----------------------------------------------------------------------
string Cataloger::getSearchString()
{
	return searchtext_;
}

//----< Return the data store object >-----------------------------------------------------------------
DataStore Cataloger::getDataStore()
{
	return datastore;
}

#ifdef TEST_CATALOGER
int main( int argc, char *argv[])
{
	cout << "Parse Input: " << endl;
	parseInput(argc, argv);
	displayInput();

	cout << "Build the catalogue" << endl;
	beginScan();
	saveToDataStore();

	cout << "Displaying the cataogue" << endl;
	cout << "Summary: ";
	displayDatastoreSummary();
	cout << "Catalogue content: " << endl;
	displayDatastoreContent();

	if (isRecursive()) cout << "User wants recursive search the path" << endl;
	if (displayDuplicates)
	{
		cout << "User wants to see duplicate files as well" << endl;
		displayDatastoreDuplicates();
	}
	if (searchRequired)
	{
		cout << "User has  specified a search text: " << getSearchString() << endl;
		cout << "Searching..." << endl;
		searchText();
		displaySearchResults();
	}
	while (true)
	{
		string text, pattern;
		cout << "Enter the search text: (Hit Enter to exit)";
		getline (cin, text);
		if (text == "\n" || text =="") break;
		cout << "Enter pattern";
		getline (cin, pattern);
		extractNewSearchPattern();
		searchText(text);
		displaySearchResults();
	}

	return 0;
}
#endif