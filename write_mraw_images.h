// This file probably needs to be deleted... Check again after sleep.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <opencv2/highgui/highgui.hpp>

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
// USAGE: unpack_12To16(char* INPUT_FILE_PATH, char* OUTPUT_FILE_PATH, int IMAGE_HEIGHT_PIXELS, int IMAGE_WIDTH_PIXELS, int number_of_images)

int write_mraw_12to16( char *INPUT_FILE_PATH, char *OUTPUT_FILE_PATH, int IMAGE_HEIGHT_PIXELS, int IMAGE_WIDTH_PIXELS, int START_IMAGE, int END_IMAGE, int BITS){

	// Number of images to unpack and write
	int number_of_images = END_IMAGE - START_IMAGE + 1;

	// Number of bits per pixel value in the input data
	int bits_per_val_packed = 12;

	// Pointers to files
	FILE *input_file;
	FILE *output_file;

	// Open the input file for reading
	input_file = fopen(INPUT_FILE_PATH, "r");

	// Bug out if the input file isn't found
	if(input_file == NULL){
		printf(KRED "Error: failed to load file" KBLU " %s\n" RESET, INPUT_FILE_PATH);
		return(-1);
	}

	// Open an output file for writing, discarding any existing contents
	output_file = fopen(OUTPUT_FILE_PATH, "w");

	// Bug out if the output file couldn't be opened.
	if(output_file == NULL){
		printf(KRED "Error: failed to create output file " KBLU "%s\n" RESET, OUTPUT_FILE_PATH);
		return(-1);
	}

	// Bytes per unpacked value (8 or 16 usually)
	int bytes_per_val_unpacked = (int)sizeof(uint8_t);

	// Bits per byte (should always be 8)
	int bits_per_byte = 8;

	// Number of pixel values in the whole data set (image dimensions * number of images)
	double nPixels = IMAGE_HEIGHT_PIXELS * IMAGE_WIDTH_PIXELS * number_of_images;

	// Number of 8-bit bytes in the output file.
	double n_bytes_unpacked = (int)sizeof(uint16_t) * nPixels;

	// Number of 8-bit bytes in the raw file
	double n_bytes_packed = nPixels * bits_per_val_packed / bits_per_byte;

	// This is the byte number containing the first bit in each 12-bit chunk of data.
	int start_byte = 0;

	// Bit shift variable
	int bitShift = 0;

	// Allocate space in which to open the source data
	uint8_t *input_data;
	input_data = malloc(n_bytes_packed * sizeof(uint8_t));

	// Allocate space in which to save the new data
	// uint8_t *output_data;
	// output_data = malloc(n_bytes_unpacked * sizeof(uint8_t));

	// Initialize the two 8-bit values that make up each 16-bit value
	uint8_t LOWER_BYTE;
	uint8_t UPPER_BYTE;

	// Inform the user
	printf("Reading file " KBLU "%s\n" RESET, INPUT_FILE_PATH);

	// Read the input binary file in 8-bit chunks
	fread(input_data, (int)sizeof(uint8_t), n_bytes_packed, input_file);

	// Variable to store the number of the image being unpacked.
	// Use double because this can get large.
	printf("Unpacking %d images containing %0.0f pixels\n", number_of_images, nPixels);

	// Initialize timer variables
	time_t tstart, tend;

	// Start a timer
	tstart = time(0);
	
	// Start bit within the binary file
	int start_bit = bits_per_val_packed * IMAGE_WIDTH_PIXELS * IMAGE_HEIGHT_PIXELS * (START_IMAGE);

	// Start byte (8-bit) within the binary file
	start_byte = 2 * (start_bit % 2) + (1 - start_bit % 2);
	
	// Bit shift offset constant
	const int bit_shift_constant = (2 * bytes_per_val_unpacked * bits_per_byte) - bits_per_val_packed;

	// Display
	printf("Bit shift constant: %d\n", bit_shift_constant);

	// Make an image in opencv 
	Mat slice(IMAGE_HEIGHT_PIXELS, IMAGE_WIDTH_PIXELS, CV_16UC1);

	// Declare pixel intensity
	uint16_t pixel_val;

	// Extract the 12-bit bytes out of the array
	for(int k = start_byte; k < nPixels; k++){
		
		// Bit shift
		bitShift = bit_shift_constant * (k % 2);

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
		pixel_val = (((uint16_t) 0 | LOWER_BYTE) << 8  ) | ((uint16_t) 0 | UPPER_BYTE);
		
		// Assign the pixel value to the image.
		Mat.at<uint16_t>(r * IMAGE_WIDTH_PIXELS + c) = pixel_val;

		// Increment the start byte.
		start_byte += 2 * (k % 2) + 1 * (1 - k % 2);
	}

	

	// End time
	tend = time(0);

	// Display time
	printf("Time in loop: %f seconds\n", difftime(tend, tstart));

	// Write the output file
	printf("Writing %d images to file " KBLU "%s\n" RESET, number_of_images, OUTPUT_FILE_PATH);
	fwrite(output_data, (int)sizeof(uint8_t), 2 * nPixels, output_file);

	// Close files
	fclose(input_file);
	fclose(output_file);

	// Free memory
	free(input_data);
	free(output_data);

	// Return success/fail
	return(0);

}
