###############################################################################
# Description: Linux Makefile for C-Compass Library
# Author: Hugo Saporetti Junior
# Create Date: Aug, 3rd 2009
# Version: 4.0
# ----------------------------------------------------------------------------
# Developed by Hugo Saporetti Junior 
# Access: https://github.com/yorevs/ccompass
# Copyright (C) 2018 Hugo Saporetti Junior
# ----------------------------------------------------------------------------
# Targets:
#
# [all]
#    Compile the entire program. This is the default target. By default, make
# rules compile and link with `-g3', so that executable programs have
# debugging symbols and optimization `-O0'.
#
# [dist]
#    Create a distribution tar file for this program. The tar file should be
# set up so that the file names in the tar file start with a subdirectory name
# which is the name of the package it is a distribution for. This name can
# include the version number.
#
# [clean]
#    Delete all files from the current directory that are normally created by
# building the program. Don't delete the files that record the
# configuration.
#
# [distclean]
#    Delete all files from the current directory that are created by 
# configuring, installing or building the program. If you have unpacked the
# source and built the program without creating any other files,`make 
# distclean' should leave only the files that were in the distribution.
#
# [install]
#    Compile the program and copy the executables, libraries, and so on to the
# directories where they should reside for actual use.
#
# [uninstall]
#    Delete all the installed files the copies that the `install' target
# creates. This rule do not modify the directories where compilation is done, 
# only the directories where files are installed. 

#!!! Notes ----------------------------------------------------------------- --
# This document id best viewed in portrait and configured not to trespass 80
# columns. There are tabs for GNU maker standard and spaces for identation.


# Project name. Used on distribution file name.
PRJ_NAME      := compass

# ---------------------------------------------------------------------------- 
### General Conventions section.
SHELL         := /bin/bash


#### Source Sufixes ####
# C-ANSI Sufixes --------------------------------------------------------------
# .c     ; source file
# .h     ; header file
# .o     ; compiled object
# C++ Sufixes -----------------------------------------------------------------
# .cpp   ; source file
# .hpp   ; header file
# .obj   ; compiled object
# QT Sufixes ------------------------------------------------------------------
# .ui    ; User Interface file (GUI)
# .moc   ; Meta-Object Compiler
.SUFFIXES:
.SUFFIXES: .c .h .o .cpp .hpp .obj .ui .moc .qrc .a .so


#### Include version control targets ####
-include ver.mk


# ---------------------------------------------------------------------------- 
### System configuration section.

CURDIR        ?= $(shell pwd)
BASE_DIR      ?= $(CURDIR)
topdir        ?= $(BASE_DIR)

# Sources and objects.
srctree       := $(topdir)/src/packages
builddir      := $(topdir)/build
objtree       := $(builddir)/bin
distdir       := $(topdir)/dist
INSTALL_DIR   := 

# Testing and examples.
unittestdir   := $(topdir)/test
sdirs         := sample

# Dependencies directories.
libdir        := $(topdir)/lib
includedir    := $(topdir)/include
confdir       := $(topdir)/conf
scriptdir     := $(topdir)/script
docdir        := $(topdir)/doc


# Host directories. Watch out permissions for them.
BINDIR        := /root/sbin
MANDIR        := /root/usr/local/man

# C/C++ Source and object Files
C_SRCS        := $(sort $(shell find $(srctree) -name "*.c" | grep -v $(sdirs)))
C_OBJS        := $(patsubst %.c,%.o,$(subst $(srctree),$(objtree),$(C_SRCS)))

# Step into these directories to find more source files
SUBDIRS       := $(sort $(dir $(C_SRCS) | uniq))


# ---------------------------------------------------------------------------- 
### Libraries, includes and Frameworks.

# Libraries and includes ( assuming "lib<lib_name>.a" ) separated by spaces " "
# Prefix '-l' will be appended automatically.
LIBS          := 
LIB_PATHS     := 
USER_PATHS    := 
INCLUDES      := $(sort $(shell find $(srctree) -name "*.h"))
INCLUDE_PATHS := $(foreach inc_d,$(SUBDIRS),-I"$(inc_d)") $(USER_PATHS)


# All source paths are specified here
vpath %.c $(srctree):$(foreach dir,$(SUBDIRS),$(dir):):$(MOC_DIR)$(RCC_DIR)
vpath %.h $(srctree):$(foreach dir,$(SUBDIRS),$(dir):)


