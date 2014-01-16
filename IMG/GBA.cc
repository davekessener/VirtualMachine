#include "GBA.h"

using namespace IMG;

void inline LOADFROM(FILE *f, void *d, size_t c, int o)
{
	fseek(f, o, SEEK_SET);
	fread(d, 1, c, f);
}

void inline SET(FILE *f, int o)
{
	fseek(f, o, SEEK_SET);
}

bool InitializeMap(FILE *ROM, OFFSET Offset, _map *Map)
{
#ifdef DEBUG
	std::cout << "Entered map initialization routine." << std::endl;
#endif

	Map->Offset = Offset;
//	Map->Alpha = 40;
	Map->ROM = ROM;

	//  #=============================#
	//  #---------- Header -----------#
	//  #=============================#
	
	//  #-- Load raw data
	BYTE HeaderData[28];
	LOADFROM(ROM, HeaderData, 28, Offset);

	//  #-- Convert data
	struct _map::_header &Header	= Map->Header;
	Header.FooderOffset				= TOOFFSET(HeaderData + 0 * 4);
	Header.EventsOffset				= TOOFFSET(HeaderData + 1 * 4);
	Header.MapscriptOffset			= TOOFFSET(HeaderData + 2 * 4);
	Header.ConnectionsOffset		= TOOFFSET(HeaderData + 3 * 4);
	Header.Music					= *((WORD *) (HeaderData + 4 * 4));
	Header.Name.Index				= HeaderData[20];
	Header.CaveType					= HeaderData[21];
	Header.WeatherType				= HeaderData[22];
	Header.MapType					= HeaderData[23];
	Header.BattleType				= HeaderData[27];
	Header.Name.OffsetOffset		= NamesOffset + 4 * (Header.Name.Index - 0x58);
	LOADFROM(ROM, &Header.Name.NameOffset, 4, Header.Name.OffsetOffset);
	Header.Name.NameOffset			= TOOFFSET((BYTE *) &Header.Name.NameOffset);
	LOADFROM(ROM, &Header.Name.Buffer, 20, Header.Name.NameOffset);
	ConvRSStoANSI(Header.Name.Buffer, Header.Name.Value, 20);

#ifdef DEBUG
	std::cout << "Read Header successfully!" << std::endl;
#endif


	//  #=============================#
	//  #---------- Fooder -----------#
	//  #=============================#
	
	//  #-- Load raw data
	BYTE FooderData[28];
	LOADFROM(ROM, FooderData, 28, Map->Header.FooderOffset);

	//  #-- Convert data
	struct _map::_fooder &Fooder	= Map->Fooder;
	Fooder.Width					= TODWORD (FooderData + 0 * 4);
	Fooder.Height					= TODWORD (FooderData + 1 * 4);
	Fooder.BorderOffset				= TOOFFSET(FooderData + 2 * 4);
	Fooder.MapOffset				= TOOFFSET(FooderData + 3 * 4);
	Fooder.TileSetOffset[0]			= TOOFFSET(FooderData + 4 * 4);
	Fooder.TileSetOffset[1]			= TOOFFSET(FooderData + 5 * 4);
	Fooder.BorderWidth				= FooderData[6 * 4];
	Fooder.BorderHeight				= FooderData[6 * 4 + 1];

#ifdef DEBUG
	std::cout << "Read Fooder successfully!" << std::endl;
#endif

	//  #=============================#
	//  #-------- TileSets -----------#
	//  #=============================#

	for(int i = 0 ; i < 2 ; i++)
	{
		//  #-- Initialize TileSet
		BYTE TileSetData[24];
		LOADFROM(ROM, TileSetData, 24, Map->Fooder.TileSetOffset[i]);

		//  #-- Convert into TileSet
		struct _map::_tile_set &TileSet		= Map->TileSets[i];
		TileSet.LZ77ImageOffset				= TOOFFSET(TileSetData + 1 * 4);
		TileSet.PalletsOffset				= TOOFFSET(TileSetData + 2 * 4);
		TileSet.TilesOffset					= TOOFFSET(TileSetData + 3 * 4);
		TileSet.BehaviorBackgroundOffset	= TOOFFSET(TileSetData + 5 * 4);

		//  #-- Load pallets
		for(int j = 7 * i ; j < 6 * i + 7 ; j++)
		{
			WORD Buffer[16];
			LOADFROM(ROM, Buffer, 32, TileSet.PalletsOffset + j * 32);
			ConvertPallet(&Map->Pallets[j], Buffer);
		}
	}

#ifdef DEBUG
	std::cout << "Read TileSets successfully!" << std::endl;
#endif


	//  #==================================#
	//  #--------- Load MapOrder ----------#
	//  #==================================#

	//  #-- Load raw data & initializa space
	DWORD MapSize = Map->Fooder.Width * Map->Fooder.Height;
	BYTE *Buffer  = (BYTE *) malloc(MapSize * 2);
	Map->MapOrder = (WORD *) malloc(MapSize * 2);
	Map->WalkData = (BYTE *) malloc(MapSize);
	LOADFROM(ROM, Buffer, MapSize * 2, Map->Fooder.MapOffset);

	//  #-- Convert MapOrder & WalkData
	for(int i = 0 ; i < MapSize ; i++)
	{
		Map->MapOrder[i] = ((Buffer[i * 2 + 1] % 4) << 8) + Buffer[i * 2];
		Map->WalkData[i] = Buffer[i * 2 + 1] / 4;
	}

	free(Buffer);

#ifdef DEBUG
	std::cout << "Read OrderData successfully!" << std::endl;
#endif


	//  #==================================#
	//  #------- Load LZ77 Images ---------#
	//  #==================================#

	//  #-- Open tmp-files
	FILE *tmp1   = fopen("LZ77_1.tmp", "w+b");
	FILE *tmp2   = fopen("LZ77_2.tmp", "w+b");
	DWORD Length1;
	DWORD Length2;

	//  #-- Decompress LZ77 images
	SET(ROM, Map->TileSets[0].LZ77ImageOffset);
	Length1 = DecompressLZ77(ROM, tmp1);
	SET(ROM, Map->TileSets[1].LZ77ImageOffset);
	Length2 = DecompressLZ77(ROM, tmp2);

	//  #-- Load LZ77 images into RAM
	Buffer = (BYTE *) malloc(Length1 + Length2);
	LOADFROM(tmp1, Buffer, Length1, 0);
	LOADFROM(tmp2, Buffer + Length1, Length2, 0);
	fclose(tmp1);
	fclose(tmp2);
//	remove("LZ77_1.tmp");
//	remove("LZ77_2.tmp");

	Map->LZ77Image = Buffer;
	Map->LZ77ImageSize = Length1 + Length2;
	Buffer = NULL;

#ifdef DEBUG
	std::cout << "Read LZ77 successfully!" << std::endl;
#endif


	//  #==================================#
	//  #--------- Create tiles -----------#
	//  #==================================#

	//  #-- Initialize tiles
	DWORD TileCount1 = Map->TileSets[0].TileCount = (Map->TileSets[0].BehaviorBackgroundOffset - Map->TileSets[0].TilesOffset) / 0x10;
	DWORD TileCount2 = Map->TileSets[1].TileCount = (Map->TileSets[1].BehaviorBackgroundOffset - Map->TileSets[1].TilesOffset) / 0x10;
	Map->TileCount = TileCount1 + TileCount2;
	Map->Tiles = (_map::_tile *) malloc(Map->TileCount * sizeof(_map::_tile));

	//  #-- Load raw data
	Buffer = (BYTE *) malloc(Map->TileCount * 0x10);
	LOADFROM(ROM, Buffer, TileCount1 * 0x10, Map->TileSets[0].TilesOffset);
	LOADFROM(ROM, Buffer + TileCount1 * 0x10, TileCount2 * 0x10, Map->TileSets[1].TilesOffset);

	//  #-- Convert into tiles
	for(int i = 0 ; i < Map->TileCount ; i++)
	{
		struct _map::_tile &Tile = Map->Tiles[i];
		memcpy(Tile.raw, Buffer + i * 0x10, 0x10);
		for(int j = 0 ; j < 8 ; j++)
		{
			Tile.BlockNr[j]		= (((Tile.raw[j * 2 + 1] % 0x10) % 0x04) << 8) + Tile.raw[j * 2];
			Tile.PalletNr[j]	= Tile.raw[j * 2 + 1] / 0x10;
			Tile.Reflection[j]	= (Tile.raw[j * 2 + 1] % 0x10) / 0x04;
		}
	}
	free(Buffer);

#ifdef DEBUG
	std::cout << "Read Tiles successfully!" << std::endl;
#endif

	return true;
}

