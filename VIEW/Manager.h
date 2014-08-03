#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <string>
#include "inc.h"

class Manager
{
	public:
		static Manager& instance( );
		int run(const std::vector<std::string>&);
		void createTexture(const BYTE *, int, int);
		void draw(float, float, float, float, int, int, int, int) const;
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

