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
> Therefore, if one wants to use coefficients \(c^{B}\) given
> by Barlat, one shall call this function as follows:
> 
> ~~~~{.cpp}
> const auto l1 = makeBarlatLinearTransformation<3>(cB_12,cB_21,cB_13,cB_31,
>                                                   cB_23,cB_32,cB_66,cBB_55,cBB_44);
> ~~~~

The `TFEL/Material` library also provide an overload of the
`makeBarlatLinearTransformation` which template parameters are the
modelling hypothesis and the orthotropic axis conventions. The purpose
of this overload is to swap appropriate coefficients to get a
consistent definition of the linear transformations for all the
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
computes an approximation of the inverse Langevin function and its
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
stresses which are not sensitive to the hydrostatic pressure.

Various functions are available:

- `projectOnPiPlane`: this function projects a stress state on the
  \(\pi\)-plane.
- `buildFromPiPlane`: this function builds a stress state, defined by
  its three eigenvalues, from its coordinate in the \(\pi\)-plane.

# Orthotropic axes convention

Most finite element solver can't have a unique definition of the
orthotropic axes valid for all the modelling hypotheses.

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

If we were to model plates, an appropriate convention is the following:

- The first material axis is the rolling direction
- The second material axis is the in plane direction perpendicular to
  the rolling direction (transverse direction).
- The third material axis is the normal to the plate.

By definition, this convention, named `Plate` in `MFront` is only valid
for \(3D\), \(2D\) plane stress, \(2D\) plane strain and \(2D\)
generalized plane strain modelling hypotheses.

# Isotropic elastic moduli

Three data structures are defined to represent the isotropic moduli of
an isotropic material:

 - `KGModuli` (attributes: `kappa`,`mu`)
 - `YoungNuModuli` (attributes: `young`,`nu`)
 - `LambdaMuModuli` (attributes: `lambda`,`mu`)

It can be constructed as follows:

~~~~{.cpp}
const auto KG = KGModuli<stress>(ka,mu);
~~~~
 
And its attributes can be recovered as follows:

~~~~{.cpp}
const auto K = KG.kappa;
const auto G = KG.mu;
~~~~

It can  be converted to the other describing isotropic moduli as follows:

~~~~{.cpp}
const auto Enu = KG.ToYoungNu();
const auto LambdaMu = KG.ToLambdaMu();
~~~~

Moreover, some useful functions allow to go from one to another:

~~~~{.cpp}
const auto C = computeIsotropicStiffnessTensor<stress>(Enu);
const auto KG = computeKGModuli<stress>(C);
~~~~

Note that `computeKGModuli` makes a projection on the fourth-order
tensors \(\tenseurq{J}\) and \(\tenseurq{K}\) if \(\tenseurq{C}\)
is not isotropic. It can be check that \(\tenseurq{C}\) is isotropic
by doing

~~~~{.cpp}
const auto eps = 1e-6;
const bool = isIsotropic(C,eps);
~~~~

Here, `isIsotropic` first projects \(\tenseurq{C}\) on
the isotropic basis, and constructs the isotropized of \(\tenseurq{C}\).
Then, it computes the relative difference between \(\tenseurq{C}\)
and its isotropized, by using the \(L2\)-norm. This difference is compared
to the tolerance `eps`.

# Homogenization

The homogenization functions are part of the namespace `tfel::material::homogenization`.
A specialization for elasticity is defined: `tfel::material::homogenization::elasticity`.

## Eshelby, Hill and localisation tensors

### Definitions

If we consider a constant stress-free strain \(\tenseur \varepsilon^\mathrm{T}\)
filling an ellipsoidal volume embedded in an infinite homogeneous medium whose
elasticity is \(\tenseurq{C}_0\), the strain tensor inside the ellipsoid is given by

\(\tenseur \varepsilon=\tenseurq S_0:\tenseur \varepsilon^\mathrm{T}\).

