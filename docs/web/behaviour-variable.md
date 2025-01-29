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

## Example of usage

### Declaration

The following snippet declares a variable named `b1` associated with a
behaviour `Plasticity` defined in the file
`BehaviourVariablePlasticity.mfront`.

~~~~{.cpp}
@BehaviourVariable b1 {
  file: "Plasticity.mfront",
  variables_suffix: "1",
  external_names_prefix: "FirstPhase",
  store_gradients: true,
  store_thermodynamic_forces: true,
  shared_external_state_variables: {".+"}
};
~~~~

This example illustrates the complexity of the declaration of a
behaviour variable. The main reason for this complexity is due to the
fact that the calling behaviour is responsible for declaring and
managing the variables associated with `b1` (material properties,
internal state variables, external state variable), etc.

In particular, the behaviour variable and the calling behaviour may
share some variables (material properties, external state variables).

In this example, since no material property is declared as shared, the
calling behaviour will create one material property for each material
property declared in the behaviour `Plasticity` (see the
`shared_material_properties` option below for details on how to declare
shared material properties). To avoid conflicts, the names and external
names of this new material properties are modified using the options
`variables_prefix`, `variables_suffix`, `external_names_prefix` or
`external_names_suffix`.

To illustrate this, let us assume that the behaviour defined in
`Plasticity.mfront` defines the following state variables:

- `eel`, with the glossary name `ElasticStrain`
- `p`, with the glossary name `EquivalentPlastictStrain`

This can be verified by using [`mfront-query`](mfront-query.html):

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

### Integration of a behaviour variable

A behaviour variable can be integrated using the `integrate` method as
follows:

~~~~{.cxx}
const auto r = b1.integrate(smflag, smt);
~~~~

where `smflag` and `smt` are parameters associated with the computation
of the consistent tangent operator (see below).

### Retrieveing the tangent operator

The tangent operator can be retrieved using the `getTangentOperator`
method as follows:

~~~~{.cxx}
Dt = b1.getTangentOperator();
~~~~

## Options of the `@BehaviourVariable` keyword

- `file`: name of the `MFront` file in which the behaviour is defined.
  This entry is required.
- `variables_prefix`: prefix added in front of the variable names
  associated with the behaviour variable in the calling behaviour. The
  prefix is added to variables that are not shared with
  the calling behaviour.
- `variables_suffix`: suffix added at the end of the names of
   variables that are not shared with the calling behaviour.
- `external_names_prefix`: prefix added to external names of variables
  that are not shared with the calling behaviour.
- `external_names_suffix`: suffix added to external names of variables
  that are not shared with the calling behaviour.
- `store_gradients`: flag stating if the gradients of the behaviour
  variable must be stored in a dedicated auxiliary state variable. This
  flag is `true` by default.
- `store_thermodynamic_forces`: flag stating if the thermodynamic forces
  of the behaviour variable must be stored in a dedicated auxiliary
  state variable. This flag is `true` by default.
- `shared_material_properties`: list of regular expressions used to
  select shared material properties. This list is empty by default.
- `shared_external_state_variables`: list of regular expressions used to
  select shared external state variables. This list is empty by default.
- `automatically_save_associated_auxiliary_state_variables`: flag
  stating if the persistent variables, gradients (if `stored_gradients`
  is true) or thermodynamic forces (if `stored_thermodynamic_forces` is
  true) of the behaviour variable shall automatically be saved in the
  `updateAuxiliaryStateVariables` method of the calling behaviour. This
  flag is `true` by default.

## Treatment of the gradients or thermodynamic variables of the behaviour variable

Setting the `store_gradients` option to `true`, which is the default
value, has the following effects:

- an associated auxiliary state variable is automatically declared by
  the calling behaviour for each gradient of the behaviour variable.
  These auxiliary state variables are obtained by applying the
  transformation rules on names and external names.
- the `initialize/reset` methods associated with this behaviour variable
  (see bellow) will automatically initialize the gradients using the
  associated auxiliary state variables. Note that the `initialize`
  method associated with this behaviour variable is automatically called
  by the main `initialize` method of the calling behaviour (which is
  always called).
