% Implementation of a perfect plastic behaviour using the Hosford equivalent stress
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
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}

The Hosford equivalent stress is defined by (see @hosford_generalized_1972):
\[
\sigmaeq^{H}=\sqrt[a]{\Frac{1}{2}\paren{\absvalue{\sigma_{1}-\sigma_{2}}^{a}+\absvalue{\sigma_{1}-\sigma_{3}}^{a}+\absvalue{\sigma_{2}-\sigma_{3}}^{a}}}
\]
where \(\sigma_{1}\), \(\sigma_{2}\) and \(\sigma_{3}\) are the eigenvalues of the
stress.

Therefore, when \(a\) goes to infinity, the Hosford stress reduces to
the Tresca stress. When \(n = 2\) the Hosford stress reduces to the
von Mises stress.

![Comparison of the Hosford stress \(a=100,a=8\) and the von Mises stress](img/HosfordStress.svg
 "Comparison of the Hosford stress \(a=100,a=8\) and the von Mises
 stress in plane stress"){width=70%}

The Hosford stress is an isotropic homogeneous function of
degree 1. The flow rule is assumed associated and the plastic strain
rate \(\tdepsilonp\) is given by:
\[
\tdepsilonp=\dot{p}\,\deriv{\sigmaeq^{H}}{\tsigma}=\dot{p}\,\tenseur{n}^{H}
\]
where \(\dot{p}\) is the rate of the equivalent plastic strain \(p\).

## Yield surface

This paper is dedicated to the implementation a perfect plastic
behaviour based on the Hosford equivalent stress. The yield surface is
thus chosen as follows:
\[
f\paren{\tsigma}=\sigmaeq^{H}-\sigma_{Y}
\]
where the yield stress \(\sigma_{Y}\) is a constant material parameter.

The whole implementation is available
[here](./gallery/plasticity/HosfordPerfectPlasticity.mfront).

# Implicit scheme

## Choice of the state variables

The behaviour is integrated by an implicit scheme. Two state variables
are introduced:

- the elastic strain \(\tepsilonel\).
- the equivalent plastic strain \(p\).

The elastic strain is automatically defined by the
`StandardElasticity` brick.

The latter could be considered as an integration variable, but, for
post-processings purpose, we choose to keep it as a state variable.

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
f_{\tepsilonel}=\Delta\,\tepsilonel-\Delta\,\tepsilonto+\Delta\,p\,\mts{\tenseur{n}^{H}}
\]

The derivatives of this equation with respect to
\(\Delta\,\tepsilonel\) and \(\Delta\,p\) are given by~:
\[
\left\{
\begin{aligned}
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel} &= \tenseurq{I}+2\,\mu\,\theta\,\Delta\,p\,\deriv{\mts{\tenseur{n}^{H}}}{\mts{\tsigma}}\\
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel} &= \mts{\tenseur{n}^{H}}\\
\end{aligned}
\right.
\]

To determine the equivalent plastic strain increment, the following
equation must be satisfied:
\[
f_{p}=\mts{\sigmaeq^{H}}-\sigma_{Y}=0
\]