where \(\tenseurq S_0\) is the Eshelby tensor. The Hill tensor \(\tenseurq P_0\)
gives the strain tensor inside the ellipsoid as a function of the
polarization tensor \(\tenseur \tau = -\tenseurq C_0:\tenseur \varepsilon^\mathrm{T}\) : 

\(\tenseur \varepsilon=-\tenseurq P_0:\tenseur \tau\).

Note that \(\quad\tenseurq P_0=\tenseurq S_0:\tenseurq C_0^{-1}\)

The expressions of Eshelby tensor can be found in [@torquato_2002]
for the spheroidal inclusions and in [@eshelby_1957] for the general ellipsoid
(three different semi-axes).

Now if we consider an ellipsoid whose elasticity is \(\tenseurq C_i\), embedded
in an infinite homogeneous medium whose elasticity is \(\tenseurq C_0\),
submitted to a external uniform strain field at infinity \(\tenseur E\),
the strain field within the ellipsoid is uniform and given by

\(\tenseur \varepsilon = \tenseurq A:\tenseur E\)

where \(\tenseurq A \) is the strain localisation (or concentration) tensor.

### Computation in isotropic reference medium

#### Eshelby and Hill tensors

The header `IsotropicEshelbyTensor.hxx` introduces
the computation of the Eshelby tensors and Hill tensors
of general ellipsoids embedded in an isotropic medium.

We can compute the Hill tensors as follows:

~~~~{.cpp}
using namespace tfel::material::homogenization::elasticity;
const auto P0 = computeSphereHillPolarisationTensor<stress>(E0,nu0);
const auto P0_axi = computeAxisymmetricalHillPolarisationTensor<stress>(E0,nu0,n_a,e);
const auto P0_ellipsoid = computeHillPolarisationTensor<stress>(E0,nu0,n_a,a,n_b,b,c);
~~~~

Here, the first line compute the Hill tensor for a sphere.
The second one computes the Hill tensor for an axisymmetrical ellipsoid (or spheroidal inclusion).
The user must provides the normal vector `n_a` for the axis, and `e` for the aspect ratio.
The third line computes the Hill tensor of a more general ellipsoid whose semi-axis lengths
are `a`,`b`,`c`. The axis `a` is related to direction given by `n_a` and `b` is related to the
direction given by `n_b`, which must be normal to `n_a`.

An `IsotropicModuli` can also be passed for the elasticity, as follows:

~~~~{.cpp}
const auto IM0=YoungNuModuli<stress>(E0,nu0);
const auto P0 = computeSphereHillPolarisationTensor<stress>(IM0);
const auto P0_axi = computeAxisymmetricalHillPolarisationTensor<stress>(IM0,n_a,e);
const auto P0_ellipsoid = computeHillPolarisationTensor<stress>(IM0,n_a,a,n_b,b,c);
~~~~

The Eshelby tensors can be computed as follows:

~~~~{.cpp}
const auto S0 = computeSphereEshelbyTensor<stress>(nu0);
const auto S0_axi = computeAxisymmetricalEshelbyTensor<stress>(nu0,e);
const auto S0_ellipsoid = computeEshelbyTensor<stress>(nu0,a,b,c);
~~~~

Note that the Eshelby tensors are not related to a basis, so that
it is recommended to use the Hill tensors instead.
In 2 dimensional framework, Eshelby tensors and Hill tensors are computed as follows:

~~~~{.cpp}
const auto S0_D = computeDiskPlaneStrainEshelbyTensor<stress>(nu0);
const auto S0_C = computePlaneStrainEshelbyTensor<stress>(nu0,e);

const auto IM0=YoungNuModuli<stress>(E0,nu0);
const auto P0_D = computeDiskPlaneStrainHillTensor<stress>(IM0);
const auto P0_C = computePlaneStrainHillTensor<stress>(IM0,n_a,a,b);
~~~~

The `computeDiskPlaneStrain` refers to a disk in plane strain framework,
whereas the `computePlaneStrain` refers to an ellipse oriented by `n_a`, in a
plane strain framework.

#### Localisation (or concentration) tensors

