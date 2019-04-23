#!/bin/bash

CTAGS_URL=https://github.com/universal-ctags/ctags

# Return test function
function stepTry {
  "$@"
  local step_status=$?
  if [ $step_status -ne 0 ]; then
    echo "Error: $1" >&2
  fi
  return $step_status
}

pd=$PWD

if [ -d ctags ]; then
  echo " Repo already cloned"
  stepTry cd ctags
  stepTry git fetch --all --prune --tags
  stepTry git pull
  #stepTry git submodule update --force
else
  echo "Cloning the repository"
  stepTry git clone $CTAGS_URL
  stepTry cd ctags
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

stepTry ./autogen.sh

stepTry cd build

stepTry ../configure --prefix=$HOME/apps/ctags --program-prefix=u_
echo "Build"
stepTry make
echo "Test"
stepTry make check
echo "Install"
stepTry make install

cd $pd
