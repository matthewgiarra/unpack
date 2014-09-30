CC = gcc

all:
	$(CC) unpack.c -o unpack
	
clean:
	rm -f unpack
	
run:
	make clean
	make
	./unpack /Volumes/Socha_MP7/giarra_grasshopper_2014-07-21/mng-1-161-A_C001H001S0001/mng-1-161-A_C001H001S0001.mraw ~/Desktop/data.bin