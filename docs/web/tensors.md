% Second and fourth-order tensor objects in `TFEL/Math`
% 2/08/2017
% Thomas Helfer

\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tns}[1]{{\underset{\tilde{}}{\mathbf{#1}}}}
\newcommand{\transpose}[1]{{#1^{\mathop{T}}}}

\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sderiv}[2]{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial #2^{2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}

\newcommand{\a}{\tns{a}}
\newcommand{\s}{\tenseur{s}}
\newcommand{\c}{\tenseur{c}}

This page is meant to describe the various tensor objects and
operations available in `TFEL/Math`.

# Classes describing second and fourth order tensors

## Symmetric second order tensors

When dealing with constitutive equations, most computations are
performed on *symmetric* tensors.

Classes describing symmetric second order tensors satisfies the
`StensorConcept`. 

An end user will mostly use the `stensor` class, which have the
following template arguments:

- The space dimesion (`1`, `2` or `3`).
- The type used to perform the computation.

Symmetric tensors are denoted as follows \(\s\).

### Special values

## General (non symmetric) second order tensors

Classes describing symmetric second order tensors satisfies the
`TensorConcept`. 

An end user will mostly use the `tensor` class, which have the
following template arguments:

- The space dimesion (`1`, `2` or `3`).
- The type used to perform the computation.

Non symmetric tensors are denoted as follows \(\a\).

### Special values

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

### Special values

# Standard operations

## Basic operations

Thanks to operator overloading, the following operations are written
using standard mathematical notations:

- Addition of two tensors
- Subtraction of two tensors
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

## Symmetrization and unsymmetrization of second order tensors

A non symmetric second order tensor can be symmetrized using the
`syme` function. The result match the `StensorConcept`.

A symmetric second order tensor can be unsymmetrized using the
`unsyme` function. The result match the `TensorConcept`.

## Diadic product

## Contracted products of second order tensors

## Application of a fourth order tensor

## Multiplication of second order tensors

### Derivatives

# Special functions

## Square of a symmetric tensor

The product of two symmetric tensors is a non symmetric
tensor. However, the square of a symmetric tensor is a symmetric
tensor.

The square of a symmetric tensor can be computed using the `square`
function, as follows:

~~~~{.cpp}
const auto s2 = square(s);
~~~~

## Derivative of the square of a symmetric tensor

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

## Transposition of a second order tensor {#sec:transpose}

A non symmetric second order tensor can be transpose using the
`transpose` function:

~~~~{.cpp}
const auto B = transpose(A);
~~~~

## Second order tensor invariants

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

### Derivatives of the invariants of a tensor

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
`computeJacobianSecondDerivative` function, as follows:

~~~~{.cpp}
const auto d2I3_dA2 = computeDeterminantSecondDerivative(A);
~~~~

## Eigenvalues, eigenvectors and eigentensors of symmetric tensors

## Isotropic functions of a symmetric tensor

### Definition

### Computation

### Derivative of an isotropic function of a symmetric

<!-- Local IspellDict: english -->



