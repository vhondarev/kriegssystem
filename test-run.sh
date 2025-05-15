#!/usr/bin/env bash

#
# EVERY RUN SHOULD FAIL AND PRINT AN ERROR
#

# Invalid vessel type
./bin/naval team -1:10 team 0:5
# Invalid vessel count
./bin/naval team 0:65536 team 0:65536
