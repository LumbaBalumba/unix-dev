#!/bin/bash

readonly BUILD_DIR=./bin
readonly BINFILES=(prog prog-a prog-so)
readonly TESTDIR=./test

for binfile in ${BINFILES[@]}; do
	LD_LIBRARY_PATH=./bin $BUILD_DIR/$binfile >$TESTDIR/$binfile
done
