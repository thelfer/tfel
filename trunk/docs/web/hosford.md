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

This paper is dedicated to the implementation a perfect plastic
behaviour based on the Hosford equivalent stress.

The whole implementation is available
[here](./gallery/plasticity/HosfordPerfectPlasticity.mfront).

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
f\paren{\tsigma}=\sigmaeq^{H}-\sigma_{Y}
\]
where:

- \(\sigmaeq^{H}\) is the Hosford equivalent stress defined hereafter.
- The yield stress \(\sigma_{Y}\) is a constant material parameter.

The Hosford equivalent stress is defined by (see @hosford_generalized_1972):
\[
\sigmaeq^{H}=\sqrt[a]{\Frac{1}{2}\paren{\absvalue{\sigma_{1}-\sigma_{2}}^{a}+\absvalue{\sigma_{1}-\sigma_{3}}^{a}+\absvalue{\sigma_{2}-\sigma_{3}}^{a}}}
\]
where \(\sigma_{1}\), \(\sigma_{2}\) and \(\sigma_{3}\) are the eigenvalues of the
stress.

Therefore, when \(a\) goes to infinity, the Hosford stress reduces to
the Tresca stress. When \(n = 2\) the Hosford stress reduces to the
von Mises stress.

![Comparison of the Hosford stress \(a=100,a=8\) and the von Mises stress in plane stress](img/HosfordStress.svg
 "Comparison of the Hosford stress \(a=100,a=8\) and the von Mises
 stress in plane stress"){width=70%}

The Hosford stress is an isotropic homogeneous function of
degree 1. The flow rule is assumed associated and the plastic strain
rate \(\tdepsilonp\) is given by:
\[
\tdepsilonp=\dot{p}\,\deriv{\sigmaeq^{H}}{\tsigma}=\dot{p}\,\tenseur{n}^{H}
\]
where \(\dot{p}\) is the rate of the equivalent plastic strain \(p\).

# Implicit scheme

## Choice of the state variables

The behaviour is integrated by an implicit scheme. Two state variables
are introduced:

- the elastic strain \(\tepsilonel\).
- the equivalent plastic strain \(p\).

The elastic strain is automatically defined by the
`Implicit` domain specific language.

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
f_{\tepsilonel}=\Delta\,\tepsilonel-\Delta\,\tepsilonto+\Delta\,p\,\mts{\tenseur{n}^{H}}
\]

The derivatives of this equation with respect to
\(\Delta\,\tepsilonel\) and \(\Delta\,p\) are given by:
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

In the implementation described below, this equation will be
normalised by the Young modulus to ensure that all equations have the
same magnitude.

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

In the `Implicit` scheme, the lame coefficients are automatically
deduced from the Young modulus and the Poisson ratio. They are
accessible though the `lambda` and `mu` local variables which are
automatically defined.

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
  // this is a small trick that is necessary if the first time
  // step leads to a plastic loading:
  // on this case, the stress are zero on the first iteration and
  // this results in a null line in the jacobian matrix because
  // the normal is then undefined.
  // To avoid the issue, we arbitrarily set dfp_ddp to 1 in that case. 
  dfp_ddp      = (seq<seps) ? 1:  0;
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

# Comparing the robustness of various algorithms

This section compares the robustness of various algorithms used to
solve the implicit system used to integrate the Hosford behaviour.

## Description of the test

The test presented is based on the paper of Scherzinger (see
@scherzinger_return_2017). The test is based on the following steps:

- A direction in the \(\pi\)-plane, characterized by an angle
  \(\alpha\), is choosen. \(\alpha\) varies from \(-\pi\) to \(\pi\):
  this range is discretized \(1000\) times.
- A stress state \(\tsigma_{\alpha}\) which lies on the yield surface
  \(\sigmaeq^{H}=\sigma_{Y}\) is determined. Such a stress state is
  characterized by its unique coordinates
  \(\tilde{\pi}\paren{\tsigma_{\alpha}}\) in the \(\pi\)-plane and
  this projection determines its three
  eigenvalues. \(\tsigma_{\alpha}\) is choosen diagonal.
- The elastic strain \(\tepsilonel_{\alpha}\) corresponding to
  \(\tsigma_{\alpha}\) is determined.
- Starting from a virgin state (all state variables are set to zero),
  the behaviour is integrated multiple times by imposing a strain
  increment \(\Delta\,\tepsilonto\) equal to
  \(x\,\tepsilonel_{\alpha}\) where \(x\) is increased from \(1\) to a
  maximal value of \(30\). If the behaviour integration succeeds,
  \(x\) is increased by an increment egal to \(30/1000\). This step is
  stopped if the behaviour integration fails or if \(x\) reaches
  \(30\).

For each values of \(\alpha\) and \(x\), the number of iterations
needed to solve the implicit system is recorded.

The implementation presented so far is modified to declare an
additional state variable using to save the number of iterations
required to reach the convergence.

The `python` script which implements the test is the following, in the
case of a Hosford exponent egal to \(100\):

