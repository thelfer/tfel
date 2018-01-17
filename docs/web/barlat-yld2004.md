% Implementation of Barlat' Yld2004-18p behaviour
% Thomas Helfer, Dominique Deloison
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

This article discusses the implementation of the Yld2004-18p
behaviour, as decribed by Barlat et al. (See @barlat_linear_2005).

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
\Frac{\sigmaeq^{B}}{\sigma_{0}}-\left(1+\Frac{p}{p_{0}}\right)^{np}=0
\]
where:

- \(\sigmaeq^{B}\) is the Barlat equivalent stress defined hereafter.
- \(p\) is the equivalent plastic strain.
- \(\sigma_{0}\), \(p_{0}\) and \(np\) are material parameters.

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

## Elastic prediction

First, an elastic prediction of the stress \(\tsigma^{\mathrm{tr}}\)
is made (The following expression is not valid in plane stress
hypothesis, this is the reason why we will use the
`computeElasticPrediction` method defined by the `StandardElasticity`
brick, see below):

\[
\tsigma^{\mathrm{tr}}=\lambda\,\trace{\bts{\tepsilonel}+\theta\,\Delta\,\tepsilonto}\,\tenseur{I}+2\,\mu\,\paren{\bts{\tepsilonel}+\theta\,\Delta\,\tepsilonto}
\]

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
f_{p}=\Frac{\mts{\sigmaeq^{B}}}{\sigma0}-\left(1+\Frac{\mts{p}}{p_{0}}\right)^{np}=0
\]

