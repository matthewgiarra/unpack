#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "tiffio.h"

// Text color definitions
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

// This function unpacks 12-bit data into 16-bit data.
// At least that's the plan.
// USAGE: int write_mraw_12to16( std::string INPUT_FILE_PATH, std::string OUTPUT_FILE_DIR, std::string OUTPUT_FILE_BASE, int IMAGE_HEIGHT_PIXELS, int IMAGE_WIDTH_PIXELS, int START_IMAGE, int END_IMAGE = -1, int PIXEL_BIT_SHIFT = 3, int FILE_DIGITS = 5)

/*
	INPUTS:
		INPUT_FILE_PATH (std::string) is the path to the mraw file that will be unpacked to TIFF images.
		
		OUTPUT_FILE_DIR (std::string) is the path to the directory in which the unpacked TIFF images will be saved.
		
		OUTPUT_FILE_BASE (std::string) is the base-name of the images that will be output. For example, if you want to 
			output images named "my_image_000001.tiff", "my_image_000002.tiff", then set OUTPUT_FILE_BASE = "my_image_"

	 	IMAGE_HEIGHT_PIXELS (int) is the number of rows in the images. If your images are 1280 pixels wide by 1024 pixels tall, then set IMAGE_HEIGHT_PIXELS = 1024

		IMAGE_WIDTH_PIXELS (int) is the number of columns in the images. If your images are 1280 x 1024, then set IMAGE_WIDTH_PIXELS = 1280

		START_IMAGE (int) is the image number at which you want to start unpacking. If you want to start unpacking at the 50th image in the sequence, set START_IMAGE = 50

		END_IMAGE (int) is the image number at which you want to stop unpacking. Setting END_IMAGE = -1 unpacks all the images from START_IMAGE to the end of the sequence. 

		PIXEL_BIT_SHIFT (int) is the bit-shift of the images that will be saved. Since the MRAW images are saved as 12-bit, but TIFFs are output as 16 bit, the 12 bits
			that contain the image data can be shifted within the 16 bits available for data storage in the TIFFs. If PIXEL_BIT_SHIFT = 0 then the 12 data bits
	 		will occupy the first 12 bits of each 16-bit TIFF pixel (bits 0 to 11); if PIXEL_BIT_SHIFT = 1 then the 12 data bits will occupy the second through
			thirteenth bits of each 16-bit TIFF pixel (bits 1 to 12), etc. In practice this makes your images appear brigter or darker when you open them for
	 		viewing, but it doesn't affect the data that the images contain. In other words, this setting does NOT truncate your data.

		FILE_DIGITS (int) is the number of digits that will be used to form the file names of the saved TIFF images. For example, if you want to 
			output images named "my_image_000001.tiff", "my_image_001999.tiff", then set FILE_DIGITS = 6 to reflect the six digits in the file names.

	OUTPUTS:
		Returns 0 on success.
*/

