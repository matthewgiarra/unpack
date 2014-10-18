PROGRAM = main
DATA_DIR = ~/Documents/School/VT/Research/EFRI/Data/argonne_2014/mng-1-161-A
CC = g++
EXT = cpp
CFLAGS = -g `pkg-config --cflags --libs opencv`

all:
	$(CC) $(CFLAGS) $(PROGRAM).$(EXT) -o $(PROGRAM)
	
clean:
	@rm -f $(PROGRAM)
	@rm -rf *dSYM
run:
	@./$(PROGRAM) $(DATA_DIR)/mng-1-161-A.mraw 1024 1024 101 101 3