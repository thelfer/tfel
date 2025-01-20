% Implementation of the Ogden hyperelastic behaviour
% T. Helfer
% 09/01/2017

\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tns}[1]{{\underset{\tilde{}}{\mathbf{#1}}}}
\newcommand{\transpose}[1]{{#1^{\mathop{T}}}}

\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sderiv}[2]{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial #2^{2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}

\newcommand{\C}{\tenseur{C}}
\newcommand{\S}{\tenseur{S}}
\newcommand{\iJb}{\bar{c}_{I_{3}}}
\newcommand{\diJb}{\deriv{\iJb}{I_{3}}}
\newcommand{\sdiJb}{\sderiv{\iJb}{I_{3}}}

This page describes how to implement the Ogden hyperelastic behaviour.

The whole implementation is available here:
[Ogden.mfront](./gallery/hyperelasticity/Ogden.mfront)

This implementation is compatible with `TFEL` version `3.1` which has
introduced several overloaded versions of the
`computeIsotropicFunction` and `computeIsotropicFunctionDerivative`
methods which reduce the number of evaluations of the `pow` function.

An implementation compatible with `TFEL` version `3.0` is available
here:
[Ogden-tfel-3.0.0.mfront](./gallery/hyperelasticity/Ogden-tfel-3.0.0.mfront)

# Description

Let \(\C\) be the right Cauchy tensor. The three invariants of \(\C\)
are defined as:
\[
\left\{
\begin{aligned}
I_{1} &= \trace{C} \\
I_{2} &= \Frac{1}{2}\left(\paren{\trace{\C}}^{2}-\trace{\C^{2}}\right) \\
I_{3} &= \det\paren{\C}
\end{aligned}
\right.
\]

\(J\) is the determinant of the deformation gradient (\(J=\sqrt{I_{3}}\))

The Ogden hyperelastic behaviour is described by a decoupled potential
\(W\) decomposed into a volumetric part \(W^{v}\) and an
isochoric part \(\bar{W}^{i}\):

\[
W\paren{\C}=W^{v}\paren{J}+\bar{W}^{i}\paren{\bar{\lambda}_{1},\bar{\lambda}_{2},\bar{\lambda}_{3}}
\]

where:

- \(\paren{\bar{\lambda}_{i}}_{i\in \{1,2,3\}}\) are the eigenvalues
  of the isochoric right Cauchy tensor
  \(\tenseur{\bar{C}}\). \(\paren{\bar{\lambda}_{i}}_{i\in
  \{1,2,3\}}\) are defined as: \(\bar{\lambda}_{i}=\iJb\,\lambda_{i}\)
- \(\lambda_{i}\) are the right Cauchy stress tensor \(\C\)
- \(\iJb=I_{3}^{-1/3}\)
- \(\tenseur{\bar{C}}=J^{-2/3}\,\C = I_{3}^{-1/3}\,\C = \iJb\,\C\)

**A general derivation of the stress and the consistent tangent
operator for these kinds of hyperelastic behaviours is described
[here](hyperelasticity.html).** The reader shall refer to this page
for intermediate computations that won't be detailed here.

For our implementation, we choose \(W^{v}\) as follows: 
\[
W^{v}\paren{J}=\Frac{1}{2}\,K\,(J-1)^{2}
\]
where \(K\) is the bulk modulus.

The Ogden behaviour mostly refers to the following choice of
\(\bar{W}^{i}\):
\[
\bar{W}^{i}\paren{\bar{\lambda}_{1},\bar{\lambda}_{2},\bar{\lambda}_{3}}=\sum_{p=1}^{N}\bar{W}^{i}_{p}
=\sum_{p=1}^{N}\Frac{\mu_{p}}{\alpha_{p}}\paren{\bar{\lambda}_{1}^{\alpha_{p}/2}+\bar{\lambda}_{2}^{\alpha_{p}/2}+\bar{\lambda}_{3}^{\alpha_{p}/2}-3} 
=\sum_{p=1}^{N}\Frac{\mu_{p}}{\alpha_{p}}\iJb^{\alpha_{p}/2}\,f_{p}\paren{\lambda_{1},\lambda_{2},\lambda_{3}}-\Frac{3\,\mu_{p}}{\alpha_{p}}
\]

with \(f_{p}\paren{\lambda_{1},\lambda_{2},\lambda_{3}}=\lambda_{1}^{\alpha_{p}/2}+\lambda_{2}^{\alpha_{p}/2}+\lambda_{3}^{\alpha_{p}/2}\)

In the following, we will make the simple choice (\(p=1\)):
\[
\bar{W}^{i}\paren{\bar{\lambda}_{1},\bar{\lambda}_{2},\bar{\lambda}_{3}}=\sum_{p=1}^{N}\bar{W}^{i}_{p}
=\Frac{\mu}{\alpha}\iJb^{\alpha/2}\,f\paren{\lambda_{1},\lambda_{2},\lambda_{3}}-\Frac{3\,\mu}{\alpha}
\]

The general expression of the second Piola-Kirchhoff stress is:

\[
\S=2\,\deriv{W^{v}}{\C}+2\deriv{W^{i}}{\C}=\S^{v}+\S^{i}
\]

where \(\S^{v}=2\,\deriv{W^{v}}{\C}\) is the
volumetric part of the second Piola-Kirchhoff stress and
\(\S^{i}=2\,\deriv{W^{i}}{\C}\) is the isochoric
part.

# Implementation 

The implementation of this behaviour is decomposed in two parts:

- The computation of the Cauchy stress.
- The computation of the consistent tangent operator.

We use the `DefaultFiniteStrain` implementation as this behaviour
does not require an integration algorithm.

~~~~~{.cpp}
@DSL DefaultFiniteStrain;
~~~~~

Then, we define the material parameters:

~~~~~{.cpp}
@Parameter alpha =     28.8;
@Parameter mu    =    27778;
@Parameter K     = 69444444;
~~~~~

In the following, we will mostly use \(\alpha/2\):

~~~~~{.cpp}
const auto a = alpha/2;
~~~~~

## Computation of the Cauchy stress

### Computation of the right Cauchy tensor \(\C\), the invariants and the invariants derivatives

The identity tensor is stored in the `id` variable for a shorter and
cleaner code:

~~~~~{.cpp}
const auto id = Stensor::Id();
~~~~~

Then, we compute the determinant of the deformation gradient at the
end of the time step, called `F1`:

~~~~~{.cpp}
const auto J  = det(F1);
~~~~~

Then we compute the right Cauchy tensor \(\C\):

~~~~~{.cpp}
const auto C  = computeRightCauchyGreenTensor(F1);
~~~~~

For the computation of the invariants of \(\C\) and their
derivatives, we need to compute \(\C^{2}\). Since the multiplication
of two symmetric tensors leads to a non symmetric tensors, we use the
special function `square` which is a more efficient equivalent of
`syme(C*C)`:

~~~~~{.cpp}
const auto C2 = square(C);
~~~~~

The first invariant \(I_{1}\) is the trace of \(\C\):

~~~~~{.cpp}
const auto I1 = trace(C);
~~~~~

The second invariant \(I_{2}\) is computed as follows:

~~~~~{.cpp}
const auto I2 = (I1*I1-trace(C2))/2;
~~~~~

The derivative of \(\deriv{I_{2}}{\C}\) is equal to
\(\trace{\C}\,\tenseur{I}-\C\), which is readily written as:

~~~~~{.cpp}
const auto dI2_dC = I1*id-C;
~~~~~

> At this stage, \(\deriv{I_{2}}{\C}\) is not evaluated: the
> associated variable `dI2_dC` is an evaluation tree that stores the
> computation that will be performed later. This is an essential
> optimization technique used in `MFront` to achieve optimal
> performances. If we wanted or needed to explicitly evaluate, we
> could either use the `eval` function or explicitly specify the type
> of `dI2_dC`.

The derivative \(\deriv{I_{3}}{\C}\) of the third invariant is given
by:
\[
\deriv{I_{3}}{\C} = \C^{2}-I_{1}\,\C+I_{2}\,\tenseur{I}
\]

The computation of the third invariant and its derivatives is
straightforward:

~~~~~{.cpp}
const auto I3      = J*J;
const auto dI3_dC  = computeDeterminantDerivative(C);
~~~~~

### Computation of the volumetric part of the second Piola-Kirchhoff stress

The volumetric part of the second Piola-Kirchhoff stress \(\S^{v}\) is
given by:
\[
\S^{v} = \Frac{1}{J}\,\deriv{W^{v}}{J}\,\deriv{I_{3}}{\C}
\]

Here,
\[
\deriv{W^{v}}{J}=K\,(J-1)
\]

We can implement it as follows:

~~~~~{.cpp}
const auto dPv_dJ   = K*(J-1);
const StressStensor Sv = dPv_dJ/J*dI3_dC;
~~~~~

Here, we have explicitly given the type of `Sv` to force its evaluation.

### Computation of the isochoric part of the second Piola-Kirchhoff stress

By chain rule, one has:
\[
\begin{aligned}
\S^{i}=2\,\deriv{\bar{W}^{i}}{\C}=
\mu\,\iJb^{\alpha/2-1}\,f\paren{\lambda_{1},\lambda_{2},\lambda_{3}}\deriv{\iJb}{\C}+
\Frac{2\,\mu\,\iJb^{\alpha/2}}{\alpha}\,\deriv{f}{\C}\\
\end{aligned}
\]

\(\deriv{f}{\C}\) is an **isotropic function of \(\C\)**, thus we can
use the various methods provided by the `TFEL` library to evaluate
\(\deriv{f}{\C}\) and its derivative \(\sderiv{f}{\C}\) using the
`computeIsotropicFunction` and `computeIsotropicFunctionDerivative`
static methods of the `Stensor` class.

First, we need to compute \(\iJb\) and its derivatives. To minimize
the number of calls to the `pow` function, we also compute
\(\iJb^{a-2}\):

~~~~~{.cpp}
const auto iJb        =  1/cbrt(I3);
const auto iJb2       =  power<2>(iJb);
const auto iJb4       =  iJb2*iJb2;
const auto iJb7       =  iJb4*power<3>(iJb);
const auto c          = pow(iJb,a-2);
// derivatives
const auto diJb_dI3   = -iJb4/3;
const auto d2iJb_dI32 = 4*iJb7/9;
const auto diJb_dC    = diJb_dI3*dI3_dC;
~~~~~~

Then, we can compute the eigenvalues and the eigentensors of \(\C\):

~~~~~{.cpp}
tvector<3u,real> vp;
tmatrix<3u,3u,real> m;
std::tie(vp,m) = C.computeEigenVectors();
~~~~~

> In C++-17, the previous code can be replaced by:
> 
> ~~~~~{.cpp}
> const auto [vp,m] = C.computeEigenVectors();
> ~~~~~

We can now evaluate \(f\) and \(\deriv{f}{\C}\).:

~~~~~{.cpp}
const tvector<3u,real> pwv  = {pow(vp(0),a-2),pow(vp(1),a-2),pow(vp(2),a-2)};
const tvector<3u,real> d2fv = {a*(a-1)*pwv(0),a*(a-1)*pwv(1),a*(a-1)*pwv(2)};
const tvector<3u,real> dfv  = {a*vp(0)*pwv(0),a*vp(1)*pwv(1),a*vp(2)*pwv(2)};
const auto fv    = vp(0)*vp(0)*pwv(0)+vp(1)*vp(1)*pwv(1)+vp(2)*vp(2)*pwv(2);
const auto df_dC = Stensor::computeIsotropicFunction(dfv,m);
~~~~~

Finally, we can compute \(\S^{i}\):

~~~~~{.cpp}
const StressStensor Si = mu*c*iJb*((fv*diJb_dC+(iJb/a)*df_dC));
~~~~~

### Computation of the Cauchy stress

The function `convertSecondPiolaKirchhoffStressToCauchyStress`
converts the second Piola-Kirchhoff stress to the Cauchy stress using
the deformation gradient. It is used as follows:

~~~~{.cpp}
sig = convertSecondPiolaKirchhoffStressToCauchyStress(Sv+Si,F1);
~~~~

## Computation of the consistent tangent operator

The most direct expression of the consistent tangent operator is given by
\(\deriv{\S}{\C}\). We let `MFront` make the appropriate conversion to
the consistent tangent operator expected by the solver.

Here,
\[
\deriv{\S}{\C}=\deriv{\S^{v}}{\C}+\deriv{\S^{i}}{\C}
\]

### Computation of the \(\sderiv{I_{3}}{\C}\)

To compute \(\sderiv{I_{3}}{\C}\), one may use the
`computeDeterminantSecondDerivative` function:

~~~~{.cpp}
const auto d2I3_dC2 = computeDeterminantSecondDerivative(C);
~~~~

### Computation of the volumetric part of the consistent tangent operator

\(\deriv{\S^{v}}{\C}\) is given by:

\[
\begin{aligned}
\deriv{\S^{v}}{\C}
	&=\deriv{}{J}\paren{\Frac{1}{J}\,\deriv{W^{v}}{J}}\,\deriv{J}{I_{3}}\,\deriv{I_{3}}{\C}\otimes\deriv{I_{3}}{\C}+\Frac{1}{J}\,\deriv{W^{v}}{J}\,\sderiv{I_{3}}{\C}\\
	&=\paren{\sderiv{W^{v}}{J}-\Frac{1}{J}\,\deriv{W^{v}}{J}}\,\frac{1}{2\,I_{3}}\,\deriv{I_{3}}{\C}\otimes\deriv{I_{3}}{\C}+\Frac{1}{J}\,\deriv{W^{v}}{J}\,\sderiv{I_{3}}{\C}\\
\end{aligned}
\]

Here:
\[
\sderiv{W^{v}}{J}=K
\]

Those expressions are straightforward to implement:

~~~~{.cpp}
const auto d2Pv_dJ2 = K;
dS_dC = ((d2Pv_dJ2-dPv_dJ/J)/(2*I3)*(dI3_dC^dI3_dC)
        +dPv_dJ/J*d2I3_dC2);
~~~~

### Computation of the isochoric part of the consistent tangent operator

As detailed [here](hyperelasticity.html), the expression of
\(\deriv{S^{i}}{\C}\) is given by:

\[
\begin{aligned}
\Frac{1}{\mu}\,\deriv{S^{i}}{\C}
&=\frac{\alpha-2}{2}\,\iJb^{\alpha/2-2}\,f\paren{\lambda_{1},\lambda_{2},\lambda_{3}}\deriv{\iJb}{\C}\otimes\deriv{\iJb}{\C}\\
&+\iJb^{\alpha/2-1}\,f\paren{\lambda_{1},\lambda_{2},\lambda_{3}}\,\sderiv{\iJb}{\C}+\iJb^{\alpha/2-1}\,\deriv{\iJb}{\C}\otimes\deriv{f}{\C}\\
&+\iJb^{\alpha/2-1}\,\deriv{f}{\C}\otimes\deriv{\iJb}{\C}+
2\,\Frac{\iJb^{\alpha/2}}{\alpha}\,\sderiv{f}{\C}
\end{aligned}
\]

This expression requires the computation of \(\sderiv{f}{\C}\), which
is a direct application of the static method
`computeIsotropicFunctionDerivative` from the `Stensor` class:

~~~~{.cpp}
const tvector<3u,real> d2fv = {a*(a-1)*pwv(0),a*(a-1)*pwv(1),a*(a-1)*pwv(2)};
const auto d2f_dC2    = Stensor::computeIsotropicFunctionDerivative(dfv,d2fv,vp,m,1.e-12);
~~~~

With this result, the computation of \(\sderiv{f}{\C}\) is a direct
translation from the previous equation:

~~~~{.cpp}
const auto d2iJb_dI32 = 4*iJb7/9;
const auto d2iJb_dC2  = d2iJb_dI32*(dI3_dC^dI3_dC)+ diJb_dI3*d2I3_dC2;
dS_dC += mu*c*((a-1)*fv*(diJb_dC^diJb_dC)+iJb*(fv*d2iJb_dC2+
               ((diJb_dC^df_dC)+(df_dC^diJb_dC))+iJb/a*d2f_dC2));
~~~~

### Tests

The comparison of previous derivation of the consistent tangent
operator to a numerical approximation is made in the `Ogden.cxx` test
that can be found in the `tests/Material` directory of the `TFEL`
sources.

The components of the consistent tangent operator match their
numerical approximation with an accuracy lower than
\(10^{-9}\max_{i,j}\deriv{D}{\C}(i,j)\).

<!-- Local IspellDict: english -->
