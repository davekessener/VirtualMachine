#include "Extractor.h"

int Extractor::run(int argc, char *argv[])
{
	std::cout << '\'' << argv[0] << "' runs." << std::endl;

	FILE *f = fopen("pkmn.gba", "rb");
	IMG::OFFSET o = GetMapHeaderOffset(f, 4, 0);
	IMG::MAP map;

	InitializeMap(f, o, &map);

	fclose(f);
	f = fopen("test.bmp", "wb");

	IMG::Image TileSet(IMG::Image::LZ77(map.LZ77Image, map.LZ77ImageSize));
	IMG::Image TileMap(16, map.TileCount * 16);
	IMG::Palette palette(13 * 16);

	for(int i = 0 ; i < 13 ; i++)
	{
		palette.setColor(map.Pallets[i].CR_Colors, i * 16, 16);
	}

	for(int i = 0 ; i < map.TileCount ; i++)
	{
		for(int j = 0 ; j < 8 ; j++)
		{
			IMG::Image Tile(8, 8);
			Tile.bitBlt(	0,
							0,
							8 * ((map.Tiles[i].Reflection[j] & 1) ? -1 : 1),
							8 * ((map.Tiles[i].Reflection[j] & 2) ? -1 : 1),
							TileSet,
							0,
							map.Tiles[i].BlockNr[j] * 8,
							j >= 4 ? 0 : -1);
			Tile.offsetBy(map.Tiles[i].PalletNr[j]);
			TileMap.bitBlt((j & 1) * 8, ((j & 2) ? 8 : 0) + i * 16, 8, 8, Tile, 0, 0, (map.Tiles[i].PalletNr[j] & 0x0f) << 4);
		}
	}

	int tc = TileMap.getHeight() / 16, w = 8;
	IMG::Image out(w * 16, (tc + w - 1) / w * 16);

	for(int i = 0 ; i < tc ; i++)
	{
		out.bitBlt((i % w) * 16, (i / w) * 16, 16, 16, TileMap, 0, i * 16);
	}

	out.setPalette(palette).toFile(f);

	ReleaseMap(&map);

	fclose(f);

	return 0;
}

