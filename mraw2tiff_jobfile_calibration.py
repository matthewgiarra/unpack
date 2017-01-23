from mraw2tiff import mraw2tiff
import os
import pdb

# This is the path to the compiled unpacking executable.
# You shouldn't need to re-state this for every unpacking job;
# just set it once here at the top of the file, and it will work
# until you move the compiled code from the path specified.
exec_path = './mraw2tiff'

suppress_messages=0

data_input_dir = '/Volumes/duo/insects/analysis/data/xray_imaging/argonne_2014-07-21/data/grasshopper_imaging_2014-07-21/Photron_20x_calibration_400_lpi_mesh'

cih_file_name = 'Photron_20x_calibration_400_lpi_mesh.cih'
mraw_file_name = 'Photron_20x_calibration_400_lpi_mesh.mraw'
data_output_dir='/Users/matthewgiarra/Desktop/cal_images_20x'
data_output_base_name='Photron_20x_calibration_400_lpi_mesh_'
start_image = 0
end_image = 10
bit_shift = 3

mraw2tiff(data_input_dir = data_input_dir, cih_file_name = cih_file_name, mraw_file_name = mraw_file_name, data_output_dir = data_output_dir, data_output_base_name = data_output_base_name, start_image = start_image, end_image = end_image, bit_shift = bit_shift, exec_path = exec_path, suppress_messages=suppress_messages);