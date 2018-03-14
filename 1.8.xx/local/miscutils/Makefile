

#factory_env	-- nvramcli for factory mtdblock
#crc32			-- calculate CRC32 checksum 
SRC_DIRS:=factory_env crc32
BIN_DIR:=$(CURDIR)/install_bin

default: build install 

build:
	$(foreach sd,$(SRC_DIRS),$(MAKE) -C $(sd);)
	
install:
	@mkdir -p $(BIN_DIR);
	$(foreach sd,$(SRC_DIRS),$(MAKE) -C $(sd) BIN_DIR=$(BIN_DIR) install;)

clean:
	-rm -fr $(BIN_DIR);
	$(foreach sd,$(SRC_DIRS),$(MAKE) -C $(sd) clean;)

