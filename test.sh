#!/bin/sh

# Performs a simple regression test by comparing
# the results of a test run on some sample files
# in test/ to a file containing reference output

exit_code=0

ALOA_PATH=make/aloa

if [ ! -x "$ALOA_PATH" ]; then
    echo "$ALOA_PATH doesn't exist. Please perform a build first."
    exit 1
fi

$ALOA_PATH test/lint.output.xml > test/output

diff test/output test/output.ref
if [ $? -ne 0 ]; then exit_code=1; fi

exit $exit_code
    