~~~~{.python}
from math import pi,cos,sin
import tfel.math     as tmath
import tfel.material as tmaterial
import mtest

nmax  = 1000
nmax2 = 1000
E  = 150e9
nu = 0.3
s0 = 150e6

for a in [pi*(-1.+(2.*i)/(nmax-1)) for i in range(0,nmax)]:
    for x in [1+(29.*i)/(nmax2-1)  for i in range(0,nmax2)]:
        nbiter=0
        s     = tmath.makeStensor3D(tmaterial.buildFromPiPlane(cos(a),sin(a)))
        seq   = tmaterial.computeHosfordStress(s,100,1.e-12);
        s    *= x*(s0/seq)
        e0    = (s[0]-nu*(s[1]+s[2]))/E
        e1    = (s[1]-nu*(s[0]+s[2]))/E
        e2    = (s[2]-nu*(s[0]+s[1]))/E
        m    = mtest.MTest()
        mtest.setVerboseMode(mtest.VerboseLevel.VERBOSE_QUIET)
        m.setModellingHypothesis('Tridimensional')
        m.setMaximumNumberOfSubSteps(1)
        m.setBehaviour('abaqus','src/libABAQUSBEHAVIOUR.so',
                       'HOSFORDPERFECTPLASTICITY100_3D');
        m.setExternalStateVariable("Temperature",293.15)
        m.setImposedStrain('EXX',{0:0,1:e0})
        m.setImposedStrain('EYY',{0:0,1:e1})
        m.setImposedStrain('EZZ',{0:0,1:e2})
        m.setImposedStrain('EXY',0)
        m.setImposedStrain('EXZ',0)
        m.setImposedStrain('EYZ',0)
        s  = mtest.MTestCurrentState()
        wk = mtest.MTestWorkSpace()
        m.completeInitialisation()
        m.initializeCurrentState(s)
        m.initializeWorkSpace(wk)
        try:
            m.execute(s,wk,0,1)
            nbiter=s.getInternalStateVariableValue("NumberOfIterations")
        except:
            nbiter=100
            break
        print(str((x/seq)*cos(a))+" "+str((x/seq)*sin(a))+" "+str(nbiter))
    print()
~~~~

This script outputs, for each couple \(\paren{\alpha,x}\) for which
the behaviour integration succeeds, the coordinates
\(\tilde{\pi}\paren{\tsigma_{\alpha}}\) scaled by \(\sigma_{Y}\) and
the number of iterations required to reach the convergence.

## Algorithms tested

The following algorithms are considered:

- **Algorithm 1**: The standard Newton-Raphson algorithm.
- **Algorithm 2**: The Newton-Raphson algorithm with a test on the
  value of the equivalent Hosford stress \(\mts{\sigmaeq^{H}}\) for
  the current estimation of the elastic strain increment. If this
  \(\mts{\sigmaeq^{H}}\) is greater than \(\beta\,\sigma_{Y}\), the
  Newton step is rejected: the direction is kept unchanged but the
  amplitude of the correction to the internal state variables
  increment is divided by two. In the following, \(\beta\) is choosen
  equal to \(\frac{3}{2}\). The test has to be made in the
  `@Integrator` code block: if the test fails, one just have to return
  the `false` value. This is obtained by adding the following line
  after the computation of the Hosford equivalent stress:
  
~~~~{.cpp}
if(seq>3*(sigy/2)){
  return false;
}
~~~~
  
- **Algorithm 3**: The Newton-Raphson algorithm with a limitation of
  the increment of the elastic strain increment at each
  iteration. This can be activated by using the
  `setMaximumIncrementValuePerIteration` method on the `eel` variable.
- **Algorithm 4**: The standard Levenberg-Marquardt algorithm. This
  algorithm is selected using the `@Algorithm` keyword.

## Hosford \(a=6\)

The Hosford yield surface for an exponent egal to \(a=6\) is smooth.

