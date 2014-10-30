CC = g++
EXT = cpp
PROGRAM = main
DATA_DIR = /Users/matthewgiarra/Documents/School/VT/Research/EFRI/Data/argonne_2014/mng-1-161-A
FILE_NAME = mng-1-161-A.cih
REG_EXP = Start Frame

all:
	@$(CC) $(PROGRAM).$(EXT) -o $(PROGRAM)
	
clean:
	@rm -f $(PROGRAM)

run:
	@make clean
	@make
	./$(PROGRAM) $(DATA_DIR)/$(FILE_NAME) 