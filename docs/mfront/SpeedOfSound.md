The `@SpeedOfSound` keyword introduces the computation of an upper bound
of the speed of sound in a material. This quantity is mostly meant to be
used by explicit mechanical solvers to evaluate an upper bound of the
critical time step.

The code block must compute a variable called `v_sound` (or `vₛ` if
`UTF-8` encoding is used).

The mass density in the initial configuration is available in a variable
called `rho_m0` (or `ρₘ₀` if `UTF-8` encoding is used).

## Example

~~~~ {#SpeedOfSound .cpp}
@SpeedOfSound{
 v_sound = sqrt(young / rho_m0);
} 
~~~~

~~~~ {#SpeedOfSound_utf8 .cpp}
@SpeedOfSound{
 vₛ = sqrt(young / ρₘ₀);
} 
~~~~
