#include <iostream>
#include "DataStore.h"
using namespace std;
using namespace nsDataStore;

class Display
{
public:
	void printline(const string& line);
	void printDelimiter();
	void printHeading(const string& heading);
	void printStart();
	void displayInput();
	void displayDatastoreSummary(DataStore datastore);
	void displayDatastoreContent(DataStore datastore);
	void displayDuplicates(DataStore datastore);
	void displaySearch(string results);
};