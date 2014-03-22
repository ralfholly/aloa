# Tools definitions.
RM := rm -rf
MKDIR := mkdir -p
GPPLINT := tools/gcclint

# Project definitions.
BUILD_DIR := build
export ALOA_EXE := $(BUILD_DIR)/aloa
SRC_DIR := src
OBJ_DIR := $(BUILD_DIR)/obj
LIB_DIR := $(SRC_DIR)/lib
GCOV_DIR := $(BUILD_DIR)/gcov
INCLUDE := $(LIB_DIR)

DOXY_DIR := $(BUILD_DIR)/doxygen
DOXY_FILE := $(BUILD_DIR)/Doxyfile
TEST_SCRIPT := test/test.sh
LINT_POLICY := $(BUILD_DIR)/policy.lnt
LINT_ALOA_XML_OUTFILE := lint.output.xml
TARGET := DEBUG

# Create list of all cpp files.
SRCS_ALOA := $(wildcard $(SRC_DIR)/*.cpp)
SRCS_TINYXML := $(wildcard $(LIB_DIR)/tinyxml/*.cpp)
SRCS := $(SRCS_ALOA) $(SRCS_TINYXML)

# By default, do a debug build.
ifeq "$(TARGET)" "RELEASE"
override CXXFLAGS += -O2 -DNDEBUG
else ifeq "$(TARGET)" "COVERAGE"
override CXXFLAGS += --coverage -DNDEBUG
else
override CXXFLAGS += -O0 -g3 -Wall
endif

CPPFLAGS += $(addprefix -I,$(INCLUDE))

define lint-base-cmd
$(GPPLINT) -b $(LINT_POLICY) -i$(INCLUDE) $(LINTFLAGS) $1 
endef

define lint
    $(eval override LINTFLAGS += $1)
    $(if $(file),\
        $(call lint-base-cmd,-u $(file)),\
        $(call lint-base-cmd,$(SRCS_ALOA)))
endef

HELPER := $(subst $(SRC_DIR),$(OBJ_DIR),$(SRCS))
# Create list of all object files.
OBJS := $(subst .cpp,.o,$(HELPER))
# Create list of all dependency files.
CPP_DEPS := $(subst .cpp,.d,$(HELPER))

# Make sure whole output directory tree exists.
OUTPUT_TREE := $(sort $(dir $(OBJS)))
make-output-dir := $(shell $(MKDIR) $(OUTPUT_TREE))

.PHONY: clean debug test todo doxy lint lintaloa deploy gcov

all:

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(COMPILE.cpp) -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o"$@" "$<"

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

all: $(ALOA_EXE)

$(ALOA_EXE): $(OBJS)
	@echo [Building $@ $(TARGET)]
	$(LINK.cc) -o $(ALOA_EXE) $^

lint:
	@echo [Linting]
	$(call lint, -zero)

lintaloa:
	@echo [Executing ALOA]
	$(call lint,env-xml.lnt -e830 -e831 -zero) > $(LINT_ALOA_XML_OUTFILE)
	-$(ALOA_EXE) -f $(LINT_ALOA_XML_OUTFILE)

clean:
	@echo [Cleaning]
	-$(RM) $(OBJ_DIR) $(ALOA_EXE) $(DOXY_DIR) $(LINT_ALOA_XML_OUTFILE) $(GCOV_DIR)

test: $(ALOA_EXE)
	@echo [Testing]
	$(TEST_SCRIPT)

todo:
	@echo [Grepping for TODOs]
	@# Put an escape before ':' to avoid finding ourselves.
	grep --exclude-dir ".svn*" -RE "TODO\:" . ; true

doxy:
	@echo [Doxygen currently not supported]
	# @echo [Generating doxygen documentation]
	# cd $(SRC_DIR) && doxygen $(DOXY_FILE)

gcov_report: 
	rm -rf *.gcov
	$(MKDIR) $(GCOV_DIR)
	gcov -b -o $(OBJ_DIR) $(SRCS_ALOA)
	mv *.gcov $(GCOV_DIR)
	
lcov_report: 
	$(MKDIR) $(GCOV_DIR)
	lcov -c -d . -b . -o $(GCOV_DIR)/gcov.info.raw  
	lcov -r $(GCOV_DIR)/gcov.info.raw /usr/include/\* -o $(GCOV_DIR)/gcov.info
	genhtml $(GCOV_DIR)/gcov.info -o $(GCOV_DIR)/html
	
deploy: 
	@$(eval ALOA_VERSION=`grep -E 'VERSION\s+=\s+"[^"]+"' src/Aloa.h | grep -oE '[0-9]+\.[0-9]+\.[0-9]+'`)
	@$(eval ALOA_VERSION_DIR="aloa-$(ALOA_VERSION)")
	@if [ -e $(ALOA_VERSION_DIR) ]; then echo "Fatal: version $(ALOA_VERSION) exist already"; exit 1; fi
	make clean
	make $(ALOA_EXE) TARGET=RELEASE
	make test
	make lint
	make doxy
	make todo
	@echo [Checking for local modifications]
	@svn export . "$(ALOA_VERSION_DIR)"
	@tar cvzf "$(ALOA_VERSION_DIR).tar.gz" "aloa-$(ALOA_VERSION)"
	@svn status . -q
	@echo "Successfully deployed version $(ALOA_VERSION)"
