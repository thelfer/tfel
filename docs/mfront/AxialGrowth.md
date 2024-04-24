The `@AxialGrowth` keyword let the user specify an linear stress free
expansion:

- along the second direction of orthotropy if the default orthotropic axes
  convention has been selected.
- along the 'z' direction if the pipe orthotropic axes has been
  defined is defined. The 'z' direction is the second direction of
  orthotropy for all hypotheses except Generalised Plane Strain, Plane
  Strain and Plane Stress hypotheses.

This keyword is followed by:

- an external state variable name.
- a string giving an mfront file implementing a model of the axial growth.

## Physical description

The value of the axial growth gives the change in length in the axial
direction \(\frac{\Delta\,l_{z}}{l_{z}}\).

The axial growth is assumed to be isochoric, so the change of length
in each direction of the plane perpendicular to the axial direction is
given by:
\[
\frac{\Delta\,l_{\parallel}}{l_{\parallel}}=\frac{1}{\sqrt{1+\frac{\Delta\,l_{z}}{l_{z}}}}-1
\]

## Notes

- The behaviour must be orthotropic (see `@OrthotropicBehaviour`).
- An orthotropic axes convention *shall* have been set (see
  `@OrthotropicBehaviour`).

## Examples

~~~~ {#AxialGrowth .cpp}
// axial growth defined by an external model
@AxialGrowth 'M5AxialGrowth.mfront';
~~~~

~~~~ {#AxialGrowth2 .cpp}
// axial growth defined by an external state variable
@ExternalStateVariable real ag;
ag.setGlossaryName("AxialGrowth");
@AxialGrowth ag;
~~~~
