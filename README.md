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
		
Users of Purdue's Redhat linux machines are fortunate that libtiff is already installed for them,
and the makefile should be modified accordingly. To do this, change the line in the makefile that begins 
with "CFLAGS = ..." to:

	CFLAGS = -std=c++0x -I/usr/local/include -L/usr/lib64 -ltiff
	
After that, running the command:
	
	make
	
"should" compile and link everything correctly. Conveniently, this line is already implemented (and commented out) in the provided makefile, so you just need to un-comment the appropriate CFLAGS line to compile on your system.



Ubuntu users can install LIBTIFF through apt by running the command:

	sudo apt-get install libtiff5
	
However, I haven't been able to compile these codes on Ubuntu; the compiler flags I've tested are

	CFLAGS = -std=c++11 -I/usr/include/x86_64-linux-gnu -L/usr/lib/x86_64-linux-gnu -ltiff
	
And similarly with 

	CFLAGS = -std=c++0x ...
	
The error that I get is that a bunch of functions are undefined:

	mraw2tiff.cpp:(.text+0x91a): undefined reference to `TIFFOpen'
	mraw2tiff.cpp:(.text+0x937): undefined reference to `TIFFSetField'
	mraw2tiff.cpp:(.text+0x950): undefined reference to `TIFFSetField'
	mraw2tiff.cpp:(.text+0x96b): undefined reference to `TIFFSetField'
	mraw2tiff.cpp:(.text+0x986): undefined reference to `TIFFSetField'
	mraw2tiff.cpp:(.text+0x9a1): undefined reference to `TIFFSetField'
	
However, it appears as though linking the libraries themselves succeeds, because I can successfuly compile a test code
that includes 

	#include "tiffio.h"
	#include "tiff.h"
	
as long as none of the LIBTIFF functions are called within the test code. So obviously this is not very useful, but perhaps it's a clue as to what's going wrong. 

Windows users, you're on your own unfortunately.

Dependencies:
	LIBTIFF 4+
	
	
	
	