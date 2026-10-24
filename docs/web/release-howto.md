---
title: Release howto
author: Thomas Helfer
date: 24/10/2026
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

This page gathers the tasks to be performed when preparing a release.

## Running the sanitized tests

Before a release, the tests shall be run with the sanitizers and the
standard library hardening, using the `Run sanitizers` continuous
integration workflow, described below. The sanitizer options themselves are
described in [Running sanitized tests](sanitizers.html).

### The `Run sanitizers` workflow

The workflow is defined in `.github/workflows/run-sanitizers.yml`. It is a
manual workflow: it is never triggered automatically by a `push` or a pull
request, and must be started explicitly, either from the web interface or
using the `GitHub` CLI. It runs the standard sequence of the continuous
integration (`build`, `build-tests`, `ctest`, `install`, `tfel-check`) with
the sanitizers and the standard library hardening, using the inputs
described below:

| Input | Description | Default |
|-------|-------------|---------|
| `build_type` | `CMake` build type: `Debug`, `RelWithDebInfo` or `Release` | `Debug` |
| `compiler` | `C++` compiler: `gcc` or `clang` | `gcc` |
| `sanitize_options` | semicolon separated list of the sanitizers to enable | `address` |
| `glibcxx_debug` | enable the debug mode of the standard library of the GNU `C++` library | `true` |

Running the workflow requires write access to the repository. Note that a
manual workflow is only proposed for the branches or tags on which the
workflow file exists: the file must be present on the selected `ref`.

### Running the workflow from the web interface

- Open the `Actions` tab of the repository.
- Select `Run sanitizers` in the list of workflows, on the left.
- Click the `Run workflow` button on the right.
- Select the branch or the tag to test in the `Use workflow from` dropdown
  menu, fill the inputs and click `Run workflow`.

The progress of the run is displayed in the `Actions` tab and the failures
are reported as usual in the annotations and in the logs of the failed job.

### Running the workflow from the command line

The same workflow can be started with the `GitHub` CLI
([`gh`](https://cli.github.com/)), which is convenient to script the checks
of a release:

~~~~ {#release-gh-run .bash}
$ gh workflow run run-sanitizers.yml --ref <branch or tag> \
    -f build_type=Debug -f compiler=clang \
    -f 'sanitize_options=address;undefined' -f glibcxx_debug=true
~~~~

The value of `sanitize_options` contains a semicolon, which is the command
separator of most shells: it shall be quoted.

The runs can be monitored and inspected with:

~~~~ {#release-gh-watch .bash}
$ gh run list --workflow run-sanitizers.yml        # list the runs
$ gh run watch <run identifier>                    # follow the progress
$ gh run view --log-failed <run identifier>        # logs of the failed steps
~~~~

### Recommended configurations

The following two configurations have been validated on a full build with
all the tests, including the installation and `tfel-check`:

- `clang`, which supports the `undefined` sanitizer without restriction:

  ~~~~ {#release-clang .bash}
  $ gh workflow run run-sanitizers.yml --ref <branch or tag> \
      -f build_type=Debug -f compiler=clang \
      -f 'sanitize_options=address;undefined' -f glibcxx_debug=true
  ~~~~

- `gcc`, for which the `vptr` check, which is part of the `undefined`
  sanitizer, shall be excluded as it leads to pathologically long
  compilation times for some files of `MFront` (see the release notes): the
  checks of the undefined behaviour sanitizer are listed individually
  instead:

  ~~~~ {#release-gcc .bash}
  $ gh workflow run run-sanitizers.yml --ref <branch or tag> \
      -f build_type=Debug -f compiler=gcc \
      -f 'sanitize_options=address;alignment;bool;bounds;bounds-strict;builtin;enum;float-cast-overflow;float-divide-by-zero;null;object-size;return;shift;shift-base;shift-exponent;signed-integer-overflow' \
      -f glibcxx_debug=true
  ~~~~

Each sanitizer report (address error, leak, undefined behaviour, container
out of bounds, etc.) makes the tested program exit with a non zero status
and thus makes the corresponding test fail: a green run means that no such
report was emitted.
