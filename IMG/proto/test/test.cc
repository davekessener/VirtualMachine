#include <iostream>
#include <iomanip>
#include <cstdint>
#include <initializer_list>
#include <functional>
#include "Screen.h"
#include "Timer.h"
#include "Keyboard.h"
#include "NBT.h"
#include <fstream>

using namespace std;

#define KEY_CTRL_C 3
#define KEY_CTRL_D 4
#define KEY_ESC 27

void test(void)
{
	ofstream out("test.nbt", ios::out | ios::binary);

	NBT::TAG_Int itag("id", (NBT::DWORD) 0xaabbccdd);
	NBT::TAG_Double dtag("xval", 75.1234);
	NBT::TAG_String stag("", "hitmonlee");
	NBT::TAG_String s2tag("", "hitmonchan");
	NBT::TAG_String s3tag("", "hitmontop");
	NBT::TAG_String s4tag("", "quilava");
	NBT::TAG_List list("pokemon", {&stag, &s2tag, &s3tag, &s4tag});
	NBT::TAG_Int_Array arr("ids", {111, 222, 333, 444, 555, 666, 777});
	NBT::TAG_Compound nbt1("one", {&itag, &dtag});
	NBT::TAG_Compound nbt2("two", {&list});
	NBT::TAG_Compound nbt3("three", {&nbt2, &arr});
	NBT::TAG_Compound nbttagcompound({&itag, &dtag, &nbt1, &nbt3, &list, &arr});

	nbttagcompound.write(out);

	out.close();
}

// # ===========================================================================

template<typename T>
function<bool(T)> createContainCheck(initializer_list<T> l)
{
	return [l](T v) -> bool
	{
		for(const T& t : l)
		{
			if(t == v) return true;
		}

		return false;
	};
}

int main(int argc, char *argv[])
{
	test();
	return 0;

	Keyboard &kb = Keyboard::instance();
	Screen s;
	Timer t;
	int h = s.height(), w = s.width();

	auto check = createContainCheck<int>({KEY_ESC, KEY_CTRL_C});

	s.resetColors();
	s.clear();
	s.setColor(0, 7);

	for(int y = 0 ; y < h ; y++)
	{
		if(y == 0 || y == h - 1)
		{
			s.setPos(0, y);
			for(int x = 0 ; x < w ; x++)
			{
				s << ((x == 0 || x == w - 1) ? '+' : '-');
			}
		}
		else
		{
			s.put(0, y, '|');
			s.put(w - 1, y, '|');
		}
	}

	s.resetColors();
	s.setPos(0, 0);
	s.flush();

	s.setPos(2,2);
	s << "\e[1;37;42m-\e[0m root";
	s.setPos(2,3);
	s << "\u2523\u2501\e[1;37;42m-\e[0m branch";
	s.setPos(2,4);
	s << "\u2503 \u2517\u2501\u2501 leaf";
	s.setPos(2,5);
	s << "\u2517\u2501\e[1;30;41m+\e[0m leaf";
	s.setPos(0, h - 1);
	s << "\n\r";
	s.flush();

	return 0;

	int c = 0;
	long i = 0;
	while(!check(c))
	{
		s.setPos(2, 2);
		s << i;
		s.setPos(0, 0);
		s.flush();

		t.reset();
		while(t.elapsed() < 1000)
		{
			if(!kb.empty()) 
			{
				if(check(c = kb.pop())) goto end;
				kb.clear();
			}
			t.sleep(15);
		}

		i++;
	}

end:

	s.setPos(0, h - 1);
	s << "\n\r";

	return 0;
}

