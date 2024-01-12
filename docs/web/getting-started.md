---
title: Getting started with `MFront`
author: Thomas Helfer
date: 16/06/2017
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

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
  [@doghri_mechanics_2000;@besson_non-linear_2009;@forest_mecanique_2013]
  for a good introduction. **If you are new to finite strain single
  crystal plasticity, do not try to directly study the examples found in
  the `MFront` tests base.**. It highly recommended to have some basic
  understanding of the algorithms used to integrate a constitutive
  equation (see
  [@besson_numerical_2004;@lemaitre_engineering_2005;@besson_non-linear_2009]).
- You must develop your behaviour *step by step*. Do not neglect unit
  testing. This is what [`MTest`](.md) has been made for.
- You must have some knowledge about your finite element solver, in
  particular when using "advanced" features (finite strain, non local
  models).

The final advice is the more important: as every open-source project,
`MFront` has its community of users. **Do not hesitate to ask
questions**:

- [on the forum](https://github.com/thelfer/tfel/discussions).
- [to the authors](mailto:tfel-contact@cea.fr).

# First step: installation

The installation of `MFront` is described in depth [here](install.html).

The main points are the followings:

- `MFront` is mostly distributed as `C++` sources that must be compiled.
  Official releases are available on
  [github](https://github.com/thelfer/tfel/releases).
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
  you do use a compatible compiler*. This is particulary important under
  the `Windows` platform: on `LinuX`, most compilers strives to be
  binary compatible with `gcc`. For example, if you use `Cast3M` under
  windows, you may want to use the compiler which is packaged with the
  `Cast3M` distribution (see [here](install-windows-Cast3M2017.html) for
  details). For `Abaqus`, you may need to have the Intel compilers (or
  at least the Intel fortran compiler).
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
mid-point rule (see
[@besson_numerical_2004;@lemaitre_engineering_2005;@besson_non-linear_2009]).

Each `DSL` has its own conventions, keywords and automatically declared
variables.

> **The `Implicit` DSL**
> 
> For most mechanical behaviours, the `Implicit` DSL is recommended: the
> behaviour integration is performed using a generalised mid-point rule
> and allow the consistent tangent operator, which is required by most
> finite element sovler to have decent numerical performances, to be
> computed.

Once the appropriate `DSL` choosen, the list of available keywords can
be retrieved from the command line:

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

> **About the tests cases**
> 
> Beware that the tests may not reflect the best way to
> implement a specific behaviour (`MFront` evolves a lot and each
> version introduces new features): they can be kept outdated to ensure
> backward compatibility.

> **Detailed documentation**
> 
> This [page](documentations.html) references most of the available
> documentations.
> 
> For example, the complete description of the `DSL`s available can
> be found [here](documents/mfront/mfront.pdf) and
> [here](documents/mfront/behaviours.pdf), in French.
> 
> However, such detailed documentation is not required for most users.
> We even consider that beginning with `MFront` from these documents
> can be overwhelmingly difficult. Reading the detailed example of the
> gallery is much easier: `Longum iter est per praecepta, breve et
> efficax per exempla` (It's a long way by the rules, but short and
> efficient with examples).
> 
> For French users, a detailed tutorial can be found
> [here](documents/tutoriel/tutoriel.pdf)

# Third step: before starting to write your first behaviour

## Look if your behaviour can be implemented using the `StandardElastoViscoPlasticity` brick

The [`StandardElastoViscoPlasticity`
brick](StandardElastoViscoPlasticityBrick.html) can be used to implement
a large class of strain based elasto-visco-plastic behaviours
implementations.

It is worth to look if your behaviour falls in that category, or a least
if part of your behaviour falls in that category:

- In the first case, implementing the behaviour using the
  `StandardElastoViscoPlasticity` brick can be straightforward. Even if
  you do not want to use the `StandardElastoViscoPlasticity` brick, it
  is worth to have a reference implementation to which one can compare
  its own implementation in terms of performance, robustness, etc.
- In the second case, implementing a complex behaviour is only tractable
  step by step. So it is worth to implement first the part which can be
  implemented using the `StandardElastoViscoPlasticity` brick and
  validate this part.

## Have a look at the `MFront` gallery

The [`MFront` gallery](gallery.html) is meant to document how to
describe in depth and step by step the implementation of some
behaviours. If one of the described behaviour is close to the one you
plan to implement, it is highly recommend to carefully read the
associated page and start by modifying the one described in the gallery.

## Write your constitutive equations as a system of ordinary differential equations using tensorial notations

Constitutive equations can be expressed as a system of ordinary
differential equations which allows to determine the evolution of the
state variables of the material.

## Select your integration algorithm

## Write your discretized equations by hand using tensorial notations

## Look at the documentation of the tensor operations

Second and fourth orders tensors are the basic mathematical tools used
to describe mechanical behaviours. `MFront` is based on the `TFEL/Math`
library which provides many standards operations and functions to handle
them. See [here](tensors.html) for an introduction.

# Fourth step: your first implementation



# References

<!-- Local IspellDict: english -->
