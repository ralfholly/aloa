#!/bin/bash

# Performs a simple regression test by comparing
# the results of a test run on some sample files
# in test/ to a file containing reference output

exit_code=0

TEST_SCRIPT_DIR=$(dirname $0)

if [ ! -x "$ALOA_EXE" ]; then
    echo "$ALOA_EXE doesn't exist. Please perform a build first."
    exit 1
fi


###################################
# Test output to stdout.
#
OUTPUT="$TEST_SCRIPT_DIR/output"
"$ALOA_EXE" -m -f "$TEST_SCRIPT_DIR/lint.output.xml" > "$OUTPUT"
diff "$OUTPUT" "$TEST_SCRIPT_DIR/output.ref"
if [ $? -ne 0 ]; then 
    echo "*** Test (stdout) failed! ***"
    exit_code=1; 
else
    rm "$OUTPUT"
fi


###################################
# Test XML ouput.
#
OUTPUT="$TEST_SCRIPT_DIR/output.xml"
"$ALOA_EXE" -m -f "$TEST_SCRIPT_DIR/lint.output.xml" -x "$OUTPUT"
diff "$OUTPUT" "$TEST_SCRIPT_DIR/output.xml.ref"
if [ $? -ne 0 ]; then 
    echo "*** Test (XML output) failed! ***"
    exit_code=1; 
else
    rm "$OUTPUT"
fi

exit $exit_code

