---
title: Release notes of the 3.4.4 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer
date: 21/09/2021
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

The page describes the new functionalities of Version 3.4.4 of the
`TFEL` project.

# New features of `mfront-query`

## Better `--generated-sources` query {#sec:tfel-3.4.4:mfront-query:generated_sources}

The `--generated-sources` query can now have an option:

- `sorted-by-libraries` (default): which returns the sources to be
  generated associated with each library
- `unsorted`: which returns all the sources to be generated

# Issues solved

## Issue #40: [mfront-query] return all the generated sources (not sorted by library)

See Section @sec:tfel-3.4.4:mfront-query:generated_sources for a
description of the new features of the `--generated-sources` query.

For more details, see <https://github.com/thelfer/tfel/issues/40>.
