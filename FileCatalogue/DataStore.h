#ifndef DATASTORE_H
#define DATASTORE_H

#include <vector>
#include <set>
#include <list>
#include <map>
#include <string>

namespace nsDataStore
{
	class DataStore
	{
	public:
		using Path = std::string;
		using Paths = std::set < Path > ;
		using PathIter = Paths::iterator;
		using ListOfIters = std::list < PathIter > ;
		using LOIIter = ListOfIters::iterator;
		using File = std::string;
		using Store = std::map < File, ListOfIters > ;
		using iterator = Store::iterator;

		void save(const std::string& filespec);
		void save(const std::string& filename, const std::string& path);
		Paths getPath(File file);
		void displaySummary();
		void displayStore();
		void displayDuplicates();

		iterator begin() { return store.begin(); }
		iterator end() { return store.end(); }
	private:
		Store store;
		Paths paths;
	};
}
#endif