#include "LZ77.h"

using namespace IMG;

int DecompressLZ77(FILE *in, FILE *out)
{
	DWORD l = 0;
	BYTE *buf, h;
	int p, i;

	if(getc(in) != 0x10) return 0;

	fread(&l, 1, 3, in);
	buf = new BYTE[l];

#ifdef DEBUG
	std::cout << "Reading LZ77(" << l << ") ...";
#endif

	p = 0;
	while(p < l)
	{
		h = getc(in);

		for(i = 7 ; i >= 0 ; i--)
		{
			if(p >= l) break;

			if(h & (1 << i))
			{
				WORD raw, back;
				BYTE len;

//				fread(&raw, 1, sizeof(WORD), in);
//				raw = getc(in) * 0x100 + getc(in);
				raw = getc(in) << 8;
				raw += getc(in);

				len = ((raw & 0xf000) >> 12) + 3;
				back = (raw & 0x0fff) + 1;

				while(len--)
				{
#ifdef DEBUG
					if(back > p) printf("ERR: Read only %i bytes but lookback is %i.\n", p, back);
#endif
					buf[p] = buf[p - back];

					if(++p >= l) break;
				}
			}
			else
			{
				buf[p++] = getc(in);
			}
		}
	}

	if(out != NULL)
	{
		fwrite(buf, l, 1, out);
	}

	delete[] buf;

#ifdef DEBUG
	std::cout << "[DONE!]" << std::endl;
#endif

	return l;
}

