#include <stdio.h>
#include <stdlib.h>
#include "nbt.h"

int main(int argc, char *argv[])
{
//	NBTTag *root = nbt_create(TAG_COMPOUND, "root", NULL);
//	NBTTag *leaf = nbt_create(TAG_COMPOUND, "leaf", NULL);
//
//	nbt_compound_set_string(root, "test", "iamastring");
//	nbt_compound_set_byte(root, "abyte", 11);
//	nbt_compound_set_short(root, "ashort", 22);
//	nbt_compound_set_int(root, "aint", 33);
//	nbt_compound_set_long(root, "along", 44);
//	nbt_compound_set_float(root, "afloat", 55.55);
//	nbt_compound_set_double(root, "adouble", 66.66);
//
//	nbt_compound_set_string(leaf, "test", "iamastring");
//	nbt_compound_set_byte(leaf, "abyte", 11);
//	nbt_compound_set_short(leaf, "ashort", 22);
//	nbt_compound_set_int(leaf, "aint", 33);
//	nbt_compound_set_long(leaf, "along", 44);
//	nbt_compound_set_float(leaf, "afloat", 55.55);
//	nbt_compound_set_double(leaf, "adouble", 66.66);
//
//	nbt_compound_set_compoundtag(root, "atag", leaf);

	FILE *in = fopen("log.nbt", "r");
	FILE *out = fopen("rewrite.nbt", "w");

	NBTTag *tag = read_nbt_from_file(in);
	write_nbt_to_file(out, tag);

	fclose(in);
	fclose(out);

	return 0;
}

