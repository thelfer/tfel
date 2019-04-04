The `@HandleThermalExpansion` keyword allow the activation or
 desactivation of the computation of thermal strain. The computation
 of the thermal strain is done by default if a material property named
 `ThermalExpansion` is defined (for isotropic materials).

This keyword is followed by a boolean. If `true`, the computation of
the thermal strain is done.

## Example

~~~~ {.cpp}
@HandleThermalExpansion false;
~~~~~~~~~~
