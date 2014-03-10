#ifndef EDITOR_SETTINGS_H
#define EDITOR_SETTINGS_H

#include <string>
#include <map>

namespace editor
{
	class Settings
	{
		public:
			static bool getBool(const std::string&);
			static void setBool(const std::string&, bool);
			static const std::string TS_GRID;
			static const std::string TS_BUTTON_GRID;
		private:
			Settings( ) { }
			static Settings& instance( ) { static Settings s; return s; }
			std::map<std::string, bool> boolMap;
	};
}

#endif

