#!/bin/sh -e
${CCPREFIX}gcc proxydns.c -O3 -Wall -Wextra -Werror -Wpedantic -Wno-deprecated-declarations -std=c89 $EXTRAFLAGS -o proxydns
${CCPREFIX}strip proxydns
