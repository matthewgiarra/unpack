#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char * argv[]){
		
	int bits_per_val_packed = 12;
	double bytes_per_val_unpacked = (int) sizeof(uint16_t);
	int bits_per_byte = 8;
	int height = 1024;
	int width = 1024;
	int nImages = 1;
	
	// Number of 8-bit byte chunks to read from the original data
	double nPixels = height * width * nImages;
	double n_bytes_unpacked = nPixels * (double)sizeof(uint16_t);
	double n_bytes_packed 	= nPixels * (double) bits_per_val_packed / (double) bits_per_byte;
	
	// Inform user
	printf("Packed bytes: %0.0f\t Unpacked bytes: %0.0f\n", n_bytes_packed, n_bytes_unpacked);
	
	// This is the bit number at which each 12-bit chunk of data starts
	int start_bit_true = 0;
	
	// This is the byte number containing the first bit in each 12-bit chunk of data.
	int start_byte = 0;
	
	// Bit shift variable
	int bitShift = 0;
	
	// Allocate space in which to open the source data
	char *input_data;
	input_data = malloc(n_bytes_packed * sizeof(uint8_t));
	
	// Allocate space in which to save the new data
	char *output_data;
	output_data = malloc(n_bytes_unpacked * sizeof(uint8_t));
	
	// Allocate space for the two 8-bit values that make up each 16-bit value
	char *data_bytes;
	data_bytes = malloc((double)sizeof(uint16_t) * sizeof(uint8_t));
	
	// Tell the user what's going on	
	// printf("Unpacking %s\n", argv[1]);
	
	// Pointer to file
	FILE *input_file;
	FILE *output_file;
	
	// Open the input file for reading
	input_file = fopen(argv[1], "r");
	
	// Open an output file for writing, discarding any existing contents
	output_file = fopen(argv[2], "w");
	
	// Read the file
	fread(input_data, 1, n_bytes_packed, input_file);

	// Extract the 12-bit bytes out of the array
	for(int k = 0; k < nPixels; k++){
		
		// True bit number of the start byte 
		start_bit_true = k * bits_per_val_packed;
		
		// Byte number of the start byte
		start_byte = floor(start_bit_true / bits_per_byte);
		
		// Avoid divison by zero
		if(start_byte > 0)
			bitShift = fmod(start_bit_true, bits_per_byte * start_byte);

		// Populate the new bytes
		data_bytes[0] = (input_data[start_byte] << bitShift) | (input_data[start_byte + 1] >> (bits_per_byte - bitShift));
		data_bytes[1] = (input_data[start_byte + 1] << bitShift) & ((int)(pow(2, bits_per_byte)-1) << (bits_per_val_packed - bits_per_byte));
		
		// printf("%d\n--\n", k);
		// for(int b = 0; b<bits_per_byte; b++){
		// 	printf("%d", (input_data[start_byte + 1] << bitShift & 128 >> b) > 0);
		// }
		// printf("\n");
		//
		// for(int b = 0; b<bits_per_byte; b++){
		// 	printf("%d", (((int)(pow(2, bits_per_byte)-1) << (bits_per_val_packed - bits_per_byte)) & 128 >>b ) > 0 );
		// }
		// printf("\n");
		//
		//
		// for(int b = 0; b < bits_per_byte; b++){
		// 	printf("%d", (data_bytes[1] & (128 >> b)) > 0);
		// }
		// printf("\n\n");
			
		// printf("%d\n--\n%d\n%d\n%d\n\n", k, input_data[start_byte + 1] << bitShift, bits_per_byte << (bits_per_val_packed - bits_per_byte), data_bytes[1]);

		// printf("Pixel %d Start bit: %d\tRead bytes: %d %d\tbitShift: %d\t", k, start_bit_true, start_byte, start_byte+1, bitShift);
		// printf("Input bytes: %d %d\t", input_data[start_byte], input_data[start_byte + 1]);
		//
		//
		// printf("Data bytes: %d %d\n", data_bytes[0], data_bytes[1]);
		
		// Write the bytes to the output array
		output_data[2 * k] = data_bytes[0];
		output_data[2 * k + 1] = data_bytes[1];
		
		// printf("Output bits: %d and %d\n", 2*k, 2*k+1);
	}
		
		// Inform user
		printf("Writing file %s\n", argv[2]);
		// Write the output file
		fwrite(output_data, 1, n_bytes_unpacked, output_file);
		
		// Close files
		fclose(input_file);
		fclose(output_file);
	
		// Free memory
		free(input_data);
		free(output_data);
		
	
	// Done!
	return(0);
}



