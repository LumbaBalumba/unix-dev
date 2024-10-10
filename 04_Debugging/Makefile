CC = gcc
CFLAGS = -O0 -g
TARGET = range

.PHONY: all clean test

all: $(TARGET)

$(TARGET): range.c
	$(CC) $(CFLAGS) -o $(TARGET) range.c

test: $(TARGET)
	gdb --batch --quiet -x gdb_script1.gdb $(TARGET) | grep '^@@@' > test_output1.txt
	gdb --batch --quiet -x gdb_script2.gdb $(TARGET) | grep '^@@@' > test_output2.txt
	diff -u test_output1.txt reference_output1.txt
	diff -u test_output2.txt reference_output2.txt

clean:
	rm -f $(TARGET) test_output1.txt test_output2.txt
