% New functionalities of the 3.2 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2017

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

The page declares the new functionalities of the 3.2 version of
the `TFEL` project.

The `TFEL` project is a collaborative development of
[CEA](http://www.cea.fr/english-portal "Commissariat à l'énergie
atomique") and [EDF](http://www.edf.com/ "Électricité de France")
dedicated to material knowledge manangement with special focus on
mechanical behaviours. It provides a set of libraries (including
`TFEL/Math` and `TFEL/Material`) and several executables, in
particular `MFront` and `MTest`.

`TFEL` is available on a wide variety of operating systems and
compilers.

# Documentation

A new page dedicated to the `python` bindings of the `TFEL` libraries
is available [here](tfel-python.html).

# Updates in `TFEL` libraries

The `TFEL` project provides several libraries. This paragraph is about
updates made in those libraries.

## TFEL/Math

### Improvements to the `Evaluator` class

The `Evaluator` class is used to interpret textual formula, as
follows:

~~~~{.cxx}
Evalutator ev("sin(x)");
ev.setVariableValue("x",12);
const auto s = ev.getValue();
~~~~

#### An overload for the `getValue` method

In the previous example, each variable value had to be set using the
`setVariableValue` method. The new overloaded version of the
`getValue` method can take a map as argument as follows:

~~~~{.cxx}
Evalutator ev("sin(x)");
const auto s = ev.getValue({{"x",12}});
~~~~

#### `Operator()`

Two overloaded versions of the `Evaluator::operator()` has been
introduced as a synonyms for the `getValue` method:

~~~~{.cxx}
Evalutator ev("sin(x)");
const auto s = ev({{"x",12}});
~~~~

#### The `getCxxFormula` method

The `getCxxFormula` method returns a string representing the
evaluation of the formula in standard `C++`. This method takes a map
as argument which describes how certain variables shall be
represented. This method can be used, as follows:

~~~~{.cxx}
Evalutator ev("2*sin(x)");
std::cout << ev({"x":"this->x"}}) << '\n';
~~~~

The previous code displays:

~~~~{.sh}
sin((2)*(this->x))
~~~~

This function is the basis of a new functionality of the `MFront` code
generator (inline material properties), see
Section&nbsp;@sec:inline-mprops for details.

#### New mathematical functions

The following new mathematical functions have been introduced:

