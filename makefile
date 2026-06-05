IDIR = ./include
_DEPS = dsp56k.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
CFLAGS = -I$(IDIR)

dsp56k: src/dsp56k.c $(DEPS)
	gcc -o dsp56k src/dsp56k.c $(CFLAGS)

clean:
	rm dsp56k