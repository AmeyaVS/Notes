# README.md

This example was observed in YouTube video about link order.
An interesting scenario where the global object is initialized before the
iostream objects are ready for use.

For best case observation use gdb to observe the issue.

**Note:** 
* For some reason this is not observed on MSVC C++ compilers.
