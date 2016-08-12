% The `Abaqus/Standard` and `Abaqus/Explicit` interfaces
% Thomas Helfer
% 12/08/2016

\newcommand{\deriv}[2]{\frac{\partial #1}{\partial #2}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tns}[1]{{\underset{\tilde{}}{\mathbf{#1}}}}
\newcommand{\transpose}[1]{{#1^{\mathop{T}}}}
\newcommand{\tsigma}{\underline{\sigma}}

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

### Isotropic case

The "Abaqus User Subroutines Reference Guide" gives indicates that the
tangent moduli required by `Abaqus/Standard` \(\tenseurq{C}^{M}\) is
closely related to \(\tenseurq{C}^{\tau\,J}\), the moduli associated
to the Jauman rate of the Kirchoff stress :

\[
J\,\tenseurq{C}^{M}=\tenseurq{C}^{\tau\,J}
\]

where \(J\) is the derterminant of the deformation gradient
\(\tns{F}\).

By definition, \(\tenseurq{C}^{\tau\,J}\) satisfies:
\[
\overset{\circ}{\tenseur{\tau}}^{J}=\tenseurq{C}^{\tau\,J}\,\colon\tenseur{D}
\]
where \(\tenseur{D}\) is the rate of deformation.

### Link with other tangent operator

#### Relation with the spatial moduli $\tenseurq{C}^{c}$

The spatial moduli \(\tenseurq{C}^{c}\) is related to the Lie
derivative of the Kirchhoff stress \(\mathcal{L}\tenseur{\tau}\) ,
which is also called the convected rate or the Oldroyd rate:

\[
\mathcal{L}\tenseur{\tau}=\tenseurq{C}^{c}\,\colon\,\tenseur{D}
\]

The relationship between \(\tenseurq{C}^{\tau\,J}\) and
\(\tenseurq{C}^{c}\) can be expressed in component form as

\[
\tenseurq{C}^{\tau\,J}_{ijlk}=\tenseurq{C}^{c}+I_{ik}\tau_{jl}+I_{jl}\tau_{ik}
\]

where \tns{I} is the rank 2 identity tensor.

#### Relation with $\tenseurq{C}^{\mathrm{SE}}$ 

The \(\tenseurq{C}^{\mathrm{SE}}\) relates the rate of the second
Piola-Kirchhoff stress \(\tenseur{S}\) and the Green-Lagrange strain
rate \(\tenseur{\varepsilon}^{\mathrm{GL}}\):

\[
\tenseur{\dot{S}}=\tenseurq{C}^{\mathrm{SE}}\,\colon\,\tenseur{\dot{\varepsilon}}^{\mathrm{GL}}
\]

As the Lie derivative of the Kirchoff stress
\(\mathcal{L}\tenseur{\tau}\) is the push-forward of the second
Piola-Kirchhoff stress rate \(\tenseur{\dot{S}}\) and the rate of
deformation \(\tenseur{D}\) is push-forward of the Green-Lagrange
strain rate \(\tenseur{\dot{\varepsilon}}^{\mathrm{GL}}\),
\(\tenseurq{C}^{c}\) is the push-forward of
\(\tenseurq{C}^{\mathrm{SE}}\):

\[
C^{c}_{ijkl}=F_{im}F_{jn}F_{kp}F_{lq}C^{\mathrm{SE}}_{mnpq}
\]

#### Link with $\deriv{\tsigma}{\tns{F}}$

### Orthotropic case

# The `Abaqus/Explicit` interface

## Energies

<!-- - Internal energy per unit mass -->
<!-- - Dissipated inelastic energy per unit mass -->

## Finite strain strategies

Two finite strain strategies are available:

- 'FiniteRotationSmallStrain'
- 'MieheApelLambrechtLogarithmicStrain'

<!-- Local IspellDict: english -->

