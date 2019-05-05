ifeq ($(OS),Windows_NT)
  EXEEXT = .exe
else
  EXEEXT =
endif

TARGET = myprog$(EXEEXT)

# compiler settings
CC      = g++
CCFLAGS = -g -std=c++17 -Wall -Wextra -Wconversion -Wpedantic

# linker/loader settings
LD      = g++
LDFLAGS = -static  # link all libraries statically

# names of all .cpp and .o files
CFILES  = $(wildcard *.cpp)  # $(..) works for gmake only
OFILES  = $(CFILES:.cpp=.o)  # $(..) works for gmake only

# name of file containing all generated dependencies
DEPENDFILE = .depend

# rule how to compile .o files from .c files
%.o: %.cpp
	$(CC) $(CCFLAGS) -c $<

.PHONY: all run depend clean

all: $(TARGET)

# main target: prg is the name of the executable to build
$(TARGET): $(OFILES)
	$(LD) -o $@ $(OFILES) $(LDFLAGS)

# execute target
run: $(TARGET)
	@./$(TARGET)

# clean up directory
clean:
	$(RM) $(TARGET) *.o core *.bak *~

# create dependencies using gcc -MM
depend:
	$(CC) $(CCFLAGS) -MM $(CFILES) > $(DEPENDFILE)

-include $(DEPENDFILE)
