#!/bin/bash

if [ "$PCLINT_PATH" == "" ] || [ ! -d "$PCLINT_PATH" ]; then
    echo "PCLINT_PATH not defined or directory does not exist"
    exit 1
fi

GCC_VERSION=$(gcc --version | head -n 1 | sed -r -e "s/.*([0-9]+\.[0-9]+\.[0-9]+).*/\1/")
if [ $? -ne 0 ]; then
        echo "Cannot find GCC. Please check your installation."
        exit 1
fi

# Get rid of directory part.
PROGNAME=$(basename $0)
# Get rid of extension.
PROGNAME=${PROGNAME%.*}

SETTINGS_DIR="$HOME/.$PROGNAME.$GCC_VERSION"

INCLUDE_PATH_FILE="include.lnt"

# If settings directory doesn't exist, create and fill it.
if [ ! -d $SETTINGS_DIR ]; then

        echo -n "Creating settings directory $SETTINGS_DIR... " 1>&2
        mkdir -p "$SETTINGS_DIR"
        if [ ! -d $SETTINGS_DIR ]; then
                echo "Cannot create $SETTINGS_DIR"
                exit 1
        fi
        
        pushd . >/dev/null
        cd $SETTINGS_DIR

        COMPILER=${PROGNAME%%lint}
        DUMMY_FILE="dummy.c"
        PREPRO_FILE="lint_cmac.h"
        if [ $COMPILER == "g++" ]; then 
            PREPRO_FILE="lint_cppmac.h"
        fi
       
        echo -n '' >$INCLUDE_PATH_FILE
        echo -n '' >$DUMMY_FILE

        # Generate predefined preprocessor symbols.
        $COMPILER -E -dM $DUMMY_FILE >$PREPRO_FILE
       
        # Generate list of include paths
        skip=1
        $COMPILER -c -v $DUMMY_FILE 2>&1 | while read line; do
                if [ "$line" == '#include "..." search starts here:' ]; then
                        read line
                        if [ "$line" == '#include <...> search starts here:' ]; then
                                read line
                                skip=0
                        fi
                fi
                if [ "$line" == 'End of search list.' ]; then
                        skip=1
                fi

                if [ $skip -eq 0 ]; then
                        echo "--i$line" >>$INCLUDE_PATH_FILE
                fi;
        done

        echo "DONE" 1>&2
        popd >/dev/null
fi

# Now, invoke PC-Lint.
$PCLINT_PATH/LINT-NT.EXE -I$SETTINGS_DIR -I$PCLINT_PATH/lnt co-gcc.lnt $INCLUDE_PATH_FILE $* | tr '\\\r' '/ ' 