//bool InitializeMap(char *FileName, OFFSET Offset, _map *Map, HINSTANCE hInst, HWND WindowHandle)
//{
//	FILE *ROM = fopen(FileName, "rb");
//	if(ROM == NULL)
//		return false;
//
//	bool RetVal = InitializeMap(ROM, Offset, Map, hInst, WindowHandle);
//	fclose(ROM);
//
//	return RetVal;
//}

OFFSET GetMapHeaderOffset(FILE *GBA, int iBank, int iMap)
{
	if(!GBA)
		return INVALID_OFFSET;

	OFFSET oMaps;
	LOADFROM(GBA, &oMaps, 4, BankOffset + 4 * iBank);
	oMaps = TOOFFSET(&oMaps);

	OFFSET oMap;
	LOADFROM(GBA, &oMap, 4, oMaps + 4 * iMap);
	oMap = TOOFFSET(&oMap);

	return oMap;
}

void ReleaseMap(_map *Map)
{
	free(Map->LZ77Image);
	free(Map->Tiles);
//	DeleteObject(Map->TileSet);
//	DeleteObject(Map->Map);
//	DeleteObject(Map->WalkMap);
//	DeleteObject(Map->WalkPallet);
//	for(int i = 0 ; i < 5 ; i ++)
//		DeleteDC(Map->WorkDCs[i]);
//	for(int i = 0 ; i < 13 ; i++)
//		DeleteObject(Map->RawImages[i]);
}

