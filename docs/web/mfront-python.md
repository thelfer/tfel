---
title:  The `mfront` module
author: Thomas Helfer
date: 04/06/2016
lang: en-EN
numbersections: true
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
lstPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
header-includes:
bibliography: bibliography.bib
csl: iso690-numeric-en.csl
---

# Introduction

TFEL provides a python module called `mfront` to extract information
from the MFront files. Typical usage is as follows:

~~~~ {.python}
from tfel.material import ModellingHypothesis
import mfront

dsl = mfront.getDSL("Chaboche.mfront")
dsl.analyseFile("Chaboche.mfront",[])

# behaviour description
bd = dsl.getBehaviourDescription()

print("List of supported modelling hypotheses:\n")
print(bd.getModellingHypotheses())
print("\n")

# a deeper look at the 3D case
d = bd.getBehaviourData(ModellingHypothesis.TRIDIMENSIONAL)
print("List of material properties in 3D:\n")
for mp in d.getMaterialProperties():
    print("- "+mp.type+" "+mp.name+" as "+d.getExternalName(mp.name))

print("\nList of state variables in 3D:\n")
for iv in d.getPersistentVariables():
    print("- "+iv.type+" "+iv.name+" as "+d.getExternalName(iv.name))
~~~~

The main class used for analysing `MFront` file is the `AbstractDSL`
class and its child classes (DSL means "domain specific language").

In the following, many classes are described. The name of those
classes match the names of the underlying C++ classes. Currently, the
`mfront` module is mostly undocumented: the user shall refer to the
C++ documentation.

# The `AbstractDSL` class

## Generating an object of type `AbstractDSL`

There are two ways of generating an object of type `AbstractDSL`:

- using the `DSLFactory` class.
- using the `getDSL` function.

### Using the `DSLFactory` class

The `DSLFactory` factory allows the creation of an object derived for
the `AbstractDSL` class by its name:

~~~~ {.python}
f   = mfront.DSLFactory.getDSLFactory()
dsl = f.createNewDSL("Implicit")
~~~~

In this example, `Implicit` is the name of the DSL.

### Using the `getDSL` function.

The direct usage of `DSLFactory` to generate an `AbstractDSL` object
is cumbersome as one generally does not know *a priori* the DSL used
in a given `MFront` file.

`MFront` uses a two-pass analysis:

- The first pass extracts the DSL to be used from the `MFront` file
- The second one consists in analyzing the file using this DSL.

The first pass can be done using the `getDSL` function which takes a
file name as an argument. Here is a typical example:

~~~~ {.python}
dsl = mfront.getDSL("Chaboche.mfront")
~~~~

## Using the `AbstractDSL` class

### Specifying interfaces

The `AbstractDSL` class provides a method named `setInterfaces` to
specify a list of interfaces to be used during the analysis.

Note that providing interfaces is optional. No specifying an interface
does not prevent the analysis to be performed.

The main interest of providing an interface is to get the (potential)
results of the output file generation, the name of the generated
libraries and the name of the entry points. See the *Retrieving
information about the (potential) outputs of the analysis of a
`MFront` file: the `TargetsDescription` and the `LibraryDescription`
classes* section below.

### Analysing a file

The `AbstractDSL` class has a method `analyseFile`. This method has
two overloads:

- the first one takes a file name
- the second one takes a file name and a list of additional keywords

Here is an example of the usage of the first overload:

~~~~ {.python}
dsl.analyseFile("Chaboche.mfront")
~~~~

Here is an example of the usage of the second overload:

~~~~ {.python}
dsl.analyseFile("Chaboche.mfront",
                ["@CompareToNumericalJacobian true;"])
~~~~

### Analysing a string

The `AbstractDSL` class has a method called `analyseString`. This
method is mainly used for generating unit-tests.

### Retrieving information about a `MFront` file: the `FileDescription` class

Once the analysis of a `MFront` file is done, one can retrieve general
information about this file using the `getFileDescription` method of
the `AbstractDSL` class. This method returns an object of type
`FileDescription`.

A `FileDescription` object has the following fields:

