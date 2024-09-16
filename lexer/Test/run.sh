#!/bin/bash

DIR=.
file=$1
lex test.l
gcc -o test.o lex.yy.c
./test.o $DIR/$file