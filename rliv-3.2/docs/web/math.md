% The TFEL/Math library
% Thomas Helfer
% 22/12/2017

\newcommand{\absvalue}[1]{{\left|#1\right|}}
\newcommand{\Frac}[2]{\displaystyle\frac{\displaystyle #1}{\displaystyle #2}}
\newcommand{\paren}[1]{\left(#1\right)}
\newcommand{\deriv}[2]{\Frac{\partial #1}{\partial #2}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\sigmaH}{\sigma_{H}}

# The `TFELMath` library

The `TFEL/Math` library defines:

- mathematical objects, such as second and fourth order tensors. See
  [this page](tensors.html) for details.

# The `TFELMathCubicSpline` library

# The `TFELMathKriging` library

# The `TFELMathParser` library

## The `Evaluator` class

The `Evaluator` class is used to interpret textual formula, as
follows:

~~~~{.cxx}
Evalutator ev("sin(x)");
ev.setVariableValue("x",12);
const auto s = ev.getValue();
~~~~

A shorthand for the previous code is the following:

~~~~{.cxx}
Evalutator ev("sin(x)");
const auto s = ev.getValue({{x:12}});
~~~~

The `getValue` method can also be replaced by the call operator, as
follows:

~~~~{.cxx}
Evalutator ev("sin(x)");
const auto s = ev({{x:12}});
~~~~

### Standard operations

Let `x` and `y` be two variables, then:

- `x+y`: computes the sum of \(x\) and \(y\).
- `x-y`: computes \(x\) minus \(y\).
- `x*y`: computes the product of \(x\) and \(y\).
- `x/y`: computes \(x\) divided by \(y\).
- `x**y`: computes \(x^{y}\).

The standard priority of those operators has been
respected. Parenthesis may be used to define sub-expressions.

### Standard function and constants

This paragraph gives a list of functions known by the `Evaluator`
class.

#### Constants

The physical constants described in
[this page](physical-constants.html) are accessible through the `Cste`
namespace, as follows:

~~~~{.cxx}
Evalutator ev("exp(-Q/(Cste::R*T))");
~~~~

#### Unary functions

Most unary function defined by the `C++11` standard have been bind and
can directly be used in formulae that can be evaluated by the
`Evaluator` class:

- `exp`: returns the base-e exponential.
- `exp2`: returns the base-2 exponential.
- `expm1`: returns the base-e exponential minus one.
- `sqrt`: returns the square root
- `cbrt`: returns the cubic root
- `ln`: computes the logarithm of the argment.
- `log`: computes the logarithm of the argment (synonymous of `ln`).
- `log10`: computes the base-10 logarithm of the argment.
- `log2`: computes the base-2 logarithm of the argment.
- `log1p`: computes the logarithm of the argment plus one.
- `cosh`:  computes the hyperbolic cosine.
- `sinh`:  computes the hyperbolic sine.
- `tanh`:  computes the hyperbolic tangent.
- `acosh`: computes the inverse of the hyperbolic cosine.
- `asinh`: computes the inverse of the hyperbolic sine.
- `atanh`: computes the inverse of the hyperbolic tangent.
- `abs`: computes absolute value.
- `cos`: computes the cosine.
- `sin`: computes the sine.
- `tan`:  computes the tangent.
- `acos`: computes the arccosine.
- `asin`: computes the arcsine.
- `atan`: computes the arctangent.
- `erf`: computes the error function.
- `erfc`: computes the complementary error function.
- `tgamma`: computes the Gamma function.
- `lgamma`: compute the natural logarithm of the gamma function.

Additionally, the following functions have been introduced:

- `H`: Heavyside function
- `power<N>` (\(1\,\leq\,N\,\leq\,16\)): raises the argument to power
  \(N\).

> **Notes**
>
> 1. Arguments of trigonometric functions are expressed in radians.
> 2. This functions are wrapped so that errors, such as invalid domain
>    for the arguments, are turned into C++ exceptions. See
>    Section&nbsp;@sec:Evaluator:error below.

#### Binary functions

- `max`: returns the minimum of its two arguments.
- `min`: returns the maximum of its two arguments.
- `hypot`: returns the hypotenuse of a right-angled triangle whose
  legs are x and y, i.e. computes \(\sqrt{x^{2}+y^{2}}.
- `atan2`: returns the principal value of the arc tangent of \(y/x\),
  expressed in radians.

### Error handling when calling the `getValue` method{#sec:Evaluator:error}

The mathematical functions described earlier are provided by the `C++`
language and reports error using the `errno` variable. The `Evaluator`
class wraps those functions to catch modification to the `errno`
variable and change it into `C++` exceptions.

<!-- Local IspellDict: english -->
