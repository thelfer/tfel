---
title: Release notes of the 3.1.6 version of `TFEL`, `MFront` and `MTest`
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
tabPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

This is mainly a bug fix version of the `3.1` series. All tickets
solved are described below.

# Noticeable fixed issues that may affect the results obtained with previous versions

Ticket #256 reported that the scalar product of two unsymmetric tensors
was not properly computed.

This may affect single crystal finite strain computations to a limited
extent, as the Mandel stress tensor is almost symmetric.

# Tickets fixed

## Ticket #245: Example generated with the zmat behaviour misses the `**parameters` section

For more details, see: <https://sourceforge.net/p/tfel/tickets/245/>

## Ticket #199: The wrapping of the `setParameter` methods of the `mtest::Behaviour` class is missing in the `python` bindings

The following methods are now available:

- `setParameter`
- `setIntegerParameter`
- `setUnsignedIntegerParameter`

For more details, see: <https://sourceforge.net/p/tfel/tickets/199/>
