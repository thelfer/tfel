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

# Tickets fixed

## Ticket #276: Support for quantities in `TFEL/PhysicalConstants`

The `PhysicalConstants` class now have an additional boolean template
parameter stating if quantities are to be used. For backward
compatibility, this boolean value is `false` by default.

~~~~{.cxx}

~~~~

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
