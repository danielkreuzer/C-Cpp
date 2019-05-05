# compiler settings
CC      = g++
CCFLAGS = -g -Wall -Wno-unknown-pragmas -std=c++14

ifeq ($(OS),Windows_NT)
	CPP_OSBRIDGE = OSBridge2Win32.cpp
else
  CCFLAGS := $(CCFLAGS) `pkg-config --cflags gtk+-2.0`
	CPP_OSBRIDGE = OSBridge2Gtk.cpp
endif

# linker/loader settings
LD        = g++
LDFLAGS   = 

# other commands
RM = rm -rf

# names of all .cpp and .o files
CPPFILES = MetaInfo.cpp \
           MLObject.cpp MLString.cpp MLCollection.cpp MLVector.cpp \
	         $(CPP_OSBRIDGE) MLApplication.cpp MLWindow.cpp
OFILES   = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(CPPFILES))
OBJ_DIR  = obj

# name of file containing all generated dependencies
DEPENDFILE = .depend

# rule how to compile .o files from .c files
$(OBJ_DIR)/%.o: %.cpp
	$(CC) $(CCFLAGS) -c -o $@ $<

.PHONY: all depend clean

all: $(OBJ_DIR) libML.a

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

# main target: libML is the archive (static library) to build
libML.a: $(OFILES)
	 ar r $@ $(OFILES)

# clean up directory
clean:
	$(RM) $(TARGET) $(OBJ_DIR) *.a core *.bak *~

# create dependencies using gcc -MM
depend:
	$(CC) $(CCFLAGS) -MM $(CPPFILES) > $(DEPENDFILE)

-include $(DEPENDFILE)