- `fileName` : this field gives the name of the file which has been
  analyzed. This name is empty if one has used the `analyseString`
  method of the `AbstractDSL` class
- `authorName`: this field gives the name of the author of the `MFront`
  file if one has been given using the `@Author` keyword.
- `date`: this field give the date at which the `MFront`
  file has been written if one has been given using the `@Date` keyword.
- `description`: this field return the description of the file, if one
  has been given using the `@Description` keyword.

Here is an example of the usage of `FileDescription` class:

~~~~ {.python}
fd = dsl.getFileDescription()
print("file author:      ", fd.authorName)
print("file date:        ", fd.date)
print("file description:\n", fd.description)
~~~~

### Retrieving information about the (potential) outputs of the analysis of a `MFront` file: the `TargetsDescription` and the `LibraryDescription` classes

Once the analysis of a `MFront` file is done, one can retrieve general
information about what would be generated by MFront using the
`getTargetsDescription` method of the `AbstractDSL` class. This method
returns an object of type `TargetsDescription`.

Note that the information contained in this output is far more
useful if at least on interface was defined.

The targets description can be printed. The results is a JSON-like
description of the `TargetsDescription` which is equivalent to the
content of the `target.lst` file generated by `MFront`.

#### Specific targets

The targets description has a field named specific targets. This field
has not been mapped to `python` yet.

#### Libraries

The main outputs of `MFront` are libraries. The `TargetsDescription`
class can be seen as a container of objects of type
`LibraryDescription`. 


Thus, one can iterate over the (to be) generated
libraries like this:

~~~~ {.python}
td = dsl.getTargetsDescription()
# loop over (to be) generated libraries
for l in td.getLibraries():
    print(l)
~~~~

> The `API` changed in version 3.3. In previous versions the
> `TargetsDescription` was iterable and one could write:
>
> ~~~~ {.python}
> td = dsl.getTargetsDescription()
> # loop over (to be) generated libraries
> for l in td:
>     print(l)
> ~~~~


The `LibraryDescription` class has the following fields:

- `name`: name of the library, without prefix (`lib` in most cases
  under most unices. `python` modules have no suffix) nor suffix
  (generally `.so` under most unices, `.dll` under most windows)
- `epts`: list of entry points. Entry points encompass exported
  functions, exported classes, etc...
- `include_directories`: path to header files.
- `link_directories`: path to libraries to be linked to the generated
  library.
- `link_libraries`: list of libraries to be linked to the generated
  library. Other libraries generated by `MFront` are excluded.
- `deps`: dependency of the library with respect to other library
  generated by `MFront`
- `cppflags`: compiler flags
- `ldflags`: linker flags
- `prefix`: prefix of the library (see the `name` field description
  for details)
- `suffix`: suffix of the library (see the `name` field description
  for details)
- `type`: type of library generated. This only makes sense on `Mac Os`
  where there is a distinction between shared libraries and modules.

## Generating outputs

Once the analysis of an `MFront` file is done, one can generate the
output files (most of the time `C++` sources) using the
`generateOutputFiles` method of the `AbstractDSL` class.

## Compiling using the `MakefileGenerator` class

Once `C++` sources have been generated, one may want to compile them
and generate the final libraries.

`MFront` relies on external tools, called generators, for this step
(this semantic is the same as the one used by `cmake`).

The only available generator is currently the `Makefile` generator,
handled by the `MakefileGenerator` class. This generator can be used
as follows:

~~~~{.python}
import mfront
dsl = mfront.getDSL("Chaboche.mfront")

dsl.setInterfaces(['castem'])
dsl.analyseFile("Chaboche.mfront")
dsl.generateOutputFiles()

m = mfront.MakefileGenerator()
m.exe(dsl.getTargetsDescription())
~~~~

The `exe` methods provides several overloads and may accept an
additional argument of type `GeneratorOptions` which lets the user
specify the optimisation level to be used. See the results of
`help(mfront.MakefileGenerator)` for details.

## Compiling using the `CMakeGenerator` class

