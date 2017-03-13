% Phase field approach of brittle fracture with the `Cast3M` finite element solver
% T. Helfer
% 13/01/2017

\newcommand{\Frac}[2]{\displaystyle\frac{\displaystyle #1}{\displaystyle #2}}
\newcommand{\deriv}[2]{\Frac{\partial #1}{\partial #2}}
\newcommand{\Psiel}{\Psi^{\mathrm{el}}}
\newcommand{\pPsiel}{\Psi^{\mathrm{el}\,+}}
\newcommand{\nPsiel}{\Psi^{\mathrm{el}\,-}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\tepsilonel}{\underline{\epsilon}^{\mathrm{el}}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\paren}[1]{\left(#1\right)}
\newcommand{\ppart}[1]{\left<#1\right>_{+}}
\newcommand{\npart}[1]{\left<#1\right>_{-}}
\newcommand{\dtot}[1]{\mathrm{d}}

This article describes how to implement the phase field approach using
`MFront` and the `Cast3M` finite element solver.

# Description of the phase field approach

The physical idea underlying the phase field approach of brittle
fracture dates back to the pioneering work of Francfort and Marigo
(see @francfort_revisiting_1998). Bourdin et al. were the first to
propose a numerical implementation of the approach by regularizing the
cracks (see @bourdin_numerical_2000).

The description of the phase field approach used here relies on the
work of Miehe et al. (see @miehe_phase_2010)

## Defining a volumic energy associated to fracture

The crack network \(\Gamma\) contained in a body \(B\) can be
regularized by the following operator defining a damage field \(d\):

\[
  \left\{
    \begin{aligned}
      d(x)-l^2 \triangle d(x) &= 0 \ \text{on} \ B \\
      d(x) &= 1 \ \text{on} \ \Gamma \\
      \nabla d(x).n &= 0 \ \text{on} \ \partial B
    \end{aligned}
  \right.
\]

where (\partial B\) the boundary of \(B\). This operator introduces a
characteristic length \(l\).

The crack surface \(\Gamma\) can be computed by the following
integral:

\[
\Gamma\paren{d}=\int_{B}\gamma\paren{d}\,\dtot\,V
\]

where \(\gamma (d)=\frac{d^{2}}{2\,l}+\frac{l}{2}\,\vec{\nabla} d\,\cdot\,\vec{\nabla}{d}\)

Let \(g_{c}\) the energy associated with the creation of a unit
surface of crack. The amount of energy \(E_{c}\) that was required to
create the crack network is:

\[
E_{c}=g_{c}\,\int_{B}\gamma\paren{d}\,\dtot\,V=\int_{B}g_{c}\,\gamma\paren{d}\,\dtot\,V
\]

\(g_{c}\,\gamma\paren{d}\) is an energy *per unit of volume*
associated with the creation of crack network. This quantity is the
fundamental brick when building a thermodynamically consistent
approach to the phase field modelling of brittle failure.

In pratice, the crack network is an unknown of the mechanical problem,
so the idea of the phase field approach is to introduce the damage
field \(d\) and add \(g_{c}\,\gamma\paren{d}\) in the free energy of
the material. Application of the Claussis-Duhem inegality then leads
to an explicit equation satisfied by the damage field, as described
below.

## Choice of the free energy

To motivate the choice of the free energy for the phase-field
behaviour, we begin to recall some classical results about the
decomposition of the free energy of an isotropic elastic material into
a positive and negative parts.

### Decomposition in positive and negative parts of the free energy of an isotropic elastic material

The free energy \(\Psiel\) of an isotropic elastic material is
given by:

\[
\Psiel=\lambda\,\trace{\tepsilonel}^{2}+2\,\mu\,\tepsilonel\,\colon\,\tepsilonel
\]

where \(\lambda\) and \(\mu\) are the Lamé coefficients of the
material.

This free energy can be decomposed in a positive part associated with
tension and a negative part associated with compression:

\[
\Psiel=\pPsiel+\nPsiel
\]

where \(\pPsiel\) and \(\nPsiel\) are defined by:
\[
\left\{
\begin{aligned}
\pPsiel &= 2\,\mu\,\ppart{\tepsilonel}\,\colon\,\ppart{\tepsilonel}+\lambda\,\ppart{\trace{\tepsilonel}}^{2}\\
\nPsiel &= 2\,\mu\,\npart{\tepsilonel}\,\colon\,\npart{\tepsilonel}+\lambda\,\npart{\trace{\tepsilonel}}^{2}
\end{aligned}
\right.
\]

In this expression, the positive part of a scalar is defined by:
\[
\ppart{x}=
\left\{
\begin{aligned}
x & \text{ if } x>0 \\
0 & \text{ if } x\leq0
\end{aligned}
\right.
\]

The positive part of a tensor is defined as an isotropic function as
follows:
\[
\ppart{\tenseur{x}}=\sum_{i=1}^{3}\ppart{\lambda_{i}}\,\tenseur{n}_{i}
\]

## Definition of the free energy for the phase-field behaviour

To take into choi The free energy \(\Psi\) is choosen to have the
following form:

\[
\Psi=\paren{m(d) + k}\,\pPsiel{} + \nPsiel{}+g_{c}\,\gamma\paren{d}
\]

where:

- \(m(d) = \paren{1-d}^2\) describe the effect of damage on the
  positive part of the energy.
- \(k\) is a small constant to ensure a minimal stiffness to the fully
  damaged material.

## Definition of the stress

The stress is defined as the thermodynamic force associated with the elastic strain:

\[
\tsigma
=\deriv{\Psi}{\tepsilonel}
=\paren{m(d) + k}\,\paren{2\,\mu\,\ppart{\tepsilonel}+\lambda\,\ppart{\trace{\tepsilonel}}\,\tenseur{I}}+
2\,\mu\,\npart{\tepsilonel}+\lambda\,\npart{\trace{\tepsilonel}}\,\tenseur{I}
\]

## Thermodynamic force associated with the damage

Inspired by thermodynamic arguments, namely the respect of the
Clausis-Duhem inegality, Miehe introduces the following equation:

\[
	\frac{g_{c}}{l}\left[d - l^2 \triangle d\right] = 2\,(1 - d)\,H 
\]

where
\[
  H(x,t) = \max_{\tau \in [0,t]}\left[\pPsiel\paren{\tau}\right]
\]

This definition of \(H\), which appears as the driving force of the
damage evolution, has been introduced to described the irreversibility
of the crack propagation, altough the demonstration that this choice
leads to an increasing value of the damage is not straightforward.

# Implementation


# References

<!-- Local IspellDict: english -->
