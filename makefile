CC = gcc
DATA_DIR = ~/Documents/School/VT/Research/EFRI/Data/argonne_2014/mng-1-161-A

all:
	@$(CC) unpack.c -o unpack
	
clean:
	@rm -f unpack
	
run:
	@make clean
	@make
	@./unpack $(DATA_DIR)/mng-1-161-A.mraw $(DATA_DIR)/mng-1-161-A.bin 1024 1024 100 12