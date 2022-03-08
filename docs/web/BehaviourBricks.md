% Behaviour bricks
% Thomas Helfer

# Introduction

The purpose of behaviour bricks is to hide (by default) part of the
behaviour implementation. In other word, they are meant to create a
context which simplifies the implementation of some specific behaviours'
classes.

The easiest way to introduce behaviour bricks is to shortly describe the
example of the `StandardElasticity` brick. This brick applies to strain
based mechanical behaviours. It assumes:

- that the total strain is split in an elastic part and a inelastic
  part.
- that the stress are related to the elastic strain through the Hooke
  law.
- that the total strain only appears in the equation associated with the
  split of the strain.

With those assumptions, the `StandardElasticity` brick can automatically
provide:

- Automatic computation of the stress tensor at various stages of the
  behaviour integration.
- Automatic computation of the consistent tangent operator.
- Automatic support for plane stress and generalized plane stress
  modelling hypotheses (definitions of the axial strain as an
  additional state variables and the associated equation enforcing the
  plane stess condition).
- Automatic addition of the "standard" terms associated
  with the elastic strain state variable.

# The `@Brick` keyword

## Syntax

The `@Brick` keyword introduces a behaviour brick. It is followed by the
name of the brick. This name can be given as a string.


## Example

~~~~{.cpp}
@Brick "StandardElasticity"{
  young_modulus: 150e9,
  poisson_ratio: 0.3
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# List of available bricks

The following brick are available:

- the `StandardElasticity` brick, which has been described in the
  introduction of this page. In practice, the evolution of `MFront` and
  the development of the `StandardElastoViscplasticity` brick, has made
  the `StandardElasticity` brick a kind of special case of the
  `StandardElastoViscoplasticity` where the stress potential is limited
  to the Hooke' one and no inelastic flow is allowed. As such, most
  information concerning the `StandardElasticity` brick can be found on
  the page dedicated to the [Hooke stress
  potential](HookeStressPotential.html). In particular, all options passed to
  the `StandardElasticity` brick are indeed forwarded to the underlying
  Hooke stress potential.
- the [`DDIF2` brick](DDIF2Brick.html). This brick is based in the
  [`DDIF2` stress potential](DDIF2StressPotential.html).
- the [`StandardElastoViscoplasticity` brick](StandardElastoViscoPlasticityBrick.html).
- the `CamClayElasticity` brick. This brick is based on the [`CamClay`
  stress potential](CamClayStressPotential.html). In particular, all
  options passed to the `CamClayElasticity` brick are indeed forwarded
  to the underlying `CamClay` stress potential.
- the [`FiniteStrainSingleCrystal` brick](finitestrainsinglecrystal.html).
