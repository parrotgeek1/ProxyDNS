#!/bin/sh
${CCPREFIX}gcc proxydns.c -O3 -Wall -Wextra -Werror -Wno-deprecated-declarations $EXTRAFLAGS -o proxydns
${CCPREFIX}strip proxydns
