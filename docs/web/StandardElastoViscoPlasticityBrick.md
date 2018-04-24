% Description of the `StandardElastoViscoPlasticity` brick
% Thomas Helfer
% 15/04/2018

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
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\nom}[1]{\textsc{#1}}
\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}

This page describes the `StandardElastoViscoPlasticity` brick. This
brick is used to describe a specific class of strain based behaviours
based on an additive split of the total strain \(\tepsilonto\) into an
elastic part \(\tepsilonel\) and an one or several inelastic strains
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
  stress measure \(\tsigma\).
- a list of inelastic flows.

# List of available stress potentials

## The Hooke stress potential

This stress potential implements the Hooke law, i.e. a linear relation
between the elastic strain and the stress. This stress potential applies
to isotropic and orthotropic materials. This stress potential provides:

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
defined by a stress criterion \(\phi\) and an isotropic hardening rule
\(R\paren{p}\), as follows:

\[
f\paren{\tsigma,p}= \phi\paren{\tsigma-\sum_{i}\tenseur{X}_{i}}-R\paren{p}
\]

where \(p\) is the equivalent plastic strain.

### The `Norton` inelastic flow

The plastic flow is defined by:

- a function \(f\paren{\tsigma}\) giving the flow intensity
- a viscoplastic potential \(g\)

\[
f\paren{\tsigma}=A\left<\Frac{\phi\paren{\tsigma-\sum_{i}\tenseur{X}_{i}}}{K}\right>^{n}
\]

## List of available stress criterion

### von Mises stress criterion

#### Definition

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

In terms of the eigenvalues of the stress, denoted by \(\sigma_{1}\),
\(\sigma_{2}\) and \(\sigma_{3}\), the von Mises stress can also be
defined by:
\[
\sigmaeq=\sqrt{\Frac{1}{2}\paren{\absvalue{\sigma_{1}-\sigma_{2}}^{2}+\absvalue{\sigma_{1}-\sigma_{3}}^{2}+\absvalue{\sigma_{2}-\sigma_{3}}^{2}}}
\]

#### Options

This stress criterion does not have any option.

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

The `Swift` isotropic hardening rule expects one of the two following
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
R\paren{p}=R_{0}\,\paren{\Frac{p+p_{0}}{p_{0}}}^{n}
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
\tenseur{X}&=\Frac{2}{3}\,C\,\tenseur{a} \\
\tenseur{\dot{a}}&=\dot{p}\,\tenseur{n}-D\,\tenseur{a} \\
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
\tenseur{X}&=\Frac{2}{3}\,C\,\tenseur{a} \\
\tenseur{\dot{a}}&=\dot{p}\,\tenseur{n}-\eta\,D\,\tenseur{a}-\paren{1-\eta}\,D\,\Frac{2}{3}\,\paren{\tenseur{a}\,\colon\,\tenseur{n}}\,\tenseur{n} \\
\end{aligned}
\right.
\]

#### Example

The following code can be added in a block defining an inelastic flow:

~~~{.cpp}
    kinematic_hardening : "Burlet-Cailletaud" {C : 250e7, D : 100, eta : 0}
~~~

# References
