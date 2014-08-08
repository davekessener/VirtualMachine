#ifndef DIR_H
#define DIR_H

#include <string>
#include <vector>

class dir
{
	public:
		dir(const std::string&);
		const std::string& getBase( ) const { return base_; }
		const std::string& getOrig( ) const { return orig_; }
		const std::string& getBest( ) const { return best_; }
		std::string getNext( );
	private:
		std::string base_, orig_, best_;
		std::vector<std::string> opt_;
		std::vector<std::string>::const_iterator cur_;
};

#endif

