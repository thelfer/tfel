The `@Relocation` keyword can be used to impose a boundary condition
specific the fuel performances describing the rigid body translation
of fuel pellet fragment as an additional strain.

- an external state variable name.
- a string giving an mfront file implementing a relocation model.

## Modelling hypotheses affected

This keyword is only effective in:

- axisymmetrical generalised plane strain.
- axisymmetrical generalised plane stress.
- generalised plane strain.
- plane strain.
- plane stress.

No expansion are added in the tridimensional and axisymmetrical
modelling hypotheses.

## Physical description

The value of the relocation expansion \(r\) is converted in an
additional expansion in the radial and orthoradial directions:

\[
\left\{
\begin{aligned}
\frac{\Delta\,l_{r}}{l_{r}}           &= \frac{r}{2} \\
\frac{\Delta\,l_{\theta}}{l_{\theta}} &= \frac{r}{2} 
\end{aligned}
\right.
\].

This additional expansion is such that it does not create any stress
in the fuel pellet for an isotropic elastic material with constant
material properties.

The axial axis is the second direction in \(1D\) hypotheses and the
third direction in \(2D\) hypotheses.

## Examples

~~~~ {#Relocation .cpp}
// relocation defined by an external model
@Relocation 'UO2Relocation.mfront';
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~ {#Relocation2 .cpp}
// relocation defined by an external state variable
@ExternalStateVariable real r;
r.setEntryName("Relocation");
@Relocation r;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
