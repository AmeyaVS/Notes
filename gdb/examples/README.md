# README.md

Following sample applications are used to demonstrate various features for GDB:

* [01Intro](01Intro): Contains a simple implementation of bubble sort and merge sort.
* [02StaticBuildAndLibrary](02StaticBuildAndLibrary): Contains a simple implementation showing debugging with and without shared libraries.
* [03SegFault](03SegFault): Simple Application showcasing the Segmentation Fault.
* [04NullPtr](04NullPtr): Simple Application showing NULL pointer deference error and memory aliasing.
* [05SystemCExample](05SystemCExample): A simple example showing debugging with SystemC C++ library.

**Note:** That some of the examples have straight forward bugs but some of them
have issues not that staright forward to catch, and some of them have no bugs at
all.

## How to build

You can build the examples in both debug and release build.  
By default the top level CMakeLists.txt has debugging symbols enabled for the
examples.

Following instructions are to be followed from base folder of the examples.

Try Debugging in both the build option with and without optimizations also keep
note of different execution observations.

### Building without Optimizations.

```sh
$ mkdir build_debug
$ cd build_debug
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make
```

### Building with Optimizations.

```sh
$ mkdir build_release
$ cd build_release
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```

