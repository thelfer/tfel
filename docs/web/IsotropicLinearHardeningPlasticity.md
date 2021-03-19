---
title: How to implement an isotropic plastic behaviour with isotropic linear hardening in MFront
author: Thomas Helfer, Lorenzo Riparbelli, Ioannis Christovasilis
date: 24/04/2018
numbersections: true
toc: true
geometry: margin=2cm
bibliography: bibliography.bib
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

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
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sderiv}[2]{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial #2^{2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}

This article shows how to implement an orthotropic plastic behaviour
with isotropic linear hardening in `MFront`.

The implementation is available [here](gallery/plasticity/IsotropicLinearHardeningPlasticity.mfront).

# Description of the behaviour

The behaviour is described by a standard decomposition of the strain
\(\tepsilonto\) in an elastic and a plastic component, respectively
denoted \(\tepsilonel\) and \(\tepsilonp\):

\[
\tepsilonto=\tepsilonel+\tepsilonp
\]{#eq:strain_slip}

## Elastic behaviour

The stress \(\tsigma\) is related to the the elastic strain
\(\tepsilonel\) by a the standard Hooke behaviour:

\[
\tsigma = \lambda\,\trace{\tepsilonel}\,\tenseur{I}+2\,\mu\,\tepsilonel
\]

## Plastic flow

The plastic part of the behaviour is described by the following yield
surface:
\[
f\paren{\sigmaeq,p} = \sigmaeq-\sigma_{0}-R\,p
\]{#eq:yield_surface}

where \(\sigmaeq\) is the vonMises stress defined below, \(p\) is the
equivalent plastic strain, \(\sigma_{0}\) is the yield stress and
\(R\) is the hardening slope.

The von Mises stress is defined by:
\[
\sigmaeq=\sqrt{\Frac{3}{2}\,\tenseur{s}\,\colon\,\tenseur{s}}=\sqrt{3\,J_{2}}
\]
where:

- \(\tenseur{s}\) is the deviatoric stress defined as follows:
\[
\tenseur{s}=\tsigma-\Frac{1}{3}\,\trace{\tsigma}\,\tenseur{I}
\]
- \(J_{2}\) is the second invariant of \(\tenseur{s}\).

The previous expression can be rewritten by introducing a fourth order
tensor called \(\tenseurq{M}\):
\[
\sigmaeq=\sqrt{\sigma\,\colon\,\tenseurq{M}\,\colon\,\tsigma}
\]

The tensor \(\tenseurq{M}\) is given by:
\[
\tenseurq{M}=\Frac{3}{2}\,\paren{\tenseurq{I}-\Frac{1}{3}\,\tenseur{I}\,\otimes\,\tenseur{I}}
\]

The plastic flow is assumed to be associated, so the flow direction
\(\tenseur{n}\) is given by \(\deriv{f}{\tsigma}\):

\[
\tenseur{n} = \deriv{f}{\tsigma} = \Frac{3}{2\,\sigmaeq}\,\tenseur{s}
\]

One may readily check that:

\[
\tenseur{n}\,\colon\tenseur{n}=\Frac{3}{2}
\]{#eq:n_norm}

# Integration algorithm

The previous constitutive equations will be integrated using a standard
implicit scheme which, due to the isotropy of the elastic behaviour, the
quadratic nature of the equivalent stress and the linearity of the
isotropic hardening rule can be solved analytically.

## Expression of the plastic strain increment

Assuming that a plastic flow occurs, the plastic increment
\(\Delta\,\tepsilonp\) over the time step is approximated by:

\[
\Delta\,\tepsilonp = \Delta\,p\,\mts{\tenseur{n}}
\]

The slip of strain given by Equation @eq:strain_slip may be written
incrementally:

\[
\Delta\,\tepsilonto=\Delta\,\tepsilonel+\Delta\,\tepsilonp=\Delta\,\tepsilonel+\Delta\,p\,\mts{\tenseur{n}}
\]{#eq:strain_increment_slip}

and projected on the deviatoric space as follows:

\[
\Delta\,\tenseur{s}^{\mathrm{to}} = \Delta\,\tenseur{s}^{\mathrm{el}}+\Delta\,p\,\mts{\tenseur{n}}
\]

The isotropy of the elastic behaviour projected on the deviatoric space leads to:

\[
\begin{aligned}
\mts{\tenseur{s}} &= \Frac{2}{3}\,\mts{\sigmaeq}\,\mts{\tenseur{n}}\\
                  &= 2\,\mts{\mu}\,\paren{\bts{\tenseur{s}^{\mathrm{el}}}+\theta\,\Delta\,\tenseur{s}^{\mathrm{el}}} \\
                  &= 2\,\mts{\mu}\,\paren{\bts{\tenseur{s}^{\mathrm{el}}}
                                   +\theta\,\Delta\,\tenseur{s}^{\mathrm{to}}
                                   -\theta\,\Delta\,p\,\mts{\tenseur{n}}}
\end{aligned}
\]

The previous equations leads to:

\[
\paren{\Frac{2}{3}\,\mts{\sigmaeq}+\theta\,\Delta\,p}\,\mts{\tenseur{n}} = 2\,\mts{\mu}\,\paren{\bts{\tenseur{s}^{\mathrm{el}}}+\theta\,\Delta\,\tenseur{s}^{\mathrm{to}}}
\]{#eq:trial_stress}

The right hand side is the value of the deviatoric part of the stress
tensor that would be obtained in a pure elastic loading. It is commonly
called the trial stress \(\mts{\tenseur{s}^{\mathrm{tr}}}\):

\[
\mts{\tenseur{s}^{\mathrm{tr}}}=2\,\mts{\mu}\,\paren{\bts{\tenseur{s}^{\mathrm{el}}}+\theta\,\Delta\,\tenseur{s}^{\mathrm{to}}}
\]

Taking the von Mises norm of both side of Equation @eq:trial_stress yields:

\[
\paren{\Frac{2}{3}\,\mts{\sigmaeq}+\theta\,\Delta\,p}\,\sqrt{\Frac{3}{2}\,\mts{\tenseur{n}}\,\colon\mts{\tenseur{n}}} = \mts{\sigmaeq^{\mathrm{tr}}}
\]

where \(\mts{\sigmaeq^{\mathrm{tr}}}\) is the so-called trial equivalent
stress.

It is important to note that \(\tenseur{s}^{\mathrm{tr}}\) and
\(\mts{\sigmaeq^{\mathrm{tr}}}\) can be computed *a priori*, only with the
known values of the elastic strain at the beginning of the time step and
the total strain increment.

The previous expressions leads to a closed form expression of the normal:

\[
\mts{\tenseur{n}} = \Frac{3}{2\,\mts{\sigmaeq^{\mathrm{tr}}}}\tenseur{s}^{\mathrm{tr}}
\]{#eq:normal_value}

Finally, taking into account Equation @eq:n_norm, the von Mises stress satisfies:

\[
\mts{\sigmaeq} = \mts{\sigmaeq^{\mathrm{tr}}} - 3\,\mts{\mu}\,\theta\,\Delta\,p
\]

Using the fact that, during plastic loading, the stress state must be on
the Yield Surface @eq:yield_surface, the von Mises stress also satisfies:

\[
\mts{\sigmaeq}=\sigma_{0}+R\,\mts{p}=\sigma_{0}+R\,\paren{\bts{p}+\theta\,\Delta\,p}
\]

Combining those equations yields this closed-form expression of the
plastic strain increment:

\[
\Delta\,p = \Frac{\mts{\sigmaeq^{\mathrm{tr}}} - \sigma_{0} - R\,\bts{p}}{\paren{3\,\mts{\mu}\, + R}\,\theta}
\]{#eq:dp}

## Expression of the elastic strain increment

The elastic strain increment is given in closed formed by Equations
@eq:strain_increment_slip, @eq:normal_value and @eq:dp:

\[
\Delta\tepsilonel = \Delta\,\tepsilonto-\Delta\,p\,\mts{\tenseur{n}}
\]

## Expression of the consistent tangent operator

# Implementation

We now assume that \(\theta=1\).

~~~~{.cpp}
@DSL DefaultDSL;
@Behaviour IsotropicLinearHardeningPlasticity;
@Author Thomas Helfer;
@Date   14/10/2016;
@Description{
  An implicit implementation of a simple
  isotropic plasticity behaviour with
  isotropic linear hardening.

  The yield surface is defined by:
  "\["
  "  f(\sigmaeq,p) = \sigmaeq-s_{0}-H\,p"
  "\]"
}

@StateVariable StrainStensor eel;
eel.setGlossaryName("ElasticStrain");
@StateVariable strain p;
p.setGlossaryName("EquivalentPlasticStrain");

@Parameter young =  70.e9;
young.setGlossaryName("YoungModulus");
@Parameter nu    =   0.34;
nu.setGlossaryName("PoissonRatio");
@Parameter H     =  10.e9;
H.setEntryName("HardeningSlope");
@Parameter s0    =  300.e6;
s0.setGlossaryName("YieldStress");

/*!
 * computation of the prediction operator: we only provide the elastic
 * operator.
 *
 * We could also provide a tangent operator, but this would mean
 * saving an auxiliary state variable stating if a plastic loading
 * occured at the previous time step.
 */
@PredictionOperator{
  const auto lambda = computeLambda(young,nu);
  const auto mu     = computeMu(young,nu);
  Dt = lambda*Stensor4::IxI()+2*mu*Stensor4::Id();
}

/*!
 * behaviour integration using a fully implicit Euler-backwark scheme.
 */
@ProvidesSymmetricTangentOperator;
@Integrator{
  const auto lambda = computeLambda(young,nu);
  const auto mu     = computeMu(young,nu);
  eel += deto;
  const auto se     = 2*mu*deviator(eel);
  const auto seq_e  = sigmaeq(se);
  const auto b      = seq_e-s0-H*p>stress{0};
  if(b){
    const auto iseq_e = 1/seq_e;
    const auto n      = eval(3*se/(2*seq_e));
    const auto cste   = 1/(H+3*mu);
    dp   = (seq_e-s0-H*p)*cste;
    eel -= dp*n;
    if(computeTangentOperator_){
      if(smt==CONSISTENTTANGENTOPERATOR){
	Dt = (lambda*Stensor4::IxI()+2*mu*Stensor4::Id()
	      -4*mu*mu*(dp*iseq_e*(Stensor4::M()-(n^n))+cste*(n^n)));
      } else {
	Dt = lambda*Stensor4::IxI()+2*mu*Stensor4::Id();
      }
    }
  } else {
    if(computeTangentOperator_){
      Dt = lambda*Stensor4::IxI()+2*mu*Stensor4::Id();
    }
  }
  sig = lambda*trace(eel)*Stensor::Id()+2*mu*eel;
}
~~~~
