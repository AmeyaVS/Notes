#!/usr/bin/bash

# This script has been tested on Ubuntu 16.04 with gcc-8

SRC_DIR=boost_1_69_0
ZIP_FILE=$SRC_DIR.tar.bz2
DOWNLOAD_URL=https://dl.bintray.com/boostorg/release/1.69.0/source/$ZIP_FILE

# This script will download the Boost library source from its official website and configure the build and build it for you.
if [ -f "$ZIP_FILE" ]; then
    echo "======================================================"
    echo "Downloaded file exists"
    echo "======================================================"
else
    echo "======================================================"
    echo "Downloading the Boost library from website"
    echo "======================================================"
    wget -c "$DOWNLOAD_URL"
fi

if [ -d "$SRC_DIR" ]; then
    echo "======================================================"
    echo " Source Directory Exists."
    echo "======================================================"
else
    echo "======================================================"
    echo "Extracting the Boost source code"
    echo "======================================================"
    tar xf $ZIP_FILE
fi

pd=$PWD

cd $SRC_DIR

# Configure the Boost library
echo "============================================================================"
echo "Configuring the Boost library"
echo "============================================================================"

./bootstrap.sh --with-libraries=all --with-python-version=3.5 --with-icu="/usr/include/x86_64-linux-gnu/"

# Build the Boost library
echo "======================================================"
echo "Building the Boost library"
echo "======================================================"

./b2 toolset=gcc-8 cxxflags="-std=c++17" -j2

