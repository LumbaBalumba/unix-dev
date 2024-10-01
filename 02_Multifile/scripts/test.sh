#!/bin/bash

readonly BUILD_DIR=./bin
readonly BINFILES=(prog prog-a prog-so)

for binfile in ${BINFILES[@]}; do
	LD_LIBRARY_PATH=./bin $BUILD_DIR/$binfile
done
