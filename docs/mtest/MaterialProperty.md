The `@MaterialProperty` keyword defines a material property.  Only the
mechanical properties defined by the behaviour for the modelling
hypothesis considered may be used with the noticeable exception of the
thermal expansions:

- isotropic thermal expansion is defined through the
  `ThermalExpansion` property
- orthotropic thermal expansion is defined through the
  `ThermalExpansion1`, `ThermalExpansion2` and `ThermalExpansion3`
  material properties.

This keyword takes the type of the material property as an option.
Three types of material properties are supported, `constant` or
`castem` or function.

The name of the material property shall then be given.

If the material property is constant, its value is expected.

If the material property is of the `castem` type, the name of the
library and the name of the function implementing the material
property are expected.

If the material property is a function, the function definition is
expected.

## Example (constant)

~~~~{.cpp}
@MaterialProperty<constant> 'YoungModulus' 150.e9;
~~~~~~~~

## Example (castem)

~~~~{.cpp}
@MaterialProperty<castem> 'YoungModulus' 'libInconel600MaterialProperties.so' 'Inconel600_YoungModulus';
~~~~~~~~
