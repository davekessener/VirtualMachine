#ifndef DAV_GSP_2_BMPREAD_H
#define DAV_GSP_2_BMPREAD_H

#include <stdio.h>
#include "common.h"
#include "bmp.h"

#define BMP_ERR_NO 0
#define BMP_ERR_TOOSHORT 1
#define BMP_ERR_INVALID 2
#define BMP_ERR_NOTSUPPORTED 3
#define BMP_ERR_DECOMPRESSION 4

int bmp_read(FILE *, BITMAPINFOHEADER *, RGBA **);

#endif

