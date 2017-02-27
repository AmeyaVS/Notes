# README.md

This folder contains the [Vim](http://www.vim.org/) configuration that I currently use for development purposes.
**Note:** Kindly refer the [vimrc_config](vimrc_config) file for current updated plug-ins.

## .vimrc configuration

Kindly refer to the [__*vimrc_config*__](vimrc_config) file for a look into my personal *.vimrc/_vimrc* configurations.

## Vim plugins

I currently use following plugins with Vim:
- [cscope](http://cscope.sourceforge.net)
- [nerdtree](https://github.com/scrooloose/nerdtree)
- [Pyclewn](http://pyclewn.sourceforge.net)
- [SrcExpl](https://github.com/wesleyche/SrcExpl)
- [taglist](https://github.com/vim-scripts/taglist.vim)
- [Trinity](https://github.com/wesleyche/Trinity)
- [vim-pathogen](https://github.com/tpope/vim-pathogen)
- [YouCompleteMe](https://github.com/Valloric/YouCompleteMe)

### Installing the individual plugins

First create following directories and files in you home folder of the user:

```sh
mkdir -p ~/.vim/autoload ~/.vim/bundle ~/.vim/plugin ~/.vim/undo
touch ~/.vimrc
```

- **Note 1:** If you are using Vim on Windows instead of creating .vimrc file and .vim folder kindly create *_vimrc* file and *vimfiles* folder under the home folder(base folder) of the user.
- **Note 2:** Currently only a subset of the plugins will work with Vim on Windows.
- **Note 3:** Kindly go through each plugin installation instructions once before reading further as these steps will vary depending on your environment and you personal preferences.

#### Cscope plugin

For installing this plugin first get the cscope_map.vim from [here](http://cscope.sourceforge.net/cscope_maps.vim).

```sh
wget http://cscope.sourceforge.net/cscope_maps.vim
mv cscope_maps.vim ~/.vim/plugin
```

#### nerdtree plugin

Following steps will help you installing the nerdtree plugin.

```sh
cd ~/.vim/bundle
git clone https://github.com/scrooloose/nerdtree.git
```

#### Pyclewn

First we need to install Pyclewn using pip:

```sh
pip install pyclewn --user
```

- **Note:** Kindly note the --user option passed to the pip command to install it as local user package.
- Also check if the path environment variable points to the ~/.local/bin folder.


Install the Vim runtime files with a vimball.

```sh
# Getting the vimball file.
python -c "import clewn; clewn.get_vimball()"
# Installing the vimball file
vim -S pyclewn-2.3.vmb
```

Quit Vim to complete the setup for Pyclewn for Vim.

**Note:** For running the pyclewn plugin with GVim you'll need to add following line to you __*~/.vimrc*__ config file.
```text
let g:pyclewn_terminal = "xterm, -e"
```

#### SrcExpl plugin

Following steps will help you installing the SrcExpl plugin.

```sh
cd ~/.vim/bundle
git clone https://github.com/wesleyche/SrcExpl.git
```

- **Note:** Kindly note that the SrcExpl along with the plugin YouCompleteMe have unexpected behaviour. Please apply the patch [SrcExpl.patch](SrcExpl.patch) available under the folder by first copying the patch file into the cloned SrcExpl git repo and applying the patch using:

```sh
git apply SrcExpl.patch
```

#### taglist plugin

Following steps will help you installing the taglist plugin.

```sh
cd ~/.vim/bundle
git clone https://github.com/vim-scripts/taglist.vim.git
```
 **Notes:**
1. Kindly Download the latest taglist plugin from [http://vim.sourceforge.net/scripts/script.php?script_id=273](http://vim.sourceforge.net/scripts/script.php?script_id=273).
2. When using [universal-ctags](https://github.com/universal-ctags/ctags) with vim kindly add the following line in the __*~/.vimrc*__ config file.
   ```text
   let g:Tlist_Ctags_Cmd = "u_ctags"
   ```

#### Trinity plugin

Following steps will help you installing the Trinity plugin.

```sh
cd ~/.vim/bundle
git clone https://github.com/wesleyche/Trinity.git
```

**Warning:** Kindly remove the nerdtree plugin packaged with Trinity.

#### vim-pathogen plugin

For installing the pathogen plugin kindly follow these steps:

```sh
git clone https://github.com/tpope/vim-pathogen.git
cp -rfv vim-pathogen/autoload/pathogen.vim ~/.vim/autoload
```

**Note:** Kindly add the line: "*execute pathogen#infect()*" in ~/.vimrc file to automatically add plugins by placing the plugin related files into the ~/.vim/bundle folder.

#### YouCompleteMe

For installing this plugin kindly refer to the [projects documentation](https://github.com/Valloric/YouCompleteMe) on building and installing the plugin for Vim.
