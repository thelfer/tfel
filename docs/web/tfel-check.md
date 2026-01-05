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

## Parallelization

The commands described in different `.check` files can now be run in
distinct processes.

Note that the command described in a `.check` are still run
sequentially.

By default, as in previous versions, only one process is used.

### Specifying the number of jobs run simultaneously

The command line argument `--jobs` (`-j`) allow specifying the number
of jobs run simultaneously.

In no option option is given to `--jobs`, the number of cores
available is used, if the system provide this information.

#### Limit on the number of jobs

By default, this number must be lower than the number of cores
available. This limit can be discarded by
passing`--discard-jobs-limit=true` to `tfel-check`.

### Caution

The user must take care that the commands described in `.check` files
can be run in parallel.

- the jobs shall no exhaust memory.
- the jobs shall not access concurrently to the same resources.

A typical example of conflict would be to have two `.check` files in the
same directory that call `mfront` to build somes shared libraries: the
two `MFront`'s processes may write concurrently files in the `src` and
`include` directories, leading to an undefined behaviour.

Another example would be to use `@CleanDirectories` and `@CleanFiles`
commands inappropriately and erase data used by commands of other
`.check` files.

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
** Exec-1 python3.5 MadnexTest.py                                      [SUCCESS]
** Exec-2 python3.5 OverridableImplementation.py                       [SUCCESS]
** Exec-3 mfront-query --author --material=A316LN --behaviour=Plast... [SUCCESS]
* end of test './madnex.check'                                         [SUCCESS]
======
~~~~

This command produces several files:

- `tfel-check.log`: this file contains the output of `tfel-check`. This
  file is created in the directory where `tfel-check` has been invoked.
- `madnex.checklog`: this file contains a summary of the execution of
  the commands and test described in the `madnex.check` file. A similar
  file is created for each file considered by `tfel-check`.
- Three files `madnex-Exec-1.out`, `madnex-Exec-2.out` and
  `madnex-Exec-3.out` associated to the three commands listed in the
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
** Exec-1 @python@ MadnexTest.py                                       [SKIPPED]
** Exec-2 @python@ OverridableImplementation.py                        [SKIPPED]
** Exec-3 mfront-query --author --material=A316LN --behaviour=Plast... [SKIPPED]
* end of test './madnex.check'                                         [SUCCESS]
======
~~~~

If the execution of a command fails, this is reported as follows: 

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
@Test "ImplicitSimoMieheElastoPlasticityUniaxialTesting-castem.res"
      "ImplicitSimoMieheElastoPlasticityUniaxialTesting-castem.ref" 2;
~~~~

The first line specifies that the `Cast3M` component must be available
and that the support of the `Cast3M` interface is available in `MFront`.
The second line compiles the behaviour.

The third one launches the `Cast3M` simulation. This line assumes that a
substitution string for the `@castem@` pattern has been defined.

The fifth line specifies an absolute criterion to compare the current
results to the reference ones. The last line defines the test to be
performed: the values of the second column of the two specified files
are compared using an absolute criteria.

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

The `@Test` keyword specifies two files to compare which represents
respectively the current simulated values and the reference ones.

### The `@TestType` keyword

The `@TestType` keyword specifies how the comparison between the
simulated values and the reference values shall be performed.

The following kind of comparison are available:

- `Absolute`:
- `Relative`:
- `RelativeAndAbsolute`:
- `Mixed`:
- `Area`:

### The `@Interpolation` keyword

The `@Interpolation` keyword specifies how the reference values can be
interpolated. The following interpolations are available:

- `None`:
- `Linear`:
- `Spline`:
- `LocalSpline`:

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