---
title: How to implement an isotropic plastic behaviour with isotropic linear hardening in MFront
author: Thomas Helfer, Lorenzo Riparbelli, Ioannis Christovasilis
date: 24/04/2018
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
\]{#eq:isotropic_linear_harderning:strain_slip}

## Elastic behaviour

The stress \(\tsigma\) is related to the the elastic strain
\(\tepsilonel\) by a the standard Hooke behaviour:

\[
\tsigma = \lambda\,\trace{\tepsilonel}\,\tenseur{I}+2\,\mu\,\tepsilonel
\]{#eq:isotropic_linear_harderning:hooke}

## Plastic flow

The plastic part of the behaviour is described by the following yield
surface:
\[
f\paren{\sigmaeq,p} = \sigmaeq-\sigma_{0}-R\,p
\]{#eq:isotropic_linear_harderning:yield_surface}

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
\]{#eq:isotropic_linear_harderning:n_norm}

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

The slip of strain given by Equation @eq:isotropic_linear_harderning:strain_slip may be written
incrementally:

\[
\Delta\,\tepsilonto=\Delta\,\tepsilonel+\Delta\,\tepsilonp=\Delta\,\tepsilonel+\Delta\,p\,\mts{\tenseur{n}}
\]{#eq:isotropic_linear_harderning:strain_increment_slip}

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
\]{#eq:isotropic_linear_harderning:trial_stress}

The right hand side is the value of the deviatoric part of the stress
tensor that would be obtained in a pure elastic loading. The latter is
commonly called the trial stress \(\tsigma^{\mathrm{tr}}\):

\[
\tsigma^{\mathrm{tr}}=
\mts{\lambda}\,\trace{\bts{\tepsilonel}+\theta\,\Delta\,\tepsilonto}\,\tenseur{I}+
2\,\mts{\mu}\,\paren{\bts{\tepsilonel}+\theta\,\Delta\,\tepsilonto}
\]

Its deviatoric part is denoted \(\mts{\tenseur{s}^{\mathrm{tr}}}\):

\[
\mts{\tenseur{s}^{\mathrm{tr}}}=2\,\mts{\mu}\,\paren{\bts{\tenseur{s}^{\mathrm{el}}}+\theta\,\Delta\,\tenseur{s}^{\mathrm{to}}}
\]

Taking the von Mises norm of both side of Equation @eq:isotropic_linear_harderning:trial_stress yields:

\[
\paren{\Frac{2}{3}\,\mts{\sigmaeq}+\theta\,\Delta\,p}\,\sqrt{\Frac{3}{2}\,\mts{\tenseur{n}}\,\colon\mts{\tenseur{n}}} = \mts{\sigmaeq^{\mathrm{tr}}}
\]

where \(\mts{\sigmaeq^{\mathrm{tr}}}\) is the so-called trial equivalent
stress.

It is important to note that \(\tenseur{s}^{\mathrm{tr}}\) and
\(\mts{\sigmaeq^{\mathrm{tr}}}\) can be computed *a priori*, only with the
known values of the elastic strain at the beginning of the time step and
the total strain increment.

The previous expressions leads to a closed form expression of the
normal:

\[
\mts{\tenseur{n}} = \Frac{3}{2\,\mts{\sigmaeq^{\mathrm{tr}}}}\tenseur{s}^{\mathrm{tr}}
\]{#eq:isotropic_linear_harderning:normal_value}

> **Derivatives of the equivalent trial stress \(\mts{\sigmaeq^{\mathrm{tr}}}\) and normal \(\mts{\tenseur{n}}\)**
>
> The following classical relationships will be useful in the following:
>
> \[
> \left\{
> \begin{aligned}
> \deriv{\mts{\sigmaeq^{\mathrm{tr}}}}{\mts{\tsigma^{\mathrm{tr}}}}&=\mts{\tenseur{n}}\\
> \deriv{\mts{\tenseur{n}}}{\mts{\tsigma^{\mathrm{tr}}}}&=
> \Frac{1}{\mts{\sigmaeq^{\mathrm{tr}}}}
> \paren{\tenseurq{M}-\mts{\tenseur{n}}\,\otimes\,\mts{\tenseur{n}}}\\
> \end{aligned}
> \right.
> \]{#eq:isotropic_linear_harderning:useful_derivatives}

Finally, taking into account Equation
@eq:isotropic_linear_harderning:n_norm, the von Mises stress satisfies:

\[
\mts{\sigmaeq} = \mts{\sigmaeq^{\mathrm{tr}}} - 3\,\mts{\mu}\,\theta\,\Delta\,p
\]

Using the fact that, during plastic loading, the stress state must be on
the Yield Surface @eq:isotropic_linear_harderning:yield_surface, the von Mises stress also satisfies:

\[
\mts{\sigmaeq}=\sigma_{0}+R\,\mts{p}=\sigma_{0}+R\,\paren{\bts{p}+\theta\,\Delta\,p}
\]

Combining those equations yields this closed-form expression of the
plastic strain increment:

\[
\Delta\,p = \Frac{\mts{\sigmaeq^{\mathrm{tr}}} - \sigma_{0} - R\,\bts{p}}{\paren{3\,\mts{\mu}\, + R}\,\theta}
\]{#eq:isotropic_linear_harderning:dp}

## Expression of the elastic strain increment

The elastic strain increment is given in closed form by Equations
@eq:isotropic_linear_harderning:strain_increment_slip,
@eq:isotropic_linear_harderning:normal_value and
@eq:isotropic_linear_harderning:dp:

\[
\Delta\tepsilonel = \Delta\,\tepsilonto-\Delta\,p\,\mts{\tenseur{n}}
\]{#eq:isotropic_linear_harderning:deel}

## Expression of the consistent tangent operator

The consistent tangent operator is the derivative
\(\deriv{\ets{\tsigma}}{\Delta\,\tepsilonto}\) of the stress at the end
of the time step with respect the increment of the strain.

Using Equation @eq:isotropic_linear_harderning:hooke, this derivative
can be expressed as:

\[
\deriv{\ets{\tsigma}}{\Delta\,\tepsilonto}=
\ets{\tenseurq{D}}\,\colon\,\deriv{\ets{\tepsilonel}}{\Delta\,\tepsilonto}=
\ets{\tenseurq{D}}\,\colon\,\deriv{\Delta\,\tepsilonel}{\Delta\,\tepsilonto}
\]

where
\(\ets{\tenseurq{D}}=\ets{\lambda}\,\,\tenseurq{I}+2\,\ets{\mu}\,\tenseur{I}\,\otimes\,\tenseur{I}\)
denotes the stiffness matrix.

The derivative \(\deriv{\Delta\,\tepsilonel}{\Delta\,\tepsilonto}\) can
be deduced from Equation @eq:isotropic_linear_harderning:deel:

\[
\deriv{\Delta\,\tepsilonel}{\Delta\,\tepsilonto}=
\tenseurq{I}
-\mts{\tenseur{n}}\,\otimes\,\deriv{\Delta\,p}{\Delta\,\tepsilonto}
-\Delta\,p\,\deriv{\mts{\tenseur{n}}}{\Delta\,\tepsilonto}
\]

Using Equation @eq:isotropic_linear_harderning:useful_derivatives, the
derivative \(\deriv{\Delta\,p}{\Delta\,\tepsilonto}\) can be deduced
from Equation @eq:isotropic_linear_harderning:dp as follows:

\[
\deriv{\Delta\,p}{\Delta\,\tepsilonto}=
\Frac{1}{\paren{3\,\mts{\mu}\, + R}\,\theta}\,
\deriv{
\mts{\sigmaeq^{\mathrm{tr}}}
}{\Delta\,\tepsilonto}
=
\Frac{2\,\mts{\mu}}{3\,\mts{\mu}\, + R}\,\mts{\tenseur{n}}
\]{#eq:isotropic_linear_harderning:ddp_ddeto}

Using Equations @eq:isotropic_linear_harderning:useful_derivatives and
@eq:isotropic_linear_harderning:normal_value, the derivative
\(\deriv{\mts{\tenseur{n}}}{\Delta\,\tepsilonto}\) is also given by:

\[
\deriv{\mts{\tenseur{n}}}{\Delta\,\tepsilonto}=
\Frac{2\,\mts{\mu}\,\theta}{\mts{\sigmaeq^{\mathrm{tr}}}}
\paren{\tenseurq{M}-\mts{\tenseur{n}}\,\otimes\,\mts{\tenseur{n}}}
\]{#eq:isotropic_linear_harderning:dn_ddeto}

Equations @eq:isotropic_linear_harderning:ddp_ddeto and
@eq:isotropic_linear_harderning:dn_ddeto, shows that the derivatives
\(\deriv{\Delta\,p}{\Delta\,\tepsilonto}\) and
\(\deriv{\mts{\tenseur{n}}}{\Delta\,\tepsilonto}\) are linear
combinations of the two fourth order tensors \(\tenseur{M}\) and
\(\mts{\tenseur{n}}\,\otimes\,\mts{\tenseur{n}}\) which only acts on the
deviatoric parts of the symmetric tensors. The following properites then holds:

\[
\left\{
\begin{aligned}
\ets{\tenseurq{D}}\,\colon\,\tenseurq{M} &= 2\,\ets{\mu}\,\tenseurq{M}\\
\ets{\tenseurq{D}}\,\colon\,\mts{\tenseur{n}}\,\otimes\,\mts{\tenseur{n}} &= 2\,\ets{\mu}\,\mts{\tenseur{n}}\,\otimes\,\mts{\tenseur{n}}\\
\end{aligned}
\right.
\]

The consistent tangent operator is then:

\[
\begin{aligned}
\deriv{\ets{\tsigma}}{\Delta\,\tepsilonto}
&=
\ets{\tenseurq{D}}
-\ets{\tenseurq{D}}\,\colon\,\paren{
\mts{\tenseur{n}}\,\otimes\,\deriv{\Delta\,p}{\Delta\,\tepsilonto}
+\Delta\,p\,\deriv{\mts{\tenseur{n}}}{\Delta\,\tepsilonto}
}\\
&=\ets{\tenseurq{D}}
-2\,\mts{\mu}\,\paren{
\mts{\tenseur{n}}\,\otimes\,\deriv{\Delta\,p}{\Delta\,\tepsilonto}
+\Delta\,p\,\deriv{\mts{\tenseur{n}}}{\Delta\,\tepsilonto}
}
\end{aligned}
\]

and finaly,

\[
\begin{aligned}
\deriv{\ets{\tsigma}}{\Delta\,\tepsilonto}
=\ets{\tenseurq{D}}
-4\,\paren{\mts{\mu}}^{2}\,
&\left(
\Frac{1}{3\,\mts{\mu}\, + R}\,\mts{\tenseur{n}} \otimes\,\mts{\tenseur{n}}+\right.\\
&\left.\Frac{\theta\Delta\,p}{\mts{\sigmaeq^{\mathrm{tr}}}}\,\paren{\tenseurq{M}-\mts{\tenseur{n}}\,\otimes\,\mts{\tenseur{n}}}\right)\\
\end{aligned}
\]{#eq:isotropic_linear_harderning:consistent_tangent_operator}

# Fully implicit implementation

We now describe how to make a fully implicit implementation, i.e. we fix
the \(\theta\) parameter to \(1\).

This choice is consistent with the description of a plastic flow as it
imposes that the material lies on the yield surface at the end of the
time step if a plastic loading occurs.

With other choices, the material may be outside the yield surface at the
end of the time step and spurious oscillations in the stress in time can
be observed, for example during a simple uniaxial tensile test test.

## Choice of the domain specific language

The behaviour integration is trivial as all the computated quantities
can be obtained thanks in closed form:

- The values of the state variables \(\tepsilonel\), \(p\) and the value
  of the stress \(\tsigma\) at the end of the time step.
- The consistent tangent operator
  \(\deriv{\tsigma}{\Delta\,\tepsilonto}\).

In this case, an appropriate domain specific language is the
`DefaultDSL`:

~~~~{.cxx}
@DSL DefaultDSL;
~~~~

Contrary to other DSLs, the `DefaultDSL` does not provide a built-in
support for a given integration scheme.

## Name of the behaviour

The name of the behaviour is introduced by the `@Behaviour` keyword:

~~~~{.cxx}
@Behaviour IsotropicLinearHardeningPlasticity;
~~~~

## Metadata

The following lines defines the author of the implementation, the date
and provides a small description:

~~~~{.cxx}
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
~~~~

## Declaration of state parameters

The behaviour used two state variables, the elastic strain
\(\tepsilonel\) and the equivalent plastic strain \(p\).

Those state variables are declared as follows:

~~~~{.cxx}
@StateVariable StrainStensor eel;
eel.setGlossaryName("ElasticStrain");
@StateVariable strain p;
p.setGlossaryName("EquivalentPlasticStrain");
~~~~

The state variables have been associated to [glossary
names](glossary.html) which are meant to:

- automatically declare the external name of the variable (i.e. the name
  as seen from the calling solver if this solver uses the metadata
  exported by `MFront`).
- automatically document the variable.

> **Increments of the state variables**
> 
> The `DefaultDSL` automatically declares the increments of those
> variables, called respectively `deel` and `dp`. The user is free to use
> those increments or to directly update the values of the state variables.
> 
> If the increment of a state variable is defined, it is added to the
> state variable **after** the `@Integrator` code block.
>
> In our implementation, we will directly update the elastic strain and
> use the plastic strain increment.

## Declaration of parameters

In this implementation, we choose to use parameters associated
respectively with the Young' modulus, Poisson' ratio, hardening slope
and yield stress, as follows:

~~~~{.cxx}
@Parameter stress young =  70.e9;
young.setGlossaryName("YoungModulus");
@Parameter real nu =   0.34;
nu.setGlossaryName("PoissonRatio");
@Parameter stress H =  10.e9;
H.setEntryName("HardeningSlope");
@Parameter stress s0 =  300.e6;
s0.setGlossaryName("YieldStress");
~~~~

> **Using material properties**
> 
> The use of parameters could be replaced by the use of material
> properties as follows:
>
> ~~~~{.cxx}
> @MaterialProperty stress young;
> young.setGlossaryName("YoungModulus");
> @MaterialProperty real nu;
> nu.setGlossaryName("PoissonRatio");
> @MaterialProperty stress H;
> H.setEntryName("HardeningSlope");
> @MaterialProperty stress s0;
> s0.setGlossaryName("YieldStress");
> ~~~~
>
> Contrary to parameters, material properties does not have default
> values and must be provided by the calling solvers, which generally
> uses values provided by the user in its input file.

## Computation of the prediction operator

The current implementation only provides the elastic operator:

~~~~{.cxx}
@PredictionOperator{
  const auto lambda = computeLambda(young, nu);
  const auto mu     = computeMu(young, nu);
  Dt = lambda * Stensor4::IxI() + 2 * mu * Stensor4::Id();
}
~~~~

We could also provide a tangent operator, but this would mean
saving an auxiliary state variable stating if a plastic loading
occured at the previous time step.

## Behaviour integration

The behaviour integration is implemented in the `@Integrator` code
block.

### Computation of the consistent tangent operator

The `DefaultDSL` DSL allows to compute the consistent tangent operator
inside the `@Integrator` code block or in a dedicated code block name
`@TangentOperator`.

The first choice is simplier and more efficient for the considered
implementation, but this requires to explicitely state that the
consistent tangent operator is defined in the `@Integrator` code block
using the `@ProvidesSymmetricTangentOperator` keyword, as follows:

~~~~{.cxx}
@ProvidesSymmetricTangentOperator;
~~~~

The consistent tangent operator may or may not be requested by the
calling solver at runtime.

### Computation of the Lamé' coefficients

The `@Integrator` code blocks begins by computing the Lamé' coefficients
as follows:

~~~~{.cxx}
@Integrator{
  const auto lambda = computeLambda(young, nu);
  const auto mu     = computeMu(young, nu);
~~~~

### Elastic laoding

The next lines assume a purely elastic loading and:

- Updates the value of the elastic strain with the increment of the
  total strain.
- Initializes the value of the consistent tangent operator to the
  elastic stiffness matrix.

~~~~{.cxx}
  eel += deto;
  if(computeTangentOperator_){
    Dt = lambda * Stensor4::IxI() + 2 * mu * Stensor4::Id();
  }
~~~~

If required, a plastic correction will be applied to the elastic strain
and the consistent tangent operator in the next part of the code.

### Computation of the trial stress

To check of if a plastic correction must be applied, we first compute
the deviator of the trial stress, the equivalent trial stress and checks
if its value is outside the elastic domain at the beginning of the time
step.

~~~~{.cxx}
  const auto se     = 2 * mu * deviator(eel);
  const auto seq_e  = sigmaeq(se);
  const auto plastic_loading = seq_e > s0 + H * p;
~~~~

The `plastic_loading` boolean value is thus `true` if a plastic
correction must be applied, `false` otherwise.

### Plastic loading

#### Computation of the normal

If a plastic correction must be applied, we first compute the normal
using Equation @eq:isotropic_linear_harderning:normal_value:

~~~~{.cxx}
  if(plastic_loading){
    const auto iseq_e = 1 / seq_e;
    const auto n      = eval(3 * se / (2 * seq_e));
~~~~

#### Update of the state variables

The increment of the equivalent plastic strain is computed using
Equation @eq:isotropic_linear_harderning:dp:

~~~~{.cxx}
    dp   = (seq_e - s0 - H * p) * cste;
~~~~

The value of the equivalent plastic strain will be updated automatically
after the `@Integrator` code block.

Once the equivalent plastic strain is known, the elastic strain can be
correction using the split of the strain (Equation
@eq:isotropic_linear_harderning:strain_slip):

~~~~{.cxx}
    eel -= dp * n;
~~~~

#### Update of the consistent tangent operator

The consistent tangent operator can be correction using
Equation @eq:isotropic_linear_harderning:consistent_tangent_operator:

~~~~{.cxx}
    if((computeTangentOperator_) && (smt==CONSISTENTTANGENTOPERATOR)){
      Dt -= -4 * mu * mu * (dp * iseq_e * (Stensor4::M()-(n ^ n)) + cste * (n ^ n));
    }
  } // end of if(plastic_loading)
~~~~

#### Computation of the stress at the end of the time step

At this stage, the elastic strain have been updated and hols its value
at the end of the time step. The last step of the implementation is to
use it to compute the stress at the end of the time step:

~~~~{.cxx}
  sig = lambda*trace(eel)*Stensor::Id()+2*mu*eel;
} // end of @Integrator
~~~~
