---
title: Debian packaging of TFEL/MFront
author: TFEL maintainers
lang: en-EN
---

# Overview

This directory contains the native Debian packaging metadata for
`TFEL`/`MFront`. The goal (see
[issue #1466](https://github.com/thelfer/tfel/issues/1466)) is to
produce official quality Debian packages, suitable for a PPA and
eventually for the Debian archive (sponsorship by the `debian-science`
team).

These files are a plain `debian/` directory which is kept in the
upstream repository for visibility and continuous testing. The
CPack-based binary packaging described in
[`packages.md`](../../docs/web/packages.md) is unchanged: it targets
ad-hoc binary `.deb` generation, not official Debian packages.

# Binary packages

| Package           | Contents                                              |
| ----------------- | ----------------------------------------------------- |
| `libtfel-5.2.0`   | TFEL/MFront shared libraries (SONAME = full version)  |
| `libtfel-dev`     | TFEL headers, CMake configuration files, `tfel-config` |
| `mfront`          | `mfront`, `mfront-query`, `mfront-doc`, `tfel-doc`, `mfm` |
| `mfront-dev`      | headers and CMake configuration files of the MFront libraries |
| `mtest`           | the `MTest` solver and its documentation              |
| `mtest-dev`       | headers and CMake configuration files of the MTest library |
| `mfront-examples` | gallery of MFront source files (behaviours, etc.)    |
| `tfel-check`      | validation tool and test suite                        |
| `python3-tfel`    | python 3 bindings of TFEL                             |
| `python3-mfront`  | python 3 bindings of MFront                           |
| `python3-mtest`   | python 3 bindings of MTest                            |
| `tfel-mfront-all` | metapackage installing all the above                   |

All the solver interfaces are enabled in `debian/rules` except those
of `ZMAT`, `Comsol` and `LS-DYNA`, which require the installation of
those proprietary solvers to be configured; the generated code still
requires the solver itself, which is not packaged in Debian. The
material property interfaces additionally pull dependencies on
`java` (`default-jdk-headless` at build time, `default-jdk` as a
recommendation), `octave`, `gnuplot` and `gfortran`.

Planned follow-ups (see the issue for details):

- `tfel-doc`: reference documentation (requires the `pandoc` and
  `texlive` tool chains);
- `Multi-Arch: same` for `libtfel-dev`: `tfel-config` is installed in
  `/usr/bin` and the `CMake` configuration files, whose content is
  architecture dependent, in `/usr/share`; both should move to
  architecture specific directories upstream;
- byte compilation of the python modules and `${python3:Depends}`
  (`dh-python`);
- manual pages for `mfront`, `mtest`, etc. (currently missing).

Note that `mfront`/`mtest` compile material laws by invoking the
system C++ compiler: `g++` is a recommended dependency and
`libtfel-dev` a strict one, as the generated code is built on top of
the TFEL header files (`--obuild`, MTest on-the-fly compilation).

# Building

`dpkg-buildpackage` requires the metadata to be in a top-level
`debian/` directory. For a local build, create a symlink:

~~~~ {#building .bash}
$ ln -sfn packages/debian debian
$ dpkg-buildpackage -us -uc -b
~~~~

The generated `.changes` file can then be fed to `dpkg-genchanges`,
`sbuild`, `pbuilder`, etc.

For the definitive Debian packaging workflow (a packaging repository
with `pristine-tar`, sponsored upload, etc.), the content of this
directory is used as the top-level `debian/` directory of a
`gbp import-dscs`-style layout; the `orig` tarball should be produced
from the release tag so that `packages/debian` is not part of it
(`Files-Excluded` in `copyright` takes care of the repackaging case).

The procedure to package a new release (version renames, changelog,
publication to a PPA or to Debian) is described in
[`docs/web/packaging-debian.md`](https://github.com/thelfer/tfel/blob/master/docs/web/packaging-debian.md).

# Continuous integration

A `.deb` build must not regress: the `.github/workflows/debian.yml`
workflow runs `dpkg-buildpackage -us -uc -b` on each push and pull
request, after checking the installed tools against the build
dependencies declared in `control` with `dpkg-checkbuilddeps`. It runs
`lintian` on the result (informational only for now: the missing manual
pages and the planned packages are tracked in the issue) and uploads
the generated packages as artifacts of the run.

The packages are published on `GitHub` (releases and weekly rolling
prereleases per `master`/`rliv-*` branch) by the
`.github/workflows/debian-release.yml` workflow, as described in
[`docs/web/packaging-debian.md`](https://github.com/thelfer/tfel/blob/master/docs/web/packaging-debian.md).
