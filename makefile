PROGRAM = mraw2tiff
CC = g++
EXT = cpp
CFLAGS = -I/usr/local/Cellar/libtiff/4.0.3/include -L/usr/local/Cellar/libtiff/4.0.3/lib -ltiff


all:
	$(CC) $(CFLAGS) $(PROGRAM).$(EXT) -o $(PROGRAM)
	
clean:
	@rm -f $(PROGRAM)
	@rm -rf *dSYM