//void WriteBack(_map *Map)
//{
//	FILE *(&ROM) = Map->ROM;
//	SAVEFROM(ROM, Map->Header_raw, 28, Map->Offset);
//	SAVEFROM(ROM, Map->Fooder_raw, 28, Map->Header.FooderOffset);
//	for(int i = 0 ; i < 2 ; i++)
//	{
//		SAVEFROM(ROM, Map->TileSets[i].raw, 24, Map->Fooder.TileSetOffset[i]);
//		for(int j = 7 * i ; j < 6 * i + 7 ; j++)
//			SAVEFROM(ROM, Map->Pallets[j].Colors_raw, 32, Map->TileSets[i].PalletsOffset + j * 32);
//		for(int j = 0 ; j < Map->TileSets[i].TileCount ; j++)
//			SAVEFROM(ROM, Map->Tiles[j + i * Map->TileSets[0].TileCount].raw, 16, Map->TileSets[i].TilesOffset + j * 16);
//	}
//	WORD Length = Map->Fooder.Width * Map->Fooder.Height;
//	BYTE *Buffer = (BYTE *) malloc(Length * sizeof(WORD));
//	for(int i = 0 ; i < Length ; i++)
//	{
//		Buffer[i * 2] = (Map->MapOrder[i] & 0xFF);
//		Buffer[i * 2 + 1] = (Map->MapOrder[i] >> 8) | (Map->WalkData[i] << 2);
//	}
//	SAVEFROM(ROM, Buffer, Length * sizeof(WORD), Map->Fooder.MapOffset);
//	free(Buffer);
//}

