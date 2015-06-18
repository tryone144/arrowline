arrowline - small shell prompt generator
========================================

Small shell (bash/zsh) prompt generator written in C. Uses powerline symbols.

Features
--------

- single executable, no external processes
- configurable with a header file (like dwm)
- powerline-style
- git branch name and status


Installation
------------

Arch Linux PKGBUILD included. Generate the `.tar.xz` and install (needs root privileges) with:

    makepkg -i

Manual install:

    git clone git://github.com/tryone144/arrowline.git
    cd ./arrowline
    make

Systemwide installation (as root) into `/usr/local/bin`:

    make install

If you install by hand make sure that `libgit2` is installed for git support.


Configuration
-------------

All configuration takes place in `config.h`.

- Define the colors for the segments.
- Select which segments to show:

    - `STATUS`: display red X if last command's exit status != 0
    - `HOST`: display username and hostname
    - `CWD`: display `CWD_LEN` (defaults to 3) folders of current working directory
    - `VCS`: display branch and status if current working dir is inside a version controlled directory
        
        - currently only supports git    
        - please note that status fetching may be slow in large projects


Using arrowline
---------------

### bash

Put into your `.bashrc`:

    # arrowline prompt
    function _update_ps1() {
        export PS1="$(arrowline ${?} 2> /dev/null)"
    }
    export PROMPT_COMMAND=_update_ps1


TODO
----

- Make segement generation more modular
- zsh compatible
- 'flat' mode (without powerline chars)
- add new segments

License
-------

(c) 2015 Bernd Busse
The MIT License. See [LICENSE](./LICENSE) for detais.

