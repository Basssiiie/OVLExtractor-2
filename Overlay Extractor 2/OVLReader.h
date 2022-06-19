#pragma once
#include <vector>
#include <string>
#include <vcclr.h>

#pragma warning(disable:4244) // FPOS_T to LONG

using namespace System;
using namespace System::Diagnostics;

typedef unsigned long ulong;
typedef unsigned short ushort;

//Header part 1
struct OVLHeader
{
	ulong magic;
	ulong headerversion;
	ulong version;	
};

//Header part 2
struct OVLHeader2
{
	ulong unknown;
	ulong totalloaders;
};

//Header part 1 v2
struct OVLHeaderVersion2Extra
{
	ulong unknown;
};

struct OVLHeaderV4Unknown
{
	ulong unknowna;
	ulong unknownb;
};

struct OVLHeaderV5Extra
{
	ulong subversion;
	OVLHeaderV5Extra()
	{
		subversion = 0;
	}
};

struct OVLHeaderV5Unknown
{
	ulong unknowna;
	ulong unknownb;
	ulong unknownc;
};

struct OVLV5LoaderExtra
{
	ulong count;
	ulong order;
};

struct OVLExtendedHeaders
{
	OVLHeaderVersion2Extra		headerversion2extra;
	OVLHeaderV5Extra			ovlv5extra;
	OVLHeaderV5Unknown			ovlv5unknown;
	OVLHeaderV4Unknown			ovlv4unknown;
};

struct V5Extra
{
	std::vector<OVLV5LoaderExtra>		ovlv5extraloaderinfo;
	ulong								unknownperblock[9];
	std::vector<ulong>					unknownafterfileblocks;
	ulong								extrastructcount;
};

struct Reference
{
	ushort length;
	std::string file;
};

struct Loader
{
	std::string loader;
	std::string name;
	ulong type;
	std::string tag;
};

struct Block
{
	size_t size;
	fpos_t position;
	size_t internal_offset;
};

struct Chunk
{
	ulong num_blocks;
	std::vector<Block> blocks;
};

struct SymbolString
{
	fpos_t pos;
	ulong internal_offset;
	std::string data;
};

struct SymbolStruct {
	ulong stringpointer;
	ulong datapointer;
	ulong ispointer;
	ulong loaderpointer;
	ulong hash;
	ulong internal_offset;
};

struct LoadReference {
	ulong loadernumber;
	ulong datapointer;
	ulong hasextradata;
	ulong symbolstructpointer;
	ulong num_symbolsresolve;
	ulong internal_offset;
};

struct SymbolResolve{
	ulong pointer;
	ulong stringpointer;
	ulong loadpointer;
	ulong stringhash;
	ulong internal_offset;
};

enum
{
	C_OVL =0,
	U_OVL
};

struct LinkedFiles
{
	LoadReference	loaderreference;
	SymbolStruct	symbolresolve;
};

struct PreResolved
{
	ulong offset;
	std::string name;
	ulong size;

	ulong count1;
	ulong count2;
	ulong count3;
};

