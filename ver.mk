###############################################################################
# Description: Linux Makefile Version Control extension for C-Compass Library
# Author: hugo
# Create Date: Jan, 23 2012
# Version: 1.0
# -----------------------------------------------------------------------------
# Developed by ICC-SWE 
# Access: https://github.com/yorevs/ccompass
# Copyright (C) 2018 Hugo Saporetti Junior
# -----------------------------------------------------------------------------

# Version control file.
BUILD_NUM_FILE   := VERSION

# If the version file does not exist. Create it and assign version 0.0.1 .
ifneq "$(wildcard $(BUILD_NUM_FILE) )" ""
VERSION          ?= $(MAJOR).$(MINOR).$(REVISION)
else
VERSION          ?= 0.0.1
endif

# Version parts.
MAJOR            ?= $(shell head -1 $(BUILD_NUM_FILE) | cut -d '.' -f 1)
MINOR            ?= $(shell head -1 $(BUILD_NUM_FILE) | cut -d '.' -f 2)
REVISION         ?= $(shell head -1 $(BUILD_NUM_FILE) | cut -d '.' -f 3)
BUILD_DATE       := $(shell date +'%d-%m-%Y')


# Create the initial BUILD_NUM_FILE.
$(BUILD_NUM_FILE): $(C_OBJS)
	$(VERBOSE)if [ ! -f $(BUILD_NUM_FILE) ]; then \
		echo "0.0.0" > $(BUILD_NUM_FILE); \
	fi;


# Version. Increment if any object file changes.
version: $(BUILD_NUM_FILE)
	$(MAKE) incr-revision
	@echo "### Last modified: $(BUILD_DATE)" >> $(BUILD_NUM_FILE)
	@echo "### Automatically generated. Do not edit !" >>$(BUILD_NUM_FILE)


# Increment the Major number.
incr-major: $(BUILD_NUM_FILE)
	@echo '[$@] Incrementing major number'
	@echo "$$(($(MAJOR) + 1)).0.0" > $(BUILD_NUM_FILE)

# Increment the Minor number.
incr-minor: $(BUILD_NUM_FILE)
	@echo '[$@] Incrementing minor number'
	@echo "$(MAJOR).$$(($(MINOR) + 1)).0" > $(BUILD_NUM_FILE)

# Increment the Revision number.
incr-revision: $(BUILD_NUM_FILE)
	@echo '[$@] Incrementing revision number'
	@echo "$(MAJOR).$(MINOR).$$(($(REVISION) + 1))" > $(BUILD_NUM_FILE)


# ---------------------------------------------------------------------------- 
### PHONY targets.
.PHONY: version incr-major incr-minor incr-revision

