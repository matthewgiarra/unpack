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
data_output_parent_dir = '/Users/matthewgiarra/Desktop/argonne_2015-06-05_grasshoppers'


# This is the number of the first image to export.
# Image 0 corresponds to the first image.
start_image = 0;

# This is the number of the final image to export.
# To export all the images in the MRAW file without
# a-priori specifying the end image number,
# set end_image = None
end_image = 1000;

# This is the bit-shift of the intensity values for the output images.
# This shifts the position of the twelve image data bits in the MRAW data
# within the sixteen bits of the output images. This field does not
# cause any data to be truncated, or affect the amount of information
# stored in the exported TIFFs; rather, it only affects how the images are
# displayed visually on your system. Setting bit_shift to higher values
# causes the images to appear brighter. A suggested default setting
# is bit_shift = 3
bit_shift = 4;

# This is binary flag (0 or 1) specifying whether to suppress (1) or
# allow (0) the console's text updates when each image is saved.
# Suppressing messages reduces the code's execution speed by about 50%,
# but provides the comfort of seeing where all your images are going.
# Recommendation is to allow message when debugging jobfiles and then
# to suppress messages for batch-running jobfiles.
# The default setting is to not suppress messages, i.e., suppress_messages = 0;
suppress_messages = 0;

# Animal numbers
animal_nums = list(range(7,8))

for k in animal_nums:
    animal_name = "grasshopper_" + str(k)
    animal_dir = os.path.join(data_input_parent_dir, animal_name)
    
    # Get all the names of the dirs in this folder
    trial_names = os.listdir(animal_dir)
    
    # Loop over this directory
    for m in trial_names:
        if not ".DS" in m:
            # Specity the data input directory
            data_input_dir = os.path.join(animal_dir, m)
            
            # Specify the output data directory
            data_output_dir = os.path.join(data_output_parent_dir, animal_name, m)
            
            # Make the dir if it doesn't already exist
            os.makedirs(data_output_dir, exist_ok=True)
            
            # Specify the CIH and mraw file names to be the same as the trial name.
            cih_file_name = m + ".cih"
            mraw_file_name = m + ".mraw"
            
            # Some printing
            print("CIH file name: " + cih_file_name)
            print("mraw file name: " + mraw_file_name)
            
            # Run the python wrapper.
            mraw2tiff(data_input_dir = data_input_dir, cih_file_name = cih_file_name, mraw_file_name = mraw_file_name, data_output_dir = data_output_dir, start_image = start_image, end_image = end_image, bit_shift = bit_shift, exec_path = exec_path, suppress_messages=suppress_messages);

            
            
            
#
# #
# ### JOB 1 ###
# # This is the path to the directory containing the
# # Photron MRAW file that you want to unpack, as well
# # as its corresponding .CIH file.
# data_input_dir = '/Volumes/duo/insects/analysis/data/xray_imaging/argonne_2015-06-05/argonne_2015-06-05_grasshoppers/grasshopper_1/mng_2-071-C_'
# # This is the path to the directory in which the output
# # TIFF images will be saved. By default (e.g. as written here)
# # the images will be saved in data_input_dir/tiff. If you want to
# # save the images in a different directory, just replace
# # the line below with:
# # data_output_dir = PATH_TO_YOUR_OUTPUT_DIRECTORY
# # data_output_dir = os.path.join(data_input_dir, 'tiff/');
# data_output_dir = '/Users/matthewgiarra/Desktop/grasshopper_tiff/'
#
#
#
# # This is the file name of the Photron CIH file
# # corresponding to the MRAW file that you want to unpack.
# # If you choose not to specify cih_file_name
# # (or set its value to None), the code
# # assumes that the name of the CIH file is the same
# # as the name of its parent folder (data_input_dir),
# # e.g., if data_input_dir = '~/Desktop/my_images,
# # and cih_file_name is not specified, then
# # the code will search for a cih file on
# # the path ~/Desktop/my_images/my_images.cih
# cih_file_name = 'mng-1-159-B.cih'
#
# # This is the file name of the Photron MRAW file
# # that you want to unpack.
# # If you choose not to specify mraw_file_name
# # (or set its value to None), the code
# # assumes that the name of the MRAW file is the same
# # as the name of its parent folder (data_input_dir),
# # e.g., if data_input_dir = '~/Desktop/my_images,
# # and mraw_file_name is not specified, then
# # the code will search for an MRAW file on
# # the path ~/Desktop/my_images/my_images.mraw
# mraw_file_name = 'mng-1-159-B.mraw'
#
# # This is the base-name of the images to be output. E.g.,
# # if you want to output images called my_images_00001.tiff, my_images_00002.tiff,
# # then set data_output_base_name = 'my_images_'
# # If you choose not to specify data_output_base_name
# # (or set its value to None),
# # the code will automatically set the base names
# # of the output images to the same name as the
# # data input directory data_input_dir
# data_output_base_name = 'photron_images_01_'
#






### JOB 2 ###
# These lines show how to specify an MRAW unpacking job
# without explicitly defining all of the input variables, and thereby
# allowing the code to make some assumptions about file names, etc.
# Whether or not this works will depend on whether or not
# the organization of your file system is consistent with the assumptions
# made by these codes, which are specified in the comments above (in Job 1)
#
# If successful, this job should unpack all of the images 
# contained in the MRAW file that resides in data_input_dir
#
# # Data input directory
# data_input_dir = '~/photron_images_02'
#
# # Data output dir
# data_output_dir = os.path.join(data_input_dir, 'tiff/');
#
# # Suppress messages flag
# suppress_messages = 1;
#
# # Run the python wrapper.
# mraw2tiff(data_input_dir = data_input_dir, data_output_dir = data_output_dir, exec_path = exec_path, suppress_messages = suppress_messages);
#
#


