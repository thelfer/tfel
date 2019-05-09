% Skorohold-Olevsky Viscous Sintering (SOVS) model
% Balaguer J. (Instituto de Tecnología Cerámica (ITC-AICE), Universitat Jaume I, Castellón (Spain)), Helfer T.
% May 2019

\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\tenseur{\varepsilon}^{\mathrm{el}}}
\newcommand{\tepsilonin}{\tenseur{\varepsilon}^{\mathrm{in}}}
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

This article shows the implementation of an isotropic linear viscous
model based on Skorohold-Olevsky Viscous Sintering (SOVS) model. This
page is inspired by the paper of Lester @Lester2017. For a more
comprenhensive reading about the model, refer to the original paper of
Olevsky @Olevsky1998.

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
\]

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
 \tdepsilonin = \frac{\tenseur{s}}{2\eta_{0}(T)\phi(\rho)} + \frac{\trace{\tsigma}-3\sigma_{s}(\rho)}{18\eta_{0}(T)\psi(\rho)}\tenseur{I}
\]

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

## Evolution of the relative density

Finally, the evolution of the relative density is:

\[
 \dot{\rho}=-\rho \, \, \trace{\tdepsilonin}
\]{#eq:density_evolution}

# Implicit scheme

## State variables

The state variables are:

- the elastic strain \(\tepsilonel\);
- the inelastic strain \(\tepsilonin\);
- the relative density \(\rho\).

## Evolution of the relative density

Equation @eq:density_evolution can be integrated exactly over the time step
as follows:

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

# Implementation

# References