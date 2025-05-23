#!/usr/bin/env bash

./scripts/build.sh

./bin/naval team 0:1 team 0:1

./bin/naval team 0:10 1:20 2:30 4:40 5:50 team 0:10 1:20 2:30 4:40 5:50

./bin/naval team 0:65535 team 0:65535
