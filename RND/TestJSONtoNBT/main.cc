#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <nbt/NBT.h>

int main(int argc, char *argv[])
try
{
	std::vector<std::string> args(argv + 1, argv + argc);

	if(args.size() != 1) throw std::string("need nbt-filename!");

	nbt::TAG_Compound_ptr_t nbt = nbt::Make<nbt::TAG_Compound>();

	nbt->setLong("ID", 42);
	nbt->setString("Name", "Example");
	nbt->setLong("Accuracy", 100);
	nbt->setLong("Strength", 65);
	
	nbt::TAG_Compound_ptr_t desc = nbt::Make<nbt::TAG_Compound>();
	desc->setString("EN-US", "desc in us eng");
	desc->setString("EN-UK", "desc in brit eng");
	desc->setString("EN-AS", "desc in as eng");
	desc->setString("FR", "decc ind french");
	desc->setString("GER", "dec in german");

	nbt->setCompoundTag("Description", desc);

	nbt::TAG_List_ptr_t effect = nbt::Make<nbt::TAG_List>();
	nbt::TAG_Compound_ptr_t e1 = nbt::Make<nbt::TAG_Compound>();
	e1->setLong("ID", 413);
	e1->setLong("chance", 10);
	e1->setString("status", "Poison");
	effect->addTag(e1);
	nbt::TAG_Compound_ptr_t e2 = nbt::Make<nbt::TAG_Compound>();
	e2->setLong("ID", 42);
	e2->setLong("chance", 30);
	e2->setString("status", "Confusion");
	effect->addTag(e2);
	nbt::TAG_Compound_ptr_t e3 = nbt::Make<nbt::TAG_Compound>();
	e3->setLong("ID", 13);
	e3->setLong("chance", 40);
	e3->setString("status", "Infatuation");
	effect->addTag(e3);

	nbt->setTagList("Effects", effect);

	nbt::writeFile(args.at(0) + ".nbt", nbt);

	std::ofstream out(args.at(0) + ".json");
	nbt::json::write(out, nbt);
	out.close();

	std::ifstream in(args.at(0) + ".json");
	nbt::TAG_Compound_ptr_t check = nbt::json::read(in);
	in.close();

	out.open(args.at(0) + "_check.json");
	nbt::json::write(out, check);
	out.close();

	return 0;
}
catch(const std::string& e)
{
	std::cerr << std::flush << e << std::endl;
}

