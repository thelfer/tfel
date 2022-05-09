---
title: Material properties in `MFront`
author: Thomas Helfer
date: 19/08/2021
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
lstPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

Material properties generally depend on some state variables which
describes the current thermodynamical state of the material.

Compared to behaviours, which often needs to solve a system of ordinary
differential equations, material properties are conceptually much
simplier. However, their implementations, while straightforward, allow
to introduce some key concepts in `MFront`, making a perfect
introduction for beginners.

This tutorial consider the example of the Young' modulus of uranium
dioxide, which can be represented, following @martin_elastic_1989, by
the following expression:
\[
E(T,f)=2.2693\,10^{11}\,(1-2.5\,f)\,(1-6.786\,10^{-5}\,T-4.23\,10^{-8}\,T^{2})
\]{#eq:mfront:material_properties:UO2_YoungModulus}

where \(T\) is the temperature (Kelvin) and \(f\) is the porosity. This
expression is assumed valid for a temperature in the range
\([273.15:2610.15]\).

# First implementation and first tests in `Python`

## Creating a first `MFront` file.

Expression @eq:mfront:material_properties:UO2_YoungModulus can be
implemented by the following code:

~~~~{#lst:mfront:material_properties:UO2YoungModulus-v0 .cpp .numberLines caption="First implementation of the Young' modulus of uranium dioxide" }
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
> strongly to associate file with the extension `.mfront` with `C++` mode.
>
> Note that an experimental text editor called `tfel-editor` with specific
> support for `MFront` is also available here: 
> <https://github.com/thelfer/tfel-editor>

## Description of the `MFront` implementation

Listing @lst:mfront:material_properties:UO2YoungModulus-v0 shows that an
`MFront` file contains special keywords starting with the at sign `@`.

### The `@DSL` keyword

The first line, staring with the `@DSL` keyword, defines the domain
specific language used by `MFront` to interpret the file. Domain
specific languages defines the kind of material knowledge treated by the
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
> $ mfront --list-keywords=<DSLName>
> ~~~~
>
> where `DSLName` is the name of the DSL. For example, the list of keywords
> defined by the `MaterialLaw` DSL using in this tutorial is given by:
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
`YoungModulus` is used. This named will be replaced by a more explicit
name latter in this tutorial.

Some restrictions apply to the name of the law, see the documentation of
the `@Law` keyword for details.

### The `@Input` keyword

The third line defines the two input variables of the material property,
named respectively `T` and `f`.

## The `@Function` keyword

The implementation of Expression
@eq:mfront:material_properties:UO2_YoungModulus is done in a code block
introduced by the `@Function` keyword. By default, this code block must
store the result of the material property in a variable called `res`.

Its is worth emphasing that this implementation is here reasonably
closed to @eq:mfront:material_properties:UO2_YoungModulus.

## Compilation of the `MFront` file as a `python` module

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
   considered (typically `Mac Os`, `LiNuX` or `Windows`).

Those two steps can be done with this single instruction:

~~~~{.bash}
$ mfront --obuild --interface=python YoungModulus.mfront
~~~~

## Testing

# Improvements

## Standard and physical bounds

### Testing in `python`

## About the meaning input and outpu variables.

### Changing the name of the output

### Documenting the variables for the calling solvers

The names of the inputs variables are very generic and not
self-explanatory. For example, nothing indicates to the reader that `f`
stands for the porosity of the material. More precisely, `MFront`
exports information about the material property that can be used by the
calling solver. Among others, these information include the name of the
inputs and the name of the output, as well as description of those
variables.

To emphasize this point, let us consider an handy tool called
[`mfront-query`](mfront-query.html) which can be used to request all
kind of information about an `MFront` file from the command line. Using
`mfront-query` to list the inputs of this implementation leads to this
disappointing and unuseful result:

~~~~{.bash}
$ mfront-query --inputs YoungModulus.mfront 
- T
- f
~~~~

While `MFront`' users hardly ever use `mfront-query` directly, its
mention here is meant to highlight the fact that and `MFront` file (or
the shared library generated thanks to this `MFront` file, see below)
generally must be self-descriptive and provide meaningful information to
the solver that will use it.

#### Variable name and external name

A first idea to solve this issue would be to used explicit variable
names such as `Temperature` or `Porosity`, instead of `T` and `p`
respectively. However, this solution rapidely becomes cumbersome in
practice.

To overcome this issue, `MFront` allows to associate an external name to
a variable. By default, the external name is the same as the variable
name. We will see in the following that this can be done using either
the `setEntryName` method or the `setGlossaryName` method

#### About standard C++ comments

Another idea would be to highlight the meaning of those variables using
`C++` comments has follows:

~~~~{.cxx}
// T stands for the temperature
@Input T;
// p stands for the porosity
@Input p;
~~~~

However, those comments are not associated to the variables by `MFront`,
so the output of `mfront-query` is unchanged.

#### `doxgen`-like comments

`MFront` support [`doxygen`-like](https://www.doxygen.nl/index.html)
comments to document variables, as follows:

~~~~{.cxx}
//! T stands for the temperature
@Input T;
//! p stands for the porosity
@Input p;
~~~~

In this case, the output of `mfront-query` is a bit more informative:

~~~~{.bash}
$ mfront-query --inputs YoungModulus.mfront 
- T: T stands for the temperature
- p: p stands for the porosity
~~~~

#### The `TFEL` glossary and the `setGlossaryName` method

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

The glossary name automatically defines the external name of the
variable and automatically adds an approriate documentation. The output
of `mfront-query` is now:

~~~~{.cxx}
$ mfront-query --inputs YoungModulus.mfront 
- Temperature (T): The temperature
- Porosity (f): Porosity of the material
~~~~

#### Variables not covered by the `TFEL` glossary and the `setEntryName` method

The `TFEL` glossary is finite. If a variable is not described by a
glossary entry, we highly recommend to give it an explicit external name
using the `setEntryName` and to document it using `doxygen`-like
comments.

## Add other metadata for assurance quality

## Add parameters

## Explicit coding: add types and quantity support.

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
Hence the declaration

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
@Input temperature f;
~~~~

By default, `temperature`, `strain`, `stress` are defined as type
aliases to floating point number type used by the interface.

### Using quantities

## Add unicode symbols


<!-- Local IspellDict: english -->