The header `LocalisationTensor.hxx` also introduces
the computation of the strain localisation tensors of an ellipsoid.
These localisation tensors can be computed as follows:

~~~~{.cpp}
const auto A = computeSphereLocalisationTensor<stress>(E0,nu0,Ei,nui);
const auto A_axi = computeAxisymmetricalLocalisationTensor<stress>(E0,nu0,Ei,nui,n_a,e);
const auto A_ellipsoid = computeLocalisationTensor<stress>(E0,nu0,Ei,nui,n_a,a,n_b,b,c);
~~~~

Here, the subscript `i` refers to the inclusion.
Here again, an `IsotropicModuli` can be passed for the elasticity, as follows:

~~~~{.cpp}
const auto IM0=YoungNuModuli<stress>(E0,nu0);
const auto IMi=YoungNuModuli<stress>(Ei,nui);
const auto A = computeSphereLocalisationTensor<stress>(IM0,IMi);
const auto A_axi = computeAxisymmetricalLocalisationTensor<stress>(IM0,IMi,n_a,e);
const auto A_ellipsoid = computeLocalisationTensor<stress>(IM0,IMi,n_a,a,n_b,b,c);
~~~~

Note that if the elasticity of the inclusion
is not isotropic, an anisotropic elasticity `C_i` can be provided, assuming that this elasticiy
is expressed in the same basis as the one defined by `n_a,n_b` (the local basis of the inclusion):

~~~~{.cpp}
const auto A_aniso = computeLocalisationTensor<stress>(IM0,C_i,n_a,a,n_b,b,c);
~~~~

In 2 dimensional framework, localisation tensors are computed as follows:

~~~~{.cpp}
const auto A_D = computeDiskPlaneStrainLocalisationTensor<stress>(IM0,C_i);
const auto A_C = computePlaneStrainLocalisationTensor<stress>(IM0,C_i,n_a,a,b);
~~~~


## Computation in anisotropic reference medium

The header `AnisotropicEshelbyTensor.hxx` introduces
the computation of the Eshelby tensors and Hill tensors
of general ellipsoids embedded in an anisotropic medium.

These tensors can be computed as follows:

~~~~{.cpp}
const auto P0 = computeAnisotropicHillTensor<stress>(C0,n_a,a,n_b,b,c);
const auto P0_2d = computePlaneStrainAnisotropicHillTensor<stress>(C0,n_a,a,b);

const auto S0 = computeAnisotropicEshelbyTensor<stress>(C0,n_a,a,n_b,b,c);
const auto S0_2d = computePlaneStrainAnisotropicEshelbyTensor<stress>(C0,n_a,a,b);
~~~~

The tensors are computed via an integration on a bi-dimensional domain.
The integration is iterative, and the user can provide the number of iterations
(basically, it corresponds to the number of subdivisions in each domain direction).
Hence, more iterations lead to a more accurate results, but longer to compute.
The default number of iterations is `12`, but is is recommended to increase it
for sharp ellipsoids:

~~~~{.cpp}
const std::size_t it = 10; 
const auto P0 = computeAnisotropicHillTensor<stress>(C0,n_a,a,n_b,b,c,10);
~~~~

The localisation tensors are introduced in the same header
`AnisotropicEshelbyTensor.hxx`. We can do as follows:

~~~~{.cpp}
const auto A = computeAnisotropicLocalisationTensor<stress>(C0_glob,Ci_loc,n_a,a,n_b,b,c);
const auto A_2d = computePlaneStrainAnisotropicLocalisationTensor<stress>(C0_glob,Ci_loc,n_a,a,b);
~~~~

The user must provide the elasticity of the inclusion as a `st2tost2` `Ci_loc`, and if it is
not isotropic, it must be provided in the local basis defined by `n_a,n_b`.

## Homogenization schemes for biphasic media

Different classical mean-field homogenization schemes are implemented
for biphasic media. These schemes are introduced by the header `LinearHomogenizationSchemes.hxx`.
They only deal with isotropic matrices and locally isotropic inclusions
(for anisotropic matrices or inclusions, see the section
"Homogenization of general microstructures").

