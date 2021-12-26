---
title: Release notes of the 3.1.11 version of `TFEL`, `MFront` and `MTest`
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

# Issues fixed

## Issue #58: [mfront-doc] skip tests on internal data structure in the getData method

`mfront-doc` may fail to process files when two data members with the
same name are defined in two specializations of the behaviour (for two
different modelling hypotheses) have different types (among other
checks). However, this check is too restrictive for internal data
structures and this special case now treated properly.

For more details, see <https://github.com/thelfer/tfel/issues/58>.

## Issue #51: [mfront] Defining a gradient with external name "Temperature" do not trigger an error

For more details, see <https://github.com/thelfer/tfel/issues/51>.