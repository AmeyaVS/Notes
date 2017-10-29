# README.md

This example is taken from the below mentioned link:
cmake swig install python example
[http://www.matthiaskauer.com/2013/11/swig-for-python-cmake-and-a-speed-comparison/](http://www.matthiaskauer.com/2013/11/swig-for-python-cmake-and-a-speed-comparison/).

## Building

For building the source code kindly follow the steps mentioned in [../cpptemplates/README.md](../cpptemplates/README.md).


## Running

 Copy the test script __*[test_fib.py](test_fib.py)*__ to the build folder.
 
 ```sh
 # pwd currently should be the build folder!!!
 cp -rfv test_fib.py .
 python test_fib.py
 ```
 
 Running native application:
 ```sh
 time ./Fibonacci 20000 14
 ```