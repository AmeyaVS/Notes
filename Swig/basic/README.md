
# This folder contains the basic example for creating Python to C++ interfaces.


## Prerequisites

You'll need swig installed and also the python development headers for building the interface modules.


## Steps for creating the Interface modules.

 1. Create the *source files* with API's you'll be calling from python.  
    e.g: [example.c](example.c)
 2. Create the interface file:  
    e.g.: [example.i](example.i)
 3. Run swig to generate the wrapper source code:
    ```sh
    swig -python example.i
    ```
    **Note:** Kindly obeserve the output after running the command.
 4. Compile your source code along with the wrapper code.
    ```sh
    # Compile your source code
    gcc -c -fPIC example.c
    # Compile the generated wrapper code from swig.
    # **Note:** The -I gcc command line option for including the header from python sources.
    gcc -c -fPIC example_wrap.c -I/usr/include/python2.7
    ```
 5. Generate the shared library from the source code.
    ```sh
    gcc -shared example.o example_wrap.o -o _example.so -lpython2.7
    ```
    **Note:** The underscore before the shared library name without which the
    wrapper is unable to import the module. Also for windows use the extension __*.dll*__ instead of __*.so*__.  
    **Note:** You can also use the provided __*[build.sh](build.sh)*__ for completing the steps 3, 4, 5 and use the __*[clean.sh](clean.sh)*__ for cleaning the workspace.

## Testing the build dynamic module from python

 Here is a sample python script to test the functionality of the C/C++ API from Python.

 ```python
 import example
 
 # Calling the fact function.
 print example.fact(5)
 # Getting the value of global variable
 print example.cvar.My_variable
 # Calling the get_time function
 print example.get_time()
 # Calling my_mod function
 print example.my_mod(10, 3)
 print example.my_mod(34, 10)
 ```
 
 **Note:** You can also use the scrpit [run.py](run.py) provided to test the functionality of the module.