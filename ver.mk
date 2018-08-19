###############################################################################
# Description: Linux Makefile Version Control extension for C-Compass Library
# Author: Hugo Saporetti Junior
# Create Date: Jan, 23 2012
# Version: 1.0
# -----------------------------------------------------------------------------
# Developed by Hugo Saporetti Junior 
# Access: https://github.com/yorevs/ccompass
# Copyright (C) 2018 Hugo Saporetti Junior
# -----------------------------------------------------------------------------
# Targets:
#
# [version]
#    Increment if any object file changes.
#
# [major]
#    Increment the Major number.
#
# [minor]
#    Increment the Minor number.
# 
# [revision]
#    Increment the Revision number.
#
# [buildnum]
#    Increment the build number.

# Version control file.
VERSION_FILE     := VERSION

# If the version file does not exist. Create it and assign version 0.0.1 .
ifneq "$(wildcard $(VERSION_FILE) )" ""
VERSION          ?= $(MAJOR).$(MINOR).$(REVISION).$(BUILDNUM)
else
VERSION          ?= 0.1.0.001
endif

# Version parts.
MAJOR            ?= $(shell head -1 $(VERSION_FILE) | cut -d '.' -f 1)
MINOR            ?= $(shell head -1 $(VERSION_FILE) | cut -d '.' -f 2)
REVISION         ?= $(shell head -1 $(VERSION_FILE) | cut -d '.' -f 3)
BUILDNUM         ?= $(shell head -1 $(VERSION_FILE) | cut -d '.' -f 4)
BUILD_DATE       := $(shell date +'%d-%m-%Y')


# Create the initial VERSION_FILE.
$(VERSION_FILE): $(C_OBJS)
	$(VERBOSE)if [ ! -f $(VERSION_FILE) ]; then \
		echo "0.1.0.000" > $(VERSION_FILE); \
	fi;

# Version. Increment if any object file changes.
version: $(VERSION_FILE)
	$(MAKE) revision
	@echo "### Last modified: $(BUILD_DATE)" >> $(VERSION_FILE)
	@echo "### Automatically generated. Do not edit !" >>$(VERSION_FILE)

# Increment the Major number.
major: $(VERSION_FILE)
	@echo '[$@] Incrementing major number'
	@echo "$$(($(MAJOR) + 1)).0.0.000" > $(VERSION_FILE)
	@make ver

# Increment the Minor number.
minor: $(VERSION_FILE)
	@echo '[$@] Incrementing minor number'
	@echo "$(MAJOR).$$(($(MINOR) + 1)).0.000" > $(VERSION_FILE)
	@make ver

# Increment the Revision number.
revision: $(VERSION_FILE)
	@echo '[$@] Incrementing revision number'
	@echo "$(MAJOR).$(MINOR).$$(($(REVISION) + 1)).000" > $(VERSION_FILE)
	@make ver

# Increment the build number.
buildnum: $(VERSION_FILE)
	@echo '[$@] Incrementing build number'
	@echo "$(MAJOR).$(MINOR).$(REVISION).$(shell printf '%03d' $$(($(BUILDNUM) + 1)))" > $(VERSION_FILE)
	@make ver

# Check the current version.
ver:
	@echo '[$@] Current version is: $(shell head -1 $(VERSION_FILE))'

# ---------------------------------------------------------------------------- 
### PHONY targets.
.PHONY: version major minor revision buildnum ver

