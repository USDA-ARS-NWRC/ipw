#ifndef	IMAGEHEADER_H
#define	IMAGEHEADER_H

/*
 * Author: Philip R. Thompson
 * Address:  phils@athena.mit.edu, 9-526 
 * Note:  size of header should be 1024 (1K) bytes.
 *
 * Header: ImageHeader.h,v 1.2 89/02/13 09:01:36 phils Locked
 * Date: 89/02/13 09:01:36
 * Source: /mit/phils/utils/RCS/ImageHeader.h,v
 */

#define IMAGE_VERSION    3

typedef struct ImageHeader {
	char            file_version[8];/* header version		 */
	char            header_size[8];	/* Size of file header in bytes	 */
	char            image_width[8];	/* Width of the raster image	 */
	char            image_height[8];/* Height of the raster imgage	 */
	char            num_colors[8];	/* Actual # entries in c_map	 */
	char            num_channels[8];/* 0,1 = pixmap, 3 = RGB buffers */
	char            num_pictures[8];/* Number of pictures in file	 */
	char            alpha_channel[4];	/* Alpha channel flag	 */
	char            runlength[4];	/* Runlength encoded flag	 */
	char            author[48];	/* Name of who made it		 */
	char            date[32];	/* Date and time image was made	 */
	char            program[16];	/* Program that created this file */
	char            comment[96];	/* other viewing info		 */
	unsigned char   c_map[256][3];	/* RGB values of pixmap indices	 */
} ImageHeader;

/*
 * Note:
 *
 * - All data is in char's in order to maintain easily portability across
 *   machines and some human readibility.
 *
 * - Images may be stored as pixmaps or in seperate channels, such as red,
 *   green, blue data.
 *
 * - An optional alpha channel is separate and is found after every
 *   num_channels of data.
 *
 * - Pixmaps, red, green, blue, alpha and other channel data are stored
 *   sequentially after the header.
 *
 * - If num_channels = 1 or 0, a pixmap is assumed and up to num_colors of
 *   colormap in the header are used.
 */

/* $Header: /usr/home/ipw/h/RCS/ImageHeader.h,v 1.1 89/10/20 15:03:06 frew Exp $ */
#endif