Once `C++` sources have been generated, one may want to compile them
and generate the final libraries.

`MFront` relies on external tools, called generators, for this step
(this semantic is the same as the one used by `cmake`).

The only available generator is currently the `CMake` generator,
handled by the `CMakeGenerator` class. This generator can be used
as follows:

~~~~{.python}
import mfront
dsl = mfront.getDSL("Chaboche.mfront")

dsl.setInterfaces(['castem'])
dsl.analyseFile("Chaboche.mfront")
dsl.generateOutputFiles()

m = mfront.CMakeGenerator()
m.exe(dsl.getTargetsDescription())
~~~~

The `exe` methods provides several overloads and may accept an
additional argument of type `GeneratorOptions` which lets the user
specify the optimisation level to be used. See the results of
`help(mfront.CMakeGenerator)` for details.

# Analysing Mechanical Behaviours

This section is dedicated to the use of `AbstractBehaviourDSL` class
which is derived from the `AbstractDSL` to get specific information on
the mechanical behaviour described in an `MFront` file.

The `AbstractBehaviourDSL` class provides the
`getBehaviourDescription` class which returns an object of type
`BehaviourDescription` which will be discussed in depth in this
section along with the `BehaviourData` class:

- The `BehaviourDescription` class gathers general information about
  the behaviour (supported modelling hypotheses, etc...).
- The `BehaviourData` class gathers the information for a specific
  modelling hypothesis (list of material properties, list of
  persistent variables, list of external state variables, code blocks,
  etc...).

Here is a typical usage example:

~~~~ {.python}
from tfel.material import ModellingHypothesis
import mfront

dsl = mfront.getDSL("Chaboche.mfront")
dsl.analyseFile("Chaboche.mfront",[])
# general information on the mechanical behaviour
bd = dsl.getBehaviourDescription()
# a deeper look at the 3D case
d = bd.getBehaviourData(ModellingHypothesis.TRIDIMENSIONAL)
~~~~

## The `BehaviourDescription` class

### Supported modelling hypotheses and specialized modelling hypotheses

Modelling hypotheses are described in the `ModellingHypothesis` class
of the `tfel.material` package.

The `getModellingHypothesis` method returns all the modelling
hypotheses supported by the behaviour.

Among those hypotheses, some of them have been specialized, i.e. have
specific variables or specific code blocks. For each specialized
hypotheses, a specific `BehaviourData` object is created. The other
hypotheses share the same `BehaviourData` object. The
`getDistinctModellingHypotheses` returns a list of hypotheses which
have their own `BehaviourData` object. This list may contain the
`UNDEFINEDHYPOTHESIS` value if it turns out that some hypotheses have
not been specialized.

For example, consider a behaviour supports the `PLANESTRESS`,
`PLANESTRAIN` and `TRIDIMENSIONAL` hypotheses. The
`getModellingHypothesis` method would return those three
hypotheses. Supposing that only the `PLANESTRESS` has a specialized
data (to introduce the axial strain as a state variable for example),
the `getDistinctModellingHypotheses` would return a list containing
`UNDEFINEDHYPOTHESIS` and `PLANESTRESS`.

Note that the `getDistinctModellingHypotheses` has no real interest
for the end user. It is however one of the most used method when
creating interfaces.

There are other methods related to modelling hypotheses:

- `areModellingHypothesesDefined` is a method used internally to see
  if the user has used one of the `@ModellingHypothesis` or
  `@ModellingHypotheses` keyword. This method shall always return
  `True` at the end of the file analysis.
- `isModellingHypothesisSupported` returns `True` if a modelling
  hypothesis is supported.
- `setModellingHypotheses` allows the definition of the supported
  modelling hypotheses. This can be used only when constructing a
  behaviour description from scratch as the object returned by the
  `getBehaviourDescription` can't be modified.

## The `BehaviourData` class

The `getBehaviourData` method of the `BehaviourDescription` class
returns all the data related to a specific modelling hypothesis.

### Variables. The `VariableDescriptionContainer` class and the `VariableDescription` class

