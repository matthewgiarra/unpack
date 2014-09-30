#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char * argv[]){
		
	int bits_per_val_packed = 12;
	int bytes_per_val_unpacked = (int) sizeof(uint16_t);
	int bits_per_byte = 8;
	
	// Number of 8-bit byte chunks to read from the original data
	double n_bytes_packed = 1024 * 1024 * 1;
	
	// This is the bit number at which each 12-bit chunk of data starts
	int start_bit_true = 0;
	
	// This is the byte number containing the first bit in each 12-bit chunk of data.
	int start_byte = 0;
	
	// Bit shift variable
	int bitShift = 0;
	
	// Allocate space in which to open the source data
	char *input_data;
	input_data = malloc(n_bytes_packed * bytes_per_val_unpacked);
	
	// Allocate space in which to save the new data
	char *output_data;
	output_data = malloc(n_bytes_packed * bytes_per_val_unpacked);
	
	// Allocate space for the 16-bit values
	char *data_bytes;
	data_bytes = malloc(sizeof(uint16_t));
	
	// Tell the user what's going on	
	printf("Unpacking %s\n", argv[1]);
	
	// Pointer to file
	FILE *input_file;
	FILE *output_file;
	
	// Open the input file for reading
	input_file = fopen(argv[1], "r");
	
	// Open an output file for writing, discarding any existing contents
	output_file = fopen(argv[2], "w");
	
	// Read the file
	fread(input_data, 1, n_bytes_packed, input_file);

	// Inform the user
	printf("Unpacking %0.0f bytes\n", n_bytes_packed);

	// Extract the 12-bit bytes out of the array
	for(int k = 0; k < n_bytes_packed; k++){
		
		// True bit number of the start byte 
		start_bit_true = k * bits_per_val_packed;
		
		// Byte number of the start byte
		start_byte = floor(start_bit_true / bits_per_byte);
		
		// Avoid divison by zero
		if(start_byte > 0)
			bitShift = fmod(start_bit_true, bits_per_byte * start_byte);
		
		// Case if bit shift is nonzer
		if(bitShift > 0){
			// Shift the bits
			data_bytes[0] = input_data[start_byte] << (bitShift) & ~(input_data[start_byte + 1] >> (bits_per_byte - bitShift));
			data_bytes[1] = input_data[start_byte + 1] << bitShift;	
		}
		else{
			// Shift and invert the second byte but not the first bit
			data_bytes[0] = input_data[start_byte];
			data_bytes[1] = ~(input_data[start_byte + 1] >> (bits_per_byte - bitShift));			
			}
			
			// Write the bytes to the output array
			output_data[2 * k] = data_bytes[0];
			output_data[2 * k - 1] = data_bytes[1];
		}
	
	
		// Inform user
		printf("Writing file %s\n", argv[2]);
		// Write the output file
		fwrite(output_data, 1, n_bytes_packed, output_file);
		
		// Close files
		fclose(input_file);
		fclose(output_file);
	
		// Free memory
		free(input_data);
		free(output_data);
		
	
	// Done!
	return(0);
}



