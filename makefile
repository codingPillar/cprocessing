# DEPENDENCIES SLD2 GLEW

OUT_DIR:=compile
SRC_DIR:=src
EXAMPLE_DIR:=examples

C_SRC_FILES:=$(wildcard $(SRC_DIR)/*.c)
C_SRC_OBJ:=$(C_SRC_FILES:$(SRC_DIR)/%=$(OUT_DIR)/%.o)

CC:=gcc
CINCLUDE:=$(shell pkg-config sdl2 --cflags) $(shell pkg-config glew --cflags)
CFLAGS:=-Wall -Wextra $(CINCLUDE)
CLIBS:=$(shell pkg-config sdl2 --libs) $(shell pkg-config glew --libs) -lm

define COMPILE = 
$(CC) $(CFLAGS) -o $@ $^
endef

$(OUT_DIR)/%.o:$(SRC_DIR)/%
	$(COMPILE) -c

$(OUT_DIR)/%.o:$(EXAMPLE_DIR)/%
	$(COMPILE) -c

# BUILD EXECUTABLE
OUTNAME:=simpleWindow
$(OUTNAME):$(C_SRC_OBJ) $(OUT_DIR)/$(OUTNAME).c.o
	$(COMPILE) $(CLIBS)

# TESTS
TEST_DIR:=tests
TEST_OUTNAME:=test
CXX:=g++
TEST_SRC_FILES:=$(wildcard $(TEST_DIR)/*.cpp)
TEST_SRC_OBJ:=$(TEST_SRC_FILES:$(TEST_DIR)/%=$(OUT_DIR)/%.o)
$(OUT_DIR)/%.o:$(TEST_DIR)/%
	$(CXX) -Wall -Wextra -Werror -o $@ -c $<
$(TEST_OUTNAME):$(TEST_SRC_OBJ) $(C_SRC_OBJ)
	$(CXX) -o $@ $^ -lgtest $(CLIBS)

clean:
	rm -rf $(OUT_DIR)/*
	rm -f $(OUTNAME) $(TEST_OUTNAME)