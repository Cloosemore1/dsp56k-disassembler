IDIR = ./include
_DEPS = dsp56k.h instruction_decode.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
CFLAGS = -I$(IDIR)

ODIR = obj

_OBJ = instruction_decode.o dsp56k.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: src/%.c $(DEPS)
	gcc -c -o $@ $< $(CFLAGS)

dsp56k: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f $(ODIR)/*.o
	rm dsp56k