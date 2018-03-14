#LTE_BIN_SUBDIRS=lte_startup lte_test checkalive ltecontrolclient gpsinfo simlock altairat tool
LTE_BIN_SUBDIRS=ltecomsrv tool ltecomsrv2 tool2
define make_bin
for subdir in $(1); \
  do                 \
    ( \
    make -C $$subdir $(2);                     \
     \
    );            \
  done	
endef

build:
	@$(call make_bin,$(LTE_BIN_SUBDIRS),build)

install:
	@$(call make_bin,$(LTE_BIN_SUBDIRS),install)

clean:
	@$(call make_bin,$(LTE_BIN_SUBDIRS),clean)
