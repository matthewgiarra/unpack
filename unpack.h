#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

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
// USAGE: unpack_12To16(char* INPUT_FILE_PATH, char* OUTPUT_FILE_PATH, int IMAGE_HEIGHT_PIXELS, int IMAGE_WIDTH_PIXELS, int NUMBER_OF_IMAGES)

int unpack_12To16( char *INPUT_FILE_PATH, char *OUTPUT_FILE_PATH, int IMAGE_HEIGHT_PIXELS, int IMAGE_WIDTH_PIXELS, int NUMBER_OF_IMAGES){

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

	// Image size (pixels)
	uint32_t pixels_per_image = IMAGE_WIDTH_PIXELS * IMAGE_HEIGHT_PIXELS;

	// Number of pixel values in the whole data set (image dimensions * number of images)
	uint64_t nPixels = IMAGE_HEIGHT_PIXELS * IMAGE_WIDTH_PIXELS * NUMBER_OF_IMAGES;

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
	uint8_t *output_data;
	output_data = malloc(n_bytes_unpacked * sizeof(uint8_t));

	// Initialize the two 8-bit values that make up each 16-bit value
	uint8_t LOWER_BYTE;
	uint8_t UPPER_BYTE;

	// Inform the user
	printf("\nReading file " KBLU "%s\n" RESET \
		"Packed data size: " KBLU "%0.2f" RESET " MB\n" \
		"Unpacked data size: " KBLU "%0.2f" RESET " MB\n", \
		INPUT_FILE_PATH, n_bytes_packed/1000000, n_bytes_unpacked/1000000);

	// Read the input binary file in 8-bit chunks
	fread(input_data, (int)sizeof(uint8_t), n_bytes_packed, input_file);

	// Variable to store the number of the image being unpacked.
	// Use double because this can get large.
	printf("\nUnpacking %d images...\n", NUMBER_OF_IMAGES);

	// Initialize timer variables
	time_t tstart, tend;

	// Start a timer
	tstart = time(0);

	// Initialize start byte.
	start_byte = 0;

	// Bit shift offset constant
	int bit_shift_constant = (2 * bytes_per_val_unpacked * bits_per_byte) - bits_per_val_packed;

	//Progress Bar updates every 1%:
	double bar_update = (double)NUMBER_OF_IMAGES / 50;

	// Image number for use in progress bar.
	uint32_t imnum;

	// Initialize counter
	uint32_t k = 0;

	// Extract the 12-bit bytes out of the array
	for(k = 0; k < nPixels; k++){
		
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

		// Assign the 16-bit output byte to the output data array.
		output_data[2 * k] = LOWER_BYTE;
		output_data[2 * k + 1]  = UPPER_BYTE;

		// Increment the start byte.
		start_byte = start_byte + 2 * (k % 2) + 1 * (1 - k % 2);

		//Progess bar
		// We should move this into its own function...
		// void(int current, int final);
		if((k + 1) % pixels_per_image == 0){
			imnum = (k + 1) / pixels_per_image;
		
			// Print image number
			printf(KBLU "%d/%d[", imnum, NUMBER_OF_IMAGES);
			
			// Print progress bar
			for(int i = 0; i <= 50; i++){
				if(i <= imnum / bar_update){
					printf("=");
				}
				else{
					printf(" ");
				}
			}
			
			// Flush the console.
			printf("] %d%%\r", (imnum * 100) / NUMBER_OF_IMAGES);
			fflush(stdout);
		}
	}
	
	
	// Reset console colors
	printf(RESET);
	printf("\nFinal k: %d\nNumber of pixels: %d\n", k, NUMBER_OF_IMAGES * pixels_per_image);


		
	// Write the output file
	printf("\n\nWriting %0.2f MB to disk...\n", n_bytes_unpacked / 1000000);
	fwrite(output_data, (int)sizeof(uint8_t), 2 * nPixels, output_file);

	// Close files
	fclose(input_file);
	fclose(output_file);

	// Free memory
	free(input_data);
	free(output_data);
	
	// End time
	tend = time(0);

	// Display time
	printf("\nSaved %d images (%0.2f MB) to file " KBLU "%s" RESET " in %0.2f seconds. \n", NUMBER_OF_IMAGES, n_bytes_unpacked / 1000000, OUTPUT_FILE_PATH, difftime(tend, tstart));
	
	// Return success/fail
	return(0);
	
}
