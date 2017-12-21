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

- The space dimesion (`1`, `2` or `3`).
- The type used to perform the computation.

Symmetric tensors are denoted as follows \(\s\).

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

\tenseur{a}\,\star_{1}\,\tenseur{b} = \Frac{1}{2}\paren{a\,.}

### Derivatives

## Symmetric product of two symmetric second order tensors

The symmetric product of two symmetric second order tensors
\(\tenseur{a}\) and \(\tenseur{b}\) can be defined as follows:

\[
\tenseur{a}\,\cdot_{s}\,\tenseur{b} = \Frac{1}{2}\paren{\tenseur{a}\,\cdot\tenseur{b}+\tenseur{b}\,\cdot\tenseur{a}}
\]

This can be computed by the `symmetric_product` function.

### Derivative

The derivative of the symmetric
\(\tenseur{a}\,\cdot_{s}\,\tenseur{b}\) with respect to
\(\tenseur{a}\) can be computed using the `st2tost2::stpd` static
method with takes \(\tenseur{b}\) as argument.

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

### von Mises stress

The von Mises stress is defined by:
\[
\sigmaeq=\sqrt{\Frac{3}{2}\,\tenseur{s}\,\colon\,\tenseur{s}}
\]
where \(\tenseur{s}\) is the deviatoric stress defined as follows:
\[
\tenseur{s}=\tsigma-\Frac{1}{3}\,\trace{\tsigma}\,\tenseur{I}
\]

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
defined by (see @hosford_generalized_1972):
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

The header `TFEL/Material/Hosford.hxx` introduces three functions
which are meant to compute the Hosford equivalent stress and its first
and second derivatives. *This header is automatically included by
`MFront`*

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

The linear transformations of the deaviator stress \(\tenseurq{C}'\)
and \(\tenseurq{C}''\), are defined as follows:
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

> **Note** In his paper, Barlat and coworkers uses the following convention for
> storing symmetric tensors:
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



