CXX = g++
CXXFLAGS = -std=gnu++1z
LIBS = -lglfw -lGL -lGLU -lX11 -lpthread -lXrandr -lXi -ldl -lassimp
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
EXEC_DIR  := $(BUILD)/exec
TARGET = openGL_test
INCLUDE  := -Iinclude/ -I../STB_IMAGE -I../dearimg_gui/
INCLUDE_DIR := include
INCLUDE_FILE := $(wildcard $(INCLUDE_DIR)/*.h)
INCLUDE_FILE := $(filter-out include/stb_image.h, $(INCLUDE_FILE))

SRCDIR = src
SRC      := $(wildcard $(SRCDIR)/*.cc)         

format:
	clang-format \
	-style=\
	"{BasedOnStyle: Google,BreakBeforeBraces: Stroustrup, IndentWidth: 4, IndentCaseLabels: false, TabWidth: 4, UseTab: ForIndentation, ColumnLimit: 0}"\
	 -i $(SRC) $(INCLUDE_FILE) 
clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(EXEC_DIR)/* 
	-@rm -rvf $(BUILD)