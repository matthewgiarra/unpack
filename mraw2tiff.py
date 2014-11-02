import re
import os
import subprocess

# This function parses a .cih file
# To do: Add functionalty to return the content of non-numeric fields.
def parse_cih(cih_file_path = None, field_name = None):
       
    # Skip everything if no field was specifed. 
    if not(field_name == None):    
    
            # Bug out if file name not specifed
            if cih_file_path == None:
                print "Error: specify an input file path.";
                return None;
                
            # Open the cih file to read
            searchfile = open(cih_file_path);
        
            # Search the fiel for the field name
            for line in searchfile:            
                # If the line was found, convert its value to an integer
                if field_name in line:
                    value = int(''.join(x for x in line if x.isdigit()));
                    return value;
        
            # Close the input file.
            searchfile.close();

# This function parses a CIH file and then uses a precompiled C function to convert an MRAW file to tiffs.
def mraw2tiff(data_input_dir = '.', cih_file_name = None, mraw_file_name = None, data_output_dir = '.', data_output_base_name = None, start_image = 0, end_image = None, bit_shift = 3, exec_path = 'mraw2tiff'):
    
    # Default to cih file name same as the data directory name
    if cih_file_name == None:
        cih_file_name = os.path.basename(os.path.normpath(data_input_dir)) + '.cih';
    
    # Default to mraw file name same as the data directory name
    if mraw_file_name == None:
        mraw_file_name = os.path.basename(os.path.normpath(data_input_dir)) + '.mraw';
        
    # Default to data base name same as data directory name
    if data_output_base_name == None:
        data_output_base_name = os.path.basename(os.path.normpath(data_input_dir)) + '_';
    
    # Path to the CIH file.
    cih_file_path = os.path.join(data_input_dir, cih_file_name);
    
    # Path to the MRAW file.
    mraw_file_path = os.path.join(data_input_dir, mraw_file_name);
    
    # Print an error message if the path to the Photron cih file doesn't exist.
    if not os.path.exists(cih_file_path):
        print "Error: " + cih_file_path + " does not exist."
    
    # Print an error message if the path to the Photron mraw file doesn't exist.
    elif not os.path.exists(mraw_file_path):
        print "Error: " + mraw_file_path + " does not exist."
    
    # If both the mraw and cih files exist, continue. 
    else:
        
        # Create the output directory if it doesn't already exist.
        if not os.path.exists(data_output_dir):
            os.makedirs(data_output_dir)
            
        # Determine image height and width (in pixels).
        image_num_rows = parse_cih(cih_file_path = cih_file_path, field_name='Image Height');
        image_num_columns = parse_cih(cih_file_path = cih_file_path, field_name='Image Width');
        
        # Determine number of frames.
        number_of_images = parse_cih(cih_file_path = cih_file_path, field_name='Original Total Frame');
        
        # Number of digits in file numbers
        number_of_digits = parse_cih(cih_file_path = cih_file_path, field_name = 'Digits Of File Number');
        
        # Default to saving all the images.
        if end_image == None:
            end_image = start_image + number_of_images - 1;
        
        # Print some outputs
        print "Start image: " + str(start_image);
        print "End image: " + str(end_image);
        print "Image height: " + str(image_num_rows);
        print "Image width: " + str(image_num_columns);
        print "Bit shift: " + str(bit_shift);
        print "Number of digits: " + str(number_of_digits);
        
        # Call the c function to extract the images!
        subprocess.call([exec_path, mraw_file_path, data_output_dir, data_output_base_name, str(image_num_rows), str(image_num_columns), str(start_image), str(end_image), str(bit_shift), str(number_of_digits)]);
    
################

##
# Below is a script that runs mraw2tiff. This is here temporarily for testing. To do: Move this out into a jobfile

# File path specifiers, etc
data_input_dir = '/Users/matthewgiarra/Documents/School/VT/Research/EFRI/Data/argonne_2014/mng-1-161-A/'
data_output_dir = os.path.join(data_input_dir, 'tiff/');
end_image = 10;

# Path to the compiled unpacking executable 
exec_path = '/Users/matthewgiarra/Dropbox/School/VT/Research/unpack/mraw2tiff'

# File name of the Photron CIH file.
cih_file_name = 'mng-1-161-A.cih'

# Run the python wrapper.
mraw2tiff(data_input_dir = data_input_dir, data_output_dir = data_output_dir, end_image = end_image, exec_path = exec_path);






