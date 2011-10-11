MODULE := engines/cryo

MODULE_OBJS := \
	debug.o \
	detection.o \
	cryo.o

# This module can be built as a plugin
ifeq ($(ENABLE_CRYO), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