//void CreateBitmaps(_map *Map, HWND hwnd)
//{
//	int Width  = 16,
//		Height = FILLUP(Map->LZ77ImageSize / 64 * 2, Width) / Width;
//	HDC hdc    = GetDC(hwnd);
//	HBITMAP hb;
//
//	Map->WindowHandle = hwnd;
//	
//	for(int i = 0 ; i < 13 ; i++)
//		Map->RawImages[i] = CreateCompatibleBitmap(hdc, Width * 8, Height * 8);
//
//	for(int i = 0 ; i < 5 ; i++)
//		Map->WorkDCs[i] = CreateCompatibleDC(hdc);
//	
//	Width			= 8;
//	Height			= FILLUP(Map->TileCount, Width) / Width;
//	Map->TileSet	= CreateCompatibleBitmap(hdc, Width * 16, Height * 16);
//
//	Width			 = Map->Fooder.Width;
//	Height			 = Map->Fooder.Height;
//	Map->Map		 = CreateCompatibleBitmap(hdc, Width * 16, Height * 16);
//	Map->WalkMap	 = CreateCompatibleBitmap(hdc, Width * 16, Height * 16);
//	Map->WalkEditMap = CreateCompatibleBitmap(hdc, Width * 16, Height * 16);
//
//	ReleaseDC(hwnd, hdc);
//}
//
//void PrepareTileSet(_map *Map)
//{
//	for(int i = 0 ; i < FILLUP(Map->TileCount, 8) ; i++)
//		CreateTile(Map, i);
//}
//
//void DrawMap(_map *Map)
//{
//	HDC &TileMap	= Map->WorkDCs[0],
//		&TileSet	= Map->WorkDCs[1];
//	HBITMAP Unused1, Unused2;
//	WORD *Order		= Map->MapOrder;
//
//	Unused1 = (HBITMAP) SelectObject(TileMap, Map->Map);
//	Unused2 = (HBITMAP) SelectObject(TileSet, Map->TileSet);
//
//	for(int i = 0 ; i < Map->Fooder.Width * Map->Fooder.Height ; i++)
//		BitBlt(TileMap, (i % Map->Fooder.Width) * 16, (i / Map->Fooder.Width) * 16, 16, 16,
//			   TileSet, (Order[i] % 8) * 16, (Order[i] / 8) * 16, SRCCOPY);
//	
//	SelectObject(TileMap, Unused1);
//	SelectObject(TileSet, Unused2);
//}
//
//void DrawWalkMap(_map *Map, int no)
//{
//	HDC &WalkMap		= Map->WorkDCs[0],
//		&FinishedMap	= Map->WorkDCs[1],
//		&MapMap			= Map->WorkDCs[2],
//		hdc				= GetDC(Map->WindowHandle),
//		WalkPallet		= CreateCompatibleDC(hdc);
//	HBITMAP Unused1, Unused2, Unused3, Unused4;
//
//	Unused1 = (HBITMAP) SelectObject(WalkMap,		Map->WalkMap);
//	Unused2 = (HBITMAP) SelectObject(WalkPallet,	Map->WalkPallet);
//	Unused3 = (HBITMAP) SelectObject(FinishedMap,	Map->WalkEditMap);
//	Unused4 = (HBITMAP) SelectObject(MapMap,		Map->Map);
//	ReleaseDC(Map->WindowHandle, hdc);
//
//	for(int i = 0 ; i < Map->Fooder.Width * Map->Fooder.Height ; i++)
//		BitBlt(WalkMap, (i % Map->Fooder.Width) * 16, (i / Map->Fooder.Width) * 16, 16, 16,
//			   WalkPallet, 0, Map->WalkData[i] * 16, SRCCOPY);
//
//	if(no < 0)
//		DrawDemiTransparentMap(MapMap, WalkMap, FinishedMap, 0, 0, Map->Fooder.Width * 16, Map->Fooder.Height * 16, Map->Alpha);
//	else
//		DrawDemiTransparentMap(MapMap, WalkMap, FinishedMap, (no % Map->Fooder.Width) * 16, (no / Map->Fooder.Width) * 16, 16, 16, Map->Alpha);
//
//	SelectObject(WalkMap,		Unused1);
//	SelectObject(WalkPallet,	Unused2);
//	SelectObject(FinishedMap,	Unused3);
//	SelectObject(MapMap,		Unused4);
//	DeleteDC(WalkPallet);
//}
//
//void DrawDemiTransparentMap(HDC hdcSRC1, HDC hdcSRC2, HDC hdcDest, int x0, int y0, int Width, int Height, int Alpha)
//{
//	for(int y = 0 ; y < Height ; y++)
//		for(int x = 0 ; x < Width ; x++)
//		{
//			COLORREF Color = GetPixel(hdcSRC1, x + x0, y + y0);
//			BYTE Red = GetRValue(Color),
//				 Green = GetGValue(Color), 
//				 Blue = GetBValue(Color);
//			Color = GetPixel(hdcSRC2, x + x0, y + y0);
//			Red = (Red * Alpha + GetRValue(Color) * (100 - Alpha)) / 100;
//			Green = (Green * Alpha + GetGValue(Color) * (100 - Alpha)) / 100;
//			Blue = (Blue * Alpha + GetBValue(Color) * (100 - Alpha)) / 100;
//			Color = RGB(Red, Green, Blue);
//			SetPixel(hdcDest, x + x0, y + y0, Color);
//		}
//}
//
//void SetAlpha(_map *Map, int newAlpha) 
//{ 
//	Map->Alpha = newAlpha; 
//	DrawWalkMap(Map);
//}
//
//WORD SetTileOnMap(_map *Map, int iTileNr, WORD wNewTile, bool Redraw)
//{
//	WORD wOldTile = Map->MapOrder[iTileNr];
//	Map->MapOrder[iTileNr] = wNewTile;
//
//	if(Redraw)
//		DrawMap(Map);
//
//	return wOldTile;
//}
//
//WORD GetTileFromMap(_map *Map, int iTileNr)
//{
//	WORD wTile = Map->MapOrder[iTileNr];
//	return wTile;
//}
//
//BYTE SetWalkTileOnMap(_map *Map, int TileNr, BYTE NewValue, bool Redraw)
//{
//	BYTE byOldTile = Map->WalkData[TileNr];
//	Map->WalkData[TileNr] = NewValue;
//
//	if(Redraw)
//		DrawWalkMap(Map, TileNr);
//
//	return byOldTile;
//}
//
//BYTE GetWalkTileFromMap(_map *Map, int iTileNr)
//{
//	BYTE byTile = Map->WalkData[iTileNr];
//	return byTile;
//}
//
//_tile_simple SetTile(_map *Map, int TileNr, int BlockNr, WORD wNewBlock, BYTE bNewPallet, BYTE bNewReflection)
//{
//	_map::_tile &Tile = Map->Tiles[TileNr];
//	_tile_simple RetTile;
//	memcpy(&RetTile, Tile.raw + 16, 32);
//
//	RetTile.BlockNr[BlockNr] = wNewBlock;
//	RetTile.PalletNr[BlockNr] = bNewPallet;
//	RetTile.Reflection[BlockNr] = bNewReflection;
//
//	return SetTile(Map, TileNr, &RetTile);
//}
//
//_tile_simple SetTile(_map *Map, int TileNr, _tile_simple *NewTile)
//{
//	_map::_tile &Tile = Map->Tiles[TileNr];
//	_tile_simple RetTile;
//	memcpy(&RetTile, ((BYTE *) Tile.raw) + 16, 32);
//
//	memcpy(((BYTE *) Tile.raw) + 16, NewTile, 32);
//
//	for(int i = 0 ; i < 8 ; i++)
//	{
//		Tile.raw[i * 2] = (Tile.BlockNr[i] & 0xFF);
//		Tile.raw[i * 2 + 1] = ((Tile.PalletNr[i] << 4) | (Tile.Reflection[i] << 2) | (Tile.BlockNr[i] >> 8));
//	}
//
//	return RetTile;
//}
//
//void CreateTile(_map *Map, int TileNr)
//{
//	HDC &hdc	= Map->WorkDCs[0],
//		&TileSet= Map->WorkDCs[1],
//		&raw	= Map->WorkDCs[1],
//		&Block	= Map->WorkDCs[2],
//		hdcTmp	= GetDC(Map->WindowHandle);
//	struct _map::_tile &Tile = Map->Tiles[(TileNr < Map->TileCount ? TileNr : 0)];
//	HBITMAP TileBitmap	= CreateCompatibleBitmap(hdcTmp, 16, 16),
//			BlockBitmap	= CreateCompatibleBitmap(hdcTmp,  8,  8),
//			Unused1, Unused2, Unused3, Mask;
//
//	Unused1 = (HBITMAP) SelectObject(hdc,	TileBitmap);
//	Unused3 = (HBITMAP) SelectObject(Block,	BlockBitmap);
//	ReleaseDC(Map->WindowHandle, hdcTmp);
//
//	for(int i = 0 ; i < 8 ; i++)
//	{
//		Unused2 = (HBITMAP) SelectObject(raw, Map->RawImages[Tile.PalletNr[i]]);
//		if(i < 4)
//		{
//				StretchBlt(hdc, 
//					   8 * ((i % 2) + (Tile.Reflection[i] % 2)) - (Tile.Reflection[i] % 2), 
//					   8 * ((i > 1) + (Tile.Reflection[i] > 1)) - (Tile.Reflection[i] > 1), 
//					   8 - 16 * (Tile.Reflection[i] % 2), 
//					   8 - 16 * (Tile.Reflection[i] > 1),
//					   raw,
//					   (Tile.BlockNr[i] % 16) * 8, 
//					   (Tile.BlockNr[i] / 16) * 8,
//					   8, 8, SRCCOPY);
//		}
//		else
//		{
//			/*TransparentBlt(hdc, 
//					   8 * ((i % 2) + (Tile.Reflection[i] % 2)) - (Tile.Reflection[i] % 2), 
//					   8 * ((i > 1) + (Tile.Reflection[i] > 1)) - (Tile.Reflection[i] > 1), 
//					   8 - 16 * (Tile.Reflection[i] % 2), 
//					   8 - 16 * (Tile.Reflection[i] > 1),
//					   Raw,
//					   (Tile.BlockNr[i] % 16) * 8, 
//					   (Tile.BlockNr[i] / 16) * 8,
//					   8, 8, Map->Pallets[Tile.BlockNr[i]].CR_Colors[0]);*/
//			StretchBlt(Block, 7 * (Tile.Reflection[i] % 2),		7 * (Tile.Reflection[i] > 1),
//					   8 - 16 * (Tile.Reflection[i] % 2),		8 - 16 * (Tile.Reflection[i] > 1),
//					   raw, (Tile.BlockNr[i] % 16) * 8,			(Tile.BlockNr[i] / 16) * 8,
//					   8, 8, SRCCOPY);
//			SelectObject(Block, Unused3);
//			Mask = CreateBitmapMask(BlockBitmap, Map->Pallets[Tile.PalletNr[i]].CR_Colors[0]);
//			SelectObject(Block, Mask);
//			BitBlt(hdc, 8 * (i % 2), 8 * (i > 5), 8, 8, Block, 0, 0, SRCAND);
//			SelectObject(Block, BlockBitmap);
//			BitBlt(hdc, 8 * (i % 2), 8 * (i > 5), 8, 8, Block, 0, 0, SRCPAINT);
//			DeleteObject(Mask);
//		}
//		SelectObject(raw, Unused2);
//	}
//
//	Unused2 = (HBITMAP) SelectObject(TileSet, Map->TileSet);
//	BitBlt(TileSet, (TileNr % 8) * 16, (TileNr / 8) * 16, 16, 16, hdc, 0, 0, SRCCOPY);
//	SelectObject(TileSet, Unused2);
//
//	SelectObject(hdc, Unused1);
//	SelectObject(Block, Unused3);
//	DeleteObject(TileBitmap);
//	DeleteObject(BlockBitmap);
//}
//
//HBITMAP CreateBitmapMask(HBITMAP hbmColor, COLORREF crTransparent)
//{
//    HDC hdcMem, hdcMem2;
//    HBITMAP hbmMask;
//    BITMAP bm;
//
//    // Create monochrome (1 bit) mask bitmap.  
//
//    GetObject(hbmColor, sizeof(BITMAP), &bm);
//    hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
//
//    // Get some HDCs that are compatible with the display driver
//
//    hdcMem  = CreateCompatibleDC(0);
//    hdcMem2 = CreateCompatibleDC(0);
//
//	SelectObject(hdcMem,  hbmColor);
//    SelectObject(hdcMem2, hbmMask);
//
//    // Set the background colour of the colour image to the colour
//    // you want to be transparent.
//    SetBkColor(hdcMem, crTransparent);
//	SetBkColor(hdcMem2, crTransparent);
//
//    // Copy the bits from the colour image to the B+W mask... everything
//    // with the background colour ends up white while everythig else ends up
//    // black...Just what we wanted.
//
//    BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
//
//    // Take our new mask and use it to turn the transparent colour in our
//    // original colour image to black so the transparency effect will
//    // work right.
//    BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);
//
//    // Clean up.
//
//    DeleteDC(hdcMem);
//    DeleteDC(hdcMem2);
//
//    return hbmMask;
//}
//
//void DrawLZ77(_map *Map)
//{
//	int Width = 16;
//	int Height = FILLUP(Map->LZ77ImageSize / 64 * 2, Width) / Width;
//	BYTE *LZ77 = Map->LZ77Image;
//	HDC &RawImage = Map->WorkDCs[0];
//	HBITMAP Unused1;
//
//	for(int r = 0 ; r < 13 ; r++)
//	{
//		_map::_pallet &Pallet = Map->Pallets[r];
//		Unused1 = (HBITMAP) SelectObject(RawImage, Map->RawImages[r]);
//		LZ77 = Map->LZ77Image;
//		for(int i = 0 ; i < Height ; i++)
//			for(int j = 0 ; j < Width ; j++)
//				for(int y = 0 ; y < 8 ; y++)
//					for(int x = 0 ; x < 8 ; x++)
//					{
//						int Index = ((x % 2) ? ((*LZ77++ & 0xF0) >> 4) : (*LZ77 & 0x0F));
//						SetPixel(RawImage, j * 8 + x, i * 8 + y, Pallet.CR_Colors[Index]);
//					}
//		SelectObject(RawImage, Unused1);
//	}
//}

