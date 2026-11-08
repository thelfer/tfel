---
title: Automation of unit-tests of scientific tools using `tfel-check`
author: Thomas Helfer, Stéphane Bernaud
date: 21/11/2017
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

`tfel-check` is a tool designed to launch unit-tests and provide tools
to test the outputs. `tfel-check` mostly targets scientific applications
which do not have in-house ability to compare their results to
reference results.

# Overview

## Basic usage

The basic usage of `tfel-check` is simply to call `tfel-check` without
options:

~~~~{.bash}
$ tfel-check
~~~~

`tfel-check` will then execute the tests and comparisons described in
every file with the extension `.check` in the current directory and,
recursively, in all the subdirectories. The syntax of of those files are
described in Section @sec:tfel_check:input_files.

One may also specify only a set of files to be considered, as follows:

~~~~{.bash}
$ tfel-check test1.check test2.check
~~~~

One may also specify a configuration file using the `--config` command
line option, as follows:

~~~~{.bash}
$ tfel-check --config=test.config
~~~~

The configuration files are described in Section
@sec:tfel_check:configuration_files.

Substitution strings can be specified in configuration files or in the
command line, as follows:

~~~~{.bash}
$ tfel-check --@python@=python3.5
~~~~

With this option, every occurrence of `@python@` will be replaced by
`python3.5`.

## A first example

Let us consider this simple test file:

~~~~{.cxx}
@Requires {"tfel::madnex","mfront::python"};
@Command "@python@ OverridableImplementation.py";
@Command "mfront-query --author --material=A316LN --behaviour=Plasticity_SRMA2022 Plasticity.madnex"{
  expected_output : "John Mac Enroe"
};
~~~~

The first line, which begins with the `@Requires` keywords, provides a
list of required components. In this case, the components `tfel::madnex`
and `mfront::python` are required to execute the tests described in this
file. The tests are skipped if those requirements are not met. The
definition of components can be done in a configuration file, as
detailed in Section @sec:tfel_check:configuration_files. The components
defined by defaults by `tfel-check` is described in Section
@sec:tfel_check:default_components.

The second line specifies the first command to be run and begins with the
`@Command` keyword. It is followed by the command to be launched. The
`@python@` syntax designates that a substitution string must be provided
to precise the `python` interpreter to be used.

The third line specifies a second command which launches `mfront-query`
and a first test which compares the command line output to an expected
one.

This file can can be launched as follows:

~~~~{.bash}
$ tfel-check --@python@=python3.5 madnex.check
entering directory '/tmp/madnex'
* beginning of test './madnex.check'
** Exec-1 python3.5 OverridableImplementation.py                       [SUCCESS]
** Exec-2 mfront-query --author --material=A316LN --behaviour=Plast... [SUCCESS]
* end of test './madnex.check'                                         [SUCCESS]
======
~~~~

This command produces several files:

- `tfel-check.log`: this file contains the output of `tfel-check`. This
  file is created in the directory where `tfel-check` has been invoked.
- `madnex.checklog`: this file contains a summary of the execution of
  the commands and test described in the `madnex.check` file. A similar
  file is created for each file considered by `tfel-check`.
- Two files `madnex-Exec-1.out` and `madnex-Exec-2.out` associated
  to the two commands listed in the
  `madnex.check` file. Those files contain the output of each command
  and the execution time of the command.
