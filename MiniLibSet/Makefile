ifeq ($(OS),Windows_NT)
  EXEEXT = .exe
else
  EXEEXT =
endif

TARGET = myprog$(EXEEXT)
MINILIB_DIR  = ../MiniLib

# compiler settings
CC      = g++
CCFLAGS = -I$(MINILIB_DIR) -g -std=c++17 -Wall -Wextra -Wconversion -Wpedantic

ifneq ($(OS),Windows_NT)
  CCFLAGS := $(CCFLAGS) `pkg-config --cflags gtk+-2.0`
endif

# linker/loader settings
LD = g++

ifeq ($(OS),Windows_NT)
  LDFLAGS = -mwindows -L$(MINILIB_DIR) -lML -static
else
  LDFLAGS = -L$(MINILIB_DIR) -lML \
            `pkg-config --libs gtk+-2.0`
endif

# other commands
RM = rm -rf

# names of all .cpp and .o files
CPPFILES = $(wildcard *.cpp)
OFILES   = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(CPPFILES))
OBJ_DIR  = obj

# name of file containing all generated dependencies
DEPENDFILE = .depend

# rule how to compile .o files from .c files
$(OBJ_DIR)/%.o: %.cpp
	$(CC) $(CCFLAGS) -c -o $@ $<

.PHONY: all depend clean

all: $(OBJ_DIR) $(TARGET)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

# main target: TARGET holds the name of the executable to build
$(TARGET): $(OFILES)
	$(LD) -o $@ $(OFILES) $(LDFLAGS)

# execute target
run: $(TARGET)
	@./$(TARGET)

# clean up directory
clean:
	$(RM) $(TARGET) $(OBJ_DIR) *.a core *.bak *~

# create dependencies using gcc -MM
depend:
	$(CC) $(CCFLAGS) -MM $(CPPFILES) > $(DEPENDFILE)

-include $(DEPENDFILE)