![Robustness of Algorithm 1 for \(a=6\)](img/Scherzinger/Hosford6/Newton.png
 "Robustness of Algorithm 1 for \(a=6\)"){#fig:hosford6:alg1 width=70%}

Figure @fig:hosford6:alg1 displays the output of the previous script
in the \(\pi\)-plane (each coordinate being scaled by
\(\sigma_{Y}\)). The maximum number of iteration allowed to reach the
convergence is \(100\) by default. However, as stated by Scherzinger,
if the convergence is not reached for \(30\) iterations, most of the
time the convergence will not be reached.

The results depicted on Figure @fig:hosford6:alg1 confirms the ones of
Scherzinger: the standard Newton algorithm conditionally converges.
Indeed, it can be seen that the standard Newton algorithm converges
for all values of \(\alpha\) if \(x\) is low enough. For large values
of \(x\), the convergence clearly depends on \(\alpha\).

![Robustness of Algorithm 2 for \(a=6\)](img/Scherzinger/Hosford6/TestOnYieldStress.png
 "Robustness of Algorithm 2 for \(a=6\)"){#fig:hosford6:alg2 width=70%}

Figure @fig:hosford6:alg2 displays the output of the previous script
for the second algorithm. This figure shows that this slight
modification of the standard Newton algorithm considerably increases
the robustness: the convergence is reached for all tested values of
\(\alpha\) and \(x\).

Results for the third and fourth algorithms are not shown but are very
similar to the one obtained with the second algorithm. Indeed, in
terms of robustness, differences between those three algorithms only
arises for high values of the Hosford exponent \(a\).

## Hosford \(a=8\)

For an exponent equal to \(a=8\), the curvature of the yield surface
varies more rapidly.

![Robustness of Algorithm 1 for \(a=8\)](img/Scherzinger/Hosford8/Newton.png
 "Robustness of Algorithm 1 for \(a=8\)"){#fig:hosford8:alg1 width=70%}

As already stated by Scherzinger, Figure @fig:hosford8:alg1 shows that
the performance in terms of robustness of the standard Newton
algorithm is getting worse as the Hosford exponent increases.

![Robustness of Algorithm 2 for \(a=8\)](img/Scherzinger/Hosford8/TestOnYieldStress.png
 "Robustness of Algorithm 2 for \(a=8\)"){#fig:hosford8:alg2 width=70%}

Again, Algorithm 2 shows a remarkable benefit in terms of robustness.

## Hosford \(a=100\)

For a very high exponent \(a=100\), the yield surface is very close to
the Tresca yield surface and shows very sharp edges.

![Robustness of Algorithm 1 for \(a=100\)](img/Scherzinger/Hosford100/Newton.png
 "Robustness of Algorithm 1 for \(a=100\)"){#fig:hosford100:alg1 width=70%}

Figure @fig:hosford100:alg1 shows that the standard Newton algorithm
performs very poorly.

![Robustness of Algorithm 2 for \(a=100\)](img/Scherzinger/Hosford100/TestOnYieldStress.png
 "Robustness of Algorithm 2 for \(a=100\)"){#fig:hosford100:alg2 width=70%}

In this case, the second algorithm does not improve the robustness as
shown on Figure @fig:hosford100:alg2. This algorithm performs almost
as poorly than the standard Newton algorithm.

![Robustness of Algorithm 3 for \(a=100\)](img/Scherzinger/Hosford100/MaximumIncrementValuePerIteration.png
 "Robustness of Algorithm 3 for \(a=100\)"){#fig:hosford100:alg3 width=70%}

Figure @fig:hosford100:alg3 shows that the third algorithm clearly
increases the robustness in some directions but the robustness remains
low for angles close the edges of the yield surfaces.

![Robustness of Algorithm 4 for \(a=100\)](img/Scherzinger/Hosford100/LevenbergMarquardt.png
 "Robustness of Algorithm 4 for \(a=100\)"){#fig:hosford100:alg4 width=70%}

Figure @fig:hosford100:alg3 shows that the Levenberg-Marquardt
algorithm outperforms Algorithm 3. However, the robustness remains low
for angles close the edges of the yield surfaces.

### Influence of the eigen solver

In this section, we consider the influence of the eigen solver. In the
previous tests, the default eigen solver was used. This analytical
solver has been shown to provide a very interesting trade-off between
accuracy and numerical efficiency. It was also shown that the Jacobi
eigensolver provides very accurate results but is less efficient by
our default eigen solver (about 2 times less efficient).

Our tests, not reported here, shows that changing the eigen solver:

- does not change the results presented for exponents \(a=6\) and
  \(a=8\).
- does not increase the robustness of the first and second algorithms
  for \(a=100\).

![Robustness of Algorithm 3 for \(a=100\)](img/Scherzinger/Hosford100/MaximumIncrementValuePerIterationJacobi.png
 "Robustness of Algorithm 3 for \(a=100\)"){#fig:hosford100:alg3:jacobi width=70%}

Figure @fig:hosford100:alg3:jacobi shows that using the Jacobi eigen
solver has a strong influence on the robustness of Algorithm 3.

![Robustness of Algorithm 4 for \(a=100\)](img/Scherzinger/Hosford100/LevenbergMarquardtJacobi.png
 "Robustness of Algorithm 4 for \(a=100\)"){#fig:hosford100:alg4:jacobi width=70%}

Using the Jacobi eigen solver with Algorithm 4 shows that convergence
is reached for all tested values of \(\alpha\) and \(x\).

## Conclusions

Our tests shows that for high values of the Hosford exponent,
sophisticated algorithms and accurate eigen solvers are required to
guarantee the convergence of the algorithm for large trial stress.

However, for standard values of the Hosford exponent (\(a=6\) and
\(a=8\)), our results shows that Algorithm 2, which a very simple
modification of the standard Newton algorithm based on a simple and
intuitive physical consideration, is very robust.

Those results has to confirmed on real structural analyses. In
particular, the impact of each algorithms on the overall efficiency of
the simulations has to be evaluated.

# References

<!-- Local IspellDict: english -->
