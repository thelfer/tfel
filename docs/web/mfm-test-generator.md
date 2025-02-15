---
title: mfm-test-generator
author: Thomas Helfer
date: 02/10/2019
lang: en-EN
link-citations: true
colorlinks: true
geometry:
  - margin=2cm
papersize: a4
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

`mfm-test-generator` provides a way to generate input files for various
targets (finite element solvers, fuel performance code, etc.) for
specific test cases.

`mfm-test-generator` is based on two main concepts:

- test cases, which derives from the `AbstractTestCase` class. Those
  class are meant to interpret the input file.
- generators. Generators are related to one test case and one target
  (finite element solvers, fuel performance code, etc.). From an object
  describing a test case, the generator generates the input file for its
  target.

This utility thus proceeds by following these three steps:

1. The input file, written in a JSON-like format, is read.
2. The extracted data is passed to the test case specified in the
  input file. The test case interprets the data in a form ready to be
  used by generators.
3. The generators are called.

# Usage

A standard invocation of `mfm-test-generator` is:

~~~~{.cxx}
$ mfm-test-generator --target=mtest pipe.mfmtg \
                     --@library@="'src/libUmatBehaviour.so'"\
                     --@behaviour@="'umatnorton'"
~~~~

## Command line options

The main command line options are:

- `--target` which allows specifying one target
- `--targets` which allows specifying a list of target, separated by
  commas.
- `--targets` which allows specifying a list of shared libraries
  containing user defined " "test cases or generators, separated by a
  commas.
- `--verbose` which allows specifying the verbosity level. Possible
  choices are: `quiet`, `level0`, `level1`, `level2`, `level3`, `debug`
  and `full`.

## Syntax of the input file

The input file has a `JSON`-like interface.

It associates the name to a value as follows:

~~~~{.cxx}
name: "ssna303"
~~~~

A value can be a string, a floating-point value, an integer, a boolean,
an evolution and a list of times. Values are separated by a comma `,`.

### Times

Times can be specified using:

1. an explicit list as follows:

~~~~{.cxx}
times : {0., 1.}
~~~~

2. an external file as follows
~~~~{.cxx}
times : {
  file: "data.txt",
  values: 1}
~~~~
The `file` field gives the name of the external file. The `values`
field can be:

  - a positive integer specifying in which column the time values are
    read.
  - a string giving a formula allowing to interpret the external
    files. For example, the formula `$1*60` multiplies the first
    column by \(60\).

### Evolutions

Evolutions may be specified using:

1. an explicit map between time and values as follows:
~~~~{.cxx}
outer_pressure: {0: 0, 1:1.e-2}
~~~~
This means that the value of the outer pressure evolution is `0` at
time \(0\) and \(10^{-2}\) at time \(1\).

2. an external file as follows:
~~~~{.cxx}
inner_pressure: {
  file : 'pressure.txt',
  times: 1,
  values: 2}
~~~~
The `file` field specifies the external file name, the `times` field
specifies the column specification for extracting the times (can be a
positive integer or a formula), the `values` field specifies the
column specification for extracting the values of the evolution (can
be a positive integer or a formula).

### Variables expected by standard test case

Test cases shall inherit from the `TestCaseBase` class which expects the
following variables to be defined:

- `name` (type `string`, mandatory): name associated with the test case.
- `times` (type `Times`, mandatory): list of times.
- `author` (type `string`, optional): author of the test case.
- `date` (type `string`, optional): date at which the test case has been
  written.
- `description` (type `string`, optional): description of the test case.

## Extensions

The `MFM_TEST_GENERATOR_ADDITIONAL_LIBRARIES` environment variable
allows specifying a list of shared libraries separated by commas. Those
shared libraries may contain new test cases and/or new generators.

# Description of the "UniaxialTensileTest" test case

This test case describes a simple uniaxial tensile test in small or
finite strain.

## Input file example

~~~~{.cxx}
test_case: "UniaxialTensileTest",
name: "ssna303",
author: "Thomas Helfer",
date: "30/09/2019",
description: "A first test case",
behaviour: {
  library: @library@,
  function : @behaviour@,
  material_properties: { YoungModulus: 150e9,
                         PoissonRatio: 0.3},
  external_state_variables : { Temperature: 293.15 }
},
times : {0., 1.},
imposed_strain : {
  times:{0, 1},
  values: {0,1.e-2}},
~~~~

# The "ClosedPipeTest" test

## Input file example

~~~~{.cxx}
test_case: "ClosedPipeTest",
name: "pipe",
author: "Thomas Helfer",
date: "30/09/2019",
description: "A first test case",
behaviour: {
  library: @library@,
  function : @behaviour@,
  material_properties: { YoungModulus: 150e9,
                         PoissonRatio: 0.3},
  external_state_variables : { Temperature: 293.15 }
},
// 
inner_radius : 4.e-3,
outer_radius : 4.2e-3,
number_of_elements : 5,
//
times : {0., 1.},
inner_pressure: {
 file : 'pressure.txt',
    times: 1,
    values: 2},
outer_pressure: {0: 0, 1:1.e-2}
~~~~