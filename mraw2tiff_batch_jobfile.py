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

# This is the path to the compiled unpacking executable.
# You shouldn't need to re-state this for every unpacking job;
# just set it once here at the top of the file, and it will work
# until you move the compiled code from the path specified.
exec_path = './mraw2tiff'

### JOB 1 ###
# These lines show how to specify an MRAW unpacking job
# without explicitly defining all of the input variables, and thereby
# allowing the code to make some assumptions about file names, etc.
# Whether or not this works will depend on whether or not
# the organization of your file system is consistent with the assumptions
# made by these codes, which are specified in the comments above (in Job 1)
#
# If successful, this job should unpack all of the images 
# contained in the MRAW file that resides in data_input_dir

# Data input repository.
# This is the directory that contains
# all the sub-directories which themselves
# contain the MRAW and CIH files. 
# For this batch file to work, 
# the MRAW and CIH file names have to 
# be the same as the directories in which
# they reside.
data_input_repository = '/input/data/repository';

# Data output repository.
# This is the directory in which sub-directories
# exist (or will be automatically created if
# they don't already exist) to store the 
# directories containing the exported TIFF images.
data_output_repository = '/output/data/repository/';

# Specify start and end images.
# This is temporary for just looking them over.
start_image = 0;
end_image = None;

# Suppress messages flag
suppress_messages = 1;

# Loop over the dirs in the rep
for case_name in os.listdir(data_input_repository):
    
    # Form the input directory name
    data_input_dir = os.path.join(data_input_repository, case_name);
    
    # Form the output directory name
    data_output_dir = os.path.join(data_output_repository, case_name, 'tiff/');
    
    # Run the python wrapper.
    mraw2tiff(data_input_dir = data_input_dir, data_output_dir = data_output_dir, start_image = start_image, end_image = end_image, exec_path = exec_path, suppress_messages = suppress_messages);
    
    


