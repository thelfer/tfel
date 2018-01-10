% How to implement an isotropic viscoplastic behaviour combining isotropic hardening and  multiple kinematic hardenings following an Armstrong-Frederic evolution of the back stress
% Thomas Helfer, Jérémy Hure
% 27/01/2017

\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tns}[1]{{\underset{\tilde{}}{\mathbf{#1}}}}
\newcommand{\transpose}[1]{{#1^{\mathop{T}}}}

\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\tenseur{\varepsilon}^{\mathrm{el}}}
\newcommand{\tepsilonvis}{\tenseur{\varepsilon}^{\mathrm{vis}}}
\newcommand{\tdepsilonvis}{\tenseur{\dot{\varepsilon}}^{\mathrm{vis}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\sigmaH}{\sigma_{H}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sderiv}[2]{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial #2^{2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}

This article shows how to implement an isotropic viscoplastic
behaviour combining isotropic hardening and multiple kinematic
hardenings following an Armstrong-Frederic evolution of the back
stress.

Such an example illustrates:

- The usage of `StandardElasticity` brick (see
  [this page](BehaviourBricks.html)).

The whole implementation is available
[here](./gallery/viscoplasticity/IsotropicViscoplasticityAmstrongFredericKinematicHardening.mfront).

# Description of the behaviour

The behaviour is described by a standard split of the strain
\(\tepsilonto\) in an elastic and a plastic parts, respectively
denoted \(\tepsilonel\) and \(\tepsilonvis\):

\[
\tepsilonto=\tepsilonel+\tepsilonvis
\]

## Elastic behaviour

The stress \(\tsigma\) is related to the the elastic strain
\(\tepsilonel\) by a the standard Hooke behaviour:

\[
\tsigma = \lambda\,\trace{\tepsilonel}\,\tenseur{I}+2\,\mu\,\tepsilonel
\]

## Viscoplastic behaviour

The viscoplastic behaviour follows a standard viscoplastic behaviour:
\[
\tdepsilonvis=\left\langle\Frac{F}{K}\right\rangle^{m}\,\tenseur{n}=\dot{p}\,\tenseur{n}
\]

where \(F\) is the yield surface defined below, \(<.>\) is Macaulay
brackets, \(\tenseur{n}\) is the normal to \(F\) with respect to the
stress and \(p\) is the equivalent plastic strain.

The yield surface is defined by:
\[
F\paren{\tsigma,\tenseur{X}_{i},p}=\paren{\tsigma-\sum_{i=1}^{N}\tenseur{X}_{i}}_{\mathrm{eq}}-R\paren{p}=s^{e}_{\mathrm{eq}}-R\paren{p}
\]

where:

- \(R\paren{p}\) describes the isotropic hardening as a function
  of the equivalent viscoplastic strain \(p\).
- the \(N\) tensors \(\tenseur{X}_{i}\) (i\in\left[1,N\right]) are
  backstresses describing the kinematic hardening.
- \(\paren{.}_{\mathrm{eq}}\) is the Von Mises norm.

We have introduced an effective deviatoric stress \(\tenseur{s}^{e}\) defined by:
\[
\tenseur{s}^{e}=\tenseur{s}-\sum_{i=1}^{N}\tenseur{X}_{i}
\]
where \(\tenseur{s}\) is the deviatoric part of the stress.

The normal is then given by:
\[
\tenseur{n}=\deriv{F}{\tsigma}=\Frac{3}{2}\,\Frac{\tenseur{s}^{e}}{s^{e}_{\mathrm{eq}}}
\]

## Evolution of the isotropic hardening

The isotropic hardening is defined by:
\[
R\paren{p}=R_{\infty} + \paren{R_{0}-R_{\infty}}\,\exp\paren{-b\,p}
\]

## Evolution of the kinematic hardenings

\[
\tenseur{X}_{i}=\Frac{2}{3}\,C_{i}\,\tenseur{a}_{i}
\]

The evolution of the kinematic variables \(\tenseur{a}_{i}\) follows
the Armstrong-Frederic rule:

\[
\tenseur{\dot{a}}_{i}=\tdepsilonvis-g[i]\,\tenseur{a}_{i}\,\dot{p}=\dot{p}\,\paren{\tenseur{n}-g[i]\,\tenseur{a}_{i}}
\]

# \(\theta\)-scheme

The previous consitutive equations will be integrated using a
\(\theta\)-scheme. The increments of the unknowns satisfy:

\[
\left\{
\begin{aligned}
\Delta\,\tepsilonel&=\Delta\,\tepsilonto+\Delta\,p\,\mts{\tenseur{n}}\\
\Delta\,p&=\left\langle\Frac{\mts{F}}{K}\right\rangle^{m}\,\Delta\,t\\
\Delta\,\tenseur{a}_{i}&=\Delta\,p\,\paren{\tenseur{n}-g[i]\,\mts{\tenseur{a}_{i}}}\\
\end{aligned}
\right.
\]

where the following notations have been used:

- \(\mts{\tepsilonel}=\bts{\tepsilonel}+\theta\,\Delta\,\tepsilonel\)
- \(\mts{\tsigma} = \lambda\,\trace{\mts{\tepsilonel}}\,\tenseur{I}+2\,\mu\,\mts{\tepsilonel}\)
- \(\mts{p} = \bts{p}+\theta\Delta\,p\)
- \(\mts{\tenseur{a}_{i}} = \bts{\tenseur{a}_{i}}+\theta\Delta\,\tenseur{a}\)
- \(\mts{\tenseur{X}_{i}} = C_{i}\,\mts{\tenseur{a}_{i}}\)
- \(\mts{\tenseur{s}^{e}}=\mts{\tenseur{s}}-\sum_{i=1}^{N}\mts{\tenseur{X}_{i}}\)
- \(\mts{s^{e}_{\mathrm{eq}}}=\paren{\mts{\tenseur{s}^{e}}}_{\mathrm{eq}}\)
- \(\mts{F}=\mts{s^{e}_{\mathrm{eq}}}-R\paren{\mts{p}}\)

# Implicit system formulation and implementation

The previous equations can be translated in an
implicit system in a straightforward manner:

\[
\left\{
\begin{aligned}
f_{\tepsilonel}&=\Delta\,\tepsilonel+\Delta\,p\,\mts{\tenseur{n}}-\Delta\,\tepsilonto=\tenseur{0}\\
f_{p}&=\Delta\,p-\left\langle\Frac{\mts{F}}{K}\right\rangle^{m}\,\Delta\,t=0\\
f_{\tenseur{a}_{i}}&=\Delta\,\tenseur{a}_{i}-\Delta\,\tepsilonvis-g[i]\,\mts{\tenseur{a}_{i}}\,\Delta\,p=\tenseur{0}
\end{aligned}
\right.
\]

## Computation of the jacobian

### Terms related to \(f_{\tepsilonel}\)

> **Derivative of the normal**
>
> In the following, we will make use of the "classical" relationship
> giving the derivative of the normal:
> \[
> \deriv{\mts{\tenseur{n}}}{\mts{\tenseur{s}}}=\Frac{1}{\mts{s^{e}_{\mathrm{eq}}}}\,\paren{\tenseurq{M}-\mts{\tenseur{n}}\otimes\mts{\tenseur{n}}}
> \]
>
> Here, \(\tenseurq{M}\) is a tensor space defined by:
> \[
> \sigmaeq=\sqrt{\tsigma\,\colon\,\tenseurq{M}\,\colon\,\tsigma}
> \]
>
> \(\tenseurq{M}\) is available in MFront as the result of a static
> member of the `st2tost2` template class. In `MFront`, the alias
> `Stensor4` is usually used to refer to the `st2tost2` class for the
> current numeric type and space dimension.

Here are the expressions of the term related to \(f_{\tepsilonel}\):

\[
\left\{
\begin{aligned}
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel}&=\tenseurq{I}+\Delta\,p\deriv{\mts{\tenseur{n}}}{\Delta\,\tepsilonel}
=\tenseurq{I}+\Delta\,p\,\cdot\,\underbrace{\deriv{\mts{\tenseur{n}}}{\mts{\tsigma}}}_{\Frac{1}{\mts{s^{e}_{\mathrm{eq}}}}\,\paren{\tenseurq{M}-\mts{\tenseur{n}}\otimes\mts{\tenseur{n}}}}\,\cdot\,\underbrace{\deriv{\mts{\tsigma}}{\mts{\tepsilonel}}}_{\lambda\,\tenseur{I}\otimes\tenseur{I}+2\,\mu\,\tenseur{I}}\,\cdot\,\underbrace{\deriv{\mts{\tepsilonel}}{\Delta\,\tepsilonel}}_{\theta\,\tenseur{I}}\\
\deriv{f_{\tepsilonel}}{\Delta\,p}&=\mts{\tenseur{n}}\\
\deriv{f_{\tepsilonel}}{\Delta\,\tenseur{a}_{i}}&=\Delta\,p\,\cdot\,\underbrace{\deriv{\mts{\tenseur{n}}}{\mts{\tenseur{s}}}}_{\Frac{1}{\mts{s^{e}_{\mathrm{eq}}}}\,\paren{\tenseurq{M}-\mts{\tenseur{n}}\otimes\mts{\tenseur{n}}}}\,\cdot\,\underbrace{\deriv{\mts{\tenseur{s}}}{\mts{\tenseur{a}_{i}}}}_{-\Frac{2}{3}\,C_{i}}\,\cdot\,\underbrace{\deriv{\mts{\tenseur{a}_{i}}}{\Delta\,a_{i}}}_{\theta\,\tenseurq{I}}\\
\end{aligned}
\right.
\]

Finally,

\[
\left\{
\begin{aligned}
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel}
&=\tenseurq{I}+\Frac{2\,\,\mu\,\theta\,\Delta\,p}{\mts{s^{e}_{\mathrm{eq}}}}\,\paren{\tenseurq{M}-\mts{\tenseur{n}}\otimes\mts{\tenseur{n}}}\\
\deriv{f_{\tepsilonel}}{\Delta\,p}
&=\mts{\tenseur{n}}\\
\deriv{f_{\tepsilonel}}{\Delta\,\tenseur{a}_{i}}
&=-\Frac{2\,C_{i}\,\theta\,\Delta\,p}{3\,\mts{s^{e}_{\mathrm{eq}}}}\,\paren{\tenseurq{M}-\mts{\tenseur{n}}\otimes\mts{\tenseur{n}}}\\
\end{aligned}
\right.
\]

### Terms related to \(f_{p}\)

> **Derivative of the equivalent stress**
>
> In the following, we will make use of another "classical"
> relationship giving the derivative of the equivalent stress:
> \[
> \deriv{\mts{s^{e}_{\mathrm{eq}}}}{\mts{\tenseur{s}}}=\mts{\tenseur{n}}
> \]

To compute the terms of the jacobian associated with \(f_{p}\), we
need the derivatives of \(\mts{F}\) with respect to
\(\Delta\,\tepsilonel\), \(\Delta\,p\) and
\(\Delta\,\tenseur{a}_{i}\). Assuming that \(\mts{F}\) is positive, we
have:

\[
\left\{
\begin{aligned}
\deriv{\mts{F}}{\Delta\,\tepsilonel}
&=\underbrace{\deriv{\mts{F}}{\mts{\tsigma}}}_{\mts{\tenseur{n}}}\,\cdot\,\underbrace{\deriv{\mts{\tsigma}}{\mts{\tepsilonel}}}_{\lambda\,\tenseur{I}\otimes\tenseur{I}+2\,\mu\,\tenseur{I}}\,\cdot\,\underbrace{\deriv{\mts{\tepsilonel}}{\Delta\,\tepsilonel}}_{\theta\,\tenseur{I}}=2\,\mu\,\theta\,\mts{\tenseur{n}}\\
\deriv{\mts{F}}{\Delta\,p} &=-\theta\,\deriv{R}{p} \\
\deriv{\mts{F}}{\Delta\,\tenseur{a}_{i}} &=-=\underbrace{\deriv{\mts{F}}{\tenseur{s}}}_{\mts{\tenseur{n}}}\,\cdot\,\underbrace{\deriv{\mts{\tenseur{s}}}{\mts{\tenseur{a}_{i}}}}_{-\Frac{2}{3}\,C_{i}\,\tenseurq{I}}\,\cdot\,\underbrace{\deriv{\mts{\tenseur{a}_{i}}}{\Delta\,\tenseur{a}_{i}}}_{\theta\,\tenseur{I}}=-\Frac{2}{3}\,C_{i}\,\mts{\tenseur{n}}\\
\end{aligned}
\right.
\]

The blocks of the jacobian associated with \(f_{p}\) are then:
\[
\left\{
\begin{aligned}
\deriv{f_{p}}{\Delta\,\tepsilonel}&=\deriv{f_{p}}{\mts{F}}\,\cdot\,\deriv{\mts{F}}{\Delta\,\tepsilonel}\\
\deriv{f_{p}}{\Delta\,p}&=1+\deriv{f_{p}}{\mts{F}}\,\cdot\,\deriv{\mts{F}}{\Delta\,p}\\
\deriv{f_{p}}{\Delta\,\tenseur{a}_{i}}&=\deriv{f_{p}}{\mts{F}}\,\cdot\,\deriv{\mts{F}}{\Delta\,\tenseur{a}_{i}}\\
\end{aligned}
\right.
\]

where \(\deriv{f_{p}}{\mts{F}}=-\Frac{m\,\Delta\,t}{K}\left\langle\Frac{\mts{F}}{K}\right\rangle^{m-1}\,\Delta\,t\)

### Terms related to \(f_{a_{i}}\)

<!-- \Delta\,\tenseur{a}_{i}&=\Delta\,p\,\paren{\tenseur{n}-g[i]\,\mts{\tenseur{a}_{i}}} -->

\[
\left\{
\begin{aligned}
\deriv{f_{\tenseur{a}_{i}}}{\Delta\,\tepsilonel}&
=-\Frac{2\,\mu\,\theta\,\Delta\,p}{\mts{s^{e}_{\mathrm{eq}}}}\,\paren{\tenseurq{M}-\mts{\tenseur{n}}\otimes\mts{\tenseur{n}}} \\
\deriv{f_{\tenseur{a}_{i}}}{\Delta\,p}&
=-\paren{\tenseur{n}-g[i]\,\mts{\tenseur{a}_{i}}} \\
\deriv{f_{\tenseur{a}_{i}}}{\Delta\,\tenseur{a}_{j}}&
=\delta_{ij}\,\tenseurq{I}+\Frac{2\,C_{j}\,\theta\,\Delta\,p}{3\,\mts{s^{e}_{\mathrm{eq}}}}\,\paren{\tenseurq{M}-\mts{\tenseur{n}}\otimes\mts{\tenseur{n}}} \\
\end{aligned}
\right.
\]

## Implementation

The `Implicit` domain specific language is used to integrate a
behaviour using a \(\theta\)-scheme:

### Choice of the DSL

~~~~{.cpp}
@DSL Implicit;
~~~~

### Behaviour name

We then give the name of the behaviour:

~~~~{.cpp}
@Behaviour IsotropicViscoplasticityAmstrongFredericKinematicHardening;
~~~~

### The `StandardElasticity` brick

To implement this behaviour, we will use the `StandardElasticity`
brick which provides:

- Automatic computation of the stress tensor at various stages of the
  behaviour integration.
- Automatic computation of the consistent tangent operator.
- Automatic support for plane stress and generalized plane stress
  modelling hypotheses (definitions of the axial strain as an
  additional state variables and the associated equation enforcing the
  plane stess condition).
- Automatic addition of the "standard" terms (see below) associated
  with the elastic strain state variable.

This behaviour brick is fully described [here](BehaviourBricks.html).

The usage of the `StandardElasticity` is declared as follows:

~~~~{.cpp}
@Brick StandardElasticity;
~~~~

### Default numerical parameters

A fully implicit integration scheme is choosen as the default:

~~~~{.cpp}
@Theta 1;
~~~~

This value can be dynamically changed at runtime by modifying the
`theta` parameter.

The stopping criterion is chosen low, to ensure the quality of the
consistent tangent operator (the default value, \(10^{-8}\) is enough
to ensure a good estimation of the state variable evolution, but is
not enough to have a proper estimation of the consistent tangent
operator):

~~~~{.cpp}
@Epsilon 1e-13;
~~~~

This value can be dynamically changed at runtime by modifying the
`epsilon` parameter.

### Definition of the state variables

We first define the equivalent viscoplastic strain:

~~~~{.cpp}
@StateVariable strain    p;
p.setGlossaryName("EquivalentViscoplasticStrain");
~~~~

We then define the kinematic variables as an array of two state
variables:

~~~~{.cpp}
@StateVariable StrainStensor a[2];
a.setEntryName("KinematicVariables");
~~~~

### Definition of the material properties

~~~~{.cpp}
@MaterialProperty stress young;
young.setGlossaryName("YoungModulus");
@MaterialProperty real nu;
nu.setGlossaryName("PoissonRatio");

@MaterialProperty stress Rinf;
@MaterialProperty stress R0;
@MaterialProperty real b;
@MaterialProperty stress C[2];
@MaterialProperty real   g[2];
@MaterialProperty real m;
@MaterialProperty real K;
~~~~

### Definition of the local variable

~~~~{.cpp}
@LocalVariable stress mu;
~~~~

### Initialisation of the local variable

~~~~{.cpp}
/* Initialize Lame coefficients */
@InitializeLocalVariables{
  mu = computeMu(young,nu);
}
~~~~

### Definition of the implicit system

The definition of the implicit system is done in the `@Integrator`
code block:

~~~~{.cpp}
@Integrator{
~~~~

To understand this implementation of this code block, one shall bear
in mind the following conventions:

- The current estimation of the stress state \(\mts{\tsigma}\) at
  \(t+\theta\,dt\) is computed by the `Standardlasticity` brick prior
  the evaluation of the implicit system and is stored in the `sig`
  variable.
- For a given integration variable \(X\), the variable `X` denotes
  \(\bts{X}\) and `dX` the current estimation of the increment of
  \(X\). Thus, the computation of \(\mts{X}\) is written
  `X+theta*dX`. In the following, we declare a new variable called
  `X_` to hold the current estimation of \(\mts{X}\).
- For two integration variables \(X\) and \(Y\), the variable `fX`
  denotes the equation of the implicit system associated with \(X\)
  and `fY` the equation associated with \(Y\). The variable `dfX_ddY`
  holds the derivative \(\deriv{f}{\Delta\,Y}\).

First, a few constant values are defined:

~~~~{.cpp}
  const real two_third     = 2/real(3);
  constexpr const real eps = real(1.e-12);
~~~~

According the previous conventions, we compute the current estimation
of \(\mts{p}\):

~~~~{.cpp}
  const auto p_ = p +theta*dp ;
~~~~

The computation of the current effective stress and the current
estimation of the kinematic variables is straightforward:

~~~~{.cpp}
  auto se     = sig ;
  StrainStensor a_[2];
  for(unsigned short i=0;i!=2;++i){
     a_[i]  = a[i]+theta*da[i];
     se  -= C[i]*a_[i]*two_third;
  }
~~~~

The current estimation of the equivalent stress is then:

~~~~{.cpp}
  const auto seq = sigmaeq(se);
~~~~

The current estimation of \(\mts{R}\) is then evaluated:

~~~~{.cpp}
  const auto Rp  = Rinf + (R0-Rinf)*exp(-b*p_) ;
~~~~

\(\mts{F}\) is then evaluated:

~~~~{.cpp}
  const auto F   = seq - Rp ;
~~~~

If the current estimation of \(F\) is positive, we define the implicit
system. Otherwise, the default initialization of the implicit equations

~~~~{.cpp}
  if(F > 0){
    const auto Fexp = pow(F/K,m-1)/K ;
    const auto iseq = 1/(max(seq,eps*young));
    const auto n    = 3*iseq*deviator(se)/2;
    const auto vp   = Fexp*F;
    const auto Jmn = eval(Stensor4::M()-(n^n));
    feel += dp*n;
    fp   -= vp*dt;
    for(unsigned short i=0;i!=2;++i){
      fa[i] -= dp*(n-g[i]*a_[i]);
    }
    // computation of the jacobian matrix
    dfeel_ddeel += 2*mu*theta*dp*Jmn*iseq ;
    dfeel_ddp    = n;
    dfp_ddeel    = - Fexp*m*dt* 2*mu*theta*(n| Stensor4::M() )*two_third;
    dfp_ddp     += theta* Fexp *m *dt*b*(Rinf-Rp);
    for(unsigned short i=0;i!=2;++i){
      dfeel_dda(i)   = -C[i]*dp*theta*iseq*two_third*Jmn ;
      dfp_dda(i)     =  Fexp*m*dt*C[i]*theta*two_third*n;
      dfa_ddeel(i)   = -2*mu*theta*dp*Jmn *iseq;
      dfa_ddp(i)     = -n + g[i]*a_[i];
      dfa_dda(i,i)   =  (1+dp*g[i]*theta)*Stensor4::Id()+C[i]*dp*theta*iseq*two_third*Jmn;
    }
    dfa_dda(0,1)   =  C[1]*dp*theta*iseq*two_third*Jmn;
    dfa_dda(1,0)   =  C[0]*dp*theta*iseq*two_third*Jmn;
  }
}
~~~~

## An easy optimization

The last equation can be easily eliminated as
\(\Delta\,\tenseur{a}_{i}\) can be expressed as a simple function of
\(\Delta\,p\) and \(\Delta\,\tepsilonel\):

\[
\left\{
\begin{aligned}
\Delta\,\tenseur{a}_{i}=\Frac{\Delta\,p\,\mts{\tenseur{n}}}{1+g_{i}\,\bts{\tenseur{a}_{i}}\,\Delta\,p}
\end{aligned}
\right.
\]

Such an optimization is strongly encouraged as the reduction in the
system size is significant. This optimization can be introducted in a
straightforward manner:

- The kinematic variable \(\tenseur{a}_{i}\) must be defined as an
  auxiliary state variable (using the `@AuxiliaryStateVariable`
  keyword) and updated after the integration using the
  `@UpdateAuxiliaryStateVariables` code block.
- The updated values of \(\mts{\tenseur{a}_{i}}\) must be computed
  with the previous update formulae.
- The derivatives with respect to the kinematic variables
  \(\tenseur{a}_{i}\) must be added to the derivatives with respect to
  \(\Delta\,p\) and \(\Delta\,\tepsilonel\) using the chain rule.

> **Reduction to a scalar equation**
>
> Using the isotropy assumption, this system of equations can be
> further optimised by a reduction to a scalar equation with
> \(\Delta\,p\) as the only unknown (see @chaboche_integration_1996
> for details). However, with this operation, the `StandardElasticity`
> brick can't be used anymore. Thus plane stress an generalised plane
> stress hypotheses would require a specific treatment and the
> expression of the consistent tangent operator, which becomes quite
> complex, has to be written by hand.

# References

<!-- Local IspellDict: english -->

