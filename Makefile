ifeq ($(strip $(LIBTRANSISTOR_HOME)),)
$(error "Please set LIBTRANSISTOR_HOME in your environment. export LIBTRANSISTOR_HOME=<path>")
endif

PROGRAM := atari800
ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

include $(LIBTRANSISTOR_HOME)/libtransistor.mk
export LD
export CC
export CXX
export AS
export AR
export LD_FOR_TARGET = $(LD)
export CC_FOR_TARGET = $(CC)
export AS_FOR_TARGET = $(AS) -arch=aarch64 -mattr=+neon
export AR_FOR_TARGET = $(AR)
export RANLIB_FOR_TARGET = llvm-ranlib$(LLVM_POSTFIX)
export CFLAGS_FOR_TARGET = $(CC_FLAGS) -Wno-unused-command-line-argument

all: src/Makefile
	make -C src

clean:
	make -C src clean
	rm src/Makefile

src/Makefile:
	mkdir -p $(@D)
	cd $(@D); ./autogen.sh;  LIBTRANSISTOR_HOME=$(LIBTRANSISTOR_HOME)  ./configure "CFLAGS=$(CFLAGS)" --host=aarch64-none-switch --target=switch --prefix=$(ROOT_DIR)/build
