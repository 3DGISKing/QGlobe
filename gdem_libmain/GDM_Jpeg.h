#pragma once

// comress to the jpeg file data
int gdm_JPEGCompress( unsigned char *tileBuffer, unsigned char *jpegBuffer,
		 int width, int height, int comps, int compbytes,  int quality_level );

// decompress from the jpeg file data
extern int gdm_JPEGDecompressTile(void *jpegFileBuffer, void *tileBuffer, int file_size);

