#ifndef	RASTERFILE_H
#define	RASTERFILE_H

// NOTE: These integer values must be stored in Big Endian format
struct rasterfile {
	int             ras_magic;
	int             ras_width;
	int             ras_height;
	int             ras_depth;
	int             ras_length;
	int             ras_type;
	int             ras_maptype;
	int             ras_maplength;
};

#define	RAS_MAGIC	0x59a66a95

#define	RT_OLD		0
#define	RT_STANDARD	1
#define	RT_BYTE_ENCODED	2
#define	RT_EXPERIMENTAL	0xffff

#define	RMT_RAW		2
#define	RMT_NONE	0
#define	RMT_EQUAL_RGB	1

#endif
