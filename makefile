OUTPUT=aubatch
SRC_DIR=src/
OUT_FOLDER=target/
OBJS=$(SRC_DIR)aubatch.c $(SRC_DIR)commandline_parser.c



all:build_aubatch

build_aubatch:
	@echo 'Building tool. Hope it works'
	if [ ! -d $(OUT_FOLDER) ]; then mkdir $(OUT_FOLDER); fi
	gcc -o $(OUT_FOLDER)$(OUTPUT) $(OBJS)
	@echo 'Compilation completed.'
	
clean:
	@echo 'Taking out the garbage'
	rm -rf $(OUT_FOLDER)