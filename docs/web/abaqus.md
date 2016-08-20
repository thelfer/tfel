% The `Abaqus/Standard` and `Abaqus/Explicit` interfaces
% Thomas Helfer
% 12/08/2016

\newcommand{\deriv}[2]{\frac{\partial #1}{\partial #2}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tns}[1]{{\underset{\tilde{}}{\mathbf{#1}}}}
\newcommand{\transpose}[1]{{#1^{\mathrm{T}}}}
\newcommand{\tsigma}{\tenseur{\sigma}}
\newcommand{\ctsigma}{\tenseur{\hat{\sigma}}}
\newcommand{\cD}{\tenseur{\hat{D}}}
\newcommand{\cC}{\tenseurq{\hat{C}}}
\newcommand{\CtJ}{\tenseurq{C}^{\tau\,J}}
\newcommand{\CsT}{\tenseurq{C}^{\sigma\,T}}
\newcommand{\CsG}{\tenseurq{C}^{\sigma\,G}}
\newcommand{\CSE}{\tenseurq{C}^{\mathrm{SE}}}
\newcommand{\Cs}{\tenseurq{C}^{s}}
\newcommand{\Cspin}{\tenseurq{C}^{\mathrm{spin}}}
\newcommand{\CMJ}{\tenseurq{C}^{MJ}}
\newcommand{\tpld}[1]{\partial^{\star}_{l}\left(#1\right)}
\newcommand{\tprd}[1]{\partial^{\star}_{r}\left(#1\right)}
<!-- \newcommand{\paren}[1]{\left(#1\right)} -->

`MFront` version 3.0 provides two interfaces for the `Abaqus/Standard`
and `Abaqus/Explicit` finite element solvers.

# Main features

The introduction material describe the main features available

## Supported behaviours

Isotropic and orthotropic behaviours are both supported. For
orthotropic behaviours, an orientation *must* be defined.

For `Abaqus/Standard`, small and finite strain behaviours are
supported.

For `Abaqus/Explicit`, only finite strain are supported. Small strain
behaviours can be used using one of the finite strain strategies
available.

### Notes

- For `Abaqus/Standard`, a small strain behaviour can be used with the
  build-in hypoelastic formulation as the internal state variables
  will be rotated appropriately.
- For `Abaqus/Explicit`, direct use of small strain behaviours is
  prohibited.

## Modelling hypotheses

The following modelling hypotheses are supported:

- tridimensional
- plane stress
- plane strain (including shell elements)
- axisymmetrical

The generalised plane strain hypothesis is currently *not* supported.

# The `Abaqus/Standard` interface

## Finite strain strategies

Two finite strain strategies are available:

- 'FiniteRotationSmallStrain'
- 'MieheApelLambrechtLogarithmicStrain'

## Consistent tangent operator for finite strain behaviours

Most information reported here are extracted from the book of
Belytschko (@belytschko_nonlinear_2000).

### Isotropic case

The "Abaqus User Subroutines Reference Guide" gives indicates that the
tangent moduli required by `Abaqus/Standard` \(\CMJ\) is
closely related to \(\tenseurq{C}^{\tau\,J}\), the moduli associated
to the Jauman rate of the Kirchhoff stress :

\[
J\,\CMJ=\CtJ
\]

where \(J\) is the derterminant of the deformation gradient
\(\tns{F}\).

By definition, \(\CtJ\) satisfies:
\[
\overset{\circ}{\tenseur{\tau}}^{J}=\CtJ\,\colon\tenseur{D}
\]
where \(\tenseur{D}\) is the rate of deformation.

#### Relation with the moduli associated to the Truesdell rate of the Cauchy Stress $\CsT$

The moduli associated to the Truesdell rate of the Cauchy Stress
\(\CsT\) is related to \(\CtJ\) by the following relationship:

\[
\CtJ=J\,\left(\CsT+\tenseurq{C}^{\prime}\right)\quad\text{with}\quad\tenseurq{C}^{\prime}\colon\tenseur{D}=\tsigma\,.\,\tenseur{D}+\tenseur{D}\,.\,\tsigma
\]

Thus,

\[
\CMJ=\CsT+\tenseurq{C}^{\prime}
\]

#### Relation with the spatial moduli $\Cs$

The spatial moduli \(\Cs\) is associated to the Lie
derivative of the Kirchhoff stress \(\mathcal{L}\tenseur{\tau}\) ,
which is also called the convected rate or the Oldroyd rate:

\[
\mathcal{L}\tenseur{\tau}=\Cs\,\colon\,\tenseur{D}
\]

The spatial moduli is related to the moduli associated to Truesdell
rate of the Cauchy stress \(\CsT\):

\[
\CsT=J^{-1}\,\Cs
\]

Thus, we have:
\[
\CMJ = J^{-1}\Cs+\tenseurq{C}^{\prime} = J^{-1}\left(\Cs+\tenseurq{C}^{\prime\prime}\right)\quad\text{with}\quad\tenseurq{C}^{\prime\prime}\colon\tenseur{D}=\tenseur{\tau}\,.\,\tenseur{D}+\tenseur{D}\,.\,\tenseur{\tau}
\]

<!-- The relationship between \(CsJ\) and -->
<!-- \(\Cs\) can be expressed in component form as -->

<!-- \[ -->
<!-- CsJ_{ijlk}=\Cs+I_{ik}\tau_{jl}+I_{jl}\tau_{ik} -->
<!-- \] -->

<!-- where \tns{I} is the rank 2 identity tensor. -->

#### Relation with $\CSE$ 

The \(\CSE\) relates the rate of the second
Piola-Kirchhoff stress \(\tenseur{S}\) and the Green-Lagrange strain
rate \(\tenseur{\varepsilon}^{\mathrm{GL}}\):

\[
\tenseur{\dot{S}}=\CSE\,\colon\,\tenseur{\dot{\varepsilon}}^{\mathrm{GL}}
\]

As the Lie derivative of the Kirchhoff stress
\(\mathcal{L}\tenseur{\tau}\) is the push-forward of the second
Piola-Kirchhoff stress rate \(\tenseur{\dot{S}}\) and the rate of
deformation \(\tenseur{D}\) is push-forward of the Green-Lagrange
strain rate \(\tenseur{\dot{\varepsilon}}^{\mathrm{GL}}\),
\(\Cs\) is the push-forward of
\(\CSE\):

\[
C^{c}_{ijkl}=F_{im}F_{jn}F_{kp}F_{lq}C^{\mathrm{SE}}_{mnpq}
\]

#### Link with $\deriv{\tsigma}{\tns{F}}$

For all variation of the deformation gradient \(\delta\,\tns{F}\), the
Jauman rate of the Kirchhoff stress satisfies:
\[
\CtJ\,\colon\delta\tenseur{D}=\delta\tenseur{\tau}-\delta\tns{W}.\tenseur{\tau}+\tenseur{\tau}.\delta\tns{W}
\]

with:

- \(\delta\tns{L}= \delta\tns{F}\,.\,\tns{F}^{-1}\)
- \(\delta\tns{W}= \frac{1}{2}\left(\delta\tns{L}-\transpose{\delta\tns{L}}\right)\)
- \(\delta\tns{D}= \frac{1}{2}\left(\delta\tns{L}+\transpose{\delta\tns{L}}\right)\)

Thus, the derivative of the Kirchhoff stress with respect to the
deformation gradient yields:
\[
\deriv{\tenseur{\tau}}{\tns{F}}=\CtJ\,.\,\deriv{\tenseur{D}}{\tns{F}}+\left(\tprd{\tau}-\tpld{\tau}\right)\,.\,\deriv{\tns{W}}{\tns{F}}
\]

with \(\delta\,\tenseur{D}=\deriv{\tenseur{D}}{\tns{F}}\,\colon\,\delta\,\tns{F}\) and \(\delta\,\tns{W}=\deriv{\tns{W}}{\tns{F}}\,\colon\,\delta\,\tns{F}\)

\[
\deriv{\tsigma}{\tns{F}}=\frac{1}{J}\left(\deriv{\tenseur{\tau}}{\tns{F}}-\tsigma\,\otimes\,\deriv{J}{\tns{F}}\right)
\]

### Numerical approximation of \(\CMJ\)

Following @sun_numerical_2008, an numerical approximation of \(\CMJ\)
is given by:
\[
\CMJ_{ijkl}\approx\frac{1}{J\,\varepsilon}\left(\tenseur{\tau}_{ij}\left(\tns{F}+\tns{\delta F}^{kl}\right)-\tenseur{\tau}_{ij}\left(\tns{F}\right)\right) \]

where the perturbation \(\tns{\delta F}^{kl}\) is given by:

\[
\tns{\delta F}^{kl}=\frac{\varepsilon}{2}\left(\vec{e}_{k}\otimes\vec{e}_{l}+\vec{e}_{l}\otimes\vec{e}_{k}\right)\,.\,\tns{F}
\]

Such perturbation leads to the following rate of deformation:
\[
\delta\,\tenseur{D}=\left(\tns{\delta F}^{kl}\right)\,\tns{F}^{-1}=\frac{\varepsilon}{2}\left(\vec{e}_{k}\otimes\vec{e}_{l}+\vec{e}_{l}\otimes\vec{e}_{k}\right)
\]

The spin rate \(\delta\,\tenseur{W}\) associated with
\(\tns{\delta F}^{kl}\) is null.

### Orthotropic case

The orthotropic case, when an orientation is defined, is much more
complex and poorly documented. Much of what follows is a matter of
deduction and numerical experiments and need to be strengthened.

For non-linear geometric analyses, `Abaqus/Standard` uses an
hypoelastic based on a corotational stress formulation fully described
in the Abaqus manual and the book of Belytschko
[see @belytschko_nonlinear_2000].

The deformation gradient is given in the corotational framework. The
output of the `UMAT` subroutine is the corotational stress
\(\ctsigma\) defined by:

\[
\tsigma=\tns{R}\,.\,\ctsigma\,\transpose{\tns{R}}
\]

where \(\tns{R}\) is the rotation matrix obtained by the polar
decomposition of the deformation gradient \(\tns{F}\).

For consistency, one expects the appropriate tangent operator to be be
defined by:

\[
\ctsigma=\cC\,\colon\,\cD
\]

\(\cC\) is directly related to the to the moduli associated to the
Green-Nagdi stress rate \(\CsG\): \(\cC\) is obtained by rotationg
\(\CsG\) in the corotational framework.

#### Relation with the moduli associated to the Truesdell rate of the Cauchy Stress $\CsT$

The moduli associated with Truesdell rate of the Cauchy Stress can be
related to the moduli associated to the Green-Nagdi stress rate.

\[
\CsG=\CsT+\tenseurq{C}^{\prime}-\tsigma\otimes\tenseur{I}+\Cspin
\]

where \(\Cspin\) is given in @simo_computational_1998.

#### Relation with other moduli

The previous relation can be used to relate to other moduli. See the
section describing the isotropic case for details.

# The `Abaqus/Explicit` interface

## Energies

<!-- - Internal energy per unit mass -->
<!-- - Dissipated inelastic energy per unit mass -->

## Finite strain strategies

Two finite strain strategies are available:

- 'FiniteRotationSmallStrain'
- 'MieheApelLambrechtLogarithmicStrain'

# Biblography

<!-- Local IspellDict: english -->

