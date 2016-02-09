#ifndef CATALOGER_H
#define CATALOGER_H
#include <string>
#include <iostream>
#include "FileSystem.h"
#include "DataStore.h"
#include "InputParser.h"

using namespace std;
using namespace FileSystem;
using namespace clparser;
using namespace nsDataStore;

namespace catalog
{

	class Cataloger
	{
	public:
		using filePatterns = vector < string >;
		using filepatrnIter = filePatterns::iterator;
		using searchResults = map < string, set<int> >;

		Cataloger();
		~Cataloger() { }
		void parseInput(int, char**);
		void displayInput();
		void beginScan();
		void displayDatastoreContent();
		void displayDatastoreSummary();
		void displayDatastoreDuplicates();
		string displaySearchResults();
		void extractNewSearchPattern(const string&);
		DataStore getDataStore();
		void searchText();
		void searchText(string);
		bool isRecursive();
		bool displayDuplicates();
		bool searchRequired();
		string getSearchString();

		
	private:
		string path_, searchtext_;
		filePatterns searchpatterns_;
		filePatterns filepatterns_;
		inputParser clparse;
		DataStore datastore;
		searchResults searchresults;

		void saveToDataStore(const string&, const string&);
		const int NPOS = -1;
		bool matchFnameForSearch(const string& filename);
	};

}

#endif