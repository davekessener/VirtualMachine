#include <zlib.h>
#include "NBT.h"

enum
{
	EXTRACT,
	INSERT
};

int nameEquals(const char *, const char *);
uint getOffset(int, int);
void anvil_insert(FILE *, FILE *, uint);
void anvil_extract(FILE *, FILE *, uint);

void nbt_print(const NBT *, FILE *);

int nbt_print_main(int argc, char *argv[])
{
	if(argc != 2)
	{
		fprintf(stderr, "ERR: usage: %s file.nbt\n", argv[0]);
		return EXIT_FAILURE;
	}

	FILE *f = fopen(argv[1], "rb");

	if(!f)
	{
		fprintf(stderr, "ERR: Couldn't open file '%s'!\n", argv[1]);
		return EXIT_FAILURE;
	}

	NBT *nbt = nbt_read(f);

	fclose(f);
	
//	nbt_print(nbt, stdout);
	nbt_write(nbt, stdout);

	nbt_delete(nbt);

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
	const char **arg = (const char **) (argv + 1);
	int mode = nameEquals(argv[0], "anvil-insert") ? INSERT : EXTRACT;
	int ac = argc - 1;

	if(nameEquals(argv[0], "nbt-print"))
	{
		return nbt_print_main(argc, argv);
	}

	if(ac > 0)
	{
		if(strcmp(*arg, "-e") == 0)
		{
			mode = EXTRACT;
			++arg; --ac;
		}
		else if(strcmp(*arg, "-i") == 0)
		{
			mode = INSERT;
			++arg; --ac;
		}
	}

	if(ac != 4)
	{
		fprintf(stderr, "ERR: Usage: %s [-i | -e] region.mca cx cz data.nbt\n", argv[0]);
		return EXIT_FAILURE;
	}

	const char *region_file = *arg++;

	int chunk_x = atoi(*arg++);
	int chunk_z = atoi(*arg++);

	const char *data_file = *arg;

	int cx = chunk_x % 32, cz = chunk_z % 32;
	if(cx < 0) cx += 32;
	if(cz < 0) cz += 32;

	FILE *rf = fopen(region_file, mode == INSERT ? "r+b" : "rb");
	FILE *df = fopen(data_file,   mode == INSERT ? "rb"  : "wb");

	if(!rf)
	{
		fprintf(stderr, "Couldn't open region file '%s'.\n", region_file);
		return EXIT_FAILURE;
	}

	if(!df)
	{
		fprintf(stderr, "Couldn't open data file '%s' for %s.\n", data_file, (mode == INSERT ? "read" : "write"));
		return EXIT_FAILURE;
	}

	if(mode == INSERT)
	{
		anvil_insert(rf, df, getOffset(cx, cz));
	}
	else
	{
		anvil_extract(rf, df, getOffset(cx, cz));
	}

	fclose(rf);
	fclose(df);

	return EXIT_SUCCESS;
}

// # ===========================================================================

int nameEquals(const char *s, const char *n)
{
	const char *t = s;
	while(*t)
	{
		if(*t == '/') s = t + 1;
		++t;
	}
	
	return strcmp(s, n) == 0;
}

uint getOffset(int x, int z)
{
	assert(x>=0&&x<32&&z>=0&&z<32);
	return (x | (z << 5)) << 2;
}

BYTE *anvil_uncompress(const BYTE *d, DWORD l, DWORD *s, BYTE *buf)
{
	uLong t = *s;
	int r = uncompress(buf, &t, d, l);
	
	switch(r)
	{
		case Z_OK:
			*s = t;
			break;
		case Z_MEM_ERROR:
		case Z_BUF_ERROR:
			free(buf);
			return anvil_uncompress(d, l, s, buf = malloc(*s *= 2));
		default:
			fprintf(stderr, "ERR: Error during decompression (%d)\n", r);
			exit(EXIT_FAILURE);
	}

	return buf;
}