# ---------------------------------------------------------------------------- 
### Compilation and platforms.

### Compiler flags.
# 
# -O0             => No Optimization for debugging.
# -g3             => Full debug symbols.
# -ansi           => Specify the standard to which the code should conform to.
# -std=c99        => Conforms to standard ANSI-C99. Not valid for C++.
# -Wall           => Enables all warnings.
# -Werror         => Turn warnings into errors.
# -Wfatal-errors  => Abort compilation on the first error occurred.
#
# Archiever flags
# -s              => - Create an archive index.
# -c              => - Do not warn if the library had to be created.
#
# Make flags
#
# --no-print-directory  => Turn off printing the current directory.
# --no-keep-going => Stop making on the first error.
#
DBG_CFLAGS    := -O0 -g3
WRN_CFLAGS    := -Wall -Wfatal-errors -Wimplicit-function-declaration
STD_FLAGS     := -ansi -std=c99
SAFETY_FLAGS  := -fsanitize=safe-stack
CFLAGS        := $(DBG_CFLAGS) $(WRN_CFLAGS) $(STD_FLAGS) -fmessage-length=0 
ARFLAGS       := -s -c
LDFLAGS       := 
MAKEFLAGS     += --no-print-directory --no-keep-going

### MACROS

# Toggle Verbose Mode on/off.
# To put more focus on warnings, be less verbose as default. Use 'make V=1' to
# see the full commands.
ifdef V
	ifeq ("$(origin V)", "command line")
		VERBOSE = $(if $(V:1=),@)
	endif
endif
ifndef VERBOSE
	VERBOSE = @
endif


### Include cross compilation stuff.
-include xcc.mk

# If case cross compilation stuff was not included.
ifndef CROSS_COMPILE
CC            := gcc
LD            := ld
AR            := ar
STRIP         := strip
endif


### Test Applications
# Choose which test application to compile (or all). Check sample dirs for
# options, e.g:
#  - base_Test: make test STAPP=base
#  - string_Test: make test STAPP=string
# "_Test" is appended automatically.

# Sample file namming convention. Changing this will impact target leak-check.
SAMPLE_PTRN   := ".*/$(sdirs)/[a-zA-Z0-9]*_Test.c"

TEST_DIRS     := $(shell find $(srctree) -name $(sdirs) -type d)
TEST_SRCS     := $(shell find $(srctree) -regex "$(SAMPLE_PTRN)")
TEST_OBJS     := $(patsubst %.c,%.o, $(TEST_SRCS))

# if specified, build only this sample app., else, all samples will be built.
STAPP          ?= 

# Utilities.
RM            := rm
MKDIR         := mkdir
MAKE          := make $(MAKEFLAGS)
MV            := mv
CP            := cp
FIND          := find
DOC_TOOL      := doxygen
TOOLCHECK     := $(scriptdir)/toolcheck.sh


# ---------------------------------------------------------------------------- 
### Targets.

### Artifact names.
PREFIX        := $(if $(CROSS_COMPILE:0=),u)
ARTIFACT      := $(PREFIX)$(PRJ_NAME).a
TARGET        := $(objtree)/$(addprefix lib,$(ARTIFACT))
DISTNAME      ?= $(PREFIX)$(PRJ_NAME)-$(VERSION)

# Default make target.
all: init pre-build $(TARGET)
	@echo '[$(notdir $(TARGET))] Build v$(VERSION), $(BUILD_DATE)'
	@echo '[$(notdir $(TARGET))] Finished building project.'
	@echo ' '


###############################################################################
# Pre-Build

# Build initialization. Create build directory tree.
init: check-tools
	$(VERBOSE)if [ ! -d "$(objtree)" ] ; then \
		echo "[$@] Creating build directories ..."; \
		$(MKDIR) -p $(objtree)/$(sdirs); \
		for dir in $(dir $(subst $(srctree),$(objtree),$(C_SRCS))); do \
			$(MKDIR) -p $$dir; \
		done;	\
		echo "[$@] Done."; \
	fi;
	$(VERBOSE)if [ ! -d "$(libdir)" ] ; then \
			$(MKDIR) -p $(libdir); \
	fi;
	$(VERBOSE)if [ ! -d "$(includedir)" ] ; then \
			$(MKDIR) -p $(includedir); \
	fi;
	@echo '[$@] Initialization complete !'
	@echo ' '
	
