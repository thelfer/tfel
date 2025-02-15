% How to build and use `TFEL` and `MFront` on `Windows` with the `Visual Studio` compiler
% Thomas Helfer
% 1/09/2017

Building and using `MFront` on `Windows` is more complex than on other
systems for the following reasons:

- There is no default compiler on this platform. In this tutorial, we
  will use the `Microsoft` `Visual Studio` compiler.
- The usage of command line is less common.

This page is dedicated to show how this issues can be overcome and
ease the `MFront` experience on `Windows`.

This tutorial will show that:

- The installation does not require any administrator privileges.
- The main trick is to appropriately update the `PATH` variable so
  that the `TFEL` libraries are found when needed. This can be done by
  creating small wrapper scripts as discussed bellow.

This tutorial is mostly based on `bat` scripts that can be downloaded
[here](downloads/windows-install-scripts.tar.bz2).

## Notes on the installation directory

For information, the author use the following convention: the scripts
are decompressed in `C:\tfel-master\src`.

By default, the previous scripts will install `TFEL` and `MFront` in
`C:\tfel-master\install-vs`. The user is, of course, free to choose
another installation location: he simply has to edit the scripts
accordingly.

## Notes on `bat` scripts

Executing `bat` scripts is fairly easy: in `Windows` `Explorer`,
right-click on the script and select `Open`.

Modifying `bat` scripts is also easy: in `Windows` `Explorer`,
right-click on the script and select `Modify`. By default, this action
will open the script in `Notepad`, but any text editor will do.

# Decompressing the archive

As said before, this tutorial is mostly based on `bat` scripts that
can be downloaded [here](downloads/windows-install-scripts.tar.bz2).

The archive contains the following files:

- `git.bat`: a script used to fetch the latest source from the
  `github` repository.
- `git-update.bat`: a script used to update the sources to the latest
  version.
- `compil-vs.bat`: a script used to compile `TFEL` and `MFront` using
  the `Visual Studio` compiler.

The two first scripts are not mandatory: the user can directly
download an archive from the github website
(<https://github.com/thelfer/tfel/releases>).

It also contains a directory called `test`. The usage is those scripts
is fully detailed in the following sections.

The other files contained in the archive will not be used in this
tutorial.

# Prerequisites

In the following, we will suppose that:

- `git` is installed. This is not mandatory. Users that do not want to
  follow the latest developments of `TFEL` and `MFront` can download a
  packaged version on the github website. The tutorial will
  mostly apply, but the name of the directory containing the sources
  must be changed. `git` for `Windows` can be downloaded
  [here](https://git-scm.com/download/win)
- `cmake` is installed. `cmake` can be downloaded
  [here](https://cmake.org/download/).
- `Visual Studio` `2015` is installed.

> **Notes**
>
> At least version `2015` of the `Visual Studio` compiler suite is
> required.
>
> Full support of `Visual Studio` on `Windows` requires at least
> version `3.1.0` of `TFEL`, which is currently under development.
> Indeed, the last part of this tutorial is based on the support of
> the `CMake` generator by `MFront` (see the
> [release notes of version 3.1](./release-notes-3.1.html) for
> details).

> **Notes**
>
> In the following, it is expected that `git` and `cmake` are
> located in directories listed in the `PATH` variables. If this is
> not the case, the scripts must be modified to update this variable
> appropriately.

# Getting the sources using `git`

This section can be skipped if the user wants to use a packaged
version of `TFEL` (<https://github.com/thelfer/tfel/releases>).

## Cloning the repository

The `git.bat` will clone the `git` repository on your machine in the
current directory. The directory containing the repository is called
`TFEL`.

## Updating the repository

After the `git` repository has been cloned, the `git-update.bat`
script can be used to synchronize our local repository with the
official repository and follow the latest developments.

# Compiling `MFront`

The `compil-vs.bat` can now be used to compile and install `TFEL`
using the `Visual Studio` compiler suite.

This script supposes that the user uses `Visual` `Studio` version
`2015`. Although untested yet, we expect `TFEL` to build with more
recent versions. The version of `Visual` `Studio` to be used can be
changed by modifying the value following the `-G` parameter:

~~~~{.bash}
cmake.exe ..\tfel -G "Visual Studio 14 2015 Win64" ...
~~~~

See the [`cmake` documentation for details](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html).

## Changing the location of the sources

By default, the script assumes that the sources are in the `tfel`
subdirectory. This is the case if the user cloned the `git` repository
as explained before.

If the user has downloaded an archive from the `github` website,
the name of the directory can be different and the user must update
the following line:

~~~~{.bat}
cmake.exe ..\tfel ...
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
`C:\tfel-master\install-vs`. The user can modify this value
to fit its needs.

# A simple test showing how to perform `Cast3M` `2017` simulations with a `MFront` behaviour

The `test-vs` directory contains an example showing how to build a
mfront library and how to perform `Cast3M` `2017` simulations with a
`MFront` behaviour.

It contains:

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
  `LogarithmicStrainPlasticity.mfront` file and a `CMakeLists.txt`
  file.
- calls the `cmake` utility to compile the sources an generate the
  `UmatBehaviour.dll` library. This library will be located in the
  `src\Release` directory.

## Launch the `Cast3M` `2017` simulation

The `Cast3M` solver can be used directly because it can't find the
`TFEL` libraries automatically. The `launch-castem.bat` script thus
first updates the `PATH` variable and then launches the `Cast3M`
solver script.

Launch the `launch-castem.bat` script to perform the simulation.

<!-- Local IspellDict: english -->
