% Implementing an isotropic elliptic yield criterion of the Green type
% Thomas Helfer
% 30/11/2017

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
\newcommand{\tepsilonp}{\tenseur{\varepsilon}^{\mathrm{p}}}
\newcommand{\tdepsilonp}{\tenseur{\dot{\varepsilon}}^{\mathrm{p}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sderiv}[2]{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial #2^{2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}

This article shows how to implement an isotropic elliptic yield
criterion of the Green type in `MFront`. Such an example illustrates
the usage of `StandardElasticity` brick (see
[this page](BehaviourBricks.html)). This page is inspired by the paper
of Fritzen et al. (See @fritzen_computational_2013).

The whole implementation is available
[here](./gallery/plasticity/GreenPerfectPlasticity.mfront).

# Description of the behaviour

The behaviour is described by a standard decomposition of the strain
\(\tepsilonto\) in an elastic and a plastic component, respectively
denoted \(\tepsilonel\) and \(\tepsilonp\):

\[
\tepsilonto=\tepsilonel+\tepsilonp
\]

## Elastic behaviour

The stress \(\tsigma\) is related to the the elastic strain
\(\tepsilonel\) by a the standard Hooke law expressed using the Lamé
coefficients:
\[
\tsigma = \lambda\,\trace{\tepsilonel}\tenseur{I}+2\,\mu\,\tepsilonel
\]

Cette relation est directement prise en charge par la brique
`StandardElasticity`.

## Plastic flow

The behaviour is based on the definition of an equivalent stress
\(\sigmaeq\) defined as follows:
\[
\sigmaeq=\sqrt{\Frac{3}{2}\,C\,\tenseur{s}\,\colon\,\tenseur{s}+F\,\trace{\tsigma}^{2}}
\]
where \(\tenseur{s}\) is the deviatoric stress tensor:
\[
\tenseur{s}=\tsigma-\Frac{1}{3}\,\trace{\tsigma}\,\tenseur{I}
\]
This equivalent stress is an homogeneous function of the stress of
degree 1.

The plastic part of the behaviour is described by the following yield
surface:
\[
f\paren{\tsigma} = \sigmaeq-\sigma_{0}
\]
where \(\sigma_{0}\) is the yield stress.

Following the principle of maximum plastic dissipation, the plastic
flow is assumed to be associated, so the flow direction
\(\tenseur{n}\) is given by \(\deriv{f}{\tsigma}\):

\[
\tenseur{n} = \deriv{f}{\tsigma} = \Frac{1}{\sigmaeq}\,\paren{\Frac{3}{2}\,C\,\tenseur{s}+F\,\trace{\tsigma}\,\tenseur{I}}
\]

Since the equivalent stress is an homogeneous function of degree 1,
the equivalent plastic strain \(p\) is well defined and the plastic
strain rate can be written:
\[
\tdepsilonp=\dot{p}\,\tenseur{n}
\]

# Integration algorithm

The previous constitutive equations will be integrated using a
standard implicit scheme.

The first thing that the integration will determine is whether a
plastic flow occurs during the time step. To do this, a plastic
prediction of the stress is made. If this prediction is inside the
yield surface, the step is purely elastic and the integration is
trivial. On the other hand, the material must lie on the yield surface
at the end of the time step.

## State variables

The state variable considered will be:

- the elastic strain \(\tepsilonel\), which is automatically declared
  by the `StandardElasticity` brick as `eel`.
- the equivalent plastic strain \(p\).

## Plastic loading case

### Implicit system

Assuming a plastic loading, the system of equations to be solved is:
\[
\left\{
\begin{aligned}
	\Delta\,\tepsilonel-\Delta\,\tepsilonto+\Delta\,p\,\mts{\tenseur{n}} &= 0 \\
	f\paren{\mts{\sigmaeq}} &= 0 \\
\end{aligned}
\right.
\]

where \(\mts{X}\) is the value of \(X\) at \(t+\theta\,\Delta\,t\),
\(\theta\) being a numerical parameter. In the following, the first
(tensorial) equation is noted \(f_{\tepsilonel}\) and the second
(scalar) equation is noted \(f_{p}\).

In practice, it is physically sound to make satisfy exactly the yield
condition at the end of the time step (otherwise, stress extrapolation
can lead to stress state outside the yield surface and spurious
oscillations can also be observed). This leads to the choice
\(\theta=1\).

### Computation of the jacobian

The jacobian \(J\) of the implicit system can be decomposed by blocks:
\[
J=
\begin{pmatrix}
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel} & \deriv{f_{\tepsilonel}}{\Delta\,p} & \\\\
\deriv{f_{p}}{\Delta\,\tepsilonel} & \deriv{f_{p}}{\Delta\,p} \\
\end{pmatrix}
\]

The expression of the previous terms is given by:

\[
\left\{
\begin{aligned}
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel} &= \tenseur{I} + dp\,\deriv{\mts{\tenseur{n}}}{\Delta\,\tepsilonel}\\
\deriv{f_{\tepsilonel}}{\Delta\,p} &= \mts{\tenseur{n}}\\
\deriv{f_{p}}{\Delta\,\tepsilonel} &= -\theta\,\mts{\tenseur{n}}\,\colon\,\mts{\tenseurq{D}}\\
\deriv{f_{p}}{\Delta\,p}           &= 0
\end{aligned}
\right.
\]

\[
\begin{aligned}
\deriv{\mts{\tenseur{n}}}{\Delta\,\tepsilonel}
&=\deriv{\mts{\tenseur{n}}}{\mts{\sigma}}\,\deriv{\mts{\sigma}}{\mts{\tepsilonel}}\,\deriv{\mts{\tepsilonel}}{\Delta\,\tepsilonel}\\
&=\Frac{\theta}{\sigmaeq}\,\paren{\Frac{3}{2}\,C\,\paren{\tenseurq{I}-\Frac{1}{3}\,\tenseur{I}\,\otimes\,{\tenseur{I}}}+F\,\tenseur{I}\,\otimes\,{\tenseur{I}}-\mts{\tenseur{n}}\,\otimes\,\mts{\tenseur{n}}}\,\colon\,\mts{\tenseurq{D}}\\
&=\Frac{\theta}{\sigmaeq}\,\paren{\Frac{3}{2}\,C\,\,\tenseurq{I}+\paren{F-\paren{\Frac{C}{2}}}\,\tenseur{I}\,\otimes\,\tenseur{I}-\mts{\tenseur{n}}\,\otimes\,\mts{\tenseur{n}}}\,\colon\,\mts{\tenseurq{D}}\\
\end{aligned}
\]

This expression could be further simplified.

\paren{\mts{\lambda}\,\tenseur{I}\,\otimes\,\tenseur{I}+2\,\mts{\mu}\,\tenseurq{I}}

## Elastic loading case

Assuming an elastic loading, the system of equations to be solved is
trivially:
\[
\left\{
\begin{aligned}
	\Delta\,\tepsilonel-\Delta\,\tepsilonto &= 0 \\
	\Delta\,p &= 0 \\
\end{aligned}
\right.
\]

The jacobian associated with this system is the identity matrix.

# Implementation

In the plastic loading case, the system of equations to be solved is
*non-linear*. We choose the Newton-Raphson algorithm with an
analytical jacobian. Compared to other algorithm available in `MFront`
(Runge-Kutta, Broyden, Newton-Raphson with numerical jacobian, etc..),
this algorithm is generally the *most efficient* in pratice.

## Preamble

The implementation begins with the choice of the `Implicit` domain
specific language (dsl):

~~~~{.cpp}
@DSL Implicit;
~~~~

Note that this dsl automatically declares the elastic strain `eel` as
a state variable.

As discussed before, we explicit state that a fully implicit
integration will be used by default:

~~~~{.cpp}
@Theta 1;
~~~~

This value can be changed at runtime by modifying the `theta`
parameter.

The stopping criterion is chosen low, to ensure the quality of the
consistent tangent operator (the default value, \(10^{-8}\) is enough
to ensure a good estimation of the state variable evolution, but is
not enough to have a proper estimation of the consistent tangent
operator):

~~~~{.cpp}
@Epsilon 1e-14;
~~~~

We then declare that we want to support all the modelling hypotheses:

~~~~{.cpp}
@ModellingHypotheses {".+"};
~~~~

The support of plane stress modelling hypotheses are handled by the
`StandardElasticity` brick and will not be discussed here.

### Usage of the `StandardElasticity` brick

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

### Elastic stiffness tensor

The elastic stiffness tensor \(D\) is defined using
`@ComputeStiffnessTensor` keyword by giving the elastic material
properties as constants:

~~~~{.cpp}
@ComputeStiffnessTensor<UnAltered> {150e9,0.3};
~~~~

This computed stiffness is stored in a variable `D`. A second variable
`D_tdt` is also introduced. As the material properties are constants,
`D_tdt` is an alias to `D`.

The elastic material properties can be changed at runtime time by
modifying the following parameters: `YoungModulus` and `PoissonRatio`.

Rather than constants, one can also use correlations implemented in
seperate `MFront` files. This allows to take into account the
dependency of the material properties with the temperature for
example. In this case, the variable `D` contains the stiffness tensor
at \(t+\theta\,\Delta\,t\) and the variable `D_tdt` contains the
stiffness tensor at \(t+\Delta\,t\).

Another possibility is to use the `@RequireStiffnessTensor`
keyword. In this case, the elastic material properties must be
computed by the calling solver at the end of the time step (and
furnished to the mechanical behaviours through hidden material
properties).

### Variable declarations

#### State variables

As stated earlier, the state variable `eel` is automatically declared
by the `Implicit` dsl.

The equivalent plastic strain state variable `p` is declared as:

~~~~{.cpp}
@StateVariable real p;
~~~~

We then associate the appropriate glossary name to this variable:

~~~~{.cpp}
p.setGlossaryName("EquivalentPlasticStrain");
~~~~

#### Parameters

The definition of yield surface introduces three material coefficients
\(C\), \(F\) and \(\sigma_{0}\) that we declare as a parameter:

~~~~{.cpp}
@Parameter C = 0.8;
C.setEntryName("GreenYieldCriterion_C");
@Parameter F = 0.2;
F.setEntryName("GreenYieldCriterion_F");
@Parameter s0 = 150e6;
s0.setGlossaryName("YieldStress");
~~~~

The `YieldStress` is an entry of the glossary (see
[here](glossary.html)).

#### Local variable

To select the implicit system associated either with the elastic or
plastic loading case, we introduce a boolean local variable `b`.

~~~~{.cpp}
@LocalVariable bool b; // if true, plastic loading
~~~~

## Initialization of the local variables, determination of the loading case

Before solving the implicit system, the code block introduced by the
`@InitLocalVariables` keyword is executed. For this behaviour, this
block will select either the elastic or plastic loading case.

We first make an *elastic prediction* of the stress at the end of the
time step. We use the `computeElasticPrediction` method introduced by
the `StandardElasticity` brick. This method takes into account the
modelling hypothesis, which is mandatory for plane stress modelling
hypotheses. We then make an elastic prediction of the Hill equivalent
stress and check whether or not this elastic prediction is inside the
elastic domain. The latter information is stored in the boolean value
`b` which will be `false` (no plastic loading) if the loading is
elastic.

~~~~{.cpp}
@InitLocalVariables{
  const auto sig_el  = computeElasticPrediction();
  const auto s_el    = deviator(sig_el);
  const auto tr_el   = trace(sig_el);
  const auto seq     = sqrt(3*C*(s_el|s_el)/2+F*tr_el*tr_el);
  b = seq-s0 > 0;
}
~~~~

## Implicit system and jacobian

Finally, we describe how the implicit system and the computation of
the jacobian is written in a code block introduced by the
`@Integrator` keyword. 

We use the following facts:

- The equations of implicit system are initialized to the state
  variables increments (i.e. `feel` is initialized to `deel`).
- The jacobian \(J\) is initialized to the identity
  (i.e. `dfeel_ddeel` is initialized to the identity tensor).
- The increment of the total strain is automatically deduced from
  `feel` by the `StandardElasticity` brick.

Apart from those facts, the code is an almost direct translation of
the mathematical expression described in previous sections and boils
down to the following lines of code:

~~~~{.cpp}
@Integrator{
constexpr const auto id    = Stensor::Id();
  constexpr const auto id4 = Stensor4::Id();
  if(b){
    const auto hC   = C/2;
    const auto s    = deviator(sig);
    const auto tr   = trace(sig);
    const auto seq  = sqrt(3*hC*(s|s)+F*tr*tr);
    const auto iseq = 1/(max(seq,real(1.e-10*young)));
    const auto n    = eval(iseq*(3*hC*s+F*tr*id));
    // elasticity
    feel        += dp*n;
    dfeel_ddeel += theta*dp*iseq*(3*hC*id4+(F-hC)*(id^id)-(n^n))*D;
    dfeel_ddp    = n;
    // plasticity
    fp           = (seq-s0)/young;
    dfp_ddp      = strain(0);
    dfp_ddeel    = theta*(n|D)/young;
  }
}
~~~~

# References

<!-- Local IspellDict: english -->
