---
title:  Numerical intgration in the `TFEL/Math` library
author: Thomas Helfer
date: 12/10/2025
lang: en-EN
numbersections: true
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
lstPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
header-includes:
bibliography: bibliography.bib
csl: iso690-numeric-en.csl
---

Numerical integration refers to algorithms meant to evaluate numerically
integrals. Such an algorithm is also known as quadarature rule.

# 1D integration 

## Gauss-Kronrod adapative scheme

The `GaussKronrodQuadrature` class provides an implementation of the
Kronrod's 15 pts quadrature rule with an estimate of the error based on
the comparison of Gauss' 7pts rule with only the 15 evaluations of the
function required by the Kronrod's quadrature.

> **Note**
>
> The implementation in `TFEL/Math` is strongly inspired by the
> implementation available in the `boost::math` library.

### Treatment of unbouded intervals

If any of the integration interval is finite, a change of variable is
automatically perform to perform an integration on the interval \([-1,
1]\):

- when integrating from \([-\infty, b]\), the following change of
  variable is performed:
  \[
  u\left(x\right)= b-\left(\frac{2}{1+x}-1\right)
  \]
- when integrating from \([a, \infty]\), the following change of
  variable is performed:
  \[
  u\left(x\right)= \left(\frac{2}{1+x}-1\right) +a
  \]
- when integrating from \([-\infty, \infty]\), the following change of
  variable is performed:
  \[
  u\left(x\right) = \frac{x}{1 - x^2}
  \]

This estimation of the error can be used to refine the integration
interval to ensure that the final error is below a given bound.

### Adaptative refinements

The estimate of the error allows to divide the integration interval
\([a, b]\) to match a user defined tolerance. In the tolerance, the
interval is divided in \(2\) and in the integration is performed
recusively on the left interval and the right interval: on each of those
intervals, the tolerance is divided by two, which is conservative. This
procedure is performed up to a user defined maximum number of
refinements.

> **Note**
>
> Note that if \(n\) is the maximum number of refinements,
> the minimal interval is as small as \(\frac{b-a}{2^{n}}\).


### Example of usage

#### Standard interval:

Let \(f_{1}\) be the defined by:
\[
f_{1}\left(x\right) = \exp\left(-x^2\right)
\]

The following code computes an approximation of the integral
\(\displaystyle\int_{0}^{10}f_{1}\left(x\right)\,\mathrm{d}x\):

~~~~{.cxx}
constexpr auto a = double{};
constexpr auto b = double{10};
const auto oi = tfel::math::gauss_kronrod_integrate(
    [](const double x) noexcept { return std::exp(-x * x); }, a, b,
    {.absolute_tolerance = 1e-12, .maximum_number_of_refinements = 8});
~~~~

#### Unbounded intervals

Let \(f_{2}\) be the defined by:
\[
f_{2}\left(x\right) = \frac{\mathrm{d}}{\mathrm{d}x}\left(\arctan(x)\right)=\frac{1}{1+x^2}
\]

The following code computes an approximation of the integral
\(\displaystyle\int_{-\infty}^{0}f_{2}\left(x\right)\,\mathrm{d}x\):

~~~~{.cxx}
const auto or2 = tfel::math::gauss_kronrod_integrate(
    f1, -std::numeric_limits<double>::max(), double{},
    {.absolute_tolerance = 1e-12, .maximum_number_of_refinements = 8});
~~~~

The following code computes an approximation of the integral
\(\displaystyle\int_{0}^{\infty}f_{2}\left(x\right)\,\mathrm{d}x\):

~~~~{.cxx}
const auto or3 = tfel::math::gauss_kronrod_integrate(
    f1, double{}, std::numeric_limits<double>::max(),
    {.absolute_tolerance = 1e-12, .maximum_number_of_refinements = 8});
~~~~

The following code computes an approximation of the integral
\(\displaystyle\int_{-\infty}^{\infty}f_{2}\left(x\right)\,\mathrm{d}x\):

~~~~{.cxx}
const auto or4 = tfel::math::gauss_kronrod_integrate(
    f1, -std::numeric_limits<double>::max(),
    std::numeric_limits<double>::max(),
    {.absolute_tolerance = 1e-12, .maximum_number_of_refinements = 8});
~~~~

#### Vector-valuated function

Let \(f_{3}\) be the defined by:
\[
f_{3}\left(x\right) = 
\begin{pmatrix}
\sin\left(x\right)\\
\cos\left(x\right)
\end{pmatrix}
\]

The following code computes an approximation of the integral
\(\displaystyle\int_{0}^{\pi/4}f_{3}\left(x\right)\,\mathrm{d}x\):

~~~~{.cxx}
constexpr auto a = double{};
constexpr auto b = std::numbers::pi_v<double> / 4;
const auto or = tfel::math::gauss_kronrod_integrate(
    [](const double x) noexcept -> tfel::math::tvector<2u, double> {
      return {std::sin(x), std::cos(x)};
    },
    a, b,
    {.absolute_tolerance = 1e-12, .maximum_number_of_refinements = 8});
~~~~

#### Support of quantities

The following snippet shows that quantities are also supported:

~~~~{.cxx}
constexpr auto a = time{};
constexpr auto b = time{3600};
constexpr auto c = tfel::math::derivative_type<strainrate, time>{2.3e-5};
const auto or1 = tfel::math::gauss_kronrod_integrate(
    [c](const time t) noexcept { return c * t; }, a, b);
~~~~