void anvil_extract(FILE *rf, FILE *df, uint o)
{
	DWORD sector;

	fseek(rf, o, SEEK_SET);
	fread(&sector, 1, sizeof(sector), rf);
	
	uint a = ((sector & 0xff) << 16) | (sector & 0xff00) | ((sector & 0xff0000) >> 16);
//	uint l = (sector >> 24) & 0xff;

	fseek(rf, a << 12, SEEK_SET);
	DWORD l;
	fread(&l, 1, sizeof(l), rf);
	l = ((l & 0xff) << 24) | ((l & 0xff00) << 8) | ((l & 0xff0000) >> 8) | ((l & 0xff000000) >> 24);

	BYTE *buf = malloc(l);
	fread(buf, l, sizeof(BYTE), rf);

	if(buf[0] != 2)
	{
		fprintf(stderr, "ERR: Wrong compression scheme (%d).\nWrite compressed data instead.\n", (int)buf[0]);
	}
	else
	{
		DWORD s = l * 2;
		BYTE *zbuf = anvil_uncompress(buf + 1, l - 1, &s, malloc(s));
		free(buf);
		l = s;
		buf = zbuf;
	}

	fwrite(buf, l, sizeof(BYTE), df);

	free(buf);
}

void anvil_insert(FILE *rf, FILE *df, uint o)
{
	DWORD sector;

	fseek(rf, o, SEEK_SET);
	fread(&sector, 1, sizeof(sector), rf);
	
	uint a = ((sector & 0xff) << 16) | (sector & 0xff00) | ((sector & 0xff0000) >> 16);
//	uint l = (sector >> 24) & 0xff;

	fseek(rf, a << 12, SEEK_SET);
	DWORD l;
	fread(&l, 1, sizeof(l), rf);
	l = ((l & 0xff) << 24) | ((l & 0xff00) << 8) | ((l & 0xff0000) >> 8) | ((l & 0xff000000) >> 24);

	fseek(df, 0, SEEK_END);
	long p = ftell(df);
	fseek(df, 0, SEEK_SET);

	BYTE *buf = malloc(p);
	fread(buf, p, sizeof(BYTE), df);

	BYTE *zbuf = malloc(p);
	uLong s = p;
	int r = compress(zbuf, &s, buf, p);

	if(r != Z_OK)
	{
		fprintf(stderr, "ERR: Compression error (%d).\n", r);
		exit(EXIT_FAILURE);
	}

	free(buf);

	buf = zbuf;
	p = s + 1;
	l = ((p & 0xff) << 24) | ((p & 0xff00) << 8) | ((p & 0xff0000) >> 8) | ((p & 0xff000000) >> 24);

	if(p > l)
	{
		fseek(rf, 0, SEEK_END);
		a = ftell(rf);
		int i;
		
		for(i = (0x1000 - (a & 0x0fff)) % 0x1000 ; i > 0 ; --i) putc(0, rf);
		a = (a >> 12) + (a & 0x0fff ? 1 : 0);

		sector =   ((((p >> 12) + (p & 0x0fff ? 1 : 0)) & 0xff) << 24)
				 | ((a & 0xff) << 16) | (a & 0xff00) | ((a & 0xff0000) >> 16);

		fwrite(&l, 1, sizeof(l), rf);
		putc(2, rf);
		fwrite(buf, p - 1, sizeof(BYTE), rf);
		
		p = ftell(rf);
		for(i = (0x1000 - (p & 0x0fff)) % 0x1000 ; i > 0 ; --i) putc(0, rf);

		fseek(rf, o, SEEK_SET);
		fwrite(&sector, 1, sizeof(sector), rf);
	}
	else
	{
		fseek(rf, a << 12, SEEK_SET);
		fwrite(&l, 1, sizeof(l), rf);
		putc(2, rf);
		fwrite(buf, p - 1, sizeof(BYTE), rf);

		sector =   ((((p >> 12) + (p & 0x0fff ? 1 : 0)) & 0xff) << 24)
				 | ((a & 0xff) << 16) | (a & 0xff00) | ((a & 0xff0000) >> 16);

		fseek(rf, o, SEEK_SET);
		fwrite(&sector, 1, sizeof(sector), rf);
	}

	free(buf);
}

