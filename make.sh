#!/bin/sh
${CCPREFIX}gcc proxydns.c -O3 -Wall -Wextra -Werror $EXTRAFLAGS -o proxydns
${CCPREFIX}strip proxydns
