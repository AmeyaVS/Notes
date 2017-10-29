#!/usr/bin/bash

# Generate the interface
swig -python example.i

# Compile the source.
gcc -c -fPIC example.c

# Compile the Generated wrapper.
gcc -c -fPIC example_wrap.c -I /usr/include/python2.7

# Link the library with the wrapper library
gcc -shared example.o example_wrap.o -o _example.so -lpython2.7
