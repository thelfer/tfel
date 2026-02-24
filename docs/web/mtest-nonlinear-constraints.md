---
title: On physical interpretations of Lagrange multipliers for kinematic and static constraints in MTest
author: Mehran Ghasabeh, Thomas Helfer, Thomas Nagel, Jérémy Hure
date: 2025
lang: en-EN
numbersections: true
documentclass: article
from: markdown+tex_math_single_backslash
geometry:
  - margin=2cm
papersize: a4
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

# Introduction {.unnumbered}

`MTest` allows defining nonlinear constraints involving gradients and
thermodynamic forces.

This document details the derivation of the Lagrange multiplier in
`MTest` and the usage of the `@NonLinearConstraint` keyword.


A detailed example is then presented, showing how to compute the pore
pressure under undrained conditions through the Lagrange multiplier. The
undrained condition leads to an effectively incompressible material
response, characterised by zero volumetric strain. The Lagrange
multiplier corresponding to the zero-volumetric-strain constraint is
obtained by minimising the free energy function and can be interpreted
as a pore pressure. The resulting pore pressure is then compared with
the pore pressure computed using OpenGeoSys.

In general, this approach can be used to interpret Lagrange multipliers
associated with constraints physically.

# Nonlinear constraints

> **Note**
>
> The latest version of the documentation is available on the command
> line
>
> ~~~~
> mtest --help-keyword=@NonLinearConstraint
> ~~~~

The `@NonLinearConstraint` keyword let the user introduce arbitrary
constraints on driving variables and thermodynamic forces.

## On the physical meaning of a constraint

A constraint \(c\) is imposed by introducing a Lagrange multiplier
\(\lambda\).

Consider a small strain elastic behaviour characterised by its free
energy \(\Psi\). In the only loading is the constraint \(c\), the
solution satisfies:
\[ \underset{\underline{\varepsilon},\lambda}{\min}\Psi-\lambda\,c \]

In this case, the constraint \(c\) is equivalent to the following
imposed stress:

\[
-\lambda\,\frac{\partial c}{\partial \underline{\varepsilon}}
\]

If the constraint is \(\sigma_{xx}-\sigma_{0}\), where \(\sigma_{0}\)
is a constant value, the previous equation shows that imposing this
constraint *is not equivalent* to imposing an uniaxial stress state
\(\left(\sigma_{xx}\,0\,0\,0\,0\,0\right)\).

## Normalisation policy

This keyword must be followed by an option giving the normalisation
policy. The normalisation policy can have one of the following values:

- `DrivingVariable`, `Strain`, `DeformationGradient`,
  `OpeningDisplacement` stating that the constraint is of the order of
  magnitude of the driving variable.
- `ThermodynamicForce`, `Stress`, `CohesiveForce` stating that the
  constraint is of the order of magnitude of the thermodynamic force.

## Examples

The following line imposes the a constant stress triaxiality (assuming
no shear stress components):

~~~~~{.python}
@Real 'B' '0.4';
@NonLinearConstraint<Stress> 'SYY - B * SXX';
@NonLinearConstraint<Stress> 'SZZ - B * SXX';
~~~~~

The following line imposes that the determinant of the deformation
gradient remains equal to \(1\):

~~~~~ {.python}
// ensure that the loading is isochoric in 1D
@NonLinearConstraint<DeformationGradient> 'FRR*FTT*FZZ-1';
~~~~~~~~~~

The following line imposes the value of the first Piola-Kirchhoff
stress (in 1D):

~~~~~ {.python}
// impose the first piola kirchhoff stress
// in an uniaxial compression test
@Real 'Pi0' -40e6
@NonLinearConstraint<Stress> 'SXX*FYY*FZZ-Pi0';
@NonLinearConstraint<Stress> 'SYY';
@NonLinearConstraint<Stress> 'SZZ';
@NonLinearConstraint<Stress> 'SXY';
@NonLinearConstraint<Stress> 'SXZ';
@NonLinearConstraint<Stress> 'SYZ';
~~~~~~~~~~

## Pitfalls

