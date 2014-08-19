#include <iostream>
#include <vector>
#include <nbt/NBT.h>

int main(int argc, char *argv[])
{
	using namespace nbt;

	TAG_Compound::ptr_t nbt = Make<TAG_Compound>();
	nbt->setByte("Data", 0xcc);
	nbt->setDouble("PI", 3.1415);
	nbt->setString("Password", "adminadmin");
	TAG_List::ptr_t list = Make<TAG_List>("Pokemon");
	list->addTag(Make<TAG_String>("", "Squirtle"));
	list->addTag(Make<TAG_String>("", "Charmander"));
	list->addTag(Make<TAG_String>("", "Bulbasaur"));
	nbt->setTagList(list);
	TAG_Compound::ptr_t pkmn = Make<TAG_Compound>("Quilava");
	pkmn->setString("Name", "Quilava");
	pkmn->setInt("ID", 155);
	pkmn->setIntArray("Stats", std::vector<DWORD>{151, 120, 174, 200, 100});
	nbt->setCompoundTag(pkmn);

	writeFile("test.nbt", nbt);

	return 0;
}

