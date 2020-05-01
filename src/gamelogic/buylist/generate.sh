#!/bin/bash

set -e
set -x

bison -d buylist.ypp
flex -o lex.yy.cpp buylist.l
