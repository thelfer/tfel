% The TFEL/Material library
% Thomas Helfer
% 19/12/2017

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

# Equivalent stress

## Hosford equivalent stress{#sec:hosford}

The header `TFEL/Material/Hosford1972YieldCriterion.hxx` introduces
three functions which are meant to compute the Hosford equivalent
stress and its first and second derivatives. *This header is
automatically included by `MFront`*

The Hosford equivalent stress is defined by:
\[
\sigmaeq^{H}=\sqrt[a]{\Frac{1}{2}\paren{\absvalue{\sigma_{1}-\sigma_{2}}^{a}+\absvalue{\sigma_{1}-\sigma_{3}}^{a}+\absvalue{\sigma_{2}-\sigma_{3}}^{a}}}
\]
where \(s_{1}\), \(s_{2}\) and \(s_{3}\) are the eigenvalues of the
stress.

Therefore, when \(a\) goes to infinity, the Hosford stress reduces to
the Tresca stress. When \(n = 2\) the Hosford stress reduces to the
von Mises stress.

The following functions has been implemented:

- `computeHosfordStress`: return the Hosford equivalent stress
- `computeHosfordStressNormal`: return a tuple containing the Hosford
  equivalent stress and its first derivative (the normal)
- `computeHosfordStressSecondDerivative`: return a tuple containing
  the Hosford equivalent stress, its first derivative (the normal) and
  the second derivative.

#### Example

The following example computes the Hosford equivalent stress, its
normal and second derivative:

~~~~{.cpp}
stress seq;
Stensor  n;
Stensor4 dn;
std::tie(seq,n,dn) = computeHosfordStressSecondDerivative(s,a,seps);
~~~~

In this example, `s` is the stress tensor, `a` is the Hosford
exponent, `seps` is a numerical parameter used to detect when two
eigenvalues are equal.

If `C++-17` is available, the previous code can be made much more readable:

~~~~{.cpp}
const auto [seq,n,dn] = computeHosfordStressSecondDerivative(s,a,seps);
~~~~

## Barlat equivalent stress{#sec:barlat}

The header `TFEL/Material/Barlat2004YieldCriterion.hxx` introduces
various functions which are meant to compute the Barlat equivalent
stress and its first and second derivatives. *This header is
automatically included by `MFront`* for orthotropic behaviours.

