# DSP56k Disassembler

A command-line disassembler written in C for the Motorola DSP56000/DSP56k family of digital signal processors.

I originally developed this project to analyze legacy embedded DSP firmware where source code was unavailable. The disassembler translates 24-bit DSP56k machine instructions into human-readable assembly, including DSP56k-specific features such as parallel data moves and the processor's various effective addressing modes.

## Features

* Decodes 24-bit Motorola DSP56k machine instructions
* Decodes instruction operands and registers
* Supports DSP56k effective addressing modes
* Decodes conditional instructions
* Decodes parallel data moves
* Handles instructions containing extension words
* Identifies unrecognized words as data rather than assuming a valid instruction
* Reads binary program images containing packed 24-bit DSP words
* Written in C with no external runtime dependencies

## Why I Built It

I developed this disassembler while analyzing software for legacy Motorola DSP56k-based embedded systems.

The DSP56k architecture presented several interesting challenges for a disassembler. Instructions are encoded as 24-bit words, some operations require extension words, and many arithmetic instructions can execute a parallel memory transfer in the same instruction. Addressing modes and operands are therefore encoded across several different portions of an instruction word.

Implementing the disassembler required working directly from processor architecture and instruction-set documentation, constructing opcode masks, decoding bit fields, and validating the resulting assembly against known DSP56k code.

## Project Structure

```text
dsp56k-disassembler/
├── include/
│   ├── dsp56k.h
│   └── instruction_decode.h
├── src/
│   ├── dsp56k.c
│   └── instruction_decode.c
├── test/
│   ├── unity/
│   └── test_instruction_decode.c
├── Makefile
└── README.md
```

`dsp56k.c` contains the command-line entry point and binary program loading logic.

`instruction_decode.c` contains the DSP56k instruction, operand, addressing-mode, and parallel-move decoding logic.

`test_instruction_decode.c` contains Unity test coverage of the decoding functionality.


## Building

The project requires a C compiler and GNU Make.

```bash
make
```

This produces the `dsp56k` executable.

To remove generated build files:

```bash
make clean
```

To run tests:

```bash
make test
```

## Usage

```bash
./dsp56k <binary>
```

The input file is interpreted as packed 24-bit DSP program words.

Example output:

```text
$009E: MOVEP 	A,Y:<<$FFF0
$009F: MOVEP 	Y:<<$FFF0,X0
$00A0: CLR 	    A 	#000040,R0
$00A2: DO 	    #516,$00A4
$00A4: MOVE 	A,L:(R0)+
$00A5: MOVEP 	#000000,X:<<$FFE1
```

Each output line contains the DSP program-memory address followed by the decoded instruction.

## DSP56k Instruction Decoding

Instruction recognition is implemented using opcode masks and constants. Each instruction family identifies the portions of the 24-bit word that determine the operation, after which instruction-specific fields are decoded into operands.

The decoder also separately interprets DSP56k parallel-move fields, allowing instructions containing simultaneous arithmetic and data-transfer operations to be represented as a single assembly instruction.

Effective-address decoding supports the DSP56k address-register forms as well as absolute and immediate operands.

## Current Status

This project is a work in progress and was developed primarily for analysis of DSP56k firmware encountered during reverse-engineering projects.

A substantial portion of the instruction set and parallel-move encodings is implemented, but instruction coverage is not yet complete. Unsupported or unrecognized encodings may be emitted as data.

Future work includes:

* Expanding instruction and parallel-move coverage
* Add additional test coverage
* Supporting arbitrary program-image sizes and load addresses
* Improving command-line options and error reporting

## References

The implementation was developed using the "DSP56000 24-Bit Digital Signal Processor Family Manual" as a guide for the instruction set and processor architecture.
