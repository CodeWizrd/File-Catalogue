#include "Dsiplay.h"

void Display::printline(const string& line)
{
	cout << line << endl;
}

//----< Print the delimiter >-----------------------------------------------------------------
void Display::printDelimiter()
{
	cout << "---------------------------------------------------------------------------" << endl;
}

//----< Print a given heading >---------------------------------------------------------------
void Display::printHeading(const string& heading)
{
	cout << "                               " << heading << " :                                  " << endl;
}

//----< print welcome message >----------------------------------------------------------------
void Display::printStart()
{
	cout << "===========================================================================" << endl;
	cout << "                      File Catalogue - CSE 687 - Project 1                 " << endl;
	cout << "===========================================================================" << endl;
}

//----< Displaya summary of the datastore contents >--------------------------------------------
void Display::displayDatastoreSummary(DataStore datastore)
{
	datastore.displaySummary();
}

//----< Display the datastore contents >--------------------------------------------------------
void Display::displayDatastoreContent(DataStore datastore)
{
	datastore.displayStore();
}

//----< Display list of duplicates in the datastore >------------------------------------------------------------------
void Display::displayDuplicates(DataStore datastore)
{
	datastore.displayDuplicates();
}

//----< Display the search results >------------------------------------------------------------------
void Display::displaySearch(string results)
{
	cout << results;
}


#ifdef DISPLAY_TEST

int main()
{
	Datastore ds;
	print start();
	printline("Testing Display module");
	displayDatastoreSummary(ds);
	displayDatastoreContent(ds);

	printline("Displaying duplicates:\n");
	displayDuplicates(ds);

	string results = search_results;
	displaySearch(search_results);

	printline("Test Succesful");
	return 0;
}

#endif