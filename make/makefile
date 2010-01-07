# Tools definitions.
RM := rm -rf
MKDIR := mkdir -p

# Project definitions.
TARGET_EXE := aloa
SRC_DIR := ../src
OBJ_DIR := obj

# Make sure output directory exists.
make-output-dir := $(shell [ -d $(OBJ_DIR) ] || $(MKDIR) $(OBJ_DIR))

# Create list of all cpp files.
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
HELPER := $(subst $(SRC_DIR),$(OBJ_DIR),$(SRCS))
# Create list of all object files.
OBJS := $(subst .cpp,.o,$(HELPER))
# Create list of all dependency files.
CPP_DEPS := $(subst .cpp,.d,$(HELPER))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"

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

.PHONY: all clean 

