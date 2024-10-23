#!/bin/bash

set -e

echo "Running tests..."

rm -f infile outfile PROTECT_infile

function check_exit_code() {
	expected=$1
	actual=$2
	description=$3
	if [ "$actual" -eq "$expected" ]; then
		echo "$description passed."
	else
		echo "$description failed: Expected exit code $expected, got $actual"
		exit 1
	fi
}

echo "Test 1: Normal operation"
echo "This is a test file." >infile
./move infile outfile
if [ -f outfile ] && [ ! -f infile ]; then
	echo "Test 1 passed."
else
	echo "Test 1 failed: infile was not moved to outfile correctly"
	exit 1
fi

echo "Test 2: infile does not exist"
rm -f infile outfile
set +e
./move infile outfile
ret=$?
set -e
check_exit_code 2 $ret "Test 2"

echo "Test 3: Simulate read error"
echo "Test content" >infile
set +e
strace -e inject=read:error=EIO:when=2 ./move infile outfile >/dev/null 2>&1
ret=$?
set -e
if [ $ret -eq 6 ] && [ -f infile ]; then
	echo "Test 3 passed: Expected read error and infile remains intact."
else
	echo "Test 3 failed: Expected exit code 6 and infile to remain"
	exit 1
fi

echo "Test 4: Simulate write error"
echo "Test content" >infile
set +e
strace -e inject=write:error=EIO ./move infile outfile >/dev/null 2>&1
ret=$?
set -e
if [ $ret -eq 5 ] && [ -f infile ] && [ ! -f outfile ]; then
	echo "Test 4 passed: Expected write error, infile remains intact, outfile removed."
else
	echo "Test 4 failed: Expected exit code 5, infile to remain, and outfile not to exist."
	exit 1
fi

echo "Test 5: PROTECTed file with LD_PRELOAD"
echo "Test content" >PROTECT_infile
set +e
LD_PRELOAD=./protect_unlink.so ./move PROTECT_infile outfile >/dev/null 2>&1
ret=$?
set -e

if [ $ret -eq 0 ] && [ -f PROTECT_infile ] && [ -f outfile ]; then
	echo "Test 5 passed: PROTECTed file was not deleted."
else
	echo "Test 5 failed: PROTECTed file was deleted or move failed"
	exit 1
fi
