Matthew Giarra
Virginia Tech
Department of Mechanical Engineering
matthew.giarra@gmail.com
19 March 2015

This repository, called "unpack," contains codes written in C++ and Python to extract the contents of 12-bit packed binary files (specifically Photron MRAW files) and save them as a series of 16-bit grayscale TIFF images.

To compile the C++ codes, navigate a terminal to the directory containing the file called "makefile" and type "make", which will call the compiler and produce the executable files that perform the unpacking operations. You may need to modify the field "CFLAGS" in the makefile to specify the paths on your system to the LIBTIFF header files and libraries.

Once the C++ codes are compiled, you can interact with them using the Python script / wrapper called mraw2tiff_jobfile.py. That script serves as a "job file" for specifying the parameters such as file paths, etc., that are necessary to perform the unpacking. It is the only script you should need to interact with to successfully unpack MRAW files into TIFF images. As such, a detailed description of its usage is given, along with examples, within the file itself. It's recommended that you make a copy of mraw2tiff_jobfile.py to serve as a reference for constructing your own jobfiles later on.  

These codes require LIBTIFF to operate. Mac users can install LIBTIFF using homebrew by running the command:

	brew install libtiff
	
Ubuntu users can install LIBTIFF through apt by running the command:

	sudo apt-get install libtiff5
	
Users of Purdue's Redhat linux machines are fortunate that libtiff is already installed for them,
and the makefile should be modified accordingly. To do this, change the line in the makefile that begins 
with "CFLAGS = ..." to:

	CFLAGS = -I/usr/include -L/usr/lib -ltiff
	
After that, running the command:
	
	make
	
"should" compile and link everything correctly.
	
Windows users, you're on your own unfortunately.

Dependencies:
	LIBTIFF