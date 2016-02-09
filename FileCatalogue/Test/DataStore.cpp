#include <iostream>
#include "DataStore.h"
#include "Cataloger.h"
using namespace nsDataStore;

//----< Build the datastore >------------------------------------------------------------------
void DataStore::save(const std::string& filename, const std::string& path)
{
	PathIter pItr = paths.find(path);
	if (pItr == paths.end())
	{
		pair<PathIter, bool> insert;
		insert = paths.insert(path);
		pItr = insert.first;
	}
	iterator fIt = store.find(filename);
	if (fIt == store.end())
	{
		ListOfIters litr;
		litr.push_back(pItr);
		store.insert(std::pair<File, ListOfIters>(filename, litr));
	}
	else
		fIt->second.push_back(pItr);
}

void save(const std::string& filespec)
{

}

//----< Return the path >-----------------------------------------------------------------------
set<string> DataStore::getPath(File file)
{
	Paths path;
	iterator iter;
	for (iter = store.begin(); iter != store.end(); ++iter)
	{
		if ((*iter).first == file)
		{
			ListOfIters list = iter->second;
			LOIIter listIter;
			for (listIter = list.begin(); listIter != list.end(); ++listIter)
			{
				PathIter pathiter = *listIter;
				path.insert(*pathiter);
			}
		}
	}

	return path;
}

//----< Display a summary of the data store contents >-------------------------------------------
void DataStore::displaySummary()
{
	cout << "---------------------------------------------------------------------------" << endl;
	cout << "                                 Summary :                                 " << endl;
	cout << "The DataStore contains " << store.size() << " file(s) in " << paths.size() << " path(s)" << endl << endl;
}

//----< Display the data store contents >--------------------------------------------------------
void DataStore::displayStore()
{
	cout << "---------------------------------------------------------------------------" << endl;
	iterator iter;
	string path, pathnames;
	PathIter pathIter;
	ListOfIters list;
	LOIIter listIter;
	for (iter = store.begin(); iter != store.end(); ++iter)
	{
		cout << " FileName: " << iter->first << endl;
		cout << " Paths: ";
		list = iter->second;
		for (listIter = list.begin(); listIter != list.end(); ++listIter)
		{
			pathIter = *listIter;
			path = *pathIter;
			pathnames += "\n   ---" + path;
		}
		pathnames = pathnames.substr(0, pathnames.length());// -2);
		cout << pathnames;
		pathnames.clear();
		cout << endl << endl;
	}
	displaySummary();
}

//----< Display list of stored duplcate files >---------------------------------------------------
void DataStore::displayDuplicates()
{
	iterator iter;
	PathIter pathiter;
	string pathnames, path;
	for (iter = store.begin(); iter != store.end(); ++iter)
	{
		size_t size = (iter->second).size();
		if (size > 1)
		{
			cout << " FileName: " << iter->first << endl;
			cout << " Paths: ";
			ListOfIters list = iter->second;
			LOIIter listIter;
			for (listIter = list.begin(); listIter != list.end(); ++listIter)
			{
				pathiter = *listIter;
				path = (*pathiter);
				pathnames += "\n   ---" + path;
			}
			pathnames = path.substr(0, path.length());
			cout << path;
			cout << endl << endl;
		}
	}
}

#ifdef TEST_DATASTORE
int main(int argc, char *argv[])
{
	cout << "Testing Data Store..." << endl;

	filename = argv[1];
	path = argv[2];
	cout << "Save to Data Store: " << endl;
	save(filename, path);
	cout << "Saved!" < endl;

	cout << "Dispaying the data store" << endl;
	displaySummary();
	displayStore();

	cout << "Displaying the duplicates: " << endl;
	displayDuplicates();

	cout << "Test Success" << endl;

	return 0;
}
#endif