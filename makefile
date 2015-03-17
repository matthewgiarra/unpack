PROGRAM = mraw2tiff
DATA_DIR = ~/Documents/School/VT/Research/EFRI/Data/argonne_2014/mng-1-161-A
CC = g++
EXT = cpp
CV_CFLAGS = -std=c++11 `pkg-config --cflags opencv`
CV_LDFLAGS = -L/usr/local/Cellar/opencv/2.4.9/lib		  
CV_LDLIBS = -lopencv_core  -lopencv_highgui

all:
	$(CC) $(CV_CFLAGS) $(CV_LDFLAGS) $(CV_LDLIBS) $(PROGRAM).$(EXT) -o $(PROGRAM)
	
clean:
	@rm -f $(PROGRAM)
	@rm -rf *dSYM
run:
	@./$(PROGRAM) $(DATA_DIR)/mng-1-161-A.mraw 1024 1024 1 100 3
	@./$(PROGRAM) $(DATA_DIR)/mng-1-161-A.mraw $(DATA_DIR)/mng-1-161-A.bin 1024 1024 10