#include <stdio.h>
#include "GDM_Define.h"
extern "C"
{
	#include <jpeglib.h>
	#include <jerror.h>
}

typedef int jmp_buf[16];

void null_init_source( j_decompress_ptr cinfo ) {}
boolean null_fill_input_buffer( j_decompress_ptr cinfo ) {return TRUE;}
boolean null_resync_to_restart( j_decompress_ptr cinfo, int d ) {return TRUE;}
void null_term_source( j_decompress_ptr cinfo ) {}
void null_skip_input_data( j_decompress_ptr cinfo, long num_bytes ) {
  cinfo->src->next_input_byte += num_bytes;
  cinfo->src->bytes_in_buffer -= num_bytes;
}

void null_init_destination( j_compress_ptr cinfo ) {}
boolean null_empty_output_buffer( j_compress_ptr cinfo ) {return TRUE;}
void null_term_destination( j_compress_ptr cinfo ) {}

int gdm_JPEGCompress( unsigned char *tileBuffer, unsigned char *jpegBuffer,
		 int width, int height, int comps, int compbytes,
		 int quality_level )
{
  const char                  *funcName = "tsmJPEGCompressTile";
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr       jerr;
  struct jpeg_destination_mgr dest_mgr;
  JSAMPROW                    buffer;
  int                         tile_size, jpeg_tile_size;
  int                         jpeg_data_size, row_size;

  if ( tileBuffer == NULL || jpegBuffer == NULL ) {
    return FALSE;
  }

  /* First we shall work out the size of the uncompressed tile (bytes) */
  /* The size of the jpeg tile is currently specified to be the same   */
  /* as the raw (uncompressed) tile.                                   */

  tile_size = width * height * comps * compbytes;

  jpeg_tile_size = tile_size;

  /* Initialise the JPEG compression object with default error handling. */

  cinfo.err = jpeg_std_error( &jerr );
  jpeg_create_compress( &cinfo );
 
  /* Initialize the JPEG colour space parameters and the quality setting */
  /* If we have a DEM tile, then we set the colour space to GRAYSCALE    */
  /* because we only have one component per pixel, otherwise, it's RGB   */

  cinfo.input_components = comps;
  if ( comps == 1 )
    cinfo.in_color_space = JCS_GRAYSCALE;
  else
    cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults( &cinfo );

  /* Set the colour space to store the jpeg data in. This has to be  */
  /* YCbCr for compatibility with the JFIF standard. We also set the */
  /* compression quality and specify that we don't want to force the */
  /* generation of quantization table entires that are limited to    */
  /* 1..255 for JPEG baseline compat (this is what libtiff does also)*/

  jpeg_set_colorspace( &cinfo, JCS_YCbCr );
  jpeg_set_quality( &cinfo, quality_level, FALSE ); 

  /* Now fill in all of the image dimension details. We specify the */
  /* width to include the number of bytes per component so that we  */
  /* could feasible JPEG compress DEM tiles okay.                   */

  cinfo.image_width        = (JDIMENSION) width * compbytes;
  cinfo.image_height       = (JDIMENSION) height;

  /* we don't want any JFIF or Adobe headers - so tell 'em that. */

  cinfo.write_JFIF_header  = FALSE;
  cinfo.write_Adobe_marker = FALSE;

  /* We specify here that we want the compressor to compute optimal */
  /* Huffman coding tables for the image.  This requires a bit more */
  /* computation, but it saves a couple hundred bytes per image.    */
  /* This optimisation is also performed by libtiff.                */

  cinfo.optimize_coding = TRUE;

  /* These sampling factors aren't setup correctly by default, so */
  /* we should reset these to {1,1} here, again just like libtiff */

  cinfo.comp_info[0].h_samp_factor = 1;
  cinfo.comp_info[0].v_samp_factor = 1;

  /* set up the destination manager so that the jpeg_write_scanlines() */
  /* routine will write directly to our jpeg buffer. We set all of the */
  /* callbacks to empty functions because they should never be needed  */
  /* because we should have a buffer big enough to fit the entire image*/

  dest_mgr.next_output_byte    = (JOCTET *) jpegBuffer;
  dest_mgr.free_in_buffer      = (size_t) jpeg_tile_size;
  dest_mgr.init_destination    = null_init_destination;
  dest_mgr.empty_output_buffer = null_empty_output_buffer;
  dest_mgr.term_destination    = null_term_destination;

  cinfo.dest = &dest_mgr;

  /* Start the JPEG compressor... and include all tables */

  jpeg_start_compress( &cinfo, TRUE );

  /* Compress the data into jpegBuffer, starting with the last row    */
  /* and working back to the first row because we also need to switch */
  /* the origin from lrbt to lrtb for JPEG.                           */

  row_size = cinfo.image_width * cinfo.input_components;
  buffer   = (JSAMPROW) tileBuffer + row_size * ( cinfo.image_height - 1 );

  while ( cinfo.next_scanline < cinfo.image_height ) {
    jpeg_write_scanlines( &cinfo, &buffer, 1 );
    buffer -= row_size;
  }

  /* Finish the compressor, then work out how large the written */
  /* JPEG data is, and release all memory for the JPEG Object   */

  jpeg_finish_compress( &cinfo );
  jpeg_data_size = jpeg_tile_size - dest_mgr.free_in_buffer;
  jpeg_destroy_compress( &cinfo );

  return jpeg_data_size;
}
int gdm_JPEGDecompressTile(void *jpegFileBuffer, void *tileBuffer, int file_size)
{
	// allocate and initialize JPEG decompression object
	struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr         jerr;
    struct jpeg_source_mgr        jsrc;
    JSAMPROW                      buffer;
    int                           row_size;

	if ( tileBuffer == NULL || jpegFileBuffer == NULL ) {
		return FALSE;
	}

    /* Initialise the JPEG decompression object with default error handling. */
	cinfo.err = jpeg_std_error(&jerr);
	// Now we can initialize the JPEG decompression object.
	jpeg_create_decompress(&cinfo);


	// Set up data pointer
	jsrc.bytes_in_buffer = file_size;
	jsrc.next_input_byte = (JOCTET*)jpegFileBuffer;
	cinfo.src = &jsrc;

	jsrc.init_source = null_init_source;
	jsrc.fill_input_buffer = null_fill_input_buffer;
	jsrc.skip_input_data = null_skip_input_data;
	jsrc.resync_to_restart = null_resync_to_restart;
	jsrc.term_source = null_term_source;

	// read file parameters with jpeg_read_header()
	jpeg_read_header(&cinfo, TRUE);

	cinfo.out_color_space=JCS_RGB;
	cinfo.out_color_components=3;
	cinfo.do_fancy_upsampling=FALSE;

	// Start decompressor
	jpeg_start_decompress(&cinfo);

	/* Decompress the data into tileBuffer, starting with the last row  */
	/* and working back to the first row because we also need to switch */
	/* the origin from JPEG's lrtb to TV's lrbt                         */
	row_size = cinfo.image_width * cinfo.num_components;
	buffer   = (JSAMPROW) tileBuffer + row_size * ( cinfo.image_height - 1 );

	while ( cinfo.output_scanline < cinfo.output_height ) {
		jpeg_read_scanlines( &cinfo, &buffer, 1 );
		buffer -= row_size;
	}

	/* Finish decompression and release memory. */
	jpeg_finish_decompress( &cinfo );
	jpeg_destroy_decompress( &cinfo );

	return TRUE;
}
