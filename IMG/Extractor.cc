#include "Extractor.h"

int Extractor::run(int argc, char *argv[])
{
	std::cout << '\'' << argv[0] << "' runs." << std::endl;

	Map<Vector<Map<>::KEY>> bank_map;
	std::fstream ROM("pkmn.gba", std::ios::in | std::ios::binary);

	for(int bank_no = 1 ; bank_no <= 4 ; bank_no++)
	{
		for(int map_no = 0 ; map_no < 10 ; map_no++)
		{
			IMG::OFFSET o = GetMapHeaderOffset(ROM, bank_no, map_no);

			if(o == INVALID_OFFSET) break;

			IMG::MAP map;

			InitializeMap(ROM, o, &map);

			Map<>::KEY keys[2] = 
				{(Map<>::KEY) map.TileSets[0].LZ77ImageOffset,
				 (Map<>::KEY) map.TileSets[1].LZ77ImageOffset };

			if(bank_map.hasKey(keys[0]))
			{
				if(bank_map[keys[0]].hasValue(keys[1]))
				{
					ReleaseMap(&map);
					continue;
				}
				else
				{
					bank_map[keys[0]].push(keys[1]);
				}
			}
			else
			{
				Vector<Map<>::KEY> *v = new Vector<Map<>::KEY>;
				v->push(keys[1]);
				bank_map.insert(keys[0], v);
			}

			write_to_disk(map);
			
			ReleaseMap(&map);
		}
	}

	ROM.close();

	return 0;
}

void Extractor::write_to_disk(IMG::MAP& map)
{
	std::stringstream fn;
	fn 	<< "tilemap_" << std::internal << std::setfill('0') << std::uppercase << std::hex 
		<< std::setw(6) << map.TileSets[0].LZ77ImageOffset
		<< "_" << std::setw(6) << map.TileSets[1].LZ77ImageOffset << ".bmp";

	std::fstream f(fn.str(), std::ios::out | std::ios::binary);

	IMG::palette_image TileSet(IMG::palette_image::LZ77(map.LZ77Image, map.LZ77ImageSize));
	IMG::rgba_image TileMap(16, map.TileCount * 16);
	IMG::Palette palette(13 * 16);

	for(int i = 0 ; i < 13 ; i++)
	{
		palette.setColor(map.Pallets[i].CR_Colors, i * 16, 16);
	}

	for(int i = 0 ; i < map.TileCount ; i++)
	{
		for(int j = 0 ; j < 8 ; j++)
		{
			IMG::palette_image Tile(8, 8);
			Tile.setPalette(IMG::Palette(map.Pallets[map.Tiles[i].PalletNr[j]].CR_Colors, 16));
			Tile.bitBlt(	0,
							0,
							8 * ((map.Tiles[i].Reflection[j] & 1) ? -1 : 1),
							8 * ((map.Tiles[i].Reflection[j] & 2) ? -1 : 1),
							TileSet,
							0,
							map.Tiles[i].BlockNr[j] * 8);
			TileMap.bitBlt((j & 1) * 8, ((j & 2) ? 8 : 0) + i * 16, 8, 8, (IMG::rgba_image) Tile, 0, 0, 
				map.Pallets[map.Tiles[i].PalletNr[j]].CR_Colors[0], j >= 4);
		}
	}

	int tc = TileMap.Height() / 16, w = 8;
	IMG::rgba_image out(w * 16, (tc + w - 1) / w * 16);

	for(int i = 0 ; i < tc ; i++)
	{
		out.bitBlt((i % w) * 16, (i / w) * 16, 16, 16, TileMap, 0, i * 16);
	}

	out.toFile(f);

	f.close();

	std::cout << "Successfully extracted tilemap '" << fn.str() << "'." << std::endl;
}