# Check for all required tools
check-tools:
	@echo '[$@] Checking required tools ...'
	@echo ' '
	@$(TOOLCHECK) $(MKDIR)
	@$(TOOLCHECK) $(RM)
	@$(TOOLCHECK) $(CP)
	@$(TOOLCHECK) $(FIND)
	@$(TOOLCHECK) $(MV)
	@$(TOOLCHECK) $(DOC_TOOL)
	@$(TOOLCHECK) $(CC)
	@$(TOOLCHECK) $(LD)
	@$(TOOLCHECK) $(AR)
	@$(TOOLCHECK) $(STRIP)
	@echo ' '


# Tasks to be executed before building
pre-build: version
	@echo '[$@] Finished.'
	@echo ' '


# Tasks executed after building
post-build:
	@echo '[$@] Updating library files ...'
	$(VERBOSE)$(CP) $(shell find $(objtree) -name *.a) $(libdir)
	@echo '[$@] Updating header files ...'
	$(VERBOSE)$(CP) $(shell find $(srctree) -name *.h) $(includedir)
	@echo '[$@] Finished.'
	@echo ' '


### Build of main targets.
$(TARGET): $(QT_ALL) $(INCLUDES) $(C_OBJS)
	@echo '[build] Building target: $(strip $(subst $(objtree)/, , $@))'
	@echo -n '[build] Invoking $(AR)'
	@echo '$(if $(CROSS_COMPILE:0=),[$(ARCH)] Cross) Archiever'
	$(VERBOSE)$(AR) $(ARFLAGS) -r$(if $(VERBOSE:@=),v) "$@" $(C_OBJS)
ifeq ($(CROSS_COMPILE),1)
	@echo '[build] Striping target: $(notdir $@)'
	$(VERBOSE)$(STRIP) "$@"
endif
	@echo '[build] Finished building target.'
	@echo '[build] Done.'
	@echo ' '
	@$(MAKE) post-build


### Build of m dependency files.
$(objtree)/%.obj: %.cpp %.h
$(objtree)/%.o: %.c %.h
$(objtree)/%.o: %.c
ifeq ($(VERBOSE),@)
	@echo '[build-dep] Building dependency files: $(notdir $?)'
else
	@echo -n '[build-dep] Building dependency files: '
	@echo '$(strip $(subst $(BASE_DIR), ., $?))'
endif
	@echo -n '[build-dep] Invoking $(CC)'
	@echo '$(if $(CROSS_COMPILE:0=),[$(ARCH)] Cross) Compiler'
	$(VERBOSE)$(CC) $(INCLUDE_PATHS) -c $(CFLAGS) -o"$@" "$<"
	@echo '[build-dep] Finished building file.'
	@echo ' '


# Generate API documentation.
docgen:
	@echo '[$@] Generating API documentation from source ...'
	@-$(RM) $(docdir)/api
	@$(MKDIR) $(docdir)/api
	@echo '[$@] Yet TODO'
	@echo '[$@] Done.'
	@echo ' '

# ---------------------------------------------------------------------------- 
### Installation, uninstallation, distribution and deployment.

