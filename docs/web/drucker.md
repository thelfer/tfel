% Implementation of a perfect plastic behaviour using the Drucker yield criterion (@drucker_relation_1949) and its extension to orthotropic (@cazacu_generalization_2001)
% Thomas Helfer
% 21/11/2017

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
\newcommand{\sderiv}[2]{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial #2^{2}}}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}
\newcommand{\transpose}[1]{{#1^{\mathrm{T}}}}

<!-- This paper is dedicated to the implementation a perfect plastic -->
<!-- behaviour based on the Drucker yield criterion. -->

<!-- The two implementations are available -->
<!-- [here](./gallery/plasticity/DruckerPerfectPlasticity.mfront). -->

# Description

## Elasticity

The elasticity is assumed linear and isotropic, i.e. given by the
Hooke law:
\[
\tsigma=\lambda\,\trace{\tepsilonel}\,\tenseur{I}+2\,\mu\,\tepsilonel 
\]
where \(\lambda\) and \(\mu\) are the first and second Lamé parameters.

## Plasticity

The yield surface is chosen as follows:
\[
f\paren{\tsigma}=\sigmaeq^{D}-\sigma_{Y}
\]
where:

- \(\sigmaeq^{D}\) is the Drucker yield criterion defined hereafter.
- The yield stress \(\sigma_{Y}\) is a constant material parameter.

The Drucker yield criterion is defined by (@drucker_relation_1949):
\[
\sigmaeq^{D}=\sqrt{3}\,\paren{J_{2}^{3}-c\,J_{3}^2}^{\tfrac{1}{6}}
\]
where \(J_{2}\) and \(J_{3}\) are the invariants of \(\tenseur{s}\)
the deviatoric part of the stress tensor which are defined as follows:
\[
\left\{
\begin{aligned}
\tenseur{s} &= \tsigma-\Frac{1}{3}\,\trace{\tsigma}\,\tenseur{I}\\
J_2 &= \Frac{1}{2}\,\tenseur{s}\,\colon\,\tenseur{s} \\
J_3 &= \det\paren{\tenseur{s}} \\
\end{aligned}
\right.
\]

The Drucker stress is an isotropic homogeneous function of
degree 1. The flow rule is assumed associated and the plastic strain
rate \(\tdepsilonp\) is given by:
\[
\tdepsilonp=\dot{p}\,\deriv{\sigmaeq^{D}}{\tsigma}=\dot{p}\,\tenseur{n}^{D}
\]
where \(\dot{p}\) is the rate of the equivalent plastic strain \(p\).

## Derivatives of the equivalent stress

In the following, the first and second derivatives of the equivalent
stress with respect to the stress \(\tsigma\) will be required.

### First derivative of the equivalent stress

The first derivative of the equivalent stress \(\sigmaeq^{D}\) with
with respect to the stress \(\tsigma\) are readily obtained by chain
rule:
\[
\deriv{\sigmaeq^{D}}{\tsigma}=\deriv{\sigmaeq^{D}}{J_{2}}\,\deriv{J_{2}}{\tsigma}+\deriv{\sigmaeq^{D}}{J_{3}}\,\deriv{J_{3}}{\tsigma},
\]

The first derivatives of the Drucker equivalent stress
\(\sigmaeq^{D}\) with respect \(J_{2}\) and \(J_{3}\) are given by the
following formulae:

\[
\left\{
\begin{aligned}
\deriv{\sigmaeq^{D}}{J_{2}}&=\Frac{J_{2}^{2}}{2\,\paren{J_{2}^{3}-c\,J_{3}^{2}}}\,\sigmaeq^{D}\\
\deriv{\sigmaeq^{D}}{J_{3}}&=\Frac{-c\,J_{3}}{3\,\paren{J_{2}^{3}-c\,J_{3}^{2}}}\,\sigmaeq^{D}\\
\end{aligned}
\right.
\]

The derivative of \(J_{2}\) with respect to \(\tsigma\) is trivial:
\[
\deriv{J_{2}}{\sigma}=\tenseur{s}
\]

The derivative of \(J_{2}\) with respect to \(\tsigma\) can be
computed using the `computeJ3Derivative` function, which is available
since `TFEL` version `3.2`.

See [this page](tensors.html) for details.

### Second derivative of the equivalent stress

The second derivative of the equivalent stress can also be obtained by
chain rule:

\[
\begin{aligned}
\sderiv{\sigmaeq^{D}}{\tsigma}=&
\sderiv{\sigmaeq^{D}}{J_{2}}\,\deriv{J_{2}}{\tsigma}\,\otimes\,\deriv{J_{2}}{\tsigma}+\deriv{\sigmaeq^{D}}{J_{2}}\,\sderiv{J_{2}}{\tsigma}+\\
&\sderiv{\sigmaeq^{D}}{J_{3}}\,\deriv{J_{3}}{\tsigma}\,\otimes\,\deriv{J_{3}}{\tsigma}+\deriv{\sigmaeq^{D}}{J_{3}}\,\sderiv{J_{3}}{\tsigma}+\\
&\Frac{\partial^{2}\sigmaeq^{D}}{\partial\,J_{2}\,\partial\,J_{3}}\,\left[\deriv{J_{3}}{\tsigma}\,\otimes\,\deriv{J_{2}}{\tsigma}+\deriv{J_{2}}{\tsigma}\,\otimes\,\deriv{J_{3}}{\tsigma}\right]
\end{aligned}
\]

The second derivatives of the Drucker equivalent stress
\(\sigmaeq^{D}\) with respect \(J_{2}\) and \(J_{3}\) are given by the
following formulae:

\[
\left\{
\begin{aligned}
\sderiv{\sigmaeq^{D}}{J_{2}}&=
\Frac{-4\,c\,J_{2}\,J_{3}^{2}-J_{2}^{4}}{4\,c^{2}\,J_{3}^{4}-8\,c\,J_{2}^{3}\,J_{3}^{2}+4\,J_{2}^{6}}\,\sigmaeq^{D}\\
\sderiv{\sigmaeq^{D}}{J_{3}}&=
\Frac{-2\,c^{2}\,J_{3}^{2}-3\,\,c\,J_{2}^{3}}{9\,c^{2}\,J_{3}^{4}-18\,c\,J_{2}^{3}\,J_{3}^{2}+9\,J_{2}^{6}}\,\sigmaeq^{D}\\
\Frac{\partial^{2}\,\sigmaeq^{D}}{\partial\,J_{2}\,\partial\,J_{3}}&=
\Frac{5\,c\,J_{2}^{2}\,J_{3}}{6\,c^{2}\,J_{3}^{4}-12\,c\,J_{2}^{3}\,J_{3}^{2}+6\,J_{2}^{6}}\,\sigmaeq^{D}\\
\end{aligned}
\right.
\]

The second derivative of \(J_{2}\) is trivial:
\[
\sderiv{J_{2}}{\tsigma}=\tenseurq{I}-\Frac{1}{3}\,\tenseur{I}\,\otimes\,\tenseur{I}
\]

The second derivative of \(J_{3}\) can be computed using the
`computeJ3SecondDerivative` function.

> **Notes**
>
> Another approach is to express explicitly the equivalent stress and
> to compute the previous derivatives explicitly. This approach is
> more cumbersome and one have to take care of:
>
> - The conventions used by `TFEL` to represent symmetric tensors as
>   vectors (see [this page](tensors.html) for details).
> - Providing specializations for every supported space dimension
>   (\(1D\), \(2D\) or \(3D\)).
>
> This is the approach used most of the time by the `TFEL` developpers
> to provide optimised versions. One can refer to the implementation
> of the `computeJ3Derivative` and `computeJ3SecondDerivative` for
> concrete example. Those functions are declared in the
> `IsotropicPlasticity.hxx` header and implemented in the
> `IsotropicPlasticity.ixx` file. Those headers are explicitly
> included in the code generated by `MFront`.
> 
> Note that for the extension to orthotropy we will use various
> functions (`computeJ20`, `computeJ20Derivative`, etc.) that have
> also been implemented by this approach. Those functions are declared
> in the `OrthotropicPlasticity.hxx` header and implemented in the
> `OrthotropicPlasticity.ixx` file. Those headers are explicitly
> included in the code generated by `MFront` for orthotropic
> behaiours.
>
> We did not take this approach here for two reasons:
>
> - Most users will have to deal with the existing functions.
> - The extension to orthotropy will be straightforward.

# Implicit scheme

## Choice of the state variables

The behaviour is integrated by an implicit scheme. Two state variables
are introduced:

- the elastic strain \(\tepsilonel\).
- the equivalent plastic strain \(p\).

The elastic strain is automatically defined by the
`StandardElasticity` brick.

The latter could be considered as an integration variable, but, for
post-processing purposes, we choose to keep it as a state variable.

## Elastic prediction

First, an elastic prediction of the stress \(\tsigma^{\mathrm{tr}}\)
is made (The following expression is not valid in plane stress
hypothesis, see below):
\[
\tsigma^{\mathrm{tr}}=\lambda\,\trace{\bts{\tepsilonel}+\theta\,\Delta\,\tepsilonto}\,\tenseur{I}+2\,\mu\,\paren{\bts{\tepsilonel}+\theta\,\Delta\,\tepsilonto} \]

- If the predicted stress is inside the elastic domain, no plastic
  flow occurs.
- Otherwise, the material state at the end of the time step lies on
  the yield surface.

## Equations governing the material evolution under plastic loading

The equation associated with the evolution of the elastic strain is
given by the split of strain:
\[
f_{\tepsilonel}=\Delta\,\tepsilonel-\Delta\,\tepsilonto+\Delta\,p\,\mts{\tenseur{n}^{D}}
\]

The derivatives of this equation with respect to
\(\Delta\,\tepsilonel\) and \(\Delta\,p\) are given by:
\[
\left\{
\begin{aligned}
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel} &= \tenseurq{I}+2\,\mu\,\theta\,\Delta\,p\,\deriv{\mts{\tenseur{n}^{D}}}{\mts{\tsigma}}\\
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel} &= \mts{\tenseur{n}^{D}}\\
\end{aligned}
\right.
\]

To determine the equivalent plastic strain increment, the following
equation must be satisfied:
\[
f_{p}=\mts{\sigmaeq^{D}}-\sigma_{Y}=0
\]

In the implementation described below, this equation will be
normalised by the Young modulus to ensure that all equations have the
same magnitude.

The derivatives of this equation with respect to
\(\Delta\,\tepsilonel\) and \(\Delta\,p\) are given by:
\[
\left\{
\begin{aligned}
\deriv{f_{p}}{\Delta\,\tepsilonel} &= 2\,\mu\,\theta\,\mts{\tenseur{n}^{D}}\\
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

~~~~{.cpp}
@DSL       Implicit;
@Behaviour DruckerPerfectPlasticity;
@Author    Thomas Helfer;
@Description{
  A simple implementation of a perfect
  plasticity behaviour using the
  Drucker stress.
};
~~~~

## Supported modelling hypothesis

Thanks to the `StandardElasticity` brick, all the modelling hypotheses
can be supported. The following statement, starting with the
`@ModellingHypotheses`, enables all the modelling hypotheses:

~~~~{.cpp}
@ModellingHypotheses {".+"};
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
@Theta 1;
~~~~

## State variables

The elastic strain is automatically declared the `StandardElasticity`
brick. The associated variable is `eel`.

The following statement introduces the equivalent plastic strain named
`p`:

~~~~{.cpp}
@StateVariable strain p;
p.setGlossaryName("EquivalentPlasticStrain");
~~~~

## Material constants

The material properties are hard-coded. The
`@ElasticMaterialProperties` is used to declare the Young modulus and
the Poisson ratio.

~~~~{.cpp}
@ElasticMaterialProperties {150e9,0.3};
~~~~

In the `Implicit` scheme, the lame coefficients are automatically
deduced from the Young modulus and the Poisson ratio. They are
accessible though the `lambda` and `mu` local variables which are
automatically defined.

The parameters associated with the plastic part of the behaviour are
defined as follows:

~~~~{.cpp}
@Parameter c    = 2;
c.setEntryName("DruckerCoefficient");
@Parameter sigy = 150e6;
c.setEntryName("YieldStrength");
~~~~

## Local variable declaration

A boolean `b` is declared as a local variable.

~~~~{.cpp}
@LocalVariable bool b;
~~~~

This boolean will be `true` if plastic loading occurs.

## Local variable initialization

The main goal of the local variable initialization is to test if the
elastic prediction of the stress lies inside the yield surface.

~~~~{.cpp}
@InitializeLocalVariables{
  constexpr const auto e = real(1)/6;
  constexpr const auto sqrt3 = Cste<real>::sqrt3;
  const auto sigel = computeElasticPrediction();
  const auto sel   = deviator(sigel);
  const auto J2    = (sel|sel)/2;
  const auto J3    = det(sel);
  const auto seqel = sqrt3*pow(J2*J2*J2-c*J3*J3,e);
  b = seqel>sigy;
}
~~~~

The `computeElasticPrediction` method, provided by the
`StandardElasticity` brick, computes the elastic prediction of the
stress and takes into account the modelling hypothesis. This
prediction is thus valid under the plane stress hypothesis.

At the last line, the boolean variable `b` is set to `true` if the
elastic prediction of the Drucker stress exceeds the yield stress.

## Implicit system

The code describing the implicit system is rather short:

~~~~{.cpp}
@Integrator{
  constexpr const auto e = real(1)/6;
  constexpr const auto sqrt3 = Cste<real>::sqrt3;
  const stress seps = 1.e-10*young;
  if(!b){
    // elastic loading, nothing to be done
    return true;
  }
  constexpr const auto id  = Stensor::Id();
  constexpr const auto id4 = Stensor4::Id();
  const auto  s    = deviator(sig);
  const auto  J2   = (s|s)/2;
  if(J2>seps*seps){
    const auto  J2_2 = J2*J2;
    const auto  J2_3 = J2_2*J2;
    const auto  J2_4 = J2_2*J2_2;
    const auto  J2_6 = J2_4*J2_2;
    const auto  J3   = det(s);
    const auto  J3_2 = J3*J3;
    const auto  J3_4 = J3_2*J3_2;
    const auto& dJ2  = s;
    const auto  d2J2 = eval(id4-(id^id)/3);
    const auto  dJ3  = computeJ3Derivative(sig);
    const auto  d2J3 = computeJ3SecondDerivative(sig);
    const auto  s6   = J2_3-c*J3_2;
    const auto  seq  = sqrt3*pow(s6,e);
    const auto  dseq_dJ2 = J2_2/(2*s6)*seq;
    const auto  dseq_dJ3 = -c*J3/(3*s6)*seq;
    const auto  n  = dseq_dJ2*dJ2+dseq_dJ3*dJ3;
    const auto  d2seq_dJ2dJ2 =
      seq*(-4*J2*J3_2*c-J2_4)/(4*J3_4*c*c-8*J2_3*J3_2+4*J2_6);
    const auto  d2seq_dJ3dJ3 =
      seq*(-2*J3_2*c*c-3*J2_3*c)/(9*J3_4*c*c-18*J2_3*J3_2+9*J2_6);
    const auto  d2seq_dJ2dJ3 =
      seq*(5*J2_2*J3*c)/(6*J3_4*c*c-12*J2_3*J3_2+6*J2_6);
    // d2f=f*d(df/f)+df*df/f
    const auto  dn = eval(d2seq_dJ2dJ2*(dJ2^dJ2)+
			  dseq_dJ2*d2J2+
			  d2seq_dJ2dJ3*((dJ2^dJ3)+(dJ3^dJ2))+
			  d2seq_dJ3dJ3*(dJ3^dJ3)+
			  dseq_dJ3*d2J3);
    feel        += dp*n;
    dfeel_ddeel += 2*mu*theta*dp*dn;
    dfeel_ddp    = n;
    fp           = (seq-s0)/young;
    dfp_ddeel    = 2*mu*theta*n/young;
    dfp_ddp      = 0;
  } else {
    // This case can happen for example at the first time step if the
    // elastic prediction directly leads to a plastic loading: at the
    // first iteration the stress is still zero.
    fp           = -s0/young;
    // this is arbitrary, but this avoids
    // a singularity in the  jacobian
    dfp_ddp      = 1;
  }
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

# A first Extension to orthotropy (@cazacu_generalization_2001)

Within the framework of the theory of representation, generalizations
to orthotropic conditions of the invariants of the deviatoric stress
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

\(J_{2}^{0}\), \(J_{3}^{0}\) and their first and second derivatives
with respect to the stress tensor \(\tsigma\) can be computed
by the following functions:

- `computesJ2O`, `computesJ2ODerivative` and
  `computesJ2OSecondDerivative`.
- `computesJ3O`, `computesJ3ODerivative` and
  `computesJ3OSecondDerivative`.

Those functions take the stress tensor as first argument and each
orthotropic coefficients. Each of those functions has an overload
taking the stress tensor as its firs arguments and a tiny vector
(`tfel::math::tvector`) containing the orthotropic coefficients.

Using those invariants, building a new yield criterion is quite
straight-forward:

\[
\sigmaeq^{D,O}=\sqrt{3}\,\paren{\paren{J_{2}^{O}}^{3}-c\,\paren{J_{3}^{O}}^2}^{\tfrac{1}{6}}
\]

# A second extension to orthotropy (@barlat_linear_2007)

Another approach is to introduce a linear transformation of the
stress, as follows:
\[
\tenseur{s'}=\tenseurq{L}\,\colon\,\tsigma
\]

The linear transformation \(\tenseurq{L}\) are defined by \(9\)
coefficients (each) which describe the material orthotropy. There are
defined through auxiliary linear transformation \(\tenseurq{C}\) as
follows:
\[
\tenseurq{L} =\tenseurq{C}\,\colon\,\tenseurq{M}
\]
where \(\tenseurq{M}\) is the transformation of the stress to its deviator:
\[
\tenseurq{M}=\tenseurq{I}-\Frac{1}{3}\tenseur{I}\,\otimes\,\tenseur{I}
\]

The linear transformation \(\tenseurq{C}\) of the deviator stress 
is defined as follows:
\[
\tenseurq{C}=
\begin{pmatrix}
0 & -c_{12} & -c_{13} & 0 & 0 & 0 \\
-c_{21} & 0 & -c_{23} & 0 & 0 & 0 \\
-c_{31} & -c_{32} & 0 & 0 & 0 & 0 \\
0 & 0 & 0 & c_{44} & 0 & 0 \\
0 & 0 & 0 & 0 & c_{55} & 0 \\
0 & 0 & 0 & 0 & 0 & c_{66} \\
\end{pmatrix}
\]

Another equivalent stress can then be defined, as follows:

\[
\sigmaeq^{D,OL}=\sqrt{3}\,\paren{\paren{J_{2}'}^{3}-c\,\paren{J_{3}'}^2}^{\tfrac{1}{6}}
\]

where \(J_{2}'\) and \(J_{3}'\) are the invariants associated with
\(\tenseur{s'}\)

The implementation is here made straight-forward because of the
following relations:
\[
\left\{
\begin{aligned}
\deriv{\sigmaeq^{D,OL}}{\tsigma}&=\deriv{\sigmaeq^{D,OL}}{\tenseur{s}'}\,\colon\,\tenseurq{L} \\
\sderiv{\sigmaeq^{D,OL}}{\tsigma}&=\transpose{\tenseurq{L}}\,\cdot\,\sderiv{\sigmaeq^{D,OL}}{\tenseur{s}'}\,\cdot\,\tenseurq{L} \\
\end{aligned}
\right.
\]

The expressions of \(\deriv{\sigmaeq^{D,OL}}{\tenseur{s}'}\) and
\(\sderiv{\sigmaeq^{D,OL}}{\tenseur{s}'}\) are the obtained from the
isotropic case by replacing \(\tsigma\) with \(\tenseur{s}'\), which
is trivial.

# References

<!-- Local IspellDict: english -->