A stress component whose value is imposed using `@ImposedStress`
command shall not appear in a non linear constraint: the numerical
treatment of those two constraints is incompatible.

# Example: Caculation of pore pressure in undrained stress paths in `MTest`

This section describe how nonlinear contraints can be used to model
undrained compression tests. We highlight how the Lagrange multiplier
$\lambda$ is computed and its relation to the pore pressure $p$.


## Theory

The undrained condition refers to a loading process in which
no fluid is allowed to enter or leave the porous medium, so the total
fluid mass (or fluid content) in the system remains constant during
deformation. As a result, changes in stress are accompanied by changes
in pore pressure rather than fluid flow, leading to effective material
properties that differ from drained ones. Hooke's law for an isotropic
elastic body, representing the strain-effective stress relations in the
theory of poro-elasticity taking into account the effect of the pore
pressure on total stress is given as follows (only normal components)
[@biot_general_1941]:
\[
\begin{aligned}
    \varepsilon_{xx} &= \frac{\sigma_{xx}}{E} - \frac{1-\nu}{E}\left( \sigma_{yy} + \sigma_{zz} \right) + \frac{\Delta\, p}{3H}, \\[1.5ex]
    \varepsilon_{yy} &= \frac{\sigma_{yy}}{E} - \frac{1-\nu}{E}\left( \sigma_{xx} + \sigma_{zz} \right) + \frac{\Delta\,p}{3H}, \\[1.5ex]
    \varepsilon_{zz} &= \frac{\sigma_{zz}}{E} - \frac{1-\nu}{E}\left( \sigma_{xx} + \sigma_{yy} \right) + \frac{\Delta\,p}{3H},
\end{aligned}
\]
where $\dfrac{1}{H}$ is an additional parameter that denotes the
poroelastic expansion coefficient, and determined as:
\[
    \cfrac{1}{H}= \cfrac{3 \left( 1 - 2\nu\right) \alpha}{2 \left( 1 + \nu\right) G} \quad \mathrm{with} \quad G = \frac{E}{2 \left( 1 + \nu\right)}
    \label{H_coeff}
\]

Moreover, the variation in water content is given as a function of the
volumetric strain as follows[@biot_general_1941]:
\[
    \zeta = \alpha\, \mathrm{tr} \boldsymbol{\varepsilon}+ \cfrac{\Delta p}{Q},
    \label{variation_in_water_content}
\]
where 
\[
    \cfrac{1}{Q} = \cfrac{1}{R} - \cfrac{\alpha}{H}.    
\]
It is a measure of the amount of water which can be forced into the soil under pressure while the
volume of the soil is kept constant. $Q$ is called the Biot's modulus in the literature [@biot_general_1941].

In a more standard notation, these constitutive relations are written
as:
\[
    \boldsymbol{\sigma} = \boldsymbol{\sigma}^{\mathrm{eff}}-\alpha p \boldsymbol{I} \quad \mathrm{with} \quad \boldsymbol{\sigma}^{\mathrm{eff}} = K\,\mathrm{tr}\left( \boldsymbol{\varepsilon }\right) + 2\,G\,\boldsymbol{\varepsilon}^{\mathrm{dev}} \quad \mathrm{and} \quad \boldsymbol{\varepsilon}^{\mathrm{dev}} - \cfrac{\mathrm{tr}(\boldsymbol{\varepsilon})}{3}\,\boldsymbol{I},
\]
by considering an effective free energy function of the form:
\[
    \psi(\boldsymbol{\varepsilon}) = \cfrac{1}{2}\,K\, \mathrm{tr}^{2}\left(\boldsymbol{\varepsilon}\right) + G\,\boldsymbol{\varepsilon}^{\mathrm{dev}}:\boldsymbol{\varepsilon}^{\mathrm{dev}},
\]
where the bulk and the shear moduli are calculated as:
\[
    K = \cfrac{E}{3\,(1-2\,\nu)} \quad \mathrm{with} \quad G=\cfrac{E}{2\,\left( 1+\nu \right)}.
\]

