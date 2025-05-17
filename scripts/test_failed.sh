#!/usr/bin/env bash

#
# EVERY RUN SHOULD FAIL AND PRINT AN ERROR
#

./build.sh

# Invalid team argument
./bin/naval x 0:10 team 0:5
./bin/naval team 0:0 team 0:1
# Invalid vessel type
./bin/naval team x team 0:5
./bin/naval team 1:10 x
./bin/naval team -1:10 team 0:5
# Invalid vessel count
./bin/naval team 0:65536 team 0:65536