The derivatives of this equation with respect to
\(\Delta\,\tepsilonel\) and \(\Delta\,p\) are given by:
\[
\left\{
\begin{aligned}
\deriv{f_{p}}{\Delta\,\tepsilonel} &= \Frac{2\,\mu\,\theta}{\sigma_{0}}\,\mts{\tenseur{n}^{B}}\\
\deriv{f_{p}}{\Delta\,p}           &= -\Frac{np\,\theta}{p0}\left(1+\Frac{\mts{p}}{p_{0}}\right)^{np-1}\\
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

## Numerical parameters

The following part of file give some default values for numerical
parameters used by the integration algorithm:

~~~~{.cpp}
@Epsilon 1.e-16;
@Theta   1;
~~~~

## Elastic properties and yield stress

The material properties are hard-coded. The
`@ElasticMaterialProperties` is used to declare the Young modulus and
the Poisson ratio.

~~~~{.cpp}
@ElasticMaterialProperties {150e9,0.3};
~~~~

This keyword automatically declares two parameters called
`YoungModulus` and `PoissonRatio`.

In the `Implicit` scheme, the lame coefficients are automatically
deduced from the Young modulus and the Poisson ratio. They are
accessible though the `lambda` and `mu` local variables which are
automatically defined.

The parameters associated with the plastic part of the behaviour are
defined as follows:

~~~~{.cpp}
@Parameter a    = 8;
// 646*0.025**0.227
@Parameter sig0 = 150e6;
sig0.setEntryName("HardeningReferenceStress");
@Parameter p0   = 1.e-4;
p0.setEntryName("HardeningReferenceStrain");
@Parameter np   = 2;
np.setEntryName("BarlatExponent");
@Parameter c1p[9]  = {-0.069888,0.079143,0.936408,
		      0.524741,1.00306,1.36318,
		      0.954322,1.06906,1.02377};
c1p.setEntryName("BarlatLinearTransformationCoefficients1");
@Parameter c2p[9]  = {0.981171,0.575316,0.476741,
		      1.14501,0.866827,-0.079294,
		      1.40462,1.1471,1.05166};
c2p.setEntryName("BarlatLinearTransformationCoefficients2");
~~~~

Here `a` stands for the Barlat exponent, `sig0`, `p0` and `np` are the
parameters describing the hardening of the material.

The parameters associated with the first linear transformation are
given in the following order (see [this page](tensors.html) for
details):

\[
\left(c^{1}_{xy},c^{1}_{yx},c^{1}_{xz},c^{1}_{zx},c^{1}_{yz},c^{1}_{zy},c^{1}_{xy},c^{1}_{xz},c^{1}_{yz}\right)
\]

> **Note** In his paper, Barlat and coworkers seems to use the
> following convention for storing symmetric tensors:
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
> by Barlat et al., one shall "swap" the appropriate coefficients.

## State variables

The elastic strain is automatically declared the `StandardElasticity`
brick. The associated variable is `eel`.

The following statement introduces the equivalent plastic strain named
`p`:

~~~~{.cpp}
@StateVariable strain p;
p.setGlossaryName("EquivalentPlasticStrain");
~~~~

## Local variables declaration

For the implementations, we will need three local variables:

- a boolean `b`. This boolean will be `true` if a plastic loading
  occurs.
- two fourth order tensors `l1` and `l2` that stands for the linear
  transformation of the stress tensors.

~~~~{.cpp}
@LocalVariable bool b;
@LocalVariable Stensor4 l1,l2;
~~~~

## Local variable initialization

The main goal of the local variable initialization is to test if the
elastic prediction of the stress lies inside the yield surface.

~~~~{.cpp}
@InitializeLocalVariables{
  // when using the `Plate` orthotropic axes convention, all the
  // modelling hypotheses uses the same convention, so we can just
  // use the `makeBarlatLinearTransformation` function with the
  // space dimension N as the template parameter
  l1 = makeBarlatLinearTransformation<N>(c1p[0],c1p[1],c1p[2],
					                     c1p[3],c1p[4],c1p[5],
					                     c1p[6],c1p[7],c1p[8]);
  l2 = makeBarlatLinearTransformation<N>(c2p[0],c2p[1],c2p[2],
	                                     c2p[3],c2p[4],c2p[5],
					                     c2p[6],c2p[7],c2p[8]);
  const stress seps = 1.e-10*young;
  const auto sigel = computeElasticPrediction();
  const auto seqel = computeBarlatStress(sigel,l1,l2,a,seps);
  b = seqel/sig0>pow(1+p/p0,np);
}
~~~~

The `computeElasticPrediction` method, provided by the
`StandardElasticity` brick, computes the elastic prediction of the
stress and takes into account the modelling hypothesis. This
prediction is thus valid under the plane stress hypothesis.

The `makeBarlatLinearTransformation` and the `computeBarlatStress`
functions are described in details [here](tensors.hml)

## Implicit system

The code describing the implicit system is rather short:

~~~~{.cpp}
@Integrator{
  const stress seps = 1.e-10*young;
  if(!b){
    // elastic loading, nothing to be done
    return true;
  }
  const auto p_ = p+theta*dp;
  if(p_<0){
    return false;
  }
  real seq;
  Stensor n;
  Stensor4 dn;
  const auto tmp = pow(1+p_/p0,np);
  std::tie(seq,n,dn) = computeBarlatStressSecondDerivative(sig,l1,l2,a,seps);
  feel        += dp*n;
  dfeel_ddeel += 2*mu*theta*dp*dn;
  dfeel_ddp    = n;
  fp           = seq/sig0-tmp;
  dfp_ddeel    = (2*mu*theta)/sig0*n;
  dfp_ddp      = (np*theta)/(p0+p_)*tmp;
}
~~~~

It shall be noted that, at the beginning of this code block:

- `feel` has been initialized to
  \(\Delta\,\tepsilonel-\Delta\,\tepsilonto\) by the
  `StandardElasticity` brick
- `fp` has been initialized to \(\Delta\,p\) following standard
  conventions defined in the the `Implicit` domain specific language.
- the jacobian has been set to identity, following standard
  conventions defined in the `Implicit` domain specific language.

Thus, all the variables have been set to describe a purely elastic
behaviour. Hence, nothing is to be done if the boolean variable `b` is
`false`. In this case, one just return `true`.

If a plastic loading has been predicted, one uses the
`computeBarlatStressSecondDerivative` function which returns:

- the Barlat stress `seq`
- the Barlat stress derivative `n` with respect to the stress
- the Barlat stress second derivative `dn` with respect to the stress

The implicit system is then readily written, using expressions given
in the previous paragraph.

In `C++17`, the previous code can be more simplier and smaller, using
*structured bindings*:

~~~~{.cpp}
@Integrator{
  const stress seps = 1.e-10*young;
  if(!b){
    // elastic loading, nothing to be done
    return true;
  }
  const auto p_ = p+theta*dp;
  if(p_<0){
    return false;
  }
  const auto tmp = pow(1+p_/p0,np);
  const auto [seq,n,dn] = computeBarlatStressSecondDerivative(sig,l1,l2,a,seps);
  feel        += dp*n;
  dfeel_ddeel += 2*mu*theta*dp*dn;
  dfeel_ddp    = n;
  fp           = seq/sig0-tmp;
  dfp_ddeel    = (2*mu*theta)/sig0*n;
  dfp_ddp      = (np*theta)/(p0+p_)*tmp;
}
~~~~

# References

<!-- Local IspellDict: english -->