We now consider an unconfined uniaxial compression test controlled by
$\varepsilon_{yy}=-\delta\,t$ under undrained conditions and an
incompressible fluid. This condition amounts to an isochoric test, so we
need to enforce $\epsilon:=\mathrm{tr}\varepsilon=0$. To obtain the
Lagrange multiplier associated with material incompressibility, the
stationary point of the energy complemented by two constraints and the
work of the external stresses is sought:
\[
    \varepsilon_{xx} + \varepsilon_{yy} + \varepsilon_{zz} = 0, \quad \mathrm{and} \quad \sigma_{xx} = \sigma_{zz}.
\]
The Lagrangian of the system reads:
\[
\mathcal{L}
= \frac{1}{2}K\,\mathrm{tr}^{2}(\boldsymbol{\varepsilon})
+ G\,\boldsymbol{\varepsilon}^{\mathrm{dev}}:\boldsymbol{\varepsilon}^{\mathrm{dev}}
- \lambda_1\,\mathrm{tr}(\boldsymbol{\varepsilon})
- \lambda_2 \left( \sigma_{xx} - \sigma_{zz} \right)
- \boldsymbol{\sigma} : \boldsymbol{\varepsilon},
\]
where $\lambda_1$, $\lambda_2$ are the Lagrange multipliers associated with the volumetric strain constraint and the stress constraint, respectively. The stationarity conditions are obtained by taking the first variations of the Lagrangian with respect to the unknown strain components and the Lagrange multipliers.

We introduce the deviatoric projector
\[
\mathbf{A}
= \mathbf{e}_x \otimes \mathbf{e}_x
- \mathbf{e}_z \otimes \mathbf{e}_z,
\qquad
\sigma_{xx} - \sigma_{zz} = \boldsymbol{\sigma} : \mathbf{A}.
\]

The variation with respect to the strain tensor yields
\[
\delta_\varepsilon \mathcal{L}
= \left(
K\,\mathrm{tr}(\boldsymbol{\varepsilon})\,\mathbf{I}
+ 2G\,\boldsymbol{\varepsilon}^{\mathrm{dev}}
- \lambda_1\,\mathbf{I}
- 2G\,\lambda_2\,\mathbf{A}
- \boldsymbol{\sigma}
\right) : \delta \boldsymbol{\varepsilon}
= 0,
\]
which must hold for all $\delta\boldsymbol{\varepsilon}$ and thus implies
\[
K\,\mathrm{tr}(\boldsymbol{\varepsilon})\,\mathbf{I}
+ 2G\,\boldsymbol{\varepsilon}^{\mathrm{dev}}
- \lambda_1\,\mathbf{I}
- 2G\,\lambda_2\,\mathbf{A}
- \boldsymbol{\sigma}
= \mathbf{0}.
\]

The variations with respect to the Lagrange multipliers give the constraints
\[
\delta_{\lambda_1} \mathcal{L}
= -\mathrm{tr}(\boldsymbol{\varepsilon})\,\delta \lambda_1 = 0,
\qquad
\delta_{\lambda_2} \mathcal{L}
= -(\sigma_{xx} - \sigma_{zz})\,\delta \lambda_2 = 0,
\]
so that
\[
\mathrm{tr}(\boldsymbol{\varepsilon}) = 0,
\qquad
\sigma_{xx} = \sigma_{zz}.
\]

From the stationarity condition with respect to $\boldsymbol{\varepsilon}$ we can express the (given) stress tensor as
\[
\boldsymbol{\sigma}
= K\,\mathrm{tr}(\boldsymbol{\varepsilon})\,\mathbf{I}
+ 2G\,\boldsymbol{\varepsilon}^{\mathrm{dev}}
- \lambda_1\,\mathbf{I}
- 2G\,\lambda_2\,\mathbf{A}.
\]
Using the result of the first constraint, $\mathrm{tr}(\boldsymbol{\varepsilon}) = 0$, this simplifies to
\[
\boldsymbol{\sigma}
= 2G\,\boldsymbol{\varepsilon}^{\mathrm{dev}}
- \lambda_1\,\mathbf{I}
- 2G\,\lambda_2\,\mathbf{A}.
\]

