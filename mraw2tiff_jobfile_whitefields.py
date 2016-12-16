# mraw2tiff_jobfile.py
#
# Matthew N. Giarra
# Virginia Tech
# Department of Mechanical Engineering
# matthew.giarra@gmail.com
# 19 March 2015
#
# This Python script serves as a "job file" for specifying 
# parameters for unpacking 12-bit grayscale Photron MRAW files
# into a series 16-bit grayscale TIFF images. This is the only
# script you should need to interact with to successfully unpack
# MRAW files into TIFF images.

from mraw2tiff import mraw2tiff
import os
import pdb

# This is the path to the compiled unpacking executable.
# You shouldn't need to re-state this for every unpacking job;
# just set it once here at the top of the file, and it will work
# until you move the compiled code from the path specified.
exec_path = './mraw2tiff'

######

data_input_parent_dir = '/Volumes/duo/insects/analysis/data/xray_imaging/argonne_2015-06-05/argonne_2015-06-05_grasshoppers'
data_output_parent_dir = '/Users/matthewgiarra/Documents/School/VT/Research/Aether/piv_test_images/grasshopper'


# This is the number of the first image to export.
# Image 0 corresponds to the first image.
start_image = 0;

# This is the number of the final image to export.
# To export all the images in the MRAW file without
# a-priori specifying the end image number,
# set end_image = None
end_image = 100;

# This is the bit-shift of the intensity values for the output images.
# This shifts the position of the twelve image data bits in the MRAW data
# within the sixteen bits of the output images. This field does not
# cause any data to be truncated, or affect the amount of information
# stored in the exported TIFFs; rather, it only affects how the images are
# displayed visually on your system. Setting bit_shift to higher values
# causes the images to appear brighter. A suggested default setting
# is bit_shift = 3
bit_shift = 3;

# This is binary flag (0 or 1) specifying whether to suppress (1) or
# allow (0) the console's text updates when each image is saved.
# Suppressing messages reduces the code's execution speed by about 50%,
# but provides the comfort of seeing where all your images are going.
# Recommendation is to allow message when debugging jobfiles and then
# to suppress messages for batch-running jobfiles.
# The default setting is to not suppress messages, i.e., suppress_messages = 0;
suppress_messages = 1;


# Here is where we specify the list of cases to export.
animal_nums = [];
trial_names = [];

# Repeat this block for each case
#
# 

##############
##############
## ANIMAL 1 ##
##############
##############

# # #
animal_num = 1;
trial_name = 'mng-2-071-F_white_field';
animal_nums.append(animal_num);
trial_names.append(trial_name);


##############
##############
## ANIMAL 2 ##
##############
##############

# # # #
animal_num = 2;
trial_name = 'mng-2-071-K_white_field';
animal_nums.append(animal_num);
trial_names.append(trial_name);



##############
##############
## ANIMAL 3 ##
##############
##############

# # # #
animal_num = 3;
trial_name = 'mng-2-072-E_white_field';
animal_nums.append(animal_num);
trial_names.append(trial_name);



##############
##############
## ANIMAL 4 ##
##############
##############


# # # #
animal_num = 4;
trial_name = 'mng-2-072-K_white_field';
animal_nums.append(animal_num);
trial_names.append(trial_name);


##############
##############
## ANIMAL 5 ##
##############
##############

# # # #
animal_num = 5;
trial_name = 'mng-2-073-E_white_field';
animal_nums.append(animal_num);
trial_names.append(trial_name);


##############
##############
## ANIMAL 6 ##
##############
##############

# # # #
animal_num = 6;
trial_name = 'mng-2-073-I_white_field';
animal_nums.append(animal_num);
trial_names.append(trial_name);


##############
##############
## ANIMAL 7 ##
##############
##############

# # # #
animal_num = 7;
trial_name = 'mng-2-073-N_white_field';
animal_nums.append(animal_num);
trial_names.append(trial_name);


# Count the number of trials
num_trials = len(trial_names)

# Export all the images.
for k in list(range(num_trials)):
    animal_name = "grasshopper_" + str(animal_nums[k])
    animal_dir = os.path.join(data_input_parent_dir, animal_name)

    # Get all the names of the dirs in this folder
    trial_name = trial_names[k]
    
    # Specity the data input directory
    data_input_dir = os.path.join(animal_dir, trial_name)
    
    # Specify the output data directory
    data_output_dir = os.path.join(data_output_parent_dir, animal_name, trial_name, "raw")
    
    # Add an underscore to the data output base name
    data_output_base_name = trial_name + "_"
    
    # Make the dir if it doesn't already exist
    os.makedirs(data_output_dir, exist_ok=True)
    
    # Specify the CIH and mraw file names to be the same as the trial name.
    cih_file_name  = trial_name + ".cih"
    mraw_file_name = trial_name + ".mraw"
    
    # Some printing
    print("CIH file name: " + cih_file_name)
    print("mraw file name: " + mraw_file_name)
    
    # Run the python wrapper.
    mraw2tiff(data_input_dir = data_input_dir, cih_file_name = cih_file_name, mraw_file_name = mraw_file_name, data_output_dir = data_output_dir, data_output_base_name = data_output_base_name, start_image = start_image, end_image = end_image, bit_shift = bit_shift, exec_path = exec_path, suppress_messages=suppress_messages);

  


