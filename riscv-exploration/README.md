# RISC-V Development environment setup #

**Disclaimer!** This document is still very much in development. Kindly also refer the official documentation on the RISCV github repositories.

Notes for setting up the development environment for the RISC-V tools and libraries.

## Prereqisites ##

1. The RISC-V tools and libraries require a C++11 compatible compiler for building the tools.
2. The process requires an active internet connection for downloading for dependency sources and libraries.

## Steps ##

1. Clone the [riscv-tools](https://github.com/riscv/riscv-tools) repository.
    ```sh
    git clone https://github.com/riscv/riscv-tools
    ```
2. Follow the steps for installing the prerequisites from the [README.md](https://github.com/riscv/riscv-tools/blob/master/README.md).  
   **Note:** Replicated here for convinience. It may be outdated also.
    ```sh
    sudo apt-get install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev libusb-1.0-0-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev device-tree-compiler pkg-config libexpat-dev
    ```
3. After the cloning is finished, and the dependencies are installed. Fetch the `git submodules`.
    ```sh
    git submodule update --init --recursive
    ```
4. Setup the `RISCV` environment variable where you want the riscv tools and libraries to be installed.
    ```sh
    # For e.g.: export RISCV=$HOME/apps/riscv
    export RISCV=/path/to/install/riscv/toolchain
    ```
5. Run the Build Shell script to build the RISC-V [spike](https://github.com/riscv/riscv-isa-sim) reference simulator and the bare metal `GNU Compiler Toolchain`.  
   **Note:** Go for a coffee break as this will take quiet a while.
    ```sh
    ./build.sh
    ```
   **Tip:** To Speed up the build you can set the environment variable `$MAKEFLAGS=-j<ncpu>`, where `<ncpu>` is the number of CPU cores available in the build system.
6. Once the build is completed without errors. Check the directory `$RISCV` provided earlier for the installed tools and libraries.
    ```
    # Sample directory structure
    $HOME/apps/riscv  #<---- Example configuration location.
    |-bin
    | |-openocd       #<---- OpenOCD Debugger Interface.
    | |-elf2hex
    | |-riscv64-unknown-elf-gcc #<---- The Bare metal GNU RISC-V Compiler Toolchain.
    | |-riscv64-unknown-elf-*   #<---- Multiple tools.
    | |-spike         #<----- The Golden Reference functional simulator.
    |-include
    |-lib
    |-libexec
    |-riscv64-unknown-elf
    |-share
    |-sysroot
    ```
7. Follow the instructions on the [README.md](https://github.com/riscv/riscv-isa-sim/blob/master/README.md) for testing the compiler toolchain and the spike simulator.

