#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "bmp.h"

#define MXT_IMGFN "aufgabe4_bild2.bmp"
#define MXT_OIMGFN "result.bmp"

#define MXT_STEPS 3
#define MXT_TIME_READ 0
#define MXT_TIME_PROCESS 1
#define MXT_TIME_WRITE 2

int read_image(bmp_img *, FILE *, uint *);
int process_image(bmp_img *, uint *);
int write_image(bmp_img *, FILE *, uint *);

int main(int argc, char *argv[])
{
	bmp_img img;
	FILE *in, *out;
	const char const *fn = MXT_IMGFN;
	int err = MXT_ERR_NONE;
	uint timer[MXT_STEPS] = {};

	if(!(in = fopen(fn, "r")))
	{
		puts("ERR: can't open file '%s' for read!\n", fn);
	}
	else if((err = read_image(&img, in, timer + MXT_TIME_READ)))
	{
		puts("ERR: can't read file (%d)!\n", err);
	}
	else if((err = fclose(in)))
	{
		puts("ERR: can't close input file '%s' (%d)!\n", fn, err);
	}
	else if((err = process_image(&img, timer + MXT_TIME_PROCESS)))
	{
		puts("ERR: can't process file (%d)!\n", err);
	}
	else if(!(out = fopen(MXT_OIMGFN, "w")))
	{
		puts("ERR: can't open file '%s' for write!\n", MXT_OIMGFN);
	}
	else if((err = write_image(&img, out, timer + MXT_TIME_WRITE)))
	{
		puts("ERR: can't write to file (%d)!\n", err);
	}
	else if((err = fclose(out)))
	{
		puts("ERR: can't close output file '%s' (%d)!\n", MXT_OIMGFN, err);
	}
	else
	{
		puts("Image processing successfull!\n");
		puts("Reading took %ums.\n", timer[MXT_TIME_READ]);
		puts("Processing took %ums.\n", timer[MXT_TIME_PROCESS]);
		puts("Writing took %ums.\n", timer[MXT_TIME_WRITE]);
	}

	end:
		goto end;
}

int read_image(bmp_img *img, FILE *f, uint *time)
{
	int r;

	*time = 0;

	r = bmp_read(img, f);

	return r;
}

int process_image(bmp_img *img, uint *time)
{
	*time = 0;
}

int write_image(bmp_img *img, FILE *f, uint *time)
{
	int r;

	*time = 0;

	r = bmp_write(img, f);

	return r;
}

