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
  stress measure \(\tsigma\). As the definition of the elastic
  properties can be part of the definition of the stress potential, the
  thermal expansion coefficients can also be defined in the block
  corresponding to the stress potential.
- a list of inelastic flows.

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
      C : "1.e6 - 98500 * (T - 100) / 96",  //
      D : "5000 - 5* (T - 100)"
    },
    K : "(4200. * (T + 20.) - 3. * (T + 20.0)**2)/4900.",
    n : "7. - (T - 100.) / 160.",
    Ksf : 3
  }
};
~~~~

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

~~~~{.cpp}
    criterion : "Mises"
~~~~

### Hosford stress criterion

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

### Example

~~~~{.cpp}
    criterion : "Hosford" {a : 6}
~~~~

#### Options

The user must provide the Hosford exponent \(a\).

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
\tenseurq{M}=\tenseurq{I}-\Frac{1}{3}\tenseur{I}\,\otimes\,\tenseur{I}
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
