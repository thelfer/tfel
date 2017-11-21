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
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\sigmaH}{\sigma_{H}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}


The Hosford equivalent stress is defined by (see @hosford_generalized_1972):
\[
\sigmaeq^{H}=\sqrt[a]{\Frac{1}{2}\paren{\absvalue{\sigma_{1}-\sigma_{2}}^{a}+\absvalue{\sigma_{1}-\sigma_{3}}^{a}+\absvalue{\sigma_{2}-\sigma_{3}}^{a}}}
\]
where \(\sigma_{1}\), \(\sigma_{2}\) and \(\sigma_{3}\) are the eigenvalues of the
stress.

Therefore, when \(a\) goes to infinity, the Hosford stress reduces to
the Tresca stress. When \(n = 2\) the Hosford stress reduces to the
von Mises stress.

![Comparison of the Hosford stress \(a=100,a=8\) and the von Mises stress](img/HosfordStress.svg "Comparison of the Hosford stress \(a=100,a=8\) and the von Mises stress"){width=70%}

This paper is dedicated to the implementation a perfect plastic
behaviour based on the Hosford equivalent stress. The yield surface is
thus chosen as follows:
\[
f\paren{\tsigma}=\sigmaeq^{H}-\sigma_{Y}
\]
where the yield stress \(\sigma_{Y}\) is a constant material parameter.

# Implicit scheme

# Implementation

## Metadata

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

The usage of the `StandardElasticity` is declared as follows:

~~~~{.cpp}
@Brick StandardElasticity;
~~~~

## Numerical parameters

~~~~{.cpp}
@Epsilon 1.e-16;
@Theta 1;
~~~~

## State variables

~~~~{.cpp}
@StateVariable strain p;
p.setGlossaryName("EquivalentPlasticStrain");
~~~~

## Material constants

~~~~{.cpp}
@ElasticMaterialProperties {150e9,0.3};
@Parameter a    = 8;
@Parameter sigy = 150e6;
~~~~

## Local variable declaration

~~~~{.cpp}
@LocalVariable bool b;
~~~~

## Local variable initialization

~~~~{.cpp}
@InitializeLocalVariables{
  const stress seps = 1.e-10*young;
  const auto sigel = computeElasticPrediction();
  const auto seqel = computeHosfordStress(sigel,8,seps);
  b = seqel>sigy;
}
~~~~

## Implicit system

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

## Compilation of the behaviour

~~~~{.bash}
$ mfront --obuild --interface=abaqus HosfordPerfectPlasticity.mfront
Treating target : all
The following library has been built :
- libABAQUSBEHAVIOUR.so :  HOSFORDPERFECTPLASTICITY_AXIS HOSFORDPERFECTPLASTICITY_PSTRESS HOSFORDPERFECTPLASTICITY_PSTRAIN HOSFORDPERFECTPLASTICITY_3D
~~~~

## A simple `python` script to determine the yield stress in 

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
