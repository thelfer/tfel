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

# List of available stress criterion

## von Mises stress criterion

### Definition

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

### Options

This stress criterion does not have any option.

## Hill stress criterion

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

### Options

This stress criterion has \(6\) mandatory options: `F`, `G`, `H`, `L`,
`M`, `N`. Each of these options must be interpreted as material
property.

> **Orthotropic axis convention** If an orthotropic axis convention
> is defined (See the `@OrthotropicBehaviour` keyword' documentation), 
> the coefficients of the Hill tensor can be exchanged for some
> modelling hypotheses. The coefficients `F`, `G`, `H`, `L`, `M`, `N`
> must always correspond to the three dimensional case.

# List of available isotropic hardening rules

## The `Linear` isotropic hardening rule

The `Linear` isotropic hardening rule is defined by:
\[
R\paren{p}=R_{0}+H\,p
\]

## The `Swift` isotropic hardening rule

The `Swift` isotropic hardening rule is defined by:
\[
R\paren{p}=R_{0}\,\paren{\Frac{p+p_{0}}{p_{0}}}^{n}
\]

## The `Voce` isotropic hardening rule

The `Voce` isotropic hardening rule is defined by:
\[
R\paren{p}=R_{\infty}+\paren{R_{0}-R_{\infty}}\,exp\paren{-b\,p}
\]

# List of available kinematic hardening rules

## The `Prager` kinematic hardening rule

## The `Armstrong-Frederick` kinematic hardening rule

## The `Burlet-Cailletaud` kinematic hardening rule

# List of available inelastic flows

## The `Plastic` inelastic flow

## The `Norton` inelastic flow

