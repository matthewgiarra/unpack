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

// Namespaces
using namespace cv;

// This function unpacks 12-bit data into 16-bit data.
// At least that's the plan.
// USAGE: unpack_12To16(char* INPUT_FILE_PATH, char* OUTPUT_FILE_PATH, int IMAGE_HEIGHT_PIXELS, int IMAGE_WIDTH_PIXELS, int number_of_images)

int write_mraw_12to16( char *INPUT_FILE_PATH, int IMAGE_HEIGHT_PIXELS, int IMAGE_WIDTH_PIXELS, int START_IMAGE, int END_IMAGE, int PIXEL_BIT_SHIFT){
	
	// Print
	printf("Start image: %d\t End image: %d\t\n", START_IMAGE, END_IMAGE);
	
	// Number of images to unpack and write
	int number_of_images = END_IMAGE - START_IMAGE + 1;
	
	// Number of pixel values in the whole data set (image dimensions * number of images)
	double nPixels = IMAGE_HEIGHT_PIXELS * IMAGE_WIDTH_PIXELS * number_of_images;
	
	// Pixels per image
	int pixels_per_image = IMAGE_HEIGHT_PIXELS * IMAGE_WIDTH_PIXELS;
	
	// Variable to store the number of the image being unpacked.
	// Use double because this can get large.
	printf("Unpacking %d images containing %0.0f pixels (%d pixels per image)\n", number_of_images, nPixels, pixels_per_image);

	// Number of bits per pixel value in the input data
	int bits_per_val_packed = 12;

	// Pointers to files
	FILE *input_file;

	// Open the input file for reading
	input_file = fopen(INPUT_FILE_PATH, "r");

	// Bug out if the input file isn't found
	if(input_file == NULL){
		printf(KRED "Error: failed to load file" KBLU " %s\n" RESET, INPUT_FILE_PATH);
		return(-1);
	}

	// Bytes per unpacked value (8 or 16 usually)
	int bytes_per_val_unpacked = (int)sizeof(uint8_t);

	// Bits per byte (should always be 8)
	int bits_per_byte = 8;

	// Number of 8-bit bytes in the output file.
	double n_bytes_unpacked = (int)sizeof(uint16_t) * nPixels;

	// Number of 8-bit bytes in the raw file
	double n_bytes_packed = nPixels * bits_per_val_packed / bits_per_byte;
	
	printf("Read %0.0f bytes\n", n_bytes_packed);

	// Bit shift variable
	int bitShift = 0;

	// Allocate space in which to open the source data
	uint8_t *input_data = (uint8_t*) malloc(n_bytes_packed * sizeof(uint8_t));

	// Initialize the two 8-bit values that make up each 16-bit value
	uint8_t LOWER_BYTE;
	uint8_t UPPER_BYTE;

	// Inform the user
	// printf("Reading file " KBLU "%s\n" RESET, INPUT_FILE_PATH);

	// Read the input binary file in 8-bit chunks
	fread(input_data, (int)sizeof(uint8_t), n_bytes_packed, input_file);

	// Initialize timer variables
	time_t tstart, tend;

	// Start a timer
	tstart = time(0);
	
	// Bit shift offset constant. This is the offset from the 
	//beginning of each byte  that start-bits may occur.
	const int bit_shift_constant = (2 * bytes_per_val_unpacked * bits_per_byte) - bits_per_val_packed;

	// Make an image in opencv 
	Mat slice(IMAGE_HEIGHT_PIXELS, IMAGE_WIDTH_PIXELS, CV_16UC1);

	// Declare pixel intensity
	uint16_t pixel_val;

	// Make a window
	namedWindow("Display Image", WINDOW_AUTOSIZE);
	
	// This is the bit within the binary file.
	int start_bit = 0;
	
	// This is the byte number containing the first bit in each 12-bit chunk of data.
	int start_byte;
	
	// Counter
	// int k;
	int start_pixel, end_pixel;	
	
	// File name stuff
	string dir = "/Users/matthewgiarra/Desktop/images/";
	string base = "image_";
	string ext = ".tif";
	string file_path;
	char numstr[5];
	
	// Loop over all the images specified, saving each one as a TIFF.
	for(int n = 0; n < number_of_images; n++){
				
		// Start and end pixels
		start_pixel = pixels_per_image * n;
		end_pixel  	= start_pixel + (pixels_per_image - 1);
		
		// Start bit within the binary file
		start_bit = bits_per_val_packed * start_pixel;
		
		// Index of the first relevant 8-byte within the binary
		start_byte = floor(start_bit / bits_per_byte);
		
		// Extract the 12-bit bytes out of the array for each image.	
		for(int k = start_pixel; k < end_pixel; k++){
					
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
			pixel_val = ((((uint16_t) 0 | UPPER_BYTE) << 8  ) | ((uint16_t) 0 | LOWER_BYTE)) << PIXEL_BIT_SHIFT;
		
			// Assign the pixel value to the image.
			slice.at<uint16_t>(k-start_pixel) = pixel_val;

			// Increment the start byte.
			start_byte += 2 * (k % 2) + 1 * (1 - k % 2);
		}
		
		// Show the image
		// imshow("Display Image", slice);
		
		// Hold your horses
		// waitKey(60);
	
		// Build save name
		sprintf(numstr, "%05.0f", (float)n + 1);
		
		// Inform user
		file_path = dir + base + numstr + ext;
		
		// Print the file name being saved
		printf("Saving file:" KBLU "%s\n" RESET, file_path.c_str());
		
		// Write the first image
		imwrite(file_path, slice);
		
	}
	// End time
	tend = time(0);
	
	// Elapsed time
	int elapsed = (int)tend - (int)tstart;
	
	// Display success
	printf("Saved " KGRN "%d " RESET "images " \
		"in " KGRN "%d " RESET "seconds " \
			"\nSave directory: " KGRN "%s\n" RESET, number_of_images, elapsed, dir.c_str());

	// Close files
	fclose(input_file);
	// fclose(output_file);

	// Free memory
	free(input_data);
	// free(output_data);

	// Return success/fail
	return(0);

}
