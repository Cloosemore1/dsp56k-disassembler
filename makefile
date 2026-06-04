DEPS = dsp56k.h

dsp56k: dsp56k.c $(DEPS)
	gcc -o dsp56k dsp56k.c

clean:
	rm dsp56k