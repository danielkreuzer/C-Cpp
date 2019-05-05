ifeq ($(OS),Windows_NT)
  EXEEXT = .exe
else
  EXEEXT =
endif

TARGET = graph$(EXEEXT)

# compiler settings
CC      = gcc
CCFLAGS = -g -std=c11 -Wpedantic -Wall -Wconversion -Wextra -Werror=vla

# linker/loader settings
LD      = gcc
LDFLAGS =

# names of all .c and .o files
CFILES  = $(wildcard *.c)  # $(..) works for gmake only
OFILES  = $(CFILES:.c=.o)  # $(..) works for gmake only

# name of file containing all generated dependencies
DEPENDFILE = .depend

# rule how to compile .o files from .c files
%.o: %.c
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
