% Parameters
% Thomas Hefer
% November 6, 2014

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

~~~~~~~~~{#parameters .cpp}
@Parameter dmax1 =  0.98; // initialisation by assignment syntax
@Parameter dmax2(0.98);   // constructor syntax
@Parameter dmax3{0.98};   // uniform initialisation syntax
@Parameter dmax4;         // only syntax valid for tfel < 2.0
dmax4.setDefaultValue(0.98);
~~~~~~~~~

A glossary name or an entry name can be associated to a parameter
through the `setGlossaryName` or the `setEntryName` method.

# Modifying a parameter at runtime

There are two ways to modify a parameter at runtime:

- through the calling solver input file;
- through an external file

From our point of view, the first method is preferable but is not
always avaiable. Some examples are provided below.

## Examples of modify the calling solver input file

### `MTest`

The keyword `@Parameter` is available in [`MTest`](mtest.html).

### `Code-Aster`

### `Zebulon`

The `***parameter` section can be used to define parameters from the
input file.

## External file

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

<!-- Local IspellDict: english -->
