---
title: Installing TFEL/MFront on [Windows](http://windows.microsoft.com) in the [`MSYS2`](https://www.msys2.org/) environment
author: Thomas Helfer
date: 13/01/2022
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

This document describes how to install `TFEL/MFront` on [Windows
plateform](http://windows.microsoft.com) in the
[`MSYS2`](https://www.msys2.org/) environment.

# Installing [`MSYS2`](https://www.msys2.org/) and the mandatory prerequisites

## Installing [`MSYS2`](https://www.msys2.org/)

[`MSYS2`](https://www.msys2.org/) is a collection of GNU
utilities, in particular `make` which is used by `MFront`. It also
provides a convenient shell which is far more easier to use than the
`DOS` terminal provided by the `cmd` command.

## Installing a minimal build environment

To build `TFEL`, one may need to install the following tools:

~~~~{.bash}
$ pacman -S base-devel gcc cmake
~~~~

# Installing the official package

~~~~{.bash}
$ pacman -S mingw-w64-x86_64-tfel
~~~~

# Compiling `TFEL`

`git` may be required if you want to work on the development branch:

~~~~{.bash}
$ pacman -S git
~~~~

Compiling `TFEL` is merely a simple as opening a new `MSYS2` session
and following the instructions given on the
[main installation page](install.html).

For example:

~~~~ {#compiling .bash}
$ cmake [path to TFEL sources] [options]
$ make
~~~~~~~~~~~~~~~~~~~~~~

## Running the tests

~~~~ {#testing .bash}
$ source env.sh
$ make check
~~~~~~~~~~~~~~~~~~~~~~


# Usage

Once done, mfront can be used as "usual" (like in Posix environments) in
[`MSYS2`](https://www.msys2.org/).

<!-- Local IspellDict: english -->
