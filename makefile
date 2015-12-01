# Compilers
CXX = g++
CC = gcc

UNAME := $(shell uname)

# Running Linux? 
ifeq ($(UNAME), Linux)
	LINKER_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
# Windows?
else
	LINKER_FLAGS = -Llib -lsfml-graphics -lsfml-window -lsfml-system
endif

CXX_FLAGS = -Wall -c -g -O0 -fbuiltin -std=c++11 -I include -I $(SRCDIR)/../include
# LINKER_FLAGS = -L lib -lsfml-graphics -lsfml-window -lsfml-system

# Directories used for input and output
SRCDIR = src/src
BUILDDIR = build
EXEDIR = bin

OUTPUT_NAME = Tower

# Where the sources are located
SRCS = $(wildcard $(SRCDIR)/*.cpp)

# Where the compiled objects are located
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCS))

# Linking all the .o files and with the libs
build: $(OBJS)
	$(CXX) $(addprefix $(BUILDDIR)/, $(notdir $^)) $(LINKER_FLAGS) -o ./bin/$(OUTPUT_NAME)

# Compiling all the .cpp files into .o files
$(OBJS): $(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) $(CXX_FLAGS) $< -o $(BUILDDIR)/$(notdir $@)

# Running the created exe
run:
	./$(EXEDIR)/$(OUTPUT_NAME)

debug:
	gdb ./$(EXEDIR)/$(OUTPUT_NAME)
val:
	valgrind ./$(EXEDIR)/$(OUTPUT_NAME)
# Cleaning everything up
clean:
	rm $(BUILDDIR)/*.o && rm $(EXEDIR)/$(OUTPUT_NAME).exe
