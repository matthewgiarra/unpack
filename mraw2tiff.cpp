#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "write_mraw_images.hpp"

int main(int argc, char *argv[]){
	
	// File paths
	std::string INPUT_FILE_PATH = argv[1];
	
	// Output file directory
	std::string OUTPUT_FILE_DIR = argv[2];
	
	// Output file base name
	std::string OUTPUT_FILE_BASE = argv[3];
	
	// Height and width of the images in pixels
	int IMAGE_HEIGHT_PIXELS = atoi(argv[4]);
	int IMAGE_WIDTH_PIXELS = atoi(argv[5]);
	
	// Start and end images
	int START_IMAGE = atoi(argv[6]);
	int END_IMAGE = atoi(argv[7]);
	
	// Pixel bit shift
	int PIXEL_BIT_SHIFT = atoi(argv[8]);
	
	// Number of digits in the saved file names.
	int FILE_DIGITS = atoi(argv[9]);
	
	// File extension
	std::string FILE_EXTENSION = argv[10];
	
	// Message suppression flag
	int SUPPRESS_MESSAGES = atoi(argv[11]);
		
	// Extract images
	write_mraw_12to16(INPUT_FILE_PATH, OUTPUT_FILE_DIR, OUTPUT_FILE_BASE, IMAGE_HEIGHT_PIXELS, IMAGE_WIDTH_PIXELS, START_IMAGE, END_IMAGE, PIXEL_BIT_SHIFT, FILE_DIGITS, FILE_EXTENSION, SUPPRESS_MESSAGES);
	
	// GTFO
	return(0);
	
}