bool ConvertPallet(_map::_pallet *Pallet, WORD *Buffer)
{
	for(int i = 0 ; i < 16 ; i++)
	{
		Pallet->Colors_raw[i]	= Buffer[i];
		Pallet->Colors[i].Red	= GBAR(Pallet->Colors_raw[i]);
		Pallet->Colors[i].Green	= GBAG(Pallet->Colors_raw[i]);
		Pallet->Colors[i].Blue	= GBAB(Pallet->Colors_raw[i]);
		Pallet->CR_Colors[i]	= GBAtoRGB(Pallet->Colors_raw[i]);
	}

	return true;
}

bool ConvRSStoANSI(char *src, char* dest, int MaxLength)
{
	int Tmp = MaxLength;
	while(--Tmp && *(dest - 1) != '\0')
	{
		char Char = ' ';
		BYTE t;

		switch((t = (BYTE) *src++))
		{
		case(0xD5):
			Char = 'a';
			break;
		case(0xD6):
			Char = 'b';
			break;
		case(0xD7):
			Char = 'c';
			break;
		case(0xD8):
			Char = 'd';
			break;
		case(0xD9):
			Char = 'e';
			break;
		case(0xDA):
			Char = 'f';
			break;
		case(0xDB):
			Char = 'g';
			break;
		case(0xDC):
			Char = 'h';
			break;
		case(0xDD):
			Char = 'i';
			break;
		case(0xDE):
			Char = 'j';
			break;
		case(0xDF):
			Char = 'k';
			break;
		case(0xE0):
			Char = 'l';
			break;
		case(0xE1):
			Char = 'm';
			break;
		case(0xE2):
			Char = 'n';
			break;
		case(0xE3):
			Char = 'o';
			break;
		case(0xE4):
			Char = 'p';
			break;
		case(0xE5):
			Char = 'q';
			break;
		case(0xE6):
			Char = 'r';
			break;
		case(0xE7):
			Char = 's';
			break;
		case(0xE8):
			Char = 't';
			break;
		case(0xE9):
			Char = 'u';
			break;
		case(0xEA):
			Char = 'v';
			break;
		case(0xEB):
			Char = 'w';
			break;
		case(0xEC):
			Char = 'x';
			break;
		case(0xED):
			Char = 'y';
			break;
		case(0xEE):
			Char = 'z';
			break;
		case(0xBB):
			Char = 'A';
			break;
		case(0xBC):
			Char = 'B';
			break;
		case(0xBD):
			Char = 'C';
			break;
		case(0xBE):
			Char = 'D';
			break;
		case(0xBF):
			Char = 'E';
			break;
		case(0xC0):
			Char = 'F';
			break;
		case(0xC1):
			Char = 'G';
			break;
		case(0xC2):
			Char = 'H';
			break;
		case(0xC3):
			Char = 'I';
			break;
		case(0xC4):
			Char = 'J';
			break;
		case(0xC5):
			Char = 'K';
			break;
		case(0xC6):
			Char = 'L';
			break;
		case(0xC7):
			Char = 'M';
			break;
		case(0xC8):
			Char = 'N';
			break;
		case(0xC9):
			Char = 'O';
			break;
		case(0xCA):
			Char = 'P';
			break;
		case(0xCB):
			Char = 'Q';
			break;
		case(0xCC):
			Char = 'R';
			break;
		case(0xCD):
			Char = 'S';
			break;
		case(0xCE):
			Char = 'T';
			break;
		case(0xCF):
			Char = 'U';
			break;
		case(0xD0):
			Char = 'V';
			break;
		case(0xD1):
			Char = 'W';
			break;
		case(0xD2):
			Char = 'X';
			break;
		case(0xD3):
			Char = 'Y';
			break;
		case(0xD4):
			Char = 'Z';
			break;
		case(0xA1):
			Char = '0';
			break;
		case(0xA2):
			Char = '1';
			break;
		case(0xA3):
			Char = '2';
			break;
		case(0xA4):
			Char = '3';
			break;
		case(0xA5):
			Char = '4';
			break;
		case(0xA6):
			Char = '5';
			break;
		case(0xA7):
			Char = '6';
			break;
		case(0xA8):
			Char = '7';
			break;
		case(0xA9):
			Char = '8';
			break;
		case(0xAA):
			Char = '9';
			break;
		case(0xAB):
			Char = '!';
			break;
		case(0x5B):
			Char = '%';
			break;
		case(0x2D):
			Char = '&';
			break;
		case(0xBA):
			Char = '/';
			break;
		case(0x5C):
			Char = '(';
			break;
		case(0x5D):
			Char = ')';
			break;
		case(0x35):
			Char = '=';
			break;
		case(0xAC):
			Char = '?';
			break;
		case(0xB8):
			Char = ',';
			break;
		case(0xAD):
			Char = '.';
			break;
		case(0xAE):
			Char = '-';
			break;
		case(0x2E):
			Char = '+';
			break;
		case(0xB4):
			Char = '\'';
			break;
		case(0xF0):
			Char = ':';
			break;
		case(0x36):
			Char = ';';
			break;
		case(0x85):
			Char = '<';
			break;
		case(0x86):
			Char = '>';
			break;
		case(0x00):
			Char = ' ';
			break;
		case(0x1B):
			Char = 'é';
			break;
		case(0xFE):
			Char = '\\';
			break;
		case(0xFA):
			Char = '\\';
			break;
		case(0xF4):
			Char = 'ä';
			break;
		case(0xF5):
			Char = 'ö';
			break;
		case(0xF6):
			Char = 'ü';
			break;
		case(0x15):
			Char = 'ß';
			break;
		case(0xB0):
			Char = '.';
			break;
		case(0xF3):
			Char = 'Ü';
			break;
		case(0xF2):
			Char = 'Ö';
			break;
		case(0xF1):
			Char = 'Ä';
			break;
		case(0xFF):
			Char = '\0';
			break;
		default:
#ifdef DEBUG
			fprintf(stderr, "ERR: Invalid RSS character '0x%02X'\n", t);
#endif
			Char = '\0';
			break;
		}

		*dest++ = Char;
	}

	dest[MaxLength - 1] = '\0';

	return (Tmp > 0 ? true : false);
}
