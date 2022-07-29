---
title: Data interpolation in `TFEL/Math`
author: Thomas Helfer, Étienne Castelier
date: 29/07/2022
lang: en-EN
link-citations: true
colorlinks: true
geometry:
  - margin=2cm
papersize: a4
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

# Linear interpolation

The `computeLinearInterpolation` allows to compute the linear
inteporlation of a set of data by the arrays of abscissae and
values respectively.

The `computeLinearInterpolationAndDerivative` returns the a pair
containing the value resulting for the linear interpolation and the
derivative.

The first template argument of those functions is a boolean stating if
extrapolation must be performed.

## Notes

1. Both functions assumes that the abscissae are ordered.
2. Both functions are `constexpr`
3. Values can be arrays or tensorial objects.
4. Both functions are compatible with quantitites.

## Example of usage

~~~~{.cxx}
constexpr std::array<time, 3u> abscissae{time{0}, time{1}, time{2}};
constexpr std::array<stress, 3u> values{stress{1}, stress{2}, stress{4}};
constexpr auto v = tfel::math::computeLinearInterpolation<true>(abscissae, values,
                                                                time{-1});
~~~~


