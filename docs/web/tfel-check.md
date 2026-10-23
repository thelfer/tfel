---
title: Automation of unit-tests of scientific tools using tfel-check
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

`tfel-check` is a tool designed to automate unit tests for scientific applications
and provide utilities to compare their outputs against reference results. It is
particularly useful for applications that lack built-in validation capabilities.

# Overview

## Basic usage

The simplest way to use `tfel-check` is to invoke it without arguments:

~~~~{.bash}
$ tfel-check
~~~~

This executes all `.check` files in the current directory and its subdirectories.
The syntax of `.check` files is described in Section @sec:tfel_check:input_files.

To run specific test files, list them explicitly:

~~~~{.bash}
$ tfel-check test1.check test2.check
~~~~

A configuration file can be specified using the `--config` option:

~~~~{.bash}
$ tfel-check --config=test.config
~~~~

Configuration files are described in Section @sec:tfel_check:configuration_files.

Substitution strings can be defined in configuration files or via the command line:

~~~~{.bash}
$ tfel-check --@python@=python3.5
~~~~

This replaces all occurrences of `@python@` with `python3.5`.

## Parallelization

Commands in different `.check` files can be executed in parallel processes.
Commands within a single `.check` file are always run sequentially.

By default, `tfel-check` uses a single process.

### Controlling parallel execution

The `--jobs` (`-j`) option specifies the number of parallel jobs.
If not provided, `tfel-check` uses the number of available CPU cores.

#### Job limit

If no option is given to `--jobs`, the number of available cores
is used, if the system provide this information.

#### Limit on the number of jobs

By default, this number must be lower than the number of cores
available. This limit can be discarded by
passing `--discard-jobs-limit=true` to `tfel-check`.

### Caution

Ensure that parallel execution is safe for your test suite:

- Jobs must not exhaust available memory.
- Jobs must not concurrently access shared resources (e.g., files, directories).

For example, running two `.check` files that invoke `mfront` to build shared
libraries in the same directory may cause race conditions, as both processes
could write to `src/` or `include/` simultaneously.

Similarly, improper use of `@CleanDirectories` or `@CleanFiles` may delete
data required by other `.check` files.

### Terminal output synchronization

By default, terminal output is **not synchronized**, meaning that individual
steps (command execution, comparisons, etc.) are displayed immediately.

To synchronize output, use `--synchronize-terminal-output=true`.
This buffers each job's output and displays it only after the job completes.

## A first example

Consider the following simple `.check` file:

~~~~{.cxx}
@Requires {"tfel::madnex","mfront::python"};
@Command "@python@ OverridableImplementation.py";
@Command "mfront-query --author --material=A316LN --behaviour=Plasticity_SRMA2022 Plasticity.madnex"{
  expected_output : "John Mac Enroe"
};
~~~~

- The `@Requires` keyword lists required components (`tfel::madnex` and
  `mfront::python`). If these are unavailable, tests are skipped.
  Components can be defined in configuration files (see Section @sec:tfel_check:configuration_files).
  Default components are listed in Section @sec:tfel_check:default_components.

- The `@Command` keyword executes a command. The `@python@` syntax is a
  substitution string that must be defined (e.g., via `--@python@=python3.5`).

- The second `@Command` includes a test that validates the command's output
  against the expected string `"John Mac Enroe"`.

To run this file:

~~~~{.bash}
$ tfel-check --@python@=python3.5 madnex.check
entering directory '/tmp/madnex'
* beginning of test './madnex.check'
** Exec-1 python3.5 MadnexTest.py                                      [SUCCESS]
** Exec-2 python3.5 OverridableImplementation.py                       [SUCCESS]
** Exec-3 mfront-query --author --material=A316LN --behaviour=Plast... [SUCCESS]
* end of test './madnex.check'                                         [SUCCESS]
======
~~~~

This generates the following files:

