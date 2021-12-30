# Fano archiver

## Description

Archiver based on Fano algorithm. The Fano algorithm uses 
variable-length prefix codes: a frequently occurring byte of data 
is encoded with a shorter code, and a rare one with a longer code.

File data archiving occurs after the program builds a Fano code 
table (a unique code is selected for each unique byte) and then 
replaces each byte with its code. Unpacking goes this way: 
the program reads stored Fano code tree from the compressed file 
and recovers the data, mapping each code to its bytes.

## Implementation

The program is a CLI application that performs data archiving
(compressing) and unpacking (decompressing). Ensure that you have
`gcc compiler` on board and use Makefile (for Linux) to perform 
the following actions:

To build the binary, run `make` (at the root of repo) \
To clean repo from the compiled objects, run `make clean`

Repository also has example files placed on `examples` folder for 
fast testing the functionality.

### Using

To run the program use next example:

`<fano-archiver> <src.file> <mode> <tgt.file>`

where\
`fano-archiver` is the name of the binary for particular 
platform\
`src.file` is the path to source file\
`mode` is the compress/decompress action, specified with `c` or 
`d` flags\
`tgt.file` is the path to target file (which will be created after 
program executing)

For example:
`./fano-archiver examples/parargaph.txt c compressed.txt`

## Theory. Example of how it works