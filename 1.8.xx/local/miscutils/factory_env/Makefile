


nvram_cli: nvram_flash.o nvram_flash.h

nvram_flash.o: nvram_flash.h

build: nvram_cli

install:
	-cp nvram_cli $(BIN_DIR)/factory_env;
	-cd $(BIN_DIR) && ln -sfn factory_env nvram_cli;

clean:
	-rm *.o nvram_cli

