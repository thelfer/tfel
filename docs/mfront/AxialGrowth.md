The `@AxialGrowth` keyword let the user specify an linear stress free
expansion:

- along the second direction of orthotropy if the default orthotropic axes
  convention has been selected.
- along the 'z' direction if the pipe orthotropic axes has been
  defined is defined.

This keyword is followed by:

- an external state variable name.
- a string giving an mfront file implementing a model of the axial growth.

## Notes

- The behaviour must be orthotropic (see `@OrthotropicBehaviour`).
- An orthotropic axes convention *shall* have been set (see
  `@OrthotropicBehaviour`).

## Examples

~~~~ {#AxialGrowth .cpp}
// axial growth defined by an external model
@AxialGrowth 'M5AxialGrowth.mfront';
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~ {#AxialGrowth2 .cpp}
// axial growth defined by an external state variable
@ExternalStateVariable real ag;
ag.setGlossaryName("AxialGrowth");
@AxialGrowth ag;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
