---
title:  Overview of some features of the `TFEL/Math` library
author: Thomas Helfer
date: 21/11/2017
lang: en-EN
numbersections: true
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
lstPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
header-includes:
bibliography: bibliography.bib
csl: iso690-numeric-en.csl
---

\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tns}[1]{{\underset{\tilde{}}{\mathbf{#1}}}}
\newcommand{\tnsq}[1]{{\underset{\tilde{}}{\underset{\tilde{}}{\mathbf{#1}}}}}
\newcommand{\transpose}[1]{{#1^{\mathop{T}}}}

\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\sderiv}[2]{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial #2^{2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\absvalue}[1]{{\left|#1\right|}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}

\newcommand{\ta}{\tns{a}}
\newcommand{\ts}{\tenseur{s}}
\newcommand{\tc}{\tenseur{c}}

This overview of the `TFEL/Math` library mostly focuses on two important
features:

- The `TFEL/Math` library provides a linear algebra engine with
  mathematical objects and operations on those objets required to
  express the constitutive equations in an efficient and natural manner,
  i.e. as close as possible to the mathematical expressions. Section
  @sec:tfel_math:math_objects is devoted to describe this linear
  algebra engine.
- The `TFEL/Math` library provides efficient and robust non-linear
  solvers for small sized problems described in Section
  @sec:tfel_math:non_linear_solvers.

This features are described in this document. Other modules of the
library are described in the following pages:

- [data interpolation](tfel-math-data-interpolation.html),
- [numerical integration](tfel-math-numerical-integration.html).

# Mathematical objects {#sec:tfel_math:math_objects}

The mathematical objects provided by the `TFEL/Math` library are based
on scalar values which can be standard `C++` numeric types (`float`,
`double` or `long double`), or so-called quantities, i.e. an object
describing a value with a unit. The latter allows to distinguish
physical quantities such as a strain from a stress and prevent illegal
operations between them. Quantities are described in depth in Section
@sec:tfel_math:quantities.

The `TFEL/Math` library then introduces some mathematical objects of
rank 1, i.e. objects which can be stored in a contiguous memory location
and access by a single index, such as:

- fixed size vectors (implemented by the `tvector` class)
- symmetric tensors (implemented by the `stensor` class)
- unsymmetric tensors (implemented by the `tensor` class)

Higher order objects are then defined as the derivatives of objects of
lower ranks. For instance, the derivative of a symmetric tensor with
respect to an unsymmetric tensor is implemented by the `t2tost2` class.
The latter can also be viewed a linear application between unsymmetric
tensor and symmetric tensor or as a fourth order tensor. In practice,
there are four kind of fourth order tensor defined in the library
respectively implemented by the `st2tost2`, `st2tot2`, `t2tost2`, and
`t2tot2` classes.

Currently only tensors up to the fourth order are specialized which
allows to provide some specific features.

Higher order tensors may be represented by a generic class which
represent derivatives of two mathematical objects. Currently, the number
of operations allowed by this generic class is rather limited and will
be extended as needed in future versions.

The main concepts behind the implementation of the mathematical objects
are described in Section @sec:tfel_math:tensorial_objects.

## Quantities {#sec:tfel_math:quantities}

Quantities were introduced in `TFEL/Math` following ideas of the book D.
Abrahams and A. Gurstovoy [@abrahams_cxx_2004] to allow dimensional
analysis at compile time. Basically, a quantity is the combination of a
value and a unit. The type of the value is called the base type of the
quantity.

The unit is encrypted in a type called `Unit` so that the `C++`
type-system can be used to detect at compile-time (without
runtime-checks) if operations are legal. According to the International
System of Units (SI), a unit is decomposed along \(7\) basic units
which are the mass (kg), the length (l), the time (s), the electric
current (A), the temperature (K), the luminous intensity (cd) and the
amount of substance (mole). To be able to describe fractional unit, such
as the fracture toughness which is has the unit of a stress times the
square root of a length, the `Unit` class is parametrized by \(14\)
integers. The `TFEL/Math` library provides some convenient aliases for
the most common units.

The `NoUnit` unit is a special case where all those integers are set to
0 for the numerator parts and 1 (by convention) for the denominator
parts.

### The `qt` class

A quantity is presented by the `qt` class which is parametrized by a
class describing the unit and a numeric base type (generally `float`,
`double` or `long double`) which stores the value of the quantity. By
default, the `double` type is used. This class can be used as follows:

~~~~{.cxx}
constexpr qt<Mass> m1(100.);
constexpr qt<Mass> m2(100.);
constexpr qt<Mass> m3 = m1 + 0.5 * m2;
constexpr qt<Acceleration> a(2);
constexpr qt<Force> f = m1 * a;
~~~~

The previous code sample illustrates how to declare a new quantity and
how to perform operations on quantities. It also highlights that those
operations can be used in a `constexpr` context, i.e. that at
compile-time.

### The `qt_ref` and `const_qt_ref` classes

The library also provides two wrapper classes called `qt_ref` and
`const_qt_ref` which allows to wrap a value into an object which acts
respectively as a mutable or an immutable quantity, as illustrated in the
following code sample:

~~~~{.cxx}
constexpr auto eps = 1e-14;
constexpr auto value = [] ()constexpr {
 constexpr qt<Mass> m(100.);
 constexpr qt<Acceleration> a(2);
 auto vf = double{};
 auto f = qt_ref<Force>(vf);
 f = m * a;
 return vf;
}();
TFEL_TESTS_STATIC_ASSERT(std::abs(value - 200.) < eps);
~~~~

While a bit contrived, this example shows that the `qt_ref` class can
also be used in a `constexpr` context.

The `qt_ref` and `const_qt_ref` classes are parametrized by two template
parameters which describe respectively the unit and a numeric base type.

The `qt_ref` and `const_qt_ref` classes are essentially used to build views of
mathematical objects from a raw memory area, as described in Section
@sec:tfel_math:views.

### The `Quantity` class

For the sake of simplicity, the `qt`, `qt_ref` and `const_qt_ref` were
described as classes. This is not actually the case in the current
implementation which defines them through type aliases to a more general
`Quantity` class which is parametrized by three template arguments: the
unit, the basic numeric type and a third argument describing how the
value associated with the quantity is handled.

### Operations on quantities

Common operations on quantities, such as additions, subtraction,
multiplications, division and negations are supported.

Scaling and multiplication by a standard numeric value works as
expected, as demonstrated by the following code:

~~~~{.cxx}
constexpr qt<Mass> m(100.);
// multiplication by a raw numeric type
auto m2 = 2. * m;
// scaling
m2 *= 4;
~~~~

A quantity with unit `NoUnit` is implicitly convertible to its base type:

~~~~{.cxx}
constexpr qt<NoUnit> q(1.2);
constexpr double q_value = q;
const auto cos_q = std::cos(q);
~~~~

The latter line shows that standard mathematical functions can be called
directly.

Fractional power of a quantity are supported through the `power`
function as follows:

~~~~{.cxx}
// declaration of a stress symmetric tensor
stensor<3u,qt<Stress>> s;
// contracted product of s which has the unit
// of the square of a stress
const auto s2 = s|s;
// norm of s
const auto norm = power<1,2>(s2);
~~~~

> **About the `power` function**
>
> The `power` function is a very convenient function which takes two
> template parameters `N` and `D` defining the exponent as the fraction
> \(N/D\). The default value of `D` is \(1\), so the following syntax
> is valid:
> 
> ~~~~{.cxx}
> auto v = power<3>(12);
> ~~~~
>
> which computes \(12^{3}\).
>
> The `power` function is optimised for the following values of the denominator:
>
> - \(1\): In this case, the `Nth` power of the argument is computed
>   using by multiplications if \(N\) is positive. If \(N\) is negative, 
>   the `Nth` power of the inverse of the argument is computed.
> - \(2\): In this case, the square root of the `Nth` power of the
>   argument is computed using the `std::sqrt` function.
> - \(3\): In this case, the square root of the `Nth` power of the
>   argument is computed using the `std::cbrt` function.

## A generic framework to build tensorial-like objects {#sec:tfel_math:tensorial_objects}

This section first introduces the notion of array policy and indexing
policy which describes respectively how data are handled (stored) and
how those data are accessed.

Common objects of rank \(1\) are then defined, including tiny vectors,
symmetric tensors and unsymmetric tensors. Higher order objects are
defined recursively as derivative of objects of lower ranks.

### Description of the `ArrayPolicy` concept

Array policies describes how data are handled and accessed. For the
latter, the description of the access pattern is delegated to an
indexing policy which will be described later in Section
@sec:tfel_math:indexing_policies. Thus, this section mostly focuses
on how array policies describe how the data are handled.

A class matching the `ArrayPolicy` concept must provide:

- An alias to a type called `value_type` which corresponds to the
  logical type hold by the array (see below).
- An alias to a type called `const_value_type` which corresponds to a
  constant value to an object of the logical type hold by the array.
- An alias to a type called `storage_type` which is the type actually
  stored in memory.
- An alias to a type called `reference` which is the type returned by
  the non-constant access operators of the array. An object of the
  `reference` type must be convertible and assignable to a `value_type`
  object and provide the following operators:
  - `operator=(const other_type&)`
  - `operator+=(const other_type&)`
  - `operator-=(const other_type&)`
  - `operator*=(const other_type&)`
  - `operator/=(const other_type&)`

  where `other_type` denotes for each operator a type for which the
  considered operator in the `value_type` class is valid.
- An alias to a type called `const_reference` which is the type returned
  by the constant access operators of the array. An object of the
  `const_reference` type must be convertible and assignable to a
  `value_type` object.
- A static function called `make_const_reference` which turns a
  constant reference to a `storage_type` object into a `const_reference`
  object.
- A static function called `make_reference` which turns a reference to a
  `storage_type` object into a `reference` object.
- An alias type a type called `IndexingPolicy` which refers an indexing
  policy. The `IndexingPolicy` concept is described in depth in Section
  @sec:tfel_math:indexing_policies.

It is worth illustrating those concepts in two commonly used cases:

- Standard array policies.
- Array policies used by views.

### Standard array policies

The `StandardArrayPolicy` class is based in the following statements:

- `storage_type` is the same as `value_type`.
- `reference` is the same as `value_type&`.
- `const_reference` is the same as `const value_type&`.
- `make_reference` is trivial an only returns a const reference to the
  stored value.
- `make_const_reference` is trivial an only returns an immutable
  reference to the stored value.

Standard array policies are which are used to implement concrete
mathematical objects such as `tvector`, `tmatrix`, `stensor`, `tensor`,
etc.

> **Note**
>
> Contrary to view array policies described in the next paragraph, the
> `storage_type` is not restricted to be a basic numeric type (`float`,
> `double` or `long double`).

### View array policies

Views generally maps a memory location made of basic numeric types
(`float`, `double` or `long double`) to a mathematical objects.

To support quantities, the type stored (`storage_type`) must be
distinguished from the logical type of the mapped object (`value_type`).
The reference type is then a simple wrapper around a numeric value which
acts as the desired quantity.

For example, let us consider the case of a view mapping a stress
symmetric tensor from an array of double precision numbers. In this
case, `storage_type` is the base type of `value_type`, so:

- `storage_type` is an alias to `double`.
- `value_type` is an alias to `qt<Stress, double>`.
- `reference` is an alias to `qt_ref<stress, double>`.
- `const_reference` is an alias to `const_qt_ref<stress, double>`.
- `make_reference` simply builds a reference for the address of the
  stored value. So does `make_const_reference`.

This mostly describes the implementation of the `ViewArrayPolicy` class.

However, those rules only applies when `storage_type` is a quantity.
Otherwise, i.e. when `value_type` is a basic numeric type, the
implementation of the `ViewArrayPolicy` class is equivalent to the
`StandardArrayPolicy` class.

## Description of the `IndexingPolicy` concept {#sec:tfel_math:indexing_policies}

A class matching the `IndexingPolicy` concept must provide:

- An alias to a type called `size_type` to the type used to index data.
- An alias to a type called `RowMajorIndexingPolicy` which can be used
  to import data from raw `C` pointers.
- A static member data called `arity` of type `size_type` which
  describes the number of indices required to describe the object.
- A static boolean data member called `hasFixedSizes`. If this data
  member is `true`, the class must be stateless, i.e. empty.
- A static boolean data member called `areDataContiguous` which states
  if the data is continuous
- A member function called `size` which takes no argument returning the
  number of data values accessible by the indexing policy. This member
  function must be `constexpr` if `hasFixedSizes` is true.
- A member function called `size` which takes a `size_type` argument
  returning range in the given dimension. This member function must be
  `constexpr` if `hasFixedSizes` is true.
- A member function called `getUnderlyingArrayMinimalSize` which returns
  the minimal size of an array required by the indexing policy. This
  member function must be `constexpr` if `hasFixedSizes` is true.
- A member function called `getIndex` which returns the memory offset of
  an element of the object with respect to the location of the first
  element of the object. The number of argument of this method must be
  equal to the `arity` member. This member function must be `constexpr`
  if `hasFixedSizes` is true.

### An example of indexing policy

![Storage of the elements of a matrix using the row-major format](img/tfel-math/matrix_row_major.svg){#fig:tfel_math:matrix_row_major width=90%}

Let us consider an \(N\,\times\,M\) matrix stored in a contiguous memory
location using the row major format, i.e. all elements of the same row
are stored continuously as depicted on Figure
@fig:tfel_math:matrix_row_major. The position of \(i,j\) element from
the start of the memory area is given by \(i \, M + j\).

Let us now consider a view on a \(L\,\times\,K\) submatrix in a which
starts at row \(i0\) and column \(j0\). The position of the \(i,j\)
element of the submatrix in the matrix is also given by \(i \, M + j\)
if we start from the first element of the submatrix.

This formula is implemented by the `getIndex` method of the
`FixedSizeRowMajorMatrixIndexingPolicy` which is used by many classes
provide by the library, such as `tmatrix` (matrices whose size is known
at compile time), `st2tost2`, `t2tot2`, etc. and views to those objects
in a matrix.

### Higher order indexing policies for fixed-size mathematical objects

Let us consider a mathematical object \(o_{3}\) resulting from the
derivation of a mathematical object \(o_{1}\) of arity \(a_{1}\) with
respect to a mathematical object \(o_{2}\) of arity \(a_{2}\).
\(o_{3}\) has an arity of \(a_{1}+a_{2}\).

The storage of those objects are described respectively by the indexing
policies \(p_{1}\) and \(p_{2}\). We assume that the object \(o_{1}\)
can be stored in a memory area of size \(s_{1}\) and that the object
\(o_{2}\) can be stored in a memory area of size \(s_{2}\). Then the
object \(o_{3}\) can be stored in a memory location \(s_{1}\,s_{2}\).

Then, an indexing policy \(p_{3}\) suitable to describe \(o_{3}\) may
compute the position of the derivative of the component
\(o_{1}\paren{i_{0},\ldots,i_{a_{1}-1}}\) with respect to the component
\(o_{2}\paren{j_{0},\ldots,j_{a_{2}-1}}\) is given by:

\[
p_{3}\paren{i_{0},\ldots,i_{a_{1}-1},j_{0},\ldots,j_{a_{2}-1}}=
p_{1}\paren{i_{0},\ldots,i_{a_{1}-1}}\,s_{2}+p_{2}\paren{j_{0},\ldots,j_{a_{2}-1}}
\]

This choice is implemented in the
`FixedSizeIndexingPoliciesCartesianProduct` class which is used to by
the `FixedSizeArrayDerivative` class to describe derivatives of two
arbitrary mathematical objects.

## Low rank mathematical objects

The library is based on a few low rank mathematical objects:

- `tvector`: a tiny vector of fixed size. This template class is
  parametrized by the size of the vector and the value hold.
- `stensor`: a symmetric tensor. This template class is parametrized by
  the space dimension and the scalar type.
- `tensor`: an unsymmetric tensor. This template class is parametrized by
  the space dimension and the scalar type.

All those objects are represented by objects of rank one using a
vector-like notations. For example, a \(3D\) symmetric tensor is
represented as follows:

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

This notation has the property that the contracted product of two
symmetric tensors is the scalar product of this vector form (hence the
\(\sqrt{2}\)).

In a similar manner, an unsymmetric tensor is represented as follows:
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

## Higher order objects defined as derivatives

The library provides a generic class called `FixedSizeArrayDerivative`
which allows to create higher order objects as being the derivative of
two objects of lowest ranks.

This class is currently very limited but will be extended in future
versions of the library.

### The case of fourth order tensors

Fourth order tensors can be defined as derivatives of two tensors or as
linear mappings from the second order tensors to second order tensors.

As there are two kinds of second order tensors (i.e. symmetric and
non-symmetric tensors), there are four kinds of fourth order tensors defined
in the `TFEL/Math` library, which satisfy the following concepts:

- `ST2toST2Concept`: linear mapping from symmetric tensors to
  symmetric tensors.
- `ST2toT2Concept`: linear mapping from symmetric tensors to
  non-symmetric tensors.
- `T2toST2Concept`: linear mapping from non-symmetric tensors to
  symmetric tensors.
- `T2toT2Concept`: linear mapping from non-symmetric tensors to
  non-symmetric tensors.

An end user will mostly use the following implementations of those
concepts: `st2tost2`, `st2tot2`, `t2tost2` and `t2tot2`
respectively. Those classes have the following template arguments:

- The space dimension (`1`, `2` or `3`).
- The type used to perform the computation.

### The `tmatrix` case

A fixed size matrix, implemented by the `tmatrix` class can be seen as
the derivative of a tiny tensor with respect to a tiny vecor.

### The `derivative_type` metafunction

The `TFEL/Math` library provides a very convenient type alias called
`derivative_type` which automatically selects the correct type as the
derivative of two objects of fixed sizes. This alias also works with
scalars. This type alias takes quantities into account if required.

Here are a few examples:

- `derivative_type<double, double>` is an alias to `double`.
- `derivative_type<qt<Stress>, double>` is an alias to `qt<Stress>`.
- `derivative_type<qt<Stress>, qt<Time>>` is an alias to `qt<StrainRate>`.
- `derivative_type<tvector<2u, double>, tvector<3u, double>>` is an alias
  to `tmatrix<2u, 3u, double>`.
- `derivative_type<stensor<2u, double>, stensor<2u, double>>` is an
  alias to `st2tost2<2u, double>>`.
- `derivative_type<stensor<2u, double>, tvector<3u, double>>` is an
  alias to `FixedSizeArray` `Derivative<stensor<2u, double>, tvector<3u,
  double>>`.

## Expressions templates {#sec:tfel_math:expression_templates}

One may expect that the addition of two vector results in a new vector.
This naive approach may lead to poor performances, due to temporaries
objects and data copying [@veldhuizen_techniques_1999].

Expression templates is a C++ template metaprogramming technique which
introduces additional classes which represent the actions to be
performed on some objects and lazily delay the execution of those
actions until the result is explicitly requested.

The objects of those classes are placeholders, also called handlers
within the library, that are meant to be assigned to an object whose
type is the expected result of the operation treated.

To illustrate this technique, let us consider the addition of three
vectors \(\vec{a}\), \(\vec{b}\) and \(\vec{c}\)
and its assignment to a vector \(\vec{d}\):

~~~~{.cxx}
d = a + b + c;
~~~~

The addition of the vectors \(\vec{a}\) and \(\vec{b}\) produces an
intermediate object \(\vec{e1}\) of type `Expr1` which keeps a reference
to those two vectors. Similarly, the addition of those three vectors
defines another object \(\vec{e2}\) of type `Expr2` which stands for the
addition of \(\vec{e1}\) and the vector \(\vec{c}\).

![Application of the expression templates technique to the addition of
three
vectors.](img/tfel-math/expression_templates.svg){#fig:tfel_math:expression_templates
width=50%}

Figure @fig:tfel_math:expression_templates shows how the access operator of
`e2` is implemented. The assignment of an object of type `Expr2` to a
vector `d` is implemented as a standard `for` loop:

~~~~{.cxx}
for(size_type i=0;i!=d.size();++i){
  d(i) = e2(i);
}
~~~~

The temporary objects `e1` and `e2` are meant to be eliminated by the
compiler optimisation process. Thanks to function inlining, the compiler
is able to produce a code that is equivalent to what would have been
obtained with the following instructions:

~~~~{.cxx}
for(size_type i=0;i!=d.size();++i){
  d[i] = a[i] + b[i] + c[i];
}
~~~~

> **About loop unrolling**
>
> When dealing with objects whose size is known at compile-time, the
> `TFEL/Math` library also performs an additional optimisation
> technique known as *loop unrolling*. For example, if \(\tenseur{a}\), 
> \(\tenseur{b}\) and \(\tenseur{c}\) are three \(1D\) symmetric
> tensors, the code:
>
> ~~~~{.cxx}
> c = a + 2 * b;
> ~~~~
>
> is equivalent to:
>
> ~~~~{.cxx}
> c[0] = a[0] + 2 * b[0];
> c[1] = a[1] + 2 * b[1];
> c[2] = a[2] + 2 * b[2];
> ~~~~


### The `eval` function

One possible caveat of the expression template technique can be
illustrated by the following example. Let us consider two vectors `a`
and `b`:

~~~~{.cxx}
a[0] = 1;
b[0] = 2;
auto c = a + b;
// Here, c[0] evaluates to 3, as expected.
a[0] = 2;
// However, at this stage, c[0] evaluates to 4 !
~~~~

Another caveat is that is sometimes more efficient to evaluate the
result of an operation once and use the result of this evaluation rather
than performing the evaluation of the operation several times.

To avoid those two caveats, the `eval` function allows the evaluation of
an expression, as follows:

~~~~{.cxx}
const auto c = eval(a + b);
~~~~

## Views {#sec:tfel_math:views}

Views allows to map memory area to mathematical objets. Two kind of view
are available in `TFEL/Math`:

- the `View` class which assumes a mapping from a contiguous memory,
- the `CoalescedView` class which allows to give a pointer for each
  component of the mapped mathematical object.

> **Typical usage of views in `MFront`**
>
> A typical usage of views is given by the example of the integration of
> a behaviour using an implicit scheme. In such a scheme, a non-linear
> solver tries to determine the increments \(\Delta\,Y\) of a set of
> internal state variables packed in a vector \(Y\) which are the zero of
> residual denoted \(F\). The derivative of the residual \(F\) with respect
> to \(\Delta\,Y\) is called the jacobian and is denoted \(J\).
>
> If one considers a simple plastic law with isotropic hardening, the
> vector of internal state variables typically contains the elastic
> strain, a symmetric tensor, and the equivalent plastic strain, a scalar.
>
> In the general case, the vector of internal state variables, the residual
> and the jacobian can be decomposed as follows:
> 
> \[
> Y=
> \begin{pmatrix}
>  y_{1} \\
>  \vdots \\
>  y_{i} \\
>  \vdots \\
>  y_{n} \\
> \end{pmatrix}
> \quad\quad
> F=
> \begin{pmatrix}
>  f_{y_{1}} \\
>  \vdots \\
>  f_{y_{i}} \\
>  \vdots \\
>  f_{y_{n}} \\
> \end{pmatrix}
> \quad\quad
> J = \deriv{F}{\Delta\,Y} =
> \begin{pmatrix}
>   \deriv{f_{y_{1}}}{\Delta\,y_{1}} & \ldots & \ldots & \ldots & \ldots \\
>   \vdots & \vdots & \vdots & \vdots & \vdots \\
>   \vdots & \vdots & \deriv{f_{y_{i}}}{\Delta\,y_{j}} & \vdots & \vdots \\
>   \vdots & \vdots & \vdots & \vdots & \vdots \\
>   \ldots & \ldots & \ldots & \ldots & \deriv{f_{y_{N}}}{\Delta\,y_{N}} \\
> \end{pmatrix}
> \]
> 
> `MFront` automatically define views to the objects \(y_{i}\), \(f_{y_{i}}\),
> \(\deriv{f_{y_{i}}}{\Delta\,y_{j}}\) which allows to compute the residual
> blocks and the jacobian blocks in a very intuitive ways using tensorial
> operations. Hence, the user never manipulate directly the vectors \(Y\),
> \(\Delta\,Y\) and \(F\) nor the jacobian matrix \(J\) but views which acts
> as tensorial objects. 

### The `map` function

The `map` function is a small utility function which simplifies the
creation of views from either raw pointers or from tiny vectors (i.e.
objects of type `tvector`).

For example, Listing @lst:tfel_4.0:view shows how a vector containing
the elastic strain and the equivalent plastic strain can be decomposed
by blocks. This decomposition is illustrated on Figure
@fig:tfel_4.0:view.

~~~~{#lst:tfel_4.0:view .cxx caption="Usage of the `map` function"}
auto Y = tvector<7, double>{0};
auto eel = map<stensor<3u, double>>(Y);
auto& p = Y[6];
~~~~

!["Exemple of decomposition by blocks of a memory area using views"](img/tfel-math/view.svg){#fig:tfel_4.0:view width=75%}

The `map` function allows to define offset at compile-time: this allows
to checks at compile-time that the memory area is large enough to store
the mapped object when mapping a fixed size object (i.e. an object whose
size is known at compile-time) from a memory area hold by a tiny vector.

### The `map_array` function

The `map_array` returns an object which acts like a fixed size of
mathematical objects. It takes one template argument which describes an
arry of of mathematical objects. This template argument is used to
determine the number of object mapped and the kind of object mapped.

It can be used as follows:

~~~~{.cxx}
auto a = map_array<tvector<2u, stensor<2u, double>>>(ptr);
~~~~

where `ptr` is a pointer to a suitable memory location.

### The `map_derivative` function

The `map_derivative` function allows to create a view of the derivative
of two math objects in a matrix as illustrated by the following example
which create a view of the object resulting of the derivation of a
symmetric stress tensor with respect to a scalar whose first element is
located in element \(1,1\) of the matrix:

~~~~{.cxx}
auto r = tmatrix<5, 3>{0};
map_derivative<1, 1, stensor<2u, qt<Stress>>, double>(r) = stensor<2u, Stress>::Id();
~~~~

The result of this operation is the matrix:

\[
\begin{pmatrix}
  0 & 0 & 0 \\
  0 & 1 & 0 \\
  0 & 1 & 0 \\
  0 & 1 & 0 \\
  0 & 0 & 0 \\
\end{pmatrix}
\]

### Views of tensorial objects with coalescent memory access

The `View` class allows to interpret a contiguous memory area as a
tensorial object. Such views are not efficient on GPUs where coalescent
memory access are preferable. The newly introduced `CoalescedView` class
offers a solution to this issue by storing one pointer per component.
This is essentially a "scattered" view that achieves coalescent memory
access when the backend storage is properly organized.

A coalesced view is initialized by an array of pointers to the
components of the mapped object, as illustrated in the following
snippet:

~~~~{.cxx}
int values[8] = {1, 10, 2, 20, 3, 30, 4, 40};
std::array ptrs{&values[0], &values[2], &values[4], &values[6]};
auto s1 = map<stensor<2u, int>>(ptrs);
~~~~

In this snippet, the components of the symmetric tensor `s1` are not
stored continuously but each component is associated to an independent
pointer.

### Views of tensorial objects with strided coalescent memory access (SoA layout)

While `CoalescedView` stores one pointer per component, the
`StridedCoalescedView` class provides an alternative approach using a
single pointer and a stride value. This is particularly advantageous for
Structure-of-Arrays (SoA) layouts where components are regularly spaced
in memory.

The main benefits of this approach are:

- **Register savings on GPU**: storing one pointer plus one stride value
  instead of \(N\) pointers significantly reduces register pressure,
  which is a critical resource on GPUs.
- **Negligible overhead**: recomputing a pointer address for each access
  costs only one floating-point operation (one multiplication and one
  addition), which is essentially free on modern GPU architectures.

A strided coalesced view is created using the `map_strided` function, as
illustrated in the following snippet:

~~~~{.cxx}
// SoA layout: [s0_c0, s1_c0, s0_c1, s1_c1, s0_c2, s1_c2, s0_c3, s1_c3]
int values[8] = {1, 10, 2, 20, 3, 30, 4, 40};
auto s1 = map_strided<stensor<2u, int>>(&values[0], 2);
auto s2 = map_strided<stensor<2u, int>>(&values[1], 2);
~~~~

In this example, `s1` and `s2` are two symmetric tensors stored in SoA
format with a stride of 2. The component `s1[i]` is located at address
`&values[0] + i * 2`, while `s2[i]` is at `&values[1] + i * 2`.

# Solvers for fixed size non-linear systems {#sec:tfel_math:non_linear_solvers}

The `TFEL/Math` library provides several non-linear solvers which are
mainly used by `MFront` implicit schemes. Those solvers mostly targets
systems of small size and focus on robustness and flexibility. Each
solver implements a classical algorithm by also provides several
customisation points which allows to create many variants of this basic
algorithms.

The solvers available are based on the following algorithms:

- The standard Newton-Raphson algorithm is implemented in the
  `TinyNewtonRaphsonSolver` class. This algorithm is coupled with a
  Powell' dog leg step in the `TinyPowellDogLegNewtonRaphsonSolver`
  class.
- The first and second Broyden algorithms, also known respectively as
  the good and bad Broyden algorithms are respectively implemented in
  the `TinyBroydenSolver` and `TinyBroyden2Solver` classes. The first
  Broyden algorithm is coupled with a Powell' dog leg step in the
  `TinyPowellDogLegBroydenSolver` class.
- The Levenberg-Marquardt algorithm is implemented in the
  `TinyLevenbergMarquardtSolver` class.

Those classes implements the curiously recurring template pattern (CRTP)
to avoid the use of virtual calls [@coplien_curiously_1995]. The derived
class must provide a method called `computeResidual` which must compute
the residual for the current estimate of the unknowns and, if required
by the solver, the jacobian.

A typical usage of those classes is given by the following example:

~~~~{.cxx}
struct NewtonRaphsonSolver
    : public tfel::math::
          TinyNewtonRaphsonSolver<2u, double, NewtonRaphsonSolver> {
  NewtonRaphsonSolver() {
    this->zeros = {0., 0.};
    this->epsilon = 1.e-14;
    this->iterMax = 20;
  }

  bool solve() { return this->solveNonLinearSystem(); }

  auto getCurrentEstimate() const noexcept { return this->zeros; }

  bool computeResidual() noexcept {
    constexpr double a = 1.;
    constexpr double b = 10.;
    auto& x = this->zeros;
    auto& f = this->fzeros;
    auto& J = this->jacobian;
    f = {1 - x(0), 100 * (x(1) - x(0) * x(0))};
    J = {-1., 0.,  //
         -200 * x(0), 100.};
    return true;
  }  // end of computeResidual

};  // end of struct NewtonRaphsonSolver
~~~~

which solves the non-linear system:

\[
\vec{f}\paren{x,y} = 
\begin{pmatrix}
0 \\
0 \\
\end{pmatrix}
\quad\text{with}\quad
\vec{f}\paren{x,y} = 
\begin{pmatrix}
1-x \\
10\,\paren{y-x^{2}} \\
\end{pmatrix}
\]

whose obvious root is \(\paren{1,1}\).

This previous example shows that:
 
1. The current estimate of the unknowns is stored in a data member
   called `zeros`.
2. The residual is stored in a data member called `fzeros`.
3. The jacobian is stored in a data member called `jacobian`.

Those names have been chosen in the early versions of `MFront` and are
kept for back-ward compatibility.

This section describes the generic framework used to implement those
algorithms.

## The `TinyNonLinearSolverBase` class

All the available solvers are based on the `TinyNonLinearSolverBase`
which provides two main methods `solveNonLinearSystem` and
`solveNonLinearSystem2` for the end users and many methods that can be
overloaded to customize the behaviour of the algorithm.

In practice, the `NewtonRaphsonSolver` class presented in the previous
example inherits from `TinyNewtonRaphson` `Solver<2u, double,
NewtonRaphsonSolver>` which itself inherits from
`TinyNonLinearSolverBase<2u, double, NewtonRaphsonSolver>`.

> **Note**
>
> For the sake of clarity, one template parameter of the `TinyNewtonRaphsonSolver`
> and `TinyNonLinearSolverBase` class have been omitted.
>
> This template parameter describes a data structure containing the
> so-called workspace of the solver, i.e. all the data members
> required by the solver. The default value of this template parameter
> allocates those data members on the stack.

While a bit contrived, this design ensures that all algorithms share
the same customization points, a constraint which is not easy to enforce
when relying on CRTP (compared to standard approach based on virtual
calls).

As a consequence, the `TinyNewtonRaphsonSolver` class only implements
some methods specific to the Newton-Raphson algorithm while the global
algorithmic structure in handled by the `TinyNonLinearSolverBase` class.

More precisely, the `TinyNonLinearSolverBase` provides a method called
`solveNonLinearSystem`. This method internally calls a method called
`solveNonLinearSystem2` which indeed implements the core of the
resolution algorithm. The `solveNonLinearSystem2` method is described in
depth in Section
@sec:tfel_math:TinyNonLinearSolverBase:solveNonLinearSystem2.

The point of the `solveNonLinearSystem` is to handle failures of the
resolution algorithms. The current strategy to handle those failures is
described in depth in Section
@sec:tfel_math:TinyNonLinearSolverBase:solveNonLinearSystem.

### Some data members of the `TinyNonLinearSolverBase`  class

The `TinyNonLinearSolverBase` class provides the following data members:

- `fzeros`: residual vector.
- `zeros`: current estimate of the unknowns.
- `delta_zeros`: last computed correction.
- `iter`: current iteration number.
- `is_delta_zeros_defined`: boolean stating if an increment of the
  unknowns has already been computed.
- `epsilon`: criterion value
- `iterMax`: maximum number of iterations.

It is worth mentioning that a few variables must be initialized, by the
base class before calling this method, such as:

- The initial estimate of the unknowns. The method `processNewEstimate`
  must be called must be called one this initial estimate is known.
- The values `epsilon` and `iterMax`, as well as other numerical
  parameters required by the core algorithm and declared in derived
  class.

The `iter` and `has_delta_zeros` members are automatically initialized
at the beginning of the `solveNonLinearSystem` method. This method also
calls the `processNewEstimate` method.

### The `solveNonLinearSystem2` method {#sec:tfel_math:TinyNonLinearSolverBase:solveNonLinearSystem2}

This method is called internally by the `solveNonLinearSystem` method.
It could be called directly if the required initialization are performed
beforehand.

The algorithm implemented by `solveNonLinearSystem2` method is depicted
in Figure @fig:tfel_math:TinyNonLinearSolverBase:solveNonLinearSystem2.

!["Flowchart for the resolution of non linear systems proposed by the `solveNonLinearSystem2` of the `TinyNonLinearSolverBase` class"](img/tfel-math/solveNonLinearSystem2.svg){#fig:tfel_math:TinyNonLinearSolverBase:solveNonLinearSystem2 width=100%}

The `computeNewCorrection` is the only method that must be implemented
in the derived class. It totally defines the resolution algorithm. This
method thus has no default implementation.

This flowchart also shows that the `solveNonLinearSystem` method has
many customization points which either defines the underlying algorithm
and or that can be used to enhance directly affects the performance and
the robustness of the algorithm:

- `executeInitialisationTaskBeforeBeginningOfCoreAlgorithm` which is
  started at the start of the method. The default implementation does
  nothing. This method is mostly meant to be used by the derived class
  to initialize members specific to the resolution algorithm.
- `computeResidual`: is meant to compute the residual for the current
  estimate of the unknowns. This method may also compute the jacobian if
  required.
- `computeResidualNorm` which must computes the norm of the residual. By
  default, this method computes the euclidian norm of the residual. This
  method.
- `checkConvergence` which checks if convergence is reached. By default,
  this method checks if the residual norm is lower than the criterion
  value `epsilon`. This method can also be used to implement active-sets
  algorithms for non-smooth functions.
- `processNewCorrection`: this method meant to set bounds on some
  components of the current correction or to implement a line search
  along the search direction returned by the `computeNewCorrection`
  method.
- `processNewEstimate`: this method is meant to update variables
  depending on the current estimate of the unknowns.
- By convention, the `computeNewCorrection` method may use the following
  helper methods:
  - `updateOrCheckJacobian`: This method can be used to compute the
    jacobian or part of the jacobian numerically. If the jacobian was
    computed in `computeResidual`, this method can be used to compare it
    to a numerical approximation.
  - `solveLinearSystem`: This method can be used to solve a linear
    system if required. By default, an LU method with partial
    decomposition is used (implemented by the `TinyMatrixSolve` class).

The algorithm also provides methods which are meant to display
information about the state of the resolution:

- `reportInvalidResidualEvaluation` which is called method when the
  evaluation of the residual failed or when the norm of the residual is
  not finite.
- `reportStandardIteration`: which is called once the residual is known.

Note that other methods for reporting the current status of the
algorithm, such as `reportFailure` and `reportSuccess` are also
available but are called in the `solveNonLinearMethod`.

> **Usage in `MFront`**
> 
> It is worth highlighting how the implicit DSL's provided by `MFront`
> may override those methods:
> 
> - The `computeResidual` method first computes the thermodynamic forces
>   using the code provided by the `@ComputeThermodynamicForces` code bock
>   (or `@ComputeStress` for mechanical behaviours) and then evaluates the
>   residual using the code given by the `@Integrator` code block and the
>   jacobian if it is computed analytically.
> - The `updateOrCheckJacobian` method is overloaded if:
>   - A numerical approximation of the jacobian or blocks of the jacobian
>     are required.
>   - A comparison of the analytical jacobian to a numerical approximation
>     is required.
> - The `checkConvergence` method is overloaded to take into account
>   additional convergence checks defined in the
>   `@AdditionalConvergenceChecks` code block. This method can be used to
>   implement active-sets algorithms or interior points methods for
>   multi-surface plasticity or the fixed-point method used by the
>   `StandardElastoViscoplasticity` brick for porous viscoplasticity.
> - The `processNewCorrection` is used either to set bounds on some
>   components of the current correction based on the physical bounds of
>   the variables or to limit the magnitude of the corrections of some
>   integration variables. See the `setMaximumIncrementValuePerIteration`
>   method for integration variables or the
>   `@MaximumIncrementValuePerIteration` keyword.
> - The `processNewEstimate` method is overloaded if some quantities
>   depend on the current estimation of the integration variables. A
>   typical example is given by the case of porous viscoplastic behaviours
>   for which the elastic properties depend on the porosity.

### The `solveNonLinearSystem` method {#sec:tfel_math:TinyNonLinearSolverBase:solveNonLinearSystem}

As highlighted by Figure
@fig:tfel_math:TinyNonLinearSolverBase:solveNonLinearSystem2, the
`solveNonLinearSystem2` methods may fail for many reasons. A very common
one is that the current estimate of the unknowns are unphysical, leading
to a failure in the evaluation of the residual.

The `solveNonLinearSystem` method implements a simple algorithm which
can be seen as a handcrafted line search method which greatly improve
the robustness of the non-linear solvers. When used correctly, this
method may also be used to increase the performances of the non-linear
solvers (i.e. reduce the total number of iterations).

!["Flowchart for the resolution of non linear systems proposed by the `solveNonLinearSystem` of the `TinyNonLinearSolverBase` class"](img/tfel-math/solveNonLinearSystem.svg){width=100%}

The idea of this hand-crafted linesearch is simply to take the last
correction to the unknowns and divide it by two and restart the core
algorithm. In other words, the search direction is leaved unchanged, but
the norm of the correction is reduced. This operation can be repeated
several times to find a suitable estimate of the unknowns.

Of course, this only works a correction is known, i.e. if the
`has_delta_zeros` flag is `true`. Otherwise, this means that the initial
guess of the unknowns is incorrect. In this case, we just divide this
initial guess by zero. This choice may seem arbitrary but does make
sense in most cases of `MFront` implicit schemes where the unknowns are
almost always the increment of the state variables: if we divide the
increments of the state variables, their estimates at the middle of the
time step tends to theirs values at the beginning of the time step,
which is generally physically
acceptable[^tfel:math:mfront_initial_guess].

[^tfel:math:mfront_initial_guess]: This is the reason why, by default,
the initial guess of the unknowns in `MFront` is simply a null vector.
The user may specify an initial guess for the unknowns using the
`@Predictor` code block, although this is seldom used.

> **Practical importance**
>
> While very simple, the strategy described in this section is in
> practice extremely powerful and can be used to easily build very
> robust and efficient algorithms based on physical considerations.
> Let us consider a few example:
> 
> - If one considers a plastic behaviour, one may want
>   to reject a prediction of the stress which too largely
>   exceeds the current estimate of the plastic limit.
> - If one considers a viscoplastic behaviour, one may want to
>   reject a prediction of the stress which would lead to
>   an unphysical viscoplastic strain rate. This is particularly
>   useful with viscoplastic behaviour having an exponential
>   sensitivity to the stress.
> - If one considers a plastic behaviour yield surface with sharp
>   corners, such as the approximations of the Mohr-Coulomb
>   criterion given by Abbo and Sloan [@Abbo1995;@Nagel2016], the
>   solver may fail because the flow direction may oscillate
>   between too iterations. To solve this issue, we can simply
>   reject iterations that lead to a large change in the flow
>   direction.
>
> The reader may find an example of such algorithms in the case of a
> perfect plastic behaviour based on the Hosford stress criterion
> on this page of the `MFront` gallery: 
> <https://thelfer.github.io/tfel/web/hosford.html>

An important caveat to this strategy is link to the use of an active-set
method to describe multi-surface plasticity. The active-set method makes
*a priori* assumptions on which plastic mechanisms are active and solve
  the non-linear equations with those assumptions. After convergence,
  those assumptions are checked and some mechanisms may be activated or
  deactivated and the non-linear solver is restarted. The activation of
  a plastic mechanism can lead the `computeResidual` method to fail (as
  described earlier, rejecting steps leading to a prediction well beyond
  the current plastic limit is generally a good strategy). The trouble
  here is that the last correction computed by the solver is very small
  since the algorithm had converged. Thus, the strategy implemented by
  the `solveNonLinearSystem` method would divide a correction that is
  already almost null. To avoid this caveat, `MFront` automatically
  resets the `is_delta_zeros_defined` member to `false`. More precisely,
  here is the implementation of the `checkConvergence` method when the
  user has defined additional convergence checks:

~~~~{.cxx}
bool checkConvergence(const NumericType error) {
  auto converged = error < this->epsilon;
  auto mfront_internals_converged = converged;
  this->additionalConvergenceChecks(converged, error);
  if((mfront_internals_converged) && (!converged)){
    this->is_delta_zeros_defined = false;
  }
}
~~~~

# References