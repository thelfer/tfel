The `ComputeThermalExansion` keyword is followed either by:

- a material property description or an array of material descriptions
  (orthotropic behaviours) giving the mean linear thermal expansion
  coefficient (first syntax).
- a map defining the mean thermal expansion coefficient(s), the
  reference temperature for the thermal expansion and the default value
  for the reference temperature for the geometry (second syntax). This
  syntax is the same as the one used by most stress potentials.

A material description is either a floating point number, a formula or
the name of an external MFront file.

The thermal expansion is computed as follows:

\[
\frac{\Delta\,l}{l_{T^{i}}} = \frac{1}{1+\alpha\left(T^{i}\right)\,\left(T^{i}-T^{\alpha}\right)}\,\left[\alpha\left(T\right)\,\left(T-T^{\alpha}\right)-\alpha\left(T^{i}\right)\,\left(T^{i}-T^{\alpha}\right)\right]
\]

where:

- \(T^{\alpha}\) is the reference temperature for the thermal
  expansion.
- \(T^{i}\) is the reference temperature at which of the geometry has
  been measured which is generally assumed to be equal to the
  temperature at the beginning of the computations, in the undeformed
  state.

## Definition of the reference temperature for the thermal expansion

The reference temperature for the thermal expansion \(T^{\alpha}\) is
intrinsically linked to the definition of the thermal expansion
coefficient.

Using the first syntax, one may define  \(T^{\alpha}\) as follows:

~~~~ {#ComputeThermalExpansion .cpp}
@Parameter a0 = 2.e-5;
@Parameter a1 = 4.e-8;
@ComputeThermalExpansion "a0 + a1 * (T - 273.15)"{
 reference_temperature : 273.15
};
~~~~

Using the second syntax, one may define  \(T^{\alpha}\) as follows:

~~~~ {#ComputeThermalExpansion2 .cpp}
@Parameter a0 = 2.e-5;
@Parameter a1 = 4.e-8;
@ComputeThermalExpansion {
 thermal_expansion: "a0 + a1 * (T - 273.15)",
 reference_temperature : 273.15
 // or equivalently
 // thermal_expansion_reference_temperature: 273.15
};
~~~~

However, if the mean linear thermal expansion coefficient is given by
an external `MFront` file, it is recommended to define a static
variable called `ReferenceTemperature` in this file.

Whatever the way used to defined \(T^{\alpha}\), a parameter called
`ThermalExpansionReferenceTemperature` is automatically defined.

> **Note**
>
> If \(T^{\alpha}\) is not explicitly defined, a default value of
> \(293.15\,K\) is used.

## Parameter associated with the reference temperature for the geometry

The reference temperature at which of the geometry has been measured,
\(T^{i}\), has a default value of \(293.15\,K\). This can be changed
using the automatically defined
`ReferenceTemperatureForInitialGeometry` parameter.

Using the second syntax, \(T^{i}\) can be defined using the
`initial_geometry_reference_temperature` option.

## Orthotropic axis convention

For orthotropic behaviours, the orthotropic axes convention is taken
into account (see `OrthotropicBehaviour`).

## Saving the computed thermal expansion

Using the second syntax, the boolean option `save_thermal_expansion`
states if the computed thermal expansion(s) at the end of the time step
shall be stored in an auxiliary state variable. The external name of
this variable is `ComputedThermalExpansion`. For an isotropic
thermal expansion, this variable is a scalar. For an orthotropic
material, this variable is an array of \(3\) scalars.

## Examples (first syntax)

~~~~ {#ComputeThermalExpansion .cpp}
@ComputeThermalExpansion 1.e-5;
~~~~

~~~~ {#ComputeThermalExpansion2 .cpp}
@ComputeThermalExpansion "UO2_ThermalExpansion.mfront"
~~~~

~~~~ {#ComputeThermalExpansion3 .cpp}
@ComputeThermalExpansion {1.e-5,0.2e-5,1.2e-5};
~~~~

## Examples (second syntax)

~~~~ {#ComputeThermalExpansion .cpp}
@ComputeThermalExpansion{
  thermal_expansion: 1.e-5
};
~~~~

~~~~ {#ComputeThermalExpansion2 .cpp}
@ComputeThermalExpansion{
  thermal_expansion1: 1.e-5,
  thermal_expansion2: 0.2e-5,
  thermal_expansion3: 1.2e-5,
  thermal_expansion_reference_temperature: 293.15, 
  initial_geometry_reference_temperature: 293.15,
  save_thermal_expansion: true
};
~~~~