The available schemes are:

 - Mori-Tanaka scheme
 - dilute scheme
 - Ponte Castaneda and Willis scheme

Each scheme is based on the average of the localisation tensor \(\tenseur A \)
defined above. This average is computed assuming different distributions
of ellipsoids. Hence different cases are considered:

 - spheres (no orientations)
 - oriented ellipsoids (two vectors \(\tenseur n_a,\tenseur n_b\) define the orientation)
 - uniform isotropic distribution of orientations (the ellipsoids have no preferential orientation)
 - transverse isotropic distribution of orientations (one axis \(\tenseur n_a\)
 of the ellipsoid is fixed, the others are uniformly distributed in the transverse plane)

Hence we can compute the homogenized stiffness returned
by the available schemes. For example, for the distribution of spheres:

~~~~{.cpp}
const auto IM0=YoungNuModuli<stress>(E0,nu0);
const auto IMi=YoungNuModuli<stress>(Ei,nui);
const auto C_DS = computeSphereDiluteScheme<stress>(IM0,f,IMi);
const auto C_MT = computeSphereMoriTanakaScheme<stress>(IM0,f,IMi);
~~~~

Here, `f` is the volume fraction, and the subscript `0` refers to the matrix, and
the subscript `i` refers to the inclusion.

For the oriented inclusions, we can do:

~~~~{.cpp}
const auto C_DS = computeOrientedDiluteScheme<stress>(IM0,f,IMi,n_a,a,n_b,b,c);
const auto C_MT = computeOrientedMoriTanakaScheme<stress>(IM0,f,IMi,n_a,a,n_b,b,c);
const auto C_PCW = computeOrientedPCWScheme<stress>(IM0,f,IMi,n_a,a,n_b,b,c,D);
~~~~

Note that `PCW` refers to the Ponte-Castaneda and Willis scheme.
For this scheme, a `Distribution` object must
be created by the user. It is defined by two vectors \(\tenseur n_a,\tenseur n_b\) and three lengths
\(a,b,c\) that define the ellipsoid which defines the distribution:

~~~~{.cpp}
Distribution<stress> D = {.n_a = n_a, .a = a, .n_b = n_b, .b = b, .c = c};
~~~~

For the isotropic distribution of ellipsoids, we can do:

~~~~{.cpp}
const auto C_DS = computeIsotropicDiluteScheme<stress>(IM0,f,IMi,a,b,c);
const auto C_MT = computeIsotropicMoriTanakaScheme<stress>(IM0,f,IMi,a,b,c);
const auto C_PCW = computeIsotropicPCWScheme<stress>(IM0,f,IMi,a,b,c,D);
~~~~

And finally, we can consider a transverse isotropic distribution
of inclusions:

~~~~{.cpp}
const auto C_DS = computeTransverseIsotropicDiluteScheme<stress>(IM0,f,IMi,n_a,a,b,c);
const auto C_MT = computeTransverseIsotropicMoriTanakaScheme<stress>(IM0,f,IMi,n_a,a,b,c);
const auto C_PCW = computeTransverseIsotropicPCWScheme<stress>(IM0,f,IMi,n_a,a,b,c,D);
~~~~
 
Because the functions are based on the average of the localisation tensor \(\tenseur A \)
associated with each distribution, a `Base` function is also defined for each scheme,
that only takes in argument the average of the localisation tensor `A_av`. We then
can compute a homogenized stiffness with a very general averaged localisator:

~~~~{.cpp}
const auto C_DS = computeDiluteScheme<stress>(E0,nu0,f,Ei,nui,A_av);
const auto C_MT = computeMoriTanakaScheme<stress>(E0,nu0,f,Ei,nui,A_av);
const auto C_PCW = computePCWScheme<stress>(E0,nu0,f,Ei,nui,A_av,D);
~~~~

A tutorial on the computation of homogenized schemes for biphasic particulate microstructures
is available [here](BiphasicLinearHomogenization.html).


## Homogenization bounds

