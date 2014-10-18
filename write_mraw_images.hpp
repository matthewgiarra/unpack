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
// USAGE: unpack_12To16(char* INPUT_FILE_PATH, int IMAGE_HEIGHT_PIXELS, int IMAGE_WIDTH_PIXELS, int number_of_images)

int write_mraw_12to16( char *INPUT_FILE_PATH, int IMAGE_HEIGHT_PIXELS, int IMAGE_WIDTH_PIXELS, int START_IMAGE, int END_IMAGE, int PIXEL_BIT_SHIFT){
	
	// Function prototypes
	unsigned long long int startByte(int IMAGE_HEIGHT, int IMAGE_WIDTH, int START_IMAGE, int BITS_PER_VAL_PACKED);
	
	// Initialize timer variables
	time_t tstart, tend;
	
	// Initialize the two 8-bit values that make up each 16-bit value
	uint8_t LOWER_BYTE;
	uint8_t UPPER_BYTE;
	
	// Bit shift variable
	int bitShift;
	
	// Pixels per image
	int pixels_per_image = IMAGE_HEIGHT_PIXELS * IMAGE_WIDTH_PIXELS;
	
	// Print
	printf("Start image: %d\t End image: %d\t\n", START_IMAGE, END_IMAGE);
	
	// Number of images to unpack and write
	int number_of_images = END_IMAGE - START_IMAGE + 1;
		
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

	// Make an image in opencv
	// Set image bit depth to 16. Hard code as grayscale. Allow color later.
	Mat slice(IMAGE_HEIGHT_PIXELS, IMAGE_WIDTH_PIXELS, CV_16UC1);

	// Declare pixel intensity
	uint16_t pixel_val;

	// Number of 8-bit bytes in the raw file
	unsigned long long int n_bytes_packed = nPixels * bits_per_val_packed / bits_per_byte;
	
	// File name stuff
	string dir = "/Users/matthewgiarra/Desktop/images/";
	string base = "image_";
	string ext = ".tif";
	string file_path;
	char numstr[5];

	// Pointers to files
	FILE *input_file;

	// Open the input file for reading
	input_file = fopen(INPUT_FILE_PATH, "r");
	
	// Bug out if the input file isn't found
	if(input_file == NULL){
		printf(KRED "Error: failed to load file" KBLU " %s\n" RESET, INPUT_FILE_PATH);
		return(-1);
	}

	// Allocate space in which to open the source data
	uint8_t *input_data = (uint8_t*) malloc(n_bytes_packed * sizeof(uint8_t));
	if(!input_data){
		printf(KRED "Error: Out of memory.\n" RESET);
	}
	
	// Start byte within the binary to read
	unsigned long long int start_byte = startByte(IMAGE_HEIGHT_PIXELS, IMAGE_WIDTH_PIXELS, START_IMAGE, bits_per_val_packed);

	// Read from the middle of the input file
	fseek(input_file, start_byte, SEEK_SET);

	// Read the input binary file in 8-bit chunks
	fread(input_data, (int)sizeof(uint8_t), n_bytes_packed, input_file);
		
	printf("Read file successfully.\n");
	
	// Start a timer
	tstart = time(0);

	// Make a window
	// namedWindow("Display Image", WINDOW_AUTOSIZE);

	// More declarations
	int start_bit, start_pixel, end_pixel;
	
	// Inform number of images to write
	printf("Number of images: %d\n", number_of_images);
	
	// Loop over all the images specified, saving each one as a TIFF.
	for(int image_num = 0; image_num < number_of_images; image_num++){
		
		// Start byte for this image
		start_byte = startByte(IMAGE_HEIGHT_PIXELS, IMAGE_WIDTH_PIXELS, image_num, bits_per_val_packed);
		
		// Start and end pixels
		start_pixel = pixels_per_image * (image_num + START_IMAGE);
		end_pixel  	= start_pixel + (pixels_per_image);
		
		// Loop over all pixels
		for(int pixel_num = 0; pixel_num < pixels_per_image; pixel_num++){
			// printf("Bit shift constant\n");
			// Bit shift
			bitShift = bit_shift_constant * (pixel_num % 2);
			
			// printf("Upper byte\n");
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

			// printf("Pixel value\n");
			// Combine 8-bit bytes into 16-bit byte
			pixel_val = ((((uint16_t) 0 | UPPER_BYTE) << 8  ) | ((uint16_t) 0 | LOWER_BYTE)) << PIXEL_BIT_SHIFT;
		
			// printf("Assign pixel\n");
			// Assign the pixel value to the image.
			slice.at<uint16_t>(pixel_num) = pixel_val;

			// printf("Increment start byte\n");
			// Increment the start byte.
			start_byte += 2 * (pixel_num % 2) + 1 * (1 - pixel_num % 2);
		}
		
		// Build save name
		sprintf(numstr, "%05d", START_IMAGE + image_num);

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

	// Free memory
	free(input_data);

	// Return success/fail
	return(0);
}

// This function returns the starting bit, at which to read an image
unsigned long long int startByte(int IMAGE_HEIGHT, int IMAGE_WIDTH, int START_IMAGE, int BITS_PER_VAL_PACKED){
	
	// Bits per byte
	const int bits_per_byte = 8;
	
	// Index of the first relevant 8-byte within the binary
	unsigned long long int start_byte = floor(BITS_PER_VAL_PACKED * IMAGE_HEIGHT * IMAGE_WIDTH * START_IMAGE / bits_per_byte);
	
	// unsigned long long int start_byte = floor(start_bit / bits_per_byte);
		
	// Return the start byte number.
	return start_byte;	
}
