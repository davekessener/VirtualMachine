#include <NBT/NBT.h>

int main(void)
{
	nbt::TAG_Compound::ptr_t ptr = nbt::readFile("test.nbt");
	nbt::writeFile("rewrite.nbt", ptr);

	return 0;
}

