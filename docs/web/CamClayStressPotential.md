---
title: Cam-Clay stress potential
author: Thomas Helfer, Christian Silbermann, Miguel Ángel Mánica Malcom
date: 08/03/2022
lang: en-EN
link-citations: true
colorlinks: true
geometry:
  - margin=2cm
papersize: a4
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

\newcommand{\absvalue}[1]{{\left|#1\right|}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tepsilonel}{\tenseur{\varepsilon}^{\mathrm{el}}}
\newcommand{\tdepsilonel}{\tenseur{\dot{\varepsilon}}^{\mathrm{el}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\sigmaH}{\sigma_{H}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}

This document describes the `CamClay` stress potential based on the
`Cam-Clay` non linear elastic behaviours. The `CamClay` stress potential
is the basis of the `CamClayElasticity` brick.

The `CamClay` stress potential provides:

- automatic computation of the consistent tangent operator.
- a method called `computeElasticPrediction`.

# Description

## Hydrostatic pressure

The hydrostatic pressure \(p\) is defined in this document using the
conventions of geomechanics, as:

\[ p=-\Frac{1}{3}\,\trace{\tsigma} \]

## Hydrostatic pressure dependant bulk modulus

The bulk modulus is defined as a coefficient of proportionality between
the hydrostatic pressure rate \(\dot{p}\) and the trace of the elastic
strain rate \(\dot{e}\):
\[
\dot{p} = -K\,\trace{\tdepsilonel}
\]{#eq:mfront:camclay:dp_de}
with \(e=\trace{\tepsilonel}\)

The bulk modulus \(K\) is assumed dependant of the hydrostatic pressure as
follows:
\[
K\paren{p} = \Frac{1+e_{0}}{\kappa}\,p
\]{#eq:mfront:camclay:bulk_modulus}
where the hydrostatic pressure is assumed strictly positive and where
\(e_{0}\) and \(\kappa\) denotes repectively the initial void ratio and
the unloading/reloading slope of the material.

## Hydrostatic pressure threshold

Equation @eq:mfront:camclay:bulk_modulus is only valid if the the
hydrostatic pressure is strictly positive. To ensure this, a threshold
\(p_{\min}\) is introduced.

Below this threshold, the behaviour is assumed linear, as follows:
\[
\dot{p} = -K_{\min}\,\trace{\tdepsilonel}
\]{#eq:mfront:camclay:dp_de:2}
where \(K_{\min}\) is defined as:
\[
K_{\min} = \Frac{1+e_{0}}{\kappa}\,p_{\min}
\]
which ensures that the hydrostatic pressure rate is continuous at
\(p=p_{\min}\).

## Integral formulation

The hypoelastic formulation of the behaviour is cumbersome, in practice.
However, Equations @eq:mfront:camclay:dp_de and
@eq:mfront:camclay:dp_de:2 can be integrated explicitly.

*Assuming that the material coefficients \(e_{0}\), \(\kappa\) and
\(p_{\min}\) are constants*, Equation @eq:mfront:camclay:dp_de:2 can
be integrated as follows:
\[
p = -K_{\min}\,\trace{\tepsilonel}
\]{#eq:mfront:camclay:p1}
where we also assumed that the trace of the elastic strain is null
for a null hydrostatic pressure

Expression @eq:mfront:camclay:p1 is valid if the pressure is below
\(p_{\min}\), i.e. if the trace of the elastic strain satisfies:
\[
\Frac{1+e_{0}}{\kappa}\,\trace{\tepsilonel} \geq -1
\]{#eq:mfront:camclay:tr_eel_threshold}

If Condition @eq:mfront:camclay:tr_eel_threshold is not met, Equation
@eq:mfront:camclay:dp_de, combined with Equation
@eq:mfront:camclay:bulk_modulus, can be integrated explicitly, which
leads to this closed-form expression of the hydrostatic pressure:
\[
p = p_{\min}\,\exp\paren{-1 -  \Frac{1+e_{0}}{\kappa}\, \trace{\tepsilonel}}
\]{#eq:mfront:camclay:p2}

Equations @eq:mfront:camclay:p1 and @eq:mfront:camclay:p2 show that a
convex free energy \(\phi^{p}\paren{\trace{\tepsilonel}}\) can be
defined such that:
\[
p = -\deriv{\phi^{p}}{\trace{\tepsilonel}}
\]

Moreover, in the authors opinion, it makes senses to use Equations
@eq:mfront:camclay:p1 and @eq:mfront:camclay:p2 as the starting point
for a proper definition of the Cam-Clay non linear elastic behaviour as
it allows to introduce dependencies of the material coefficients
\(e_{0}\) and \(\kappa\) with respect to external state variables, such
as the temperature.

## Deviatoric part

The treatment of the deviatoric part of the Cam-Clay non linear elastic
behaviour is given by the following linear relation between the rate of
the deviatoric stress \(\tenseur{\dot{s}}\) and the rate of the
deviatoric part of the elastic strain \(\tenseur{\dot{s}}^{el}\):
\[
\tenseur{\dot{s}} = 2\,\mu\,\tenseur{\dot{s}}^{el}
\]{#eq:mfront:camclay:shear_rate_behaviour}
with \(\tenseur{s}=\tsigma+p\,\tenseur{I}\) and
\(\tenseur{s}^{el}=\tepsilonel-\trace{\tepsilonel}\,\tenseur{I}\). 

Two choices are made in the litterature:

- **Case 1**: \(\mu\) is assumed independant of the stress state and can
  thus be identifier to the shear modulus of the Hooke law.
- **Case 2**: \(\mu\) is assumed to be related to a poisson ratio \(\nu\)
  and the bulk modulus \(K\) using the classical relationship of linear
  elasticity:
  \[
  \mu = \Frac{3\,\paren{1 - 2 \, \nu}}{2\,\paren{1 + \`nu}}\,K
  \]{#eq:mfront:camclay:mu_K_nu}

### Case 1

If the shear modulus is assumed constant, Expression
@eq:mfront:camclay:shear_behaviour can be integrated as follows:
\[
\tenseur{s} = 2\,\mu\,\tenseur{s}^{el}
\]{#eq:mfront:camclay:shear_behaviour}

With this assumption, the whole behaviour can be derived from a convex
free energy.

Equation @eq:mfront:camclay:shear_behaviour could indeed be the starting
point of a proper definition of the Cam-Clay non linear elastic
behaviour. Moreover, this definition can be used if the shear modulus
depends on external state variables surch as the temperature.

However, this choice may lead to unrealistic apparent Poisson ratio.

### Case 2: \(\mu = \Frac{3\,\paren{1 - 2 \, \nu}}{2\,\paren{1 + \nu}}\,K\)

Using Expression @eq:mfront:camclay:mu_K_nu solve the issue of the
unrealistic apparent Poisson ratio.

It however leads to major issue regarding thermodynamic requirements as
the behaviour can not be derived from a free energy. As a consequence,
the derivative \(\deriv{\sigma}{\tepsilonel}\) is not symmetric.

While Expression @eq:mfront:camclay:shear_rate_behaviour seems to be the
most used in the litterature, the authors would also also highlight that
there is no major reason to prefer using Equation
@eq:mfront:camclay:shear_behaviour and Equation
@eq:mfront:camclay:shear_rate_behaviour.

The user of the `CamClay` stress potential can select one of this two
Equations using the `incremental_deviatoric_part` option. By default,
Equation @eq:mfront:camclay:shear_rate_behaviour is used.

#### Treatment of Equation @eq:mfront:camclay:shear_rate_behaviour

In pratice, Equation @eq:mfront:camclay:shear_behaviour is treated
incrementally as follows:
\[
\Delta\,\tenseur{s}=2\,\mu\paren{\ets{p}}\,\Delta\,\tepsilonel
\]
and the stress tensor at the end of the time step is computed as:
\[
\ets{\tsigma}=\bts{\tenseur{s}}+2\,\mu\paren{\ets{p}}\,\Delta\,\tenseur{s}^{el}-p\,\tenseur{I}
\]

Note that even the deviatoric part is treated incrementally, the
hydrostatic part is treated using @eq:mfront:camclay:p1 and
@eq:mfront:camclay:p2.

The derivative \(\deriv{\ets{\tsigma}}{\Delta\,\ets{\tepsilonel}}\) is thus given by:
\[
\deriv{\ets{\tsigma}}{\ets{\tepsilonel}}=
2\,\mu\paren{\ets{p}}\,\tenseurq{K}+
2\,\deriv{\mu}{\ets{\trace{\tepsilonel}}}\,
\paren{\Delta\,\tenseur{s}^{el}\,\otimes\,\tenseur{I}}
-\deriv{p}{\ets{\trace{\tepsilonel}}}\,\tenseur{I}\otimes\tenseur{I}
\]
which is non symmetric du to the term propotionnal to
\(\Delta\,\tenseur{s}^{el}\,\otimes\,\tenseur{I}\).

#### Treatment of Equation @eq:mfront:camclay:shear_behaviour

The derivative \(\deriv{\ets{\tsigma}}{\ets{\tepsilonel}}\) is thus given by:
\[
\deriv{\ets{\tsigma}}{\ets{\tepsilonel}}=
2\,\mu\,\tenseurq{K}+
2\,\deriv{\mu}{\ets{\trace{\tepsilonel}}}\,
\paren{\ets{\tenseur{s}^{el}}\,\otimes\,\tenseur{I}}
-\deriv{p}{\ets{\trace{\tepsilonel}}}\,\tenseur{I}\otimes\tenseur{I}
\]

# Options of the Cam-Clay stress potential

The following options are available:

- `unloading_reloading_slope`: unloading/reloading slope \(\kappa\).
- `initial_void_ratio`: initial void ratio \(e0\).
- `poisson_ratio`: Poisson ratio of the material \(\nu\).
- `shear_modulus`: shear modulus of the material \(\mu\). 
- `pressure_threshold`: threshold below which the behaviour is linear
  elastic. The value of this threshold must be. strictly positive.
- `deduce_shear_modulus_from_poisson_ratio`: boolean stating if the
  shear modulus shall be deduced from the Poisson ratio (Case 2). If
  this option is true, the Poisson ratio is automatically declared as a
  material property. Otherwise, the shear modulus is automatically
  declared as a material property.
- `incremental_deviatoric_part`: boolean stating if the deviatoric part
  of the stress/strain relationship shall be treated incrementally. This
  option is only valid if the shear modulus is deduced from the Poisson
  ratio. In this case, an incremental treatment given by Equation
  @eq:mfront:camclay:shear_rate_behaviour is assumed by default. If
  false, Equation @eq:mfront:camclay:shear_behaviour is used.`

The options `poisson_ratio`, `shear_modulus` and
`deduce_shear_modulus_from_poisson_ratio` are mutually exclusive.

The `incremental_deviatoric_part` is not compatible with the
`shear_modulus` option.

# Examples of usage

## The `CamClayElasticity` brick

~~~~{.cxx}
@Brick CamClayElasticity{
  unloading_reloading_slope : 0.01,
  initial_void_ratio : 0.5,
  poisson_ratio : 0.3,
  pressure_threshold : 1
};
~~~~~

## Usage in the `StandardElastoViscoPlasticity` brick

~~~~{.cxx}
@Brick StandardElastoViscoPlasticity{
   stress_potential : "CamClay" {
   unloading_reloading_slope : 0.01,
   initial_void_ratio : 0.5,
   poisson_ratio : 0.3,
   pressure_threshold : 1
 }
};
~~~~~

