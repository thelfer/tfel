The `@ProcessNewEstimate` keyword introduces a code block called after
the update of the increments of the integration variables.

This method may be used to compute local variables dependent on the
updated value of the integration variables.

If a normalisation factor `nf` was given to the integration variable
`v` (see the `setNormalisationFactor` method), `dv` is, in this code
block, the normalised increment used by the solver: the updated value
of `v` is `v + nf * dv`, whereas `dv` is the physical increment in the
`@Integrator` code block.

For example, `MFront` may define or update this code block to evaluate
material properties dependent on the value of the state variable (for
example, a Young modulus depending of the porosity), if any.
