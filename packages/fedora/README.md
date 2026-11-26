# Fedora packaging for TFEL/MFront

This directory contains the packaging files required to build
**official Fedora packages** (RPMs) of TFEL/MFront. See issue
[#1468](https://github.com/thelfer/tfel/issues/1468) for the overall
strategy and [#1466](https://github.com/thelfer/tfel/issues/1466) for
the parallel Debian effort (the two share configuration options and
upstream install-layout fixes).

## Content

| file                | role                                                    |
| ------------------- | ------------------------------------------------------- |
| `tfel.spec`         | RPM spec file (single spec, multiple binary packages)   |
| `sources`           | md5 + archive name, the format used by Fedora dist-git and `rpkg` |
| `update-release.sh` | downloads a new release, bumps the spec and rewrites `sources` |
| `.gitignore`        | local `rpmbuild --define '_topdir $PWD'` artefacts      |

The packages are built automatically by the
[`.github/workflows/fedora-release.yml`](../../.github/workflows/fedora-release.yml)
workflow: on every GitHub release (assets attached to the release) and
weekly for `master` and the `rliv-x.y` branches carrying this directory
(assets of rolling prereleases named `rpm-rolling-<branch>`).

## Quick local build

The archive of the packaged release must be in `SOURCES/`; the
`update-release.sh` script downloads it (and refreshes the spec and
the `sources` file at the same time):

```sh
./update-release.sh 4.2.5
```

Then build the packages from this directory (requires `rpm-build`; any
distribution with a recent enough `rpmbuild` works for a
syntax/prep check, a Fedora container is recommended for real builds,
see below):

```sh
rpmbuild --define '_topdir '"${PWD}" -ba tfel.spec
```

The binaries are then in `RPMS/`, the sources package in `SRPMS/`.
To check the packaging of a development tree, build the archive
yourself and override the version:

```sh
git archive --prefix=tfel-TFEL-5.2.0/ -o SOURCES/TFEL-5.2.0.tar.gz HEAD
rpmbuild --define '_topdir '"${PWD}" --define 'tfel_version 5.2.0' \
         -ba tfel.spec
```

Reproducible builds against a clean Fedora root are obtained with
`mock`: build the sources package first (it embeds the archive), then
rebuild it in a chroot:

```sh
dnf install mock
rpmbuild --define '_topdir '"${PWD}" -bs tfel.spec
mock -r fedora-rawhide-x86_64 --rebuild SRPMS/*.src.rpm
```

Building inside a container, without installing anything on the host:

```sh
podman run --rm -v "$PWD":/pkg:z -w /pkg fedora:rawhide \
  bash -c 'dnf -y install rpm-build && rpmbuild --define "_topdir /pkg" -ba tfel.spec'
```

## Binary packages

| package      | content                                                |
| ------------ | ------------------------------------------------------ |
| `tfel`       | shared libraries and licences (metapackage pulling `mfront`, recommending `mtest`) |
| `tfel-devel` | headers, unversioned `.so` links, CMake package files, `tfel-config` |
| `mfront`     | `mfront`, `mfront-query`, `mfront-doc`, `mfm`, `mfm-test-generator`, gallery |
| `mtest`      | the `mtest` solver and its documentation |
| `tfel-check` | the `tfel-check` validation suite and test files |

Not built yet (second iteration, see #1468):

- `python3-tfel`: blocked on the install path of the bindings
  (`site-packages` vs. `python3_sitelib`, shared with #1466: upstream
  should honour `CMAKE_INSTALL_PYTHONDIR`);
- `tfel-doc`: requires the pandoc/LaTeX tool chain
  (`-Denable-website=ON`, `-Denable-reference-doc=ON`);
- interfaces to external solvers (`aster`, `abaqus`, `cyrano`, ...)
  stay disabled: they require non-free or non-packaged prerequisites.

## Build configuration notes

- `-Denable-portable-build=ON`: Fedora forbids `-march=native` for
  distributed binaries;
- `-DTFEL_APPEND_VERSION=OFF`: unversioned install tree
  (`/usr/include/TFEL`, `/usr/share/tfel`) and clean SONAMEs;
- `-DLIB_SUFFIX=64`: TFEL installs libraries in
  `lib${LIB_SUFFIX}` instead of honouring `GNUInstallDirs`; passing
  `64` makes them land in `%{_libdir}` (a fix making upstream honour
  `CMAKE_INSTALL_LIBDIR` is to be proposed, same issue as Debian
  multiarch in #1466);
- `enable-fortran=ON`: the Fortran 77/90 bindings only need
  `gcc-gfortran`, which is in every Fedora buildroot.

## Licensing

The sources are dual licensed: GNU GPL (v2 or later) *with a linking
exception*, or CECILL-A (v2.1, explicitly GPL-compatible). The spec
currently uses

```
License: GPL-2.0-or-later WITH Classpath-exception-2.0 OR CECILL-2.1
```

as a placeholder: the linking exception is functionally close to the
GNU Classpath exception but the exact expression must be confirmed
with [Fedora Legal](https://docs.fedoraproject.org/en-US/packaging-guidelines/LicensingGuidelines/)
during the review (see #1468). `include/FSES` is LGPL-2.1+ (see #1466)
and must be documented separately in a review submission.

## Status / trial results

First validation (TFEL 4.2.5 sources, `rpmbuild` 4.17, x86_64; the
Fedora-specific cmake macros were stubbed since the trial ran on a
non-Fedora host — a full `%cmake`-macro run on Fedora or in `mock` is
still to be done):

- `configure` + `build` + `install` with the exact options used by the
  spec: OK (~20 min, `-j 8`, no dependency beyond gcc/gcc-c++/gfortran/
  python3);
- all five binary packages (`tfel`, `tfel-devel`, `mfront`, `mtest`,
  `tfel-check`) build with correct auto-detected `DT_NEEDED`
  dependencies, correct `%{_libdir}` placement and no unpackaged files;
- findings already applied to the spec: `LIB_SUFFIX` must be passed as
  `64` (`DESTINATION lib${LIB_SUFFIX}` concatenation), no
  `%%{_libdir}/gcc` tree exists (the Fortran 90 bindings only install
  a module file under `%%{_includedir}/TFEL`), the `env` directory is
  not installed when no solver interface is enabled, the tools are
  ELF binaries (`tfel-check` is not a script, so `tfel-check` cannot
  be `noarch`).

### Tests run by `%%check`

`ctest` knows about 6428 tests on 4.2.5 (7953 on the current
development branch), but

- the test executables are `EXCLUDE_FROM_ALL`: running `ctest` without
  building them reports thousands of "Not Run" failures;
- the `generic-*` / `mfront-*` matrices (> 6000 tests) invoke
  `tfel-config` and `mfront` and therefore require an **installed**
  tree; they are what the upstream `check_all.sh` script runs and are
  also covered by the `tfel-check` package after installation.

`%check` therefore builds and runs only the self-contained unit tests
of the libraries (names without a `-`): 222 tests on 4.2.5, 628 on the
development branch — built in ~3 min with `-j 16`, executed in ~5 s,
100 % passing in both trials.

Still to check on real Fedora: `%cmake`/`%ctest`/`%ldconfig_scriptlets`
expansions, `rpmlint`, debuginfo generation, and the `%check` timings
under `mock`. The `fedora-release.yml` workflow runs `rpmbuild` and
`rpmlint` in a `fedora:latest` container on every release and every
week, which covers the first three points as soon as this directory is
merged to `master`.

## Path to an official package

1. iterate here until `rpmbuild` produces clean packages
   (`rpmlint tfel.spec RPMS/*/*.rpm SRPMS/*.src.rpm` silent or with
   justified overrides only);
2. publish a [COPR](https://fedora-copr.github.io/) repository
   (`packit build in-copr` or the GitHub Pages continuous packaging
   workflow) so that users get up-to-date packages;
3. optionally add [Packit](https://packit.dev/) (`packit.yaml`) for
   downstream-first CI on pull requests and dist-git commits;
4. open a [review
   request](https://docs.fedoraproject.org/en-US/mentors/Complete_Reviewer_Manual/)
   in Fedora Infra Issues and get sponsored; the dist-git repository
   `rpms/tfel` then becomes the reference, this directory staying a
   convenience copy synchronised on every release (or the other way
   round, with `packit` pulling from dist-git).

Requirements for step 4 that are not met yet: a stable upstream
release cadence is fine already, but the python bindings, the
`LIB_SUFFIX`/`CMAKE_INSTALL_LIBDIR` fix and the license expression
check remain open (see #1468).
