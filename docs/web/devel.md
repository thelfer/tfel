% Developing in `TFEL`
% Helfer Thomas
% October 20, 2014

# Organisation of the `svn` repository

Access to the `svn` repository is described [here](svn.html).

The root of the `svn` repository is split in three directories:

- `trunk` is the current version of `TFEL`. This branch is meant to be
  **stable**: one shall be able to create a new version of `TFEL` out of
  `trunk` at any time. New or experimental features shall be added in
  dedicated subdirectories of the `branches` directory described below.
  Note that many engineers at `CEA` work with the `trunk` in their daily
  work, so committing to `trunk` requires special care. 
- `tags`, used to fix a new official version of `TFEL`. Once created,
  subdirectories in the `tags` repository can not be modified.
- `branches` contains forks of `trunk` where new or experimental
  features are developed. Once those developments finished and
  backported to `trunk`, the associated branches shall be erased.

# Build systems and testing

`TFEL` supports two build systems (see the
[installation page](install.html) for details). Although the `cmake`
is the preferred build system, the support for the `autotools` is
still mandatory.

New developments shall compile with both build systems. However, new
tests can only be added in the `cmake` build system but required
files shall be distributed in source package created with the
`autotools`. This is the case for mechanical behaviours tests.

To check that `TFEL` compiles with both build systems, we provide the
`check_all` script at the root of the source directory. This script
can be invoked as follows:

~~~~ {#check_all .bash}
$ $SRCDIR/check_all [-j $NBPROCS]
~~~~

where `SRCDIR` is the path to the source directory. The `-j` option
allows the user to specify the number of jobs that the `make` command
is allowed to run simultaneously.

**Committing to `trunk` is only allowed if the `check_all` script runs
  successfully**

<!-- Local IspellDict: english -->
