
# @file - makefile_template
# @author - Trinity Lundgren <lundgret@oregonstate.edu>

# Project name
project = marble-maze

# Compiler
CXX = g++

# Source files
sources = level-loader.cpp

# Create objects from source files
objects = $(sources:.cpp=.o)

# Output executable
EXE = $(project)

# Compiler flags
CFLAGS = -Wall -pedantic -std=c++14

#Valgrind options
VOPT = --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes

# Phony targets
.PHONY: default debug clean zip

# Default behavior: clean, compile, pass through valgrind
default: clean $(EXE) #debug # Debug is toggled off for submission

# Debug: pass to valgrind to check for memory leaks
debug: $(EXE)
	valgrind $(VOPT) ./$(EXE)

# '$@' refers to tag, '$^' to dependency
$(EXE) : $(objects)
	$(CXX) $(CFLAGS) $^ -o $@

# Create .o files from corresponding .cpp files
%.o: %.cpp
	$(CXX) $(CFLAGS) -c $^

# Create a zip archive of the project files for submission
zip:
	zip $(project)_Lundgren_Trinity.zip *.cpp *.hpp Makefile *pdf

clean:
	rm -f *.o *.zip $(EXE)
