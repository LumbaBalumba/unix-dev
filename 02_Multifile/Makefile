.RECIPEPREFIX = ~

BUILD_DIR = bin
SOURCE_DIR = src
GENERATES = prog prog-a prog-so README liboutput_static.a liboutput.so
TARGET_FILES = ("${GENERATES[@]/#/$(BUILD_DIR)}")
TRASH = $(BUILD_DIR)/*.o $(BUILD_DIR)/*~ $(BUILD_DIR)/o.*

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
~ rm -f $(TRASH)

distclean: clean
~ rm -rf $(BUILD_DIR)

test: distclean all
~ mkdir -p test
