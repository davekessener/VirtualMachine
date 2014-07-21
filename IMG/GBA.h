#ifndef _GBA_H
#define _GBA_H

#include <iostream>
#include <fstream>
#include <string.h>
#include "IMG.h"
#include "LZ77.h"

#define INVALID_OFFSET ((::IMG::OFFSET)-1)
#define TODWORD(p) (*((::IMG::DWORD *) (p)))

#define GBAR(gba) (((gba)&0x1f)<<3)
#define GBAG(gba) ((((gba)>>5)&0x1f)<<3)
#define GBAB(gba) ((((gba)>>10)&0x1f)<<3)
#define GBAtoRGB(gba) RGBA(GBAR(gba),GBAG(gba),GBAB(gba),0xff)

namespace IMG
{
	const OFFSET BankOffset = 0x3526A8;
	const OFFSET NamesOffset = 0x3F1CAC;
	
	typedef struct _map
	{
		OFFSET Offset;
		struct _header
		{
			OFFSET	FooderOffset;
			OFFSET	EventsOffset;
			OFFSET	MapscriptOffset;
			OFFSET	ConnectionsOffset;
			WORD	Music;
			struct	_name
			{
				BYTE	Index;
				OFFSET	OffsetOffset;
				OFFSET	NameOffset;
				char	Buffer[20];
				char	Value[20];
			} Name;
			BYTE	CaveType;
			BYTE	WeatherType;
			BYTE	MapType;
			BYTE	BattleType;
		} Header;
		struct _fooder
		{
			DWORD	Width;
			DWORD	Height;
			OFFSET	BorderOffset;
			OFFSET	MapOffset;
			OFFSET	TileSetOffset[2];
			BYTE	BorderWidth;
			BYTE	BorderHeight;
		} Fooder;
		struct _palette
		{
			WORD Colors_raw[16];
			struct _color
			{
				BYTE Red;
				BYTE Green;
				BYTE Blue;
			} Colors[16];
			RGBA CR_Colors[16];
		} Pallets[13];
		WORD *MapOrder;
		BYTE *WalkData;
		struct _tile_set
		{
			OFFSET LZ77ImageOffset;
			OFFSET PalletsOffset;
			OFFSET TilesOffset;
			OFFSET BehaviorBackgroundOffset;
			WORD   TileCount;
		} TileSets[2];
		struct _tile
		{
			BYTE raw[16];
			WORD BlockNr[8];
			BYTE PalletNr[8];
			BYTE Reflection[8];
		} *Tiles;
		WORD *Behavior;
		WORD *Background;
		BYTE *LZ77Image;
		int   TileCount;
		int   BehaviorBackgroundLength;
		int   LZ77ImageSize;
		const std::fstream *ROM;
	} MAP;
	
	typedef struct _tile_simple
	{
		WORD BlockNr[8];
		BYTE PalletNr[8];
		BYTE Reflection[8];
	} TILE;
}

	bool InitializeMap(std::fstream&, IMG::OFFSET, IMG::_map *);
	void ReleaseMap(IMG::_map *);
	IMG::OFFSET GetMapHeaderOffset(std::fstream&, int, int);
	bool ConvertPallet(IMG::_map::_palette *, IMG::WORD *);
	bool ConvRSStoANSI(char *, char *, int);

#endif
