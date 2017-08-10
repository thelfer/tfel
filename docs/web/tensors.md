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

### The identity tensor

The symmetric second order identity tensor is returned by the `Id`
static member of the `stensor` class.

## General (non symmetric) second order tensors

Classes describing symmetric second order tensors satisfies the
`TensorConcept`. 

An end user will mostly use the `tensor` class, which have the
following template arguments:

- The space dimesion (`1`, `2` or `3`).
- The type used to perform the computation.

Non symmetric tensors are denoted as follows \(\a\).

### The identity tensor

The symmetric second order identity tensor is returned by the `Id`
static member of the `stensor` class.

### The identity tensor

The second order identity tensor is returned by the `Id` static member
of the `tensor` class.

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

##  Frobenius inner product of second order tensors

The Frobenius inner product \(\tns{a}\,\colon\,\tns{b}\) of two tensors
\(\tns{a}\) and \(\tns{b}\) is defined by:

\[
\tns{a}\,\colon\,\tns{b}=\trace{\transpose{\tns{a}}\,\dot\,\tns{b}}=\sum_{i,j}a_{ij}b_{ij}
\]

This operation is implemented in `TFEL/Math` using the `^` operator,
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
\begin{array}
\,\tns{c}\,\colon\,\paren {\tns{a}\,\otimes\,\tns{b}}=\paren{\tns{c}\,\colon\,\tns{a}}\,\tns{b} \\
\,\paren {\tns{a}\,\otimes\,\tns{b}}\,\colon\,\tns{c}=\paren{\tns{c}\,\colon\,\tns{b}}\,\tns{a} \\
\end{array}
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

### Eigenvalue

The eigenvalues can be computed by the `computeEigenValues` method, as
follows:

~~~~{.cpp}
const auto vp = s.computeEigenValues();
~~~~

Those eigen values can be ordered by using one of the following argument:

- `ASCENDING`: eigenvalues are sorted from the lowest to the greatest.
- `DESCENDING`: eigenvalues are sorted from the greatest to the lowest.

~~~~{.cpp}
const auto vp = s.computeEigenValues(stensor::ASCENDING);
~~~~

By default, the eigenvalues are computed using Cardano
formula. However, one may use one of the following eigensolver
decribed in the next paragraph as follows:

~~~~{.cpp}
constexpr const auto es = stensor<3u,real>::FSESQLEIGENSOLVER;
const auto vp = s.computeEigenValues<es>();
~~~~

### Eigenvectors

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

# References

<!-- Local IspellDict: english -->



