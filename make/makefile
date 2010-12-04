# Tools definitions.
RM := rm -rf
MKDIR := mkdir -p
# Remember: COMPILE.cpp = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
# By default, do a debug build; pass -O3 as CXXFLAGS to get a release build.
CXXFLAGS := -O0 -g3 -Wall

# Project definitions.
TARGET_EXE := aloa
SRC_DIR := ../src
OBJ_DIR := obj

# Create list of all cpp files.
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
HELPER := $(subst $(SRC_DIR),$(OBJ_DIR),$(SRCS))
# Create list of all object files.
OBJS := $(subst .cpp,.o,$(HELPER))
# Create list of all dependency files.
CPP_DEPS := $(subst .cpp,.d,$(HELPER))

# Make sure whole output directory tree exists.
OUTPUT_TREE:= $(sort $(dir $(OBJS)))
make-output-dir := $(shell $(MKDIR) $(OUTPUT_TREE))

all:

# Generate C/C++ dependencies.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(COMPILE.cpp) -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

all: $(TARGET_EXE)

$(TARGET_EXE): $(OBJS) 
	g++  -o"$(TARGET_EXE)" $(OBJS) 
	@echo 'Done.'

clean:
	-$(RM) $(OBJ_DIR) $(TARGET_EXE) 

debug:
	@echo $(OBJS)
	@echo $(CPP_DEPS)
	@echo $(OUTPUT_TREE)

.PHONY: all clean debug

