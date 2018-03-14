MYSRC=${SRC_PATH}/bin/lte_test

CFLAGS+=-I$(LTE_INCLUDE)
CFLAGS+=-I$(USB_INCLUDE)
CFLAGS+=-fPIC

MYLDFALGS=-L${LTE_LIB} -L$(USB_LIB)
MYLIBS=-llte -lusb-1.0

BINSRC=$(shell find $(MYSRC) -name *.c)
BINOBJ=$(patsubst %.c,%.o, $(BINSRC))

BINTARGET=lte_test

build: all

all:${BINTARGET}

${BINTARGET}: ${BINOBJ}
	$(CC) $^ -o $@ $(MYLDFALGS) ${MYLIBS}
	
install:
	mkdir -p ${BIN_PATH}/lte/bin/
	#cp -f ${MYSRC}/${BINTARGET} ${BIN_PATH}/lte/bin/
	
	
clean:
	find . -follow $(REMOVE_PATTERN) | xargs rm -fr
	rm -f ${BINTARGET}