The `BehaviourData` class gives access to all the variables defined by
the mechanical behaviour for the considered hypothesis. The following
methods are available:

- `getMaterialProperties`: returns the list of material
  properties. Those are the material properties from the mechanical
  behaviour point of view. Additional material properties can be added
  by an interface to fulfill their own internal conventions (example
  of the `Cast3M` solver) or to provide additional information to the
  behaviour (for example, when the `@RequireStiffnessTensor` keyword
  has been used).
- `getParameters`: returns the list of parameters
- `getPersistentVariables`: returns the list of variables describing
  the material state. *Those are the variables to consider when
  defining the input of the mechanical behaviour.* This list is the
  concatenation of the state variables' list and the auxiliary state
  variables' list, see below for details. The auxiliary state
  variables are always stored after the state variables
- `getStateVariables`: returns the list of state variables. The state
  variables are persistent variables which are also used as
  integration variables.
- `getAuxiliaryStateVariables`: returns the list of state
  variables. The auxiliary state variables are persistent variables
  which are *not* used as integration variables. This can be variables
  that have been eliminated from the integration procedure to gain
  more numerical performances or post-processing variables.
- `getIntegrationVariables`: return the list of variables used to
  build the behaviour integration. 
- `getExternalStateVariables`: returns the list of external state
  variables (such as the Burn-Up, the grain-size, the humidity,
  etc...). Note that the temperature is not part of this list.
- `getLocalVariables`: returns the list of local variables (private
  data members in the `C++` wording)

All the previous methods return an object of type
`VariableDescriptionContainer` which is barely a container of objects
of type `VariableDescription`.

A `VariableDescription` has the following fields:

- `type`: the type of the variable
- `name`: the name of the variable (in the `MFront` file). This is not
  the external name of the variable, i.e. the name of the variable
  for the "outside". See below for details.
- `description`: the description of the variable, if any.
- `arraySize`: if the `arraySize` member is not equal to 1, the
  variable has been defined as an array.
- `lineNumber`: number at which the variable has been defined in the
  `MFront` file. A value of 0 generally indicates a variable declared
  automatically during the DSL analysis

The `BehaviourData` class also provides the following method to
directly retrieve a variable description using its name:

- `getAuxiliaryStateVariables`
- `getPersistentVariableDescription`
- `getIntegrationVariableDescription`
- `getStateVariableDescription`

The name of those methods is self-explanatory.

### External names

For a given variable, `MFront` introduces the concept of external
names which is used by the interfaces for defining how the mechanical
behaviour shall be called by a solver.

There are two kinds of external names:

- glossary names. 
- entry names

The `BehaviourData` class provide the `getExternalName` method to get
the external name of a variable. This method takes the variable name
as an argument.

One can also use the `getExternalNames` to get a list of external
names associated to a `VariableDescriptionContainer`.

### Other methods

- `isMemberUsedInCodeBlocks`
- `hasAttribute`
- `hasGlossaryName`
- `hasEntryName`
- `getExternalName`
- `getExternalNames`
- `setGlossaryName`
- `isGlossaryNameUsed`
- `setEntryName`
- `isUsedAsEntryName`
- `getVariableNameFromGlossaryNameOrEntryName`

- `isMaterialPropertyName`
- `isLocalVariableName`
- `isPersistentVariableName`
- `isIntegrationVariableName`
- `isIntegrationVariableIncrementName`
- `isStateVariableName`
- `isStateVariableIncrementName`
- `isAuxiliaryStateVariableName`
- `isExternalStateVariableName`
- `isExternalStateVariableIncrementName`
- `isParameterName`
- `isStaticVariableName`

# The `mfront.database` module

The `mfront.database` module wraps the [`TFELMFrontDatabase`
library](tfel-mfront-database.html).

## Example of usage

~~~~{.python}
import mfront.database
db = mfront.database.MFrontDatabase()
r = db.analyseDirectory('/tmp/test-lib/')
epts = db.getEntryPoints(interface = 'generic')
for e in epts:
    print(f"{e.library}: {e.name}")
~~~~

<!-- Local IspellDict: english -->