The Barlat equivalent stress is defined as follows (see
@barlat_linear_2005):
\[
\sigmaeq^{B}=
\sqrt[a]{
  \frac{1}{4}\left(
  \sum_{i=0}^{3}
  \sum_{j=0}^{3}
  \absvalue{s'_{i}-s''_{j}}^{a}
  \right)
}
\]

where \(s'_{i}\) and \(s''_{i}\) are the eigenvalues of two
transformed stresses \(\tenseur{s}'\) and \(\tenseur{s}''\) by two
linear transformation \(\tenseurq{L}'\) and \(\tenseurq{L}''\):
\[
\left\{
\begin{aligned}
\tenseur{s}'  &= \tenseurq{L'} \,\colon\,\tsigma \\
\tenseur{s}'' &= \tenseurq{L''}\,\colon\,\tsigma \\
\end{aligned}
\right.
\]

The linear transformations \(\tenseurq{L}'\) and \(\tenseurq{L}''\)
are defined by \(9\) coefficients (each) which describe the material
orthotropy. There are defined through auxiliary linear transformations
\(\tenseurq{C}'\) and \(\tenseurq{C}''\) as follows:
\[
\begin{aligned}
\tenseurq{L}' &=\tenseurq{C}'\,\colon\,\tenseurq{M} \\
\tenseurq{L}''&=\tenseurq{C}''\,\colon\,\tenseurq{M}
\end{aligned}
\]
where \(\tenseurq{M}\) is the transformation of the stress to its deviator:
\[
\tenseurq{M}=\tenseurq{I}-\Frac{1}{3}\tenseur{I}\,\otimes\,\tenseur{I}
\]

The linear transformations \(\tenseurq{C}'\) and \(\tenseurq{C}''\) of
the deviator stress are defined as follows:
\[
\tenseurq{C}'=
\begin{pmatrix}
0 & -c'_{12} & -c'_{13} & 0 & 0 & 0 \\
-c'_{21} & 0 & -c'_{23} & 0 & 0 & 0 \\
-c'_{31} & -c'_{32} & 0 & 0 & 0 & 0 \\
0 & 0 & 0 & c'_{44} & 0 & 0 \\
0 & 0 & 0 & 0 & c'_{55} & 0 \\
0 & 0 & 0 & 0 & 0 & c'_{66} \\
\end{pmatrix}
\quad
\text{and}
\quad
\tenseurq{C}''=
\begin{pmatrix}
0 & -c''_{12} & -c''_{13} & 0 & 0 & 0 \\
-c''_{21} & 0 & -c''_{23} & 0 & 0 & 0 \\
-c''_{31} & -c''_{32} & 0 & 0 & 0 & 0 \\
0 & 0 & 0 & c''_{44} & 0 & 0 \\
0 & 0 & 0 & 0 & c''_{55} & 0 \\
0 & 0 & 0 & 0 & 0 & c''_{66} \\
\end{pmatrix}
\]

The following functions have been implemented:

- `computeBarlatStress`: return the Barlat equivalent stress
- `computeBarlatStressNormal`: return a tuple containing the Barlat
  equivalent stress and its first derivative (the normal)
- `computeBarlatStressSecondDerivative`: return a tuple containing the
  Barlat equivalent stress, its first derivative (the normal) and the
  second derivative.

### Linear transformations

To define the linear transformations, the
`makeBarlatLinearTransformation` function has been introduced. This
function takes two template parameter:

- the space dimension (\(1\), \(2\), and \(3\))
- the numeric type used (automatically deduced)

This functions takes the \(9\) coefficients as arguments, as follows:

~~~~{.cpp}
const auto l1 = makeBarlatLinearTransformation<3>(c_12,c_21,c_13,c_31,
                                                  c_23,c_32,c_44,c_55,c_66);
~~~~

> **Note** In his paper, Barlat and coworkers uses the following convention for
> storing symmetric tensors:
> 
> \[
> \begin{pmatrix}
> xx & yy & zz & yz & zx & xy
> \end{pmatrix}
> \]
> 
> which is not consistent with the
> `TFEL`/`Cast3M`/`Abaqus`/`Ansys` conventions:
> 
> \[
> \begin{pmatrix}
> xx & yy & zz & xy & xz & yz
> \end{pmatrix}
> \]
> 
> Therefore, if one wants to uses coeficients \(c^{B}\) given
> by Barlat, one shall call this function as follows:
> 
> ~~~~{.cpp}
> const auto l1 = makeBarlatLinearTransformation<3>(cB_12,cB_21,cB_13,cB_31,
>                                                   cB_23,cB_32,cB_66,cBB_55,cBB_44);
> ~~~~

The `TFEL/Material` library also provide an overload of the
`makeBarlatLinearTransformation` which template parameters are the
modelling hypothesis and the orthotropic axis conventions. The purpose
of this overload is to swap appriopriate coefficients to get a
consistent definition of the linear transforamtions for all the
modelling hypotheses.

# General functionalities

## Computation of the inverse of the Langevin function

The inverse Langevin function is used in many statistically based
network behaviours describing rubber-like materials.

The Langevin function \(\mathcal{L}\) is defined as follows:
\[
\mathcal{L}\paren{x}=\Frac{1}{\coth\paren{x}}-\Frac{1}{x}
\]

The complexity of the inverse Langevin function
\(\mathcal{L}^{-1}\paren{x}\) motivated the development of various
approximations
[@cohen_pade_1991;@bergstrom_constitutive_1998;@jedynak_approximation_2015].

![Comparison of various approximations of the inverse Langenvin
function](img/InverseLangevinFunction.svg){#fig:tfel_4.1:tfel_material:inverse_langevin_function
width=95%}

Figure @fig:tfel_4.1:tfel_material:inverse_langevin_function compares
those approximations. The approximations of Bergström and Boyce
[@bergstrom_constitutive_1998] and Jedynak [@jedynak_approximation_2015]
are undistinguishable. See Jedynak for a quantitative discussion of the
error generated by those approximations [@jedynak_approximation_2015].
It is worth noting that all those approximations mostly differs near the
pole of inverse Langevin function \(\mathcal{L}^{-1}\paren{x}\).

The `InverseLangevinFunctionApproximations` enumeration lists the
approximations that have been implemented and that can be evaluated in a
`constexpr` context:

- The value `COHEN_1991` is associated with the approximation proposed by Cohen [@cohen_pade_1991]:
  \[
  \mathcal{L}^{-1}\paren{x} \approx
  y\Frac{3-y^{2}}{1-y^{2}}.
  \]
- The value `JEDYNAK_2015` is associated with the approximation proposed
  by Jedynak [@jedynak_approximation_2015]:
  \[
  \mathcal{L}^{-1}\paren{x} \approx
  y \, \Frac{c_{0} + c_{1}\,y + c_{2}\,y^{2}}{1 + d_{1}\,y + d_{2}\,y^{2}}
  \]
- `KUHN_GRUN_1942` or `MORCH_2022` are associated with a taylor expansion of
  \(\mathcal{L}^{-1}\paren{x}\) at \(0\):
  \[
  \mathcal{L}^{-1}\paren{x} \approx
  y\,P\paren{y^{2}} \quad\text{with}\quad P\paren{y^{2}}=\sum_{i=0}^{9}c_{i}\,\paren{y^{2}}^{i}
  \]
  where \(P\) is a \(9\)th order polynomial. Hence, the
  Taylor expression is of order \(19\).

The `computeApproximateInverseLangevinFunction` computes one
approximation of the inverse Langevin function and the
`computeApproximateInverseLangevinFunctionAndDerivative` function
computes ne approximation of the inverse Langevin function and its
derivative. These functions have two template parameters: the
approximation selected and the numeric type to be used. By default, the
`JEDYNAK_2015` approximation is used and the numeric type can be deduced
from the type of the argument.

The approximation proposed by Bergström and Boyce
[@bergstrom_constitutive_1998] is given by the following function:
\[
\mathcal{L}^{-1}\paren{x}
\approx
\left\{
\begin{aligned}
c_{1} \tan\paren{c_{2} \, x} + c_{3} \, x &\quad\text{if}\quad \left|x\right| \leq c_{0}\\
\Frac{1}{\mathop{sign}\paren{x}-x}&\quad\text{if}\quad \left|x\right| > c_{0}
\end{aligned}
\right.
\]

The `computeBergstromBoyce1998ApproximateInverseLangevinFunction`
function computes this approximation and the
`computeBergstromBoyce1998ApproximateInverseLangevinFunctionAndDerivative`
function computes this function and its derivative. These functions
can't be declared `constexpr` because of tangent function is not
`constexpr`. These functions have one template parameter, the numeric
type to be used. This template parameter can be automatically deduced
from the type of the argument.

## Example of usage

~~~~{.cxx}
using ApproximationFunctions = InverseLangevinFunctionApproximations;
// compute Cohen's approximation of the inverse Langevin function
const auto v = computeApproximateInverseLangevinFunction<
            ApproximationFunctions::COHEN_1991>(y)
// compute Jedynak's approximation of the inverse Langevin function and its derivative
const auto [f, df] = computeApproximateInverseLangevinFunctionAndDerivative(y)
~~~~

## \(\pi\)-plane

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

# Orthotropic axes convention

Most finite element solver can't have a unique definition of the
orthotropic axes  valied for all the modelling hypotheses.

For example, one can define a pipe using the following axes
definition:

- \(\left(rr,zz,tt,...\right)\) in \(3D\), \(2D\) axysymmetric, \(1D\)
  axisymmetric generalised plane strain or \(1D\)
  axisymmetric generalised plane stress.
- \(\left(rr,tt,zz,...\right)\) in \(2D\) plane stress, \(2D\) plane
  strain and \(2D\) generalized plane strain.

![The `Pipe` orthotropic axes convention for \(3D\), \(2D\)
  axysymmetric, \(1D\) axisymmetric generalised plane strain or
  generalised plane stress (left) and \(2D\) plane stress, strain,
  generalized plane strain (right)](img/pipe.svg "The `Pipe`
  orthotropic axes convention for \(3D\), \(2D\) axysymmetric, \(1D\)
  axisymmetric generalised plane strain or generalised plane stress
  (left) and \(2D\) plane stress, strain, generalized plane strain
  (right)"){width=50%}

With those conventions, named `Pipe` in `MFront`, the axial direction is
either the second or the third material axis, a fact that must be taken
into account when defining the stiffness tensor, the Hill tensor(s), the
thermal expansion, etc.

This convention is only valid for \(3D\), \(2D\) axysymmetric, \(1D\)
  axisymmetric generalised plane strain or generalised plane stress.
- \(\left(rr,tt,zz,...\right)\) in \(2D\) plane stress, strain,
  generalized plane strain.

![The `Plate` orthotropic axes convention](img/plate.svg "The `Plate`
  orthotropic axes convention"){width=50%}

If we were to model plates, a appropriate convention is the following:

- The first material axis is the rolling direction
- The second material axis is the in plane direction perpendicular to
  the rolling direction (transverse direction).
- The third material axis is the normal to the plate.

By definition, this convention, named `Plate` in `MFront` is only valid
for \(3D\), \(2D\) plane stress, \(2D\) plane strain and \(2D\)
generalized plane strain modelling hypotheses.

# Homogeneisation

## Eshelby tensor

The header `Eshelby.hxx` introduces
the function `EshelbyTensor` which computes the Eshelby tensor
of an ellipsoid.
If we consider a constant stress-free strain \(\tenseur \varepsilon^\mathrm{T}\)
filling an ellipsoidal volume embedded in an infinite homogeneous medium whose
elasticity is \(\tenseur C_0\), the strain tensor inside the ellipsoid is given by

\(\tenseur \varepsilon=\tenseur S_0:\tenseur \varepsilon^\mathrm{T}\).

where \(\tenseur S_0\) is the Eshelby tensor.
Note that it is related to the Hill tensor (\tenseur P_0\) by

\(\tenseur P_0=\tenseur S_0:\tenseur C_0^{-1}\)

which gives the strain tensor inside the ellipsoid as a function of the
polarization tensor \(\tenseur \tau = -\tenseur C_0:\tenseur \varepsilon^\mathrm{T}\) : 

\(\tenseur \varepsilon=-\tenseur P_0:\tenseur \tau\).

The function `EshelbyTensor` computes the Eshelby tensor of an ellipsoid
whose semi-axis lengths are `a`, `b`, `c`, embedded in an isotropic
matrix. It returns an object of `type st2tost2<3u,real>`, which is the
fourth-order Eshelby tensor, in a basis which is adapted to the ellipsoid.
The expressions can be found in Torquato, Random Heterogeneous Materials (2002)
for the axisymetric ellipsoid and in Eshelby (1957) for other cases.

## Strain localisation tensors

The header `Eshelby.hxx` also introduces
three functions that compute the strain localisation tensor of an ellipsoid.
If we consider an ellipsoid whose elasticity is \(\tenseur C_i\), embedded
in an infinite homogeneous medium whose elasticity is \(\tenseur C_0\),
submitted to a external uniform strain field at infinity \(\tenseur E\),
the strain field within the ellipsoid is uniform and given by

\(\tenseur \varepsilon = \tenseur A:\tenseur E\)

where \(\tenseur A \) is the localisation tensor.

Three functions are implemented for the different possible shapes :
GeneralEllipsoidLocalisationTensor, AxisymEllipsoidLocalisationTensor
and SphereLocalisationTensor. 
The ellipsoid is parametrized by its semi-axis lengths \(a,b,c\) but also
by its axis orientations.
The functions then return the localisation tensors taking into account the orientations.

<!-- Local IspellDict: english -->
