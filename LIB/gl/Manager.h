#ifndef MANAGER_H
#define MANAGER_H

struct Block;

class Manager
{
	public:
		static Manager& instance( );
		const Block& getBlock(int) const;
	private:
		Manager( );
		Manager(const Manager&);
		Manager& operator=(const Manager&);
		~Manager( );
	private:
		struct Impl;
		Impl *impl_;
};

#endif

