CC = gcc
CFLAGS = -Wall -Wextra -std=c99

.PHONY: all clean test

all: esub

esub: esub.c
	$(CC) $(CFLAGS) -o esub esub.c

clean:
	rm -f esub

test1: esub
	@echo "Test 1:"
	@./esub '([a-z]+)' 'Hello, \1!' 'world' > output_esub.txt
	@echo 'world' | sed -E 's/([a-z]+)/Hello, \1!/' > output_sed.txt
	@diff output_esub.txt output_sed.txt && echo "Test 1 passed." || echo "Test 1 failed."
	@rm -f output_esub.txt output_sed.txt

test2: test1
	@echo "Test 2: Substitution with backreferences"
	@./esub '([a-z]+) ([a-z]+)' 'Hello, \2 \1!' 'good morning' > output_esub.txt
	@echo 'good morning' | sed -E 's/([a-z]+) ([a-z]+)/Hello, \2 \1!/' > output_sed.txt
	@diff output_esub.txt output_sed.txt && echo "Test 2 passed." || echo "Test 2 failed."
	@rm -f output_esub.txt output_sed.txt

test3: test2
	@echo "Test 3: Substitution with invalid backreference"
	@./esub '([a-z]+)' 'Hello, \2!' 'world' 2> error_esub.txt || true
	@echo 'world' | sed -E 's/([a-z]+)/Hello, \2!/' 2> error_sed.txt || true
	@diff error_esub.txt error_sed.txt && echo "Test 3 passed." || echo "Test 3 failed."
	@rm -f error_esub.txt error_sed.txt

test4: test3
	@echo "Test 4: Substitution with escaped backslash"
	@./esub 'foo' 'bar\\baz' 'foo' > output_esub.txt
	@echo 'foo' | sed -E 's/foo/bar\\baz/' > output_sed.txt
	@diff output_esub.txt output_sed.txt && echo "Test 4 passed." || echo "Test 4 failed."
	@rm -rf output_esub.txt output_sed.txt

test5: test4
	@echo "Test 5: No match"
	@./esub 'foo' 'bar' 'baz' > output_esub.txt
	@echo 'baz' | sed -E 's/foo/bar/' > output_sed.txt
	@diff output_esub.txt output_sed.txt && echo "Test 5 passed." || echo "Test 5 failed."
	@rm -f output_esub.txt output_sed.txt

test6: test5
	@echo "Test 6: Invalid regular expression"
	@./esub '([a-z+' 'sub' 'string' 2> error_esub.txt || true
	@echo 'string' | sed -E 's/([a-z+/sub/' 2> error_sed.txt || true
	@diff error_esub.txt error_sed.txt && echo "Test 6 passed." || echo "Test 6 failed."
	@rm -f error_esub.txt error_sed.txt

test7: test6
	@echo "Test 7: Multiple backreferences"
	@./esub '([0-9]+)-([0-9]+)-([0-9]+)' 'Year: \3, Month: \2, Day: \1' '12-05-2021' > output_esub.txt
	@echo '12-05-2021' | sed -E 's/([0-9]+)-([0-9]+)-([0-9]+)/Year: \3, Month: \2, Day: \1/' > output_sed.txt
	@diff output_esub.txt output_sed.txt && echo "Test 7 passed." || echo "Test 7 failed."
	@rm -f output_esub.txt output_sed.txt

test8: test7
	@echo "Test 8: Escaped special characters in substitution"
	@./esub 'foo' 'bar\&baz' 'foo' > output_esub.txt
	@echo 'foo' | sed -E 's/foo/bar\&baz/' > output_sed.txt
	@diff output_esub.txt output_sed.txt && echo "Test 8 passed." || echo "Test 8 failed."
	@rm -f output_esub.txt output_sed.txt

test: test8
	@echo "Tests passed"
