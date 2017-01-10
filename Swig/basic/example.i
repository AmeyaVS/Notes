/* example.i */
%module example
%{
#include "example.h"
%}

/* Parse the header file to generate wrappers */
%include "example.h"
