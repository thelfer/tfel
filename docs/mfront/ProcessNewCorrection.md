The `@ProcessNewCorrection` keyword introduces a code block called when
a new correction of the increments of the integration variables is
available.

This method can be used to:

- Limit the amplitude of the correction (see also the
  `setMaximumIncrementValuePerIteration` method).
- Implement a line-search algorithm.
- Implement the decondensation step when some integration variables were
  eliminated by static condensation.

This increment is stored in an array called `delta_zeros`. The
`delta_zeros` variable is not meant to be used directly by the users and
views to the corrections of the increments of the integration variables
are automatically declared in this code block.

Let `v` be an integration variable, then the variable `delta_dv` is a
view to the correction of the increment of this variable. If unicode
notations are used, let `υ` be the symbolic name of `v`, then `δΔv` is
an alias for `delta_dv`.

The increments of the integration variables are not updated at this
stage of the resolution algorithm.

## Example

The following code limits the amplitude of the correction given to the
increment of the elastic strain:

~~~~{.cpp}
@ProcessNewCorrection{
 constexpr const real δΔεᵉˡ_m = 1.e-4;
 const auto e = abs(δΔεᵉˡ);
 if(e > δΔεᵉˡ_m){
   δΔεᵉˡ *= e / δΔεᵉˡ_m;
 }
}
~~~~