Different bounds are implemented and are introduced by the header
`LinearHomogenizationBounds.hxx`.
The available bounds are:

 - Voigt bound
 - Reuss bound
 - Hashin-Shtrikman bounds
 
## Homogenization of general microstructures

A `ParticulateMicrostructure` object can be created for
homogenization of general matrix-inclusion microstructures.

### The `ParticulateMicrostructure class`

The `ParticulateMicrostructure class` is available in 3d an 2d
via 2 template parameters: `ParticulateMicrostructure<N,stress>`
with `N` the dimension. For the details, see the file 'MicrostructureDescription.hxx'
which introduces the `class`.

A ParticulateMicrostructure consists on a matrix, in which are embedded
several distributions of inclusions. The class has three (private) attributes:

 - `number_of_phases`
 - `matrixPhase`
 - `inclusionPhases`

The `matrixPhase` is of type `Phase`.
 
The `inclusionPhases` is a `std::vector` of pointers on
`InclusionDistribution` objects (which represent the distributions of inclusions).

#### The `Phase` class

The `Phase class` is very simple. It has two attributes:

 - `fraction` (`real` type)
 - `stiffness` (`st2tost2` type)

and a method `is_isotropic()` which returns true if the phase is isotropic.
A `Phase` can be initialized as:

~~~~{.cpp}
Phase<3u,stress> Ph1(C0);
Phase<3u,stress> Ph2(IM0);
~~~~

If `C0` is a `st2tost2`, hence `Ph1` is not isotropic, whereas if `IM0` is
an `IsotropicModuli`, hence `Ph2` is isotropic (and `Ph2.is_isotropic();`
will return `true`.

#### The `InclusionDistribution class`

The `InclusionDistribution class` is an abstract class which represents a distribution
of inclusions. It is a child of the `Phase class`.
There are 4 child `class` of the `InclusionDistribution class`:
 
 - `SphereDistribution` (distribution of spheres)
 - `IsotropicDistribution` (isotropic distribution of ellipsoids)
 - `TransverseDistribution` (transverse isotropic distribution of ellipsoids)
 - `OrientedDistribution` (aligned distribution of ellipsoids)
 
Each `class` has a unique attribute `inclusion` (which is of type `Inclusion`, see below),
and a method, `computeMeanLocalisator`, which computes the average of the localisation
tensor on the distribution. This function is mainly used in the computation of the homogenization
schemes (see below).

These `class` are currently available in 3d only.
 
#### The `Inclusion class`

The `Inclusion class` is characterized by
its unique attribute: `semiLengths`. It is a `std::array` of `N`
lengths, which are the semi-lengths of the ellipsoid/ellipse,
where `N` is the dimension considered (2 or 3).
Hence, `Inclusion` has two template parameters: `Inclusion<N,LengthType>`.
Some particular `Inclusion` objects are also defined:
 
 - `Ellipsoid` (child of `Inclusion` in 3d)
 - `Spheroid` (child of `Ellipsoid` with the last two semi-lengths identical)
 - `Sphere` (child of `Spheroid` with 3 semi-lengths equal to unity)

#### Methods of the `ParticulateMicrostructure class`

The `ParticulateMicrostructure` has the following methods (see 'MicrostructureDescription.hxx'
for details):

 - `addInclusionPhase`
 - `removeInclusionPhase`
 - `replaceMatrixPhase`
 - `get_number_of_phases`, `get_matrix_fraction`, `get_matrix_elasticity`, `is_isotropic_matrix`, `get_inclusionPhase`

 
### Homogenization schemes

The file `MicrostructureLinearHomogenization.ixx` introduces the `HomogenizationScheme class`
which has three attributes:
 
 - `homogenized_stiffness`
 - `effective_polarisation`
 - `mean_strain_localisation_tensors`
 
In the same file are introduced some functions which take a `ParticulateMicrostructure`
as an argument and returns a `HomogenizationScheme` object. Three schemes are available:

 - dilute scheme
 - Mori-Tanaka scheme
 - Self-Consistent scheme


<!-- Local IspellDict: english -->