The derivatives of this equation with respect to
\(\Delta\,\tepsilonel\) and \(\Delta\,p\) are given by:
\[
\left\{
\begin{aligned}
\deriv{f_{p}}{\Delta\,\tepsilonel} &= 2\,\mu\,\theta\,\mts{\tenseur{n}^{H}}\\
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
@Behaviour HosfordPerfectPlasticity;
@Author    Thomas Helfer;
@Description{
  A simple implementation of a perfect
  plasticity behaviour using the
  Hosford stress.
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

In the `Implicit` scheme, the lame coefficient are automatically
deduced from the Young modulus and the Poisson ratio.

The parameters associated with the plastic part of the behaviour are
defined as follows:

~~~~{.cpp}
@Parameter a    = 8;
@Parameter sigy = 150e6;
~~~~

Here `a` stands for the Hosford exponent and `sigy` is the yield
stress.

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
  const stress seps = 1.e-10*young;
  const auto sigel = computeElasticPrediction();
  const auto seqel = computeHosfordStress(sigel,a,seps);
  b = seqel>sigy;
}
~~~~

The `computeElasticPrediction` method, provided by the
`StandardElasticity` brick, computes the elastic prediction of the
stress and takes into account the modelling hypothesis. This
prediction is thus valid under the plane stress hypothesis.

The `computeHosfordStress` is then called. It takes three arguments:

- the stress,
- the Hosford exponent,
- a criterion used to check if the von Mises stress is non zero. The
  von Mises stress is used to normalize the eigenvalues of the stress
  and avoid numerical overflows (See @scherzinger_return_2017 for
  details).

At the last line, the boolean variable `b` is set to `true` if the
elastic prediction of the Hosford stress exceeds the yield stress.

## Implicit system

The code describing the implicit system is rather short:

~~~~{.cpp}
@Integrator{
  const stress seps = 1.e-10*young;
  if(!b){
    // elastic loading, nothing to be done
    return true;
  }
  real seq;
  Stensor n;
  Stensor4 dn;
  std::tie(seq,n,dn) = computeHosfordStressSecondDerivative(sig,a,seps);
  feel        += dp*n;
  dfeel_ddeel += 2*mu*theta*dp*dn;
  dfeel_ddp    = n;
  fp           = (seq-sigy)/young;
  dfp_ddeel    = 2*mu*theta*n/young;
  dfp_ddp      = 0;
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
`computeHosfordStressSecondDerivative` function which returns:

- the Hosford stress `seq`
- the Hosford stress derivative `n` with respect to the stress
- the Hosford stress second derivative `dn` with respect to the stress

The implicit system is then readily written, using expressions given
in the previous paragraph.

> **Note** With `C++-17` structured bindings enabled, the previous
> code is much more readable:
> 
> ~~~~{.cpp}
> @Integrator{
>   const stress seps = 1.e-10*young;
>   if(!b){
>     // elastic loading, nothing to be done
>     return true;
>   }
>   const auto [seq,n,dn] = computeHosfordStressSecondDerivative(sig,a,seps);
>   feel        += dp*n;
>   dfeel_ddeel += 2*mu*theta*dp*dn;
>   dfeel_ddp    = n;
>   fp           = (seq-sigy)/young;
>   dfp_ddeel    = 2*mu*theta*n/young;
>   dfp_ddp      = 0;
> }
> ~~~~

# Determining the yield surface in plane stress

In this section, we show how to use a simple `python` script to
determine the yield surface in plane stress.

## Compilation of the behaviour

~~~~{.bash}
$ mfront --obuild --interface=abaqus HosfordPerfectPlasticity.mfront
Treating target : all
The following library has been built :
- libABAQUSBEHAVIOUR.so :  HOSFORDPERFECTPLASTICITY_AXIS HOSFORDPERFECTPLASTICITY_PSTRESS HOSFORDPERFECTPLASTICITY_PSTRAIN HOSFORDPERFECTPLASTICITY_3D
~~~~

## A simple `python` script to determine the yield surface in plane stress

The following script determines the yield stress under the plane
stress assumption in the principal stress space by a brute force
approach. It imposes strain paths in varying directions and a
constraint the shear stress is null. The computations stops when the
plastic equivalent strain exceeds \(10^{-3}\). The current stress
state is then printed.

~~~~{.python}
from math import pi,cos,sin,sqrt
import mtest

for theta in [pi*(-1+2*float(i)/99) for i in range(0,100)]:
    em   = 2.e-2
    npas = 100
    tmax = 1
    c    = cos(theta)
    s    = sin(theta)
    m    = mtest.MTest()
    mtest.setVerboseMode(mtest.VerboseLevel.VERBOSE_QUIET)
    m.setModellingHypothesis('PlaneStress')
    m.setMaximumNumberOfSubSteps(1)
    m.setBehaviour('abaqus','src/libABAQUSBEHAVIOUR.so',
	               'HOSFORDPERFECTPLASTICITY_PSTRESS');
    m.setExternalStateVariable("Temperature",293.15)
    m.setImposedStrain('EXX',{0:0,tmax:em*c})
    m.setImposedStrain('EYY',{0:0,tmax:em*s})
    m.setNonLinearConstraint('SXY','Stress')
    s  = mtest.MTestCurrentState()
    wk = mtest.MTestWorkSpace()    
    m.completeInitialisation()    
    m.initializeCurrentState(s)
    m.initializeWorkSpace(wk)
    ltime=[(tmax/(npas-1))*i for i in range(npas)]
    for i in range(npas-1):
        m.execute(s,wk,ltime[i],ltime[i+1])
        p = s.getInternalStateVariableValue('EquivalentPlasticStrain');
        if(p>0.001):
            print(str(theta)+" "+str(s.s1[0])+" "+str(s.s1[1]))
            break
~~~~

# References

<!-- Local IspellDict: english -->
