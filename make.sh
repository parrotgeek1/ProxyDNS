#!/bin/sh
${CCPREFIX}gcc proxydns2.c -O3 -Wall -Wextra -Werror $EXTRAFLAGS -o proxydns2
${CCPREFIX}strip proxydns2