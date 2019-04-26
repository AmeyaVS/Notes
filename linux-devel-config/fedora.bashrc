# Source global definitions
if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# User specific environment
PATH="$HOME/.local/bin:$HOME/bin:$PATH"
export PATH

#!/bin/bash

# Get Bash-Completion to work first
# https://github.com/scop/bash-completion
[[ $PS1 && -f /usr/share/bash-completion/bash_completion ]] && \
    . /usr/share/bash-completion/bash_completion

# https://fedoraproject.org/wiki/Git_quick_reference
source /usr/share/git-core/contrib/completion/git-prompt.sh
# Refer: https://gist.github.com/sheldonh/41d4422605d4909cd2de
export GIT_PS1_SHOWDIRTYSTATE=true
export GIT_PS1_SHOWUNTRACKEDFILES=true
export GIT_PS1_SHOWCOLORHINTS=true
# My Custom Bash Setup
# http://syswolflinux.com/2018/04/how-to-easily-customize-your-bash-prompt-on-ubuntu-fedora
# https://www.kirsle.net/wizards/ps1.html
#export PS1=
# Custom bash prompt via kirsle.net/wizards/ps1.html
#export PS1="\[$(tput bold)\]\[$(tput setaf 2)\]\u\[$(tput setaf 7)\]@\[$(tput setaf 4)\]\h \[$(tput sgr0)\]\[$(tput setaf 3)\]\W\[$(tput setaf 6)\]\[\$( __git_ps1 '(%s)')\]\[$(tput setaf 2)\]\\$\[$(tput sgr0)\]"
# Custom bash prompt via kirsle.net/wizards/ps1.html
#export PS1="[\u@\h:\W \$( __git_ps1 '(%s)')]\$"

# Custom bash prompt via kirsle.net/wizards/ps1.html
export PS1="\[$(tput bold)\]\[$(tput setaf 2)\]\u\[$(tput setaf 7)\]@\[$(tput setaf 4)\]\h:\[$(tput sgr0)\]\[$(tput setaf 3)\]\w\[$(tput setaf 6)\]\$(__git_ps1 '(%s)')\[\]\[$(tput setaf 1)\]\\$ \[$(tput sgr0)\]"

# VirtualEnv and VirtualEnvWrapper
export WORKON_HOME=$HOME/apps/pyenv
export VIRTUALENVWRAPPER_PYTHON=/usr/bin/python3
source ~/.local/bin/virtualenvwrapper.sh

# SystemC
export SYSTEMC_HOME=$HOME/apps/systemc-2.3.3
export LD_LIBRARY_PATH=$SYSTEMC_HOME/lib64:$SYSTEMC_HOME/lib

# Sphinx Documentation
#export Sphinx_DIR=$HOME/.local/bin

# Jansson
export JANSSON_HOME=$HOME/apps/jansson
export LD_LIBRARY_PATH=$JANSSON_HOME/lib:$LD_LIBRARY_PATH
export PKG_CONFIG_PATH=$JANSSON_HOME/lib/pkgconfig

# Ctags
export UCTAGS_HOME=$HOME/apps/ctags
export PATH=$UCTAGS_HOME/bin:$PATH

# GoogleTest
export GTEST_HOME=$HOME/apps/gtest
export GTEST_COLOR=1
export LD_LIBRARY_PATH=$GTEST_HOME/lib64:$GTEST_HOME/lib:$LD_LIBRARY_PATH
export PKG_CONFIG_PATH=$GTEST_HOME/lib64/pkgconfig:$GTEST_HOME/lib/pkgconfig:$PKG_CONFIG_PATH

# uftrace
export UFTRACE_HOME=$HOME/apps//uftrace
export PATH=$UFTRACE_HOME/bin:$PATH
source $UFTRACE_HOME/etc/bash_completion.d/uftrace

# tracer
export TRACER_HOME=$HOME/apps/tracer
export PATH=$TRACER_HOME/bin:$PATH

# Jacksum
export JACKSUM_HOME=$HOME/apps/src/jacksum
export PATH=$JACKSUM_HOME/unix:$PATH

# Boost
export BOOST_ROOT=$HOME/apps/src/boost/boost_1_70_0
export BOOST_HOME=$BOOST_ROOT
export LD_LIBRARY_PATH=$BOOST_ROOT/stage/lib:$LD_LIBRARY_PATH
