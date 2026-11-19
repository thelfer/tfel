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

| Package        | Contents                                              |
| -------------- | ----------------------------------------------------- |
| `libtfel-5.2.0` | TFEL/MFront shared libraries (SONAME = full version) |
| `libtfel-dev`  | headers, CMake configuration files, `tfel-config`     |
| `mfront`       | `mfront`, `mfront-query`, `mfront-doc`, `tfel-doc`, `mfm` |
| `mtest`        | the `MTest` solver and its documentation              |
| `tfel-examples` | gallery of MFront source files (behaviours, etc.)    |
| `tfel-check`   | validation tool and test suite                        |

Planned follow-ups (see the issue for details):

- `tfel-python`: Python bindings. They are disabled in
  `debian/rules` for the first iteration because `TFEL` installs them
  in `lib/python3.X/site-packages` while Debian policy requires
  `dist-packages`;
- `tfel-doc`: reference documentation (requires the `pandoc` and
  `texlive` tool chains);
- manual pages for `mfront`, `mtest`, etc. (currently missing).

Note that `mfront`/`mtest` compile material laws by invoking the
system C++ compiler: users need `g++` and `libtfel-dev` installed to
actually use them (`--obuild`, MTest on-the-fly compilation).

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

# Continuous integration

A `.deb` build must not regress: a GitHub Actions job runs
`dpkg-buildpackage -us -uc -b` (and ideally `lintian`) on each push.
See the workflow `.github/workflows/debian.yml`.
