# mraw2tiff.py

# Matthew N. Giarra
# Virginia Tech
# Department of Mechanical Engineering
# matthew.giarra@gmail.com
# 19 March 2015

# This function serves to interface with the compiled 
# C++ codes for unpacking 12-bit Photron MRAW files
# into series of 16-bit grayscale TIFF images.
#
# This function reads and parses the Photron CIH
# file corresponding to the MRAW file to be unpacked
# in order to read image properties such as image
# dimensions, etc. Then it calls the executable
# file compiled from the C++ code mraw2tiff.cpp
# to perform the image unpacking.  

# Import libraries
import re
import os
import sys
import subprocess
from os.path import expanduser
import pdb

# This function parses a .cih file
# To do: Add functionalty to return the content of non-numeric fields.
def parse_cih(cih_file_path = None, field_name = None):
           
    # Skip everything if no field was specifed. 
    if not(field_name == None):    
    
            # Bug out if file name not specifed
            if cih_file_path == None:
                print("Error: specify an input file path.")
                return None;
                
            # Open the cih file to read
            searchfile = open(cih_file_path);
        
            # Search the field for the field name
            for line in searchfile:            
                # If the line was found, convert its value to an integer
                if field_name in line:
                    value = int(''.join(x for x in line if x.isdigit()));
                    return value;
        
            # Close the input file.
            searchfile.close();

# This function parses a CIH file and then uses a precompiled C function to convert an MRAW file to tiffs.
def mraw2tiff(data_input_dir = '.', cih_file_name = None, mraw_file_name = None, data_output_dir = '.', data_output_base_name = None, start_image = 0, end_image = None, bit_shift = 3, exec_path = 'mraw2tiff', file_extension = '.tiff', suppress_messages = 0):
        
    # Some colors for printing
    # These are ANSI escape sequences.
    KRED  = "\x1B[31m";
    KBLU = "\x1B[34m";
    KCYN  = "\x1B[36m";
    RESET = "\033[0m";
    
    # Expand the paths to the data input directory
    data_input_dir = os.path.expanduser(data_input_dir);
    
    # Expand the path to the data output directory.
    data_output_dir = os.path.join(os.path.expanduser(data_output_dir), '');
    
    # Default to cih file name same as the data directory name
    if cih_file_name == None:
        cih_file_name = os.path.basename(os.path.normpath(data_input_dir)) + '.cih';
    
    # Default to mraw file name same as the data directory name
    if mraw_file_name == None:
        mraw_file_name = os.path.basename(os.path.normpath(data_input_dir)) + '.mraw';
        
    # Default to data base name same as data directory name
    if data_output_base_name == None:
        data_output_base_name = os.path.basename(os.path.normpath(data_input_dir));
    
    # Path to the CIH file.
    cih_file_path = os.path.join(data_input_dir, cih_file_name);
    
    # Path to the MRAW file.
    mraw_file_path = os.path.join(data_input_dir, mraw_file_name);
         
    # Print an error message if the path to the Photron cih file doesn't exist.
    if not os.path.exists(cih_file_path):
        print(KRED + "Error: " + RESET + cih_file_path + KRED + " not found." + RESET)
    
    # Print an error message if the path to the Photron mraw file doesn't exist.
    elif not os.path.exists(mraw_file_path):
        print(KRED + "Error: " + RESET + mraw_file_path + KRED + " not found." + RESET)
        
    # Throw an error if the compiled unpacking code doesn't exist
    elif not os.path.exists(exec_path):
        print(KRED + "Error: the compiled code " + RESET + exec_path + KRED + " does not exist at the path specified.\nCompile using the supplied makefile, e.g., by typing \'make\' \n" + RESET)
    
    # If both the mraw and cih files exist, continue. 
    else:
        
        # Create the output directory if it doesn't already exist.
        if not os.path.exists(data_output_dir):
            os.makedirs(data_output_dir)
            
        # Determine image height and width (in pixels).
        image_num_rows = parse_cih(cih_file_path = cih_file_path, field_name='Image Height :');
        image_num_columns = parse_cih(cih_file_path = cih_file_path, field_name='Image Width :');
        
        # Determine number of frames.
        number_of_images = parse_cih(cih_file_path = cih_file_path, field_name='Original Total Frame :');
        
        # Throw an error if the specified start image is greater than the total number of images recorded.
        if start_image > number_of_images:
            print(KRED + "Error: Specified start image number (" + RESET + str(start_image) + KRED + ") is greater than the total number of images in the MRAW file (" + RESET + str(number_of_images) + KRED + ")\n" + RESET)
            sys.exit()
            
            # Throw an error if the starting number is greater than the ending number.
        elif ((not(end_image == None) and (start_image > end_image))):
            print(KRED + "Error: specified start image number (" + RESET + str(start_image) + KRED + ") is greater than specified end image number (" + RESET + str(end_image) + KRED + ")\n" + RESET)
            sys.exit()
        
        # Number of digits in file numbers
        number_of_digits = parse_cih(cih_file_path = cih_file_path, field_name = 'Digits Of File Number :');
        
        # Default to saving all the images.
        if (end_image == None) or (end_image > number_of_images):
            end_image = number_of_images - 1;
        
        # Print some outputs
        print(KBLU + "\nUnpacking file: " + RESET + mraw_file_path + RESET);  
        print( KBLU + "Start image: " + RESET +  str(start_image));
        print(KBLU + "End image: " + RESET + str(end_image));
        print(KBLU + "Images to unpack: " + RESET +  str(end_image - start_image + 1));
        print(KBLU + "Images in MRAW file: " + RESET + str(number_of_images));
        print(KBLU + "Image height: " + RESET +  str(image_num_rows));
        print(KBLU + "Image width: " + RESET +  str(image_num_columns));
        print(KBLU + "Bit shift: " + RESET +  str(bit_shift) + "\n");
        
        # Display a warning message if message suppression is enabled.
        if suppress_messages:
            print("Messages suppressed. Unpacking file ...\n")
        
        # Call the c function to extract the images!
        subprocess.call([exec_path, mraw_file_path, data_output_dir, data_output_base_name, str(image_num_rows), str(image_num_columns), str(start_image), str(end_image), str(bit_shift), str(number_of_digits), file_extension, str(suppress_messages)]);
    