// # ===========================================================================

void print_named_tag(FILE *, const NBT *, int);
void print_unnamed_tag(FILE *, const void *, BYTE, int);
void indent(FILE *f, int l) { putc('\n', f); while(l--) putc('\t', f); }

void print_BYTE_ARRAY(FILE *f, const NBT_Array *a)
{
	fprintf(f, "b/%d [", a->i);

	int i, first = 1;
	for(i = 0 ; i < a->i ; ++i)
	{
		if(!first) fprintf(f, ", ");
		fprintf(f, "0x%02X", (uint) ((const BYTE *) a->data)[i]);
		first = 0;
	}

	fprintf(f, "]");
}

void print_INT_ARRAY(FILE *f, const NBT_Array *a)
{
	fprintf(f, "i/%d [", a->i);

	int i, first = 1;
	for(i = 0 ; i < a->i ; ++i)
	{
		if(!first) fprintf(f, ", ");
		fprintf(f, "0x%08X", (uint) ((const DWORD *) a->data)[i]);
		first = 0;
	}

	fprintf(f, "]");
}

void print_LIST(FILE *f, const NBT_List *list, int l)
{
	fprintf(f, "[%x]", list->type);
	indent(f, l++);
	fprintf(f, "[");

	int i, first = 1;
	for(i = 0 ; i < list->i ; ++i)
	{
		if(!first) fprintf(f, ",");
		indent(f, l);
		print_unnamed_tag(f, list->data[i], list->type, l);
		first = 0;
	}

	indent(f, --l);
	fprintf(f, "]");
}

void print_COMPOUND(FILE *f, const NBT_Compound *tag, int l)
{
	indent(f, l++);
	fprintf(f, "{");

	int i, first = 1;
	for(i = 0 ; i < tag->i ; ++i)
	{
		if(!first) fprintf(f, ",");
		indent(f, l);
		print_named_tag(f, tag->tags[i], l);
		first = 0;
	}

	indent(f, --l);
	fprintf(f, "}");
}

void print_unnamed_tag(FILE *f, const void *d, BYTE t, int l)
{
	switch(t)
	{
		case NBT_BYTE:
			fprintf(f, "0x%02X (%d)", (uint) *((const BYTE *) d), (int) *((const BYTE *) d));
			break;
		case NBT_SHORT:
			fprintf(f, "0x%04X (%d)", (uint) *((const WORD *) d), (int) *((const WORD *) d));
			break;
		case NBT_INT:
			fprintf(f, "0x%08X (%d)", (uint) *((const DWORD *) d), (int) *((const DWORD *) d));
			break;
		case NBT_LONG:
			fprintf(f, "0x%016lX (%ld)", (uint64_t) *((const QWORD *) d), (int64_t) *((const QWORD *) d));
			break;
		case NBT_FLOAT:
			fprintf(f, "%ff", *((const float *) d));
			break;
		case NBT_DOUBLE:
			fprintf(f, "%lfd", *((const double *) d));
			break;
		case NBT_STRING:
			fprintf(f, "'%s'", (const char *) d);
			break;
		case NBT_BYTE_ARRAY:
			print_BYTE_ARRAY(f, (const NBT_Array *) d);
			break;
		case NBT_INT_ARRAY:
			print_INT_ARRAY(f, (const NBT_Array *) d);
			break;
		case NBT_LIST:
			print_LIST(f, (const NBT_List *) d, l);
			break;
		case NBT_COMPOUND:
			print_COMPOUND(f, (const NBT_Compound *) d, l);
			break;
		default: assert(!"Unknown tag type!");
	}
}

void print_named_tag(FILE *f, const NBT *t, int l)
{
	fprintf(f, "[%x] '%s' : ", (int) t->type, t->name);
	print_unnamed_tag(f, t->data, t->type, l);
}

void nbt_print(const NBT *tag, FILE *f)
{
	print_named_tag(f, tag, 0);
	putc('\n', f);
}

