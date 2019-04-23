#!/bin/bash

GTEST_URL=https://github.com/google/googletest

# Return test function
function stepTry {
  "$@"
  local step_status=$?
  if [ $step_status -ne 0 ]; then
    echo "Error: $1" >&2
    exit $step_status
  fi
  return $step_status
}

pd=$PWD

if [ -d googletest ]; then
  echo " Repo already cloned"
  stepTry cd googletest
  stepTry git fetch --all --prune --tags
  stepTry git pull
  #stepTry git submodule update --force
else
  echo "Cloning the repository"
  stepTry git clone $GTEST_URL
  stepTry cd googletest
  #stepTry git submodule init
fi

echo "======================================================"
echo "Creating build directory"
echo "======================================================"
if [ -d "build" ]; then
  echo "!!!Directory Already Exists!!!"
else
  stepTry mkdir build
fi


echo "Configuration"

#stepTry ./autogen.sh

stepTry cd build

stepTry cmake -DCMAKE_INSTALL_PREFIX=$HOME/apps/gtest\
              -DCMAKE_BUILD_TYPE=Release\
              -Dgtest_build_samples=ON\
              -Dgtest_build_tests=ON\
              -Dgmock_build_tests=ON\
              -DBUILD_SHARED_LIBS=ON\
              ..
echo "Build"
stepTry make
echo "Test"
stepTry ctest
echo "Install"
stepTry make install

cd $pd
