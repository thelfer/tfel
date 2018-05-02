% Getting started with `MFront`
% Thomas Helfer
% 16/06/2017

This document is meant to help new users to find their ways in the
existing documentation of `MFront` and summarize the most important
points.

> **Material properties and models**
> 
> As most users are focused on mechanical behaviours, this document
> will not discuss [*material properties*](material-properties.html)
> and [*models*](models.html).

`MFront` has been designed to help the user writing complex
mechanical behaviours using a domain specific language that are:

- numerically efficient.
- portable from one finite element to another.
- easy to read (by your colleagues) and maintain.

Before writing your own behaviours, a few advices are helpful:

- You must have some knowledge of this behaviour and of its physical
  meaning. See
  [@doghri_mechanics_2000,@besson_non-linear_2009,@forest_mecanique_2013]
  for a good introduction. **If you are new to finite strain single
  crystal plasticity, do not try to directly study the examples found in
  the `MFront` tests base.**. It highly recommended to have some basic
  understanding of the algorithms used to integrate a constitutive
  equation. See
  [@besson_numerical_2004,@lemaitre_engineering_2005,@besson_non-linear_2009].
- You must develop your behaviour *step by step*. Do not neglect unit
  testing. This is what [`MTest`](.md) has been made for.
- You must have some knowledge about your finite element solver, in
  particular when using "advanced" features (finite strain, non local
  models).

The final advice is the more important: as every open-source project,
`MFront` has its community of users. Do not hesitate to ask questions:

- [on the forum](https://sourceforge.net/p/tfel/discussion/).
- [on the mailing lists](https://sourceforge.net/p/tfel/tfel/).
- [to the authors](mailto:tfel-contact@cea.fr).

# First step: installation

The installation of `MFront` is described in depth [here](install.html).

The main points are the followings:

- `MFront` is mostly distributed as `C++` sources that must be compiled.
  Official releases are available on
  [sourceforge](https://sourceforge.net/projects/tfel/files/).
- For those wanting to follow the latest developments, a clone of the
  source repository is available on
  [GitHub](https://github.com/thelfer/tfel). You obviously need to
  install `git` to do this.
- In both cases a more or less recent `C++` compiler is required. Under
  `Unix` systems, this is always the case, each system having some
  official compiler suite (`gcc` under `Linux`, `clang` under `Mac Os`
  and `FreeBSD`). This is not the case under `Windows` where you have to
  install a compiler beforehand.
- *You must understand how extending your solver works and ensure that
  you do use a compatible compiler*. This is more specifically true
  under the `Windows` platform than under `LinuX`: on `LinuX`, most
  compilers strives to be binary compatible with `gcc`. For example, if
  you use `Cast3M` under windows, you may want to use the compiler which
  is package with the `Cast3M` distribution (see
  [here](install-windows-Cast3M2017.html) for details). For `Abaqus`,
  you may need to have the Intel compilers (or at least the Intel
  fortran compiler).
- It is required to have [`cmake`](https://cmake.org/) installed to
  build `MFront` under `Windows`. It is strongly recommended under the
  other platform.

# Second step: understanding how `MFront` works

`MFront` translate a set of input files into `C++` sources and compile
them. A basic knowledge of the `C` and `C++` syntax is then advisable.
`MFront` specific keywords begins with the `@` letter.

`MFront` first introduces the notion of `Domain Specific Language`
(`DSL`), which gives a context about what the input file describes,
mainly the type of the material knowledge treated (material property,
model, finite strain behaviour, strain based behaviour, cohesive zone
moel) and/or the algorithm used to integrate the state variables'
evolution. One highly used `DSL` is called `Implicit` and described a
strain based behaviour whose integration is based on a generalised
mid-point rule.

Each `DSL` has its own conventions, keywords and automatically declared
variables.

**For most mechanical behaviours, the `@Implicit` DSL is recommended:
  the behaviour integration is performed using a generalised mid-point
  rule and the consistent tangent operator, which is required by most
  finite element sovler to have decent numerical performances, can be
  computed**.

> **Detailed documentation**
> 
> The complete description of the `DSL`s available can be found
> [here](documents/mfront/mfront.pdf) and
> [here](documents/mfront/behaviours.pdf), in French.

Once the `DSL` choosen (using the `@DSL` keyword), the list of available
keywords can be retrieved from the command line:

~~~~{.bash}
$ mfront --help-keywords-list=Implicit
~~~~

where `Implicit` is the name of the `DSL`.

The help of for a specific keyword can be retrieved as follows:

~~~~{.bash}
$ mfront --help-keywords-list=Implicit:@Author
~~~~

where `Implicit` is the name of the `DSL` and `@Author` the name of the
keyword.

A good way to get familiar with a specific DSL is to look in the
[`MFront` gallery](gallery.html) if some examples describe its usage.

Otherwise, one may want to look at the tests distributed with `MFront`
in the `mfront/tests/behaviours` directory (online access on github
[here](https://github.com/thelfer/tfel/tree/master/mfront/tests/behaviours)).
Beware that the tests may not reflect the current best way to implement
a specific behaviour (`MFront` evolves a lot and each version introduces
new features): they can be kept outdated to test backward compatibility.

# Third step: before starting to write our first behaviour

## Start looking at the documentation of the tensor operations

Second and fourth orders tensors are the basic mathematical tools used
to describe mechanical behaviours. `MFront` is based on the `TFEL/Math`
library which provides many standards operations and functions to handle
them. See [here](tensors.html) for an introduction.

## Start looking in the `MFront` gallery

The [`MFront` gallery](gallery.html) is meant to document how to
describe in depth and step by step the implementation of some chosen
behaviours. If one of the described behaviour is close to the one you
plan to implement, it is highly recommend to start by modifying the one
described in the gallery.

## Write your discretized equations by hand

## If starting from scratch, proceed step by step

## Do not forget that you can ask for help

# References

<!-- Local IspellDict: english -->
