#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <string>
#include <memory>
#include "inc.h"
#include "Screen.h"

class Manager
{
	public:
	typedef std::shared_ptr<Screen> screen_ptr;

	public:
		static Manager& instance( );
		int run(const std::vector<std::string>&);
		void createTexture(const BYTE *, int, int);
		void draw(float, float, float, float, int, int, int, int) const;
		void setCharSize(int);
		int getStringWidth(const std::string&) const;
		void renderString(const std::string&, int, int, int = 0xffffff) const;
		void renderCenteredString(const std::string&, int, int, int = 0xffffff) const;
		void pushScreen(screen_ptr);
	private:
		struct Impl;
		Impl *impl_;
	private:
		Manager( );
		Manager(const Manager&);
		~Manager( );
		Manager& operator=(const Manager&);
};

#endif

