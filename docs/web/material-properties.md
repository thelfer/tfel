---
title: Material properties in `MFront`
author: Thomas Helfer, Maxence Wangermez
date: 06/2022
lang: en-EN
geometry:
  - margin=2cm
papersize: a4
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
lstPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
header-includes:
- \usepackage{tcolorbox}
- \newtcolorbox{myquote}{colframe=gray}
- \renewenvironment{quote}{\begin{myquote}}{\end{myquote}}
abstract: |
  Material properties are function of some state variables which describe
  the current thermodynamical state of the material.
  
  Compared to behaviours, which often requires to solve a system of
  ordinary differential equations to describe the evolution of the state
  variables describing the material, material properties are conceptually
  much simplier. However, their implementations, while straightforward,
  allow to introduce some key concepts of `MFront`, making a perfect
  introduction for beginners.
  
  This tutorial considers the example of the Young's modulus of uranium
  dioxide, which can be represented, according to Martin
  [@martin_elastic_1989], by the following expression:
  \[
  E(T,f)=2.2693\,10^{11}\,(1-2.5\,f)\,(1-6.786\,10^{-5}\,T-4.23\,10^{-8}\,T^{2})
  \]
  where \(T\) is the temperature (Kelvin) and \(f\) is the porosity. This
  expression is assumed valid for a temperature in the range
  \([273.15:2610.15]\).
---

<!--
pandoc -f markdown+tex_math_single_backslash -F pandoc-crossref --citeproc material-properties.md --bibliography=bibliography.bib --csl=iso690-numeric-en.csl --highlight-style=tango --toc --toc-depth=3 --number-sections -o material-properties.pdf
-->

# Introduction

Material properties are function of some state variables which describe
the current thermodynamical state of the material.

Compared to behaviours, which often require to solve a system of
ordinary differential equations to describe the evolution of the state
variables describing the material, material properties are conceptually
much simpler. However, their implementations, while straightforward,
allow to introduce some key concepts of `MFront`, making a perfect
introduction for beginners.

