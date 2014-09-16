#include <iostream>
#include <vector>
#include <ctime>
#include <aux>

class DaT
{
	public:
		DaT( ) : now_(time(0)), local_(localtime(&now_)) { }
		inline int year( ) const { return local_->tm_year + 1900; }
		inline int month( ) const { return local_->tm_mon + 1; }
		inline int day( ) const { return local_->tm_mday; }
		inline int hour( ) const { return local_->tm_hour; }
		inline int minute( ) const { return local_->tm_min; }
		inline int second( ) const { return local_->tm_sec; }
		void print(std::ostream& os) const { os << year() << "-" << month() << "-" << day() << " " << hour() << ":" << minute() << ":" << second(); }
	private:
		time_t now_;
		tm *local_;
};

inline std::ostream& operator<<(std::ostream& os, const DaT& dat)
{
	dat.print(os);
	return os;
}

int main(int argc, char *argv[])
try
{
	using dav::aux::lexical_cast;

	std::vector<std::string> args(argv, argv + argc);

	std::string s("");
	int year, month, day, hour, minute;

	auto filter = [&s](int l, int d) -> int
	{
		if((int)s.length() < l) return d;
		int r = lexical_cast<int>(s.substr(0, l));
		s = s.substr(l);
		return r;
	};

	std::getline(std::cin, s);

	year = filter(4, 0);
	month = filter(2, 0);
	day = filter(2, 0);
	hour = filter(2, 24);
	minute = filter(2, 60);

	if(year < 0 || month < 0 || day < 0 || hour < 0 || minute < 0) return 1;

	if(year == 0) year = -1;
	if(month == 0) month = -1;
	if(day == 0) day = -1;
	if(hour == 24) hour = -1;
	if(minute == 60) minute = -1;

	DaT cur;

	if(    (year >= 0 && year != cur.year())
		|| (month >= 0 && month != cur.month())
		|| (day >= 0 && day != cur.day())
		|| (hour >= 0 && hour != cur.hour())
		|| (minute >= 0 && minute != cur.minute())) return 1;

	std::cout << "SUCCESS" << std::endl;

	return 0;
}
catch(...)
{
	return 1;
}

