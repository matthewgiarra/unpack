#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "unpack.h"

int main(int argc, char *argv[]){
	
	// File paths
	char *INPUT_FILE_PATH = argv[1];
	char *OUTPUT_FILE_PATH = argv[2];
	
	// Height and width of the images in pixels
	int IMAGE_HEIGHT_PIXELS = atoi(argv[3]);
	int IMAGE_WIDTH_PIXELS = atoi(argv[4]);
	
	// Number of images
	int NUMBER_OF_IMAGES = atoi(argv[5]);
	
	// Unpack the binary file
	unpack_12To16(INPUT_FILE_PATH, OUTPUT_FILE_PATH, IMAGE_HEIGHT_PIXELS, IMAGE_WIDTH_PIXELS, NUMBER_OF_IMAGES);
	
	// GTFO
	return(0);
	
}












