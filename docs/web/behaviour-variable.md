---
title: Calling behaviours inside behaviours using the `@BehaviourVariable` keyword
author: Thomas Helfer, Antoine Martin
date: 08/01/2025
geometry: a4paper
fontsize: 12pt
---

Calling other behaviours from a behaviour is useful in many situations
and allows, for instance, to

- write a macroscopic behaviour that calls microscopic behaviours inside
  an homogeneization scheme.
- implement a simple switch from one behaviour to another based on some
  criteria in a wrapper behaviour. This technique is often used in
  nuclear fuel performance codes since the behaviour of materials are
  described by distinct constitutive equations in normal and off-normal
  operating conditions.
- calling complex models based on any of the following DSLs:
  `DefaultModel`, `RungeKuttaModel`, `ImplicitModel`.

This document describes a solution based on so-called behaviour
variables (i.e. variables that are in fact behaviours) which are
introduced by the `@BehaviourVariable` keyword.

# Declaration of a behaviour variable

## A first example

The following code introduces a behaviour variable named
`first_phase_behaviour`. This declaration is followed by a set of
options describing the interaction with the calling behaviour:

~~~~{.cxx}
@BehaviourVariable first_phase_behaviour {
  file: "Plasticity.mfront",
  variables_suffix: "1",
  external_names_prefix: "FirstPhase",
  store_gradients: true,
  store_thermodynamic_forces: true,
  shared_external_state_variables: {".+"}
};
~~~~

The `file` option points to a path to an `MFront` file implementing the
behaviour represented by the behaviour variable.

Options like `variables_suffix` and `external_names_prefix` allows to
avoid conflicts between the variables declared by the calling behaviour
and variables declared by other behaviour variables.

For instance, let us assume that the behaviour defined in
`Plasticity.mfront` defines the following state variables:

- `eel`, with the glossary name `ElasticStrain`
- `p`, with the glossary name `EquivalentPlastictStrain`

~~~~{.bash}
$ mfront-query --unicode-output=false --state-variables Plasticity.mfront
- ElasticStrain (eel): The elastic strain
- EquivalentPlasticStrain (p): the equivalent plastic strain
~~~~

Those state variables are automatically declared as auxiliary state
variables by the calling behaviour, but their names is changed, as
follows:

~~~~{.bash}
$ mfront-query --unicode-output=false --auxiliary-state-variables MacroscopicBehaviour.mfront 
- FirstPhaseStrain (eto1)
- FirstPhaseStress (sig1)
- FirstPhaseElasticStrain (eel1)
- FirstPhaseEquivalentPlasticStrain (p1)
~~~~

This output only indicates that, due to the `store_gradients` and
`store_thermodynamic_forces` options, the strain and stress of the
behaviour are also declared as auxiliary state variables

Finally, the last option, `shared_external_state_variables`, states that
all external state variables are shared with the calling behaviour. As a
result, the macroscopic behaviour declares the same state variable as
the behaviour defined in `Plasticity.mfront`:

~~~~{.bash}
$ mfront-query --unicode-output=false --external-state-variables tests.mfront 
- Temperature (T): the temperature
$ mfront-query --unicode-output=false --external-state-variables MacroscopicBehaviour.mfront 
- Temperature (T): the temperature
~~~~

## Detailed description of the various options of the `@BehaviourVariable` keyword

As illustrated by the previous paragraph, calling a behaviour from
another one requires a lot of flexibility depending on the needs of the
calling behaviour.

For instance, if the Taylor/Voigt homogeneization scheme is used, the
microscopic strain is assumed equal to the macroscopic strain, so there
is no need to store the strain of the behaviour variable in a dedicated
auxiliary state variables.

The options of the `@BehaviourVariable` keyword allows to customize how
a behaviour variable is treated by the calling behaviour. The following
options are available:

- `file`: the name of the file defining the behaviour.
- `variables_prefix` and `variables_suffix` allows to modify the names
  of variables of the behaviour variable that are not shared with the
  calling behaviour. Those options are not required.
- `external_names_prefix` and `external_names_suffix` allows to modify
  the external names of variables of the behaviour variable that are not
  shared with the calling behaviour. Those options are not required.
- `store_gradients` specifies if the gradients of the behaviour variable
  must be stored in an auxiliary state variables (the default). The
  `store_thermodynamic_forces` option does the same for thermodynamics
  forces. Those options are not required.
- The `shared_material_properties` option allows to specify a set of
  regular expressions which select the material properties that are
  shared with the calling behaviour. The regular expressions are tested
  against the names of the material properties and their external names.
  The `shared_external_state_variables` does the same for external state
  variables. Those options are not required.

# Usage of behaviour variables

