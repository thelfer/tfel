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

### Efficient computations of the first and second derivatives of the invariants of the stress deviator tensor with respect to the stress {#sec:deviatoric:invariants}

Let \(\tsigma\) be a stress tensor. Its deviatoric part
\(\tenseur{s}\) is:

\[
\tenseur{s}=\tsigma-\Frac{1}{3}\,\trace{\tsigma}\,\tenseur{I}
=\paren{\tenseurq{I}-\Frac{1}{3}\,\tenseur{I}\,\otimes\,\tenseur{I}}\,\colon\,\tsigma
\]

The deviator of a tensor can be computed using the `deviator`
function.

As it is a second order tensor, the stress deviator tensor also has a
set of invariants, which can be obtained using the same procedure used
to calculate the invariants of the stress tensor. It can be shown that
the principal directions of the stress deviator tensor \(s_{ij}\) are
the same as the principal directions of the stress tensor
\(\sigma_{ij}\). Thus, the characteristic equation is

\[
\left| s_{ij}- \lambda\delta_{ij} \right| = -\lambda^3+J_1\lambda^2-J_2\lambda+J_3=0,
\]

where \(J_1\), \(J_2\) and \(J_3\) are the first, second, and third
*deviatoric stress invariants*, respectively. Their values are the same
(invariant) regardless of the orientation of the coordinate system
chosen. These deviatoric stress invariants can be expressed as a
function of the components of \(s_{ij}\) or its principal values \(s_1\),
\(s_2\), and \(s_3\), or alternatively, as a function of \(\sigma_{ij}\) or
its principal values \(\sigma_1\), \(\sigma_2\), and \(\sigma_3\). Thus,

\[
\begin{aligned}
J_1 &= s_{kk}=0,\, \\
J_2 &= \textstyle{\frac{1}{2}}s_{ij}s_{ji} = \Frac{1}{2}\trace{\tenseur{s}^2}\\
&= \Frac{1}{2}(s_1^2 + s_2^2 + s_3^2) \\
&= \Frac{1}{6}\left[(\sigma_{11} - \sigma_{22})^2 + (\sigma_{22} - \sigma_{33})^2 + (\sigma_{33} - \sigma_{11})^2 \right ] + \sigma_{12}^2 + \sigma_{23}^2 + \sigma_{31}^2 \\
&= \Frac{1}{6}\left[(\sigma_1 - \sigma_2)^2 + (\sigma_2 - \sigma_3)^2 + (\sigma_3 - \sigma_1)^2 \right ] \\
&= \Frac{1}{3}I_1^2-I_2 = \frac{1}{2}\left[\trace{\tenseur{\sigma}^2} - \frac{1}{3}\trace{\tenseur{\sigma}}^2\right],\,\\
J_3 &= \det\paren{\tenseur{s}} \\
&= \Frac{1}{3}s_{ij}s_{jk}s_{ki} = \Frac{1}{3} \trace{\tenseur{s}^3}\\
&= \Frac{1}{3}(s_1^3 + s_2^3 + s_3^3) \\
&= s_1s_2s_3 \\
&= \Frac{2}{27}I_1^3 - \Frac{1}{3}I_1 I_2 + I_3 = \Frac{1}{3}\left[\trace{\tenseur{\sigma}^3} - \trace{\tenseur{\sigma}^2}\trace{\tenseur{\sigma}} +\Frac{2}{9}\trace{\tenseur{\sigma}}^3\right].
\end{aligned}
\]

where \(I_{1}\), \(I_{2}\) and \(I_{3}\) are the invariants of
\(\tsigma\).

\(J_{2}\) and \(J_{3}\) are building blocks for many isotropic yield
critera. Classically, \(J_{2}\) is directly related to the von Mises
stress \(\sigmaeq\):

\[
\sigmaeq=\sqrt{\Frac{3}{2}\,\tenseur{s}\,\colon\,\tenseur{s}}=\sqrt{3\,J_{2}}
\]

The first and second derivatives of \(J_{2}\) with respect to
\(\sigma\) can be trivially implemented, as follows:

~~~~{.cpp}
constexpr const auto id  = stensor<N,real>::Id();
constexpr const auto id4 = st2tost2<N,real>::Id();
// first derivative of J2
const auto dJ2  = deviator(sig);
// second derivative of J2
const auto d2J2 = eval(id4-(id^id)/3);
~~~~

In comparison, the computation of the first and second derivatives of
\(J_{3}\) with respect to \(\sigma\) are more cumbersome. In previous
versions `TFEL`, one had to write:

~~~~{.cpp}
constexpr const auto id = stensor<N,real>::Id();
constexpr const auto id4 = st2tost2<N,real>::Id();
const auto I1   = trace(sig);
const auto I2   = (I1*I1-trace(square(sig)))/2;
const auto dI2  = I1*id-sig;
const auto dI3  = computeDeterminantDerivative(sig);
const auto d2I2 = (id^id)-id4;
const auto d2I3 = computeDeterminantSecondDerivative(sig);
// first derivative of J3
const auto dJ3  = eval((2*I1*I1/9)*id-(I2*id+I1*dI2)/3+dI3);
// second derivative of J3
const auto d2J3 = eval((4*I1/9)*(id^id)-((id^dI2)+(dI2^id)+i1*d2I2)/3+d2I3);
~~~~

