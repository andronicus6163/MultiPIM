UTIL_DIR = ../utilities
CC = gcc
CFLAGS = -O2 -fopenmp
INCPATHS = -I$(UTIL_DIR)
EXE = stream
SRC = stream.c

FF = gfortran
FFLAGS = -O2
SRC += $(UTIL_DIR)/polybench.c

DEPS        := Makefile.dep
DEP_FLAG    := -MM
all : exe

exe : $(EXE)


$(EXE) : $(SRC)
        $(CC) $(CFLAGS) $(INCPATHS) $^ -o $@ $(EXTRA_FLAGS)

$(DEPS): $(SRC) $(HEADERS)
        $(CC) $(INCPATHS) $(DEP_FLAG) $(SRC) > $(DEPS)

-include $(DEPS)

clean:
        rm -f stream