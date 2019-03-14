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
                 libzmq1 \
                 libzmq-dev \
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
                 libseccomp-dev
```
**Note:** This list is transient as it will keep on changing overtime, as and when I will be working on various tools and libraries.

## Fedora Shell Configuration

Refer the file: [fedora.bashrc](fedora.bashrc) for more details.

### Features

* Enables bash tab completion feature.
* Setup a custom PS1 prompt.
* Setup Git PS1 prompt for git repos.
* Plus extra config for external libraries and utilities.