- the `updateAuxiliaryStateVariables` method associated with this
  behaviour variable (see bellow) stores the values of the gradients at
  the end of the time step in the associated auxiliary state variables.
  Note that if the
  `automatically_save_associated_auxiliary_state_variables` option is
  set to true, the `updateAuxiliaryStateVariables` method associated
  with this behaviour variable is called inside the main
  `updateAuxiliaryStateVariables` method of the calling behaviour.

If the `store_gradients` option to `false`, the calling behaviour is
responsible for setting the gradients at the beginning of the time step
and either their increments or their values at the end of the time step,
depending on the case.

The `store_thermodynamic_forces` option has a similar role for
thermodynamic forces.

### Note on orthotropic behaviours

If the called behaviour is orthotropic, the gradients and thermodynamic
forces must be defined in the material frame associated with the
behaviour variable. The calling behaviour is responsible for making the
appropriate rotations.

### Note on the strain measures

If the called behaviour is based on a strain measure, then the calling
behaviour is responsible for consistently initializing the strain at the
beginning of the time step (in particular if `store_gradients` is set to
`false`), the thermodynamic forces (in particular if
`store_thermodynamic_forces` is set to `false`) and the strain
increment.

### Note on stress free strains

Strain based Behaviours requires special care if the called behaviour
declares stress free expansions (volumetric swelling, thermal expansion,
etc.). In the case, the called behaviour exposes a method called
`computeStressFreeStrain` that shall only be called **after** the
initialization of the behaviour. This method returns a pair containing
the stress free strains at the beginning of the time step and at the end
of the time step. 

Assuming that the calling behaviour wants to assign a total strain
increment `deto_b1`, the method `computeStressFreeStrain` can be used as
follows:

~~~~{.cxx}
// assuming that the behaviour is able to compute the thermal strains
const auto [eth_bts, eth_ets] = b1.computeStressFreeStrain();
b1.deto = deto_b1 - eth_ets + eth_bts;
~~~~

Note that these stress free strains are:

- defined consistently with the strain measure used,
- defined in the material frame associated with the behaviour variable.

## Treatment of the persistent variables of the behaviour variable

A behaviour variable may declares persistent variables, i.e. state
variables or auxiliary state variables. Those persistent variables are
automatically declared as auxiliary state variables by the calling
behaviour. The transformation rules on the names and external names are
applied to define those new variables (see the `variables_prefix`,
`variables_suffix`, `external_names_prefix` or `external_names_suffix`
options).

The persistent variables of the behaviour variable are automatically
initialized by copying the values of the associated auxiliary state
variables of the calling behaviour.

If the `automatically_save_associated_auxiliary_state_variables` option
is set to true, the persistent variables of the behaviour variable are
automatically saved in the associated auxiliary state variables at the
beginning of the `updateAuxiliaryStateVariables` method of the calling
behaviour.

## Treatment of the material properties and external state variables of the behaviour variable

When a material property (or an external state variable) of the
behaviour variable is declared as shared (see the
`shared_material_properties` and `shared_external_state_variables`), the
calling behaviour will check if a matching material property (or an
external state variable) is declared. If not, it will declare exactly
the same material property (or external state variable), i.e. the
transformation rules on the names and external names are
**not** applied.

When a material property (or an external state variable) of the
behaviour variable is **not** declared as shared (the default), a new
material property (or an external state variable) is automatically
declaring applying the transformation rules on names.

## Access to the variables of the behaviour variable

The calling behaviour has access to all the variables of the behaviour
variable (material properties, parameters, external state variables,
gradients, local variables, etc..).

## Initialization of the behaviour variable

A specialized version of the `initialize` method which takes the
behaviour variable as its only argument is automatically defined and can
be called as follows:

~~~~{.cxx}
initialize(b1);
~~~~

This method initializes:

- the persistent variables of the behaviour variable to the values of
  the associated auxiliary state variables, and does the same thing for
  gradients and thermodynamic forces depending on the values of the
  `store_gradients` and `store_thermodynamic_forces` options.
- the time increment with the value of the time increment of the calling
  behaviour.