- `tfel-check.log`: Global log file in the invocation directory.
- `madnex.checklog`: Summary of command executions and tests for `madnex.check`.
- `madnex-Exec-1.out`, `madnex-Exec-2.out`, `madnex-Exec-3.out`: Output and execution time for each command.
- `TEST-madnex.xml`: JUnit-compatible XML file for integration with [Jenkins](https://www.jenkins.io/).

If requirements are unmet, tests are skipped:

~~~~{.bash}
$ tfel-check
entering directory '/tmp/madnex'
* beginning of test './madnex.check'
** Exec-1 @python@ MadnexTest.py                                       [SKIPPED]
** Exec-2 @python@ OverridableImplementation.py                        [SKIPPED]
** Exec-3 mfront-query --author --material=A316LN --behaviour=Plast... [SKIPPED]
* end of test './madnex.check'                                         [SUCCESS]
======
~~~~

Command failures are reported as follows:

~~~~{.bash}
$ tfel-check --@python@=python2.7
entering directory '/tmp/madnex'
* beginning of test './madnex.check'
** Exec-1 toto MadnexTest.py                                           [ FAILED]
** Exec-2 toto OverridableImplementation.py                            [ FAILED]
** Exec-3 mfront-query --author --material=A316LN --behaviour=Plast... [ FAILED]
* end of test './madnex.check'                                         [ FAILED]
======
~~~~

## A more complex example

Consider a tensile test simulated with the `Cast3M` finite element solver.
An `MFront` file defines the material behaviour, and the simulation generates
a result file (`ImplicitSimoMieheElastoPlasticityUniaxialTesting-castem.res`)
containing the stress evolution along the tensile axis over time.

The `ImplicitSimoMieheElastoPlasticity.check` file:

~~~~{.cxx}
@Requires {"Cast3M", "mfront::behaviour::castem_interface"};
@Command "mfront --obuild --interface=castem ImplicitSimoMieheElastoPlasticity.mfront";
@Command "@castem@ ImplicitSimoMieheElastoPlasticityUniaxialTesting.dgibi";

@Precision 100; // absolute tolerance: 100 Pa
@Test "ImplicitSimoMieheElastoPlasticityUniaxialTesting-castem.ref"
      "ImplicitSimoMieheElastoPlasticityUniaxialTesting-castem.res" 2;
~~~~

- The `@Requires` keyword ensures `Cast3M` and its `MFront` interface are available.
- The first `@Command` compiles the behaviour.
- The second `@Command` runs the `Cast3M` simulation, using the `@castem@` substitution.
- `@Precision 100` sets an absolute tolerance of 100 Pa for comparisons.
- The `@Test` keyword compares the second column of the reference and result files.

A configuration file (`castem.config`) defines the substitution:

~~~~{.cxx}
components : {"Cast3M"};
substitutions : {"castem": "castem2019_PLEIADES"};
~~~~

Run the test with:

~~~~{.bash}
$ tfel-check --config=castem.config
entering directory '/tmp/tfel-check'
* beginning of test './ImplicitSimoMieheElastoPlasticity.check'
** Exec-1 mfront --obuild --interface=castem ImplicitSimoMieheElast... [SUCCESS]
** Exec-2 castem2019_PLEIADES ImplicitSimoMieheElastoPlasticityUnia... [SUCCESS]
** Compare-1 'ImplicitSimoMieheElastoPlasticityUniaxialTesting-cast... [SUCCESS]
* end of test './ImplicitSimoMieheElastoPlasticity.check'              [SUCCESS]
~~~~

## Command failure handling

By default, `tfel-check` ignores command failures if at least one test is defined.
This accounts for cases where a command produces correct results but exits with a
non-zero status.

Use `--discard-commands-failure` to control this behaviour:

~~~~{.bash}
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

`.check` files describe tests and comparisons using a set of keywords.

## List of available keywords

### The `@Requires` keyword

The `@Requires` keyword lists the components required to run the tests in the file.
If any component is unavailable, all tests in the file are skipped.

### The `@Environment` keyword

The `@Environment` keyword defines environment variables for commands:

~~~~{.cxx}
@Environment {"VAR1": "value1", "VAR2": "value2"};
~~~~

### The `@Command` keyword

The `@Command` keyword executes a command. It supports the following options:

- `expected_output`: Expected output as a string (single line) or an array of strings (multiline).
  The test fails if the actual output differs.
- `expected_numerical_output`: Tests numerical output. Requires a map with:
  - `value`: Expected numerical value.
  - `criterion_value`: Tolerance for the comparison.
- `output_validation_regex`: Regular expression to validate the command output.
  The output is concatenated into a single string for validation.
- `shall_fail`: Boolean. If `true`, the command must fail for the test to pass.
- `timeout`: Integer. Maximum allowed duration in seconds.

Example:

~~~~{.cxx}
@Command "echo 42" {
  expected_output: "42"
};
@Command "./my_script" {
  shall_fail: true
};
@Command "sleep 10"{
 timeout: 1
};
~~~~

### The `@Precision` keyword

The `@Precision` keyword sets the tolerance(s) for value comparisons.

- For `Absolute`, `Relative`, or `Area` comparisons: a single value.
- For `RelativeAndAbsolute` or `Mixed` comparisons: two values (relative and absolute).

Example:

~~~~{.cxx}
@Precision 1.e-6;       // single precision for Absolute/Relative
@Precision 1.e-3 1.;    // relative and absolute precision for Mixed
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
- `L2`: Same as `Area`, with the L2 norm of the difference instead of
  the integral of its absolute value. See Section
  @sec:tfel_check:l2_comparison.

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

For `Area` and `L2` comparisons, the interpolation and the abscissa
column used for the integration are given by the `@TestType` keyword:

~~~~{.cxx}
@TestType <Area|L2> interpolation <None|Linear|Spline|LocalSpline> using <column>;
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
This section details the available comparison types, interpolation methods, and precision settings.

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

### L2 comparison {#sec:tfel_check:l2_comparison}

Same as the `Area` comparison, with the L2 norm of the difference
between the two curves:

\[
e = \frac{1}{\max_{i}\left|y^{\mathrm{ref}}_{i}\right|}\sqrt{\int\left(y^{\mathrm{ref}}\left(x\right)-y^{\mathrm{res}}\left(x\right)\right)^{2}\,\mathrm{d}x}
\]

The integral is computed by the trapezoidal rule on the same points as
for the `Area` comparison, with the same rules. The error \(e\) has the
unit of the square root of the abscissa.

~~~~{.cxx}
@TestType L2 interpolation Linear using 'time';
@Precision 1.e-2;
@Test 'reference.res' 'results.res' 'stress';
~~~~

**Use case**: Same as the `Area` comparison, with more weight given to
large local differences.

## Interpolation methods {#sec:tfel_check:interpolation_methods}

Interpolation is required when the abscissa values (e.g., time, strain) in the result and reference files do not match.
The `@Interpolation` keyword specifies how the values of one file are
interpolated at the abscissas of the other one.

### No interpolation

No interpolation is performed. The values are compared line by line, so
both files must have the same number of values. If a column is given
with `using`, the abscissas of both files must be the same, up to a
relative tolerance of \(10^{-10}\). Otherwise, the abscissas are not
checked.

~~~~{.cxx}
@Interpolation None using 'time'; // the times must be the same
@Test 'reference.res' 'results.res' 'stress';
~~~~

**Use case**: Fastest method when both files use identical abscissa values.

### Linear interpolation

Linear interpolation between reference points.

~~~~{.cxx}
@Interpolation Linear using 'time';
@Test 'reference.res' 'results.res' 'stress';
~~~~

**Use case**: Simple and robust for most smooth data.

### Spline interpolation

Cubic spline interpolation for smooth curves.

~~~~{.cxx}
@Interpolation Spline using 'strain';
@Test 'reference.res' 'results.res' 'stress';
~~~~

**Use case**: High-quality interpolation for smooth data with continuous second derivatives.

### Local Spline interpolation

Local spline interpolation avoids global smoothness constraints, making it more robust for noisy or non-smooth data.

~~~~{.cxx}
@Interpolation LocalSpline using 'time' AllowLessResults;
@Test 'reference.res' 'results.res' 'stress';
~~~~

**Use case**: Preferred for data with sharp transitions or when the result file has fewer points than the reference.

### Allowing fewer results

The `AllowLessResults` option allows the result file to have fewer data points than the reference file.
This is useful when the simulation uses adaptive time-stepping or when reference data is highly resolved.
The results must still reach the bounds of the abscissas of the reference.

~~~~{.cxx}
@Interpolation Linear using 'time' AllowLessResults;
@Test 'reference.res' 'results.res' 1;
~~~~

## Multi-column comparisons

Multiple columns can be compared in a single `@Test` directive by listing their indices or names.

~~~~{.cxx}
@TestType Absolute;
@Precision 1.e-6;
@Test 'reference.res' 'results.res' 1 2 3;       // compare columns 1, 2, and 3
@Test 'reference.res' 'results.res' 'A' 'B';    // compare columns named 'A' and 'B'
~~~~

**Note**: All specified columns are compared using the same `@TestType` and `@Precision`.

## Practical examples

### Example 1: Simple absolute comparison

Compare the second column of two files with an absolute tolerance of 1e-6:

~~~~{.cxx}
@TestType Absolute;
@Precision 1.e-6;
@Test 'reference.res' 'results.res' 2;
~~~~

### Example 2: Relative comparison with interpolation

Compare stress-strain curves where the strain values (column 1) differ between files:

~~~~{.cxx}
@TestType Relative;
@Precision 1.e-3;
@Interpolation Spline using 1;
@Test 'reference.res' 'results.res' 2;
~~~~

### Example 3: Mixed comparison with local spline

Strict comparison of temperature evolution with local spline interpolation:

~~~~{.cxx}
@TestType Mixed;
@Precision 1.e-4 1.e-6; // relative and absolute tolerances
@Interpolation LocalSpline using 'time' AllowLessResults;
@Test 'reference.res' 'results.res' 'temperature';
~~~~

### Example 4: Area comparison of force-displacement curves

Compare two force-displacement curves sampled differently:

~~~~{.cxx}
@TestType Area interpolation Linear using 'displacement';
// displacement in mm, range of 2 mm: mean deviation of 0.5 % of the
// maximum force
@Precision 1.e-2;
@Test 'reference.res' 'results.res' 'force';
~~~~

### Example 5: Multi-column comparison with different precisions

Compare multiple columns with different precisions (requires separate `@Test` directives):

~~~~{.cxx}
@TestType Absolute;
@Precision 1.e-3;
@Test 'reference.res' 'results.res' 'stress';

@TestType Relative;
@Precision 1.e-4;
@Test 'reference.res' 'results.res' 'strain';
~~~~

# Description of the configuration files{#sec:tfel_check:configuration_files}

Configuration files define:

- Substitution strings (e.g., `@python@` → `python3.5`).
- Available components (e.g., `Cast3M`, `tfel::madnex`).
- Environment variables.

The syntax is JSON-inspired:

~~~~{.cxx}
components: {"Cast3M", "tfel::madnex"};
substitutions: {"python": "python3.5", "castem": "castem2019"};
environment_variables: {"CXXFLAGS": "-Wall", "OMP_NUM_THREADS": "4"};
~~~~

## Defining environment variables in configuration files

The `environment_variables` section defines a map of environment variables:

~~~~{.cxx}
environment_variables: {"CXXFLAGS": "-Wall", "MPI_ROOT": "/path/to/mpi"};
~~~~

### Caution

- Environment variables can only be defined **once** across all configuration files.
- If an environment variable is defined in both a configuration file and a test file
  (via `@Environment`), the test file's definition takes precedence.

# Default components {#sec:tfel_check:default_components}

`tfel-check` automatically defines components based on the enabled features:

- If `TFEL`'s Python bindings are enabled: `tfel::python` and `mfront::python`.
- If `madnex` support is enabled: `tfel::madnex`.
- For each `MFront` material property interface: `mfront::material_property::<name>_interface`.
- For each `MFront` behaviour interface: `mfront::behaviour::<name>_interface`.
- For each `MFront` model interface: `mfront::model::<name>_interface`.

To list all default components, use:

~~~~{.bash}
$ tfel-check --list-default-components
- tfel::python
- mfront::python
- tfel::madnex
- mfront::material_property::C++_interface
- mfront::material_property::c_interface
- mfront::behaviour::generic_interface
- mfront::model::mfront_interface
...
~~~~

# Automatic declaration of substitutions

`tfel-check` automatically declares substitutions for `TFEL` executables:

- `@mfront@`
- `@mfront-query@`
- `@mtest@`
- `@mfront-doc@`
- `@mfm-test-generator@`

If Python bindings are enabled, `@python` is also declared.

These default substitutions are applied **after** reading configuration files
and command-line arguments, so they can be overridden by the user.

# Command line arguments

For a full list of command-line options, run:

~~~~{.bash}
$ tfel-check --help
~~~~

## Summary of options

| Option | Description |
|--------|-------------|
| `--clean-up` | clean generated files and directories listed by the `@CleanFiles` and `@CleanDirectories` keywords in case of success. |
| `--discard-clean-up-on-failure` | don't clean generated files and directories listed by the `@CleanFiles` and `@CleanDirectories` keywords in case of failure (true by default). |
| `--config, -c` | Add a configuration file. |
| `--discard-commands-failure` | Ignore command failures if comparisons pass (default: `true`). If no comparisons are defined, failures are never ignored. |
| `--discard-jobs-limit` | Disable the limit on parallel jobs (default: enforced). |
| `--help, -h` | Display help message. |
| `--jobs, -j` | Number of parallel jobs (default: number of CPU cores). |
| `--list-default-components` | List all default components. |
| `--synchronize-terminal-output` | Synchronize terminal output in parallel (default: `false`). |
| `--use-terminal-colors` | Use terminal colors for output. |
| `--version, -v` | Display version information. |

## Usage examples

Run all `.check` files in the current directory:

~~~~{.bash}
$ tfel-check
~~~~

Run specific files with 4 parallel jobs:

~~~~{.bash}
$ tfel-check -j 4 test1.check test2.check
~~~~

List default components:

~~~~{.bash}
$ tfel-check --list-default-components
~~~~

Override a substitution and enable terminal colors:

~~~~{.bash}
$ tfel-check --@python@=python3.10 --use-terminal-colors=true
~~~~