More efficient implementations are now available using the
`computeDeviatorDeterminantDerivative` and
`computeDeviatorDeterminantSecondDerivative` functions:

~~~~{.cpp}
// first derivative of J3
const auto dJ3  = computeDeviatorDeterminantDerivative(sig);
// second derivative of J3
const auto d2J3 = computeDeviatorDeterminantSecondDerivative(sig);
~~~~

## TFEL/Material

### Isotropic Plasticity

By definition, \(J_{2}\) and \(J_{3}\) are the second and third
invariants of the deviatoric part \(\tenseur{s}\) of the stress tensor
\(\tsigma\) (see also Section&nbsp;@sec:deviatoric:invariants):

\[
\left\{
\begin{aligned}
J_2 &= \Frac{1}{2}\trace{\tenseur{s}^2}\\
J_3 &= \det(\tenseur{s}) \\
\end{aligned}
\right.
\]

The first and second derivatives of \(J_{2}\) with respect to the
stress tensor \(\tsigma\) are trivially computed and implemented (see
Section&nbsp;@sec:deviatoric:invariants).

The first and second derivatives of \(J_{2}\) with respect to the
stress tensor \(\tsigma\) can be computed respectively by:

- The `computesJ3Derivative` function, which is a synonym for the
  `computeDeviatorDeterminantDerivative` function defined in the
  `tfel::math` namespace (see
  Section&nbsp;@sec:deviatoric:invariants for details).
- The `computeJ3SecondDerivative` function, which is a synonym for the
  `computeDeviatorDeterminantSecondDerivative` function defined in the
  `tfel::math` namespace (see Section&nbsp;@sec:deviatoric:invariants
  for details).

### Orthotropic plasticity

Within the framework of the theory of representation, generalizations
to anisotropic conditions of the invariants of the deviatoric stress
have been proposed by Cazacu and Barlat (see
@cazacu_generalization_2001):

- The generalization of \(J_{2}\) is denoted \(J_{2}^{O}\). It is
  defined by:
  \[
  J_{2}^{O}= a_6\,s_{yz}^2+a_5\,s_{xz}^2+a_4\,s_{xy}^2+\frac{a_2}{6}\,(s_{yy}-s_{zz})^2+\frac{a_3}{6}\,(s_{xx}-s_{zz})^2+\frac{a_1}{6}\,(s_{xx}-s_{yy})^2
  \]
  where the \(\left.a_{i}\right|_{i\in[1:6]}\) are six coefficients
  describing the orthotropy of the material.
- The generalization of \(J_{3}\) is denoted \(J_{3}^{O}\). It is
  defined by:
  \[
  \begin{aligned}
  J_{3}^{O}=
  &\frac{1}{27}\,(b_1+b_2)\,s_{xx}^3+\frac{1}{27}\,(b_3+b_4)\,s_{yy}^3+\frac{1}{27}\,(2\,(b_1+b_4)-b_2-b_3)\,s_{zz}^3\\
  &-\frac{1}{9}\,(b_1\,s_{yy}+b_2s_{zz})\,s_{xx}^2\\
  &-\frac{1}{9}\,(b_3\,s_{zz}+b_4\,s_{xx})\,s_{yy}^2\\
  &-\frac{1}{9}\,((b_1-b_2+b_4)\,s_{xx}+(b_1-b3+b_4)\,s_{yy})\,s_{zz}^3\\
  &+\frac{2}{9}\,(b_1+b_4)\,s_{xx}\,s_{yy}\,s_{zz}\\
  &-\frac{s_{xz}^2}{3}\,(2\,b_9\,s_{yy}-b_8\,s_{zz}-(2\,b_9-b_8)\,s_{xx})\\
  &-\frac{s_{xy}^2}{3}\,(2\,b_{10}\,s_{zz}-b_5\,s_{yy}-(2\,b_{10}-b_5)\,s_{xx})\\
  &-\frac{s_{yz}^2}{3}\,((b_6+b_7)\,s_{xx}-b_6\,s_{yy}-b_7\,s_{zz})\\
  &+2\,b_{11}\,s_{xy}\,s_{xz}\,s_{yz}
  \end{aligned}
  \]
  where the \(\left.b_{i}\right|_{i\in[1:11]}\) are eleven coefficients
  describing the orthotropy of the material.

Those invariants may be used to generalize isotropic yield criteria
based on \(J_{2}\) and \(J_{3}\) invariants to orthotropy.

The following functions 

\(J_{2}^{0}\), \(J_{3}^{0}\) and their first and second derivatives
with respect to the stress tensor \(\tsigma\) can be computed
by the following functions:

- `computesJ2O`, `computesJ2ODerivative` and
  `computesJ2OSecondDerivative`.
- `computesJ3O`, `computesJ3ODerivative` and
  `computesJ3OSecondDerivative`.


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
