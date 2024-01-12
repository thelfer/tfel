---
title: Support of `madnex` files in `MFront`
author: Thomas Helfer
date: 2020
lang: en-EN
numbersections: true
documentclass: article
from: markdown+tex_math_single_backslash
geometry:
  - margin=2cm
papersize: a4
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
bibliography: bibliography.bib
---

The `madnex` file format has been developed by `EDF` to store their
experimental data. It extends the `HDF5` file format by providing a
suitable data model. Internally, `EDF` uses a closed-source `python`
library to insert, query and extract data from `madnex` file.

The `madnex` library is an open-source `C++` library which currently
focus on the storage of `MFront` implementations. This library can be
downloaded here:

<https://github.com/thelfer/madnex>

The website of this library [@cea_edf_madnex_library_2020] details the
data model used to store `MFront` implementations.

# Compilation of `TFEL` with `madnex` files support

By default, support of `madnex` files is disabled.

Support for `madnex` files as inputs of `MFront` must be explicitly
enabled during the compilation of the `TFEL` project. More precisely,
when invoking the [`cmake`](https://cmake.org/) build system, the
following variables must be set:

1. `enable-madnex` must be set to `ON`.
2. `Madnex_DIR` must be set to the path where the `MadnexConfig.cmake`
  file has been installed. This path is usually given by
  `$MADNEXHOME/share/madnex/cmake/` where `MADNEXHOME` is the
  installation directory of the `madnex` library.

Hence, a typical call `cmake` to cmake would be similar as:

~~~~{.bash}
$ cmake -Denable-madnex=ON -DMadnex_DIR=$MADNEXHOME/share/madnex/cmake ...
~~~~

This call assumes that that the `MADNEXHOME` environment variable has
been set.

More information about the options that can be passed to `cmake` are
given in the `INSTALL-cmake` file which is located at the root of the
`TFEL` sources.

# `mfront` usage

The only thing specific to `madnex` files is the selection of the
implementation to be treated.

For a behaviour, this selection is done through the `--material` and
`--behaviour` options. For example:

~~~~{.bash}
$ mfront --obuild --interface=generic --material=<material_id> --behaviour=<behaviour_id> file.mdnx
~~~~

Here, `behaviour_id` can be a regular expression.

For material properties and models, the `--material-property` and
`--model` options have a similar role than the `--behaviour` option for
behaviours.

The `--all-material-properties`, `--all-behaviours`, and `--all-models`
options allows to select all the material properties, behaviours and
models respectively, as follows:

~~~~{.bash}
$ mfront --obuild --interface=generic --material=<material_id> --all-behaviours file.mdnx
~~~~

The material name `<none>` has a special meaning here: it allows the
selection of implementations that not associated with a material.

## Alternative way to select a single material knowledge (material property, behaviour, model) in a `madnex` file {#sec:tfel-3.4.4:mfront:mandex_full_path}

`mfront`, `mfront-query` and `mfront-doc` allow to select a specific
material knowledge inside a `madnex` file using the following syntax:

~~~~
madnex:<file>:<type>:<material>:<name>
~~~~

where:

- `<file>` is the path to the `madnex` file.
- `<type>` is the type of the material knowledge (`material_property`,
  `behaviour` or `model`).
- `<material>` is the name of the material considered. This name can be
  empty or `<none>` if the considered material knowledge is not
  associated to a material.
- `<name>` is the name of theconsidered material knowledge.

### Example of usage

~~~~{.bash}
$ mfront --obuild --interface=generic madnex:Plasticity.mdnx:behaviour::Plasticity
~~~~

or, equivalently:

~~~~{.bash}
$ mfront --obuild --interface=generic madnex:Plasticity.mdnx:behaviour:"<none>":Plasticity
~~~~

## Add a `madnex` file to the search paths

`MFront` files may depend on other `MFront` files. A `madnex` file can
be used resolve those dependencies using the `--madnex-search-path`
command line argument.

### A first example

Let us consider a `madnex` file containing the `YoungModulusTest`
material property associated with no material and a file `Test.mfront`
requiring to have access to this material property. For example, the
`Test.mfront` may contain an instruction such as:

~~~~{.cxx}
@MaterialLaw "YoungModulusTest";
~~~~

The `Test.mfront` file can be compiled as follows:

~~~~{.cxx}
$ mfront --obuild --interface=generic                 `
         --madnex-search-path=MaterialProperties.mdnx `
         Test.mfront
~~~~

### `madnex` search paths

Options to the `--madnex-search-path` can be decomposed as

`<file_path>:<material_knowledge_type>:<material_identifier>`

where `<material_knowledge_type>` and `<material_identifier>` are
optionals, as in the previous example.

#### Selecting a specific type of material kowledge

`material_knowledge_type` may have one of the following values
`material_property`, `behaviours` and `models`.

If `material_knowledge_type` is not specified, materials properties,
behaviours and models are all considered (in that order).

#### About the material identifier

The `<material_identifier>` is interpreted as a regular expression.

The regular expression that selects material knowledge associated with
all materials is `.+`, but this will exclude material knowledge
associated with no material.

The regular expression `.*` will select material knowledge associated to
all material *and* material knowledge associated with no material. This
is what happen if not `material_identifier` is specified.

The special material identifier `<none>` selects only material knowledge
associated with no material.

#### Examples of madnex search paths

- `MaterialProperties.mdnx`: appends all the materials properties,
  behaviours, models associated to any materials or to no material to
  the search path.
- `MaterialProperties.mdnx:material_property`: appends all the
  materials properties, associated to any materials or to no material to
  the search path.
- `MaterialProperties.mdnx:material_property:Zircaloy4`: appends all
  the materials properties, associated to `Zircaloy4` to the search
  path.

### How `madnex` files are searched

The `madnex` files specified in `madnex` search paths are first search
in the current directory, and then in the directories specified by the
`--search-path` command line arguments.

## Automatic declaration of a `madnex` input file as a `madnex` search path

When `MFront` uses a `madnex` input file, this file is automatically
added to the `madnex` search path.

### Example of usage

Let us consider a file a `madnex` file containing a behaviour `Test` and
a material property `YoungModulusTest` which is used by the `Test`
behaviour, then the following instructions work as expected:

~~~~{.bash}
$ mfront --obuild --interface=aster --behaviour=Test Example.mdnx 
Treating target : all
The following libraries have been built :
- libAsterBehaviour.so :  astertest
$ mfront-query --list-dependencies --behaviour=Test Example.mdnx 
madnex:Example.mdnx:MaterialProperty::YoungModulusTest 
~~~~

# `mfront` python module

The `mfront` python module is fully described here:

<https://thelfer.github.io/tfel/web/mfront-python.html>

The main function related to `madnex` file support is the
`getImplementationsPaths` which takes five arguments:

- the name of the file. The name of this argument is `file`;
- the name of a material. If the material name is empty, the
  implementations associated with all the materials are are considered
  and also the implementations associated with no material. To select
  only implementations associated with no material, one shall use the
  special material name `<none>`. The name of this argument is `material`.
- a regular expression describing the searched material properties. This
  argument can be empty. The name of this argument is
  `material_property`.
- a regular expression describing the searched behaviours. This argument
  can be empty. The name of this argument is `behaviour`.
- a regular expression describing the searched models. This argument can
  be empty. The name of this argument is `model`.

This function returns a list of strings where the path of the
implementation is encoded.

~~~~{.python}
import mfront

paths = mfront.getImplementationsPaths("file.mdnx",
                                       material=<material_id>,
                                       behaviour=behaviour_id>)
if len(paths) == 0:
  raise("no matching implementation")

dsl = mfront.getDSL(paths[0])
dsl.analyseFile(paths[0])
~~~~

# `mtest` usage

## Executing a test stored in a `madnex` file

To execute a test stored in a `madnex` file, the user must specify:

- the name of the `madnex` file
- the name of the test using the `--test` (or `-t`) command line
  argument.
- the name of the behaviour to which the test is associated using the
  `--behaviour` (or `-b`) command line argument.
- the name of the material is the behaviour to which the test is
  associated is associated with a material using the `--material` (or
  `-m`) command line argument. If the material is not specified, or if
  the special material name `<none>` is used, the behaviour associated
  with the test is assumed not associated with any material.

Note that the `--test` (or `-t`) command line argument can accept
regular expressions to select as set of tests.

### Example of usage

The following example executes the `UniaxialTensileTest` test 
associated with the `Plasticity` behaviour (and not attached to any
material) using the behaviour `cyranoplasticity` compiled with the
`cyrano` interface in a shared library `libCyranoBehaviours.so` located
in the `src` subdirectory and stored in the `Plasticity.mdnx` file:

~~~~{.bash}
$ mtest  --behaviour=Plasticity --test=UniaxialTensileTest        \
         --@interface@=cyrano --@behaviour@="'cyranoplasticity'"  \
         --@library@="'src/libCyranoBehaviours.so'"               \
         Plasticity.mdnx
~~~~

## Execution all tests associated with a behaviour stored in a `madnex` file

The user can execute all tests associated with a behaviour using the
`--all-tests` command line arguments. The user must specify the name of
the behaviour (using the `--behaviour` (or `-b`) command line argument)
and optionally the name of the material (using the `--material` (or
`-m`) command line argument).

~~~~{.bash}
$ mtest  --behaviour=Plasticity --@interface@=cyrano \
         --@behaviour@="'cyranoplasticity'"          \
         --@library@="'src/libCyranoBehaviours.so'"  \ 
         --all-tests Plasticity.mdnx                 \
~~~~

The `--all-tests` command line argument is equivalent to `--test=".+"`.

## Alternative way to select a single test in a `madnex` file

`mtest` allows to select a test inside a `madnex` file using the
following path syntax:

~~~~
madnex:<file>:<material>:<behaviour>:<test>
~~~~

where:

- `<file>` is the path to the `madnex` file.
- `<material>` is the name of the material considered. This name can be
  empty or `<none>` if the considered test is not
  associated to a material.
- `<behaviour>` is the name of the behaviour
- `<test>` is the name of the test

# `mfront-query` usage

## Options specific to `madnex` files

Several options has been added to the `mfront-query` specifically for
`madnex` files.

Queries of the file itself (list of materials, behaviours, etc...) are
also provided by the `madnex-query` tool.

### Listing all material a `madnex` file

The `--list-materials` returns the list of all materials in a `MFront`
file:

~~~~{.bash}
$ mfront-query --list-materials file.mdnx
~~~~

### Listing of all material properties, all behaviours or all models

The following options are available:

- `--list-material-properties`: list of material properties.
- `--list-behaviours`: list of all behaviours.
- `--list-models`: list of all models.

A typical call is as follows:

~~~~{.bash}
$ mfront-query --list-behaviours file.mdnx
~~~~

The previous options are affected by the the definition of a material,
as follows:

~~~~{.bash}
$ mfront-query --material=<material_id> --list-behaviours file.mdnx
~~~~

## Querying information about implementation, options specific to `madnex` files

For a behaviour, the selection of an implementation on which the queries
are made is done through the `--material` and `--behaviour` options. For
example:

~~~~{.bash}
$ mfront-query --obuild --interface=generic --material=<material_id> --behaviour=<behaviour_id> file.mdnx --state-variables
~~~~

Here, `behaviour_id` can be a regular expression.

For material properties and models, the `--material-property` and
`--model` options have a similar role than the `--behaviour` option for
behaviours.

### List of `MTest` tests associated with a behaviour in a `madnex` file

The `--list-behaviour-mtest-tests` command line argument can be used to
display the list of tests associated with a behaviour in a `madnex`
file.

Optionnally, this command line argument accept the options
`sorted-by-behaviours` or `unsorted` (see the examples below).

#### Examples of usage

~~~~{.cxx}
$ mfront-query --list-behaviour-mtest-tests --test=".+Tensile.+" Plasticity.mdnx
- tests associated with behaviour Plasticity
    - UniaxialTensileTest
~~~~

~~~~{.cxx}
$ mfront-query --list-behaviour-mtest-tests=unsorted --test=".+Tensile.+" Plasticity.mdnx
UniaxialTensileTest
~~~~

### List of `mfm-test-generator` tests associated with a behaviour in a `madnex` file

The `--list-behaviour-mfm-test-generator-tests` command line argument
can be used to display the list of tests associated with a behaviour in
a `madnex` file.

Optionnally, this command line argument accept the options
`sorted-by-behaviours` or `unsorted` (see the examples below).

# Storing an `MFront` file in a `madnex` file

The easiest way to store an `MFront` file is to create an instance of
the `OverridableImplementation` class and then call the `write`
function. Here is an example in `python`:

~~~~{.python}
import mfront

i = mfront.OverridableImplementation("Plasticity.mfront")
mfront.write(i, "Plasticity.mdnx")
~~~~

Note that an override of the `write` function is available to specify a
template file, as follows:

~~~~{.python}
mfront.write(i, template_file_path,"Plasticity.mdnx")
~~~~

However, the `OverridableImplementation` class also allows modifying
the medata of a implementation, as follows:

~~~~{.python}
import mfront

i = mfront.OverridableImplementation("Plasticity.mfront")
i.overrideBehaviourName("Plasticity_SRMA2022")
i.overrideMaterial("A316LN")
i.overrideAuthor("John Mac Enroe")
mfront.write(i, "Plasticity.mdnx")
~~~~

# Storing an `MTest` file in a `madnex` file

## `C++` Application Programming Interface (API)

The `TFELMTest` library exposes a data structure named `TestDescription` which
describes an `MTest` file and two functions called respectively
`loadMTestFileContent` and `write`.

### The `TestDescription` data structure

The `TestDescription` data structure exposes the following data members:

- `author`, which describes the author of the test.
- `date`, which describes the date at which the test has been created.
- `scheme`, which describes the type of test. Valid values are `mtest`
  and `ptest`.
- `description`, which describes a description of the test.
- `behaviour`, name of the behaviour to which the test is associated.
  This data member is required to export the file in the `madnex` file
  format.
- `material`, name of the material to which the test is associated. This
  data member can be empty.
- `content`, content of the `MTest` file. This content can be filled
  from an existing `MTest` file using the `loadMTestFileContent`
  function.

### The `loadMTestFileContent` function

The `loadMTestFileContent` function loads the content of an `MTest` file
and stores it in the `content` data member of a `TestDescription` data
structure.

### The `write` function

The `write` function exports an `MTest` test, described by a
`TestDescription` data structure, to a file.

The file format is deduced from the extension of the file.

Currently, only extensions associated with the [`madnex` file
format](https://github.com/thelfer/madnex) are supported if `TFEL` is
compiled with support of this file format. Those extensions are: `mdnx`,
`madnex` (deprecated) or `edf` (experimental data file, deprecated).
Note that the behaviour member of the metadata must be specified for
export in the `madnex` file format.

## `Python` Application Programming Interface (API)

The `mtest` `python` module reflects the `C++` API and exposes the
`TestDescription` data structure and the `loadMTestFileContent` and
`write` functions.

### Example of usage

The following example shows how to store an existing `MTest` file to a
`madnex` file:

~~~~{.python}
import mtest

d = mtest.TestDescription()
d.author = 'John Doe'
d.date = '01/03/2022'
d.name = 'UniaxialTensileTest'
d.scheme = 'mtest'
d.behaviour = 'Plasticity'
mtest.loadMTestFileContent(d, 'Plasticity.mtest')

mtest.write(d,'Plasticity.mdnx')
~~~~

## Best practices

We highly recommend to use the following substitution variables when
defining the test:

- `@interface@`, which is meant to be replaced by the interface to be
  used. This is very handy if the test can be run for different
  interfaces
- `@library@`, which is meant to be replaced by the path to the shared
  library containing the tested behaviour.
- `@behaviour@`, which contains the name of the function implementing
  the behaviour for the considered interface.

# Storing an `mfm-test-generator` file in a `madnex` file

## `C++` Application Programming Interface (API)

The `MFMTestGenerator` library exposes a data structure named
`TestDescription` which describes an `mfm-test-generator` file and two
functions called respectively `loadMFMTestGeneratorFileContent` and `write`.

### The `TestDescription` data structure

The `TestDescription` data structure exposes the following data members:

- `author`, which describes the author of the test.
- `date`, which describes the date at which the test has been created.
- `description`, which describes a description of the test.
- `behaviour`, name of the behaviour to which the test is associated.
  This data member is required to export the file in the `madnex` file
  format.
- `material`, name of the material to which the test is associated. This
  data member can be empty.
- `content`, content of the `mfm-test-generator` file. This content can
  be filled from an existing `mfm-test-generator` file using the
  `loadMFMTestGeneratorFileContent` function.

### The `loadMFMTestGeneratorFileContent` function

The `loadMFMTestGeneratorFileContent` function loads the content of an
`mfm-test-generator` file and stores it in the `content` data member of
a `TestDescription` data structure.

### The `write` function

The `write` function exports an `mfm-test-generator` test, described by a
`TestDescription` data structure, to a file.

The file format is deduced from the extension of the file.

Currently, only extensions associated with the [`madnex` file
format](https://github.com/thelfer/madnex) are supported if `TFEL` is
compiled with support of this file format. Those extensions are: `mdnx`,
`madnex` (deprecated) or `edf` (experimental data file, deprecated).
Note that the behaviour member of the metadata must be specified for
export in the `madnex` file format.

## `Python` Application Programming Interface (API)

The `mfm_test_generator` `python` module reflects the `C++` API and
exposes the `TestDescription` data structure and the
`loadMFMTestGeneratorFileContent` and `write` functions.

### Example of usage

The following example shows how to store an existing
`mfm-test-generator` file to a `madnex` file:

~~~~{.python}
import mfm_test_generator

d = mfm_test_generator.TestDescription()
d.author = 'John Doe'
d.date = '01/03/2022'
d.name = 'UniaxialTensileTest'
d.behaviour = 'Plasticity'
mfm_test_generator.loadMFMTestGeneratorFileContent(d, 'Plasticity.mdnx')

mfm_test_generator.write(d,'Plasticity.mdnx')
~~~~

## Best practices

We highly recommend to use the following substitution variables when
defining the test:

- `@interface@`, which is meant to be replaced by the interface to be
  used. This is very handy if the test can be run for different
  interfaces
- `@library@`, which is meant to be replaced by the path to the shared
  library containing the tested behaviour.
- `@behaviour@`, which contains the name of the function implementing
  the behaviour for the considered interface.

# References
