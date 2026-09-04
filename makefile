CC		= gcc
CFLAGS 	= -Wall -Wextra -Wpedantic -I$(IDIR)
TARGET	= dsp56k

IDIR 	= ./include
ODIR 	= ./obj

_DEPS 	= dsp56k.h instruction_decode.h
DEPS 	= $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = instruction_decode.o dsp56k.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(ODIR)/*.o
	rm dsp56k