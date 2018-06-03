# Ceasar cipher example

## Task

Develop `ceasar` program which makes encoding-decoding string by Ceasar's cipher

Program must be according to these requirements:

* case insensitive
* -e and -d parameters specify encoding and decoding respectively
* parameter -n specifies step
* parameter -src specifies source file with string, if -src is ommited - read string from stdin
* parameter -abc specifies abc, if -abc is ommiter - abc is standard
* parameter -out specifies output file, if -out is ommited - print result to stdout

For example, running program like this

```
ceasar -e -n 3 -src in.txt -abc abc.txt -out out.txt
```

should be done such way:

* string must be encoded
* step equals n
* source string must be read from in.txt
* abc must be read from abc.txt
* encoded string must be written to out.txt

If parameters ammount is not enough, print this message:

```
usage: ceasar -(e|d) -n N [-src srcfile] [-abc abcfile] [-out outfile]
```
## Requirements

* C compiler
* CMake

## Build

* change dir to `build`
* run `cmake ..`
* run `make`

## Run

* run `ceasar` from `build` dir

## Notes

* default abc is english
* case insensitive