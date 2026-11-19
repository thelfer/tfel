---
title: Building official Debian packages for a new TFEL/MFront release
author: Thomas Helfer
date: 19/11/2026
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

The latest version of this document is always available at
<https://thelfer.github.io/tfel/web/packaging-debian.html>.

This document describes how to create official quality `Debian`
packages of `TFEL`/`MFront` for a new release, either to publish a
`Ubuntu` PPA, to attach binary packages to a `GitHub` release, or to
prepare a Debian upload (NEW queue). It is a complement to the general
[packaging document](packages.html) which deals with the binary
packages generated with `CPack`.

The packaging metadata is maintained in the
[`packages/debian`](https://github.com/thelfer/tfel/tree/master/packages/debian)
directory of the sources (see [issue
\#1466](https://github.com/thelfer/tfel/issues/1466) for the overall
strategy). This directory is a plain `debian/` directory, so that
`TFEL` can be packaged with the standard `debhelper` tool chain.

# Building the packages

## Install the build tools

On a `Debian` or `Ubuntu` system:

~~~~ {#tools .bash}
$ sudo apt-get update
$ sudo apt-get install -y --no-install-recommends \
  build-essential cmake debhelper devscripts dpkg-dev \
  fakeroot gfortran libpython3-dev python3-dev
~~~~~~~~~~~~~~~~~~~~~~

`dpkg-checkbuilddeps`, run from the directory of the sources, checks
the installed packages against the `Build-Depends` of
`packages/debian/control`: the `Debian` workflow uses it to make sure
that this list stays complete.

## Binary-only build (test a release)

This is the way to check that the packaging is still valid for a new
release. `TFEL` is packaged from the sources with the following
commands:

~~~~ {#binary_build .bash}
$ git checkout TFEL-x.y.z
$ ln -sfn packages/debian debian
$ DEB_BUILD_OPTIONS=nocheck dpkg-buildpackage -us -uc -b
~~~~~~~~~~~~~~~~~~~~~~

The unit tests are skipped (`DEB_BUILD_OPTIONS=nocheck`) since they
are run by the `GitHub Actions` workflow when the release is tested;
remove this variable to run them during the package build as well
(`tests` are built by default).

The `.deb` files are written in the parent directory of the sources.
The following binary packages are generated:

| Package | Contents |
| :-----: | :------- |
| `libtfel-x.y.z` | `TFEL` and `MFront` shared libraries |
| `libtfel-dev` | headers, CMake configuration files, `tfel-config` |
| `mfront` | the `MFront` code generator and companion tools |
| `mtest` | the `MTest` solver and its markdown documentation |
| `tfel-examples` | gallery of MFront source files |
| `tfel-check` | validation tool and test suite |

Automatic debug symbol packages (`*-dbgsym`) are also produced.

A typical failure after a release is a `dh_missing` or `dh_install`
error complaining about files which are not installed to anywhere or
missing matches: this means that new files appeared (or disappeared)
in the install tree. Update the corresponding
`packages/debian/<package>.install` files (see the `dh_missing`
output, which lists the candidates) and, if a new binary is introduced
in the release, decide which package it belongs to in
`packages/debian/control`.

## Analysing the packages with `lintian`

~~~~ {#lintian .bash}
$ sudo apt-get install -y lintian
$ lintian --no-tag-display-limit ../*.changes
~~~~~~~~~~~~~~~~~~~~~~

The `GitHub Actions` `Debian` workflow runs the same commands on every
push to `master`. The `tfel-python` and `tfel-doc` packages, which are
still to be introduced, are discussed in [issue
\#1466](https://github.com/thelfer/tfel/issues/1466), as well as the
absence of man pages.

# Preparing a release for a Debian upload

A `.changes`/`.dsc` source package cannot be built from a symlinked
`debian/` directory: the packaging metadata must be the top-level
`debian/` directory of a source tree whose `orig` tarball is the
release archive. A convenient workflow uses `git-buildpackage`
(`sudo apt-get install git-buildpackage pristine-tar`):

~~~~ {#source_package .bash}
$ git clone https://github.com/thelfer/tfel.git
$ cd tfel
$ git checkout -b debian/x.y.z TFEL-x.y.z
$ git mv packages/debian debian
$ git commit -m "Move packages/debian to debian/ (packaging branch)"
~~~~~~~~~~~~~~~~~~~~~~

The `packages/debian/gbp.conf` file is used by `git-buildpackage`. The
packaging branch must update the packaging metadata for the new
release (next section) before building the source package:

~~~~ {#gbp_build .bash}
$ gbp buildpackage
~~~~~~~~~~~~~~~~~~~~~~

which fetches the upstream tarball from the `TFEL-x.y.z` tag
(`pristine-tar` keeps it reproducible; upstream tags are named
`TFEL-<major>.<minor>.<patch>` and `debian/watch` can check for new
releases with `uscan`).

If a source package must be generated without `git-buildpackage`, run
`dpkg-buildpackage -us -uc` in a tree where the content of
`packages/debian/` is the `debian/` directory and where the release
archive is available as `../tfel_x.y.z.orig.tar.gz`.

# Updating the packaging for a new release

The release process itself is not modified: the packaging metadata
lives in `packages/debian/` on `master` and should be kept in sync as
much as possible so that the release tag is directly packageable.

When the new release does not use version `5.2.0`, which is hard coded
in the name of the library package, the following steps are required:

1. rename the library package `libtfel-5.2.0` to the new SONAME-based
   name in `packages/debian/control`,
   `packages/debian/libtfel-dev.install`,
   `packages/debian/libtfel-5.2.0.install` (rename the file too) and
   all `Depends` references, e.g. with:

~~~~ {#rename .bash}
$ cd packages/debian
$ old=5.2.0
$ new=x.y.z
$ git mv libtfel-${old}.install libtfel-${new}.install
$ sed -i "s/libtfel-${old}/libtfel-${new}/g" \
  control libtfel-${new}.install libtfel-dev.install README.md
~~~~~~~~~~~~~~~~~~~~~~

1. update the new entry of `debian/changelog` with the release number:

~~~~ {#dch .bash}
$ dch -v x.y.z-1 "New upstream release TFEL x.y.z."
~~~~~~~~~~~~~~~~~~~~~~

1. check the packaging metadata for the new release:
   - `debian/copyright`: copyright years, new third party code,
   - `debian/control`: new build dependencies if the release requires
     a new optional dependency (remember that `TFEL` only depends on
     `pybind11`, when the `python` bindings are enabled, which is not
     the case in the current packaging),
   - `debian/install` files, as explained above, using the `dh_missing`
     diagnostics,
   - `debian/README.md` (binary package table).

1. build the binary packages, analyse them with `lintian` and run a
   smoke test (install the `.deb` files in a container, run
   `mfront --version`, `mtest` on a gallery example, etc.).

# Publishing

At the moment, three publication channels are planned, in increasing
order of effort:

1. attach the generated `.deb` files to the `GitHub` release: this is
   automated by the `Debian packages release` workflow
   (`.github/workflows/debian-release.yml`), which:
   - publishes the packages of every published `GitHub` release (tag
     `TFEL-x.y.z`), attaching the `.deb` files and the `.changes` file
     to the release,
   - every week, builds `master` and all `rliv-x.y` maintenance
     branches which carry the packaging metadata
     (`packages/debian/changelog`), publishing them to rolling
     prereleases named `deb-rolling-<branch>`; the packages are marked
     with a date stamped version (`<version>~dev.<YYYYMMDD>-1`),
   - can be run manually for any branch or tag,
   
   so that the latest packages of every maintained version (`master`,
   `rliv-5.1`, `rliv-5.0`, ...) are always available for download and
   installation with `sudo apt install ./<package>.deb`. Only branches
   to which the packaging has been backported are built;
1. publish the source package to a `Ubuntu` PPA with `dput`: this
   provides `apt` users with a repository kept in sync with the
   releases;
1. upload to `Debian` itself: open a Request For Packaging bug, ask
   for sponsorship by the `debian-science` team
   (<https://alioth-lists.debian.net/>), and follow the `MENTORS`
   procedure.

# Limitations and follow-up work

The following points are tracked in [issue
\#1466](https://github.com/thelfer/tfel/issues/1466):

- `tfel-python`: the `python` bindings are disabled in
  `debian/rules` because `TFEL` installs them in
  `lib/python3.X/site-packages` whereas `Debian` policy requires
  `dist-packages` (the `TFEL_PYTHON_SITE_PACKAGES_DIR` CMake variable
  is already honoured by the build system). Enabling them will also
  mean dropping `-Denable-numpy-support=OFF`, which is only needed
  because `-Denable-python=ON` makes the `NumPy` component of
  `FindPython` mandatory whereas `NumPy` is used by the bindings only,
  and adding `python3-numpy` to the build dependencies;
- `tfel-doc`: the reference documentation and the website require the
  `pandoc` and `texlive` tool chains and are disabled in
  `debian/rules`;
- `GNUInstallDirs`: `TFEL` installs the libraries in
  `lib${LIB_SUFFIX}`; the multiarch path is currently injected with
  `-DLIB_SUFFIX=/$(DEB_HOST_MULTIARCH)` in `debian/rules` until
  upstream honours `CMAKE_INSTALL_LIBDIR`;
- `SOVERSION`: the shared libraries use the full version as SONAME,
  which forces the renaming of the `libtfel-x.y.z` package at each
  release (see above); a major-only SONAME policy should be discussed;
- `Multi-Arch`: `libtfel-dev` does not declare `Multi-Arch: same`
  because it installs the `tfel-config` binary program in `/usr/bin`
  and `CMake` configuration files with architecture dependent paths in
  `/usr/share`; both paths should become architecture specific
  (`CMAKE_INSTALL_BINDIR` and `CMAKE_INSTALL_LIBDIR`) upstream;
- man pages for `mfront`, `mtest`, etc. are missing (expected
  `lintian` tags).

<!-- Local IspellDict: english -->