This tutorial considers the example of the Young's modulus of uranium
dioxide, which can be represented, according to Martin
[@martin_elastic_1989], by the following expression:
\[
E(T,f)=2.2693\,10^{11}\,(1-2.5\,f)\,(1-6.786\,10^{-5}\,T-4.23\,10^{-8}\,T^{2})
\]{#eq:mfront:material_properties:UO2_YoungModulus}

where \(T\) is the temperature (Kelvin) and \(f\) is the porosity. This
expression is assumed valid for a temperature in the range
\([273.15:2610.15]\).

# First implementation and first tests in `Python`

## Creating a first `MFront` file {#sec:mfront:material_properties:UO2YoungModulus-v0}

Expression @eq:mfront:material_properties:UO2_YoungModulus can be
implemented by the following code:

~~~~{.cpp}
@DSL MaterialLaw;
@Law YoungModulus;
@Input T, f;
@Function {
  res = 2.2693e11 * (1 - 2.5 * f) * (1 - 6.786e-05 * T - 4.23e-08 * T * T);
}
~~~~

This code is assumed to be in a standard text file called
`YoungModulus.mfront` which can be edited using any text editor.

> **About editing `MFront` files**
>
> Most text editors have special modes for `C++` highlighting and we
> strongly recommand to associate files with the extension `.mfront` with `C++` mode.
>
> Note that an experimental text editor called `tfel-editor` with specific
> support for `MFront` is also available here: 
> <https://github.com/thelfer/tfel-editor>

## Compilation of the `MFront` file as a `python` module {#sec:mfront:material_properties:compiling}

Assuming that `MFront` was compiled with support for the `python`
interface, the `YoungModulus.mfront` can now be used to generate a
`python` module in two steps:

1. Generate sources files from the `MFront` file. This is done by the
   following call to the `mfront` executable:

   ~~~~{.bash}
   $ mfront --interface=python YoungModulus.mfront
   ~~~~

   `MFront` will generally generate two sub-directory named  respectively
   `include` and `src`.
2. Compile the generated sources in shared libraries (here a single 
   `python` module):

   ~~~~{.bash}
   $ mfront --obuild
   Treating target : all
   The following library has been built :
   - materiallaw.so :  YoungModulus
   ~~~~

   Note that the `materiallaw` shared library is compiled in the `src`
   directory. Its name may differ depending on the operating system
   considered (typically `MacOS`, `Linux` or `Windows`).

   Note also that `obuild` stands for "optimised build", i.e. `MFront`
   automatically specifies compiler flags selected for performances.

Those two steps can be done with this single instruction:

~~~~{.bash}
$ mfront --obuild --interface=python YoungModulus.mfront
~~~~

At any of those two steps, errors may happen. Section
@sec:mfront:material_properties:compiling_errors provides a brief
overview on how to handle compiling errors.

## Testing

At this stage, a shared library called `materiallaw.so` shall have been
generated by `MFront` in the `src` directory. This shared library is a
module directly usable in `python` as follows:

~~~~{.python}
import materiallaw
import numpy as np 
from matplotlib import pyplot as plt 

T = np.linspace(400, 1600) 
E = np.array([materiallaw.YoungModulus(Ti, 0.1) for Ti in T])
plt.xlabel("Temperature (K)") 
plt.ylabel("Young's modulus (Pa)") 
plt.plot(T, E) 
plt.show()
~~~~

![Plot of the material property using
`MatPlotlib`](img/material-property-tutorial-figure1.png "Result of the
evaluation of the material
property"){#fig:mfront:material_properties:YoungModulus width=75%}

The result of the previous script is shown on Figure
@fig:mfront:material_properties:YoungModulus.

## Description of the `MFront` implementation

Listing in Section @sec:mfront:material_properties:UO2YoungModulus-v0
shows that an `MFront` file contains special keywords starting with the
at sign `@`.

### The `@DSL` keyword

The first line, starting with the `@DSL` keyword, defines the domain
specific language used by `MFront` to interpret the file. Domain
specific languages define the kind of material knowledge treated by the
file (material property, behaviour, model) and also, for behaviours, a
class of numerical algorithms used to integrate the constitutive
equations over a time step. Each DSL has its own conventions and defines
its own keywords (however, many keywords are common to several DSLs).

> **About domain specific languages and getting help**
> 
> The list of DSLs, as well as a small description of each DSL, can be
> retrieved as follows:
>
> ~~~~{.bash}
> $ mfront --list-dsl
> available dsl: 
> - DefaultCZMDSL                 : this parser is the most generic one as it
> does not make any restriction on the behaviour or the integration method that
> may be used.
> .... 
> ~~~~
> The list of keywords associated with a DSL can be requested as follows:
>
> ~~~~{.bash}
> $ mfront --help-keywords=<DSLName>
> ~~~~
>
> where `DSLName` is the name of the DSL. For example, the list of keywords
> defined by the `MaterialLaw` DSL used in this tutorial is given by:
>
> ~~~~{.bash}
> $ mfront --help-keywords-list=MaterialLaw
> - @Author         (documented)
> - @Bounds         (documented)
> - @Constant       (documented)
> - @DSL            (documented)
> - ...
> ~~~~
>
> Finally, the documentation of a keyword can be displayed by:
>
> ~~~~{.bash}
> $ mfront --help-keyword=MaterialLaw:@Input
> The `@Input` keyword specifies one or several inputs of a material
> law. This keyword is followed by the names of the inputs, separated by
> commas.
> ....
> ~~~~

### The `@Law` keyword

The second line defines the name of the material property. For this
first implementation, the very generic and unspecific name
`YoungModulus` is used. 

This name can be replaced by a more explicit
name as `YoungModulus` is a poor name for the material law.

Some restrictions apply to the name of the law, see the documentation of
the `@Law` keyword for details.

A good practice for the choice of a more explicit name for the material property is
to add a descriptive and explicit identifier. For example, one may add a
reference to the original paper as follows:

~~~~{.cxx}
@Law YoungModulus_Martin1989; // name of the material property
~~~~

Note that the generated name of the function also changes, so the
previous test case must be adapted. This is stated by the ouput of
`MFront`:

~~~~{.bash}
$ mfront --obuild --interface=python YoungModulus_Martin1989.mfront 
Treating target : all
The following libraries have been built :
- materiallaw.so :  YoungModulus YoungModulus_Martin1989
~~~~

Note that the previous implementation is still present. The user may
want to remove the `src` and `include` directories to avoid conflicts
and avoid using the previous implementation by mistake.

Note that we also recommend to change the name of the `MFront` file to
reflect the name of the material law, although this is not required by
`MFront`.

### The `@Input` keyword

The third line defines the two input variables of the material property,
named respectively `T` and `f`.

### The `@Function` keyword

The implementation of Expression
@eq:mfront:material_properties:UO2_YoungModulus is done in a code block
introduced by the `@Function` keyword. By default, this code block must
store the result of the material property in a variable called `res`.

It is worth noting that this implementation here is reasonably close to Equation @eq:mfront:material_properties:UO2_YoungModulus.

The code introduced by `@Function` is standard `C++` with a few implicit
additions, such as:

- the declaration of some `MFront` types, such as `real`, `stress`,
  `temperature`, etc. (see Section
  @sec:mfront:material_properties:types),
- the declaration of external material properties (not discussed in this
  tutorial, see the documentation of the `@MaterialLaw` keyword for
  details),
- the fact that every function of the standard library is automatically
  used (i.e. the line `using namespace std;` is inserted before the code
  block).

Most of the time, only a very limited subset of the `C++` language is
required to use `MFront`.

Note that the `cmath` header, which contains the declaration of all the
standard mathematical functions, is automatically included. Usage of
external libraries, although not discussed in this tutorial, is possible
using the `@Includes` and `@Library` keywords.

> **On the usage of `C++` exceptions**
>
> In most interfaces (except the `C++` interface notably), the code block
> is also placed inside a `try/catch` block to avoid the propagation of
> `C++` exceptions in languages and solvers that can't handle them.
> 
> Interfaces are responsible for translating `C++` exceptions into errors
> in an appropriate way if possible. For example, most interfaces will
> handle exceptions derived from `std::exception` specifically and
> use the `what` method to retrieve the error message.

# Improvements

At this stage, the `MFront`'s implementation is barely working. This
section is devoted to the improvements that must be considered to have a
production-ready implementation.

## Associating the material property to a material

At this stage, nothing indicates that the material property describes
uranium dioxide. This can be done using the `@Material` keyword as
follows:

~~~~{.cxx}
@Material UO2; // material name
~~~~

Associating a material name to the material property has two effects:

- it changes the name of the generated function,
- it changes the name of the library generated.

This can be readily seen on the output of `MFront`:

~~~~{.cxx}
$ mfront --obuild --interface=python UO2_YoungModulus_Martin1989.mfront 
Treating target : all
The following libraries have been built :
- uo2.so :  UO2_YoungModulus_Martin1989
~~~~

At this stage, a shared library called `uo2.so` shall have been
generated by `MFront` in the `src` directory and shall be directly
usable in `python` as follows:

~~~~{.python}
from uo2 import UO2_YoungModulus_Martini1989 as UO2YoungModulus
import numpy as np 
from matplotlib import pyplot as plt 

T = np.linspace(400, 1600) 
E = np.array([UO2YoungModulus(Ti, 0.1) for Ti in T])
plt.xlabel("Temperature (K)") 
plt.ylabel("Young's modulus (Pa)") 
plt.plot(T, E) 
plt.show()
~~~~

> **The `@Library` keyword**
>
> The `@Library` provides more control on the name of the generated
> shared library.
>
> However, we do not recommend to use this keyword in the
> `MFront` source. This keyword can be specified on the command line
> as follows:
>
> ~~~~{.bash}
> $ mfront --@Library=UraniumDioxide --obuild --interface=python \
>   UO2_YoungModulus_Martin1989.mfront
> Treating target : all
> The following libraries have been built :
> - uraniumdioxide.so :  UO2_YoungModulus_Martin1989
> ~~~~

## Changing the name of the output

`res` is a poor name for the ouput variable. It can be changed using the
`@Ouput` keyword as follows:

~~~~{.cxx}
@Output E;
~~~~

As a consequence, the name body of the function must be changed to
reflect the change in the name of the output:

~~~~{.cxx}
@Function {
  E = 2.2693e11 * (1 - 2.5 * f) * (1 - 6.786e-05 * T - 4.23e-08 * T * T);
}
~~~~

## Physical bounds and standard bounds

### Physical bounds

At this stage, the function barely works, but can be used very
inappropriately. For example, one may request the value of the Young's
modulus for a negative temperature, something which in our opinion shall
lead to an error to indicate to the user that something very wrong is
happening their code.

Indeed, many quantities are intrinsically bounded. In our example, the
temperature (in Kelvin) can't be negative and the porosity shall be
bounded in the range \([0:1]\). Those bounds are depicted as
*physical bounds* in `MFront`.

Such bounds can be introduced using the `@PhysicalBounds` keyword as
follows:

~~~~{.cxx}
@PhysicalBounds T in [0:*[;
@PhysicalBounds f in [0:1];
~~~~

where the `*` character means infinity (and is followed by an
open-bracket).

A violation of the physical bounds is __always__ a cause for the calculation stop and the return of an error.

### Standard bounds

Another issue is the fact that the considered material property is only
reliable on a limited range of temperature and porosity. Outside of this
domain of validity, the prediction may become inaccurate.

This domain of validity is described in `MFront` by (standard) bounds,
introduced by the `@Bounds` keyword.

In our example, the prediction are considered reliable for temperatures
in the range \([273.15:2610.15]\):

~~~~{.cxx}
@Bounds T in [273.15:2610.15]; // Validity range
~~~~

`MFront` defines three policies to let the solver choose how to treat
an out-of-bounds evaluation:

- `None`: nothing is done,
- `Warning`: the user shall be informed but the computation is performed
  as usual. How the user is informed depends on the interface
  considered,
- `Strict`: the computation are stopped and an error is reported. The
  mechanism used to stop the computation depends on the interface
  considered.

`MFront` automatically selects the `None` policy by default. 

#### Changing the default policy with DSL options -

Since Version 4.1, this behaviour can now be changed by using the
`default_out_of_bounds_policy` string option of the `MaterialLaw` DSL as
follows:

~~~~{.cxx}
@DSL MaterialLaw {
  default_out_of_bounds_policy: "Strict"
};
~~~~

> **List of available DSL options**
>
> DSL options are a powerful tool to modify the default behaviour of
> `MFront`. The list of options associated with a DSL, as well as a
> short description of each option, can be retrieved
> using the `--list-dsl-options` command line argument, as follows:
> 
> ~~~~{.bash}
> $ mfront --list-dsl-options=MaterialLaw
> - default_out_of_bounds_policy  : string specifying the default out
>   of bounds policy. Allowed values are `None`, `Warning`, `Strict`.
> - out_of_bounds_policy_runtime_modification: boolean stating if the
>   runtime modification of the out of bounds policy is allowed.
> - parameters_as_static_variables: boolean stating if parameters
>   shall be treated as static variables.
> - parameters_initialization_from_file: boolean stating if the values
>   of parameters can be changed from an external file.
> - build_identifier              : string specifying a build identifier.
>   This option shall only be specified on the command line.
> ~~~~

We do not recommend to hard coding DSL options in the source file as
`MFront` files must be sharable across different users which may have
different needs.

#### Changing the default policy with command line -

`DSL` options can be specified on the command line as follows:

~~~~{.bash}
$ mfront --obuild --interface=python                   \
  --dsl-option=default_out_of_bounds_policy:'"Strict"' \
  UO2_YoungModulus_Martin1989.mfront
~~~~

Note that a double-quote is required here to pass a string option.

> **Statement of need**
> 
> DSL options have been introduced to customize `MFront`.
> While most users may want to control the out-of-bounds policy
> at runtime, this may not be compatible with quality assurance
> in safety critical studies.
>
> Consider a nuclear fuel performance code which delivers a set of
> well-qualified material properties to its users. The developpers
> of the fuel performance code must guarantee the proper the usage
> of their code. In particular, the material properties must be
> used inside their validity domain. A `Strict` policy may be
> a convenient choice.
>
> Users shall thus not be able to use the code outside
> its validation domain without explicit stating it in their input
> files using a specific keyword. In this case, the developpers of
> the fuel performance code are no more responsible of the results
> obtained and may explicitly state it in the result files by
> a warning (for example).

### Testing in `python`

After specifying the physical bounds of our variables, the
implementation is as follows:

~~~~{.cxx}
@DSL MaterialLaw;
@Law YoungModulus;
@Output E;
@Input T, f;
@PhysicalBounds T in [0:*[;    // Temperature is positive
@PhysicalBounds f in [0:1.];   // Porosity is positive and lower than one
@Bounds T in [273.15:2610.15]; // Validity range
@Function {
  E = 2.2693e11 * (1 - 2.5 * f) * (1 - 6.786e-05 * T - 4.23e-08 * T * T);
}
~~~~

After recompiling the `python` module, we may try to evaluate the Young's
modulus for a negative temperature. The output is the following:

~~~~{.python}
>>> import uo2
>>> print(uo2.YoungModulus_Martin1989(-1, 0.1))
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
RuntimeError: YoungModulus : T is below its physical lower bound (-1<0).
~~~~

In `python`, violation of physical bounds is reported using the standard
exception mechanism of `Python`. More precisely, an exception of the
`RuntimeError` type is thrown.

With the default policy, an out-of-bounds evaluation does not trigger
any specific behaviour:

~~~~{.python}
>>> print(uo2.YoungModulus_Martin1989(3000, 0.1))
70754504700.0
~~~~

#### Changing the out-of-bounds policy -

By default, the out-of-bounds policy can be changed using the
environment variable `PYTHON_OUT_OF_BOUNDS_POLICY` as follows:

~~~~{.python}
>>> import os
>>> os.environ['PYTHON_OUT_OF_BOUNDS_POLICY'] = 'WARNING'
>>> print(uo2.YoungModulus_Martin1989(3000, 0.1))
YoungModulus : T is over its upper bound (3000>2610.15).
70754504700.0
~~~~

Here, one sees that a warning is displayed in the terminal on the
standard error stream.

If the out-of-bounds policy is set to `STRICT`, violation of standard
bounds is treated as the violation of physical bounds:

~~~~{.python}
>>> os.environ['PYTHON_OUT_OF_BOUNDS_POLICY'] = 'STRICT'
>>> print(uo2.YoungModulus_Martin1989(3000, 0.1))
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
RuntimeError: YoungModulus : T is over its upper bound (3000>2610.15).
~~~~

#### Disabling the runtime modification of the out-of-bounds policy -

Since Version 4.1, it is possible to use the `out_of_bounds_policy_runtime_modification` DSL option boolean to enable or disable the runtime
modification of the out-of-bounds policy.

If this option is set to `false`, the environment variable
`PYTHON_OUT_OF_BOUNDS_POLICY` will have any effect.

## Documenting the variables for the calling solvers

The names of the inputs variables are very generic and not
self-explanatory. For example, nothing indicates to the reader that `f`
stands for the porosity of the material. More precisely, `MFront`
exports information about the material property that can be used by the
calling solver. Among others, these information include the name of the
inputs and the name of the output, as well as description of those
variables.

To emphasize this point, let us consider a handy tool called
[`mfront-query`](mfront-query.html) which can be used to request all
kind of information about an `MFront` file from the command line. Using
`mfront-query` to list the inputs of this implementation leads to this
disappointing and unuseful result:

~~~~{.bash}
$ mfront-query --inputs YoungModulus.mfront 
- T
- f
~~~~

While `MFront`'s users hardly ever use `mfront-query` directly, its
mention here is meant to highlight the fact that an `MFront` file (or
the shared library generated thanks to this `MFront` file, see below)
generally must be self-descriptive and provide meaningful information to
the solver that will use it.

### About using explicit variable names

A first idea to solve this issue would be to used explicit variable
names such as `Temperature` or `Porosity`, instead of `T` and `f`
respectively. However, this solution rapidely becomes cumbersome in
practice. We do not recommend this solution.

### About standard C++ comments

Another idea would be to highlight the meaning of those variables using
`C++` comments as follows:

~~~~{.cxx}
// T stands for the temperature
@Input T;
// f stands for the porosity
@Input f;
~~~~

However, those comments are not associated to the variables by `MFront`,
so the output of `mfront-query` is unchanged.

### `doxygen`-like comments

`MFront` supports [`doxygen`-like](https://www.doxygen.nl/index.html)
comments to document variables, as follows:

~~~~{.cxx}
//! T stands for the temperature
@Input T;
//! f stands for the porosity
@Input f;
~~~~

In this case, the output of `mfront-query` is a bit more informative:

~~~~{.bash}
$ mfront-query --inputs YoungModulus.mfront 
- T: T stands for the temperature
- f: f stands for the porosity
~~~~

### The `TFEL` glossary and the `setGlossaryName` method

Still, all the previous solutions lack standardisation: all
implementations using the temperature shall state explicitly and
unambiguously that they are using this quantity.

The [`TFEL` glossary](glossary.html) defines a set of uniquely defined
names that can be used to qualify a variable. A variable can be
associated with a glossary name using the `setGlossaryName` as follows:

~~~~{.cxx}
@Input T;
T.setGlossaryName("Temperature");
@Input f;
f.setGlossaryName("Porosity");
~~~~

The glossary name automatically defines the **external name** of the
variable and automatically adds an appropriate documentation. The output
of `mfront-query` is now:

~~~~{.cxx}
$ mfront-query --inputs YoungModulus.mfront 
- Temperature (T): The temperature
- Porosity (f): Porosity of the material
~~~~

> **Variables not covered by the `TFEL` glossary and the `setEntryName` method**
>
> The `TFEL` glossary is finite. If a variable is not described by a
> glossary entry, we highly recommend to give it an explicit external name
> using the `setEntryName` and to document it using `doxygen`-like
> comments.

## Parameters

Sensitivity analysis, uncertainties propagations, re-identification,
require to be able to modify the coefficients of the material property.

`MFront` introduces the concept of *parameters*: a parameter is a
variable which has a default value that can be changed at runtime. In
practice, parameters can be seen as global variables. The way to change the
value of a parameter depends on the interface considered.

Parameters are introduced using the `@Parameter` keyword. The
implementation can be turned into a more explicit version as follows:

~~~~{.cxx}
@Parameter E0 = 2.2693e11;
@Parameter dE_dT =  -1.53994698e7;
@Parameter d2E_dT2 = -1.9198278e4;
@Parameter f0 = 0.4;

@Function {
  E = (1 - f / f0) * (E0 + dE_dT * T + (d2E_dT2 / 2) * T * T);
}
~~~~

### Modification of parameters

#### Using external txt files -

Parameters can usually be changed using an external txt file,
although this may depend on the interface considered.

Since Version 4.1, the name of expected parameters file can be retrieved
using `mfront-query` as follows:

~~~~{.bash}
$ mfront-query --parameters-file UO2_YoungModulus_Martin1989.mfront
UO2_YoungModulus_Martin1989-parameters.txt
~~~~

This file has the following structure:

~~~~{.bash}
<parameter name> <new parameter value>
~~~~

For example, the file `UO2_YoungModulus_Martin1989-parameters.txt` may
looks like:

~~~~{.bash}
# new material parameters
E0 2.2693e11
dE_dT  -1.53994698e7
d2E_dT2 -1.9198278e4
f0 0.4
~~~~

This file may contain commented lines starting with the `#` character.

Note that this file is read, if present in the current working
directory, only at the very first evaluation of the material property.
As a consequence, changing this file after this first evaluation
will not have any effect.

#### Using the `ExternalLibraryManager` -

The `ExternalLibraryManager` class, provided by the `TFEL/System`
library provides the `setParameter` method which can be used to modify
the parameters dynamically.

This method expects that the interface exports a dedicated function.
While this is the case for most interfaces, it has only been introduced
in the `python` interface in Version 4.1.

This method can be used as follows:

~~~~{.python}
>>> from uo2 import UO2_YoungModulus_Martin1989
>>> print(UO2_YoungModulus_Martin1989(800, 0.1))
156350208000.0
>>> from tfel.system import ExternalLibraryManager
>>> elm = ExternalLibraryManager.getExternalLibraryManager()
>>> elm.setParameter('uo2.so', 'UO2_YoungModulus_Martin1989', 'E0', 2e11)
>>> print(UO2_YoungModulus_Martin1989(800, 0.1))
136152708000.0
~~~~

#### Disabling the modification of parameter from external txt file -

Since Version 4.1, initializing parameters from an external file can be
disabled using the `parameters_initialization_from_file` boolean DSL
option:

~~~~{.bash}
$ mfront --obuild --interface=python                            \
         --dsl-option=parameters_initialization_from_file:false \
         UO2_YoungModulus_Martini1989.mfront
~~~~

This option has mostly been introduced to let solver developpers have
control on the users ability to modify parameters of well-qualified
material properties: if such parameter is modified, strict assurance
quality of the solver is compromised.
 
#### Disabling the runtime modification of parameters -

Since Version 4.1, it is possible to use the boolean DSL option
`parameters_as_static_variables` in order to modify the way parameters are
processed:

~~~~{.bash}
$ mfront --obuild --interface=python                      \
         --dsl-option=parameters_as_static_variables:true \
         UO2_YoungModulus_Martini1989.mfront
~~~~

When treated as static variables, parameters can not be modified at
runtime (neither using an external txt file nor using the `setParameter`
function of the `ExternalLibraryManager` class).

### Effect on the runtime performances

Modification of parameters may have runtime performances since a global
variable is used to store the values of the parameters.


## Other metadata for assurance quality

Metadata describing the author of the implementation, the date of the
implementation and a short description can be added by the `@Author`,
`@Date` and `@Description` keywords respectively.


For example, those information can be added as follows:

~~~~{.cxx}
@Author   T. Helfer;  // author name
@Date     04/04/2014; // implentation date
@Description          // detailled description
{
  The elastic constants of polycrystalline UO2 and
  (U, Pu) mixed oxides: a review and recommendations
  Martin, DG
  High Temperatures. High Pressures, 1989
};
~~~~

Those information will be exported by `MFront` and can be retrieved by
the calling solver.

## Explicit coding: add types and quantity support {#sec:mfront:material_properties:types}

### The `real` type alias

By default, the input variables and the output variable will be defined
as standard floating-point values with single precision (i.e. the
`float` type in `C++`), double precision (`double` type in `C++`) or
extended precision (`long double` type in `C++`) depending on the
interface selected.

`MFront` always defines a type alias named `real` which corresponds to
the floating point type effectively used.

We strongly encourage `MFront` users to never use directly any of the
`C++` standard types and use the `real` type alias instead.

### Default variable type in the `MaterialLaw` DSL

By default, variables in the `MaterialLaw` DSL have the `real` type.
Hence the declaration:

~~~~{.cxx}
@Input f;
~~~~

is equivalent to:

~~~~{.cxx}
@Input real f;
~~~~

### Explicit type aliases

`MFront` defines a set of type aliases, listed on [this
page](mfront-types.html), such as `temperature`, `strain`, `stress`,
etc.

Hence, the temperature could be defined as follows:

~~~~{.cxx}
@Input temperature T;
~~~~

### Using quantities

By default, `temperature`, `strain`, `stress` are defined as type
aliases to floating point number type used by the interface.

The keyword `@UseQT` allows the usage of quantities, i.e. floatting
point values associated with units. Quantities allow the compiler to
perform *dimensional analysis* at compile-time.

It must clearly emphasized that quantities are only used *inside* the
body of the function and does not change the interface of the generated
functions.

Note that quantities are implemented by the `TFEL/Math` library and
supported in `MFront` only since Version 4.0.

#### Error generated by invalid operations on quantities -

The following code will not compile if quantities are set:

~~~~{.cxx}
const auto a = strain{1e-8};
const auto b = stress{10e6};
const auto c = a + b;
~~~~

For instance, the `gcc` compiler generates the following error:

~~~~{.bash}
TFEL/Math/Quantity/qtOperations.hxx:302:19: error: static assertion failed:
invalid operation
    static_assert(std::is_same_v<UnitType, UnitType2>, "invalid operation");
~~~~

#### The `derivative_type` metafunction -

Defining the type of the parameters `E0` and `f0` is straightforward:

~~~~{.cxx}
@Parameter stress E0 = 2.2693e11;
@Parameter real f0 = 0.4;
~~~~

Defining the type of the parameters `dE_dT` and `d2E_dT2` is a bit more
involved. The `derivative_type` metafunction allows to build their types
as follows:

~~~~{.cxx}
@Parameter derivative_type<stress, temperature> dE_dT =  -1.53994698e7;
@Parameter derivative_type<derivative_type<stress, temperature>, temperature>
           d2E_dT2 = -1.9198278e4;
~~~~

In Version 4.1, the definition of `d2E_dT2` can be simplified as follows:

~~~~{.cxx}
@Parameter derivative_type<stress, temperature, temperature> d2E_dT2 = -1.9198278e4;
~~~~

#### Impact on runtime performances -

Quantities have a compile-time overhead as:

- the compiler must perform the dimensional analysis to check if every
  operation is valid,
- quantities have been implemented to have a negligible runtime
  overhead, if any (see comment below), at least in optimised builds.
  This requires that the compiler does extra work, such as function
  inlining and intermediate variable elimination.


The following listings show the exact same
assemblies resulting from the compilation of the code generated by the `c`
interface with (right) and without (left) the usage of quantities.

~~~~{.nasm}
UO2_YoungModulus_Martin1989:                  | UO2_YoungModulus_Martin1989:
.LFB4378:                                     |.LFB4714:
	.cfi_startproc                            |	.cfi_startproc
	vmulsd	.LC0(%rip), %xmm0, %xmm2          |	vmulsd	.LC0(%rip), %xmm0, %xmm2 
	vmovsd	.LC1(%rip), %xmm3                 |	vmovsd	.LC1(%rip), %xmm3 
	vdivsd	.LC3(%rip), %xmm1, %xmm1          |	vdivsd	.LC3(%rip), %xmm1, %xmm1 
	vfmadd213sd	.LC2(%rip), %xmm0, %xmm3      | 	vfmadd213sd	.LC2(%rip), %xmm0, %xmm3
	vfmadd132sd	%xmm2, %xmm3, %xmm0           |	vfmadd132sd	%xmm2, %xmm3, %xmm0 
	vmovsd	.LC4(%rip), %xmm2                 |	vmovsd	.LC4(%rip), %xmm2 
	vsubsd	%xmm1, %xmm2, %xmm2               |	vsubsd	%xmm1, %xmm2, %xmm2 
	vmulsd	%xmm2, %xmm0, %xmm0               |	vmulsd	%xmm2, %xmm0, %xmm0
	vmovq	%xmm0, %rax                       |	vmovq	%xmm0, %rax 
	shrq	$52, %rax                         |	shrq	$52, %rax 
	andl	$2047, %eax                       |	andl	$2047, %eax
	cmpl	$2047, %eax                       |	cmpl	$2047, %eax
	je	.L4                                   |	je	.L4
	ret                                       |	ret
	.p2align 4,,10                            |	.p2align 4,,10
	.p2align 3                                |	.p2align 3 
~~~~

In this case, the compiler was able to generate exactly the same
assembly in both cases. Hence, in this simple case, usage of quantities
does not have any runtime overhead.

# Advanced topics

## `mfront-query`

`mfront-query` is a convenient tool to analyse an `MFront` source file.

The list of queries for material properties can be displayed using the
`--help-material-property-queries-list` command line argument, as
follows:

~~~~{.bash}
$ mfront-query --help-material-property-queries-list
Usage: mfront-query [options] [files]

Available options are : 
--class-name                    : show the class name
--date                          : show the file implementation date
--description                   : show the file description
--generated-headers             : show all the generated headers
--generated-sources             : show all the generated sources
--inputs                        : show the list of inputs
--interface                     : define an interface
--law-name                      : show the law name
....
~~~~

For example, the following query shows the list of `C++` files generated
when using the `python` interface:

~~~~{.bash}
$ mfront-query --interface=python \
               --generated-sources UO2_YoungModulus_Martin1989.mfront 
uo2 : UO2_YoungModulus_Martin1989-python.cxx UO2lawwrapper.cxx
$ # do not sort by library
$ mfront-query --interface=python \
               --generated-sources=unsorted UO2_YoungModulus_Martin1989.mfront 
UO2_YoungModulus_Martin1989-python.cxx UO2lawwrapper.cxx 
~~~~

`mfront-query` is the basis for advanced compilation projects such as
[`MFrontGallery`](https://thelfer.github.io/MFrontGallery/web/index.html).

## The `tfel::system::ExternalMaterialPropertyDescription` class

The `TFEL/System` provides the `ExternalMaterialPropertyDescription` class
which can be used to retrieve numerous information about a material
property contained in a shared library. This class has been wrapped in
`python`.

The `ExternalMaterialPropertyDescription` is a convenient layer around the
`ExternalLibraryManager` which is more flexible but also more
difficult to use directly. It allows to retrieve various other
information, such as:

- most metadata exported by `MFront` (list of arguments and parameters,
  author, date, description, etc..),
- the standard and physical bounds of the inputs (arguments) of a
  material property,
- the default values of the parameters of a material property.

### Example of usage

~~~~{.python}
>>> from tfel.system import ExternalMaterialPropertyDescription
>>> d = ExternalMaterialPropertyDescription('uo2.so',
                                            'UO2_YoungModulus_Martin1989') 
>>> print(d.author)
T . Helfer
>>> print(d.description)
The elastic constants of polycrystalline UO2 and
( U , Pu ) mixed oxides : a review and recommendations
Martin , DG
High Temperatures . High Pressures , 1989
>>> print(d.arguments)
['Temperature', 'Porosity']
>>> print(d.parameters)
['E0', 'dE_dT', 'd2E_dT2']
>>> if (d.hasBounds('Temperature')):
...     if (d.hasLowerBound('Temperature')):
...         print(d.getLowerBound('Temperature'))
...     if (d.hasUpperBound('Temperature')):
...         print(d.getUpperBound('Temperature'))
... 
273.15
2610.15
~~~~

## The `mtest::MaterialProperty` class

The `mtest::MaterialProperty` class allows to load a material property
generated using one of the following interfaces: `castem`, `cyrano` and
`generic`.

This class as been wrapped in `python`, as described on [this
page](mtest-python.html).

This class has the following core interface:

- `getVariablesNames`, which returns the names of the arguments of
  the material property,
- `setVariableValue` which allows to set the value of an argument of the
  material property,
- `getValue` which evaluates the material property for the values of the
  arguments set by the `setVariableValue` method,
- `getParametersNames`, which returns the names of the parameters of
  material property,
- `setParameter` which allows to change the value of a parameter.

As described in the next paragraph, several convenient methods and
operators are also provided to simplify the usage of this class.

### Usage

Here is an example of the usage of the `MaterialProperty` class in
`python`.

~~~~{.python}
import mtest
young_modulus = mtest.MaterialProperty(
    'src/libCastemUO2.so', 'UO2_YoungModulus_Martin1989')
~~~~

Note that the constructor of `MaterialProperty` automatically detects the interface used to generate the material property and instantiates the correct implementation internally. 

In this example, an instantiation
of the `CastemMaterialProperty` class is created.
The arguments of the material property can then be set and the material
property can be evaluated:

~~~~{.python}
young_modulus.setVariableValue('Temperature', 562)
young_modulus.setVariableValue('Porosity', 0.1)
E = young_modulus.getValue()
~~~~

Setting the variables' values and evaluating the material property can
be tedious. To avoid this, overloaded versions of the `getValue` are
available:

~~~~{.python}
E = young_modulus.getValue({'Temperature': 562, 'Porosity': 0.1})
~~~~

To make the code even more explicit, the call operator can also be used,
as follows:

~~~~{.python}
E = young_modulus({'Temperature': 562, 'Porosity': 0.1})
~~~~

## The `mfm` utility

Once a shared library has been generated by `MFront`, it is convenient
to be able to analyse its content rapidly. This can be done in `python`
and `C++` using the `tfel::system::ExternalLibraryManager` class.

The `mfm` utility is a small command-line program built on top of the
`ExternalLibraryManager` class. It allows to request all the entry
points generated by `MFront` in shared library, i.e. the list of material
properties, behaviours and models. Those entry points can be filtered by
name and material using regular expressions.


### Example of usage

~~~~{.bash}
$ mfm --filter-by-type=material_property --filter-by-name='.+Young.+' src/uo2.so 
- UO2_YoungModulus_Martin1989
~~~~

<!--
# Conclusions

In this tutorial, several key concepts of `MFront` are discussed using an example of a material property: the Young's modulus of uranium dioxide.

With increasing difficulty, this tutorial leads the user to write a first `MFront` file, to implement it and to use the resulting libraries. 

Moreover, different implementation or compilation options are described as well as good practices in terms of using the material properties of the `MFront` software.

\appendix
-->

# Appendices

## About the analysis of errors generated by `MFront` or the `C++` compiler {#sec:mfront:material_properties:compiling_errors}

As described in Section @sec:mfront:material_properties:compiling, the
`MFront` file is first parsed, interperted by `MFront` and used to
generate a set of `C++` source files. In a second step, those `C++`
source files are compiled by a `C++` compiler into a shared library.

Each step may lead to errors, as described in the following paragraphs.

### Example of an error generated by `MFront`

#### First example

Consider the following incorrect code which declares the `T` variable
twice:

~~~~{.cxx}
@Input T, T;
~~~~

Compiling a file with this error leads to the following message:

~~~~{.bash}
$ mfront --interface=python YoungModulus.mfront 
Error while treating file 'UO2YoungModulus.mfront'
MaterialPropertyDSL::analyse: error while treating keyword '@Input'.
MaterialPropertyDescription::reserveName: name 'T' already reserved
Error at line 4
~~~~

#### Second example

If the `@Function` is not used, the following error is generated:

~~~~{.bash}
$ mfront --interface=python YoungModulus.mfront 
Error while treating file 'UO2YoungModulus-test.mfront'
MaterialPropertyDSL::generateOutputFiles: no function defined.
~~~~

#### Third example

If the `@Function` body is empty, the following error is generated:

~~~~{.bash}
$ mfront --interface=python YoungModulus.mfront 
Error while treating file 'UO2YoungModulus-test.mfront'
MaterialPropertyDSL::treatFunction: function is empty.
Error at line 5
~~~~

#### Fourth example

If the name of the material property is not defined, the following error
is generated:

~~~~{.bash}
$ mfront --interface=python YoungModulus.mfront 
MaterialPropertyDSL::generateOutputFiles: no material property name defined.
~~~~

#### Fifth example

If the name of the material property is ill formed (for example
`@YoungModulus`), the following error is generated:

~~~~{.bash}
$ mfront --interface=python YoungModulus.mfront 
MaterialPropertyDSL::treatLaw: '@YoungModulus' is not a valid law name
Error at line 3
~~~~

The rules defining a valid law name are given in the help of the `@Law`
keyword:

~~~~{.bash}
$ mfront --help-keyword=MaterialLaw:@Law
The `@Law` keyword allows the user to associate a name to the material
law being treated. This keyword is followed by a name.

This name must be a valid C++ identifier. The following characters are
legal as the first character of an identifier, or any subsequent
character:

`_` `a` `b` `c` `d` `e` `f` `g` `h` `i` `j` `k` `l` `m`
`n` `o` `p` `q` `r` `s` `t` `u` `v` `w` `x` `y` `z`
`A` `B` `C` `D` `E` `F` `G` `H` `I` `J` `K` `L` `M`
`N` `O` `P` `Q` `R` `S` `T` `U` `V` `W` `X` `Y` `Z`

The following characters are legal as any character in an identifier
except the first:

`0` `1` `2` `3` `4` `5` `6` `7` `8` `9`

### Name of the generated functions or classes

The names of the generated functions or classes depend on the law name
as specifified with the `@Law` keyword but may also include the
material name, as specified by the `@MaterialLaw` keyword.
~~~~

### Example of an error generated by the `C++` compiler

Assume that one forgets to declare the porosity \(f\). This error will
not detected by `MFront` but by the `C++` compiler as illustrated below:

~~~~{.bash}
$ mfront --interface=python YoungModulus.mfront 
$ mfront --obuild
UO2YoungModulus.mfront: In function ‘PyObject* YoungModulus_wrapper(PyObject*, PyObject*)’:
UO2YoungModulus.mfront:5:31: error: ‘f’ was not declared in this scope
   res = 2.2693e11 * (1 - 2.5 * f) * (1 - 6.786e-05 * T - 4.23e-08 * T * T);
~~~~

Although the message mentions an implementation detail (the name of the
function doing the interface with `python`, i.e.
`YoungModulus_wrapper`), the error clearly mentions the `MFront` source
file and not the generated `C++` files. The syntax
`UO2YoungModulus.mfront:5:31` states that the error has been detected in
the file `UO2YoungModulus.mfront` at Line 5 (column 31).

# References {.unnumbered}

<!-- Local IspellDict: english -->
