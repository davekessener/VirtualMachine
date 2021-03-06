#ifndef PKMN_EDITOR_FILE_H
#define PKMN_EDITOR_FILE_H

#include "../common.h"
#include <string>
#include <vector>
#include "../abstract/Map.h"

namespace editor
{
	class FileHelper;

	class File
	{
		public:
		typedef pkmn::abstract::Map map_t;
		typedef std::vector<map_t> vec_t;
		typedef vec_t::iterator iterator;
		typedef vec_t::const_iterator const_iterator;

		public:
			static iterator begin( );
			static iterator end( );
			static DWORD insert(iterator, const std::string&, uint = 1, uint = 1);
			static DWORD insert(DWORD, const std::string&, uint = 1, uint = 1);
			static void erase(iterator);
			static const map_t& get(QWORD);
			static void set(const map_t&);
			static bool hasName( );
			static void save(const std::string& = "");
			static bool hasChanged( );
			static DWORD getNextID( );
		public:
			static void load(const std::string&);
			static void create( ) { load(""); }
			static void close( );
			static bool isLoaded( );
		private:
			File(const std::string&);
			~File( ) = default;
			friend class FileHelper;
		private:
			static File& instance( );
			struct Impl;
			Impl *impl_;
		private:
			File(const File&) = delete;
			File& operator=(const File&) = delete;
	};
}

#endif

