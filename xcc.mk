###############################################################################
# Description: Makefile cross compilation extension.
# Author: Hugo Saporetti Junior
# Create Date: Jan, 23 2012
# Version: 1.0
# -----------------------------------------------------------------------------
# Developed by Hugo Saporetti Junior 
# Access: https://github.com/yorevs/ccompass
# Copyright (C) 2018 Hugo Saporetti Junior
# -----------------------------------------------------------------------------

# Toggle Cross Compiler on/off.
# To enable cross compiling, use make XCC=1. As default, no cross compiling is
# enabled.
ifdef XCC
	ifeq ("$(origin XCC)", "command line")
		CROSS_COMPILE = $(XCC)
	endif
endif
ifndef CROSS_COMPILE
	CROSS_COMPILE = 0
endif


# Cross-Compiler definitions.
ARCH        := arm
ARCH_PREFIX := $(ARCH)-linux-
ARCH_CC     := $(ARCH_PREFIX)gcc
ARCH_LD     := $(ARCH_PREFIX)ld
ARCH_AR     := $(ARCH_PREFIX)ar
ARCH_STRIP  := $(ARCH_PREFIX)strip

# Compilation Programs.
ifeq ($(CROSS_COMPILE),0)
CC          := gcc
LD          := ld
AR          := ar
STRIP       := strip
else
CC          := $(ARCH_CC)
LD          := $(ARCH_LD)
AR          := $(ARCH_AR)
STRIP       := $(ARCH_STRIP)
endif
