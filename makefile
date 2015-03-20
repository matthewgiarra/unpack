PROGRAM = mraw2tiff
DATA_DIR = ~/Documents/School/VT/Research/EFRI/Data/argonne_2014/mng-1-161-A
CC = g++
EXT = cpp
CFLAGS = -I/usr/local/Cellar/libtiff/4.0.3/include -L/usr/local/Cellar/libtiff/4.0.3/lib -ltiff -ltiffxx


all:
	$(CC) $(CFLAGS) $(PROGRAM).$(EXT) -o $(PROGRAM)
	
clean:
	@rm -f $(PROGRAM)
	@rm -rf *dSYM
run:
	@./$(PROGRAM) $(DATA_DIR)/mng-1-161-A.mraw 1024 1024 1 100 3
	@./$(PROGRAM) $(DATA_DIR)/mng-1-161-A.mraw $(DATA_DIR)/mng-1-161-A.bin 1024 1024 10