This method is automatically called by the main `initialize` method of
the calling behaviour (which is always called) after initializing the
material properties, external state variables, persistent variables of
the behaviour variables.

## Reinitializing and resetting a behaviour variable

The `initialize` method associated with a behaviour variable can be
called several times. This might be useful to integrate this behaviour
variable several times.

However, this does not guarantee that calling the integration of the
behaviour variable will lead to the same result as some internal
variables may have evolved.

In particular, the increment of the integration variables will not
zeroed. Not zeroing those increment may accelerate the integration when
the behaviour integration is called multiple times as the solution of
the previous call may be a good estimate for the new resolution.

To initialize the behaviour and zero the increment of the integration
variables , one may call the `reset` method instead:

~~~~{.cxx}
reset(b1); // calls initialize and zeros integration variable increments
~~~~

> Note that even the `reset` method does not absolutely guarantee that
> calling the integration of the behaviour variable will lead to the
> same result as some internal variables may have evolved. This will probably
> be the case assuming that the implementation of the called behaviour does
> not make any strange things internally.

## Integration

The `integrate` method performs the integration of the behaviour
variable.

Note that this method must be called after having the gradients at the
beginning of the time step (see also the `store_gradients` option for
details), the thermodynamic forces at the beginning of the time step
(see also the `store_thermodynamic_forces` option for details), and
either the gradients at the end of the time step or their increments
depending on the case.

The `integrate` method takes two arguments:

- one value of the enumeration `SMFlag` defined in the
  `TangentOperatorTraits` class associated with the
  behaviour. The class `TangentOperatorTraits` is templated by the
  behaviour type:
  - If the behaviour type is not
    `MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR`,
    then the enumeration `SMFlag` only contains one value
    `STANDARDTANGENTOPERATOR`;
  - If the behaviour type is
    `MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR`,
    then the values of the enumeration `SMFlag` are described in the
    `FiniteStrainBehaviourTangentOperatorBase` class.
- one value of the enumeration `SMType` defined in the class
  `MechanicalBehaviourBase` which is a base class of all
  behaviours generated by `MFront` (even non mechanical behaviours).
  This enumeration defines the following values:

  - `ELASTIC`,
  - `SECANTOPERATOR`,
  - `TANGENTOPERATOR`,
  - `CONSISTENTTANGENTOPERATOR`,
  - `NOSTIFFNESSREQUESTED`

Here is an example on how to call the integrate method of a behaviour
variable which is not a standard finite strain behaviour: 

~~~~{.cxx}
constexpr auto b1_smflag = TangentOperatorTraits<
    MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR>::
    STANDARDTANGENTOPERATOR;
const auto r = b1.integrate(b1_smflag, CONSISTENTTANGENTOPERATOR);
~~~~

The `integrate` method returns `IntegrationResult` object which is
implicitly convertible to a boolean. This boolean is `true` if the
integration succeeded.

## Retrieving the tangent operator

The tangent operator computed by the `integrate` method can be retrieved
using the `getTangentOperator` method. If the `integrate` method did not
compute the tangent operator (i.e. if the value `NOSTIFFNESSREQUESTED`
was passed as the second argument), the result is undefined.

### Note on orthotropic behaviours

If the called behaviour is orthotropic, the tangent operator is defined
in the material frame associated with the behaviour variable. The
calling behaviour is responsible for making the appropriate rotations.

### Note on the strain measures

If the called behaviour is based on a strain measure, the tangent
operator relates the dual stress and the strain measure. The calling
behaviour is responsible for making the appropriate conversions, if
required.

## Updating the auxiliary state variables associated with a behaviour variable

As stated in the previous paragraph, the
`automatically_save_associated_auxiliary_state_variables` option
controls if the auxiliary state variables associated with a behaviour
variable are automatically saved.

If this option is `false`, the author of the calling behaviour is
responsible for updating those state variables by hand. This task can
be deleguated to a specialized version of the
`updateAuxiliaryStateVariables` method which takes the behaviour
variable as its only argument, as follows:

~~~~{.cxx}
updateAuxiliaryStateVariables(b1);
~~~~

The last solution is to cherry-pick the variables to be saved by hand.
