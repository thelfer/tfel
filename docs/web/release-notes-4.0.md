---
title: Release notes of the 4.0 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer
date: 2020
lang: en-EN
numbersections: true
documentclass: article
from: markdown+tex_math_single_backslash
geometry:
  - margin=2cm
papersize: a4
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

# Known incompatibilities
 
- Material properties now define a list of aliases to scalar types that
  can conflict with variables declarations. The name of those aliases is
  thus reserved. See [this page](mfront-types.html) for details.
- The `TFELPhysicalConstants` library has been removed.
- The `TFEL_NORETURN` macro has been removed.
 
# Changes to existing models

## Mohr Coulomb Abbo Sloan

- The corner smoothing of the Mohr Coulomb surface in the deviatoric
  plane is changed from the C1-continuous version (Abbo and Sloan, 1995)
  to the C2-continuous version from Abbo et al., 2011.

# New MFront features

## Improvements to implicit DSLs

### New keywords

####  The `@ProcessNewCorrection` keyword

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

##### Example

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

####  The `@ProcessNewEstimate` keyword

The `@ProcessNewEstimate` keyword introduces a code block called after
the update of the increments of the integration variables.

This method may be used to compute local variables dependent on the
updated value of the integration variables.

For example, `MFront` may define or update this code block to evaluate
material properties dependent on the value of the state variable (for
example, a Young modulus depending of the porosity), if any.

# Tickets fixed

## Ticket:#277: [material properties] Support for quantities

The `UseQt` keyword now allows to turn on support for quantities in
material properties for interfaces that supports it.

All interfaces delivered by `MFront` have proper support for quantities.

For more details, see : <https://sourceforge.net/p/tfel/tickets/277/>

## Ticket #276: Support for quantities in `TFEL/PhysicalConstants`

The `PhysicalConstants` class now have an additional boolean template
parameter stating if quantities are to be used. For backward
compatibility, this boolean value is `false` by default.

The inline variables in the `tfel::constants` now also have a similar
template parameter.

For more details, see : <https://sourceforge.net/p/tfel/tickets/276/>

## Ticket #275: [material properties] Define standard MFront scalar types

For consistency with behaviours, aliases to many scalar types are now
automatically defined in material properties, such as:

- `temperature`, `strain`, etc...

A complete list of those aliases can be found on [this page](mfront-types.html).

For more details, see : <https://sourceforge.net/p/tfel/tickets/275/>

# References
