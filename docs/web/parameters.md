---
title: Parameters in `MFront`
author: Thomas
date: 6/10/2014
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

# Rationale

Being able to modify some coefficients of a behaviour without
recompiling is a very handy feature for the end user and is mandatory
for parametric studies.

To achieve this, most mechanical solvers have introduced material
properties, declared with the `@MaterialProperty` keyword in `MFront`,
which are evaluated by the calling solver and then passed to the
behaviour. The main idea behind material properties is to write
generic behaviours that can be used to describe a wide range of
materials.

There are a important shortcomings associated with the use of material
properties:

- For Assurance Quality reasons, we consider that the usage of generic
  behaviours shall be avoided, mainly because part of the definition
  of the behaviour is put in the user input file: we do think that the
  whole description of the behaviour shall be included in the `MFront`
  implementation. This can not be achieved with material properties
  since their is no easy cross-solver way to define their default
  values from the behaviour implementation.
- The use of material properties can have significant numerical impact
  in some finite element solver: those costs are associated to the
  evaluation and transfer of material properties values to the
  behaviour.

Parameters, defined through the `@Parameter` keyword, are meant to be
an alternative solution: parameters are internal coefficients that can
be modified by the end user at (almost) no cost. **A parameter must
have a default value** which must be considered as the reference
value.

Parameters are global values shared by all instances of the
behaviour. Parameters are thus associated to the behaviour and not to
a material (or model in the `Cast3M` wording). Parameters thus have a
few shortcomings (compared to material properties).

# Defining a parameter

Parameters are defined with the `@Parameter` keyword. The following
syntax are allowed:

~~~~~{#parameters .cpp}
@Parameter real dmax1 =  0.98; // initialisation by assignment syntax
@Parameter real dmax2(0.98);   // constructor syntax
@Parameter real dmax3{0.98};   // uniform initialisation syntax
@Parameter real dmax4;         // only syntax valid for tfel < 2.0
dmax4.setDefaultValue(0.98);
~~~~~

A glossary name or an entry name can be associated to a parameter
through the `setGlossaryName` or the `setEntryName` method.

> **Note**
>
> Historically, the type of the parameter was not required and was
> implicitly assumed to be of the `real` type. Although deprecated,
> the following syntax is still valid:
> 
> ~~~~~{#parameters .cpp}
> @Parameter dmax1 =  0.98; // initialisation by assignment syntax
> ~~~~~

# Modifying a parameter at runtime

There are two ways to modify a parameter at runtime:

- through the calling solver input file;
- through an external file

From our point of view, the first method is preferable but is not
always available. Some examples are provided below.

## Examples of modify the calling solver input file

### `MTest`

The keyword `@Parameter` is available in [`MTest`](mtest.html).

### `Code-Aster`

### `Zebulon`

The `***parameter` section can be used to define parameters from the
input file.

## External file {#sec:mfront:parameters:parameters_file}

Let us take an example of a behaviour named `Mazars` (the behaviour
name is defined by the value given to the `@Behaviour` keyword and
prefixed by the material name, given by `@Material` keyword (if any)).
We suppose that this behaviour defines a parameter whose entry name is
`DamageThreshold` (entry names are defined through the `setEntryName`
method).

If a file named `Mazars-parameters.txt` is in the current directory,
it is automatically read at the first behaviour call. This file must
contain lines beginning with a parameter name and its value. No
comments are allowed.

If our case, this file may contain the following line:

~~~~~~~~~{#external-file .txt}
DamageThreshold 0.95
~~~~~~~~~

If a parameter is not defined in this file, it will have its default
value.

This functionality is illustrated below:

![Defintion of parameters through an external file](img/parameters.svg
 "Defintion of parameters through an external file")

# Querying information about parameters in `mfront-query`

## List of parameters

The list of parameters in an `mfront` file can be retrieved as follows:

~~~~{.bash}
$ mfront-query --parameters Rousselier.mfront
...
- safety_factor_for_the_upper_bound_of_the_porosity: a safety factor for the porosity upper bound
- safety_factor_for_the_upper_bound_of_the_porosity_for_fracture_detection: a safety factor for the porosity upper bound
- staggered_scheme_porosity_criterion: stopping criterion value of the staggered scheme
- staggered_scheme_maximum_number_of_iterations: maximum number of iterations of the staggered scheme allowed
- minimal_time_step_scaling_factor: minimal value for the time step scaling factor
- maximal_time_step_scaling_factor: maximal value for the time step scaling factor
- numerical_jacobian_epsilon: perturbation value used to compute a numerical approximation of the jacobian
- iterMax: maximum number of iterations allowed
~~~~

## Querying the default value of a parameter

The default value of a parameter can be retrieved using the
`--parameter-default-value` query, as follows:

~~~~{.bash}
$ mfront-query --parameter-default-value=epsilon Rousselier.mfront 
1.e-14
$ mfront-query --parameter-default-value=iterMax Rousselier.mfront
100
~~~~

## Querying the type of a parameter

The type of a parameter can be retrieved through the `--parameter-type`
query, as follows:

~~~~{.bash}
$ mfront-query --parameter-type=iterMax Rousselier.mfront
ushort
~~~~

Here `ushort` means that the `iterMax` parameter is stored as unsigned
short integer.

## Querying the name of the parameters' file through `mfront-query`

The name of the file which can be used to modify the parameters' values
(see Section @sec:mfront:parameters:parameters_file) can be retrieved
with the `--parameters-file` query:

~~~~{.bash}
$ mfront-query --parameters-file Rousselier.mfront 
Rousseliertest-parameters.txt
~~~~

<!-- Local IspellDict: english -->
