% Implementation of Barlat' Yld2004 behaviour
% Thomas Helfer
% 7/01/2018

\newcommand{\absvalue}[1]{{\left|#1\right|}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\tenseur{\varepsilon}^{\mathrm{el}}}
\newcommand{\tepsilonvis}{\tenseur{\varepsilon}^{\mathrm{vis}}}
\newcommand{\tdepsilonvis}{\tenseur{\dot{\varepsilon}}^{\mathrm{vis}}}
\newcommand{\tepsilonp}{\tenseur{\varepsilon}^{\mathrm{p}}}
\newcommand{\tdepsilonp}{\tenseur{\dot{\varepsilon}}^{\mathrm{p}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\sigmaH}{\sigma_{H}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}

This article discusses the implementation of the Yld2004 behaviour, as
decribed by Barlat et al. (See @barlat_linear_2005).

# Description of the behaviour

## Elasticity

The elasticity is assumed linear and isotropic, i.e. given by the
Hooke law:
\[
\tsigma=\lambda\,\trace{\tepsilonel}\,\tenseur{I}+2\,\mu\,\tepsilonel 
\]
where \(\lambda\) and \(\mu\) are the first and second Lamé parameters.

## Plasticity

The yield surface is given by:
\[
\sigmaeq^{B}-\sigma_{0}\left(1+\Frac{p}{p_{0}}\right)^{n}=0
\]
where:

- \(\sigmaeq^{B}\) is the Barlat equivalent stress defined hereafter.
- \(p\) is the equivalent plastic strain.
- \(\sigma_{0}\), \(p_{0}\) and \(n\) are material parameters.

The Barlat equivalent stress is defined as follows (See @barlat_linear_2005):
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

The linear transformations of the deaviator stress \(\tenseurq{C}'\)
and \(\tenseurq{C}''\), are defined as follows:
\[
\tenseurq{C}'=
\Frac{1}{3}\,
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

When all the coefficients \(c'_{ji}\) and \(c''_{ji}\) are equal to
\(1\), the Barlat equivalent stress reduces to the Hosford equivalent
stress.

# Implicit scheme

## Choice of the state variables

The behaviour is integrated by an implicit scheme. Two state variables
are introduced:

- the elastic strain \(\tepsilonel\).
- the equivalent plastic strain \(p\).

The elastic strain is automatically defined by the
`StandardElasticity` brick.

The latter could be considered as an integration variable, which means
that its value would not be saved from one time step to
antoher. However, for post-processing purposes, we choose to keep it
as a state variable.

## Elastic prediction

First, an elastic prediction of the stress \(\tsigma^{\mathrm{tr}}\)
is made (The following expression is not valid in plane stress
hypothesis, see below):
\[ \tsigma^{\mathrm{tr}}=\lambda\,\trace{\bts{\tepsilonel}+\theta\,\Delta\,\tepsilonto}\,\tenseur{I}+2\,\mu\,\paren{\bts{\tepsilonel}+\theta\,\Delta\,\tepsilonto} \]

- If the predicted stress is inside the elastic domain, no plastic
  flow occurs.
- Otherwise, the material state at the end of the time step lies on
  the yield surface.

## Equations governing the material evolution under plastic loading

The equation associated with the evolution of the elastic strain is
given by the split of strain:
\[
f_{\tepsilonel}=\Delta\,\tepsilonel-\Delta\,\tepsilonto+\Delta\,p\,\mts{\tenseur{n}^{B}}
\]

The derivatives of this equation with respect to
\(\Delta\,\tepsilonel\) and \(\Delta\,p\) are given by:
\[
\left\{
\begin{aligned}
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel} &= \tenseurq{I}+2\,\mu\,\theta\,\Delta\,p\,\deriv{\mts{\tenseur{n}^{B}}}{\mts{\tsigma}}\\
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel} &= \mts{\tenseur{n}^{B}}\\
\end{aligned}
\right.
\]

To determine the equivalent plastic strain increment, the following
equation must be satisfied:
\[
f_{p}=\mts{\sigmaeq^{B}}-\sigma_{Y}=0
\]

The derivatives of this equation with respect to
\(\Delta\,\tepsilonel\) and \(\Delta\,p\) are given by:
\[
\left\{
\begin{aligned}
\deriv{f_{p}}{\Delta\,\tepsilonel} &= 2\,\mu\,\theta\,\mts{\tenseur{n}^{B}}\\
\deriv{f_{p}}{\Delta\,p}           &= 0\\
\end{aligned}
\right.
\]

# Implementation

## Metadata

The beginning of the file gives some information about the behaviour:

- the integration scheme used, selected by the `@DSL` keyword.
- the name of the behaviour, introduced by the `@Behaviour` keyword.
- the author of the implementation (`@Author`).
- a small description of the behaviour (`@Description`).

## Orthotropic axes convention

An orthotropic axes convention has to be defined to have
consistent definition of the linear transformations \(l_{1}\) and
\(l_{2}\). The behaviour has been identified for plates, so the
natural choice is the `Plate` orthotropic convention:

~~~~{.cxx}
@OrthotropicBehaviour<Plate>;
~~~~

This choice limits the number of possible modelling hypotheses that
are available to the following list: `Tridimensional`, `PlaneStrain`,
`GeneralisedPlaneStrain`, `PlaneStress`.

Thanks to the `StandardElasticity` brick defined below, all those
modelling hypotheses can be supported, but the support for the plane
stress hypothesis as to be explicitly requested. The following
statement, starting with the `@ModellingHypotheses`, enables all the
previously listed modelling hypotheses:

~~~~{.cpp}
@ModellingHypotheses {Tridimensional,PlaneStrain,
                      GeneralisedPlaneStrain,PlaneStress};
~~~~

## The standard elasticity brick

To implement this behaviour, we will use the `StandardElasticity`
brick which provides:

- Automatic computation of the stress tensor at various stages of the
  behaviour integration.
- Automatic computation of the consistent tangent operator.
- Automatic support for plane stress and generalized plane stress
  modelling hypotheses (The axial strain is defined as an additional
  state variable and the associated equation in the implicit system is
  added to enforce the plane stess condition).
- Automatic addition of the standard terms associated with the elastic
  strain state variable.

This behaviour brick is fully described [here](BehaviourBricks.html).

The usage of the `StandardElasticity` is introduced as follows:

~~~~{.cpp}
@Brick StandardElasticity;
~~~~

## Implicit system

The following example computes the Barlat equivalent stress, its
normal and second derivative:

~~~~{.cpp}
const auto l1 = makeBarlatLinearTransformation<N,double>(-0.069888,0.936408,
                                                         0.079143,1.003060,
                                                         0.524741,1.363180,
                                                         1.023770,1.069060,
                                                         0.954322);
const auto l2 = makeBarlatLinearTransformation<N,double>(-0.981171,0.476741,
    							                         0.575316,0.866827,
    							                         1.145010,-0.079294,
    							                         1.051660,1.147100,
    							                         1.404620);
stress seq;
Stensor  n;
Stensor4 dn;
std::tie(seq,n,dn) = computeBarlatStressSecondDerivative(s,l1,l2,a,seps);
~~~~

In this example, `s` is the stress tensor, `a` is the Hosford
exponent, `seps` is a numerical parameter used to detect when two
eigenvalues are equal.

If `C++-17` is available, the previous code can be made much more readable:

~~~~{.cpp}
const auto l1 = makeBarlatLinearTransformation<N,double>(-0.069888,0.936408,
                                                         0.079143,1.003060,
                                                         0.524741,1.363180,
                                                         1.023770,1.069060,
                                                         0.954322);
const auto l2 = makeBarlatLinearTransformation<N,double>(-0.981171,0.476741,
    							                         0.575316,0.866827,
    							                         1.145010,-0.079294,
    							                         1.051660,1.147100,
    							                         1.404620);
const auto [seq,n,dn] = computeBarlatStressSecondDerivative(s,l1,l2,a,seps);
~~~~

# References

<!-- Local IspellDict: english -->
