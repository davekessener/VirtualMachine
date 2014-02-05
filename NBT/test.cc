#include <iostream>
#include <iomanip>
#include <cstdint>
#include <initializer_list>
#include <vector>
#include <functional>
#include <fstream>
#include <NBT/NBT.h>
#include <GZ/gzstream.h>
#include "Screen.h"
#include "Timer.h"
#include "Keyboard.h"

using namespace std;

#define KEY_CTRL_C 3
#define KEY_CTRL_D 4
#define KEY_ESC 27

void test_write();
void test_read();

void test(void)
{
	test_write();
	test_read();
}

void test_read(void)
{
	ifstream in("wrtest.nbt", ios::in | ios::binary);
	//gzip::igzstream in("test.nbt.gz");
	ofstream out("rewrite.nbt", ios::out | ios::binary);
	//gzip::ogzstream out("rewrite.nbt.gz.dat");

	NBT::TAG_Compound_ptr_t nbt = NBT::Read(in);

	nbt->write(out);

	out.close();
	in.close();
}

void test_write(void)
{
	ofstream out("wrtest.nbt", ios::out | ios::binary);

	NBT::NBT_ptr_t itag  = NBT::NBT_ptr_t(new NBT::TAG_Int("id", (NBT::DWORD) 0xaabbccdd));
	NBT::NBT_ptr_t dtag  = NBT::NBT_ptr_t(new NBT::TAG_Double("xval", 75.1234));
	NBT::NBT_ptr_t stag  = NBT::NBT_ptr_t(new NBT::TAG_String("", "hitmonlee"));
	NBT::NBT_ptr_t s2tag = NBT::NBT_ptr_t(new NBT::TAG_String("", "hitmonchan"));
	NBT::NBT_ptr_t s3tag = NBT::NBT_ptr_t(new NBT::TAG_String("", "hitmontop"));
	NBT::NBT_ptr_t s4tag = NBT::NBT_ptr_t(new NBT::TAG_String("", "quilava"));
	vector<NBT::NBT_ptr_t> svec = {stag, s2tag, s3tag, s4tag};
	NBT::NBT_ptr_t list  = NBT::NBT_ptr_t(new NBT::TAG_List("pokemon", svec.cbegin(), svec.cend()));
	NBT::NBT_ptr_t arr   = NBT::NBT_ptr_t(new NBT::TAG_Int_Array("ids", {111, 222, 333, 444, 555, 666, 777}));
	NBT::NBT_ptr_t nbt1  = NBT::NBT_ptr_t(new NBT::TAG_Compound("one", {itag, dtag}));
	NBT::NBT_ptr_t nbt2  = NBT::NBT_ptr_t(new NBT::TAG_Compound("two", {list}));
	NBT::NBT_ptr_t nbt3  = NBT::NBT_ptr_t(new NBT::TAG_Compound("three", {nbt2, arr}));
	vector<NBT::NBT_ptr_t> vec = {itag, dtag, nbt1, nbt3, list, arr};
	NBT::NBT_ptr_t nbttagcompound = NBT::NBT_ptr_t(new NBT::TAG_Compound(vec.cbegin(), vec.cend()));//{itag, dtag, nbt1, nbt3, list, arr}));

	nbttagcompound->write(out);

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

