.RECIPEPREFIX = ~

BUILD_DIR = bin
SOURCE_DIR = src
TEST_DIR = test
GENERATES = prog prog-a prog-so README liboutput_static.a liboutput.so
TARGET_FILES = ("${GENERATES[@]/#/$(BUILD_DIR)}")
TRASH = $(BUILD_DIR)/*.o $(BUILD_DIR)/*~ $(BUILD_DIR)/o.* README

$(BUILD_DIR)/%.o:   $(SOURCE_DIR)/%.c
~ mkdir -p $(BUILD_DIR)
~ cc -fPIC $< -c -o $@

all: README $(BUILD_DIR)/prog $(BUILD_DIR)/prog-a $(BUILD_DIR)/prog-so

$(BUILD_DIR)/prog-a: $(BUILD_DIR)/prog.o $(BUILD_DIR)/liboutput_static.a
~ cc $^ -o $@

$(BUILD_DIR)/prog-so: $(BUILD_DIR)/prog.o $(BUILD_DIR)/liboutput.so
~ cc $(BUILD_DIR)/prog.o -L$(BUILD_DIR) -loutput -o $@

$(BUILD_DIR)/prog: $(BUILD_DIR)/prog.o $(BUILD_DIR)/fun.o $(BUILD_DIR)/const.o
~ cc $^ -o $@

$(BUILD_DIR)/liboutput_static.a: $(BUILD_DIR)/fun.o $(BUILD_DIR)/const.o
~ ar rcs $@ $^

$(BUILD_DIR)/liboutput.so: $(BUILD_DIR)/fun.o $(BUILD_DIR)/const.o
~ cc -shared $^ -o $@

README: $(BUILD_DIR)/prog $(BUILD_DIR)/prog-a
~ ./$< 2> $@

clean:
~ rm -rf $(TRASH) $(TEST_DIR)

distclean: clean
~ rm -rf $(BUILD_DIR)

test: test-no-input test-1-input test-3-input



test-prep:
~ mkdir -p $(TEST_DIR)

test-no-input: distclean $(BUILD_DIR)/prog $(BUILD_DIR)/prog-a $(BUILD_DIR)/prog-so test-prep
~ LD_LIBRARY_PATH=$(BUILD_DIR) $(BUILD_DIR)/prog > $(TEST_DIR)/prog 2>&1
~ LD_LIBRARY_PATH=$(BUILD_DIR) $(BUILD_DIR)/prog-a > $(TEST_DIR)/prog-a 2>&1
~ LD_LIBRARY_PATH=$(BUILD_DIR) $(BUILD_DIR)/prog-so > $(TEST_DIR)/prog-so 2>&1
~ cmp $(TEST_DIR)/prog $(TEST_DIR)/prog-a && cmp $(TEST_DIR)/prog-so $(TEST_DIR)/prog-a && cmp $(TEST_DIR)/prog $(TEST_DIR)/prog-so

test-1-input: distclean $(BUILD_DIR)/prog $(BUILD_DIR)/prog-a $(BUILD_DIR)/prog-so test-prep
~ LD_LIBRARY_PATH=$(BUILD_DIR) $(BUILD_DIR)/prog 1 > $(TEST_DIR)/prog 2>&1
~ LD_LIBRARY_PATH=$(BUILD_DIR) $(BUILD_DIR)/prog-a 1 > $(TEST_DIR)/prog-a 2>&1
~ LD_LIBRARY_PATH=$(BUILD_DIR) $(BUILD_DIR)/prog-so 1 > $(TEST_DIR)/prog-so 2>&1
~ cmp $(TEST_DIR)/prog $(TEST_DIR)/prog-a && cmp $(TEST_DIR)/prog-so $(TEST_DIR)/prog-a && cmp $(TEST_DIR)/prog $(TEST_DIR)/prog-so

test-3-input: distclean $(BUILD_DIR)/prog $(BUILD_DIR)/prog-a $(BUILD_DIR)/prog-so test-prep
~ LD_LIBRARY_PATH=$(BUILD_DIR) $(BUILD_DIR)/prog 1 2 3 > $(TEST_DIR)/prog 2>&1
~ LD_LIBRARY_PATH=$(BUILD_DIR) $(BUILD_DIR)/prog-a 1 2 3 > $(TEST_DIR)/prog-a 2>&1
~ LD_LIBRARY_PATH=$(BUILD_DIR) $(BUILD_DIR)/prog-so 1 2 3 > $(TEST_DIR)/prog-so 2>&1
~ cmp $(TEST_DIR)/prog $(TEST_DIR)/prog-a && cmp $(TEST_DIR)/prog-so $(TEST_DIR)/prog-a && cmp $(TEST_DIR)/prog $(TEST_DIR)/prog-so
