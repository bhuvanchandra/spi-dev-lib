# file      Makefile
# copyright Copyright (c) 2014 Toradex AG
#           [Software License Agreement]
# author    $Author$
# version   $Rev$
# date      $Date$
# brief     a simple makefile to (cross) compile.
#           uses the openembedded provided sysroot and toolchain
# target    linux on Colibri T20 / Colibri T30 / Colibri VF50 / Colibri VF61 / Apalis T30 / Apalis IMX6Q/D
# caveats   -

##############################################################################
# Setup your project settings
##############################################################################

# Set the input source files, the binary name and used libraries to link
SRCS_ADC = spi-dev-lib.o mcp3202-adc.o
SRCS_DAC = spi-dev-lib.o mcp4921-dac.o
PROG_ADC := mcp3202
PROG_DAC := mcp4921
LIBS = -lm

# Set arch flags
ARCH_CFLAGS = -march=armv7-a -fno-tree-vectorize -mthumb-interwork -mfloat-abi=hard

ifeq ($(MACHINE), colibri-t20)
  ARCH_FLAGS += -mfpu=vfpv3-d16 -mtune=cortex-a9
  TOOLCHAIN = armv7ahf-vfp-angstrom-linux-gnueabi
else ifeq ($(MACHINE), colibri-t30)
  ARCH_FLAGS += -mfpu=neon -mtune=cortex-a9
  TOOLCHAIN = armv7ahf-vfp-neon-angstrom-linux-gnueabi
else ifeq ($(MACHINE), colibri-vf)
  ARCH_FLAGS += -mfpu=neon -mtune=cortex-a5
  TOOLCHAIN = armv7ahf-vfp-neon-angstrom-linux-gnueabi
else ifeq ($(MACHINE), apalis-t30)
  ARCH_FLAGS += -mfpu=neon -mtune=cortex-a9
  TOOLCHAIN = armv7ahf-vfp-neon-angstrom-linux-gnueabi
else ifeq ($(MACHINE), apalis-imx6)
  ARCH_FLAGS += -mfpu=neon -mtune=cortex-a9
  TOOLCHAIN = armv7ahf-vfp-neon-angstrom-linux-gnueabi
else 
  $(error MACHINE is not set to a valid target)
endif

# Set flags to the compiler and linker
CFLAGS += -O2 -g -Wall -DNV_IS_LDK=1 $(ARCH_CFLAGS) --sysroot=$(OECORE_TARGET_SYSROOT)
LDFLAGS += 

##############################################################################
# Setup your build environment
##############################################################################

# Set the path to the oe built sysroot and
# Set the prefix for the cross compiler
OECORE_NATIVE_SYSROOT ?= $(HOME)/oe-core/build/out-eglibc/sysroots/i686-linux/
OECORE_TARGET_SYSROOT ?= $(HOME)/oe-core/build/out-eglibc/sysroots/$(MACHINE)/
CROSS_COMPILE ?= $(OECORE_NATIVE_SYSROOT)usr/bin/$(TOOLCHAIN)/arm-angstrom-linux-gnueabi-

##############################################################################
# The rest of the Makefile usually needs no change
##############################################################################

# Set differencies between native and cross compilation
ifneq ($(strip $(CROSS_COMPILE)),)
  LDFLAGS += -L$(OECORE_TARGET_SYSROOT)usr/lib -Wl,-rpath-link,$(OECORE_TARGET_SYSROOT)usr/lib -L$(OECORE_TARGET_SYSROOT)lib -Wl,-rpath-link,$(OECORE_TARGET_SYSROOT)lib
  BIN_POSTFIX =
  PKG-CONFIG = export PKG_CONFIG_SYSROOT_DIR=$(OECORE_TARGET_SYSROOT); \
               export PKG_CONFIG_PATH=$(OECORE_TARGET_SYSROOT)/usr/lib/pkgconfig/; \
               $(OECORE_NATIVE_SYSROOT)usr/bin/pkg-config
else
# Native compile
  PKG-CONFIG = pkg-config
  ARCH_CFLAGS = 
# Append .x86 to the object files and binaries, so that native and cross builds can live side by side
  BIN_POSTFIX = .x86
endif

# Toolchain binaries
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
STRIP = $(CROSS_COMPILE)strip
RM = rm -f

# Sets the output filename and object files
PROG_ADC := $(PROG_ADC)$(BIN_POSTFIX)
PROG_DAC := $(PROG_DAC)$(BIN_POSTFIX)
OBJS_ADC = $(SRCS_ADC:.c=$(BIN_POSTFIX).o)
OBJS_DAC = $(SRCS_DAC:.c=$(BIN_POSTFIX).o)
DEPS = $(OBJS_ADC:.o=.o.d) $(OBJS_DAC:.o=.o.d)

# pull in dependency info for *existing* .o files
-include $(DEPS)

.DEFAULT_GOAL := all

all: $(PROG_ADC) $(PROG_DAC)

$(PROG_ADC): $(OBJS_ADC) Makefile
	$(CC) $(CFLAGS) $(OBJS_ADC) $(LIBS) $(LDFLAGS) -o $@
	$(STRIP) $@

$(PROG_DAC): $(OBJS_DAC) Makefile
	$(CC) $(CFLAGS) $(OBJS_DAC) $(LIBS) $(LDFLAGS) -o $@
	$(STRIP) $@

%$(BIN_POSTFIX).o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<
	$(CC) -MM $(CFLAGS) $< > $@.d

clean:
	$(RM) $(OBJS_ADC) $(OBJS_DAC) $(PROG_ADC) $(PROG_DAC) $(DEPS)

.PHONY: all clean
