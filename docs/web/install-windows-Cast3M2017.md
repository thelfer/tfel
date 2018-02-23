% How to build and use `TFEL` and `MFront` on `Windows` with the `MinGW` compiler delivered with `Cast3M` `2017`
% Thomas Helfer
% 1/09/2017

Building and using `MFront` on `Windows` is more complex than on other
systems for the following reasons:

- There is no default compiler on this platform.
- The usage of command line is less common.

This page is dedicated to show how this issues can be overcome and
ease the `MFront` experience on `Windows`. We will focus on coupling
with `Cast3M` `2017` which is packaged with a version of the `MinGW`
compiler suite. **This tutorial makes the assumption that the user
have installed the developer version of `Cast3M`**.

This tutorial will show that:

- The installation does not require any administrator privileges.
- The main trick is to appropriately update the `PATH` variable so
  that the `MinGW` and `TFEL` libraries are found when needed. This
  can be done by creating small wrapper scripts as discussed bellow.

This tutorial is mostly based on `bat` scripts that can be downloaded
[here](downloads/windows-install-scripts.tar.bz2).

## Notes on the installation directory

For information, the author use the following convention: the scripts
are decompressed in `C:\tfel-master\src`.

By default, the previous scripts will install `TFEL` and `MFront` in
`C:\tfel-master\install-castem2017`. The user is, of course, free to
choose another installation location: he simply has to edit the
scripts accordingly.

## Notes on `bat` scripts

Executing `bat` scripts is fairly easy: in `Windows` `Explorer`,
right-click on the script and select `Open`.

Modifying `bat` scripts is also easy: in `Windows` `Explorer`,
right-click on the script and select `Modify`. By default, this action
will open the script in `Notepad`, but any text editor will do.

## About binary packages

Various binary packages can be downloaded on the `sourceforge`
website. Those binary packages are built against specific versions of
`Cast3M`. When installing thoses packages, the user have the choice to
modify the `PATH` variable globally.

This tutorial can nevertheless be usefull: small scripts wrapping
calls to `MFront` can be a viable choice in the `Windows` environment,
rather than relying on the command line. Be careful that modify the
examples appropriately to reflect where `TFEL` and `MFront` have been
installed.

# Decompressing the archive

As said before, this tutorial is mostly based on `bat` scripts that
can be downloaded [here](downloads/windows-install-scripts.tar.bz2).

The archive contains the following files:

- `git.bat`: a script used to fetch the latest source from the
  `github` repository.
- `git-update.bat`: a script used to update the sources to the latest
  version.
- `compil-castem2017.bat`: a script used to compile `TFEL` and
  `MFront` using the compiler delivered with `Cast3M` `2017`.

The two first scripts are not mandatory: the user can directly
download an archive from the sourceforge website
(<https://sourceforge.net/projects/tfel/files/>).

It also contains a directory called `test`. The usage is those scripts
is fully detailed in the following sections.

The other files contained in the archive will not be used in this
tutorial.

# Prerequisites

In the following, we will suppose that:

- `git` is installed. This is not mandatory. Users that do not want to
  follow the latest developments of `TFEL` and `MFront` can download a
  packaged version on the sourceforge website. The tutorial will
  mostly apply, but the name of the directory containing the sources
  must be changed. `git` for `Windows` can be downloaded
  [here](https://git-scm.com/download/win)
- `cmake` is installed. `cmake` can be downloaded
  [here](https://cmake.org/download/).
- `Cast3M` `2017` **developer version** is installed. `Cast3M` can be
  downloaded [here](http://www-cast3m.cea.fr/index.php?xml=download1).

> **Notes**
>
> In the following, it is expected that those prerequisites are
> located in directories listed in the `PATH` variables. If this is
> not the case, the scripts must be modified to update this variable
> appropriately.

# Getting the sources using `git`

This section can be skipped if the user wants to use a packaged
version of tfel (<https://sourceforge.net/projects/tfel/files/>).

## Cloning the repository

The `git.bat` will clone the `git` repository on your machine in the
current directory. The directory containing the repository is called
`tfel`.

## Updating the repository

After the `git` repository has been cloned, the `git-update.bat`
script can be used to synchronize our local repository with the
official repository and follow the latest developments.

# Compiling `MFront`

The `compil-castem2017.bat` can now be used to compile and install
`TFEL` using the `MinGW` compiler delivered with `Cast3M` `2017`.

## Changing the location of `Cast3M` `2017`

The script assumes that `Cast3M` `2017` is installed in
`C:\Cast3M\PCW_17`. If this is not the case, edit the script and
change the following line appropriately:

~~~~{.bash}
set PATH=C:\Cast3M\PCW_17\MinGW\x86_64-6.3.0-posix-sjlj-rt_v5-rev1\mingw64\bin;%PATH%
~~~~

## Changing the location of the sources

By default, the script assumes that the sources are in the `tfel`
subdirectory. This is the case if the user cloned the `git` repository
as explained before.

If the user has downloaded an archive from the `sourceforge` website,
the name of the directory can be different and the user must update
the following line:

~~~~{.bat}
cmake.exe ..\tfel -G "MinGW Makefiles" ...
~~~~

For example, if you have downloaded version `3.0.1`, the name of the
archive is `tfel-3.0.1.tar.bz2`. Assuming that this archive is
decompressed in the same directory than the scripts, the name of the
directory containing the sources will be `tfel-3.0.1`. The previous
line can thus be udpated as follows:

~~~~{.bat}
cmake.exe ..\tfel-3.0.1 -G "MinGW Makefiles" ...
~~~~

## Changing the installation directory

The installation directory is passed to `cmake` through the
`CMAKE_INSTALL_PREFIX` variable. The default install directory is
`C:\tfel-master\install-castem-2017`. The user can modify this value
to fit its needs.

# A simple test showing how to perform `Cast3M` `2017` simulations with a `MFront` behaviour

The `test` directory contains an example showing how to perform
`Cast3M` `2017` simulations with a `MFront` behaviour. It contains:

- a `mfront.bat` script.
- a `launch-castem.bat` script.
- a `MFront` behaviour called `LogarithimcStrainPlasticity`.
- a `Gibiane` script called `LogarithimcStrainPlasticity-ssna303.dgibi`.

## Compilation the `MFront` behaviour

Launch the `mfront.bat` script. This script is basically a wrapper
around the following call to `MFront`:

~~~~{.bash}
mfront.exe --obuild --interface=castem LogarithmicStrainPlasticity.mfront
~~~~

This command:

- generates `C++` sources from the
  `LogarithmicStrainPlasticity.mfront` file and a `Makefile.mfront`
  file.
- calls the `make` utility to compile the sources an generate the
  `libUmatBehaviour.dll` library.

There is one trick here: the name of `make` utility in the `MinGW`
compiler suite is `mingw32-make`. For `MFront` to call the appropriate
program, the `MAKE` environment variable must be set. This is done in
the script at the following line:

~~~~{.bash}
set MAKE=mingw32-make
~~~~

## Launch the `Cast3M` `2017` simulation

The `Cast3M` solver can be used directly because it can't find the
`TFEL` libraries automatically. The `launch-castem.bat` script thus
first updates the `PATH` variable and then launches the `Cast3M`
solver script.

Launch the `launch-castem.bat` script to perform the simulation.

<!-- Local IspellDict: english -->
