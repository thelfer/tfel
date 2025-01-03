---
title: Description of the `StandardElastoViscoPlasticity` brick
author: Thomas Helfer
date: 2020
lang: en-EN
documentclass: article
from: markdown+tex_math_single_backslash
geometry:
  - margin=2cm
papersize: a4
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tabPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
bibliography: bibliography.bib
---

\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\mathbf{#1}}}
\newcommand{\tns}[1]{{\underset{\tilde{}}{\mathbf{#1}}}}
\newcommand{\transpose}[1]{{#1^{\mathop{T}}}}
\newcommand{\absvalue}[1]{{\left|#1\right|}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}

\newcommand{\epsilonth}{\epsilon^{\mathrm{th}}}

\newcommand{\tepsilonto}{\underline{\epsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\underline{\epsilon}^{\mathrm{el}}}
\newcommand{\tepsilonp}{\underline{\epsilon}^{\mathrm{p}}}
\newcommand{\tepsilonvp}{\underline{\epsilon}^{\mathrm{vp}}}
\newcommand{\tepsilonth}{\underline{\epsilon}^{\mathrm{th}}}

\newcommand{\tepsilonvis}{\underline{\epsilon}^{\mathrm{vis}}}
\newcommand{\tdepsilonvis}{\underline{\dot{\epsilon}}^{\mathrm{vis}}}
\newcommand{\tdepsilonp}{\underline{\dot{\epsilon}}^{\mathrm{p}}}

\newcommand{\talpha}{\underline{\alpha}}
\newcommand{\tdalpha}{\underline{\dot{\alpha}}}
\newcommand{\txi}{\underline{\xi}}
\newcommand{\tdxi}{\underline{\dot{\xi}}}

\newcommand{\tDq}{{\underline{\mathbf{D}}}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\deriv}[2]{{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\nom}[1]{\textsc{#1}}
\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}
\newcommand{\norm}[1]{\lVert #1\rVert}

This page describes the `StandardElastoViscoPlasticity` brick. This
brick is used to describe a specific class of strain based behaviours
based on an additive split of the total strain \(\tepsilonto\) into an
elastic part \(\tepsilonel\) and one or several inelastic strains
describing plastic (time-independent) flows and/or viscoplastic
(time-dependent) flows:
\[
\tepsilonto=\tepsilonel
+\sum_{i_{\mathrm{p}}=0}^{n_{\mathrm{p}}}\tepsilonp_{i_{\mathrm{p}}}
+\sum_{i_{\mathrm{vp}}=0}^{n_{\mathrm{vp}}}\tepsilonvp_{i_{\mathrm{vp}}}
\]

This equation defines the equation associated with the elastic strain
\(\tepsilonel\).

The brick decomposes the behaviour into two components:

- the stress potential which defines the relation between the elastic
  strain \(\tepsilonel\) and possibly some damage variables and the
  stress measure \(\tsigma\). As the definition of the elastic
  properties can be part of the definition of the stress potential, the
  thermal expansion coefficients can also be defined in the block
  corresponding to the stress potential.
- a list of inelastic flows.


> **Porous viscoplasticity**
>
> This page only introduces stress criteria which are not coupled with
> the evolution of the porosity. Stress criteria coupled with the
> evolution of porosity is described on a [dedicated
> page](StandardElastoViscoPlasticityBrick-PorousPlasticity.html).

## A detailled Example

~~~~{.cpp}
@Brick "StandardElastoViscoPlasticity" {
  // Here the stress potential is given by the Hooke law. We define:
  // - the elastic properties (Young modulus and Poisson ratio).
  //   Here the Young modulus is a function of the temperature.
  //   The Poisson ratio is constant.
  // - the thermal expansion coefficient
  // - the reference temperature for the thermal expansion
  stress_potential : "Hooke" {
    young_modulus : "2.e5 - (1.e5*((T - 100.)/960.)**2)",
    poisson_ratio : 0.3,
    thermal_expansion : "1.e-5 + (1.e-5  * ((T - 100.)/960.) ** 4)",
    thermal_expansion_reference_temperature : 0
  },
  // Here we define only one viscplastic flow defined by the Norton law,
  // which is based:
  // - the von Mises stress criterion
  // - one isotorpic hardening rule based on Voce formalism
  // - one kinematic hardening rule following the Armstrong-Frederick law
  inelastic_flow : "Norton" {
    criterion : "Mises",
    isotropic_hardening : "Voce" {R0 : 200, Rinf : 100, b : 20},
    kinematic_hardening : "Armstrong-Frederick" {
      C : "1.e6 - 98500 * (T - 100) / 96",
      D : "5000 - 5* (T - 100)"
    },
    K : "(4200. * (T + 20.) - 3. * (T + 20.0)**2)/4900.",
    n : "7. - (T - 100.) / 160.",
    Ksf : 3
  }
};
~~~~

# List of available stress potentials

![Relations between stress potentials. The stress potentials usable by the end users are marked in red.](img/StressPotentials.svg
 "Relations between stress potentials")

## The Hooke stress potential

This stress potential implements the Hooke law, i.e. a linear relation
between the elastic strain and the stress, as follows:

\[
 \tsigma=\tenseurq{D}\,\colon\,\tepsilonel
\]
where \(\tenseurq{D}\) is the elastic stiffness tensor.

This stress potential applies to isotropic and orthotropic materials.
This stress potential provides:

- Automatic computation of the stress tensor at various stages of the
  behaviour integration.
- Automatic computation of the consistent tangent operator.
- Automatic support for plane stress and generalized plane stress
  modelling hypotheses (The axial strain is defined as an additional
  state variable and the associated equation in the implicit system is
  added to enforce the plane stess condition).
- Automatic addition of the standard terms associated with the elastic
  strain state variable.

The Hooke stress potential is fully described
[here](HookeStressPotential.html).

## The `IsotropicDamage` stress potential

This stress potential adds to the Hooke stress potential the description
of an isotropioc damage. The relation 
\[
 \tsigma=\left(1-d\right)\,\tenseurq{D}\,\colon\,\tepsilonel
\]
where \(\tenseurq{D}\) is the elastic stiffness tensor and \(d\) is the
isotropic damage variable.

This stress potential inherits all the features and options provided by
the Hooke stress potential. The Hooke stress potential is fully
described [here](HookeStressPotential.html).

## The `DDIF2` stress potential

The `DDIF2` behaviour is used to describe the brittle nature of nuclear
fuel ceramics and is usually coupled with a description of the
viscoplasticity of those ceramics (See for example
[@monerie_overall_2006,@salvo_experimental_2015;@salvo_experimental_2015-1]).

This stress potential adds to the Hooke stress potential the description
of cracking through an additional strain. As such, it inherits all the
features provided by the Hooke stress potential.

The Hooke stress potential is fully described
[here](HookeStressPotential.html).

The DDIF2 stress potential is fully described
[here](DDIF2StressPotential.html).


# Inelastic flows

## List of available inelastic flows

### The `Plastic` inelastic flow

The plastic flow is defined by:

- a yield surface \(f\)
- a plastic potential \(g\)

The plastic strain rate satisfies:
\[
\tdepsilonp=\dot{\lambda}\,\deriv{g}{\tsigma}
\]

The plastic multiplier satifies the Kuhn-Tucker relation:
\[
\left\{
\begin{aligned}
\dot{\lambda}\,f\paren{\tsigma,p}&=0\\
\dot{\lambda}&\geq 0
\end{aligned}
\right.
\]

The flow is associated is \(f\) is equal to \(g\). In practice \(f\) is
defined by a stress criterion \(\phi\), a set of kinematic hardening
rules, and an isotropic hardening rule, as follows:

\[
f\paren{\tsigma,p}= \phi\paren{\tsigma-\sum_{i}\tenseur{X}_{i}}-\sum_{i}R_{i}\paren{p}
\]

where \(p\) is the equivalent plastic strain. Here we have decomposed
the limit of the elastic domain as a sum, denoted
\(\sum_{i}R_{i}\paren{p}\), to indicate that one may define it by
combining various predefined forms of isotropic hardening rules (Voce,
Linear, etc.) defined hereafter.

#### Maximum equivalent stress in the `Plastic` flow

During the Newton iterations, the current estimate of the equivalent
stress \(\sigmaeq\) may be significantly higher than the elastic limit
\(R\). This may lead to a divergence of the Newton algorithm.

One may reject the Newton steps leading to such high values of the
elastic limit by specifying a relative threshold denoted \(\alpha\),
i.e. if \(\sigmaeq\) is greater than \(\alpha\,\cdot\,R\). A typical
value for \(\alpha\) is \(1.5\). This relative threshold is specified by
the `maximum_equivalent_stress_factor` option.

In some cases, rejecting steps may also lead to a divergence of the
Newton algorithm, so one may specify a relative threshold \(\beta\) on
the iteration number which desactivate this check, i.e. the check is
performed only if the current iteration number is below
\(\beta\,\cdot\,i_{\max{}}\) where \(i_{\max{}}\) is the maximum number
of iterations allowed for the Newton algorithm. A typical value for
\(\beta\) is \(0.4\). This relative threshold is specified by the
`equivalent_stress_check_maximum_iteration_factor` option.

##### Example

~~~~{.cxx}
@DSL Implicit;
@Behaviour PerfectPlasticity;
@Author Thomas Helfer;
@Date 17 / 08 / 2020;
@Description{};

@Epsilon 1.e-14;
@Theta 1;

@Brick StandardElastoViscoPlasticity{
  stress_potential : "Hooke" {young_modulus : 200e9, poisson_ratio : 0.3},
  inelastic_flow : "Plastic" {
    criterion : "Mises",
    isotropic_hardening : "Linear" {R0 : 150e6},
    maximum_equivalent_stress_factor : 1.5,
    equivalent_stress_check_maximum_iteration_factor: 0.4
  }
};
~~~~

### The `Norton` inelastic flow

The plastic flow is defined by:

- a function \(f\paren{\tsigma}\) giving the flow intensity:
  \[
  f\paren{\tsigma}=A\,\left<\dfrac{\phi\paren{\tsigma-\sum_{i}\tenseur{X}_{i}}-\sum_{i}R_{i}\paren{p}}{K}\right>^{n}
  \]
- a stress criterion \(\phi\)
- a viscoplastic potential \(g\)
- one or more kinematic hardening rule (optional), denoted \(\tenseur{X}_{i}\)
- one isotropic hardening rule (optional), denoted \(R_{i}\)

### The `HyperbolicSine` inelastic flow

The viscoplastic flow is defined by:

- a function \(f\paren{\tsigma}\) giving the flow intensity given by:
  \[
  f\paren{\tsigma}=A\,\sinh\paren{\dfrac{\left<\phi\paren{\tsigma-\sum_{i}\tenseur{X}_{i}}-\sum_{i}R_{i}\paren{p}\right>}{K}}^{n}
  \]
- a stress criterion \(\phi\)
- one or more kinematic hardening rule (optional), denoted \(\tenseur{X}_{i}\)
- one isotropic hardening rule (optional), denoted \(R_{i}\)

### The `HarmonicSumOfNortonHoffViscoplasticFlows` inelastic flow 

The equivalent viscoplastic strain rate \(\dot{p}\) is defined as:

\[
\dfrac{1}{\dot{p}}=\sum_{i=1}^{N}\dfrac{1}{\dot{p}_{i}}
\]

where \(\dot{p}_{i}\) has an expression similar to the the Norton-Hoff
viscoplastic flow:

\[
\dot{p}_{i}=A_{i}\,\paren{\dfrac{\left<\phi\paren{\tsigma-\sum_{j}\tenseur{X}_{j}}-\sum_{j}R_{j}\paren{p}\right>}{K_{i}}}^{n_{i}}
\]

in which appear:

- a stress criterion \(\phi\)
- one or more kinematic hardening rule (optional), denoted \(\tenseur{X}_{i}\)
- one isotropic hardening rule (optional), denoted \(R_{i}\)

#### Example

~~~~{.cxx}
@Brick StandardElastoViscoPlasticity{
  stress_potential : "Hooke" {young_modulus : 150e9, poisson_ratio : 0.3},
  inelastic_flow : "HarmonicSumOfNortonHoffViscoplasticFlows" {
    criterion : "Mises",
    A : {8e-67, 8e-67},
    K : {1,1},
    n : {8.2,8.2}
  }
};
~~~~

#### Newton steps rejection

The exponential nature of the hyperbolic sinus function may lead to
divergence of the Newton method. To avoid this, one may specify a
relative threshold denoted \(K_{sf}\): if the stress estimate is greater
than \(K_{sf}\,K\), the step is rejected.

### The `UserDefinedViscoplasticity` inelastic flow

The `UserDefinedViscoplasticity` inelastic flow allows the user to
specify the viscoplastic strain rate `vp` as a function of `f` and `p`
where:

- `f` is the positive part of the
  \(\phi\paren{\tsigma-\sum_{i}\tenseur{X}_{i}}-\sum_{i}R_{i}\paren{p}\)
  where \(\phi\) is the stress criterion.
- `p` is the equivalent viscoplastic strain.

This function shall be given by a string option named `vp`. This
function must depend on `f`. Dependance to `p` is optional.

The function may also depend on other variables. Let `A` be such a
variable. The `UserDefinedViscoplasticity` flow will look if an option
named `A` has been given to the flow:


- If this option exists, it will be interpreted as a material
  coefficient as usal and this option can be a number, a formula or the
  name of an external `MFront` file.
- If this option does not exist, a suitable variable will be search in
  the variables defined in the behaviour (static variables, parameters,
  material properties, etc...).

If required, the derivatives of `vp` with respect to `f` and `p` can be
provided through the options `dvp_df` and `dvp_dp`. The derivatives
`dvp_df` and `dvp_dp` can depend on two additional variables, `vp` and
`seps`, which denotes the viscoplastic strain rate and a stress
threshold.

If those derivatives are not provided, automatic differentiation will be
used. The user shall be warned that the automatic differentiation
provided by the `tfel::math::Evaluator` class may result in inefficient
code.

#### Example of usage {.unnumbered}

~~~~{.cxx}
@Parameter temperature Ta = 600;
@Parameter strain p0 = 1e-8;

@Brick StandardElastoViscoPlasticity{
  stress_potential : "Hooke" {young_modulus : 150e9, poisson_ratio : 0.3},
  inelastic_flow : "UserDefinedViscoplasticity" {
    criterion : "Mises",
    E : 8.2,
    A : "8e-67 * exp(- T / Ta)",
    m : 0.32,
    vp : "A * (f ** E) / ((p + p0) ** m)",
    dvp_df : "E * vp / (max(f, seps))"
    // dvp_dp is evaluated by automatic differentiation (which is not recommended)
  }
};
~~~~

## Newton steps rejections based on the change of the flow direction between two successive estimates {#sec:cosine_checks}

Some stress criteria (Hosford 1972, Barlat 2004, Mohr-Coulomb) shows
sharp edges that may cause the failure of the standard Newton algorithm,
due to oscillations in the prediction of the flow direction.

Rejecting Newton steps leading to a too large variation of the flow
direction between the new estimate of the flow direction and the
previous estimate is a cheap and efficient method to overcome this
issue. This method can be viewed as a bisectional linesearch based on
the Newton prediction: the Newton steps magnitude is divided by two if
its results to a too large change in the flow direction.

More precisely, the change of the flow direction is estimated by the
computation of the cosine of the angle between the two previous
estimates:

\[
\cos\paren{\alpha_{\tenseur{n}}}=\dfrac{\tenseur{n}\,\colon\,\tenseur{n}_{p}}{\norm{\tenseur{n}}\,\norm{\tenseur{n}}}
\]

with \(\norm{\tenseur{n}}=\sqrt{\tenseur{n}\,\colon\,\tenseur{n}}\).

The Newton step is rejected if the value of
\(\cos\paren{\alpha_{\tenseur{n}}}\) is lower than a user defined
threshold. This threshold must be in the range \(\left[-1:1\right]\),
but due to the slow variation of the cosine near \(0\), a typical value
of this threshold is \(0.99\) which is equivalent to impose that the
angle between two successive estimates is below \(8\mbox{}^{\circ}\).


## List of available stress criteria

The following section describes stress criteria available by default.
However, the `StandardElastoViscoPlasticity` brick can also be extended
by the user:

- [this
  page](ExtendingStandardElastoViscoPlasticityBrick-StressCriterion.html)
  describes how to add a new stress criterion which is not coupled with
  the evolution of the porosity.
- [this
  page](ExtendingStandardElastoViscoPlasticityBrick-PorousStressCriterion.html)
  describes how to add a new stress criterion coupled with the evolution
  of the porosity.


### von Mises stress criterion

#### Definition

The von Mises stress is defined by:
\[
\sigmaeq=\sqrt{\dfrac{3}{2}\,\tenseur{s}\,\colon\,\tenseur{s}}=\sqrt{3\,J_{2}}
\]
where:
- \(\tenseur{s}\) is the deviatoric stress defined as follows:
\[
\tenseur{s}=\tsigma-\dfrac{1}{3}\,\trace{\tsigma}\,\tenseur{I}
\]
- \(J_{2}\) is the second invariant of \(\tenseur{s}\).

In terms of the eigenvalues of the stress, denoted by \(\sigma_{1}\),
\(\sigma_{2}\) and \(\sigma_{3}\), the von Mises stress can also be
defined by:
\[
\sigmaeq=\sqrt{\dfrac{1}{2}\paren{\absvalue{\sigma_{1}-\sigma_{2}}^{2}+\absvalue{\sigma_{1}-\sigma_{3}}^{2}+\absvalue{\sigma_{2}-\sigma_{3}}^{2}}}
\]

#### Options

This stress criterion does not have any option.

~~~~{.cpp}
    criterion : "Mises"
~~~~

### Drucker 1949 stress criterion

The Drucker 1949 stress is defined by:
\[
\sigmaeq=\sqrt{3}\sqrt[6]{J_{2}^3-c\,J_{3}^{2}}
\]
where:

- \(J_{2}=\dfrac{1}{2}\,\tenseur{s}\,\colon\,\tenseur{s}\) is the second
  invariant of \(\tenseur{s}\).
- \(J_{3}=\mathrm{det}\paren{\tenseur{s}}\) is the third invariant of
  \(\tenseur{s}\).
- \(\tenseur{s}\) is the deviatoric stress defined as follows:
\[
\tenseur{s}=\tsigma-\dfrac{1}{3}\,\trace{\tsigma}\,\tenseur{I}
\]

### Example

~~~~{.cpp}
    criterion : "Drucker 1949" {c : 1.285}
~~~~

#### Options

The user must provide the \(c\) coefficient.

### Hosford 1972 stress criterion

The Hosford equivalent stress is defined by (see @hosford_generalized_1972):
\[
\sigmaeq^{H}=\sqrt[a]{\dfrac{1}{2}\paren{\absvalue{\sigma_{1}-\sigma_{2}}^{a}+\absvalue{\sigma_{1}-\sigma_{3}}^{a}+\absvalue{\sigma_{2}-\sigma_{3}}^{a}}}
\]
where \(\sigma_{1}\), \(\sigma_{2}\) and \(\sigma_{3}\) are the eigenvalues of the
stress.

Therefore, when \(a\) goes to infinity, the Hosford stress reduces to
the Tresca stress. When \(n = 2\) the Hosford stress reduces to the
von Mises stress.

![Comparison of the Hosford stress \(a=100,a=8\) and the von Mises stress in plane stress](img/HosfordStress.svg
 "Comparison of the Hosford stress \(a=100,a=8\) and the von Mises
 stress in plane stress"){width=70%}

### Options

The Hosford exponent `a` is mandatory.

Specifying the eigen solver using the `eigen_solver` option is optional.
This option can have the value `default` or the value `Jacobi`.

### Example

~~~~{.cpp}
    criterion : "Hosford" {a : 6}
~~~~

### Notes

The Hosford yield surface may have sharp edges which may lead to
divergence of the Newton algorithm du to oscillations of the flow
direction. Specifying a threshold for the angle between. See Section
@sec:cosine_checks for details.

#### Options

The user must provide the Hosford exponent \(a\).

### Isotropic Cazacu 2004 stress criterion

In order to describe yield differential effects, the isotropic Cazacu
2004 equivalent stress criterion is defined by (see
@cazacu_criterion_2004):

\[
\sigmaeq=\sqrt[3]{J_{2}^{3/2} - c \, J_{3}}
\]

where:

- \(J_{2}=\dfrac{1}{2}\,\tenseur{s}\,\colon\,\tenseur{s}\) is the second
  invariant of \(\tenseur{s}\).
- \(J_{3}=\mathrm{det}\paren{\tenseur{s}}\) is the third invariant of
  \(\tenseur{s}\).
- \(\tenseur{s}\) is the deviatoric stress defined as follows:
\[
\tenseur{s}=\tsigma-\dfrac{1}{3}\,\trace{\tsigma}\,\tenseur{I}
\]

### Example

~~~~{.cpp}
    criterion : "Isotropic Cazacu 2004" {c : -1.056}
~~~~

### Hill stress criterion

This `Hill` criterion, also called `Hill1948` criterion, is based on the
equivalent stress \(\sigmaeq^{H}\) defined as follows:
\[
\begin{aligned}
\sigmaeq^{H}&=\sqrt{\tsigma\,\colon\,\tenseurq{H}\,\colon\,\tsigma}\\
	        &=\sqrt{F\,\paren{\sigma_{11}-\sigma_{22}}^2+
                    G\,\paren{\sigma_{22}-\sigma_{33}}^2+
			        H\,\paren{\sigma_{33}-\sigma_{11}}^2+
					2\,L\sigma_{12}^{2}+
					2\,M\sigma_{13}^{2}+
					2\,N\sigma_{23}^{2}}
\end{aligned}
\]

> **Warning** This convention is given in the book of Lemaître et
> Chaboche and seems to differ from the one described in most other
> books.

#### Options

This stress criterion has \(6\) mandatory options: `F`, `G`, `H`, `L`,
`M`, `N`. Each of these options must be interpreted as material
property.

> **Orthotropic axis convention** If an orthotropic axis convention
> is defined (See the `@OrthotropicBehaviour` keyword' documentation), 
> the coefficients of the Hill tensor can be exchanged for some
> modelling hypotheses. The coefficients `F`, `G`, `H`, `L`, `M`, `N`
> must always correspond to the three dimensional case.

### Example

~~~~{.cpp}
    criterion : "Hill" {F : 0.371, G : 0.629, H : 4.052, L : 1.5, M : 1.5, N : 1.5},
~~~~

### Cazacu 2001 stress criterion

![Plane stress yield surface (\(\sigma_{xy}=0\) and \(\sigma_{xy}=0.45\,\sigma_{0}\)) of 2090-T3 alloy sheet as predicted by the generalization of the Drucker yield criterion using generalized invariants (See @cazacu_generalization_2001, Figure 6).](img/Cazacu2001_2090-T3.svg
 "Plane stress yield surface (\(\sigma_{xy}=0\) and
 \(\sigma_{xy}=0.45\,\sigma_{0}\)) of 2090-T3 alloy sheet as predicted
 by the generalization of the Drucker yield criterion using
 generalized invariants (See @cazacu_generalization_2001, Figure
 3)"){width=80%}

Within the framework of the theory of representation, generalizations
to orthotropic conditions of the invariants of the deviatoric stress
have been proposed by Cazacu and Barlat (see
@cazacu_generalization_2001):

- The generalization of \(J_{2}\) is denoted \(J_{2}^{O}\). It is
  defined by:
  \[
  J_{2}^{O}= a_6\,s_{yz}^2+a_5\,s_{xz}^2+a_4\,s_{xy}^2+\frac{a_2}{6}\,(s_{yy}-s_{zz})^2+\frac{a_3}{6}\,(s_{xx}-s_{zz})^2+\frac{a_1}{6}\,(s_{xx}-s_{yy})^2
  \]
  where the \(\left.a_{i}\right|_{i\in[1:6]}\) are six coefficients
  describing the orthotropy of the material.
- The generalization of \(J_{3}\) is denoted \(J_{3}^{O}\). It is
  defined by:
  \[
  \begin{aligned}
  J_{3}^{O}=
  &\frac{1}{27}\,(b_1+b_2)\,s_{xx}^3+\frac{1}{27}\,(b_3+b_4)\,s_{yy}^3+\frac{1}{27}\,(2\,(b_1+b_4)-b_2-b_3)\,s_{zz}^3\\
  &-\frac{1}{9}\,(b_1\,s_{yy}+b_2s_{zz})\,s_{xx}^2\\
  &-\frac{1}{9}\,(b_3\,s_{zz}+b_4\,s_{xx})\,s_{yy}^2\\
  &-\frac{1}{9}\,((b_1-b_2+b_4)\,s_{xx}+(b_1-b3+b_4)\,s_{yy})\,s_{zz}^3\\
  &+\frac{2}{9}\,(b_1+b_4)\,s_{xx}\,s_{yy}\,s_{zz}\\
  &-\frac{s_{xz}^2}{3}\,(2\,b_9\,s_{yy}-b_8\,s_{zz}-(2\,b_9-b_8)\,s_{xx})\\
  &-\frac{s_{xy}^2}{3}\,(2\,b_{10}\,s_{zz}-b_5\,s_{yy}-(2\,b_{10}-b_5)\,s_{xx})\\
  &-\frac{s_{yz}^2}{3}\,((b_6+b_7)\,s_{xx}-b_6\,s_{yy}-b_7\,s_{zz})\\
  &+2\,b_{11}\,s_{xy}\,s_{xz}\,s_{yz}
  \end{aligned}
  \]
  where the \(\left.b_{i}\right|_{i\in[1:11]}\) are eleven coefficients
  describing the orthotropy of the material.

Those invariants may be used to generalize isotropic yield criteria
based on \(J_{2}\) and \(J_{3}\) invariants to orthotropy. The Cazacu
2001 equivalent stress criterion is defined as the orthotropic
counterpart of the Drucker 1949 yield criterion, as follows (see
@cazacu_generalization_2001):

\[
\sigmaeq=\sqrt{3}\sqrt[6]{\left(J_{2}^{O}\right)^3-c\,\left(J_{3}^{O}\right)^{2}}
\]

#### Options

This criterion requires the following options:

- `a`, as an array of \(6\) material properties.
- `b`, as an array of \(11\) material properties.
- `c`, as a material property.

#### Example

~~~~{.cpp}
    criterion : "Cazacu 2001" {
      a : {0.586, 1.05, 0.823, 0.96, 1, 1},
      b : {1.44, 0.061, -1.302, -0.281, -0.375, 1, 1, 1, 1, 0.445, 1},
      c : 1.285
    },
~~~~

#### Restrictions

Proper support of orthotropic axes conventions has not been implemented
yet for the computation of the \(J_{2}^{O}\) and \(J_{3}^{O}\). Thus,
the following restrictions apply:

- if no orthotropic axis convention is defined, only the
  `Tridimensional` modelling hypothesis is supported.q
- if the `Plate` orthotropic axis convention is used, only the
  `Tridimensional` and `PlaneStress` modelling hypotheses are supported.
- if the `Pipe` orthotropic axis convention is used, only theI
  `Tridimensional`, `Axisymmetrical`,
  `AxisymmetricalGeneralisedPlainStrain`, and
  `AxisymmetricalGeneralisedPlainStres` modelling hypotheses are
  supported.


### Orthotropic Cazacu 2004 stress criterion

![Plane stress yield loci for a magnesium sheet (See @cazacu_criterion_2004, Figure 6).](img/Cazacu2004.svg "Plane stress yield loci for a magnesium sheet"){width=80%}

Using the invariants \(J_{2}^{O}\) and \(J_{3}^{O}\) previously defined,
Cazacu and Barlat proposed the following criterion (See @cazacu_criterion_2004):

\[
\sigmaeq=\sqrt[3]{\left(J_{2}^{O}\right)^{3/2} - c\,J_{3}^{O}}
\]

#### Options

This criterion requires the following options:

- `a`, as an array of \(6\) material properties.
- `b`, as an array of \(11\) material properties.
- `c`, as a material property.

#### Example

~~~~{.cpp}
    criterion : "Orthotropic Cazacu 2004" {
      a : {0.586, 1.05, 0.823, 0.96, 1, 1},
      b : {1.44, 0.061, -1.302, -0.281, -0.375, 1, 1, 1, 1, 0.445, 1},
      c : 1.285
    },
~~~~

#### Restrictions

Proper support of orthotropic axes conventions has not been implemented
yet for the computation of the \(J_{2}^{O}\) and \(J_{3}^{O}\). Thus,
the following restrictions apply:

- if no orthotropic axis convention is defined, only the
  `Tridimensional` modelling hypothesis is supported.q
- if the `Plate` orthotropic axis convention is used, only the
  `Tridimensional` and `PlaneStress` modelling hypotheses are supported.
- if the `Pipe` orthotropic axis convention is used, only the
  `Tridimensional`, `Axisymmetrical`,
  `AxisymmetricalGeneralisedPlainStrain`, and
  `AxisymmetricalGeneralisedPlainStres` modelling hypotheses are
  supported.

### Barlat 2004 stress criterion

The Barlat equivalent stress is defined as follows (See @barlat_linear_2005):
\[
\sigmaeq^{B}=
\sqrt[a]{
  \frac{1}{4}\left(
  \sum_{i=0}^{3}
  \sum_{j=0}^{3}
  \absvalue{s'_{i}-s''_{j}}^{a}
  \right)
}
\]

where \(s'_{i}\) and \(s''_{i}\) are the eigenvalues of two
transformed stresses \(\tenseur{s}'\) and \(\tenseur{s}''\) by two
linear transformation \(\tenseurq{L}'\) and \(\tenseurq{L}''\):
\[
\left\{
\begin{aligned}
\tenseur{s}'  &= \tenseurq{L'} \,\colon\,\tsigma \\
\tenseur{s}'' &= \tenseurq{L''}\,\colon\,\tsigma \\
\end{aligned}
\right.
\]

The linear transformations \(\tenseurq{L}'\) and \(\tenseurq{L}''\)
are defined by \(9\) coefficients (each) which describe the material
orthotropy. There are defined through auxiliary linear transformations
\(\tenseurq{C}'\) and \(\tenseurq{C}''\) as follows:
\[
\begin{aligned}
\tenseurq{L}' &=\tenseurq{C}'\,\colon\,\tenseurq{M} \\
\tenseurq{L}''&=\tenseurq{C}''\,\colon\,\tenseurq{M}
\end{aligned}
\]
where \(\tenseurq{M}\) is the transformation of the stress to its deviator:
\[
\tenseurq{M}=\tenseurq{I}-\dfrac{1}{3}\tenseur{I}\,\otimes\,\tenseur{I}
\]

The linear transformations \(\tenseurq{C}'\) and \(\tenseurq{C}''\) of
the deviator stress are defined as follows:
\[
\tenseurq{C}'=
\begin{pmatrix}
0 & -c'_{12} & -c'_{13} & 0 & 0 & 0 \\
-c'_{21} & 0 & -c'_{23} & 0 & 0 & 0 \\
-c'_{31} & -c'_{32} & 0 & 0 & 0 & 0 \\
0 & 0 & 0 & c'_{44} & 0 & 0 \\
0 & 0 & 0 & 0 & c'_{55} & 0 \\
0 & 0 & 0 & 0 & 0 & c'_{66} \\
\end{pmatrix}
\quad
\text{and}
\quad
\tenseurq{C}''=
\begin{pmatrix}
0 & -c''_{12} & -c''_{13} & 0 & 0 & 0 \\
-c''_{21} & 0 & -c''_{23} & 0 & 0 & 0 \\
-c''_{31} & -c''_{32} & 0 & 0 & 0 & 0 \\
0 & 0 & 0 & c''_{44} & 0 & 0 \\
0 & 0 & 0 & 0 & c''_{55} & 0 \\
0 & 0 & 0 & 0 & 0 & c''_{66} \\
\end{pmatrix}
\]

When all the coefficients \(c'_{ji}\) and \(c''_{ji}\) are equal to
\(1\), the Barlat equivalent stress reduces to the Hosford equivalent
stress.

#### Options

This stress criterion has \(3\) mandatory options:

- the coefficients of the first linear transformation \(\tenseurq{L}'\),
  as `l1`;
- the coefficients of the second linear transformation
  \(\tenseurq{L}''\), as `l2`;
- the Barlat exponent \(a\)

> **Orthotropic axis convention** If an orthotropic axis convention
> is defined (See the `@OrthotropicBehaviour` keyword' documentation), 
> the coefficients of the linear transformationscan be exchanged for some
> modelling hypotheses. The coefficients given by the user must always
> correspond to the three dimensional case.

Specifying the eigen solver using the `eigen_solver` option is optional.
This option can have the value `default` or the value `Jacobi`.

### Example

~~~~{.cpp}
    criterion : "Barlat" {
      a : 8,
      l1 : {-0.069888, 0.079143, 0.936408, 0.524741, 1.00306, 1.36318, 0.954322,
            1.06906, 1.02377},
      l2 : {0.981171, 0.575316, 0.476741, 1.14501, 0.866827, -0.079294, 1.40462,
            1.1471, 1.05166}
    }
~~~~

### Notes

The Barlat 2004 yield surface may have sharp edges which may lead to
divergence of the Newton algorithm du to oscillations of the flow
direction. Specifying a threshold for the angle between. See Section
@sec:cosine_checks for details.

## List of available isotropic hardening rules

> **Note**
>
> The follwing hardening rules can be combined to define
> more complex hardening rules. For example, the following
> code adds to Voce hardening:
>
> ~~~~{.cpp}
>    isotropic_hardening : "Voce" {R0 : 600e6, Rinf : 900e6, b : 1},
>    isotropic_hardening : "Voce" {R0 : 0, Rinf : 300e6, b : 10},
> ~~~~
>
> The previous code is equalivent to the following hardening rule:
> 
> \[
> R\paren{p}=R_{0}^{0}+\paren{R_{\infty}^{0}-R_{0}^{0}}\,\paren{1-\exp\paren{-b^{0}\,p}}+R_{\infty}^{1}\,\paren{1-\exp\paren{-b^{1}\,p}}
> \]
>
> with:
> 
> - \(R_{0}^{0}=600\,.\,10^{6}\,Pa\) 
> - \(R_{\infty}^{0}=900\,.\,10^{6}\,Pa\) 
> - \(R_{\infty}^{1}=300\,.\,10^{6}\,Pa\) 
> - \(b^{0}=1\) 
> - \(b^{1}=10\) 

### The `Linear` isotropic hardening rule

The `Linear` isotropic hardening rule is defined by:
\[
R\paren{p}=R_{0}+H\,p
\]

#### Options

The `Linear` isotropic hardening rule expects one of the two following
material properties:

- `R0`: the yield strength
- `H`: the hardening slope

> **Note**
>
> If one of the previous material property is not defined, the generated
> code is optimised and there will be no parameter asscoiated with it.
> To avoid this, you must define the material property and assign
> it to a zero value.

#### Example

The following code can be added in a block defining an inelastic flow:

~~~~{.cpp}
    isotropic_hardening : "Linear" {R0 : 120e6, H : 438e6},
~~~~

### The `Swift` isotropic hardening rule

The `Swift` isotropic hardening rule is defined by:
\[
R\paren{p}=R_{0}\,\paren{\dfrac{p+p_{0}}{p_{0}}}^{n}
\]

#### Options

The `Swift` isotropic hardening rule expects three material properties:

- `R0`: the yield strength
- `p0`
- `n`

#### Example

The following code can be added in a block defining an inelastic flow:

~~~~{.cpp}
    isotropic_hardening : "Swift" {R0 : 120e6, p0 : 1e-8, n : 5.e-2}
~~~~

### The `Power` isotropic hardening rule (since TFEL 3.4)

The `Power` isotropic hardening rule is defined by:
\[
R\paren{p}=R_{0}\,\paren{p+p_{0}}^{n}
\]

#### Options

The `Power` isotropic hardening rule expects at least the following
material properties:

- `R0`: the coefficient of the power law
- `n`: the exponent of the power law

The `p0` material property is *optional* and generally is considered a
numerical parameter to avoir an initial infinite derivative of the power
law when the exponent is lower than \(1\).

#### Example

The following code can be added in a block defining an inelastic flow:

~~~~{.cpp}
    isotropic_hardening : "Linear" {R0 : 50e6},
    isotropic_hardening : "Power" {R0 : 120e6, p0 : 1e-8, n : 5.e-2}
~~~~

### The `Voce` isotropic hardening rule

The `Voce` isotropic hardening rule is defined by:
\[
R\paren{p}=R_{\infty}+\paren{R_{0}-R_{\infty}}\,exp\paren{-b\,p}
\]

#### Options

The `Voce` isotropic hardening rule expects three material properties:

- `R0`: the yield strength
- `Rinf`: the utimate strength
- `b`

#### Example

The following code can be added in a block defining an inelastic flow:

~~~~{.cpp}
    isotropic_hardening : "Voce" {R0 : 200, Rinf : 100, b : 20}
~~~~

### User defined isotropic hardening rule

The `UserDefined` isotropic hardening rule allows the user to specify
the radius of the yield surface as a function of the equivalent plastic
strain `p`.

This function shall be given by a string option named `R` and must
depend on `p`. The function may also depend on other variables. Let `A`
be such a variable. The `UserDefined` isotropic hardening rule will look
if an option named `A` has been given:

- If this option exists, it will be interpreted as a material
  coefficient as usal and this option can be a number, a formula or the
  name of an external `MFront` file.
- If this option does not exist, a suitable variable will be search in
  the variables defined in the behaviour (static variables, parameters,
  material properties, etc...).

If required, the derivative of `R` with respect to `p` can be provided
through the option `dR_dp`. The derivative `dR_dp` can depend on the
variable `R`.

If this derivative is not provided, automatic differentiation will be
used. The user shall be warned that the automatic differentiation
provided by the `tfel::math::Evaluator` class may result in inefficient
code.

#### Example of usage {.unnumbered}

~~~~{.cxx}
@Parameter stress R0 = 200e6;
@Parameter stress Hy = 40e6;
@Parameter real b = 100;

@Brick StandardElastoViscoPlasticity{
  stress_potential : "Hooke" {young_modulus : 150e9, poisson_ratio : 0.3},
  inelastic_flow : "Plastic" {
    criterion : "Mises",
    isotropic_hardening : "UserDefined" {
      R : "R0 + Hy * (1 - exp(-b * p))",     // Yield radius
      dR_dp : "b * (R0 + Hy - R)"
    }
  }
};
~~~~

### Isotropic harderning rule based defined by points

The `Data` isotropic hardening rule allows the user to define an
isotropic hardening rule using a curve defined by a set of pairs of
equivalent strain and equivalent stress.

This isotropic hardening rule can be parametrised using three entries:

- `values`: which must a dictionnary giving the value of the yield
  surface radius as a function of the equivalent plastic strain.
- `interpolation`: which allows to select the interpolation type.
  Possible values are `linear` (default choice) and `cubic_spline`.
- `extrapolation`: which allows to select the extrapolation type.
  Possible values are `bound_to_last_value` (or `constant`) and
  `extrapolation` (default choice).

#### Example of usage

~~~~{.cxx}
@Brick StandardElastoViscoPlasticity{
  stress_potential : "Hooke" {young_modulus : 150e9, poisson_ratio : 0.3},
  inelastic_flow : "Plastic" {
    criterion : "Mises",
    isotropic_hardening : "Data" {
      values : {0 : 150e6, 1e-3 : 200e6, 2e-3 : 400e6},
      interpolation : "linear"
    }
  }
};
~~~~

### Strain rate sensitive isotropic hardening rule

The `StrainRateSensitive` isotropic hardening rule is defined by:
\[
 R\paren{p, \dot{p}} = R_{0}\paren{p}\,R_{rs}\paren{\dot{p}},
\]

where:

- \(R_{0}\paren{p}\) is the yield radius corresponding to an infinitly
  slow loading. It can be built by summing any isotropic hardening rule
  already implemented in the `StandardElastoViscoPlasticity\ brick.
- \(R_{rs}\paren{\dot{p}}\) is a correction describing the strain rate
  sensitivity. \(R_{rs}\paren{0}\) must be equal to 1.

This isotropic hardening rule can be parametrised using the following
options:

- `rate_independent_isotropic_hardening`: this option introduces a
  contribution to \(R_{0}\paren{p}\). This option can be repeated
  multiple times.
- `rate_sensitivity_factor`: this option introduces the rate sensivity
  factor \(R_{rs}\paren{\dot{p}}\). The list of available rate sensivity
  factors is given in section @sec:rate_sensitivity_factors.

#### Example of usage

~~~~{.cxx}
@Brick StandardElastoViscoPlasticity{
  stress_potential : "Hooke" {young_modulus : 210e9, poisson_ratio : 0.3},
  inelastic_flow : "Plastic" {
    criterion : "Mises",
    isotropic_hardening : "StrainRateSensitive" {
      rate_independent_isotropic_hardening :
          "Swift" {R0 : "alpha * Ks * (p0s ** ns)", p0 : "p0s", n : "ns"},
      rate_independent_isotropic_hardening : "Voce" {
        R0 : "(1 - alpha) * Q1 * (1 - Q2)",
        Rinf : "(1 - alpha) * Q1",
        b : "Q3"
      },
      rate_sensitivity_factor :
          "CowperSymonds" {dp0 : "dp0cs", n : "ncs", Rs_eps : 1e-8}
    }
  }
};
~~~~

## List of rate sensitivity factors {#sec:rate_sensitivity_factors}

### Cowper-Symonds's rate sensitivity factor

\[
R_{rs}\paren{\dot{p}}=1+A\,\left(\frac{\dot{p}}{\dot{p}_{0}}\right)^{n}
\]

When the exponent \(n\) is lower than one, the following regularised
version, based on the user defined value \(R_{\epsilon}\) is available:

\[
R_{rs}\paren{\dot{p}}=1+
\left\{
\begin{aligned}
R_{\epsilon}\,\left(\frac{\dot{p}}{\dot{p}_{\epsilon}}\right) &&\text{if}&&\dot{p}<\dot{p}_{\epsilon}\\
A\,\left(\frac{\dot{p}}{\dot{p}_{0}}\right)^{n} &&\text{if}&&\dot{p}\geq\dot{p}_{\epsilon}
\end{aligned}
\right.
\]

### Johnson-Cook's rate sensitivity factor

\[
R_{rs}\paren{\dot{p}}=1+
\left\{
\begin{aligned}
0                                         &&\text{if}&&\dot{p}<\dot{p}_{0}\\
A\,\log\left(\frac{\dot{p}}{\dot{p}_{0}}\right)&&\text{if}&&\dot{p}\geq\dot{p}_{0}
\end{aligned}
\right.
\]


## List of available kinematic hardening rules

### The `Prager` kinematic hardening rule

#### Example

The following code can be added in a block defining an inelastic flow:

~~~{.cpp}
    kinematic_hardening : "Prager" {C : 33e6},
~~~

### The `Armstrong-Frederick` kinematic hardening rule


The `Armstrong-Frederick` kinematic hardening rule can be described as
follows (see @armstrong_mathematical_1966):
\[
\left\{
\begin{aligned}
\tenseur{X}&=\dfrac{2}{3}\,C\,\tenseur{a} \\
\tenseur{\dot{a}}&=\dot{p}\,\tenseur{n}-D\,\dot{p}\,\tenseur{a} \\
\end{aligned}
\right.
\]

#### Example

The following code can be added in a block defining an inelastic flow:

~~~{.cpp}
    kinematic_hardening : "Armstrong-Frederick" {C : 1.5e9, D : 5}
~~~

### The `Burlet-Cailletaud` kinematic hardening rule

The `Burlet-Cailletaud` kinematic hardening rule is defined as follows
(see @burlet_modelling_1987):

\[
\left\{
\begin{aligned}
\tenseur{X}&=\dfrac{2}{3}\,C\,\tenseur{a} \\
\tenseur{\dot{a}}&=\dot{p}\,\tenseur{n}
-\eta\,D\,\dot{p}\,\tenseur{a}
-\paren{1-\eta}\,D\,\dfrac{2}{3}\,\dot{p}\,\paren{\tenseur{a}\,\colon\,\tenseur{n}}\,\tenseur{n} \\
\end{aligned}
\right.
\]

#### Example

The following code can be added in a block defining an inelastic flow:

~~~{.cpp}
    kinematic_hardening : "Burlet-Cailletaud" {C : 250e7, D : 100, eta : 0}
~~~

### The `Chaboche 2012` kinematic hardening rule

The `Chaboche 2012` kinematic hardening rule is defined as follows
(see @chaboche_cyclic_2012):

\[
\tenseur{\dot{a}}
=\tenseur{\dot{\varepsilon}}^{p}-\frac{3\,D}{2\,C}\,\Phi\left(p\right)\,
\Psi^{\left(\tenseur{X}\right)}\left(\tenseur{X}\right)\,\dot{p}\,\tenseur{X}
=\tenseur{\dot{\varepsilon}}^{p}-
D\,\Phi\left(p\right)\,\Psi\left(\tenseur{a}\right)\dot{p}\,\tenseur{a}
\]

with:

- \(\tenseur{X}=\frac{2}{3}\,C\,\tenseur{a}\)
- \(
\Phi\left(p\right)=\phi_{\infty}+
\left(1-\phi_{\infty}\right)\,\exp\left(-b\,p\right)
\)
- \(
\Psi^{\left(\tenseur{X}\right)}\left(\tenseur{X}\right)=
\frac{\left<D\,J\left(\tenseur{X}\right)-\omega\,C\right>^{m}}{1-\omega}\,
\frac{1}{\left(D\,J\left(\tenseur{X}\right)\right)^{m}}
\)
- \(
\Psi\left(\tenseur{a}\right)=
\frac{\left<D\,J\left(\tenseur{a}\right)-\frac{3}{2}\omega\right>^{m}}{1-\omega}\,
\frac{1}{\left(D\,J\left(\tenseur{a}\right)\right)^{m}}
\)

#### Example

The following code can be added in a block defining an inelastic flow:

~~~~{.cpp}
    kinematic_hardening : "Chaboche 2012" {
      C : 250e7,
      D : 100,
      m : 2,
      w : 0.6,
    }
~~~~

### Delobelle-Robinet-Schaffler (DRS) kinematic hardening rule

The Delobelle-Robinet-Schaffler (DRS) kinematic hardening rule has been
introduced to describe orthotropic viscoplasticity of Zircaloy alloys
[@delobelle_model_1996;@schaffler_thermomechanical_1999]. It describes
both dynamic and static recovery by the following evolution law:
\[
\tenseur{\dot{a}}=
\dot{p}\,\tenseurq{E}_{c}\,\colon\,\tenseur{n}
-D\,\dot{p}\,\tenseurq{R}_{d}\,\colon\,\tenseur{a}
-f\,\paren{\frac{a_{\mathrm{eq}}}{a_{0}}}^{m}\,\deriv{a_{\mathrm{eq}}}{\tenseur{a}}
\]
with \(a_{\mathrm{eq}}=\sqrt{\tenseur{a}\,\colon\,\tenseurq{R}_{s}\,\colon\,\tenseur{a}}\) and 
\(\deriv{a_{\mathrm{eq}}}{\tenseur{a}}=\frac{\tenseurq{R}_{s}\,\colon\,\tenseur{a}}{a_{\mathrm{eq}}}\)

The three fourth order tensors \(\tenseurq{E}_{c}\),
\(\tenseurq{R}_{d}\) and \(\tenseurq{R}_{s}\) are assumed to have the
same structure as the Hill tensors and are defined by \(6\) components
(see [this page](tensors.html) for details).

The `f` and `a0` parameters are optional and defaults to \(1\).

#### Example

~~~~{.cxx}
    kinematic_hardening : "DRS" {
      C : 150.e9,  // kinematic moduli
      D : 1e2,    // back-strain callback coefficient
      f : 10,
      m : 5,
      Ec : {0.33, 0.33, 0.33, 1, 1, 1},
      Rs : {0.33, 0.63, 0.33, 1, 1, 1},
      Rd : {0.33, 0.33, 0.33, 1, 1, 1}  //
    },
~~~~

# References
