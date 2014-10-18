#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "write_mraw_images.hpp"

int main(int argc, char *argv[]){
	
	// File paths
	char *INPUT_FILE_PATH = argv[1];
	
	// Height and width of the images in pixels
	int IMAGE_HEIGHT_PIXELS = atoi(argv[2]);
	int IMAGE_WIDTH_PIXELS = atoi(argv[3]);
	int START_IMAGE = atoi(argv[4]);
	int END_IMAGE = atoi(argv[5]);
	int BIT_SHIFT = atoi(argv[6]);
		
	// Extract images
	write_mraw_12to16(INPUT_FILE_PATH, IMAGE_HEIGHT_PIXELS, IMAGE_WIDTH_PIXELS, START_IMAGE, END_IMAGE, BIT_SHIFT);
	
	// GTFO
	return(0);
	
}












