#!/bin/bash

DIR=.
lex test.l
gcc -o test.o lex.yy.c