int write_mraw_12to16( std::string INPUT_FILE_PATH, std::string OUTPUT_FILE_DIR, \
 	std::string OUTPUT_FILE_BASE, const int IMAGE_HEIGHT_PIXELS, const int IMAGE_WIDTH_PIXELS, \
	int START_IMAGE, int END_IMAGE = -1, int PIXEL_BIT_SHIFT = 3, int FILE_DIGITS = 5, \
	std::string FILE_EXTENSION = ".tiff"){
	
	// Function prototypes
	unsigned long long int startByte(int IMAGE_HEIGHT, int IMAGE_WIDTH, int START_IMAGE, int BITS_PER_VAL_PACKED);
	
	// Function prototype for writeTiff_bw16
	void writeTiff_bw16(char *output_file_path, uint16_t *image_data, int image_height, int image_width);
	
	// Initialize timer variables
	time_t tstart, tend;
	
	// Initialize the two 8-bit values that make up each 16-bit value
	uint8_t LOWER_BYTE;
	uint8_t UPPER_BYTE;
	
	// Bit shift variable
	int bitShift;
	
	// Pixels per image
	int pixels_per_image = IMAGE_HEIGHT_PIXELS * IMAGE_WIDTH_PIXELS;
	
	// Default to single image.
	if(END_IMAGE == -1){
		END_IMAGE = START_IMAGE;
	}
		
	// Number of images to unpack and write
	int number_of_images = END_IMAGE - START_IMAGE + 1;
	
	// Print
	std::cout << "Start image: " << START_IMAGE << "\tEnd image: " << END_IMAGE << "\tNumber of images: " << number_of_images << "\n";
		
	// Number of pixel values in the whole data set (image dimensions * number of images)
	unsigned long long int nPixels = IMAGE_HEIGHT_PIXELS * IMAGE_WIDTH_PIXELS * number_of_images;
	
	// Number of bits per pixel value in the input data
	int bits_per_val_packed = 12;

	// Bytes per unpacked value (8 or 16 usually)
	int bytes_per_val_unpacked = (int)sizeof(uint8_t);

	// Bits per byte (should always be 8)
	const int bits_per_byte = 8;
	
	// Bit shift offset constant. This is the offset from the 
	//beginning of each byte  that start-bits may occur.
	const int bit_shift_constant = (2 * bytes_per_val_unpacked * bits_per_byte) - bits_per_val_packed;

	// Allocate memory for a 16-bit "slice" which will hold the image data.
	uint16_t *slice = new uint16_t[IMAGE_WIDTH_PIXELS * IMAGE_HEIGHT_PIXELS];

	// Declare pixel intensity
	uint16_t pixel_val;

	// Number of 8-bit bytes in the raw file
	unsigned long long int n_bytes_packed = nPixels * bits_per_val_packed / bits_per_byte;
				
	// String for number format
	std::string num_spec = "\%0" + std::to_string(FILE_DIGITS) + (std::string)"d";
	
	// String for the whole file path
	std::string output_file_path;
	
	// Char array for image digits
	char numstr[FILE_DIGITS];

	// Pointers to files
	FILE *input_file;

	// Open the input file for reading
	input_file = fopen(INPUT_FILE_PATH.c_str(), "r");
	
	// Bug out if the input file isn't found
	if(input_file == NULL){
		// Display an error if the mraw file wasn't loaded successfully
		std::cout << KRED << "Error: failed to load file " << KBLU << INPUT_FILE_PATH << RESET << "\n";
		return(-1);
	}

	// Allocate space in which to open the source data
	uint8_t *input_data = (uint8_t*) malloc(n_bytes_packed * sizeof(uint8_t));
	if(!input_data){
		std::cout << KRED << "ERROR: Out of memory\n" << RESET;
		return(-1);
	}
	
	// Start byte within the binary to read
	unsigned long long int start_byte = startByte(IMAGE_HEIGHT_PIXELS, IMAGE_WIDTH_PIXELS, START_IMAGE, bits_per_val_packed);

	// Read from the middle of the input file
	fseek(input_file, start_byte, SEEK_SET);

	// Read the input binary file in 8-bit chunks
	fread(input_data, (int)sizeof(uint8_t), n_bytes_packed, input_file);

	// Start a timer
	tstart = time(0);

	// More declarations
	int start_bit, start_pixel, end_pixel;
		
	// Loop over all the images specified, saving each one as a TIFF.
	for(int image_num = 0; image_num < number_of_images; image_num++){
		
		// Start byte for this image
		start_byte = startByte(IMAGE_HEIGHT_PIXELS, IMAGE_WIDTH_PIXELS, image_num, bits_per_val_packed);
		
		// Start and end pixels
		start_pixel = pixels_per_image * (image_num + START_IMAGE);
		end_pixel  	= start_pixel + (pixels_per_image);
		
		// Loop over all pixels
		for(int pixel_num = 0; pixel_num < pixels_per_image; pixel_num++){

			// Bit shift
			bitShift = bit_shift_constant * (pixel_num % 2);
			
			// Populate the new bytes
			// Most significant bits (MSB) in big-endian format.
			// This shifts the first 8-bit byte to the left by BITSHIFT bits, and then
			// concatonates it with the first (BITS_PER_BYTE - BITSHIFT) bits of the subsequent byte.
			UPPER_BYTE = (input_data[start_byte] << bitShift) | (input_data[start_byte + 1] >> (bits_per_byte - bitShift));

			// Least significant bit (LSB) in bit-endian format.
			// This shifts the second 8-bit byte by (8 - bitShift) bits to the left
			// and then truncates its to the first (bits_per_val_packed - bits_per_byte) bits
			// (e.g. bits_per_val_packed = 12; bits_per_byte = 8; so the second 8-bit byte is truncated to the first (12-8) = 4 bits).
			LOWER_BYTE = (input_data[start_byte + 1] << bitShift) & (255 << (bits_per_val_packed - bits_per_byte));

			// Combine 8-bit bytes into 16-bit byte
			pixel_val = ((((uint16_t) 0 | UPPER_BYTE) << 8  ) | ((uint16_t) 0 | LOWER_BYTE)) << PIXEL_BIT_SHIFT;
		
			// Assign the pixel value to the image.
			slice[pixel_num] = pixel_val;

			// Increment the start byte.
			start_byte += 2 * (pixel_num % 2) + 1 * (1 - pixel_num % 2);
		}
		
		// Build save name
		// This is the string containing the file number (i.e. 00001)
		sprintf(numstr, num_spec.c_str(), START_IMAGE + image_num);

		// Create the file path to the saved image.
		std::string output_file_path_string = OUTPUT_FILE_DIR + OUTPUT_FILE_BASE + std::string(numstr) + FILE_EXTENSION;
		
		char *output_file_path = (char*)output_file_path_string.c_str();
		
		// Print the file name being saved
		std::cout << "Saving file: " << KBLU << output_file_path_string << RESET << "\n";
		
		// Write the first image
		writeTiff_bw16(output_file_path, slice, IMAGE_HEIGHT_PIXELS, IMAGE_WIDTH_PIXELS);
	}
	
	// End time
	tend = time(0);
	
	// Elapsed time
	int elapsed = (int)tend - (int)tstart;
	
	// Display success
	std::cout << "Saved " << KGRN << number_of_images \
		<< RESET << " images in " << KGRN << elapsed << RESET << " seconds\n";
	std::cout << "Save directory: " << KGRN << OUTPUT_FILE_DIR << RESET << "\n";

	// Close files
	fclose(input_file);

	// Free memory
	free(input_data);
	
	// GTFO
	return(0);

}

