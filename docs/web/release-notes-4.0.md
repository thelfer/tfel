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
tabPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

# Tickets fixed

# Changes to existing models

## Mohr Coulomb Abbo Sloan

- The corner smoothing of the Mohr Coulomb surface in the deviatoric plane is changed from the C1-continuous version (Abbo and Sloan, 1995) to the C2-continuous version from Abbo et al., 2011.
