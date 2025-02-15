---
title: Second and fourth-order tensor objects in `TFEL/Math`
author: Thomas Helfer, Jérémy Hure, Mohamed Shokeir
date: 08/2020
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tns}[1]{{\underset{\tilde{}}{\mathbf{#1}}}}
\newcommand{\tnsq}[1]{{\underset{\tilde{}}{\underset{\tilde{}}{\mathbf{#1}}}}}
\newcommand{\transpose}[1]{{#1^{\mathop{T}}}}

\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sderiv}[2]{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial #2^{2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\absvalue}[1]{{\left|#1\right|}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}

\newcommand{\a}{\tns{a}}
\newcommand{\s}{\tenseur{s}}
\newcommand{\c}{\tenseur{c}}

This page is meant to describe the various tensor objects and
operations available in `TFEL/Math` and some functionalities provided
by the `TFEL/Material` library.

# Classes describing second and fourth order tensors

## Symmetric second order tensors

When dealing with constitutive equations, most computations are
performed on *symmetric* tensors.

Classes describing symmetric second order tensors satisfies the
`StensorConcept`. 

An end user will mostly use the `stensor` class, which have the
following template arguments:

- The space dimension (`1`, `2` or `3`).
- The type used to perform the computation.

The following code declares a symmetric second order tensor in \(2D\)
using single precision floating-point number:

~~~~{.cxx}
stensor<2,float> s;
~~~~

Symmetric tensors are denoted as follows \(\s\).

### Aliases used in `MFront`

In `MFront`, various aliases are introduced to ease the implementation
of mechanical behaviours:

- `Stensor` is an alias to `stensor<N,real>` where `N` is the space
  dimension and `real` is the numerical type used. The value of `N`
  depends of the current modelling hypothesis. The concrete type for
  `real` depends on the interface used.
- `StrainStensor` is an alias to `stensor<N,strain>`.
- `StressStensor` is an alias to `stensor<N,stess>`.

## Vector notations for symmetric tensors

A symmetric tensor is stored as an array of values, as follows in
\(3D\):
\[
  \tenseur{s}=
  \begin{pmatrix}
    s_{\,11}\quad
    s_{\,22}\quad
    s_{\,33}\quad
    \sqrt{2}\,s_{\,12}\quad
    \sqrt{2}\,s_{\,13}\quad
    \sqrt{2}\,s_{\,23}
  \end{pmatrix}^{T}
\]

The contracted product of two symmetric tensors is the scalar product
of their vector forms (hence the \(\sqrt{2}\)).

## General (non symmetric) second order tensors

Classes describing symmetric second order tensors satisfies the
`TensorConcept`. 

An end user will mostly use the `tensor` class, which have the
following template arguments:

- The space dimesion (`1`, `2` or `3`).
- The type used to perform the computation.

Non symmetric tensors are denoted as follows \(\a\).

## Vector notations for non symmetric tensors

A tensor is stored as an array of values, as follows in
\(3D\):
\[
  \tenseur{s}=
  \begin{pmatrix}
    s_{\,11}\quad
    s_{\,22}\quad
    s_{\,33}\quad
    s_{\,12}\quad
    s_{\,21}\quad
    s_{\,13}\quad
    s_{\,31}\quad
    s_{\,23}\quad
    s_{\,32}
  \end{pmatrix}^{T}
\]

### The identity tensor

The symmetric second order identity tensor is returned by the `Id`
static member of the `stensor` class as follows:

~~~~{.cpp}
constexpr const auto id = stensor<3u,real>::Id();
~~~~

## Fourth order tensors

Fourth order tensors can be defined as linear mappings from the second
order tensors to second order tensors. As there is two kinds of second
order tensors (i.e. symmetric and non symmetric tensors), there are
four kinds of fourth order tensors defined in the `TFEL/Math` library,
which satisfy the following concepts:

- `ST2toST2Concept`: linear mapping from symmetric tensors to
  symmetric tensors.
- `ST2toT2Concept`: linear mapping from symmetric tensors to non
  symmetric tensors.
- `T2toST2Concept`: linear mapping from non symmetric tensors to
  symmetric tensors.
- `T2toT2Concept`: linear mapping from non symmetric tensors to
  non symmetric tensors.

An end user will mostly use the following implementations of those
concepts: `st2tost2`, `st2tot2`, `t2tost2` and `t2tot2`
respectively. Those classes have the following template arguments:

- The space dimesion (`1`, `2` or `3`).
- The type used to perform the computation.

### Aliases used in `MFront`

In `MFront`, various aliases are introduced to ease the implementation
of mechanical behaviours:

- `Stensor4` is an alias to `st2tost2<N,real>` where `N` is the space
  dimension and `real` is the numerical type used. The value of `N`
  depends of the current modelling hypothesis. The concrete type for
  `real` depends on the interface used.
- `StiffnessTensor` is an alias to `st2tost2<N,stress>`.

### Special values

#### Special values of the `st2tost2` class

The `st2tost2` provides the following static methods:

- `Id`: returns the identity matrix.
- `IxI`: returns the tensor defined by
  \(\tenseur{I}\,\otimes\,\tenseur{I}\). This tensor
  satisfies, for every symmetric tensor \(\s\):
  \[
  \tenseur{I}\,\otimes\,\tenseur{I}\,\colon\,\s=\trace{\s}\,\tenseur{I}
  \]
- `J`: returns the tensor defined by
  \(\Frac{1}{3}\,\tenseur{I}\,\otimes\,\tenseur{I}\). This tensor
  satisfies, for every symmetric tensor \(\s\):
  \[
  \tenseurq{J}\,\colon\,\s=\Frac{\trace{\s}}{3}\,\tenseur{I}
  \]
- `K`: returns the tensor defined by
  \(\tenseur{I}-\Frac{1}{3}\,\tenseur{I}\,\otimes\,\tenseur{I}\). This
  is tensor is indeed the projector on the deviatoric space.
- `M`: returns the tensor defined by
  \(\Frac{3}{2}\left(\tenseur{I}-\Frac{1}{3}\,\tenseur{I}\,\otimes\,\tenseur{I}\right)\).
  This tensor appears in the definition of the von Mises stress:
  \[
  \sigmaeq=\sqrt{\tsigma\,\colon\,\tenseurq{M}\,\colon\,\tsigma}
  \]

#### Special values of the `t2tot2` class

The `t2tot2` provides the following static method:

- `Id`: returns the identity matrix.
- `IxI`: returns the tensor defined by
  \(\tns{I}\,\otimes\,\tns{I}\). This tensor
  satisfies, for every tensor \(\a\):
  \[
  \tns{I}\,\otimes\,\tns{I}\,\colon\,\a=\trace{\a}\,\tns{I}
  \]
- `J`: returns the tensor defined by
  \(\Frac{1}{3}\,\tns{I}\,\otimes\,\tns{I}\). This tensor
  satisfies, for every tensor \(\a\):
  \[
  \tnsq{J}\,\colon\,\a=\Frac{\trace{\a}}{3}\,\tns{I}
  \]
- `K`: returns the tensor defined by
  \(\tns{I}-\Frac{1}{3}\,\tns{I}\,\otimes\,\tns{I}\). This
  is tensor is indeed the projector on the deviatoric space.

# Standard operations

## Basic operations

Thanks to operator overloading, the following operations are written
using standard mathematical notations:

- Addition of two tensors.
- Subtraction of two tensors.
- Multiplication of two tensors. Be cautious of the fact that the
  multiplication of two symmetric tensors results in a non symmetric
  tensor.
- Multiplication and division of a tensor by a scalar.

For example, the following code shows how to perform the addition of two tensors `a` and `b`

~~~~{.cpp}
const auto c=a+b;
~~~~

### Expression templates

For optimization purpose, the result of the previous operations are
not evaluted.  In the previous example, `c` is not the result but a
special class built on the fly representing the addition of the
tensors `a` and `b`.

The `eval` function can be used to explicitly evaluate the result of
the operation.

~~~~{.cpp}
const auto c=eval(a+b);
~~~~

### In-place operations

Operations like `b=b+a` can be also be written using operator `+=` as
follows:

~~~~{.cpp}
b+=a;
~~~~

The operator `-=` is also available for operations like `b=b-a`.

The operators `*=` and `/=` are also available for inplace
multiplication or division by a scalar :

~~~~{.cpp}
// divide tensor a by 2
a/=2;
~~~~

## Symmetrization and unsymmetrization of second order tensors

A non symmetric second order tensor can be symmetrized using the
`syme` function. The result match the `StensorConcept`.

A symmetric second order tensor can be unsymmetrized using the
`unsyme` function. The result match the `TensorConcept`.

## Frobenius inner product of second order tensors

The Frobenius inner product \(\tns{a}\,\colon\,\tns{b}\) of two tensors
\(\tns{a}\) and \(\tns{b}\) is defined by:

\[
\tns{a}\,\colon\,\tns{b}=\trace{\transpose{\tns{a}}\,\cdot\,\tns{b}}=\sum_{i,j}a_{ij}b_{ij}
\]

This operation is implemented in `TFEL/Math` using the `|` operator,
as follows:

~~~~{.cpp}
const auto r = a|b;
~~~~

The user must be aware that this operator as a low priority in `C++`,
so one must usually use parenthesis to properly evaluate operations
involving those operations.

## Diadic product

The diadic product \(\tns{a}\,\otimes\,\tns{b}\) of two tensors
\(\tns{a}\) and \(\tns{b}\) satisfies, for any tensor \(\tns{c}\):

\[
\left\{
\begin{aligned}
\tns{c}\,\colon\,\paren {\tns{a}\,\otimes\,\tns{b}}&=\paren{\tns{c}\,\colon\,\tns{a}}\,\tns{b} \\
\paren {\tns{a}\,\otimes\,\tns{b}}\,\colon\,\tns{c}&=\paren{\tns{c}\,\colon\,\tns{b}}\,\tns{a} \\
\end{aligned}
\right.
\]

The diadic product is implemented in `TFEL/Math` using operator
`^`. The user must be aware that this operator as a low priority in
`C++`, so one must usually use parenthesis to properly evaluate
operations involving diadic products.

~~~~{.cpp}
dfeel_ddeel += 2.*mu*theta*dp*iseq*(Stensor4::M()-(n^n));
~~~~

The diadic product of two symmetric tensors results in an object
matching the `ST2toST2Concept`.

The diadic product of two non symmetric tensors results in an object
matching the `T2toT2Concept`.

## Polar decomposition

The polar decomposition of a tensor `F` can be computed as follows in
`MFront`:

~~~~{.cxx}
tensor<N, real> R;
stensor<N, strain> U;
polar_decomposition(R, U, F);
~~~~

where:

- `N` is the space dimension.
- `real` is an alias to the numeric type.
- `strain` is an alias to the numeric type.

## Application of a fourth order tensor

## Multiplication of second order tensors

Second order tensors can be multiplied using the `*` operator. Note that
multiplying two symmetric tensors results in a non symmetric tensor.

### Derivatives of the product of two symmetric tensors

Let \(\tenseur{a}\) and \(\tenseur{b}\) two symmetric tensors and
\(\tenseur{c}\) their products.

The derivative \(\deriv{\tenseur{c}}{\tenseur{a}}\) is an object of the
`st2tot2` type, i.e. a linear transformation of a symmetric tensor to a
non symmetric tensor. It can be computed using the `st2tot2::tpld`
method (tensor product left derivative) as follows:

~~~~cxx
const auto dc_da = st2tot2<N,real>::tpld(b);
~~~~

Note that the `st2tot2::tpld` method takes the tensor \(\tenseur{b}\)
as its argument.

If \(\tenseur{a}\) is a function of another symmetric tensor
\(\tenseur{d}\), the derivative \(\deriv{\tenseur{c}}{\tenseur{d}}\) can
be computed by chain rule:

~~~~cxx
const auto dc_dd = st2tot2<N,real>::tpld(b) * da_dd;
~~~~

However, as \(\deriv{\tenseur{c}}{\tenseur{a}}\) is a fourth order tensor
with many zeros, this operation may be inefficient. A specialisation of
the `st2tot2::tpld` method has thus been introduced and can be used as
follows:

~~~~cxx
const auto dc_dd = st2tot2<N,real>::tpld(b, da_dd);
~~~~

The derivative \(\deriv{\tenseur{c}}{\tenseur{b}}\) can be computed in a
similar manner using the `st2tot2::tprd` method (tensor product right
derivative).

### Derivatives of the product of two non symmetric tensors

Let \(\tns{a}\) and \(\tns{b}\) two non symmetric tensors and
\(\tns{c}\) their products.

The derivative \(\deriv{\tns{c}}{\tns{a}}\) is an object of the
`t2tot2` type, i.e. a linear transformation of a non symmetric tensor to a
non symmetric tensor. It can be computed using the `st2tost2::tpld`
method (tensor product left derivative) as follows:

~~~~cxx
const auto dc_da = st2tost2<N,real>::tpld(b);
~~~~

Note that the `t2tot2::tpld` method takes the tensor \(\tns{b}\)
as its argument.

If \(\tns{a}\) is a function of another non symmetric tensor
\(\tns{d}\), the derivative \(\deriv{\tns{c}}{\tns{d}}\) can
be computed by chain rule:

~~~~cxx
const auto dc_dd = t2tot2<N,real>::tpld(b) * da_dd;
~~~~

However, as \(\deriv{\tns{c}}{\tns{a}}\) is a fourth order tensor
with many zeros, this operation may be inefficient. A specialisation of
the `t2tot2::tpld` method has thus been introduced and can be used as
follows:

~~~~cxx
const auto dc_dd = t2tot2<N,real>::tpld(b, da_dd);
~~~~

The derivative \(\deriv{\tns{c}}{\tns{b}}\) can be computed in a
similar manner using the `t2tot2::tprd` method (tensor product right
derivative).

## Symmetric product of two symmetric second order tensors

The symmetric product of two symmetric second order tensors
\(\tenseur{a}\) and \(\tenseur{b}\) can be defined as follows:

\[
\tenseur{a}\,\cdot_{s}\,\tenseur{b} = \Frac{1}{2}\paren{\tenseur{a}\,\cdot\tenseur{b}+\tenseur{b}\,\cdot\tenseur{a}}
\]

This can be computed by the `symmetric_product` function.

### Derivative

The derivative of the symmetric product
\(\tenseur{a}\,\cdot_{s}\,\tenseur{b}\) with respect to
\(\tenseur{a}\) can be computed using the `st2tost2::stpd` static
method with takes \(\tenseur{b}\) as argument.

## Second symmetric product of two symmetric second order tensors \(\tenseur{a}\,\cdot\,\tenseur{b}\,\cdot\,\tenseur{a}\)

Another symmetric product of two symmetric second order tensors
\(\tenseur{a}\) and \(\tenseur{b}\) can be defined as follows:

\[
\tenseur{a}\,\cdot\,\tenseur{b}\,\cdot\,\tenseur{a}
\]

This can be computed by the `symmetric_product_aba` function.

### Derivative

The derivative of
\(\tenseur{a}\,\cdot\,\tenseur{b}\,\cdot\,\tenseur{a}\) with respect to
\(\tenseur{a}\) can be computed using the
`symmetric_product_derivative_daba_da` function.

The derivative of
\(\tenseur{a}\,\cdot\,\tenseur{b}\,\cdot\,\tenseur{a}\) with respect to
\(\tenseur{b}\) can be computed using the
`symmetric_product_derivative_daba_db` function.

### Computation of \(\tenseur{a}\,\underline{\overline{\otimes}}\,\tenseur{a}\)

By definition, given a symmetric tensor \(\tenseur{a}\), the tensor
product \(\tenseur{a}\,\underline{\overline{\otimes}}\,\tenseur{a}\) is
the fourth order tensor (of type `st2tost2`) which satisfies, for any
tensor \(\tenseur{b}\):
\[
\left(\tenseur{a}\,\underline{\overline{\otimes}}\,\tenseur{a}\right)\,\colon\,\tenseur{b}=
\tenseur{a}\,\cdot\,\tenseur{b}\,\cdot\,\tenseur{a}
\]

\(\tenseur{a}\,\underline{\overline{\otimes}}\,\tenseur{a}\) can thus
readily be computed using the `symmetric_product_derivative_daba_db`
function.

# Special mathematical functions

## Change the basis

The `change_basis` functions can:

- rotate a symmetric tensor
- rotate a (non-symmetric) tensor
- rotate a fourth order tensor of type `st2tost2`.
- rotate a fourth order tensor of type `t2tot2`.

Those functions takes two constant arguments: the object to be rotated
and the rotation matrix. The rotated object is returned.

### Example

~~~~{.cpp}
const auto sr = change_basis(s,r);
~~~~

### Fourth order tensors standing for the rotation of tensors

The `st2tost2` class provide the `fromRotationMatrix` static method
which computes a fourth order tensor which has the same effect on a
symmetric tensor than applying a given rotation.

~~~~{.cpp}
const auto rt = st2tost2<N,real>::fromRotationMatrix(r);
~~~~

The `t2tot2` class provide the `fromRotationMatrix` static method
which computes a fourth order tensor which has the same effect on a
(non-symmetric) tensor than applying a given rotation.

~~~~{.cpp}
const auto rt = t2tot2<N,real>::fromRotationMatrix(r);
~~~~

> **Note**
> 
> In pratice, the `fromRotationMatrix` static methods can be used to
> compute the rotation of any second and fourth order tensors (including
> the ones of the `t2tost2` and `st2tot2` types). They are used internally
> in the implementation of the `change_basis` functions provided for the
> fourth order tensors of types `st2tost2` and `t2tot2`.

## Inverses

The `invert` functions can compute:

- the inverse of a symmetric tensor.
- the inverse of a tensor.
- the inverse of a fourth order tensor of type `st2tost2`.

Those functions takes the object to be inverted as constant argument and
returned the inverse.

## Square of a symmetric tensor

The product of two symmetric tensors is a non symmetric
tensor. However, the square of a symmetric tensor is a symmetric
tensor.

The square of a symmetric tensor can be computed using the `square`
function, as follows:

~~~~{.cpp}
const auto s2 = square(s);
~~~~

### Derivative of the square of a symmetric tensor

The derivative of the square of a symmetric tensor is a fourth order
tensor mapping a symmetric tensor toward a symmetric tensor. It can be
computed using the `dsquare` function, as follows:

~~~~{.cpp}
const auto ds2_ds = st2tost2<N,real>::dsquare(s);
~~~~

The result of this operation of mostly filled with zero. If \(\s\) is
a function of \(\tenseur{\c}\), this fact can be used to optimize the
computation of the derivative of \(\s^{2}\) with respect to \(\c\):

~~~~{.cpp}
const auto ds2_dc = st2tost2<N,real>::dsquare(s2,ds_dc);
~~~~

which is more efficient than:

~~~~{.cpp}
const auto ds2_dc = st2tost2<N,real>::dsquare(s2)*ds_dc;
~~~~

## Positive and negative parts of a symmetric tensor

The Positive and negative parts of a symmetric tensor can be computed
respectively by the `positive_part` and `negative_part` function.

## Transposition {#sec:transpose}

### Transposition of a second order tensor

A non symmetric second order tensor can be transpose using the
`transpose` function:

~~~~{.cpp}
const auto B = transpose(A);
~~~~

#### Derivative of the transpose of a second order tensor

The linear operation which turns a second order tensor into its
transpose can be retrieved using the static method
`transpose_derivative` of the `t2tot2` class as follows:

~~~~{.cpp}
const auto dtA_dA = t2tot2<real,N>::transpose_derivative();
~~~~

As its name suggests, this linear operation is also the derivative of
the transpose of a second order tensor with respect to itself.

### Transposition of a fourth order tensor

A fourth order tensor matching the `ST2toST2Concept` (i.e. a linear form
mapping a symmetric second order tensor to a symmetric second order
tensor) can be transposed using the `transpose` function:

~~~~{.cpp}
const auto B = transpose(A);
~~~~

## Second order tensor invariants {#sec:invariants}

### Defintion

The three invariants of a second order tensor are defined by:
\[
\left\{
\begin{aligned}
I_{1} &= \trace{\a} \\
I_{2} &= \Frac{1}{2}\left(\paren{\trace{\a}}^{2}-\trace{\a^{2}}\right) \\
I_{3} &= \det\paren{\a}
\end{aligned}
\right.
\]

## Cauchy-Green tensor and derivative

The `computeRightCauchyGreenTensor` computes the right Cauchy-Green
symmetric tensor \(\tenseur{C}\) associated with a non symmetric tensor
\(\tns{F}\):

\[
\tenseur{C}=\transpose{\tns{F}}\,\cdot\,\tns{F}
\]

The derivative of \(\tenseur{C}\) with respect to \(\tns{F}\) can be
computed using the `t2tost2::dCdF` static method.`

## Left Cauchy-Green tensor and derivative

The `computeLeftCauchyGreenTensor` computes the left Cauchy-Green
symmetric tensor \(\tenseur{B}\) associated with a non symmetric tensor
\(\tns{F}\):

\[
\tenseur{B}=\tns{F}\,\cdot\,\transpose{\tns{F}}
\]

The derivative of \(\tenseur{B}\) with respect to \(\tns{F}\) can be
computed using the `t2tost2::dBdF` static method.`

### Computation

\(I_{1}\) can be computed thanks to `trace` function as follows:

~~~~{.cpp}
const auto I1 = trace(A);
~~~~

Of course, \(I_{1}\) can also be computed directly by accessing the
components of the tensor, i.e. :

~~~~{.cpp}
const auto I1 = A(0)+A(1)+A(2);
~~~~

\(I_{2}\) can be computed by translating its definition as follows:

~~~~{.cpp}
const auto I2 = (I1*I1-trace(square(A)))/2;
~~~~

\(I_{3}\) can be computed thanks to `det` function as follows:

~~~~{.cpp}
const auto I3 = det(A);
~~~~

### Derivatives of the invariants of a tensor {#sec:invariants:derivatives}

The derivative of the invariants are classically given by:
\[
\left\{
\begin{aligned}
\deriv{I_{1}}{\a} &= \tenseur{I}\\
\deriv{I_{2}}{\a} &= I_{1}\,\tenseur{I}-\a^{T}\\
\deriv{I_{3}}{\a} &= \det\paren{\a}\,\a^{-T}=\paren{\a^{2}-I_{1}\,\a+I_{2}\,\tenseur{I}}^{T} \\
\end{aligned}
\right.
\]

Those expressions are simplier for symmetric tensors:
\[
\left\{
\begin{aligned}
\deriv{I_{1}}{\s} &= \tenseur{I}\\
\deriv{I_{2}}{\s} &= I_{1}\,\tenseur{I}-\s\\
\deriv{I_{3}}{\s} &= \det\paren{\s}\,\s^{-1}=\s^{2}-I_{1}\,\s+I_{2}\,\tenseur{I} \\
\end{aligned}
\right.
\]

\(\deriv{I_{1}}{\a}\) and \(\deriv{I_{2}}{\a}\) are trivial to compute.

\(\deriv{I_{3}}{\a}\) can be computed using the
`computeDeterminantDerivative` function as follows:

~~~~{.cpp}
const auto dI3_dA = computeDeterminantDerivative(A);
~~~~

### Second derivatives of the invariants of a tensor

\(\sderiv{I_{1}}{\a}\) is null.

\(\sderiv{I_{2}}{\s}\) can be computed as follows:

\[
\sderiv{I_{2}}{\s}=\tenseur{I}\otimes\tenseur{I}-\deriv{\a^{T}}{\a}
\]

The last term, \(\deriv{\a^{T}}{\a}\) can be computed using the
`t2tot2::transpose_derivative` static method, see Paragraph
@sec:transpose for details.

~~~~{.cpp}
const auto id = tensor<N,real>::Id();
const auto d2I2_dA2 = (id^id)-t2tot2<N,real>::transpose_derivative();
~~~~

For symmetric tensors, this computation is much simplier:

~~~~{.cpp}
const auto id = stensor<N,real>::Id();
const auto d2I2_dA2 = (id^id)-st2tot2<N,real>::Id();
~~~~

The \(\sderiv{I_{3}}{\a}\) term can be computed using the
`computeDeterminantSecondDerivative` function, as follows:

~~~~{.cpp}
const auto d2I3_dA2 = computeDeterminantSecondDerivative(A);
~~~~

## Invariants of the stress deviator tensor @wikipedia_invariants_2017 {#sec:deviatoric:invariants}

Let \(\tsigma\) be a stress tensor. Its deviatoric part
\(\tenseur{s}\) is:
\[
\tenseur{s}=\tsigma-\Frac{1}{3}\,\trace{\tsigma}\,\tenseur{I}
=\paren{\tenseurq{I}-\Frac{1}{3}\,\tenseur{I}\,\otimes\,\tenseur{I}}\,\colon\,\tsigma
\]

The deviator of a tensor can be computed using the `deviator`
function.

As it is a second order tensor, the stress deviator tensor also has a
set of invariants, which can be obtained using the same procedure used
to calculate the invariants of the stress tensor. It can be shown that
the principal directions of the stress deviator tensor \(s_{ij}\) are
the same as the principal directions of the stress tensor
\(\sigma_{ij}\). Thus, the characteristic equation is

\[
\left| s_{ij}- \lambda\delta_{ij} \right| = -\lambda^3+J_1\lambda^2-J_2\lambda+J_3=0,
\]

where \(J_1\), \(J_2\) and \(J_3\) are the first, second, and third
*deviatoric stress invariants*, respectively. Their values are the same
(invariant) regardless of the orientation of the coordinate system
chosen. These deviatoric stress invariants can be expressed as a
function of the components of \(s_{ij}\) or its principal values \(s_1\),
\(s_2\), and \(s_3\), or alternatively, as a function of \(\sigma_{ij}\) or
its principal values \(\sigma_1\), \(\sigma_2\), and \(\sigma_3\). Thus,

\[
\begin{aligned}
J_1 &= s_{kk}=0,\, \\
J_2 &= \textstyle{\frac{1}{2}}s_{ij}s_{ji} = \Frac{1}{2}\trace{\tenseur{s}^2}\\
&= \Frac{1}{2}(s_1^2 + s_2^2 + s_3^2) \\
&= \Frac{1}{6}\left[(\sigma_{11} - \sigma_{22})^2 + (\sigma_{22} - \sigma_{33})^2 + (\sigma_{33} - \sigma_{11})^2 \right ] + \sigma_{12}^2 + \sigma_{23}^2 + \sigma_{31}^2 \\
&= \Frac{1}{6}\left[(\sigma_1 - \sigma_2)^2 + (\sigma_2 - \sigma_3)^2 + (\sigma_3 - \sigma_1)^2 \right ] \\
&= \Frac{1}{3}I_1^2-I_2 = \frac{1}{2}\left[\trace{\tenseur{\sigma}^2} - \frac{1}{3}\trace{\tenseur{\sigma}}^2\right],\,\\
J_3 &= \det\paren{\tenseur{s}} \\
&= \Frac{1}{3}s_{ij}s_{jk}s_{ki} = \Frac{1}{3} \trace{\tenseur{s}^3}\\
&= \Frac{1}{3}(s_1^3 + s_2^3 + s_3^3) \\
&= s_1s_2s_3 \\
&= \Frac{2}{27}I_1^3 - \Frac{1}{3}I_1 I_2 + I_3 = \Frac{1}{3}\left[\trace{\tenseur{\sigma}^3} - \trace{\tenseur{\sigma}^2}\trace{\tenseur{\sigma}} +\Frac{2}{9}\trace{\tenseur{\sigma}}^3\right].
\end{aligned}
\]

where \(I_{1}\), \(I_{2}\) and \(I_{3}\) are the invariants of
\(\tsigma\) (see Section&nbsp;@sec:invariants).

The invariants \(J_{2}\) and \(J_{3}\) of deviatoric part of the
stress are the basis of many isotropic yield criteria, some of them
being described below.

### First derivative

This paragraph details the first derivative of \(J_{2}\) and \(J_{3}\)
with respect to \(\tsigma\).

The computation of \(\deriv{J_{2}}{\tsigma}\) is straight-forward by
chain rule, using the expression of the derivatives of the invariants
of a tensor (see Section&nbsp;@sec:invariants:derivatives):
\[
\deriv{J_{2}}{\tsigma}= \deriv{J_{2}}{\tenseur{s}}\,\cdot\,\deriv{\tenseur{s}}{\tsigma}= \tenseur{s}
\]

In pratice, this can be implemented as follows:

~~~~{.cpp}
const auto dJ2 = deviator(sig);
~~~~

For the expression of \(\deriv{J_{3}}{\tsigma}\), one can derive its
expression based of the three invariants of \(\tsigma\), as follows:

\[
\begin{aligned}
\deriv{J_{3}}{\tsigma}
&=\deriv{}{\tsigma}\paren{\Frac{2}{27}\,I_{1}^3 - \Frac{1}{3}\,I_{1}\,I_{2} + I_{3}}\\
&=\Frac{2}{9}\,I_{1}^{2}\,\tenseur{I}-\Frac{1}{3}\left[I_{2}\,\tenseur{I}+I_{1}\,\deriv{I_{2}}{\tsigma}\right]+\deriv{I_{3}}{\tsigma}\\
\end{aligned}
\]

For `TFEL` versions prior to 3.2, one can implement this formula as follows:

~~~~{.cpp}
constexpr const auto id = stensor<N,real>::Id();
const auto I1  = trace(sig);
const auto I2  = (I1*I1-trace(square(sig)))/2;
const auto dI2 = I1*id-sig;
const auto dI3 = computeDeterminantDerivative(sig);
const auto dJ3 = eval((2*I1*I1/9)*id-(I2*id+I1*dI2)/3+dI3);
~~~~

For `TFEL` versions greater than \(3.2\), one may want to use the
optimised `computeDeviatorDeterminantDerivative` function, defined in
the namespace `tfel::math`, as follows:

~~~~{.cpp}
const auto dJ3 = computeDeviatorDeterminantDerivative(sig);
~~~~

The `computeJ3Derivative`, defined in `tfel::material` namespace, is a
synonym for the `computeDeviatorDeterminantDerivative` function.

### Second derivative

This paragraph details the second derivatives of \(J_{2}\) and \(J_{3}\)
with respect to \(\tsigma\).

The second derivative \(\sderiv{J_{2}}{\tsigma}\) is straight-forward:
\[
\sderiv{J_{2}}{\tsigma}=
\tenseurq{I}-\Frac{1}{3}\,\tenseur{I}\,\otimes\,\tenseur{I}
\]

It can be readily implemented:

~~~~{.cpp}
constexpr const auto id  = stensor<N,real>::Id();
constexpr const auto id4 = st2tost2<N,real>::Id();
const auto d2J2 = eval(id4-(id^id)/3);
~~~~

The second derivative \(\sderiv{J_{3}}{\tsigma}\) is also
straight-forward to compute (see also
Section&nbsp;@sec:invariants:derivatives):
\[
\deriv{J_{3}}{\tsigma}
=\Frac{4}{9}\,I_{1}\,\tenseur{I}\,\otimes\,\tenseur{I}
-\Frac{1}{3}\left[\tenseur{I}\,\otimes\,\deriv{I_{2}}{\tsigma}+\deriv{I_{2}}{\tsigma}\,\otimes\,\tenseur{I}+I_{1}\,\sderiv{I_{2}}{\tsigma}\right]+
\sderiv{I_{3}}{\tsigma}
\]

For `TFEL` versions prior to 3.2, one can implement this formula as follows:

~~~~{.cpp}
constexpr const auto id = stensor<N,real>::Id();
constexpr const auto id4 = st2tost2<N,real>::Id();
const auto I1   = trace(sig);
const auto I2   = (I1*I1-trace(square(sig)))/2;
const auto dI2  = I1*id-sig;
const auto d2I2 = (id^id)-id4;
const auto d2I3 = computeDeterminantSecondDerivative(sig);
const auto d2J3 = eval((4*I1/9)*(id^id)-((id^dI2)+(dI2^id)+i1*d2I2)/3+d2I3);
~~~~

For `TFEL` versions greater than \(3.2\), one may want to use the
optimised `computeDeviatorDeterminantSecondDerivative` function,
defined in the `tfel:math` namespace, as follows:

~~~~{.cpp}
const auto d2J3 = computeDeviatorDeterminantSecondDerivative(J);
~~~~

The `computeJ3SecondDerivative`, defined in `tfel::material`
namespace, is a synonym for the
`computeDeviatorDeterminantSecondDerivative` function.

## Orthotropic generalization of the invariants of the stress deviator tensor {#sec:deviatoric:orthotropic:invariants}

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
based on \(J_{2}\) and \(J_{3}\) invariants to orthotropy.

\(J_{2}^{0}\), \(J_{3}^{0}\) and their first and second derivatives
with respect to the stress tensor \(\tsigma\) can be computed
by the following functions:

- `computesJ2O`, `computesJ2ODerivative` and
  `computesJ2OSecondDerivative`.
- `computesJ3O`, `computesJ3ODerivative` and
  `computesJ3OSecondDerivative`.

Those functions take the stress tensor as first argument and each
orthotropic coefficients. Each of those functions has an overload
taking the stress tensor as its firs arguments and a tiny vector
(`tfel::math::tvector`) containing the orthotropic coefficients.

## Eigenvalues, eigenvectors and eigentensors of symmetric tensors

### Eigenvalue

The eigenvalues can be computed by the `computeEigenValues` method, as
follows:

~~~~{.cpp}
const auto vp = s.computeEigenValues();
~~~~

> **Note**
>
> In `2D`, the last eigenvalue always corresponds to the out-of-plane
> direction.

Those eigen values can be ordered by using one of the following argument:

- `ASCENDING`: eigenvalues are sorted from the lowest to the greatest.
- `DESCENDING`: eigenvalues are sorted from the greatest to the lowest.

~~~~{.cpp}
const auto vp = s.computeEigenValues(stensor::ASCENDING);
~~~~

> **Note**
>
> In `1D`, the sorting parameter has no effect.
> In `2D`, the last eigenvalue always corresponds to the out-of-plane
> direction.

By default, the eigenvalues are computed using Cardano
formula. However, one may use one of the following eigensolver
decribed in the next paragraph as follows:

~~~~{.cpp}
constexpr const auto es = stensor<3u,real>::FSESQLEIGENSOLVER;
const auto vp = s.computeEigenValues<es>();
~~~~

### Eigenvectors {#sec:eigensolvers}

The default eigen solver for symmetric tensors used in `TFEL` is based
on analitical computations of the eigen values and eigen vectors. Such
computations are more efficient but less accurate than the iterative
Jacobi algorithm (see [@kopp_efficient_2008;@kopp_numerical_2017]).

With the courtesy of Joachim Kopp, we have created a `C++11` compliant
version of his routines that we gathered in header-only library called
`FSES` (Fast Symmetric Eigen Solver). This library is included with
`TFEL` and provides the following algorithms:

- Jacobi
- QL with implicit shifts
- Cuppen
- Analytical
- Hybrid
- Householder reduction

We have also introduced the Jacobi implementation of the `Geometric`
`Tools` library (see [@eberly_robust_2016;@eberly_geometric_2017]).

Those algorithms are available in 3D. For 2D symmetric tensors, we
fall back to some default algorithm as described below.

| Name                        | Algorithm  in 3D          | Algorithm  in 2D   |
|:---------------------------:|:-------------------------:|:------------------:|
| `TFELEIGENSOLVER`           | Analytical (TFEL)         | Analytical (TFEL)  |
| `FSESJACOBIEIGENSOLVER`     | Jacobi                    | Analytical (FSES)  |
| `FSESQLEIGENSOLVER`         | QL with implicit shifts   | Analytical (FSES)  |
| `FSESCUPPENEIGENSOLVER`     | Cuppen's Divide & Conquer | Analytical (FSES)  |
| `FSESANALYTICALEIGENSOLVER` | Analytical			      | Analytical (FSES)  |
| `FSESHYBRIDEIGENSOLVER`     | Hybrid				      | Analytical (FSES)  |
| `GTESYMMETRICQREIGENSOLVER` | Symmetric QR              | Analytical (TFEL)  |
: List of available eigen solvers. {#tbl:eigensolvers}

The various eigen solvers available are enumerated in Table
@tbl:eigensolvers.

The eigen solver is passed as a template argument of the
`computeEigenValues` or the `computeEigenVectors` methods as
illustrated in the code below:

~~~~~{.cpp}
tmatrix<3u,3u,real> m2;
tvector<3u,real>    vp2;
std::tie(vp,m)=s.computeEigenVectors<stensor::GTESYMMETRICQREIGENSOLVER>();
~~~~~

> **Note**
>
> In `2D`, the last eigenvector always corresponds to the out-of-plane
> direction.

The `computeEigenVectors` method can also order the eigenvalues in
ascending or descending order, see the `computeEigenValues` method for
details.

> **Note**
>
> In `1D`, the ordering parameter has no effect.
> In `2D`, the last eigenvector always corresponds to the out-of-plane
> direction.

## Isotropic functions of a symmetric tensor

Given a scalar valuated function \(f\), one can define an associated
isotropic function for symmetric tensors as:
\[
f\paren{\tenseur{s}}=\sum_{i=1}^{3}f\paren{\lambda_{i}}\tenseur{n}_{i}
\]

where \(\left.\lambda_{i}\right|_{i\in[1,2,3]}\) are the eigen values
of the symmetric tensor \(\tenseur{s}\) and
\(\left.\tenseur{n}_{i}\right|_{i\in[1,2,3]}\) the associated eigen
tensors.

If \(f\) is \(\mathcal{C}^{1}\), then \(f\) is a differentiable
function of \(\tenseur{s}\).

\(f\) can be computed with the `computeIsotropicFunction` method of
the stensor class. \(\deriv{f}{\tenseur{s}}\) can be computed with
`computeIsotropicFunctionDerivative`. One can also compute \(f\) and
\(\deriv{f}{\tenseur{s}}\) all at once by the
`computeIsotropicFunctionAndDerivative` method. All those methods are
templated by the name of the eigen solver (if no template parameter is
given, the `TFELEIGENSOLVER` is used).

Various new overloaded versions of those methods have been
introduced. Those overloaded methods are meant to:

- allow the user to explicitly give the values of \(f\) or \(df\),
  rather than the functions to compute them. This allows to reduce the
  computational cost of the evaluation of the isotropic function when
  the values of the derivatives can directly be computed from the
  values of \(f\). See the example \(\exp\) example below.
- return the results by value. This allow a much more readable code if
  the *structured bindings* feature of the `C++17` standard is
  available.

To illustrate this new features, assuming that the *structured
bindings* feature of the `C++17` standard is available, one can now
efficiently evaluate the exponential of a symmetric tensor and its
derivative as follows:

~~~~{.cpp}
const auto [vp,m] = s.computeEigenVectors();
const auto evp    = map([](const auto x){return exp(x)},vp);
const auto [f,df] = Stensor::computeIsotropicFunctionAndDerivative(evp,evp,vp,m,1.e-12);
~~~~

# Special operations for mechanical behaviours

## Yield criteria

### von Mises stress @wikipedia_von_Mises_2017

The von Mises stress is defined by:
\[
\sigmaeq=\sqrt{\Frac{3}{2}\,\tenseur{s}\,\colon\,\tenseur{s}}=\sqrt{3\,J_{2}}
\]
where:

- \(\tenseur{s}\) is the deviatoric stress defined as follows:
\[
\tenseur{s}=\tsigma-\Frac{1}{3}\,\trace{\tsigma}\,\tenseur{I}
\]
- \(J_{2}\) is the second invariant of \(\tenseur{s}\) (see
  also Section&nbsp;@sec:deviatoric:invariants).

The previous expression can be rewritten by introducing a fourth order
tensor called \(\tenseurq{M}\):
\[
\sigmaeq=\sqrt{\sigma\,\colon\,\tenseurq{M}\,\colon\,\tsigma}
\]

The tensor \(\tenseurq{M}\) is given by:
\[
\tenseurq{M}=\Frac{3}{2}\,\paren{\tenseurq{I}-\Frac{1}{3}\,\tenseur{I}\,\otimes\,\tenseur{I}}
\]

The tensor \(\tenseurq{M}\) is accessible through the `M` `constexpr`
`static` method of the `st2tost2` class, as follows:

~~~~{.cpp}
constexpr const auto M = st2tost2<N,real>::M();
~~~~

In terms of the eigenvalues of the stress, denoted by \(\sigma_{1}\),
\(\sigma_{2}\) and \(\sigma_{3}\), the von Mises stress can also be
defined by:
\[
\sigmaeq=\sqrt{\Frac{1}{2}\paren{\absvalue{\sigma_{1}-\sigma_{2}}^{2}+\absvalue{\sigma_{1}-\sigma_{3}}^{2}+\absvalue{\sigma_{2}-\sigma_{3}}^{2}}}
\]

The von Mises stress can be computed using the `sigmaeq` function, as
follows:

~~~~{.cpp}
const auto seq = sigmaeq(s);
~~~~

The derivative \(\tenseur{n}\) of the von Mises stress with respect to
the stress is called the normal and is given by:
\[
\tenseur{n}=\deriv{\sigmaeq}{\tsigma}=\Frac{3}{2\,\sigmaeq}\,\tenseur{s}=\Frac{1}{\sigmaeq}\,\tenseur{M}\,\colon\,\tsigma
\]

The normal can be computed by:

~~~~{.cpp}
const auto n = eval(3*deviator(sig)/(2*seq));
~~~~

> **Note** The `eval` function is used to evaluate the
> normal. Otherwise, the expression template mechanism used by `TFEL`
> would delay its evaluation.

Another way to compute it is to use the \(\tenseurq{M}\) tensor, as
follows:

~~~~{.cpp}
const auto n = eval(M*sig/seq);
~~~~

The second derivative of the von Mises stress with respect to the von
Mises stress is given by:

\[
\sderiv{\sigmaeq}{\tsigma}=\Frac{1}{\sigmaeq}\paren{\tenseur{M}-\tenseur{n}\otimes\tenseur{n}}
\]

This second derivative can be computed as follows:

~~~~{.cpp}
const auto dn_ds = eval((M-(n^n))/seq);
~~~~

### Hill stress

The Hill tensor \(\tenseurq{H}\) is defined by:
\[
\tenseurq{H}=
\left(
\begin{array}{cccccc}
F+H & -F  & -H  & 0 & 0 & 0 \\
-F  & G+F & -G  & 0 & 0 & 0 \\
-H  & -G  & H+G & 0 & 0 & 0 \\
0   & 0   & 0   & L & 0 & 0 \\
0   & 0   & 0   & 0 & M & 0 \\
0   & 0   & 0   & 0 & 0 & N \\
\end{array}
\right)
\]

The Hill stress \(\sigmaeq^{H}\) is defined by:
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

The first derivative of the Hill stress is given by:
\[
\tenseur{n}^{H}=\deriv{\sigmaeq^{H}}{\tsigma}=\Frac{1}{\sigmaeq^{H}}\,\tenseur{H}\,\colon\,\tsigma
\]

The second derivative of the Hill stress is given by:
\[
\sderiv{\sigmaeq^{H}}{\tsigma}=\Frac{1}{\sigmaeq^{H}}\,\paren{\tenseur{H}-\tenseur{n}^{H}\,\otimes\,\tenseur{n}^{H}}
\]

The header `TFEL/Material/Hill.hxx` introduces various functions to
build the Hill tensor:

- `hillTensor` or `makeHillTensor`, which has:
    - two template parameters: the space dimension and the underlying
      numeric type.
	- the six arguments giving the Hill coefficients \(F\), \(G\),
      \(H\), \(L\), \(M\), \(N\).
- `computeHillTensor` or `makeHillTensor`, which has:
    - three template parameters: the modelling hypothesis, the
      orthotropic axis convention, and the underlying numeric type.
	- the six arguments giving the Hill coefficients \(F\), \(G\),
      \(H\), \(L\), \(M\), \(N\).

> **Note** In `MFront`, one shall use the `@HillTensor` to compute the
> `Hill` tensor, which takes into account the modelling hypothesis and
> the orthotropic axis convention.

### Hosford stress

![Comparison of the Hosford stress \(a=100,a=8\) and the von Mises stress](img/HosfordStress.svg
 "Comparison of the Hosford stress \(a=100,a=8\) and the von Mises
 stress in plane stress"){width=70%}

The header `TFEL/Material/Hosford1972YieldCriterion.hxx` introduces
three functions which are meant to compute the Hosford equivalent
stress and its first and second derivatives. *This header is
automatically included by `MFront`.*

The Hosford equivalent stress is defined by (see @hosford_generalized_1972):
\[
\sigmaeq^{H}=\sqrt[a]{\Frac{1}{2}\paren{\absvalue{\sigma_{1}-\sigma_{2}}^{a}+\absvalue{\sigma_{1}-\sigma_{3}}^{a}+\absvalue{\sigma_{2}-\sigma_{3}}^{a}}}
\]
where \(\sigma_{1}\), \(\sigma_{2}\) and \(\sigma_{3}\) are the eigenvalues of the
stress.

Therefore, when \(a\) goes to infinity, the Hosford stress reduces to
the Tresca stress. When \(n = 2\) the Hosford stress reduces to the
von Mises stress.

The following function has been implemented:

- `computeHosfordStress`: return the Hosford equivalent stress
- `computeHosfordStressNormal`: return a tuple containg the Hosford
  equivalent stress and its first derivative (the normal)
- `computeHosfordStressSecondDerivative`: return a tuple containg the
  Hosford equivalent stress, its first derivative (the normal) and the
  second derivative.

The implementation of those functions are greatly inspired by the work
of Scherzinger (see @scherzinger_return_2017). In particular, great
care is given to avoid overflows in the computations of the Hosford
stress.

Those functions have two template parameters:

- the type of symmetric tensors used for the stress tensor
  (automatically deduced, but required if the second parameter is
  specified).
- the eigen solver to be used (See Section&nbsp;@sec:eigensolvers).

### Barlat stress

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
\Frac{1}{3}\,
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

The following function has been implemented:

- `computeBarlatStress`: return the Barlat equivalent stress.
- `computeBarlatStressNormal`: return a tuple containg the Barlat
  equivalent stress and its first derivative (the normal).
- `computeBarlatStressSecondDerivative`: return a tuple containg the
  Barlat equivalent stress, its first derivative (the normal) and the
  second derivative.

The implementation of those functions are greatly inspired by the work
of Scherzinger (see @scherzinger_return_2017). In particular, great
care is given to avoid overflows in the computations of the Barlat
stress.

Those functions have two template parameters:

- the type of symmetric tensors used for the stress tensor
  (automatically deduced, but required if the second parameter is
  specified).
- the eigen solver to be used (See Section&nbsp;@sec:eigensolvers).

#### Linear transformations

To define the linear transformations, the
`makeBarlatLinearTransformation` function has been introduced. This
function takes two template parameter:

- the space dimension (\(1\), \(2\), and \(3\))
- the numeric type used (automatically deduced)

This functions takes the \(9\) coefficients as arguments, as follows:

~~~~{.cpp}
const auto l1 = makeBarlatLinearTransformation<3>(c_12,c_21,c_13,c_31,
                                                  c_23,c_32,c_44,c_55,c_66);
~~~~

> **Note** In his paper, Barlat and coworkers seems to use the
> following convention for storing symmetric tensors:
> 
> \[
> \begin{pmatrix}
> xx & yy & zz & yz & zx & xy
> \end{pmatrix}
> \]
> 
> which is not consistent with the
> `TFEL`/`Cast3M`/`Abaqus`/`Ansys` conventions:
> 
> \[
> \begin{pmatrix}
> xx & yy & zz & xy & xz & yz
> \end{pmatrix}
> \]
> 
> Therefore, if one wants to uses coeficients \(c^{B}\) given
> by Barlat, one shall call this function as follows:
> 
> ~~~~{.cpp}
> const auto l1 = makeBarlatLinearTransformation<3>(cB_12,cB_21,cB_13,cB_31,
>                                                   cB_23,cB_32,cB_66,cBB_55,cBB_44);
> ~~~~

The `TFEL/Material` library also provide an overload of the
`makeBarlatLinearTransformation` which template parameters are the
modelling hypothesis and the orthotropic axis conventions. The purpose
of this overload is to swap appriopriate coefficients to get a
consistent definition of the linear transforamtions for all the
modelling hypotheses.

# References

<!-- Local IspellDict: english -->



