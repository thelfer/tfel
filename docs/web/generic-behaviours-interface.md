% The generic behaviour interface
% Thomas Helfer
% 12/09/2018

The generic behaviour interface has been introduced to provide an
interface suitable for most needs. Whereas other interfaces target a
specific solver and thus are restricted by choices made by this specific
solver, this interface has been created for developers of homebrew
solvers who are able to modify their code to take full advantage of
`MFront` behaviours.

This interface is tightly linked with the
`MFrontGenericInterfaceSupport` project which is available on `github`:
<https://github.com/thelfer/MFrontGenericInterfaceSupport>. This project
has a more liberal licence which allows it to be included in both
commercial and open-source solvers/library. This licensing choice
explains why this project is not part of the `TFEL`.

# Features

## Supported behaviours

The generic behaviour interface supports:

- strain based behaviours. The definition of the strain depends of the
  strain measure used (see the `MFront`'`@StrainMeasure` keyword).
- finite strain behaviours.
- cohesive zone models
- general behaviours which relates a set of gradients to a set of
  thermodynamic forces.

## Supported modelling hypotheses

- Axisymmetrical generalised plane strain.
- AXISYMMETRICAL,
- PLANESTRESS,
- PLANESTRAIN,
- GENERALISEDPLANESTRAIN,
- TRIDIMENSIONAL,

## Finite strain behaviours {#sec:generic_behaviour_interface:finite_strain_behaviours}

The finite strain behaviours are a special case. For greater
flexibility, the generic behaviour interface allows choosing at runtime
the stress measure and the tangent operator to be returned.

The following stress measures are available:

- the Cauchy stress
- the first Piola-Kirchoff stress
- the second Piola-Kirchoff stress

The following tangent operators are available:

- the derivative of the Cauchy stress with respect to the deformation
  gradient.
- the derivative of the first Piola-Kirchhoff stress with respect to the
  deformation gradient.
- the derivative of the second Piola-Kirchhoff stress with respect to
  the Green-Lagrange strain.

## Support of orthotropic behaviours

Orthotropic behaviours requires to:

- rotate the gradients in the material frame in the global frame before
  the behaviour integration.
- rotate the thermodynamic forces and the tangent operator blocks from
  the material frame in the global frame.

By design, the `generic` behaviour interface does not automatically
perform those rotations as part of the behaviour integration but
generates additional functions to do it. This choice allows the calling
solver to use their own internal routines to handle the rotations
between the global and material frames.

However, the `generic` interface also generates helper functions which
can perform those rotations. Those functions are named as follows:

- `<behaviour_function_name>_<hypothesis>_rotateGradients`
- `<behaviour_function_name>_<hypothesis>_rotateThermodynamicForces`
- `<behaviour_function_name>_<hypothesis>_rotateTangentOperatorBlocks`

They all take three arguments: 

- a pointer to the location where the rotated variables will be stored.
- a pointer to the location where the original variables are stored.
- a pointer to the rotation matrix from the global frame to the material
  frame. The rotation matrix has 9 components stored in column-major
  format. For the function handling the thermodynamic forces and the
  tangent operators blocks, this rotation matrix is transposed
  internally to get the rotation matrix from the material frame to the
  global frame.

In place rotations is explicitly allowed, i.e. the first and second
arguments can be a pointer to the same location.

The three previous functions works for an integration point. Three other
functions are also generated:

- `<behaviour_function_name>_<hypothesis>_rotateArrayOfGradients`
- `<behaviour_function_name>_<hypothesis>_rotateArrayOfThermodynamicForces`
- `<behaviour_function_name>_<hypothesis>_rotateArrayOfTangentOperatorBlocks`

Those functions takes an additional arguments which is the number of
integration points to be treated.

### Finite strain behaviours

As devised in Section
@sec:sec:generic_behaviour_interface:finite_strain_behaviours, finite
strain behaviours are a special case, because the returned stress
measure and the returned tangent operator can be chosen at runtime time.
A specific rotation function is generated for each supported stress
measure and each supported tangent operator.

Here is the list of the generated functions:

-
  `<behaviour_function_name>_<hypothesis>_rotateThermodynamicForces_CauchyStress`.
  This function assumes that its first argument is the Cauchy stress in
  the material frame.
-
  `<behaviour_function_name>_<hypothesis>_rotateThermodynamicForces_PK1Stress`.
  This function assumes that its first argument is the first
  Piola-Kirchhoff stress in the material frame.
-
  `<behaviour_function_name>_<hypothesis>_rotateThermodynamicForces_PK2Stress`.
  This function assumes that its first argument is the second
  Piola-Kirchhoff stress in the material frame.
  `<behaviour_function_name>_<hypothesis>_rotateTangentOperatorBlocks_dsig_dF`.
  This function assumes that its first argument is the derivative of the
  Cauchy stress with respect to the deformation gradient in the material
  frame.
-
  `<behaviour_function_name>_<hypothesis>_rotateTangentOperatorBlocks_dPK1_dF`.
  This function assumes that its first argument is the derivative of the
  first Piola-Kirchhoff stress with respect to the deformation gradient
  in the material frame.
-
  `<behaviour_function_name>_<hypothesis>_rotateTangentOperatorBlocks_PK2Stress`.
  This function assumes that its first argument is the derivative of the
  second Piola-Kirchhoff stress with respect to the Green-Lagrange
  strain in the material frame.
