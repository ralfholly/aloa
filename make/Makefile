# Tools definitions.
RM := rm -rf
MKDIR := mkdir -p
GPPLINT := ../tools/g++lint

# Project definitions.
TARGET_EXE := aloa
SRC_DIR := ../src
INCLUDE := ../src
OBJ_DIR := obj
DOXY_DIR := ../doxygen
DOXY_FILE := $(PWD)/Doxyfile
LINT_POLICY := policy.lnt
LINTFLAGS := 

# Remember: COMPILE.cpp = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
# By default, do a debug build; pass -O3 as CXXFLAGS to get a release build.
CXXFLAGS := -O0 -g3 -Wall
CPPFLAGS += -I $(INCLUDE)

define lint-base-cmd
$(GPPLINT) -b $(LINT_POLICY) -i$(INCLUDE) $(LINTFLAGS) $1 
endef

define lint
    $(eval LINTFLAGS += $1)
    $(if $(file),\
        $(call lint-base-cmd,-u $(file)),\
        $(call lint-base-cmd,$(SRCS_ALOA)))
endef

# Create list of all cpp files.
SRCS_ALOA := $(wildcard $(SRC_DIR)/*.cpp)
SRCS_TINYXML := $(wildcard $(SRC_DIR)/tinyxml/*.cpp)
SRCS := $(SRCS_ALOA) $(SRCS_TINYXML)

HELPER := $(subst $(SRC_DIR),$(OBJ_DIR),$(SRCS))
# Create list of all object files.
OBJS := $(subst .cpp,.o,$(HELPER))
# Create list of all dependency files.
CPP_DEPS := $(subst .cpp,.d,$(HELPER))

# Make sure whole output directory tree exists.
OUTPUT_TREE := $(sort $(dir $(OBJS)))
make-output-dir := $(shell $(MKDIR) $(OUTPUT_TREE))

.PHONY: clean debug test todo doxy lint lintaloa deploy

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
	@echo [Building $@]
	$(CXX) -o $(TARGET_EXE) $^

lint:
	@echo [Linting]
	$(call lint)

lintaloa:
	@echo [Executing ALOA]
	$(call lint,env-xml.lnt -e830 -e831) > lint.output.xml
	aloa -f lint.output.xml

clean:
	@echo [Cleaning]
	-$(RM) $(OBJ_DIR) $(TARGET_EXE) $(DOXY_DIR)

test: $(TARGET_EXE)
	@echo [Testing]
	cd ../test && test.sh ; cd ../make

todo:
	@echo [Grepping for TODOs]
	@# Put an escape before ':' to avoid finding ourselves.
	grep --exclude-dir ".svn*" -RE "TODO\:" .. ; true

doxy:
	@echo [Generating doxygen documentation]
	cd $(SRC_DIR) && doxygen $(DOXY_FILE)

deploy: 
	make clean
	make $(TARGET_EXE)
	make test
	make lint
	make doxy
	make todo
	@echo [Checking for local modifications]
	svn status .. -q