- `exp2`: returns the base-2 exponential.
- `expm1`: returns the base-e exponential minus one.
- `cbrt`: returns the cubic root
- `log2`: computes the base-2 logarithm of the argment.
- `log1p`: computes the logarithm of the argment plus one.
- `acosh`: computes the inverse of the hyperbolic cosine.
- `asinh`: computes the inverse of the hyperbolic sine.
- `atanh`: computes the inverse of the hyperbolic tangent.
- `erf`: computes the error function.
- `erfc`: computes the complementary error function.
- `tgamma`: computes the Gamma function.
- `lgamma`: compute the natural logarithm of the gamma function.
- `hypot`: returns the hypotenuse of a right-angled triangle whose
  legs are x and y, i.e. computes \(\sqrt{x^{2}+y^{2}}.
- `atan2`: returns the principal value of the arc tangent of \(y/x\),
  expressed in radians.

## TFEL/Material

### \(\pi\)-plane

![Comparison of the isosurfaces of various equivalent stresses (Tresca, von Mises, Hosford \(a=8\)) in the \(\pi\)-plane](img/IsotropicEquivalentStressesInPiPlane.svg
 "Comparison of the isosurfaces of various equivalent stresses
 (Tresca, von Mises, Hosford \(a=8\)) in the
 \(\pi\)-plane"){width=70%}

The \(\pi\)-plane is defined in the space defined by the three
eigenvalues \(S_{0}\), \(S_{1}\) and \(S_{2}\) of the stress by the
following equations:
\[
S_{0}+S_{1}+S_{2}=0
\]

This plane contains deviatoric stress states and is perpendicular to
the hydrostatic axis. A basis of this plane is given by the following
vectors:
\[
\vec{n}_{0}=
\frac{1}{\sqrt{2}}\,
\begin{pmatrix}
1  \\
-1 \\
0
\end{pmatrix}
\quad\text{and}\quad
\vec{n}_{1}=
\frac{1}{\sqrt{6}}\,
\begin{pmatrix}
-1 \\
-1 \\
 2
\end{pmatrix}
\]

This plane is used to characterize the iso-values of equivalent
stresses which are not sensitive to the hydrostatic pression.

Various functions are available:

- `projectOnPiPlane`: this function projects a stress state on the
  \(\pi\)-plane.
- `buildFromPiPlane`: this function builds a stress state, defined by
  its three eigenvalues, from its coordinate in the \(\pi\)-plane.

## `python` bindings

### `tfel.math` module

#### Updated bindings for the `stensor` class

The following operations are supported:

- addition of two symmetric tensors.
- substraction of two symmetric tensors.
- multiplication by scalar.
- in-place addition by a symmetric tensor.
- in-place substraction by a symmetric tensor.
- in-place multiplication by scalar.
- in-place division by scalar.

The following functions have been introduced:

- `makeStensor1D`: builds a \(1D\) symmetric tensor from a tuple of
  three values.
- `makeStensor2D`: builds a \(2D\) symmetric tensor from a tuple of
  three values.
- `makeStensor3D`: builds a \(3D\) symmetric tensor from a tuple of
  three values.

### `tfel.material` module

#### Bindings related to the \(\pi\)-plane

The following functions are available:

- `buildFromPiPlane`: returns a tuple containing the three eigenvalues
  of the stress corresponding to the given point in the \(\pi\)-plane.
- `projectOnPiPlane`: projects a stress state, defined its three
  eigenvalues or by a symmetric tensor, on the \(\pi\)-plane.

The following script shows how to build an isosurface of the von Mises
equivalent stress in the \(\pi\)-plane:

~~~~{.python}
from math import pi,cos,sin
import tfel.math     as tmath
import tfel.material as tmaterial

nmax = 100
for a in [pi*(-1.+(2.*i)/(nmax-1)) for i in range(0,nmax)]:
    s      = tmath.makeStensor1D(tmaterial.buildFromPiPlane(cos(a),sin(a)))
    seq    = tmath.sigmaeq(s);
    s  *= 1/seq;
    s1,s2  = tmaterial.projectOnPiPlane(s);
    print(s1,s2);
~~~~

#### Bindings related to the Hosford equivalent stress

The `computeHosfordStress` function, which compute the Hosford
equivalent stress, is available.

The following script shows how to print an iso-surface of the Hosford
equivalent stress in the \(\pi\)-plane:

~~~~{.python}
from math import pi,cos,sin
import tfel.math     as tmath
import tfel.material as tmaterial

nmax = 100
for a in [pi*(-1.+(2.*i)/(nmax-1)) for i in range(0,nmax)]:
    s      = tmath.makeStensor1D(tmaterial.buildFromPiPlane(cos(a),sin(a)))
    seq    = tmaterial.computeHosfordStress(s,8,1.e-12);
    s     /= seq;
    s1,s2  = tmaterial.projectOnPiPlane(s);
    print(s1,s2);
~~~~

#### Bindings related to the Barlat equivalent stress

The following functions are available:

- `makeBarlatLinearTransformation1D`: builds a \(1D\) linear
  transformation of the stress tensor.
- `makeBarlatLinearTransformation2D`: builds a \(2D\) linear
  transformation of the stress tensor.
- `makeBarlatLinearTransformation3D`: builds a \(3D\) linear
  transformation of the stress tensor.
- `computeBarlatStress`: computes the Barlat equivalent Barlat stress.

The following script shows how to print an iso-surface of the Barlat
equivalent stress for the 2090-T3 aluminum alloy in the \(\pi\)-plane
(see @barlat_linear_2005):

~~~~{.python}
from math import pi,cos,sin
import tfel.math     as tmath
import tfel.material as tmaterial

nmax = 100
l1 = tmaterial.makeBarlatLinearTransformation1D(-0.069888,0.079143,0.936408,
                                                0.524741,1.00306,1.36318,
                                                0.954322,1.06906,1.02377);
l2 = tmaterial.makeBarlatLinearTransformation1D(0.981171,0.575316,0.476741,
                                                1.14501,0.866827,-0.079294,
                                                1.40462,1.1471,1.05166);
for a in [pi*(-1.+(2.*i)/(nmax-1)) for i in range(0,nmax)]:
    s      = tmath.makeStensor1D(tmaterial.buildFromPiPlane(cos(a),sin(a)))
    seq    = tmaterial.computeBarlatStress(s,l1,l2,8,1.e-12);
    s     *= 1/seq;
    s1,s2  = tmaterial.projectOnPiPlane(s);
    print(s1,s2);
~~~~

# New functionalities in `MFront`

## Various improvements

### Inline material properties in mechanical behaviours {#sec:inline-mprops}

Various keywords (such as `@ElasticMaterialProperties`,
`@ComputeThermalExpansion`, `@HillTensor`, etc.)  expects one or more
material properties. In previous versions, those material properties
were constants or defined by an external `MFront`.

This new version allows those material properties to be defined by
formulae, as follows:

~~~~{.cxx}
@Parameter E0 =2.1421e11,E1 = -3.8654e7,E2 = -3.1636e4;
@ElasticMaterialProperties {"E0+(T-273.15)*(E1+E2*(T-273.15))",0.3}
~~~~

As for material properties defined in external `MFront` files, the
material properties evaluated by formulae will be computed for updated
values of their parameters. For example, if the previous lines were
used in the `Implicit` DSL, two variables `young` and `young_tdt` will
be automatically made available:

- the `young` variable will be computed for the temperature
  \(T+\theta\,\Delta\,T\).
- the `young_tdt` variable will be computed for the temperature
  \(T+\Delta\,T\).

# References

<!-- Local IspellDict: english -->