For a uniaxial compression test with $\varepsilon_{yy}$ as the control variable,
the volumetric constraint
\[
\mathrm{tr}(\boldsymbol{\varepsilon}) = 
\varepsilon_{xx} + \varepsilon_{yy} + \varepsilon_{zz} = 0
\]
together with the symmetry condition $\varepsilon_{xx} = \varepsilon_{zz}$ yields
\[
\varepsilon_{xx} = \varepsilon_{zz}
= -\frac{1}{2}\,\varepsilon_{yy}.
\]

Inserting this strain state into the stationarity condition
\[
\sigma_{ij}
= 2G\,\varepsilon_{ij}^{\mathrm{dev}}
- \lambda_1\,\delta_{ij}
- 2G\,\lambda_2\,A_{ij},
\qquad
A_{ij} = \mathrm{diag}(1,0,-1),
\]
gives the componentwise relations
\begin{align}
\sigma_{xx} &= 2G\,\varepsilon_{xx} - \lambda_1 - 2G\,\lambda_2,\\
\sigma_{yy} &= 2G\,\varepsilon_{yy} - \lambda_1,\\
\sigma_{zz} &= 2G\,\varepsilon_{zz} - \lambda_1 + 2G\,\lambda_2.
\end{align}

The second constraint, $\sigma_{xx} = \sigma_{zz}$, then reduces to
\[
-2G\,\lambda_2 = +2G\,\lambda_2 
\qquad\Longrightarrow\qquad
\lambda_2 = 0.
\]

For a uniaxial test the lateral stresses vanish,
\[
\sigma_{xx} = \sigma_{zz} = 0.
\]
Using $\lambda_2=0$ and $\varepsilon_{xx} = -\tfrac12\varepsilon_{yy}$ in
\(\sigma_{xx} = 2G\varepsilon_{xx} - \lambda_1\) gives
\[
0 = 2G\!\left(-\frac{1}{2}\varepsilon_{yy}\right) - \lambda_1,
\qquad\Longrightarrow\qquad
\lambda_1 = -G\,\varepsilon_{yy}.
\]

Thus, the Lagrange multipliers for the uniaxial compression case are
\[
\boxed{
\lambda_1 = -G\,\varepsilon_{yy},
\qquad
\lambda_2 = 0.
}
\]

Finally, the resulting axial stress follows as
\[
\sigma_{yy}
= 2G\,\varepsilon_{yy} - \lambda_1
= 3G\,\varepsilon_{yy}.
\]

## Treatment in `MTest`

By default, the Lagrange multiplier is scaled by the maximum absolute
value of the components of the stiffness tensor with the `Strain`
normalization policy at the first resolution.

In the presented example, the scaling factor is $K+\cfrac{4\,G}{3}$,
which is the first component of the elastic stiffness.

Finally, we get:
\[
\lambda^{*}_1 = \cfrac{\lambda_1}{K + 4\,G/3}
\]

The Lagrange multiplier $\lambda_1$ represents the pore pressure under
undrained conditions, i.e. $p=\lambda_1$.

> **Note**
>
> Rather than relying on `MTest`'s choice on the normalization factor,
> one may choose an appropriate one and use the `Stress` normalization
> policy, as follows:
>
> ~~~~{.python}
> @Real 'c' '70e9';
> @NonLinearConstraint<Stress> 'c * (EXX + EYY + EZZ)';
> ~~~~

## Verification

The comparison between the pore pressure obtained with
[OpenGeoSys](https://www.opengeosys.org/stable/) and the Lagrange
multiplier computed with `MTest` is presented in Figure
@fig:undrained_pore_pressure.

![Undrained pore-pressure response under uniaxial compression, comparing
numerical results from [OpenGeoSys](https://www.opengeosys.org/stable/)
and
`MTest`.](img/mtest-undrained_pore_pressure.svg){#fig:undrained_pore_pressure
width=80%}

# References {.unnumbered}