- `TEST-madnex.xml` is an `XML` file conforming the `JUnit` standard
  that summarises the results and which is suitable for the integration
  of the test case in the [`jenkins` automation
  server](https://www.jenkins.io/).

If the requirements are not met, the execution and the tests are
marked as skipped:

~~~~{.bash}
$ tfel-check
entering directory '/tmp/madnex'
* beginning of test './madnex.check'
** Exec-1 @python@ OverridableImplementation.py                        [SKIPPED]
** Exec-2 mfront-query --author --material=A316LN --behaviour=Plast... [SKIPPED]
* end of test './madnex.check'                                         [SUCCESS]
======
~~~~

If the execution of a command fails, this is reported as follows: 

~~~~{.bash}
$ tfel-check --@python@=python2.7
entering directory '/tmp/madnex'
* beginning of test './madnex.check'
** Exec-1 toto OverridableImplementation.py                            [ FAILED]
** Exec-2 mfront-query --author --material=A316LN --behaviour=Plast... [ FAILED]
* end of test './madnex.check'                                         [ FAILED]
======
~~~~

## A more complex example

For this second example, let us consider a tensile test simulated using
the `Cast3M` finite element solver. The test case uses an `MFront` file
to define the behaviour and generates a text file called
`ImplicitSimoMieheElastoPlasticityUniaxialTesting-castem.res` containing
the evolution of the stress along the tensile axis as a function of
time.

The `ImplicitSimoMieheElastoPlasticity.check` file has the following
content:

~~~~{.cxx}
@Requires {"Cast3M", "mfront::behaviour::castem_interface"};
@Command "mfront --obuild --interface=castem ImplicitSimoMieheElastoPlasticity.mfront";
@Command "@castem@ ImplicitSimoMieheElastoPlasticityUniaxialTesting.dgibi";

@Precision 100; // criterion for the check, here a difference of 100Pa is allowed
@Test "ImplicitSimoMieheElastoPlasticityUniaxialTesting-castem.ref"
      "ImplicitSimoMieheElastoPlasticityUniaxialTesting-castem.res" 2;
~~~~

The first line specifies that the `Cast3M` component must be available
and that the support of the `Cast3M` interface is available in `MFront`.
The second line compiles the behaviour.

The third one launches the `Cast3M` simulation. This line assumes that a
substitution string for the `@castem@` pattern has been defined.

The fifth line specifies an absolute criterion to compare the current
results to the reference ones. The last line defines the test to be
performed: the values of the second column of the reference and result
files are compared using an absolute criteria.

To trigger those tests, the following config file, called
`castem.config` can be used:

~~~~{.cxx}
components : {"Cast3M"};
substitutions : {"castem": "castem2019_PLEIADES"};
~~~~

The tests are run as follows:

~~~~{.bash}
$ tfel-check --config=castem.config
entering directory '/tmp/tfel-check'
* beginning of test './ImplicitSimoMieheElastoPlasticity.check'
** Exec-1 mfront --obuild --interface=castem ImplicitSimoMieheElast... [SUCCESS]
** Exec-2 castem2019_PLEIADES ImplicitSimoMieheElastoPlasticityUnia... [SUCCESS]
** Compare-1 'ImplicitSimoMieheElastoPlasticityUniaxialTesting-cast... [SUCCESS]
* end of test './ImplicitSimoMieheElastoPlasticity.check'              [SUCCESS]
~~~~

## Commands failure

By default, `tfel-check` discards a command failure if at least one test
is defined. The rationale behind this choice is that some command may
succeed in producing the expected results but may still fail to exit
properly.

This behavior can be controlled by the `--discard-commands-failure`
which takes `true` or `false` as argument.

### Example of usage

~~~~
$ tfel-check --discard-commands-failure=false
entering directory '/tmp/tests'
* beginning of test './test.check'
** Exec-1 ./main                                                       [ FAILED]
** Compare-1 'results.res' and 'results.res', column '1'               [SUCCESS]
* end of test './test.check'                                           [ FAILED]
======
$ tfel-check --discard-commands-failure=true
entering directory '/tmp/tests'
* beginning of test './test.check'
** Exec-1 ./main                                                       [ FAILED]
** Compare-1 'results.res' and 'results.res', column '1'               [SUCCESS]
* end of test './test.check'                                           [SUCCESS]
====== 
~~~~

# Description of the input files{#sec:tfel_check:input_files}

## List of available keywords

### The `@Requires` keyword

The `@Requires` keyword specifies the components that are required to
run the tests described in the considered input file.

### The `@Environment` keyword

The `@Environment` keyword specifies a set of environment variables that
will be defined accessible to the commands.

If a variable is already defined, in the environment of `tfel-check`
or earlier in the test file, the value is prepended to it, separated by
`:`, as for search paths (`PATH`, `LD_LIBRARY_PATH`, etc.).

### The `@Command` keyword

The `@Command` keyword specifies a command to be launched.

This keyword has the following options:

- `expected_output`: a string (single line output) or an array of
  strings (multiline output) specifying the expected output. If the
  actual output differs from the expected one, the tests is marked as
  failed.
- `expected_numerical_output`, which allows to test the numerical output
  of a command. This option must be defined as a map with two entries:
  - `value`: the expected value
  - `criterion_value`: the criterion value used to test the output
- `output_validation_regex`, which allows to specify a regular
  expression which shall validate the output of the command. The output
  of the command is concatenated in a single string for the test.
- `shall_fail`: a boolean stating if the command shall succeed or fail.

### The `@Precision` keyword

The `@Precision` keyword specifies the comparison criterion used to
compare the current values and the reference ones.

~~~~{.cxx}
@Precision 1.e-6;
~~~~

### The `@Test` keyword

The `@Test` keyword compares two files (reference and current results).

Syntax:

~~~~{.cxx}
@Test <reference_file> <results_file> <columns>;
~~~~

- `reference_file`: Path to the file containing reference values.
- `results_file`: Path to the file containing current results.
- `columns`: List of column numbers or names to compare. At least one
  column is required. Each column defines a separate test.

The order of the files matters:

- with an interpolation (see the `@Interpolation` keyword), the values
  of the second file are interpolated at the abscissas of the first one,
- the `AllowLessResults` option allows the second file to have fewer
  values than the first one,
- the `Area` comparison is normalized by the values of the first file,
- the relative tolerance of the `Mixed` comparison is scaled by the
  values of the second file.

Columns can be designated by their names only if the first line of the
file gives the names of the columns. This line must not start with `#`:
a line starting with `#` is a comment. A column can also be given by a
formula of column numbers, such as `'$2*1.e-6'`.

Examples:

~~~~{.cxx}
@Test 'reference.res' 'results.res' 2;        // compare column 2
@Test 'reference.res' 'results.res' 'B';     // compare column named 'B'
@Test 'reference.res' 'results.res' 1 3 5;   // compare columns 1, 3, and 5
@Test 'reference.res' 'results.res' 'A' 'B'; // compare columns named 'A' and 'B'
~~~~

Different columns can be compared with the following syntax:

~~~~{.cxx}
// compare column 2 of 'reference.res' with column 3 of 'results.res'
@Test {'reference.res' : 2} {'results.res' : 3};
~~~~

### The `@TestType` keyword

The `@TestType` keyword defines the comparison method for values.
Available types:

- `Absolute`: Absolute difference between values must be ≤ precision.
- `Relative`: Relative difference between values must be ≤ precision.
- `RelativeAndAbsolute`: Combines relative and absolute criteria. Requires two precision values.
- `Mixed`: Mixed relative/absolute comparison. Requires two precision values.
- `Area`: Integral of the absolute difference between the two curves,
  normalized by the maximum absolute value of the reference. Requires a
  single precision value. See Section @sec:tfel_check:area_comparison.

For all types, a NaN value makes the test fail.

Example:

~~~~{.cxx}
@TestType Absolute;
@Precision 1.e-6;
@Test 'reference.res' 'results.res' 1;

@TestType RelativeAndAbsolute;
@Precision 1.e-3 1.e-6; // relative and absolute tolerances
@Test 'reference.res' 'results.res' 2;
~~~~

### The `@Interpolation` keyword

The `@Interpolation` keyword specifies how the values of the results are
interpolated at the abscissas of the reference. This is useful when the
reference and result files have different abscissa values (e.g., time
steps).

Available interpolation methods:

- `None`: No interpolation. The values are compared line by line: both
  files must have the same number of values. If a column is given with
  `using`, the abscissas of both files must be the same. Otherwise, they
  are not checked.
- `Linear`: Linear interpolation between reference points.
- `Spline`: Cubic spline interpolation.
- `LocalSpline`: Local spline interpolation (avoids global smoothness constraints).

The `using` clause specifies the column to use as the interpolation abscissa.

If the results have at least as many distinct abscissas as the
reference, they are interpolated at the abscissas of the reference.
Otherwise, the `AllowLessResults` option is required, and the values of
the reference are interpolated at the abscissas of the results. In both
cases, the results must reach the bounds of the abscissas of the
reference: results stopping early make the test fail.

No extrapolation is made: the abscissas of the interpolated file must
cover the abscissas at which it is evaluated, otherwise the test fails.
Both files must have values. The abscissas are sorted and, if an
abscissa appears more than once in a file, only its first point is
used.

Examples:

~~~~{.cxx}
@Interpolation Linear using 'time';
@Interpolation Spline using 'tps' AllowLessResults;
@Interpolation LocalSpline using 'x';
~~~~

#### Interpolation for integration

For `Area` comparisons, the interpolation and the abscissa column used
for the integration are given by the `@TestType` keyword:

~~~~{.cxx}
@TestType Area interpolation <None|Linear|Spline|LocalSpline> using <column>;
~~~~

For example:

~~~~{.cxx}
@TestType Area interpolation Linear using 'time';
@Precision 1.e-2;
@Test 'reference.res' 'results.res' 'y';
~~~~

This interpolation is independent of the one given by the
`@Interpolation` keyword. See Section @sec:tfel_check:area_comparison.

# Comparing values between files

`tfel-check` provides powerful tools to compare numerical results against reference data.
This section details the available comparison types.

## Comparison types {#sec:tfel_check:comparison_types}

The `@TestType` keyword defines how values are compared. Each type has specific use cases:

### Absolute comparison

Checks if the absolute difference between corresponding values is within the specified precision.

~~~~{.cxx}
@TestType Absolute;
@Precision 1.e-6;
@Test 'reference.res' 'results.res' 1;
~~~~

**Use case**: Suitable for quantities with a known absolute tolerance (e.g., stress in Pa).

### Relative comparison

Checks if the relative difference between corresponding values is within the specified precision.

~~~~{.cxx}
@TestType Relative;
@Precision 1.e-3; // 0.1% tolerance
@Test 'reference.res' 'results.res' 1;
~~~~

**Use case**: Ideal for quantities where relative accuracy matters (e.g., normalized values).

### Relative and Absolute comparison

Combines both criteria: the test passes if **either** the relative **or** the absolute difference is within tolerance.

~~~~{.cxx}
@TestType RelativeAndAbsolute;
@Precision 1.e-3 1.e-6; // relative and absolute tolerances
@Test 'reference.res' 'results.res' 1;
~~~~

**Use case**: Flexible comparison for quantities that may have small absolute values (where relative comparison fails) or large values (where absolute comparison is too strict).

### Mixed comparison

Combines both tolerances: the test passes if
\(\left|a-b\right| \leq p_{r}\,\left|b\right| + p_{a}\), where \(a\)
and \(b\) are the values of the first and second files, and \(p_{r}\)
and \(p_{a}\) the relative and absolute precisions.

~~~~{.cxx}
@TestType Mixed;
@Precision 1.e-3 1.e-6; // relative and absolute tolerances
@Test 'reference.res' 'results.res' 1;
~~~~

**Use case**: Tolerance with a relative part and an absolute part, the
latter being useful for values close to zero.

### Area comparison {#sec:tfel_check:area_comparison}

Computes the area between the two curves, i.e. the integral of the
absolute value of their difference, normalized by the maximum absolute
value of the reference:

\[
e = \frac{1}{\max_{i}\left|y^{\mathrm{ref}}_{i}\right|}\int\left|y^{\mathrm{ref}}\left(x\right)-y^{\mathrm{res}}\left(x\right)\right|\,\mathrm{d}x
\]

where \(x\) is the abscissa column given after the `using` keyword.
The test succeeds if \(e\) is lower than the precision. This error is
a normalized \(L^{1}\) norm of the difference.

The integral is computed by the trapezoidal rule on the union of the
abscissas of both files. Each curve is evaluated:

- at its own abscissas, with its own values,
- between them, with the chosen interpolation,
- outside its range, with its first or last value.

Abscissas are sorted. If an abscissa appears more than once in a file,
only its first point is used. At least two distinct abscissas are
required. With the `None` interpolation, both files must have the same
abscissas.

If all the values of the reference are null, the area is not normalized.

The error \(e\) has the unit of the abscissa: it is the product of the
length of the range of the abscissas and the mean deviation relative to
the maximum of the reference. For example, for a test lasting 100 s, a
precision of 1 corresponds to a mean deviation of 1 % of the maximum of
the reference.

~~~~{.cxx}
@TestType Area interpolation Linear using 'time';
@Precision 1; // time range of 100 s: mean deviation of 1 %
@Test 'reference.res' 'results.res' 'stress';
~~~~

**Use case**: Global comparison of two curves, possibly sampled
differently, when local differences (for instance a slight time shift
of a sharp transition) are acceptable.

# Description of the configuration files{#sec:tfel_check:configuration_files}

Configuration files can be used to define:

- substitution strings
- available components

The syntax of configuration file is loosely inspired by the `JSON`
format and looks like:

~~~~{.cxx}
components : {"pleiades::python"};
substitutions : {"python" : "python3.5"};
~~~~

# Default components {#sec:tfel_check:default_components}

Some components are conditionally defined by `tfel-check`.

- If the `python` bindings of the `TFEL` project have been enabled, the
  following components are declared: `tfel::python` and
  `mfront::python`.
- If the `madnex` support has been enabled, the component `tfel::madnex`
  is declared.

For each `MFront`' interface for material properties, a component called
`mfront::material_property::<name>_interface` is defined, where `<name>`
shall be replaced by the name of the interface.

For each behaviour interface, a component named
`mfront::behaviour::<name>_interface` is declared, where `<name>` is the
name of the interface.

For each model interface, a component named
`mfront::model::<name>_interface` is declared, where `<name>` is the
name of the interface.

Those default components can be listed using the
`--list-default-components` command line options, as follows:

~~~~{.cxx}
$ tfel-check --list-default-components
- tfel::python
- mfront::python
- tfel::madnex
- mfront::material_property::C++_interface
- mfront::material_property::Cpp_interface
- mfront::material_property::Cxx_interface
- mfront::material_property::Python_interface
- mfront::material_property::c_interface
- mfront::material_property::c++_interface
- mfront::material_property::cpp_interface
- mfront::material_property::cpptest_interface
- mfront::material_property::cxx_interface
- mfront::material_property::excel_interface
- mfront::material_property::excel-internal_interface
- mfront::material_property::gnuplot_interface
- mfront::material_property::mfront_interface
- mfront::material_property::octave_interface
- mfront::material_property::python_interface
- mfront::behaviour::generic_interface
- mfront::model::mfront_interface
~~~~

# Automatic declaration of substitutions for `TFEL` executables and `python` interpreter

The following substitutions are automatically declared: `@mfront@`,
`@mfront-query@`, `@mtest@`, `@mfront-doc@`, `@mfm-test-generator@`.

In `python` bindings are enabled, the `@python` substitution is also
automatically declared.

Those substitutions are declared after reading the configuration files
and after parsing the command line arguments, so those default
substitutions can be overridden by the user.