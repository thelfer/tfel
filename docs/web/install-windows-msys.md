---
title: Installing TFEL/MFront on [Windows](http://windows.microsoft.com) in the [`MSYS2`](https://www.msys2.org/) environment
author: Thomas Helfer, Antoine Martin and Valentin Gallican
date: 18/03/2026
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

This document describes how to install `TFEL/MFront` on [Windows
plateform](http://windows.microsoft.com) using the
[`MSYS2`](https://www.msys2.org/) environment.

# Installing and updating [`MSYS2`](https://www.msys2.org/)

## Installing [`MSYS2`](https://www.msys2.org/)

[`MSYS2`](https://www.msys2.org/) is a collection of GNU
utilities, in particular `make` which is used by `MFront`. It also
provides a convenient shell which is far more easier to use than the
`DOS` terminal provided by the `cmd` command.

## Updating [`MSYS2`](https://www.msys2.org/)

Open an [`MSYS2`](https://www.msys2.org/) MSYS terminal and run:

~~~~{.bash}
$ pacman -Syu
~~~~

Close the terminal after completion.

# Installing essential setup requirements

Open a [`MSYS2`](https://www.msys2.org/) MINGW64 terminal and run:

~~~~{.bash}
$ pacman -S mingw-w64-x86_64-toolchain \
            mingw-w64-x86_64-cmake \
            mingw-w64-x86_64-eigen3 \
            mingw-w64-x86_64-pybind11 \
            mingw-w64-x86_64-python \
            mingw-w64-x86_64-python-numpy \
            mingw-w64-x86_64-python-matplotlib \
            mingw-w64-x86_64-python-setuptools \
            git
~~~~

These packages supply the compiler, `cmake`, numerical libraries, and `Python` bindings that `TFEL` depends on.

# Cloning the `TFEL` repository

Go to a directory where you want to download the TFEL sources and run:

~~~~{.bash}
$ git clone https://github.com/thelfer/tfel.git
$ cd tfel
$ mkdir -p build
$cd build
~~~~

Then run:

~~~~{.bash}
$ cmake .. -G "MinGW Makefiles" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/mingw64 \
        -DPYTHON_EXECUTABLE=/mingw64/bin/python \
        -Denable-python=ON \
        -Denable-python-bindings=ON
        
  ~~~~

# Compiling and installing `TFEL`

Compile the project using:

~~~~{#compiling .bash}
$ mingw32-make -jYY
~~~~

Note: `YY` corresponds to the number of parallel jobs (for example, a CPU with 12 threads → -j12).

Then install it with:

~~~~{#compiling .bash}
$ mingw32-make install
~~~~

# Verifying the installation

To check whether MFront has been successfully installed, run:

~~~~{.bash}
$ which mfront
~~~~

If the installation succeeded, this command should return the path to the mfront executable. Alternatively, `TFEL` can also be installed using the official package by running:

~~~~{.bash}
$ pacman -S mingw-w64-x86_64-tfel
~~~~

However, this installation method does not provide the `Cast3M` and `Python` interfaces.

# Setting up the environment

Create a file named `env.sh` and add the following lines:

~~~~{.bash}
$ export TFELHOME=/mingw64
$ export PATH=/mingw64/bin:$PATH
$ export LD_LIBRARY_PATH=/mingw64/lib:$LD_LIBRARY_PATH
$ export PYTHONPATH=/mingw64/bin/python3.XX/site-packages
$ export MAKE=mingw32-make.exe
~~~~

Note: `XX` corresponds to the `Python` version installed in [`MSYS2`](https://www.msys2.org/).

# Loading the environment and running tests

Before using TFEL/MFront, load the environment with:

~~~~{.bash}
$ source env.sh
$ make check
~~~~

# Usage

Once done, MFront can be used as "usual" (like in Posix environments) in
[`MSYS2`](https://www.msys2.org/).

<!-- Local IspellDict: english -->
