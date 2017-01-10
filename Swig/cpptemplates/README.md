# This folder contains the example for using C++ Templates with SWIG

## Directory Structure

 * [CMakeLists.txt](CMakeLists.txt): CMake build configuration file.
 * [include/pair.h](include/pair.h): Implementation of Template Class.
 * [src/pair.i](src/pair.i): Interface file for SWIG interface generation instantiaing the Template Classes.
 * [test.py](test.py): Test python script.

## Building the example

 1. Create the build directory and navigate into the build directory.
    
    ```sh
    mkdir build
    cd build
    ```
 2. Generate Makefile using CMake generator:
    
    ```sh
    cmake ..
    ```
 3. Build the shared library:
    
    ```sh
    make
    ```
 4. Copy the [test.py](test.py) to build directory:
    ```sh
    cp -rfv ../test.py .
    ```
## Testing
 
 Run the test python script
 ```sh
 python test.py
 ```
 **Note:** You should see output something like this:
 ```text
 a= 3 4
 a= 3 16
 b= 3.14159 8
 c= 3 16
 c= 3 10
 a= 3 10
 ```