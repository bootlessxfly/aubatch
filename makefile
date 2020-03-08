OUTPUT=aubatch
SRC_DIR=src/
OUT_FOLDER=target/
OBJS=$(SRC_DIR)aubatch.c $(SRC_DIR)commandline_parser.c

RM=rm -rf
CC=gcc -o



all:build_aubatch

build_aubatch:
	@echo 'Building tool. Hope it works'
	if [ ! -d $(OUT_FOLDER) ]; then mkdir $(OUT_FOLDER); fi
	$(CC) $(OUT_FOLDER)$(OUTPUT) $(OBJS)
	@echo 'Compilation completed.'
	
clean:
	@echo 'Taking out the garbage'
	$(RM) $(OUT_FOLDER)