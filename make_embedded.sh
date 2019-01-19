#!/bin/sh
${CCPREFIX}gcc proxydns.c -O3 -Wall -Wextra -Werror -Wno-deprecated-declarations -Wno-unused-parameter -DEMBEDDED -static -s -o proxydns