// Code for testing the C TIFF library, libtiff
void writeTiff_bw16(char *output_file_path, uint16_t *image_data, int image_height, int image_width){
		
	// Set the number of channels in the image to one, since we are assuming these images come out grayscale.
	const int samples_per_pixel = 1;
						
	// Open the file
	TIFF *output_image = TIFFOpen(output_file_path, "w");
	
	/* Set some fields for the TIFF image */
	// Image width
	TIFFSetField(output_image, TIFFTAG_IMAGEWIDTH, image_width);
	
	// Image height
	TIFFSetField(output_image, TIFFTAG_IMAGELENGTH, image_height);
	
	// Samples per pixel
	TIFFSetField(output_image, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
	
	// Bits per sample. Set to 16 bits for these images.
	TIFFSetField(output_image, TIFFTAG_BITSPERSAMPLE, 16);
	
	// Set the origin of the image to the top-left corner.
	TIFFSetField(output_image, TIFFTAG_ORIENTATION, ORIENTATION_BOTLEFT);
	
	// No compression
	TIFFSetField(output_image, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
	
	// Some other important fields that I don't understand yet.
	TIFFSetField(output_image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(output_image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
	
	// Set the length in memory of one row of pixels in the image.
	tsize_t line_bytes = samples_per_pixel * image_width;
		
	// Buffer used to store the row of pixel information for writing to a file
	uint16_t *buf = NULL;
	
	// Allocate memory to store the pixels of the current row
	if (TIFFScanlineSize(output_image) == line_bytes)
		// buf = (unsigned char*)_TIFFmalloc(line_bytes);
		buf = (uint16_t*)_TIFFmalloc(line_bytes);
	else
		buf = (uint16_t*)_TIFFmalloc(TIFFScanlineSize(output_image));
	
	// Set the strip size of the file to be the size of one row of pixels
	TIFFSetField(output_image, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(output_image, image_width * samples_per_pixel));
	
	// Write the image to the file one strip at a time
	for(int row = 0; row < image_height; row++){
		memcpy(buf, &image_data[(image_height - row - 1) * line_bytes], sizeof(uint16_t) * line_bytes);
		if(TIFFWriteScanline(output_image, buf, row, 0) < 0)
			break;
	}
	
	// Destroy the buffer
	_TIFFfree(buf);
	
	// Close the output image
	TIFFClose(output_image);
		
}

// This function returns the starting bit, at which to read an image
unsigned long long int startByte(int IMAGE_HEIGHT, int IMAGE_WIDTH, int START_IMAGE, int BITS_PER_VAL_PACKED){
	
	// Bits per byte
	const int bits_per_byte = 8;
	
	// Index of the first relevant 8-byte within the binary
	unsigned long long int start_byte = floor(BITS_PER_VAL_PACKED * IMAGE_HEIGHT * IMAGE_WIDTH * START_IMAGE / bits_per_byte);
	
	// Return the start byte number.
	return start_byte;	
}
// 