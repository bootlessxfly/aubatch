OUTPUT=aubatch
SRC_DIR=src/
OUT_FOLDER=target/
OBJS=$(SRC_DIR)aubatch.c $(SRC_DIR)commandline_parser.c $(SRC_DIR)batch_job.c $(SRC_DIR)schedular.c $(SRC_DIR)jobs.h

RM=rm -rf
CC=gcc -g -o



all:build_aubatch

build_aubatch:
	@echo 'Building tool. Hope it works'
	if [ ! -d $(OUT_FOLDER) ]; then mkdir $(OUT_FOLDER); fi
	$(CC) $(OUT_FOLDER)$(OUTPUT) $(OBJS) -lpthread
	$(CC) $(OUT_FOLDER)process src/process.c
	@echo 'Compilation completed.'
	
clean:
	@echo 'Taking out the garbage'
	$(RM) $(OUT_FOLDER)