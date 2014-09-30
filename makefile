CC = gcc

all:
	$(CC) unpack.c -o unpack
	
clean:
	rm -f unpack
	
run:
	make clean
	make
	./unpack ~/Desktop/mng-1-161-A/mng-1-161-A.mraw ~/Desktop/mng-1-161-A/mng-1-161-A.bin