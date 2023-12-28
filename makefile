# compiler
CC = g++

# compiler flags
CXXFLAGS = -Wall

# target name
TARGET = cache_sim

# sources
SRC = cache.cpp
FUNC = fn_defs.cpp
OBJ_FN = fn_defs.o
OBJ_MAIN = cache.o


# build target
$(TARGET): $(OBJ_MAIN) $(OBJ_FN)
	$(CC) $(CXXFLAGS) $(OBJ_MAIN) $(OBJ_FN) -o $@

$(OBJ_MAIN): $(SRC)
	$(CC) $(CXXFLAGS) -c $<

$(OBJ_FN): $(FUNC)
	$(CC) $(CXXFLAGS) -c $<

# clean
.PHONY: clean
clean:
	rm -f *.o $(TARGET)