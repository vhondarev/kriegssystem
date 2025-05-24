#!/usr/bin/env bash

./scripts/build.sh

echo ""

# min fleet vs min fleet
./bin/naval team 0:1 team 0:1

echo ""

#normal fleet vs normal fleet
./bin/naval team 0:10 1:20 2:30 4:40 5:50 team 0:10 1:20 2:30 4:40 5:50

echo ""

# max fleet vs max fleet
./bin/naval team 0:65535 team 0:65535
