CC			= gcc
CFLAGS 		= -Wall -Wextra -Wpedantic -I$(IDIR)
TEST_CFLAGS = $(CFLAGS) -Itests/unity
TARGET		= dsp56k
TEST_TARGET	= test_dsp56k

IDIR 		= ./include
ODIR 		= ./obj

_DEPS 		= dsp56k.h instruction_decode.h
DEPS 		= $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ 		= instruction_decode.o dsp56k.o
OBJ 		= $(patsubst %,$(ODIR)/%,$(_OBJ))

TEST_SRC	= \
	test/test_instruction_decode.c \
	test/unity/unity.c \
	src/instruction_decode.c

.PHONY: all clean test

all: $(TARGET)

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(TEST_TARGET): $(TEST_SRC)
	$(CC) $(TEST_CFLAGS) $(TEST_SRC) -o $(TEST_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(ODIR)/*.o
	rm dsp56k