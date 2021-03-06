# linux-devel-config #

## Bash Shell Setup ##

* Shell Git Branch Configuration:

    ```sh
    # For git branches
    export PS1=$PS1'$(__git_ps1 "\[\e[38;5;87m\](%s) \[\e[0m\]")'
    ```
    **Note:** For using the `__git_ps1` one should already have the bash completion package installed.

## Tools and Libraries ##

List of tools and libraries for development purposes:

**Note:** Available as Ubuntu command line
```sh
sudo apt install \
                 autoconf \
                 automake \
                 autotools-dev \
                 bc \
                 binutils \
                 bison \
                 bridge-utils \
                 build-essential \
                 chrpath \
                 cmake \
                 cscope \
                 ctags \
                 curl \
                 device-tree-compiler \
                 dkms \
                 doxygen \
                 flex \
                 gawk \
                 gcc \
                 gdb \
                 git \
                 gitg \
                 gperf \
                 graphviz \
                 libc6-dev \
                 libexpat-dev \
                 libglib2.0-dev \
                 libgmp-dev \
                 libmpc-dev \
                 libmpfr-dev \
                 libncurses5-dev \
                 libncursesw5-dev \
                 libpixman-1-0 \
                 libpixman-1-dev \
                 libsdl1.2-dev \
                 libtool \
                 libusb-1.0-0-dev \
                 meld \
                 patchutils \
                 pkg-config \
                 putty \
                 python3-dev \
                 python3-pip \
                 python3-tk \
                 python-dev \
                 python-pip \
                 ruby \
                 ruby-dev \
                 swig \
                 texinfo \
                 uml-utilities \
                 vim \
                 zlib1g-dev \
                 libxml2-dev \
                 libjansson-dev \
                 libyaml-dev \
                 libaspell-dev \
                 docutils-common \
                 seccomp \
                 libseccomp-dev \
                 clang \
                 llvm-runtime \
                 libelf-dev \
                 libdwarf-dev \
                 libdw-dev \
                 pandoc \
                 libbz2-dev \
                 libzstd-dev \
                 clang-tidy \
                 libclang-dev \
                 llvm \
                 libcapstone-dev
```
**Note:** This list is transient as it will keep on changing overtime, as and when I will be working on various tools and libraries.

## Fedora Shell Configuration

Refer the file: [fedora.bashrc](fedora.bashrc) for more details.

### Features

* Enables bash tab completion feature.
* Setup a custom PS1 prompt.
* Setup Git PS1 prompt for git repos.
* Plus extra config for external libraries and utilities.

### Some Common applications/libraries for optimal development environment

* [apps](apps): Contains basic building bash scripts for building a set of
  commonly used tools/utilities or libraries for development activities.
  * [ctags](apps/ctags-build.sh): Universal-Ctags.
  * [googletest](apps/googletest-build.sh): C++ Unit test framework.
  * **TODO:**
    * [cppcheck](): An open source C/C++ static analysis tool.
    * [uftrace](): A C/C++ application profiler
    * [uncrustify](): Source code beautifier