# Generate distribution file
dist: all docgen
	@echo '[$@] Generating distribution file ...'
	@-${RM} -f $(shell find "$(BASE_DIR)" -type f -name "$(PROJ_NAME)*.tar.gz")
	@$(MKDIR) -p '$(DISTNAME)'
	@touch '$(DISTNAME)'
	@$(CP) $(BASE_DIR)/Makefile $(DISTNAME)
	@$(CP) $(BASE_DIR)/ver.mk $(DISTNAME)
	@$(CP) $(srctree) $(DISTNAME)
	@$(CP) $(includedir) $(DISTNAME)
	@$(CP) $(libdir) $(DISTNAME)
	@$(CP) $(docdir) $(DISTNAME)
	@-${RM} -f $(shell find $(DISTNAME) -name "CVS*")
	@echo "#### Files include on v$(VERSION) distribution:\n\n" > FILES
	@find $(DISTNAME) -type f -name "*.*" | sort | sed 's_\(.*/compass/\)_./_' >> FILES
	@$(MV) $(BASE_DIR)/FILES $(DISTNAME)
	@tar -czf $(DISTNAME).tar.gz $(subst $(BASE_DIR)/, , $(DISTNAME)/*)
	@echo '[$@] $(DISTNAME).tar.gz successfully packed !!!'
	@-$(RM) -f $(DISTNAME)
	@echo '[$@] Distribution directory successfully created !'
	@echo '[$@] Done.'
	@echo ' '

# System instalation.
install: dist
	@echo '[$@] Installing $(TARGET) on the system ...'
	@echo '[$@] Yet TODO'
	@echo '[$@] System was successfully installed !'
	@echo '[$@] Done.'
	@echo ' '

# System uninstalation.
uninstall:
	@echo '[$@] Uninstalling $(TARGET) from the system ...'
	@echo '[$@] Yet TODO'
	@echo '[$@] System was successfully uninstalled !'
	@echo '[$@] Done.'
	@echo ' '

# Compress and deploy distribution files.
deploy: dist
	@echo '[$@] Compress and deploy distribution ...'
	@echo '[$@] Yet TODO'
	@echo '[$@] Distribution file generated and deployed !'
	@echo '[$@] Done.'
	@echo ' '


# ---------------------------------------------------------------------------- 
### System cleanup.

# Cleanup of compiled files.
clean: $(QT_CLEAN)
	@echo '[$@] Cleaning all compilation objects ...'
	@-$(RM) -f $(C_OBJS)
	@-$(RM) -f $(TARGET)
	@-$(RM) -f $(builddir)/leak-report.txt
	@-$(RM) -rf $(distdir)
	@echo '[$@] Done.'
	@echo ' '


# Complete distribution cleaning
distclean: uninstall $(QT_CLEAN)
	@echo '[$@] Wiping build, distribution and installed contents ...'
	@-$(RM) -rf $(builddir)
	@-$(RM) -rf $(includedir)
	@-$(RM) -rf $(libdir)
	@-$(RM) -rf $(distdir)
	@-$(RM) -f $(BASE_DIR)/$(DISTNAME).tar.gz
	@echo '[$@] Done.'
	@echo ' '


### Sample targets

TLIBS=-lcompass -lm

test: clean all
	@echo "[$@] Building all tests from sample dirs ..."
	@echo ' '
	$(VERBOSE)for x in $(basename $(TEST_SRCS)); do \
		next=`basename "$$x"` ;\
		if [ -n "$(STAPP)" ] ;\
		then \
			if [ "$(addsuffix _Test,$(STAPP))" != "$$next" ] ;\
				then \
					continue ;\
			fi ;\
		fi ;\
		echo "[$$next] Building test application: $$next" ;\
		echo "[$$next] Invoking $(CC) C Compiler" ;\
		$(CC) -L"$(libdir)" -I"$(includedir)" \
		$(CFLAGS) -o "$$next" $$x.c $(TLIBS) ;\
		echo "[$$next] Finished building target: $$next" ;\
		if [ -f $$next ] ;\
		then \
			$(MV) $$next $(objtree)/$(sdirs) ;\
		fi ;\
		echo ' ' ;\
		if [ -d $$next.dSYM ] ;\
		then \
			test -d && rm -rf $(builddir)/$$next.dSYM ;\
			$(MV) -f $$next.dSYM $(builddir) ;\
		fi ;\
	done
	@echo '[$@] Done.'
	@echo ' '


### Debugging, profiling and static analysis.
REPORT=valgrind-report.txt

# Valgrind - Memory Leak Tests

$(REPORT): clean-reports
	@echo "[$@] Checking for \"$(STAPP)_Test\" memory leaks ..."
	@echo ' '
	@valgrind --leak-check=full $(objtree)/$(sdirs)/$(STAPP)_Test 2> $@

leak-check: test $(REPORT)
ifneq "$(STAPP)" ""
	@echo ' '
	@echo "[$@] Memory leak check complete. See summary below: "
	@echo ' '
	@echo "  $(shell grep "ERROR SUMMARY" $(REPORT))"
	@echo ' '
	@echo "[$@] For the full leak report see $(REPORT)."
endif
	@echo "[$@] Done."
	@echo ' '

clean-reports:
	$(VERBOSE)-$(RM) $(REPORT)

clean-tests:
	$(VERBOSE)-$(RM) $(objtree)/$(sdirs)/*

# ---------------------------------------------------------------------------- 
### PHONY targets.
.PHONY: all init pre-build post-build all-target dist clean distclean \
install uninstall gencdoc test leak-check clean-reports clean-tests \


#### END OF MAKEFILE ####