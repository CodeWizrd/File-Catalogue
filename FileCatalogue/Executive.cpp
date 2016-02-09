#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "Cataloger.h"
#include "InputParser.h"
#include "Dsiplay.h"

using namespace FileSystem;
using namespace catalog;
using namespace std;

int main(int argc, char** argv)
{
	Cataloger ctlg; 
	Display display;
	display.printStart();

	ctlg.parseInput(argc, argv);
	display.printline("The input provided by the user:");
	ctlg.displayInput();
	display.printDelimiter();
	
	ctlg.beginScan();
	if (!(ctlg.isRecursive() || ctlg.displayDuplicates() || ctlg.searchRequired())) 	{
		display.printHeading("Summary ");
		display.displayDatastoreSummary(ctlg.getDataStore());
	}
	display.displayDatastoreContent(ctlg.getDataStore());
	display.printDelimiter();

	if (ctlg.displayDuplicates()) 	{
		display.printline("List of duplicates");
		display.displayDuplicates(ctlg.getDataStore());
		display.printDelimiter();
	}

	if (ctlg.searchRequired()) 	{
		display.printline("Searching for " + ctlg.getSearchString() + "...");
		ctlg.searchText();
		display.displaySearch(ctlg.displaySearchResults());
		display.printDelimiter();
	}

	for (;;) 	{
		string text, patterns;
		display.printline("Enter the text to be searched (or hit 'Enter' to exit):  ");
		getline(cin, text);
		if (text == "\n" || text == "") break;
		display.printline("Enter any specific file(s) / file pattern(s) that you want to searh on: (or hit 'Enter')");
		getline(cin, patterns);
		ctlg.extractNewSearchPattern(patterns);
		ctlg.searchText(text);
		display.displaySearch(ctlg.displaySearchResults());
		display.printDelimiter();
	}
	return 0;
}
