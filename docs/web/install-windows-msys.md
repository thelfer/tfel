% Installing TFEL/MFront on [Windows](http://windows.microsoft.com) with [MingGW](http://www.mingw.org/)
% Helfer Thomas
% November 3, 2014

This document describes how to install `TFEL/MFront` on
[Windows plateform](http://windows.microsoft.com) using
[MingGW](http://www.mingw.org/) and [`cmake`](http://www.cmake.org/).

The main difficulty associated with building `TFEL/MFront` on
[Windows plateform](http://windows.microsoft.com) is the lack of an
unified build environment which must be set up by the user. We suppose
that you want to build `TFEL/MFront` on top of the `Cast3M` finite
element solver which comes with its own software environment based on
`MinGW`.

# Installing the mandatory prerequisites

## Installing [MSYS](http://www.mingw.org/wiki/MSYS)

[MSYS](http://www.mingw.org/wiki/MSYS) is a collection of GNU
utilities, in particular `make` which is used by `MFront`. It also
provides a convenient shell which is far more easier to use than the
`DOS` terminal provided by the `cmd` command.

We will use the `mingw-get-setup.exe` program that can be grabbed
[here](http://sourceforge.net/projects/mingw/files/Installer).

Installing [MSYS](http://www.mingw.org/wiki/MSYS) is a \(3\) steps
process depicted on the following figures:

![Splash Screen (Click on `Continue`)](img/mingw-get1.png
	"Splash Screen (Click on `Continue`)")

This first step allows you to choose the installation directory of
[MSYS](http://www.mingw.org/wiki/MSYS). The default installation path
is `C:\MinGW`.

![Downloading the latest list of packages](img/mingw-get2.png
 "Downloading the latest list of packages (Click on `Continue` once
 finished)")

The second windows appears while `mingw-get-setup.exe` downloads the
latest list of packages. Just click on `Continue` once finished.

![Selecting the components to be installed (Select `msys-base`)](img/mingw-get3.png
 "Selecting the components to be installed")

The third windows allows you to select the components to be
installed. **The only one required for `MFront` is the `msys-base`
package.**

Once the components are selected, choose `Apply Changes` in the
`Installation` menu and click on `OK` to launch the components
installation.

### User directory location

Once done, the `msys.bat` script launches a shell which is very
similar to standard UNiX shells.

Assuming that `MSYS` has been installed in `C:\MinGW`, the user
directory is located here:

~~~~ {#home .bash}
C:\MinGW\msys\1.0\home\[user name]
~~~~~~~~~~~~~~~~~~~~~~

This allows easy exchange of file between the
[Windows](http://windows.microsoft.com) world and the`MSYS` one.

Inside `MSYS`, one can access most
[Windows](http://windows.microsoft.com) files through the `/c/` path:

~~~~ {#c .bash}
$ ls /c/
MinGW Program Files users windows
~~~~~~~~~~~~~~~~~~~~~~

## Installing `cmake`

The `cmake` project provides a graphical installer on its
[website](http://www.cmake.org).

## Installing `NSIS`

At the end of the compilation process, we will create a graphical
installer based on `NSIS` which can be grabbed
[here](http://nsis.sourceforge.net/).

# Compiling `TFEL`

Compiling `TFEL` is merely a simple as opening a new `MSYS` session
and following the instructions given on the
[main installation page](install.html).

The main differences are that one must:

- add to the `PATH` environment variable, the path to were `Cast3M`
  has installed the `MinGW` compilers.
- pass an appropriate flag to `cmake` to generate appropriate
  `Makefiles` for the `NSIS``build environment.

~~~~ {#compiling .bash}
$ export PATH=/c/Cast3M/PCW_14/MinGW/x32-4.6.3-posix-dwarf-rev2/mingw/bin/:$PATH
$ cmake -G "MSYS Makefiles" [path to TFEL sources] [options]
$ make
$ cpack -G "NSIS"
~~~~~~~~~~~~~~~~~~~~~~

# Installation

An executable called `tfel-..` has been build and can be
executed to install `TFEL`.

This executable is a standard graphical installer which allows you to
choose which components of `TFEL` you wish to install:

- `core` contains the `TFEL` library.
- `mfront` et `mtest` contain the `MFront` code generator and `MTest`
  respectively.
- `docs` contains the documentation in `pdf` format.
- `website` contains the `TFEL` website.

The installer also allows you to choose where `TFEL` shall be
installed: **`TFEL` must be installed in a path without blanks**, for
example in the directory `C:TFEL`.

We strongly advice the user to create or modify its `.profile` file in
its home directory in the `MSYS` environment to get the `MinGW`
compilers provided by `Cast3M` in the path by appending the following
lines:

~~~~ {#home .profile}
export PATH=/c/Cast3M/PCW_14/MinGW/x32-4.6.3-posix-dwarf-rev2/mingw/bin/:$PATH
export PATH:/c/Cast3M/PCW_14/bin/:$PATH
alias castem='cmd //c castem14.bat'
~~~~~~~~~~~~~~~~~~~~~~

Assuming a standard `MSYS` environment, the file is located here:

~~~~ {#home .bash}
C:\MinGW\msys\1.0\home\[user name]\.profile
~~~~~~~~~~~~~~~~~~~~~~

# Usage

Once done, mfront can be used as "usual" (like in Posix environments)
in [MSYS](http://www.mingw.org/wiki/MSYS).

<!-- Local IspellDict: english -->