struct MDLUnknownEnum
{
	enum EnumReverse
	{
		Unknown_1 =                 0x00000001, ///< 
		Unknown_2 =                 0x00000002, ///< Always
		Unknown_3 =					0x00000004, ///< Always
		Unknown_4 =                 0x00000008, ///< 
		Unknown_5 =					0x00000010, ///< 
		Unknown_6 =					0x00000020, ///< 
		Unknown_7 =                 0x00000040, ///< 
		Unknown_8 =					0x00000080, ///< 
		Unknown_9 =                 0x00000100, ///< 
		Unknown_10 =                0x00000200, ///< Always
		Unknown_11 =                0x00000400, ///< Always
		Unknown_12 =				0x00000800, ///< 
		Unknown_13 =                0x00001000, ///< 
		Unknown_14 =                0x00002000, ///< 
		Unknown_15 =                0x00004000, ///< 
		Unknown_16 =                0x00008000, ///< 
		Unknown_17 =				0x00010000, ///< AdultGiraffe, Gorilla, AdultGrizzly, Hippo, Zebra, 'CubLion', FemaleLion, AdultTiger, CubTiger, BabyOstrich, Leopard, Panther, FemaleAdultPolar, Rhino
		Unknown_18 =                0x00020000, ///< Chimp, Giraffe, Gorilla, Grizzly, Hippo, Zebra, Kangaroo, 'CubLion', FemaleLion, AdultTiger, CubTiger, AdultOstrich, Panda, Baby/AdultPolar, Rhino
		Unknown_19 =                0x00040000, ///< Camel, Elephant, Horse, 'CubLion', FemaleLion, AdultTiger, CubTiger, Mandrill, Orang, Leopard, Panther, Gazelle
		Unknown_20 =                0x00080000, ///< MaleLion, high endangered?
		//Note that the Horse and Zebra use the same animations, this enum could contain: endangered species level (high, low etc), Fencetype required, house required
	};
};

struct OVLData
{
	std::string					ovlname;
	bool						hassymbolresolves;
	ulong						num_references;
	OVLHeader					h1;
	OVLHeader2					h2;
	OVLExtendedHeaders			extendedheaders;
	V5Extra						v5extra;

	std::vector<Loader>			loaders;
	std::vector<Reference>		references;
	std::vector<Chunk>			chunks;
	std::vector<ulong>			relocations;
	std::vector<ulong>			relocationspointingto;
	std::vector<SymbolString>	symbolstring;

	std::vector<SymbolStruct>	symbolpointers;
	std::vector<LoadReference>	loaderreference;
	std::vector<SymbolResolve>	symbolresolves;

	std::vector<PreResolved>	presolvedfurtherdata;
	std::vector<LinkedFiles>	linkedfiles;
	OVLData(){
		hassymbolresolves = false;
	}
	ulong						postrelocunknown;
	ulong						dataend;
};

struct PositionReturn
{
	ulong currentOVL;
	ulong position;
};

struct TConvert
{
	union {
		unsigned long ul;
		float fl;
		struct {
			short hi;
			short lo;
		};
		unsigned char c[4];
	};
};

class OVLReader
{
private:
	std::string		lasterror;
	bool			ReadOVL(std::string ovlname, ulong currentOVL, int &offset);
	bool			ovlreaded;
	std::string		GetStringA(FILE *&ovl, size_t size);
	std::string		GetStringA(FILE *&ovl);
	std::wstring	GetStringW(FILE *&ovl);
	OVLData			OVLD[2];
public:
	std::string		safefilename;
	bool			HasResource(std::string resourcename);
	std::string		ReturnDatablocknameFromOffset(ulong offset, bool striptag);

	//INTERN	
	std::vector<LinkedFiles>	GetRawLinkedFiles(ulong currentOVL){return OVLD[currentOVL].linkedfiles;};
	std::vector<SymbolStruct>	GetRawSymbolReferences(ulong currentOVL){return OVLD[currentOVL].symbolpointers;};
	std::vector<LoadReference>	GetRawLoaderReferences(ulong currentOVL){return OVLD[currentOVL].loaderreference;};
	std::vector<SymbolResolve>	GetRawSymbolResolves(ulong currentOVL){return OVLD[currentOVL].symbolresolves;};
	bool						IsValid(){return ovlreaded;}
	OVLData						GetOVLD(ulong currentOVL){ return OVLD[currentOVL];}
	std::string					GetLastReadingError(){return lasterror;}
	//EXTERN
	std::string		PointerdataAtOffset(ulong offset);
	Loader			GetLoaderByID(ulong id, ulong currentOVL);
	bool			IsReloc(ulong offset);
	PositionReturn	OffsetToPosition(ulong offset);
	std::string		GetStringFromOffset(ulong offset);
	OVLReader(std::string filename);
};

