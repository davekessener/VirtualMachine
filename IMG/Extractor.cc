#include "Extractor.h"

namespace IMG {
int SGL = 0;
}

int Extractor::run(int argc, char *argv[])
{
	std::cout << '\'' << argv[0] << "' runs." << std::endl;

	typedef Map<>::KEY UID;

	Map<Map<IMG::rgba_image>> bank_map;
	Map<IMG::rgba_image> main_set;
	Vector<UID> uids;
	std::fstream ROM("pkmn.gba", std::ios::in | std::ios::binary);

	for(int bank_no = 1 ; bank_no <= 10 ; bank_no++)
	{
		for(int map_no = 0 ; map_no < 40 ; map_no++)
		{
			IMG::OFFSET o = GetMapHeaderOffset(ROM, bank_no, map_no);

			if(o == INVALID_OFFSET) break;
			if(uids.hasValue((UID) o)) continue;

			uids.push((UID) o);

			IMG::MAP map;

			InitializeMap(ROM, o, &map);

			UID keys[2] = 
				{(UID) map.TileSets[0].LZ77ImageOffset,
				 (UID) map.TileSets[1].LZ77ImageOffset };

			if(bank_map.hasKey(keys[0]))
			{
				if(bank_map[keys[0]].hasKey(keys[1]))
				{
//					char buf[20];
//					sprintf(buf, "0x%06X", keys[0]);
//					sprintf(buf + 10, "0x%06X", keys[1]);
//					std::cout << "@> Already read image w/IDs [" 
//						<< buf << ", " 
//						<< (buf + 10) << "]" << std::endl;
					ReleaseMap(&map);
					continue;
				}
				else
				{
//					bank_map[keys[0]].push(keys[1]);
				}
			}
			else
			{
				Map<IMG::rgba_image> *m = new Map<IMG::rgba_image>;
//				Vector<UID> *v = new Vector<UID>;
//				v->push(keys[1]);
				bank_map.insert(keys[0], m);
			}

			IMG::rgba_image *img = generate_tilemap(map);

			if(!main_set.hasKey(keys[0]))
			{
				IMG::rgba_image *tm = new IMG::rgba_image(16, map.TileSets[0].TileCount * 16);
				tm->bitBlt(0, 0, tm->Width(), tm->Height(), *img, 0, 0);
				main_set.insert(keys[0], tm);
			}

			img->trunc(0, map.TileSets[0].TileCount * 16);

			bank_map[keys[0]].insert(keys[1], img);

//			write_to_disk(map);
			std::cout << "@> Map '" << get_map_id(map) << "' read." << std::endl;
			
			ReleaseMap(&map);
		}
	}

	ROM.close();

	std::cout << std::endl << std::endl << "######################################################################" << std::endl;

	IMG::SGL = 1;

#define WDTH 8
	IMG::rgba_image tmpI(16 * WDTH, 16);
	Vector<UID> v = bank_map.getKeySet();

	for(int i = 0 ; i < v.Length() ; i++)
	{
		char buf[20];
		sprintf(buf, "tileset_%06X.bmp", v[i]);

		IMG::flow_image<IMG::RGBA> outT(buf, 16 * WDTH);

		Queue<IMG::rgba_image> q;
		q.push(&main_set[v[i]]);

		Vector<UID> w = bank_map[v[i]].getKeySet();
		for(int j = 0 ; j < w.Length() ; j++)
		{
			q.push(&bank_map[v[i]][w[j]]);
		}

		while(!q.isEmpty())
		{
			IMG::rgba_image *img = q.poll();

			printf("Point received: %p\n", img);
			printf("Processing image of size %i*%i\n", img->Width(), img->Height());

			for(int d = 0 ; d * 16 < img->Height() ;)
			{
				tmpI.clear();

				for(int j = 0 ; j < WDTH ; j++)
				{
					tmpI.bitBlt(j * 16, 0, 16, 16, *img, 0, d * 16);
					if(++d * 16 == img->Height()) break;
				}

				outT.draw(tmpI);
			}

			delete img;

			printf("[DONE!]\n");
		}

		outT.close();
	}

	return 0;
}

IMG::rgba_image *Extractor::generate_tilemap(IMG::MAP& map)
{
	IMG::palette_image TileSet(IMG::palette_image::LZ77(map.LZ77Image, map.LZ77ImageSize));
	IMG::rgba_image *TileMap = new IMG::rgba_image(16, map.TileCount * 16);
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
			TileMap->bitBlt((j & 1) * 8, ((j & 2) ? 8 : 0) + i * 16, 8, 8, (IMG::rgba_image) Tile, 0, 0, 
				map.Pallets[map.Tiles[i].PalletNr[j]].CR_Colors[0], j >= 4);
		}
	}

	return TileMap;
}

void Extractor::write_to_disk(const std::string& fn, const IMG::rgba_image& TileMap)
{
	std::fstream f(fn, std::ios::out | std::ios::binary);

	int tc = TileMap.Height() / 16, w = 8;
	IMG::rgba_image out(w * 16, (tc + w - 1) / w * 16);

	for(int i = 0 ; i < tc ; i++)
	{
		out.bitBlt((i % w) * 16, (i / w) * 16, 16, 16, TileMap, 0, i * 16);
	}

	out.toFile(f);

	f.close();

	std::cout << "Successfully extracted tilemap '" << fn << "'." << std::endl;
}

std::string Extractor::get_map_id(const IMG::MAP& map)
{
	std::stringstream fn;
	fn 	<< "tilemap_" << std::internal << std::setfill('0') << std::uppercase << std::hex 
		<< std::setw(6) << map.TileSets[0].LZ77ImageOffset
		<< "_" << std::setw(6) << map.TileSets[1].LZ77ImageOffset << ".bmp";
	return fn.str();
}

