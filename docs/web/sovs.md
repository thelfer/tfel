% Skorohold-Olevsky Viscous Sintering (SOVS) model
% Balaguer J. (Instituto de Tecnología Cerámica (ITC-AICE), Universitat Jaume I, Castellón (Spain)), Helfer T.
% May 2019

\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\tenseur{\varepsilon}^{\mathrm{el}}}
\newcommand{\tepsilonin}{\tenseur{\varepsilon}^{\mathrm{in}}}
\newcommand{\tdepsilonto}{\tenseur{\dot{\varepsilon}}^{\mathrm{to}}}
\newcommand{\tdepsilonel}{\tenseur{\dot{\varepsilon}}^{\mathrm{el}}}
\newcommand{\tdepsilonin}{\tenseur{\dot{\varepsilon}}^{\mathrm{in}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\sigmaH}{\sigma_{H}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/IGocYZKyvgQ" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen>
</iframe>
</center>

This article shows the implementation of an isotropic linear viscous
model based on Skorohold-Olevsky Viscous Sintering (SOVS) model. This
page is inspired by the paper of Lester @Lester2017. For a more
comprenhensive reading about the model, refer to the original paper of
Olevsky @Olevsky1998.

This behaviour will be integrated using an implicit scheme.Such an
example illustrates:

- the usage of `StandardElasticity` brick (see [this
  page](BehaviourBricks.html)).
- how to use an auxiliary state variable and an integration variable to
  reduce the size of the implicit system.

The full implementation is available [here](gallery/viscoelasticity/SkoroholdOlevskyViscousSinteringBehaviour.mfront)

# About the Skorohold-Olevsky Viscous Sintering (SOVS) model

SOVS model predicts sintering shrinkage and densification of ceramics
and composites. It serves to enhance our ability to understand, predict,
and control sintering. Additionally, it provides the potencial for more
cost-effective net or near-net-shape manufacturing @ZIPSE19971707.

SOVS model has been succesfully used to predict the sintering of ZnO
@Reiterer2006 and porous bilayered bodies [@Ni2013;@MOLLA20131297].
Regarding the ceramic tiles, viscosity models have been developed to
predict the shrinkage and the residual stresses that appear when
materials of the porcelain tile type are cooled
[@dal2012modelizacion;@MILANI2019543]. SOVS model can extend the
functionality of those viscous models to predict the sintering, whose
investigation is in process.

# Description

The behaviour is described by a standard decomposition of the strain
\(\tepsilonto\) in an elastic and a viscous component,
respectively denoted \(\tepsilonel\) and
\(\tepsilonin\):

\[
 \tepsilonto = \tepsilonel + \tepsilonin
\]{#eq:split}

This equation can be written in rate form as follows:

\[
 \tdepsilonto = \tdepsilonel + \tdepsilonin
\]{#eq:split:rate}

## Elasticity

The stress \(\tsigma\) is related to the the elastic strain \(\tepsilonel\)
by the standard Hooke law expressed using the Lamé coefficients:

\[
 \tsigma = \lambda\,\trace{\tepsilonel}\, \tenseur{I} + 2\,\mu\,\tepsilonel
\]

## Viscosity

Assuming linear viscosity, the inelastic strain rate, \(\tdepsilonin\),
may be written as:

\[
 \tdepsilonin = \frac{\tenseur{s}}{2\,\eta_{0}(T)\,\phi(\rho)} + \frac{\trace{\tsigma}-3\,\sigma_{s}(\rho)}{18\,\eta_{0}(T)\,\psi(\rho)}\tenseur{I}
\]{#eq:viscosity}

where:

- \(\eta_{0}(T)\) is the shear viscosity of the fully dense skeleton
  phase.
- \(\phi(\rho)\) is the normalized shear viscosity.
- \(\psi(\rho)\) is the normalized bulk viscosity.
- \(\sigma_s\) is the sintering stress.  
- \(T\) is the temperature
- \(\rho\) is the relative density
- \(\tenseur{s}\) is the deviatoric stress tensor.

The dependency of the viscosities and the sintering stress used are the
following:

\[
  \phi(\rho)=a_{1}\rho^{b_{1}}, \qquad \psi(\rho)=a_{2}\frac{\rho^{b_{2}}}{(1-\rho)^{c_{2}}} 
\]

\[
  \sigma_{s}(\rho)=\sigma_{s0}\overline{\sigma}_{s}(\rho), \qquad \overline{\sigma}_{s}(\rho)=a_{3}\rho^{b_{3}} 
\]

\[
 \eta_{0}(T)=a_{4}e^{b_{4}/T}
\]

where:

- \(a_{i}\), \(b_{i}\) and \(c_{i}\) are fitting coefficients.
- \(\sigma_{s0}\) is the local sintering stress given by
  \(3\alpha/r_{0}\).
- \(\alpha\) is the surface tension. 
- \(r_{0}\) the average grain size. 

By projecting Equation @eq:viscosity on the hydrostatic and deviatoric
spaces, one gets:

\[
\begin{aligned}
\trace{\tdepsilonin}&=\frac{\trace{\tsigma}-3\,\sigma_{s}(\rho)}{6\,\eta_{0}(T)\,\psi(\rho)}\\
\tenseur{s}_{\tdepsilonin}&=\frac{\tenseur{s}}{2\,\eta_{0}(T)\,\phi(\rho)}\\
\end{aligned}
\]{#eq:viscosity2}

where \(\tenseur{s}_{\tdepsilonin}\) stands for the deviatoric part of \(\tdepsilonin\).

## Evolution of the relative density

Finally, the evolution of the relative density is:

\[
 \dot{\rho}=-\rho \, \, \trace{\tdepsilonin}
\]{#eq:density_evolution}

# Implicit scheme

The state variables are:

- the elastic strain \(\tepsilonel\).
- the inelastic strain \(\tepsilonin\).
- the relative density \(\rho\).

Their evolutions is governed by a a system of ordinary differential
equations given by Equations @eq:split:rate, @eq:viscosity and
@eq:density_evolution.

In its general form, a system of ordinary differential
equations may be written as:

\[
\vec{\dot{X}}=G\paren{\vec{X},t}
\]{#eq:ode}

where \(\vec{X}\) is the vector of the unknowns. Here \(\vec{X}\) is
decomposed as follows:

\[
\vec{X}=
\left(
\begin{aligned}
\tepsilonel \\
\tepsilonin \\
\rho \\
\end{aligned}
\right)
\]

In Equation @eq:ode, the time appears as a placeholder for external
state variables that varies independently of the mechanics. Here the
only external state behaviour is the temperature.

An implicit \(\theta\)-scheme turns this system of ordinary differential
equations into a system of of non-linear equations by introducing the
increment \(\Delta\,\vec{X}\) over a finite time step increment
\(\Delta\,t\) as follows:

\[
f\paren{\Delta\,X}=\Delta\,\vec{X}-\Delta\,t\,G\paren{\vec{X}+\theta\,\Delta\vec{X},t+\theta\,\Delta\,t}
\]
where \(\theta\) is a numerical parameter (\(0<\theta\leq 1\)).

Again \(\Delta\,X\) and \(f\paren{\Delta\,X}\) can be decomposed as follows:

\[
\Delta\,\vec{X}=
\left(
\begin{aligned}
\Delta\,\tepsilonel \\
\Delta\,\tepsilonin \\
\Delta\,\rho \\
\end{aligned}
\right)
\quad\text{and}\quad
f\paren{\Delta\,\vec{X}}=
\left(
\begin{aligned}
f_{\tepsilonel} \\
f_{\tepsilonin} \\
f_{\rho} \\
\end{aligned}
\right)
\]

This equation is solved by a Newton-Raphson algorithm.

In the following, the increment of the state varaibles, respectively
denoted \(\Delta\,\tepsilonel\), \(\Delta\,\tepsilonin\) and
\(\Delta\,\rho\) and he equations associated with those increment are
respectively denoted \(f_{\tepsilonel}\), \(f_{\tepsilonin}\) and
\(f_{\rho}\).

We now start by making an explicit derivation of \(f_{\tepsilonel}\),
\(f_{\tepsilonin}\) and \(f_{\rho}\), and we then discuss how the system
of implicit equations can be reduced.

## Implicit equation associated with the elastic strain

The implicit equation associated with the the elastic strain derives
from the split of strain (Equation @eq:split:rate) as follows:

\[
f_{\tepsilonel}=\Delta\,\tepsilonel+\Delta\,\tepsilonin-\Delta\,\tepsilonto
\]{#eq:feel}

## Implicit equation associated with the trace of the inelastic strain

Using Equation @eq:viscosity can be discretised as follows:

\[
f_{\tepsilonin}=
\Delta\,\tepsilonin -
\Delta\,t\,\frac{\mts{\tenseur{s}}}{2\,\eta_{0}(\mts{T})\,\phi\paren{\mts{\rho}}} -
\Delta\,t\,\frac{\mts{\trace{\mts{\tsigma}}}-3\,\sigma_{s}\paren{\mts{\rho}}}{18\,\eta_{0}(\mts{T})\,\psi\paren{\mts{\rho}}}\,\tenseur{I}
\]{#eq:DeltaEpsilonin}

where:

- \(\mts{\tenseur{s}}=2\,\mts{\mu}\,\tenseur{s}_{\mts{\tepsilonel}}= 2\,\mts{\mu}\,\paren{\mts{\tepsilonel}-\Frac{1}{3}\trace{\mts{\tepsilonel}}\,\tenseur{I}}\)
- \(\mts{\rho}=\bts{\rho}+\theta\,\Delta\,\rho\)

## Evolution of the relative density

Equation @eq:density_evolution can be discretized as the other equations
as follows:

\[
f_{\rho}=\Delta\,\rho+\mts{\rho}\,\trace{\Delta\,\tepsilonin}
\]{#eq:f_rho}

However, using this equation has a two major disadvantages:

1. The relative density \(\rho\) is bounded (i.e. positive) but it
  fairly complex to take this into account during the Newton resolution
  or event to enforce for large time increment.
2. It is trivial to eliminate this equation from the implicit system
  because the previous equation allows to express \(\Delta\,\rho\) as an
  explicit function of \(\Delta\,\tepsilonin\). So keeping, Equation
  @eq:f_rho leads to a less efficient integration scheme.

We now propose an alternative integration scheme for Equation
@eq:density_evolution that remove the limitations associated with the
first point and still allows to eliminate \(\Delta\,\rho\) from the
implicit system.

Indeed, Equation @eq:density_evolution can be integrated exactly over
the time step as follows:

\[
\begin{aligned}
 \dot{\rho}&=-\rho \, \trace{\tdepsilonin} \\
 \Frac{\dot{\rho}}{\rho}&=-\trace{\tdepsilonin} \\
 \int_{t}^{t+dt}\Frac{\dot{\rho}}{\rho}\,dt&=-\int_{t}^{t+dt}\trace{\tdepsilonin}\,dt \\
 \log\paren{\Frac{\ets{\rho}}{\bts{\rho}}}&=-\trace{\Delta\,\tdepsilonin} \\
\end{aligned}
\]

This leads to an explicit expression of \(\ets{\rho}\) as a function of
\(\bts{\rho}\) and the trace of the inelastic strain increment
\(\trace{\Delta\,\tdepsilonin}\):

\[
\ets{\rho}=\bts{\rho}\,\exp\paren{-\trace{\Delta\,\tdepsilonin}}
\]{#eq:rho_ets}

Similarly, the value of the relative density at \(t+\theta\,\Delta\,t\)
is equal to:

\[
 \mts{\rho}=\bts{\rho}\,\exp\paren{-\trace{\theta\,\Delta\,\tdepsilonin}}
\]{#eq:rho_mts}

Equation @eq:rho_mts will be used to eliminate the relative density from
the implicit system. Equation @eq:rho_ets will be used to compute the
final value of the relative density.

> **About auxiliary state variables**
> 
> Such a variable, whose value is kept from one time to another but is 
> not part of the implicit system, is called an *auxiliary state variable*
> in `MFront`. A dedicated code block named `@UpdateAuxiliaryStateVariables`
> is meant to update those variables after the behaviour integration. At
> this stage, the state variables have been updated to their final values
> and the stress at the end of the time step has been computed.

## Reduction of the implicit scheme

A close look at Equations @eq:feel, @eq:DeltaEpsilonin, and @eq:rho_mts
shows that it can be efficient to eliminate part of the unknowns and
keep the two following variables \(\Delta\,\tepsilonel\) and
\(\trace{\Delta\,\tepsilonin}\).

In the following, \(e\) denotes \(\trace{\tepsilonin}\):
\[
e=\trace{\tepsilonin}
\]

> **About integration variables**
>
> Here \(e\) is not an internal state variable, because there is no
> reason to keep its value from one step to the other. Such a variable
> is called in `MFront` an *integration variable*.
>
> In pratice, for this integration variable \(e\), `MFront` automatically
> defines:
>
> - a variable `e` meant to store the value of \(e\) at the beginning of
>   the time step if required (this is not the case here). This initial
>   value is meant to be computed in the `@InitLocalVariables` block from
>   the values of the state variables or auxiliary state variables.
> - its increment `de`
> - an associated equation in the implicit system called `fe`.
> -  the jacobian blocks associated with `fe` (here `dfe_dde` and `dfe_ddeel`).

Combining Equations @eq:feel, @eq:DeltaEpsilonin, and @eq:rho_mts leads
to the new system of equations:

\[
\left\{
\begin{aligned}
f_{\tepsilonel} &= \Delta\,\tepsilonel+
\Delta\,t\,\frac{\mts{\tenseur{s}}}{2\,\eta_{0}(\mts{T})\,\phi\paren{\mts{\rho}\paren{\Delta\,e}}} +
\Frac{1}{3}\,\Delta\,e\,\tenseur{I}  -\Delta\,\tepsilonto\\
f_{e}&=\Delta\,e-
3\,\Delta\,t\,\frac{\mts{\trace{\mts{\tsigma}}}-3\,\sigma_{s}\paren{\mts{\rho}\paren{\Delta\,e}}}{18\,\eta_{0}(\mts{T})\,\psi\paren{\mts{\rho}\paren{\Delta\,e}}}
\end{aligned}
\right.
\]{#eq:implicit:system}

where \(\mts{\rho}\paren{\Delta\,e}\) is given by Equation @eq:rho_mts.

For the sake of clarity, the previous equation can be rewritten as follows:
\[
\left\{
\begin{aligned}
f_{\tepsilonel} &= \Delta\,\tepsilonel+
\Delta\,t\,A\paren{\mts{\rho}\paren{\Delta\,e}}\,\mts{\tenseur{s}}+
\Frac{1}{3}\,\Delta\,e\,\tenseur{I} -\Delta\,\tepsilonto\\
f_{e}&=\Delta\,e-
3\,\Delta\,t\,\paren{B\paren{\mts{\rho}\paren{\Delta\,e}}\,\trace{\mts{\tsigma}}-C\paren{\mts{\rho}\paren{\Delta\,e}}}
\end{aligned}
\right.
\]{#eq:implicit:system:final}

where:

\[
\left\{
\begin{aligned}
A\paren{\mts{\rho}\paren{\Delta\,e}}&=\Frac{1}{{2\,\eta_{0}(\mts{T})\,\phi\paren{\mts{\rho}\paren{\Delta\,e}}}}\\
B\paren{\mts{\rho}\paren{\Delta\,e}}&=\Frac{1}{18\,\eta_{0}(\mts{T})\,\psi\paren{\mts{\rho}\paren{\Delta\,e}}}\\
C\paren{\mts{\rho}\paren{\Delta\,e}}&=3\,B\paren{\mts{\rho}\paren{\Delta\,e}}\,\sigma_{s}\paren{\mts{\rho}\paren{\Delta\,e}}\\
\end{aligned}
\right.
\]

Since the deviatoric part of the inelastic strain is linear with the
stress, the System @eq:implicit:system may be further reduced to a
scalar equation with \(\Delta\,e\) as the only unknowns. However, this
considerably complicates the implementation of the behaviour, and in
particular, the derivation of the consistent tangent operator. Hence,
for the sake simplicity, this reduction to a scalar equation is not
considered here. However, the reader may refer to
@derouiche_integration_2018 for an example of such a reduction to a
scalar equation may be done in `MFront`.

To solve System @eq:implicit:system:final, one must compute its jacobian
as follows:

\[
\left\{
\begin{aligned}
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel}&=\tenseurq{I}+2\,\mts{\mu}\,\theta\,A\,\paren{\tenseurq{I}-\Frac{1}{3}\tenseur{I}\,\otimes\,\tenseur{I}}\\
\deriv{f_{\tepsilonel}}{\Delta\,e}&=\Frac{1}{3}\,\tenseur{I}+\Delta\,t\,\deriv{A}{\mts{\rho}}\,\deriv{\mts{\rho}}{\Delta\,e}\,\mts{\tenseur{s}}\\
\deriv{f_{e}}{\Delta\,\tepsilonel}&=-3\,\theta\,\Delta\,t\,B\,\paren{3\,\mts{\lambda}+2\,\mts{\mu}}\,\tenseur{I}\\
\deriv{f_{e}}{\Delta\,e}&=1-3\,\Delta\,t\paren{\deriv{B}{\mts{\rho}}\,\trace{\mts{\sigma}}-\deriv{C}{\mts{\rho}}}\,\deriv{\mts{\rho}}{\Delta\,e}\\
\end{aligned}
\right.
\]

where we omitted the dependency of \(A\), \(B\) and \(C\) to \(\mts{\rho}\paren{\Delta\,e}\).

The formal derivatives of \(A\), \(B\) and \(C\) are a bit tiedous and won't be detailled here.

# Implementation

## Choice of domain specific language

While not mandatory (the `@DSL` keyword can be place anywhere in the
file), its is convenient to start the implementation by declaring the
domain specific language to be used. For an integration by a
\(\theta\)-scheme, the `Implicit` domain specific language is choosen:

~~~~{.cxx}
@DSL Implicit;
~~~~

## Name of the behaviour

The `@Behaviour` keyword is used to give the name of the behaviour.

~~~~{.cxx}
@Behaviour sovs;
~~~~

## Metadata

The following instructions give some information about the author, the
date and some description of the behaviour.

~~~~{.cxx}
@Author Joan Balaguer Osuna;
@Date 04/2019;
@Description{
  "Verification of the Skorohod - Olevsky Viscous Sintering(SOVS) Model."
  "Brian Lester"
  "Sandia National Laboritories"
  "November 16, 2017"
}
~~~~

## Numerical parameters

The following instruction changes the default value of the stopping
criterion used by the Newton-Raphson method:

~~~~{.cxx}
@Epsilon 1.e-16;
~~~~

This can be changed at runtime by modifying the `epsilon` parameter.

The default value of the \(\theta\) parameter is left unchanged (its
default value is one half). This can be changed at runtime by modifying
the `theta` parameter.

## Usage of the `StandardElasticity` brick

The implicit scheme used satisfies the requirements of the
`StandardElasticity` brick as described [here](BehaviourBricks.html).

The `StandardElasticity` brick which provides:

- Automatic computation of the stress tensor at various stages of the
  behaviour integration.
- Automatic computation of the consistent tangent operator.
- Automatic support for plane stress and generalized plane stress
  modelling hypotheses (The axial strain is defined as an additional
  state variable and the associated equation in the implicit system is
  added to enforce the plane stess condition).
- Automatic addition of the standard terms associated with the elastic
  strain state variable.
The usage of the `StandardElasticity` is introduced as follows:

~~~~{.cpp}
@Brick StandardElasticity;
~~~~

## Supported modelling hypothesis

Thanks to the `StandardElasticity` brick, all the modelling hypotheses
can be supported. The following statement, starting with the
`@ModellingHypotheses`, enables all the modelling hypotheses:

~~~~{.cpp}
@ModellingHypotheses {".+"};
~~~~

## Elastic properties

~~~~{.cpp}
@ElasticMaterialProperties {123.7e9,0.356};
~~~~

## Material parameters

~~~~{.cxx}
@Parameter a1 = 1;
@Parameter b1 = 2;

@Parameter a2 = 0.666666666666666;
@Parameter b2 = 3;
@Parameter c2 = 1;

@Parameter a3 = 1;
@Parameter b3 = 2;
@Parameter sig_s0 = 3.81e6;

@Parameter eta0_A = 0.0000166;
@Parameter eta0_B = 48259.583;
~~~~

## The relative density as an auxiliary state variable

~~~~{.cxx}
@AuxiliaryStateVariable real r;
r.setEntryName("RelativeDensity");
~~~~

## The trace of the viscoplastic strain as an integration variable

~~~~{.cxx}
//! trace of the viscoplastic strain
@IntegrationVariable strain e;
~~~~

## Local variable

In `MFront`, an integration variable is defined to store a variable and
use it in various code block.

Here a local variable called `eta0` is declared to later computes
\(\eta_{0}\paren{T+\theta\,\Delta\,T}\) once for all.

~~~~{.cxx}
@LocalVariable stress eta0;
~~~~

## Initialisation a the local variable

The `@InitLocalVariables` code block is called before the behaviour
integration. It is used here to compute
\(\eta_{0}\paren{T+\theta\,\Delta\,T}\) once for all:

~~~~{.cxx}
@InitLocalVariables {
  eta0 = eta0_A*exp(eta0_B/(T + theta * dT));
}
~~~~

## Implicit system implementation

The implementation of the implicit system and its derivative is done in
the `@Integrator` code block:

~~~~{.cxx}
@Integrator{
~~~~

First, we define the identity for symmetric second order tensors, as
follows:

~~~~{.cxx}
  constexpr const auto id = StrainStensor::Id();
~~~~

The following lambda function (this function is local to the
`@Integration` code block) will be useful later to computed some of the
derivatives efficiently:

~~~~{.cxx}
  const auto is_zero = [=](const real v) {
    constexpr const real eps0 = 1e-8;
    return (abs(v) - eps0 < 0) ? true : false;
  };
~~~~

An estimation of the relative density \(\mts{\rho}\) is computed from
the current estimation of \(\Delta\,e\):

~~~~{.cxx}
  const auto r_ = r*exp(-theta * de);
~~~~

\(\phi\), \(\psi\) and \(sigma_s\) can now be computed:

~~~~{.cxx}
  const auto phi = a1 * pow(r_, b1);
  const auto psi = a2 * pow(r_, b2) / pow(1 - r_, c2);
  const auto sig_s = sig_s0 * a3 * pow(r_, b3);
~~~~

\(A\), \(B\) and \(C\) can now be computed:

~~~~{.cxx}
  const auto A = 1 / (2 * eta0 * phi);
  const auto B = 1 / (18 * eta0 * psi);
  const auto C = 3 * sig_s * B;
~~~~

To express the viscoplastic flow, one need the deviatoric part of the
stress:

~~~~{.cxx}
  const auto s = deviator(sig);
~~~~

> **Computation of the stress**
>
> `sig` stands for the current estimation of the stress at
> \(t+\theta\,\Delta\,t\) using \(\mts{\tepsilonel}\). It is 
> automatically computed by the `StandardElasticity` brick.

The implicit system is then readily implemented:

~~~~{.cxx}
  feel += de/3 * id + A * s;
  fe   -= 3 * dt * (B * trace(sig) - C);
~~~~

> **Implicit initialisation of the implicit equations**
> 
> This implementation takes into account the fact that the `Implicit` DSL
> automatically initializes `feel` to the current estimation of
> \(\Delta\,\tepsilonel\) and `fe` to the current estimation of
> \(\Delta\,e\). Moreover, the `StandardElasticity` brick automatically
> subtracts \(\Delta\,\tepsilonto\) to `feel`.
>
> The previous lines are thus equivalent to
>
> ~~~~{.cxx}
>   feel = deel - deto + de/3 * id + A * s;
>   fe   = de - 3 * dt * (B * trace(sig) - C);
> ~~~~

We now computes the derivatives of \(A\), \(B\) and \(C\) with respect
to \(\Delta\,e\):

~~~~{.cxx}
  const auto dr__dde = -theta * r_;
  const auto dphi_dr_ = is_zero(r_) ? real(0) : b1 * phi / r_;
  const auto dpsi_dr_ =
      is_zero(r_) ? real(0) : psi * ((c2 - b2) * r_ + b2) / (r_ * (1 - r_));
  const auto dpsi_dde = dpsi_dr_ * dr__dde;
  const auto dphi_dde = dphi_dr_ * dr__dde;
  const auto dA_dphi = -1 / (2 * eta0 * phi * phi);
  const auto dB_dpsi = -1 / (18 * eta0 * psi * psi);
  const auto dA_dde = dA_dphi * dphi_dde;
  const auto dB_dde = dB_dpsi * dpsi_dde;
  const auto dsig_s_dr_ = is_zero(r_) ? real(0) : b3 * sig_s / r_;
  const auto dsig_s_dde = dsig_s_dr_ * dr__dde;
  const auto dC_dde = 3 * sig_s * dB_dde + 3 * B * dsig_s_dde;
~~~~

The termes of the jacobian can now be readily implemented:

~~~~{.cxx}
  dfeel_ddeel += 2 * mu * theta * A * Stensor4::K();
  dfeel_dde    = id / 3 + dA_dde * s;
  dfe_dde     -= 3 * dt * (dB_dde * trace(sig) + dC_dde);
  dfe_ddeel    =  -3 * (3 * lambda + 2 * mu) * theta * B * dt * id;
~~~~

> Implicit initialization of the jacobian blocks
> 
> The `Implicit` DSL automatically initializes the jacobian to the
> identity. The previous lines are thus equivalent to:
> 
> ~~~~{.cxx}
>   dfeel_ddeel = Stensor4::Id() + 2 * mu * theta * A * Stensor4::K();
>   dfeel_dde   = id / 3 + dA_dde * s;
>   dfe_dde     = 1 -3 * dt * (dB_dde * trace(sig) + dC_dde);
>   dfe_ddeel   =  -3 * (3 * lambda + 2 * mu) * theta * B * dt * id;
> ~~~~


The following curly bracket closes the `@Integrator` block:

~~~~{.cxx}
}
~~~~

The relative density is updated after the behaviour integration as
follows:

~~~~{.cxx}
@UpdateAuxiliaryStateVariables {
  r *= exp(-de);
}
~~~~

# References
