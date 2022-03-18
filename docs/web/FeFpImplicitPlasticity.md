---
title: Implicit integration of finite strain behaviours based on a multiplicative decomposition of the deformation gradient
author: Thomas Helfer, Chao Ling, Jérémy Hure, Jean-Michel Scherer
date: 01/02/2022
lang: en-EN
geometry:
- paper=a4paper
- margin=2.5cm
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
bibliography: bibliography.bib
csl: iso690-numeric-en.csl
header-includes:
- \usepackage{tcolorbox}
- \newtcolorbox{myquote}{colback=blue!5!white, colframe=blue!75!black}
- \renewenvironment{quote}{\begin{myquote}}{\end{myquote}}
---

\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tns}[1]{{\underset{\tilde{}}{\mathbf{#1}}}}
\newcommand{\transpose}[1]{{#1^{\mathrm{T}}}}
\newcommand{\inverse}[1]{{#1^{\mathrm{-1}}}}
\newcommand{\itranspose}[1]{{#1^{\mathrm{-T}}}}
\newcommand{\pinverse}[1]{{\left(#1\right)^{\mathrm{-1}}}}
\newcommand{\inverseT}[1]{{#1^{\mathrm{-T}}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\epsilonth}{\epsilon^{\mathrm{th}}}
\newcommand{\tepsilonto}{\underline{\epsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\underline{\epsilon}^{\mathrm{el}}}
\newcommand{\tepsilonth}{\underline{\epsilon}^{\mathrm{th}}}
\newcommand{\tepsilonvis}{\underline{\epsilon}^{\mathrm{vis}}}
\newcommand{\tdepsilonel}{\underline{\dot{\epsilon}}^{\mathrm{el}}}
\newcommand{\tdepsilonvis}{\underline{\dot{\epsilon}}^{\mathrm{vis}}}
\newcommand{\tepsilonp}{\underline{\epsilon}^{\mathrm{p}}}
\newcommand{\tdepsilonp}{\underline{\dot{\epsilon}}^{\mathrm{p}}}
\newcommand{\talpha}{\underline{\alpha}}
\newcommand{\tdalpha}{\underline{\dot{\alpha}}}
\newcommand{\txi}{\underline{\xi}}
\newcommand{\tdxi}{\underline{\dot{\xi}}}
\newcommand{\tDq}{\underline{\mathbf{D}}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\derivtot}[2]{{{\displaystyle \frac{\displaystyle \dtot #1}{\displaystyle \dtot #2}}}}
\newcommand{\deriv}[2]{{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}}
\newcommand{\secondderivative}[2]{{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial^{2} #2}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\nom}[1]{\textsc{#1}}
\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}
\newcommand{\F}{\tns{F}}
\renewcommand{\L}{\tns{L}}
\newcommand{\Ce}{\tenseur{C}_{e}}
\newcommand{\Se}{\tenseur{S}_{e}}
\newcommand{\Cr}{\tenseur{C}}
\newcommand{\Sr}{\tenseur{S}}
\newcommand{\egl}{\tenseur{\varepsilon}^{GL}_{e}}
\newcommand{\degl}{\tenseur{\dot{\varepsilon}}^{GL}_{e}}
\newcommand{\Le}{\tns{L}_{e}}
\newcommand{\Lp}{\tns{L}_{p}}
\newcommand{\dLp}{\tns{L}_{p}}
\newcommand{\D}{\tenseur{D}}
\newcommand{\De}{\tns{D}_{e}}
\newcommand{\Fe}{\tns{F}_{e}}
\newcommand{\Fp}{\tns{F}_{p}}
\newcommand{\dFdt}{\tns{\dot{F}}}
\newcommand{\dFedt}{\tns{\dot{F}}_{e}}
\newcommand{\dtFedt}{\transpose{\tns{\dot{F}}}_{e}}
\newcommand{\dFpdt}{\tns{\dot{F}}_{p}}
\newcommand{\N}{\tns{N}}
\newcommand{\M}{\tns{M}}
\newcommand{\Mdev}{\tns{M}_{\mathrm{dev}}}
\newcommand{\Meq}{M_{\mathrm{eq}}}
\newcommand{\DsFp}{\Delta_{\star}\,\Fp}
\newcommand{\ns}{\vec{n}_{s}}
\newcommand{\ms}{\vec{m}_{s}}
\newcommand{\Ns}{\tns{N}_{s}}

<!--
pandoc -f markdown+tex_math_single_backslash -F pandoc-crossref --citeproc --highlight-style=tango FeFpImplicitPlasticity.md -o FeFpImplicitPlasticity.pdf
-->


This page describes an implicit algorithm for a finite strain plastic
behaviour based on a multiplicative decomposition of the deformation
gradient \(\F\) in an elastic part \(\Fe\) and a plastic part \(\Fp\)
following the work of Mandel [@mandel_equations_1973].

# Thermodynamical approach

## Kinematics

Let \(\vec{x}\) be the current position of a point located at
\(\vec{X}\) int the intial configuration.

The displacement of this point is denoted \(\vec{u}\):
\[
  \vec{u}=\vec{x}-\vec{X}
\]

### The deformation gradient

The deformation gradient \(\F\) is defined as the derivative of the
current position with respect to the initial position:

\[
 \F = \deriv{\vec{x}}{\vec{X}} = \tns{I}+ \deriv{\vec{u}}{\vec{X}}
\]{#eq:mfront:FeFpImplicitPlasticity:F}

### The velocity gradient and the rate of deformation

Let \(\vec{\dot{x}}\) be the velocity of a point \(\vec{x}\) in the
current configuration.

The gradient of the velocity \(\vec{\dot{x}}\) in the current
configuration is denoted \(\tns{L}\):

\[
\tns{L}=\deriv{\vec{\dot{x}}}{\vec{x}}
\]

Using Equation @eq:mfront:FeFpImplicitPlasticity:F, the velocity
\(\tns{L}\) can be related to the time derivative of the deformation
gradient by chain rule:

\[
\tns{L}=\deriv{\vec{\dot{x}}}{\vec{X}}\,\cdot\,\deriv{\vec{X}}{\vec{x}}=\tns{\dot{F}}\,\cdot\,\inverse{\F}
\]{#eq:mfront:FeFpImplicitPlasticity:L_F}

The rate of deformation is the symmetric part of  is \(\tns{L}\):
\[
\D= \Frac{1}{2}\paren{\tns{L}+\transpose{\tns{L}}}
\]{#eq:mfront:FeFpImplicitPlasticity:D}

The time derivative of the change of volume, \(\dot{J}\), is given by:

\[
\dot{J}=J\,\trace{\D} \quad\Leftrightarrow\quad \derivtot{\ln\paren{J}}{t}=\trace{\D}=\trace{\L}
\]{#eq:mfront:FeFpImplicitPlasticity:dJdt}

This relation shows that there is no change of volume if the trace of
the rate of deformation \(\D\) is null.

> **Demonstration of Equation @eq:mfront:FeFpImplicitPlasticity:dJdt**
>
> Equation @eq:mfront:FeFpImplicitPlasticity:dJdt can be established by
> computing the following first order development of the determinant of
> the deformation gradient:
>
> \[
> \begin{aligned}
> \ets{J}&=\det\paren{\ets{\F}}
> =\det\paren{\bts{\F}+\Delta\,\F}
> =\det\paren{\bts{\F}+\Delta\,t\,\L\,\cdot\,\bts{\F}}\\
> &\approx\det\paren{\bts{\F}}\,\det\paren{\tenseur{I}+\Delta t\,\L}
> \approx\bts{J}\,\paren{1+\Delta t\,\trace{\D}}
> \end{aligned}
> \]

## Multiplicative decomposition of the deformation gradient

\[
\F = \Fe\,\cdot\, \Fp
\]{#eq:mfront:FeFpImplicitPlasticity:FeFp}

### Decomposition of velocity gradient

Using Equation @eq:mfront:FeFpImplicitPlasticity:FeFp, the velocity
gradient \(\L\) can be expressed as:

\[
\begin{aligned}
\L &= \tns{\dot{F}}\,\cdot\,\inverse{\F} = \paren{\dFedt\,\cdot\, \Fp+\Fe\,\cdot\, \dFpdt}\,\cdot\,\inverse{\F}
   &= \paren{\dFedt\,\cdot\, \Fp+\Fe\,\cdot\, \dFpdt}\,\cdot\,\inverse{\Fp}\cdot\,\inverse{\Fe}\\
   &= \dFedt\,\cdot\,\inverse{\Fe}+\Fe\,\cdot\, \dFpdt\cdot\,\inverse{\Fp}\cdot\,\inverse{\Fe}\\
   &= \Le+\Fe\,\cdot\,\Lp\,\cdot\,\inverse{\Fe}
\end{aligned}
\]

where:

- The elastic part \(\Le=\dFedt\,\cdot\,\inverse{\Fe}\) of the velocity
  gradient is defined in the current configuration \(C\).
- The plastic part of the velocity gradient \(\Lp=\dFpdt\,\cdot\,
  \inverse{\Fp}\) in the intermediate configuration \(C_{i}\).

## Expression of the mechanical power

The mechanical power per unit of volume in the current configuration
\(p_{v}\) is given by the contracted product of the Cauchy stress and
the deformation gradient.

Assuming the symmetry of the Cauchy stress, \(p_{v}\) can be developped
as follows:

\[
\tsigma\,\colon\,\D= 
\tsigma\,\colon\,\L = 
\tsigma\,\colon\,\Le+\tsigma\,\colon\,\paren{\Fe\,\cdot\,\Lp\,\cdot\,\inverse{\Fe}}
\]

The term \(\tsigma\,\colon\,\Le\) is assumed to be reversible and must
derive from an convex free energy \(\Psi\), as developped in Section
@sec:mfront:FeFpImplicitPlasticity:hyperelasticity, such that:

\[
\tsigma\,\colon\,\Le = \Frac{1}{J_{e}}\,\Se\,\colon\,\degl
\quad\text{with}\quad\Se=\rho_{e}\,\deriv{\Psi_{e}}{\egl}
\]

where \(\Se\) is the second Piola-Kirchhoff stress, \(\egl\) is the
Green-Lagrange strain, \(J_{e}\) is the determinant of the elastic part
of the deformation gradient and \(\rho_{e}\) the mass density in the
intermediate configuration.

The second term will be associated with the plastic dissipation in
Section @sec:mfront:FeFpImplicitPlasticity:MandelStress which introduces
the Mandel stress \(M\) such that:

\[
\tsigma\,\colon\,\D= 
\tsigma\,\colon\,\Le+\Frac{1}{J_{e}}\,\M\,\colon\,\Lp
\]{#eq:mfront:FeFpImplicitPlasticity:mechanical_power}

## Hyperelastic behaviour {#sec:mfront:FeFpImplicitPlasticity:hyperelasticity}

Let \(\Se\) be the second Piola-Kirchhoff stress in the intermediate
configuration, which is related to the Cauchy stress by:

\[
\tsigma = \Frac{1}{J_{e}}\,\Fe\,\cdot\,\Se\,\transpose{\Fe}
\]

Classically, \(\Se\) is conjugated to the Green-Lagrange strain
\(\egl\), i.e.:

\[
\tsigma\,\colon\,\D = \Frac{1}{J_{e}}\,\Se\,\colon\,\degl
\]{#eq:mfront:FeFpImplicitPlasticity:Se_degl}

The Green-Lagrange strain \(\egl\) is defined by:

\[
\egl=\Frac{1}{2}\paren{\Ce-\tenseur{I}}=\Frac{1}{2}\paren{\transpose{\Fe}\,\cdot\,\Fe-\tenseur{I}}
\]

where \(\Ce=\transpose{Fe}\,\cdot\,\Fe\) is the right Cauchy tensor in
the intermediate configuration.

> **Demonstration of Equation @eq:mfront:FeFpImplicitPlasticity:Se_degl**
>
> The time derivative of the Green-Lagrange strain is:
>
> \[
> \begin{aligned}
> \degl &= \Frac{1}{2}\,\paren{\dtFedt\,\cdot\,\Fe+\transpose{\Fe}\,\cdot\,\dFedt} \\
>       &= \Frac{1}{2}\,\transpose{\Fe}\,\cdot\,\paren{\itranspose{\Fe}\,\cdot\,\dtFedt+\dFedt\,\cdot\,\inverse{\Fe}}\,\cdot\,\Fe
>       &= \Frac{1}{2}\,\transpose{\Fe}\,\cdot\,\paren{\transpose{\Le}+\Le}\,\cdot\,\Fe
>       &= \transpose{\Fe}\,\cdot\,\De\,\cdot\,\Fe
> \end{aligned}
> \]
>
> where \(\De\) is the elastic rate of deformation, i.e. the symmetric part of the elastic part of the velocity gradient.
>
> This expression shows that \(\De\) is the pull-back of \(\degl\), while \(\degl\)
> is the push-forward of \(\Le\) [@belytschko_nonlinear_2000]:
>
> \[
> \De=\itranspose{\Fe}\,\cdot\,\Le\,\cdot\,\inverse{\Fe}
> \]
>
> Then:
>
> \[
> \begin{aligned}
> \tsigma\,\colon\,\De &=& \tsigma\,\colon\,\itranspose{\Fe}\,\cdot\,\degl\,\cdot\,\inverse{\Fe}
>                      &=& \trace{\tsigma\,\cdot\,\itranspose{\Fe}\,\cdot\,\degl\,\cdot\,\inverse{\Fe}}&                  \\
>                      &=& \trace{\Fe\,\cdot\,\inverse{\Fe}\,\cdot\,\tsigma\,\cdot\,\itranspose{\Fe}\,\cdot\,\degl\,\cdot\,\inverse{\Fe}}
>                      &=& \trace{\inverse{\Fe}\,\cdot\,\tsigma\,\cdot\,\itranspose{\Fe}\,\cdot\,\degl}& \\
>                      &=& \trace{\inverse{\Fe}\,\cdot\,\tsigma\,\cdot\,\itranspose{\Fe}\,\cdot\,\degl}
>                      &=& \Frac{1}{J_{e}}\,\Se\,\colon\,\degl&
> \end{aligned}
> \]
>
> where we used the classical property of similarity-invariance of the trace:
>
> \[
> \trace{B^{-1}\,\cdot\,A\,\cdot\,B} = \trace{A}
> \]{#eq:mfront:FeFpImplicitPlasticity:trace:similarity_invariance}
> 
> which is valid for any matrices \(A\) and \(B\).

The behaviour is hyperelastic if the the second Piola-Kirchhoff stress
derives from a convex free energy \(\Psi\), such that:

\[
\tsigma\,\colon\,\Le = \Frac{1}{J_{e}}\,\Se\,\colon\,\degl
\quad\text{with}\quad\Se=\rho_{e}\,\deriv{\Psi_{e}}{\egl}
\]

For metals, the Green-Lagrange strain \(\egl\) is generally small in
practice, the choice of the free energy \(\Psi\) generally have little
influence. The Saint-Venant Kirchhoff hyperelastic behaviour, which is
the simpliest generalisation of the Hooke behaviour at finite strain, is
thus generally choosen.

### The Saint-Venant Kirchhoff hyperelastic behaviour

The Saint-Venant Kirchhoff hyperelastic behaviour is given by:
\[
\Se=\tenseurq{D}\,\colon\,\egl
\]
where \(\tenseur{D}\) is the elastic stiffness matrix.

The Saint-Venant Kirchhoff hyperelastic behaviour derives from the
following free-energy:

\[
\rho_{e}\,\Psi_{e} = \Frac{1}{2}\egl\,\colon\,\tenseurq{D}\,\colon\,\egl
\]

## Plastic dissipation and Mandel stress {#sec:mfront:FeFpImplicitPlasticity:MandelStress}

The Mandel stress \(\M\) is defined as the dual of the plastic velocity
gradient \(\Lp\), i.e. as the stress measure which satisfies:

\[
\tsigma\,\colon\,\paren{\Fe\,\cdot\,\Lp\,\cdot\,\inverse{\Fe}} = \Frac{1}{J_{e}}\,\M\,\colon\Lp
\]

The Mandel stress \(\M\) is egal to (see demonstration below):

\[
\M = \Ce\,\cdot\,\Se = \paren{2\,\egl+\tns{I}}\,\cdot\,\,\Se
\]{#eq:mfront:FeFpImplicitPlasticity:MandelStress}

Note that the Mandel stress is not symmetric.

> **Demonstration of Equation @eq:mfront:FeFpImplicitPlasticity:MandelStress**
> 
> \[
> \begin{aligned}
> \tsigma\,\colon\,\paren{\Fe\,\cdot\,\Lp\,\cdot\,\inverse{\Fe}}
> &=& \Frac{1}{J_{e}}\,\trace{\Fe\,\cdot\,\Se\,\transpose{\Fe}\,\cdot\,\Fe\,\cdot\,\Lp\,\cdot\,\inverse{\Fe}}& \\
> &=& \Frac{1}{J_{e}}\,\trace{\Se\,\transpose{\Fe}\,\cdot\,\Fe\,\cdot\,\Lp}
> = \Frac{1}{J_{e}}\,\trace{\Se\,\Ce\,\cdot\,\Lp} 
> = \Frac{1}{J_{e}}\,\M\,\colon\Lp&
> \end{aligned}
> \]
>
> where we re-used Equation @eq:mfront:FeFpImplicitPlasticity:trace:similarity_invariance.

## Clausius-Duhem inequality

The Clausius-Duhem inequality can be written as follows:
\[
-\rho (\dot{e}\paren{\egl,T,\vec{Y}} - T\,\dot{s}) + \tsigma\,\colon\, \tenseur{D} - \Frac{\vec{q}}{T} \cdot \vec{\nabla} T \geqslant 0
\] {#eq:Clausius-Duhem}
where:

- \(\rho\) is the volumetric mass density.
- \(e\) is the internal energy density in the current configuration.
- \(T\) is the temperature.
- \(s\) is the entropy density in the current configuration.
- \(\vec{y}\) a set of internal state variables

Let \(\Psi = e - T\,s\) be the free energy in in the current
configuration. Using Equation
@eq:mfront:FeFpImplicitPlasticity:mechanical_power, one gets:

\[
\begin{aligned}
-\rho\,\paren{\dot{\Psi} - s\,\dot{T}} + \tsigma\,\colon\, \tenseur{D} - \Frac{\vec{q}}{T} \cdot \vec{\nabla} T &\geqslant 0 \\
-\rho\,\paren{\deriv{\Psi}{\egl}\,\colon\,\degl+\deriv{\Phi}{\vec{y}}\,\cdot\,\vec{\dot{y}}+\paren{\deriv{\Psi}{T} - s\,}\dot{T}} + \tsigma\,\colon\, \tenseur{D} - \Frac{\vec{q}}{T} \cdot \vec{\nabla} T &\geqslant 0\\
-\rho\,\paren{\paren{\deriv{\Psi}{\egl}-\Frac{1}{\rho\,J_{e}}\,\Se}\,\colon\,\degl+\paren{\deriv{\Psi}{T} - s\,}\dot{T}} -\rho\,\deriv{\Phi}{\vec{y}}\,\cdot\,\vec{\dot{y}}+\Frac{1}{J_{e}}\M\,\colon\,\Lp - \Frac{\vec{q}}{T} \cdot \vec{\nabla} T &\geqslant 0
\end{aligned}
\]

Classical arguments (considering a reversible process), leads to:

\[
\Se = \rho_{e}\,\deriv{\Psi}{\egl}
\quad\text{and}\quad
s = \deriv{\Psi}{T}
\]

where \(\rho_{e}\) is the density in the intermediate configuration
(\(\rho_{e}=\rho\,J_{e}\)).

Assuming that the dissipation du to heat transfer \(D_{T}\) and the
mechanical dissipation \(D_{m}\) are both positive, one gets:
\[
\begin{aligned}
D_{m}&=\vec{Y}\,\cdot\,\vec{\dot{y}}+\M\,\colon\,\Lp \geqslant 0\\
D_{T}&=-\Frac{\vec{q}}{T} \cdot \vec{\nabla} T \geqslant 0
\end{aligned}
\]
where \(\vec{Y} = -\rho_{e}\,\deriv{\Phi}{\vec{y}}\) are the thermodynamic
forces associated to the internal state variables \(\vec{y}\).

The mechanical dissipation is automatically positive if we introduce a
convex dissipation potential \(\phi\) minimal at zero such that:
\[
\Lp=\deriv{\phi}{\M} \quad\text{and}\quad \vec{\dot{y}}=\deriv{\phi}{\vec{Y}}
\]

# Examples

## Isotropic incompressible (visco)-plasticity

In this section, we consider two examples:

- An isotropic incompressible viscoplastic behaviour.
- An isotropic incompressible plastic behaviour.

Those examples shares the same hyperelastic behaviour and the same
stress criterion.

### Hyperelastic behaviour

The simpliest isotropic hyperelastic behaviour is the Saint
Venant-Kirchhoff law:

\[
\Se = \lambda\,\trace{\egl}\,\tenseur{I}+2\,\,\mu\,\egl
\]

where \(\lambda\) and \(\mu\) are respectively the first and second Lamé
coefficients.

### Choice of the stress criterion

Du to isotropy, the dissipation potential must be a function of the
invariants of the Mandel stress:

- the trace of the Mandel stress, \(I_{1}\).
- the von Mises norm of the Mandel stress, \(\Meq\):
  \[
  \Meq=\sqrt{\Frac{3}{2}\,\Mdev\,\colon\,\Mdev}.
  \]
  where \(\Mdev\) is the deviatoric part of the Mandel stress.
- the determinant of the deviator of the Mandel stress.

Since the plastic flow is assumed to be isochoric, \(L_{p}\) must be
traceless, so the dissipation potentiel can't be a function of \(I_{1}\).

In this section, we assume that the behaviour oly depends on \(\Meq\).
Thus,

\[
\Lp=\deriv{\phi}{\Meq}\,\deriv{\Meq}{\M}=\deriv{\phi}{\Meq}\,\paren{\Frac{3}{2\,\Meq}\,\Mdev}=\deriv{\phi}{\Meq}\,\N
\]

where \(\N=\Frac{3\,\Mdev}{2\,\Meq}\) is the normal.

### Isotropic incompressible viscoplasticty {#sec:mfront:FeFpImplicitPlasticity:J2_viscoplasticity}

\[
\phi = \Frac{M_{0}\,\dot{\varepsilon}_{0}}{n+1}\,\paren{\Frac{\Meq}{M_{0}}}^{n+1}
\]

### Isotropic incompressible plasticity {#sec:mfront:FeFpImplicitPlasticity:J2_plasticity}

The yield surface of a plastic behaviour with linear isotropic hardening is given by:
\[
f = \Meq-M_{0}-H\,\paren{p}
\]
where \(p\) is the equivalent plastic strain, \(M_{0}\) is the yield
stress and \(H\) is the hardening slope.

## Single crystal plasticity {#sec:mfront:FeFpImplicitPlasticity:single_crystal}

It is assumed that the crystal deforms by gliding of dislocations on a
set of \(N_{s}\) prescribed cristallographic planes called *slip
planes*, also called *slip systems* or *gliding systems*.

A slip plane is characterized by its normal \(\ns\) and a slip direction
\(\ms\) in the plane (\(s\,\in\,\left[1, N_{s}\right]\)). By
definition, the vectors are \(\ns\) and \(\ms\) are orthogonal. A slip
plane is also characterized by its orientation tensor \(\Ns\) such that:
\[
\Ns = \ms\,\otimes\,\ns
\]

The gliding of dislocations leave the crystal orientations unchanged, so
the orientation tensors are the same in the initial configuration
\(C_{0}\) and in the intermediate configuration \(C_{i}\).

Finally, the plastic part of the velocity gradient is given by the sum
of the glidings on all gliding planes, as follows:
\[
\Lp = \sum_{s}^{N_{s}} \dot{\gamma}_{s}\,\Ns
\]
where \(\dot{\gamma}_{s}\) is the slip rate of the \(s^{\mathrm{th}}\)
plane.

This plastic flow is isochoric, as:
\[
\trace{\Lp}=\sum_{s}^{N_{s}} \dot{\gamma}_{s}\,\trace{\Ns}=\sum_{s}^{N_{s}} \dot{\gamma}_{s}\,\ms\,\cdot\,\ns=0
\]

The expression of the plastic dissipation naturally introduces the
resolved shear stresses \(\tau^{s}\):
\[
\M\,\colon\Lp=\sum_{s}^{N_{s}} \dot{\gamma}_{s}\,\tau^{s}
\]
with \(\tau^{s}=\M\,\colon\,\Ns\).

### A simple dissipation potential for crystal plasticity

A simple dissipation potential for the single crystal can be defined as follows:
\[
\phi\paren{\tenseur{\M}}=
\sum_{s}^{N_{s}} \phi^{s}\paren{\left|\tau^{s}\right|}
\]
where each function \(\phi^{s}\) is assumed to be convex and minimal at zero.

This dissipation potential leads to:
\[
\Lp = \sum_{s}^{N_{s}} \deriv{\phi^{s}}{\left|\tau^{s}\right|}\,\deriv{\left|\tau^{s}\right|}{\tau^{s}}\,\deriv{\tau^{s}}{\M} 
    = \sum_{s}^{N_{s}} \mathop{sgn}\paren{\tau^{s}}\,\deriv{\phi^{s}}{\left|\tau^{s}\right|}\,\deriv{\tau^{s}}{\M}
    = \sum_{s}^{N_{s}} \mathop{sgn}\paren{\tau^{s}}\,\deriv{\phi^{s}}{\left|\tau^{s}\right|}\,\Ns
\]
where \(\mathop{sgn}\paren{\tau^{s}}\) denotes the sign function:
\[
\mathop{sgn}\paren{x}=
\left\{
\begin{aligned}
 1 & \, & \text{if} \,& x \geq 0 \\
-1 & \, & \text{if} \,& x <  0 \\
\end{aligned}
\right.
\]

A classical choice for \(\phi^{s}\) is a power-law:
\[
\phi^{s}\paren{\left|\tau^{s}\right|} = \Frac{A}{n+1} \left|\tau^{s}\right|^{n+1}
\]

# Numerical implementation

## Implicit equations

The decompositions of the deformation gradient at the beginning of the
time step, denoted \(\bts{\F}\), and at the end of the time step,
denoted \(\bts{\F}\) are respectively \( \bts{\F} =
\bts{\Fe}\,\cdot\,\bts{\Fp}\) and \(\ets{\F} =
\ets{\Fe}\,\cdot\,\ets{\Fp}\).

Using those decompositions, the elastic part of the deformation gradient
at the end of the time step can expressed as:
\[
\ets{\Fe} = \Fe^{tr}\,\cdot\,\bts{\Fp}\,\cdot\,\pinverse{\ets{\Fp}}
\]
where \(\Fe^{tr}\) denotes the value of the elastic part of the
deformation gradient if no (visco-)plastic flow happens:
\[
\Fe^{tr} = \ets{\F}\,\cdot\,\pinverse{\bts{\Fp}}
\]

The plastic part of the velocity gradient can be approximated by the
following expression:
\[
\Lp\approx\Frac{1}{\Delta\,t}\,\paren{\ets{\Fp}-\bts{\Fp}}\,\cdot\,\pinverse{\ets{\Fp}}=
\Frac{1}{\Delta\,t}\,\paren{\tns{I}-\DsFp}
\]{#eq:mfront:FeFpImplicitPlasticity:Lp:forward_euler}
where \(\DsFp\) denotes the product
\(\bts{\Fp}\,\cdot\,\pinverse{\ets{\Fp}}\).

> **Alternative treatment of the plastic part of the velocity gradient**
>
> Expression @eq:mfront:FeFpImplicitPlasticity:Lp:forward_euler is
> convenient, but does not ensure the incompressiblity condition given by:
> \[
> \det\paren{\ets{\Fp}} = 1
> \]
>
> Using an exponential mapping does ensure this condition, but is much
> more computationaly intensive and will not be considered in this
> document.

\(\DsFp\) can be directly obtained by the constitutive equations. For
example, in the viscoplastic case treated in Section
@sec:mfront:FeFpImplicitPlasticity:J2_viscoplasticity or in the case of
simple single crystal behaviour described in Section
@sec:mfront:FeFpImplicitPlasticity:single_crystal, \(\DsFp\) is given
by:
\[
\DsFp=\tns{I}-\Delta\,t\,\ets{\deriv{\phi}{\M}}
\]

This expression shows that \(\DsFp\) is a function of the Mandel stress
\(\ets{\M}\) which is a function of the Green-Lagrange strain
\(\ets{\egl}\). The elastic part of the deformation gradient at the end
of the time step can now be expressed as:
\[
\ets{\Fe} = \Fe^{tr}\,\cdot\,\DsFp
\]

Hence, \(\ets{\Fe}\) is a function of the the Green-Lagrange strain
\(\ets{\egl}\) and can also be used to compute the right Cauchy-Green
tensor \(\ets{\Ce}\), which can be used to compute the Green-Lagrange
strain \(\ets{\egl}\). Finally, the Green-Lagrange strain \(\ets{\egl}\)
satisfies this non linear equation:
\[
\bts{\egl}+\Delta\,\egl=\Frac{1}{2}\,\paren{\ets{\Ce}\paren{\ets{\Fe}\paren{\ets{\DsFp\paren{\ets{\egl}}}}}-\tenseur{I}}
\]{#eq:mfront:FeFpImplicitPlasticity:egl_eq}

### Plastic case

The equation allowing to determine is value of the increment of the
equivalent plastic strain is to impose that the stress state lies on the
yield surface in case of plastic loading:
\[
\ets{M_{eq}} = M_{0} + H\,\paren{p+\Delta\,p}
\]{#eq:mfront:FeFpImplicitPlasticity:Meq}

## Resolution by a Newton algorithm. Residual and jacobian matrix

Equation @eq:mfront:FeFpImplicitPlasticity:egl_eq, combined with
Equation @eq:mfront:FeFpImplicitPlasticity:Meq in the plastic case, must
be solved numerically to find the increment of the Green-Lagrange strain
\(\Delta\,\egl\), and the increment of the equivalent plastic strain
\(\Delta\,p\) in the plastic case.

This resolution is generally based on the iterative Newton algorithm to
find the zero of a function called the residual \(\vec{R}\). Let
\(\Delta\,\vec{Y}^{n}\) be a vector containing the current estimate of
the increment of the integration variables (the Green-Lagrange strain in
the viscoplatic case, the the Green-Lagrange strain and the equivalent
plastic strain in the plastic case).

A new estimate \(\Delta\,\vec{Y}^{n+1}\) is assumed to close be the solution searched, i.e. that:
\[
\vec{R}\paren{\Delta\,\vec{Y}^{n+1}}\approx \vec{0}
\]

Taking the first order expansion of \(R\) around \(\Delta\,\vec{Y}^{n}\) leads to:
\[
\vec{R}\paren{\Delta\,\vec{Y}^{n+1}}
\approx \vec{R}\paren{\Delta\,\vec{Y}^{n}}+J\,\cdot\,\paren{\Delta\,\vec{Y}^{n+1}-\Delta\,\vec{Y}^{n}}
\approx \vec{0}
\]
where \(J\) is the jacobian matrix. Assuming that \(J\) is invertible, \(\Delta\,\vec{Y}^{n+1}\) is given by:
\[
\Delta\,\vec{Y}^{n+1}=\Delta\,\vec{Y}^{n}-J^{-1}\,\cdot\,\vec{R}\paren{\Delta\,\vec{Y}^{n}}
\]

### Viscoplastic case

The only integration variable in the viscoplastic case is the
Green-Lagrange strain \(\egl\). The residual associated with this
equation, denoted \(f_{\egl}\) using standard `MFront` notations, is
directly derived from Equation @eq:mfront:FeFpImplicitPlasticity:egl_eq:
\[
f_{\egl} = \bts{\egl}+\Delta\,\egl-\Frac{1}{2}\,\paren{\ets{\Ce}-\tenseur{I}}
\]
where we dropped the dependencies of \(\Ce\) for the sake of clarity.

The derivative of \(f_{\egl}\) with respect to the increment of the
Green-Lagrange strain is given by:
\[
\deriv{f_{\egl}}{\Delta\,\egl}=\tenseur{I}-\Frac{1}{2}\deriv{\ets{\Ce}}{\ets{\Fe}}\,\cdot\,\deriv{\ets{\Fe}}{\Delta\,\egl}
\]

The derivative \(\deriv{\ets{\Fe}}{\Delta\,\egl}\), which will also be
useful for the computation of the consistent tangent operator, is given
by:
\[
\deriv{\ets{\Fe}}{\Delta\,\egl}=\deriv{\ets{\Fe}}{\DsFp}\,\cdot\,\deriv{\DsFp}{\ets{\M}}\,\cdot\,\deriv{\ets{\M}}{\ets{\egl}}\,\cdot\,\deriv{\ets{\egl}}{\Delta\,\egl}
\]
where
\(\deriv{\DsFp}{\ets{\M}}=-\Delta\,t\,\ets{\secondderivative{\phi}{\M}}\),
\(\deriv{\ets{\M}}{\ets{\egl}}=\tenseurq{D}\) and
\(\deriv{\ets{\egl}}{\Delta\,\egl}=\tenseur{I}\). The derivative
\(\deriv{\ets{\Ce}}{\ets{\Fe}}\) can be computed by the `t2tost2::dCdF`
function. Finally, the derivative \(\deriv{\ets{\Fe}}{\DsFp}\) can be
computed with the `t2tot2::tprd` method[^tprd].

[^tprd]: tprd means "tensor product right derivative" and computes the derivative \(\deriv{}{\tns{B}}\paren{\tns{A}\,\cdot\,\tns{B}}\).

### Plastic case

The integration variables in the plastic case are the Green-Lagrange
strain \(\egl\) and the equivalent plastic strain increment \(p\):
\[
\vec{Y}=
\begin{pmatrix}
\egl\\
p
\end{pmatrix}
\]

The residual \(\vec{R}\) can be decomposed by blocks as follows:
\[
\vec{R}\paren{\vec{Y}}=
\begin{pmatrix}
f_{\egl}\\
f_{p}
\end{pmatrix}
\]

where \(f_{\egl}\) and \(f_{p}\) can be deduced from Equations
@eq:mfront:FeFpImplicitPlasticity:egl_eq, combined with Equation
@eq:mfront:FeFpImplicitPlasticity:Meq as follows:
\[
\left\{
\begin{aligned}
f_{\egl} &= \bts{\egl}+\Delta\,\egl-\Frac{1}{2}\,\paren{\ets{\Ce}-\tenseur{I}} \\
f_{p}    &=  \Frac{1}{E}\,\paren{M_{eq} - M_{0} - H\,\paren{p+\Delta\,p}}
\end{aligned}
\right.
\]
where normalisation factor \(\Frac{1}{E}\) has be introduced so that
\(f_{p}\) has the same magnitude as \(f_{\egl}\). \(E\) is generally
choosen to be Young' modulus for isotropic behaviours and the first
component of the siffness matrix for orthotropic behaviours.

## Consistent tangent operator

This section is devoted to the computation of the derivative of the
Kirchhoff stress with respect to the deformation gradient. Alternative
consistent tangent operators are described in Appendix
@sec:mfront:FeFpImplicitPlasticity:consistent_tangent_operators.

The Kirchhoff stress \(\ets{\tau}\) is defined as the push-forward of the
second Piola-Kirchhoff stress \(\ets{\Se}\):
\[
\ets{\tau}=\ets{\Fe}\,\cdot\,\ets{\Se}\,\cdot\,\transpose{\ets{\Fe}}
\]
with:
\[
\ets{\Se}=\Frac{1}{2}\,\tenseurq{D}\,\colon\,\paren{\Ce-\tns{I}}
\]

This definition of \(\ets{\tau}\) differs from the one used in the
previous section since we directly uses \(\Fe\) to compute \(\Se\)
rather than \(\egl\).

The consistent tangent operator is then:
\[
\begin{aligned}
\deriv{\ets{\tenseur{\tau}}}{\ets{\F}}
&=\paren{
\deriv{\ets{\tenseur{\tau}}}{\ets{\Se}}\,\cdot\,\deriv{\ets{\Se}}{\ets{\Fe}}+
\deriv{\ets{\tenseur{\tau}}}{\ets{\Fe}}
}\,\cdot\,\deriv{\ets{\Fe}}{\ets{\F}}\\
&=\paren{
\Frac{1}{2}\deriv{\ets{\tenseur{\tau}}}{\ets{\Se}}\,\cdot\,\tenseurq{D}\,\cdot\,\deriv{\ets{\Ce}}{\ets{\Fe}}+
\deriv{\ets{\tenseur{\tau}}}{\ets{\Fe}}
}\,\cdot\,\deriv{\ets{\Fe}}{\ets{\F}}
\end{aligned}
\]
with:
\[
\deriv{\ets{\Fe}}{\ets{\F}}=
\partial_{\star}^{l}\paren{\DsFp}\,\cdot\,\partial_{\star}^{l}\paren{\bts{\Fp^{-1}}}+
\partial_{\star}^{r}\paren{\Fe^{tr}}\,\cdot\,\deriv{\DsFp}{\Delta\,\vec{Y}}\,\cdot\,\deriv{\Delta\,\vec{Y}}{\ets{\F}}
\]

# Plastic behaviour' implementation

~~~~{.cxx}
@DSL ImplicitFiniteStrain;
@Behaviour ImplicitFeFpPlasticity;
@Author Thomas Helfer;
@Date 13 / 09 / 2021;
~~~~

~~~~{.cxx}
@Algorithm NewtonRaphson;
@Epsilon 1.e-14;
@Theta 1;
~~~~

~~~~{.cxx}
@ElasticMaterialProperties {150e9, 0.3};
~~~~

~~~~{.cxx}
@Parameter stress M0 = 100e6;
@Parameter stress H = 2e6;
~~~~

~~~~{.cxx}
@IntegrationVariable StrainStensor eel;
~~~~

~~~~{.cxx}
@StateVariable strain p;
p.setGlossaryName("EquivalentPlasticStrain");
@AuxiliaryStateVariable DeformationGradientTensor Fp;
Fp.setEntryName("PlasticPartOfTheDeformationGradient");
~~~~

~~~~{.cxx}
@LocalVariable DeformationGradientTensor Fe_tr;
@LocalVariable DeformationGradientTensor iFp;
@LocalVariable DeformationGradientTensor idFp;
@LocalVariable StiffnessTensor De;
@LocalVariable st2tot2<N,real> didFp_ddeel;
@LocalVariable tensor<N,real> n;
@LocalVariable bool b_loading;
~~~~

~~~~{.cxx}
@InitializeLocalVariables {
  constexpr auto id = Stensor::Id();
  constexpr auto uid = Tensor::Id();
  De = lambda * Stensor4::IxI() + 2 * mu * Stensor4::Id();
  iFp = invert(Fp);
  const auto Fe = F0 * iFp;
  eel = computeGreenLagrangeTensor(Fe);
  Fe_tr = F1 * iFp;
  const auto eel_tr = computeGreenLagrangeTensor(Fe_tr);
  const auto S_tr = De * eel_tr;
  const auto M_tr = eval((id + 2 * eel_tr) * S_tr);
  const auto Mdev_tr = M_tr - trace(M_tr) / 3 * uid;
  const auto Meq_tr = sqrt(3 * (Mdev_tr | Mdev_tr) / 2);
  b_loading = Meq_tr > M0 + H * p;
  if (!b_loading) {
    idFp = uid;
    didFp_ddeel = st2tot2<N, real>{0};
  }
}
~~~~

~~~~{.cxx}
@Integrator {
  if (!b_loading) {
    const auto egl = computeGreenLagrangeTensor(Fe_tr);
    feel = eel + deel - egl;
    dfeel_ddeel = Stensor4::Id();
    return true;
  }
  constexpr auto id = Stensor::Id();
  constexpr auto uid = Tensor::Id();
  constexpr const auto eeps = real(1.e-14);
  const auto seps = young * eeps;
  const auto eel_ets = eel + deel;
  const auto S = De * eel_ets;
  const auto M = eval((id + 2 * eel_ets) * S);
  const auto dM_ddeel =
      st2tot2<N, real>{2 * st2tot2<N, real>::tpld(S) +
                       st2tot2<N, real>::tprd(id + 2 * eel_ets, De)};
  const auto Mdev = M - trace(M) / 3 * uid;
  const auto Meq = sqrt(3 * (Mdev | Mdev) / 2);
  const auto iMeq = 1 / max(Meq, seps);
  n = 3 * Mdev * (iMeq / 2);
  idFp = uid - dp * n;
  // current estimate of the elastic part of the deformation gradient at the end
  // of the time step
  const auto Fe = Fe_tr * idFp;
  const auto egl = computeGreenLagrangeTensor(Fe);
  feel = eel + deel - egl;
  // jacobian
  const auto degl_didFp =
      t2tost2<N, real>::dCdF(Fe) * t2tot2<N, real>::tprd(Fe_tr) / 2;
  const auto uM = (3 * t2tot2<N, real>::K()) / 2;
  const auto dn_dM = iMeq * (uM - (n ^ n));
  const auto didFp_dM = - dp * dn_dM;
  didFp_ddeel = didFp_dM * dM_ddeel;
  dfeel_ddeel -= degl_didFp * didFp_ddeel;
  dfeel_ddp = degl_didFp * n;
  // plastic part
  fp = (Meq - M0 - H * (p + dp)) / young;
  dfp_ddeel = (n | dM_ddeel) / young;
  dfp_ddp = -H / young;
}
~~~~

~~~~{.cxx}
@ComputeFinalStress {
  const auto Fe = Fe_tr * idFp;
  const auto S = De * eel;
  sig = convertSecondPiolaKirchhoffStressToCauchyStress(S,Fe);
}
~~~~

~~~~{.cxx}
@UpdateAuxiliaryStateVariables {
  Fp = eval(invert(idFp) * Fp);
  Fp /= cbrt(det(Fp));
}
~~~~

~~~~{.cxx}
@TangentOperator<DTAU_DF> {
  const auto Fe = Fe_tr * idFp;
  const auto S = De * eel;
  const auto dCe_dFe = t2tost2<N, real>::dCdF(Fe);
  const auto dS_dFe = De * dCe_dFe / 2;
  t2tost2<N, stress> dtau_dFe;
  computePushForwardDerivative(dtau_dFe, dS_dFe, S, Fe);
  // now we need dFe_dF
  const auto dFe_tr_dF = t2tot2<N, real>::tpld(iFp);
  dfeel_ddF = -dCe_dFe * t2tot2<N, real>::tpld(idFp, dFe_tr_dF) / 2;
  auto ddeel_dF = t2tost2<N, real>{};
  auto ddp_dF = tensor<N, real>{};
  getIntegrationVariablesDerivatives_F(ddeel_dF, ddp_dF);  //
  const auto didFp_dF = didFp_ddeel * ddeel_dF - (n ^ ddp_dF);
  const auto dFe_dF = t2tot2<N, real>::tpld(idFp, dFe_tr_dF) +
                      t2tot2<N, real>::tprd(Fe_tr, didFp_dF);
  Dt = dtau_dFe * dFe_dF;
}
~~~~

<!--
# The `FiniteStrainSingleCrystal` brick

The previous framework allows the implementation of a wide range of
finite strain single crystals that differ from their flow rules and
the constitutive equations used to describe their internal state
variables' evolution.

The `FiniteStrainSingleCrystal` brick hides all the details related to
this framework and simplifies the declaration of the slip systems and
the interaction matrix.

## Variables automatically defined

The plastic slip \(g\) are automatically defined.

## Implementation of the implicit system

Le second tenseur de Piola-Kirchhoff est défini par la loi élastique de
Saint-Venant Kirchhoff: \[
\tenseur{S}=\tenseurq{D}\,.\,\paren{\bts{\tepsilonel}+\Delta\,\tepsilonel}
\]

Nous calculons (une approximation) de l'inverse de l'incrément de la
partie plastique du gradient de la transformation: \[
\paren{\Delta\,\Fp}^{-1} = \tns{I}-\sum_{i=1}^{12}\Delta\,g_{i}\,\tns{\mu}_{i}
\]

Nous calculons la partie élastique de gradient de la transformation
\(\Fe\) en fin de pas de temps:

\[
\Fe=\Fe^{\mathrm{tr}}\,\cdot\,\paren{\Delta\,\Fp}^{-1}
\]

L'équation associée à l'incrément de déformation élastique impose
qu'elle soit égale au tenseur de Green-Lagrange
\(\tepsilonel_{\mathrm{GL}}\) associé à la partie élastique \(\Fe\) du
gradient de la transformation:

\[
\bts{\tepsilonel}+\Delta\,\bts{\tepsilonel}-
\tepsilonel_{\mathrm{GL}}\paren{\Fe}=0
\]

Le calcul des termes \(\deriv{f_{\tepsilonel}}{\Delta\,g_{i}}\) est assez
technique et devra se faire en plusieurs étapes:

\[
\deriv{\tenseur{\varepsilon_{\mathrm{GL}}}}{\Delta\,g_{i}}=
\Frac{1}{2}\,\deriv{\tenseur{C}_{e}}{\Delta\,g_{i}}=
\Frac{1}{2}\,\deriv{\tenseur{C}_{e}}{\Fe}\,.\,
\deriv{\Fe}{\Delta\,g_{i}}
\]

La classe `t2tost2` fournit une méthode statique permettant de calculer la
dérivée du tenseur de Cauchy élastique \(\tenseur{C}_{e}\) par rapport au
gradient de la transformation élastique \(F_{e}\).

Il faut alors calculer le terme \(\deriv{\Fe}{\Delta\,g_{i}}\). Le
tenseur \(\Fe\) est le produit \(\star\) des représentation
matricielle de deux tenseurs:
\(\Fe^{\mathrm{tr}}\) et le tenseur
\(\paren{\Delta\,\Fp}^{-1}\).

Cette démarche est maintenant appliquée au calcul de la dérivée du
tenseur \(\Fe\): \[
\deriv{\Fe}{\paren{\Delta\,\Fp}^{-1}} =
\partial^{r}_{\star}\paren{\Fe^{\mathrm{tr}}}
\]

En négligeant la correction plastique, la dérivée
\(\deriv{\paren{\Delta\,\Fp}^{-1}}{\Delta\,g_{i}}\) s'écrit
simplement: \[
\deriv{\paren{\Delta\,\Fp}^{-1}}{\Delta\,g_{i}} = -\tns{\mu}_{i}
\]

Nous obtenons finalement l'expression la dérivée
\(\deriv{f}{\Delta\,g_{i}}\): \[
\deriv{f_{\tepsilonel}}{\Delta\,g_{i}}=
-\Frac{1}{2}\,\deriv{\tenseur{C}_{e}}{\Fe}\,.\,
\partial^{r}_{\star}\paren{\Fe^{\mathrm{tr}}}\,.\,
\tns{\mu}_{i}
\]

## Code blocks automatically declared

The `ComputeFinalStress` code block is automatically declared.

The `TangentOperator` code block is automatically declared.

## Variables automatically computed

The residual \(f_{\tepsilonel}\) associated with the elastic strain
\(\tepsilonel\) is automatically implemented.

The Mandel stress \(\tns{M}\) is automatically computed before the
integrator code block. Its value is stored in the variables `M`.  If
the algorithm requires an analytic jacobian, the derivative
\(\deriv{\tns{M}}{\Delta\,\tepsilonel}\) is also computed and stored
in the variable `dM_ddeel` respectively.
-->

\appendix

# Alternative consistent tangent operators{#sec:mfront:FeFpImplicitPlasticity:consistent_tangent_operators}

## Another expression of the derivative of the Kirchhoff stress with respect ot the deformation gradient {#sec:mfront:FeFpImplicitPlasticity:dtau_dF}

The Kirchhoff stress \(\ets{\tau}\) is defined as the push-forward of the
second Piola-Kirchhoff stress \(\ets{\Se}\):
\[
\ets{\tau}=\ets{\Fe}\,\cdot\,\ets{\Se}\,\cdot\,\transpose{\ets{\Fe}}
\]
f
his section is devoted to the computation of the derivative
\(\deriv{\ets{\tenseur{\tau}}}{\ets{\F}}\) which can be obtained by the
chain rule, as follows:
\[
\deriv{\ets{\tenseur{\tau}}}{\ets{\F}}=
\deriv{\ets{\tenseur{\tau}}}{\ets{\Se}}\,\cdot\,\deriv{\ets{\Se}}{\ets{\F}}+
\deriv{\ets{\tenseur{\tau}}}{\ets{\Fe}}\,\cdot\,\deriv{\ets{\Fe}}{\ets{\F}}
\]
where:

- \(\deriv{\ets{\tenseur{\tau}}}{\ets{\Se}}\) is computed assuming that
  \(\Fe\) is constant. This derivative can be computed using the
  `computePushForwardDerivative` function.
- \(\deriv{\ets{\tenseur{\tau}}}{\ets{\Fe}}\) is computed assuming that
  \(\Se\) is constant. This derivative can be computed using the
  `computePushForwardDerivativeWithRespectToTheDeformationGradient`
  function.

The derivative \(\deriv{\ets{\Se}}{\ets{\F}}\) is given by:
\[
\deriv{\ets{\Se}}{\ets{\F}}=\deriv{\ets{\Se}}{\ets{\egl}}\,\cdot\,\deriv{\ets{\egl}}{\ets{\F}}
=\tenseurq{M}\,\cdot\,\deriv{\Delta\,\egl}{\ets{\F}}
\]

The derivative \(\deriv{\ets{\Fe}}{\ets{\F}}\) is given by:
\[
\deriv{\ets{\Fe}}{\ets{\F}}=
\partial_{\star}^{l}\paren{\DsFp}\,\cdot\,\deriv{\Fe^{tr}}{\ets{\F}}+
\partial_{\star}^{r}\paren{\Fe^{tr}}\,\cdot\,\deriv{\DsFp}{\Delta\,\vec{Y}}\,\cdot\,\deriv{\Delta\,\vec{Y}}{\ets{\F}}
\]
where the tensors \(\partial_{\star}^{l}\paren{\DsFp}\) and
\(\partial_{\star}^{r}\paren{\Fe^{tr}}\) can be computed by the
`t2tot2::tpld` and `t2tot2::tprd` respectively[^tpld].

The derivative \(\deriv{\Fe^{tr}}{\ets{\F}}\) is given by \(\partial_{\star}^{l}\paren{\bts{\Fp^{-1}}}\), so:
\[
\deriv{\ets{\Fe}}{\ets{\F}}=
\partial_{\star}^{l}\paren{\DsFp}\,\cdot\,\partial_{\star}^{l}\paren{\bts{\Fp^{-1}}}+
\partial_{\star}^{r}\paren{\Fe^{tr}}\,\cdot\,\deriv{\DsFp}{\Delta\,\vec{Y}}\,\cdot\,\deriv{\Delta\,\vec{Y}}{\ets{\F}}
\]

[^tpld]: tpld means "tensor product left derivative" and computes the derivative \(\deriv{}{\tns{A}}\paren{\tns{A}\,\cdot\,\tns{B}}\).

Finally, the last derivative to be computed is
\(\deriv{\Delta\,\egl}{\ets{\F}}\). This derivative can be computed
using the implicit function theorem as follow:
\[
J\,\deriv{\Delta\,\vec{Y}}{\ets{\F}}=-\deriv{\vec{R}}{\ets{\F}}
\]

where \(\deriv{\vec{R}}{\ets{F}}\) must be computed assuming that the
internal state variables are constant constant. Only the block
\(\deriv{f_{\egl}}{\ets{F}}\) is not null and is given by:
\[
\deriv{f_{\egl}}{\ets{F}}=
-\Frac{1}{2}\,\deriv{\ets{\Ce}}{\ets{\Fe}}\,\cdot\,\deriv{\ets{\Fe}}{\ets{\F}}=
-\Frac{1}{2}\,\deriv{\ets{\Ce}}{\ets{\Fe}}\,\cdot\,\partial_{\star}^{l}\paren{\DsFp}\,\cdot\,\partial_{\star}^{l}\paren{\bts{\Fp^{-1}}}
\]

## Derivative of the second Piola-Kirchhoff stress in the initial configuration with respect to the right Cauchy tensor in the initial configuration

This paragraph computes the derivative \(\deriv{\ets{\Sr}}{\ets{\Cr}}\)
of the second Piola-Kirchhoff stress \(\ets{\Sr}\) in the initial
configuration with respect to the right Cauchy tensor in the initial
configuration \(\ets{\Cr}=\ets{\transpose{\F}}\,\cdot\,\ets{\F}\).

The second Piola-Kirchhoff stress \(\ets{\Sr}\) can be computed by the
pull-back of the Kirchoff stress \(\ets{\tenseur{\tau}}\):
\[
\begin{aligned}
\ets{\Sr}
&=\ets{\inverse{\F}}\,\cdot\,\ets{\tenseur{\tau}}\,\cdot\,\ets{\itranspose{\F}} \\
&=\ets{\inverse{\F}}\,\cdot\,\ets{\Fe}\,\cdot\,\ets{\Se}\,\cdot\,\transpose{\ets{\Fe}}\,\cdot\,\ets{\itranspose{\F}}\\
&=\ets{\inverse{\Fp}}\,\cdot\,\ets{\inverse{\Fe}}\,\cdot\,\ets{\Fe}\,\cdot\,\ets{\Se}\,\cdot\,\transpose{\ets{\Fe}}\,\cdot\,\ets{\itranspose{\Fe}}\,\cdot\,\ets{\itranspose{\Fp}}\\
&=\ets{\inverse{\Fp}}\,\cdot\,\ets{\Se}\,\cdot\,\ets{\itranspose{\Fp}} \\
\end{aligned}
\]

This relationship shows that \(\Se\) can be identified to the Kirchhoff
stress in the intermediate configuration and that \(\Sr\) is the
pull-back of \(\Se\) by the plastic part of the deformation gradient
\(\ets{\Fp}\).

\[
\deriv{\ets{\Sr}}{\ets{\Cr}}=
\deriv{\ets{\Sr}}{\ets{\Se}}\,\cdot\,\deriv{\Se}{\Cr}+
\deriv{\ets{\Sr}}{\ets{\inverse{\Fp}}}\,\cdot\,\deriv{\inverse{\Fp}}{\Cr}
\]
where:

- \(\deriv{\ets{\Sr}}{\ets{\Se}}\) is computed assuming that \(\inverse{\Fp}\) is constant.
- \(\deriv{\ets{\Sr}}{\inverse{\Fp}}\) is computed assuming that \(\ets{\Se}\) is constant.

\[
\deriv{\Se}{\Cr}=\Frac{1}{2}\,\tenseurq{D}\,\cdot\,\deriv{\Ce}{\Cr}
\]

\(\Ce\) satisfies:
\[
\begin{aligned}
\Ce
&=\ets{\transpose{\Fe}}\,\cdot\,\ets{\Fe}
=\ets{\itranspose{\Fp}}\,\cdot\,\ets{\transpose{\F}}\,\cdot\,\ets{\F}\,\cdot\,\ets{\inverse{\Fp}}\\
&=\ets{\itranspose{\Fp}}\,\cdot\,\ets{\Cr}\,\cdot\,\ets{\inverse{\Fp}}
\end{aligned}
\]

Its derivative requires one again to compute
\(\deriv{\inverse{\Fp}}{\ets{\Cr}}\) which can be related to the derivative
\(\deriv{\DsFp}{\ets{\Cr}}\) since the relation:
\[
\inverse{\ets{\Fp}}= \bts{\inverse{\Fp}}\,\cdot\,\DsFp
\]
implies:
\[
\deriv{\inverse{\Fp}}{\ets{\Cr}}=\partial_{\star}^{r}\paren{\bts{\inverse{\Fp}}}\,\cdot\,\deriv{\DsFp}{\ets{\Cr}}
\]

The computation of \(\deriv{\DsFp}{\ets{\Cr}}\) is similar to the
computation of \(\deriv{\DsFp}{\ets{F}}\) in Section
@sec:mfront:FeFpImplicitPlasticity:dtau_dF:
\[
\deriv{\DsFp}{\ets{\Cr}}=\deriv{\DsFp}{\vec{Y}}\,\cdot\,\deriv{\vec{Y}}{\ets{\Cr}}
\]


# References

<!-- Local IspellDict: english -->
