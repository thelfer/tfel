The `ComputeThermalExansion` keyword is followed either by a material
property description or an array of material descriptions (othotropic
behaviours).

A material decription is either a floatting point number or the name
of an external MFront file.

For orthotropic behaviours, the orthotropic axes convention is taken
into account (see `OrthotropicBehaviour`).

## Example

~~~~ {#ComputeThermalExpansion .cpp}
@ComputeThermalExpansion 1.e-5;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~ {#ComputeThermalExpansion2 .cpp}
@ComputeThermalExpansion "UO2_ThermalExpansion.mfront"
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~ {#ComputeThermalExpansion3 .cpp}
@ComputeThermalExpansion {1.e-5,0.2e-5,1.2e-5};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
