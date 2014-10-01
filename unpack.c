#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// This function unpacks 12-bit data into 16-bit data.
// At least that's the plan.

int main(int argc, char * argv[]){
	
	// Data size variables from input
	// int height = atoi(argv[3]);
	// int width = atoi(argv[4]);
	// int nImages = atoi(argv[5]);
	// int bits_per_val_packed = atoi(argv[6]);
		
	int bits_per_val_packed = 12;
	int bytes_per_val_unpacked = (int)sizeof(uint8_t);
	int bits_per_byte = 8;
	int height = 1024;
	int width = 1024;
	int nImages = 10;
	
	// Number of pixel values in the whole data set (image dimensions * number of images)
	double nPixels = height * width * nImages;
	
	// Number of 8-bit bytes in the output file.
	double n_bytes_unpacked = (int)sizeof(uint16_t) * nPixels;
	
	// Number of 8-bit bytes in the raw file
	double n_bytes_packed = nPixels * bits_per_val_packed / bits_per_byte;
	
	// Inform user
	printf("Packed bytes: %0.0f\t Unpacked bytes: %0.0f\n", n_bytes_packed, n_bytes_unpacked);
	
	// This is the bit number at which each 12-bit chunk of data starts
	int start_bit_true = 0;
	
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
		
	// Pointers to files
	FILE *input_file;
	FILE *output_file;
	
	// Open the input file for reading
	input_file = fopen(argv[1], "r");
	
	// Open an output file for writing, discarding any existing contents
	output_file = fopen(argv[2], "w");		
		
	// Read the input binary file in 8-bit chunks
	fread(input_data, (int)sizeof(uint8_t), n_bytes_packed, input_file);

	double image_num;
	
	// Extract the 12-bit bytes out of the array
	for(int k = 0; k < nPixels; k++){
		
		// Current image number
		image_num = (double)k / ( (double)width * (double)height) + 1;
		
		// Print a message at the start of each new image.
		if(fmod(image_num, 1) == 0)
			printf("Image %0.0f of %d\n",  image_num, nImages);
		
		// Bit number of the start byte within the whole array
		start_bit_true = k * bits_per_val_packed;
		
		// Byte number of the start byte within the whole array
		start_byte = floor(start_bit_true / bits_per_byte);
		
		// Avoid divison by zero
		if(start_byte > 0)
			bitShift = fmod(start_bit_true, bits_per_byte * start_byte);
		
		// Populate the new bytes
		// Most significant bits (MSB) in big-endian format.
		// This shifts the first 8-bit byte to the left by BITSHIFT bits, and then 
		// concatonates it with the first (BITS_PER_BYTE - BITSHIFT) bits of the subsequent byte.
		UPPER_BYTE = (input_data[start_byte] << (int)bitShift) | (input_data[start_byte + 1] >> ((int)bits_per_byte - (int)bitShift));
		
		// Least significant bit (LSB) in bit-endian format.
		// This shifts the second 8-bit byte by (8 - bitShift) bits to the left 
		// and then truncates its to the first (bits_per_val_packed - bits_per_byte) bits 
		// (e.g. bits_per_val_packed = 12; bits_per_byte = 8; so the second 8-bit byte is truncated to the first (12-8) = 4 bits).
		LOWER_BYTE = (input_data[start_byte + 1] << bitShift) & ((uint8_t)(pow(2, bits_per_byte) - 1) << ((int)bits_per_val_packed - (int)bits_per_byte));
		
		// Assign the 16-bit output byte to the output data array.
		// output_data[2 * k] = LOWER_BYTE;
		// output_data[2 * k + 1]  = UPPER_BYTE;
				
		output_data[2 * k] = LOWER_BYTE;
		output_data[2 * k + 1]  = UPPER_BYTE;
				
	}
		
	// Write the output file
	fwrite(output_data, (int)sizeof(uint8_t), 2 * nPixels, output_file);
		
	// Close files
	fclose(input_file);
	fclose(output_file);
	
	// Free memory
	free(input_data);
	free(output_data);
			
	// GTFO
	return(0);
}



