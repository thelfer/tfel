This document describes the `CamClay` stress potential based on the
`Cam-Clay` non linear elastic behaviours.The `CamClay` stress potential
provides:

- automatic computation of the consistent tangent operator.
- a method called `computeElasticPrediction`.

The Cam-Clay non linear elastic behaviours are fully described on the
`TFEL` web site in the `CamClayStressPotential.html` page.

# Options of the Cam-Clay stress potential

The following options are available:

- `unloading_reloading_slope`: unloading/reloading slope \(\kappa\).
- `initial_void_ratio`: initial void ratio \(e0\).
- `poisson_ratio`: Poisson ratio of the material \(\nu\).
- `shear_modulus`: shear modulus of the material \(\mu\). 
- `pressure_threshold`: threshold below which the behaviour is linear
  elastic. The value of this threshold must be. strictly positive.
- `deduce_shear_modulus_from_poisson_ratio`: boolean stating if the
  shear modulus shall be deduced from the Poisson ratio (Case 2). If
  this option is true, the Poisson ratio is automatically declared as a
  material property. Otherwise, the shear modulus is automatically
  declared as a material property.
- `incremental_deviatoric_part`: boolean stating if the deviatoric part
  of the stress/strain relationship shall be treated incrementally. This
  option is only valid if the shear modulus is deduced from the Poisson
  ratio. In this case, an incremental treatment given by Equation
  @eq:mfront:camclay:shear_rate_behaviour is assumed by default. If
  false, Equation @eq:mfront:camclay:shear_behaviour is used.`

The options `poisson_ratio`, `shear_modulus` and
`deduce_shear_modulus_from_poisson_ratio` are mutually exclusive.

The `incremental_deviatoric_part` is not compatible with the
`shear_modulus` option.

## Examples of usage

~~~~{.cxx}
@Brick StandardElastoViscoPlasticity{
   stress_potential : "CamClay" {
   unloading_reloading_slope : 0.01,
   initial_void_ratio : 0.5,
   poisson_ratio : 0.3,
   pressure_threshold : 1
 }
};
~~~~~
