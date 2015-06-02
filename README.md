arrowline - small shell prompt generator
========================================

Small shell (bash/zsh) prompt generator written in C. Uses powerline symbols.

Features
--------

- single executable, no external processes
- configurable with a header file (like dwm)
- powerline-style


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


Configuration
-------------

All configuration takes place in `config.h`.

- Define the colors for the segments.
- Select which segments to show.


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

