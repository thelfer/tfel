---
title: Release notes of the 4.0 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer
date: 2020
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

The page describes the new functionalities of Version 4.0 of the
`TFEL` project.

This version was released on October 26, 2021 along with
`TFEL-3.0.10`, `TFEL-3.1.10`, `TFEL-3.2.7`, `TFEL-3.3.3` and
`TFEL-3.4.3` and inherits from the issues fixed in those releases.

# Known incompatibilities
 
- Material properties now define a list of aliases to scalar types that
  can conflict with variables declarations. The name of those aliases is
  thus reserved. See [this page](mfront-types.html) for details.
- The `TFELPhysicalConstants` library has been removed.
- The `TFEL_NORETURN` macro has been removed.
- In implicit DSLs, the corrections to the increment of the state
  variables are now stored in a variable called `delta_zeros`. This name
  is now reserved. For each integration variable `X`, the name
  `delta_dX` is also reserved. The `delta_dX` variable is only defined
  in the `@ProcessNewCorrection` code block (See Section
  @sec:tfel-4.0:mfront:implicit:ProcessNewCorrection).
 
## Changes to existing models

### Mohr Coulomb Abbo Sloan

- The corner smoothing of the Mohr Coulomb surface in the deviatoric
  plane is changed from the C1-continuous version (Abbo and Sloan, 1995)
  to the C2-continuous version from Abbo et al., 2011.

# Rejuvenation of the `TFEL/Math` library

## Description of some shortcomings of previous versions

### Template metaprogramming

The first versions of `TFEL` were based on `C++-98` standard which had
limited support for template metaprogramming and no support for
concept based requirements.

Template metaprogramming is the basis of many optimization techniques
used in the library, such as expression templates, loop unrolling,
compile-time conditionals, etc. 

Metaprogramming in `C++` is becoming much easier at each revision of the
standard. For example, `constexpr` functions were introduced in `C++-11`
and extended in later revisions `C++-11` and `C++-17`. 

Compile-time conditionals based in the `if constexpr` construct have
been introduced in `C++-17`. This new construct eliminates the need of
recurring to partial template specialization of auxiliary class template
classes to implement compile-time conditionals, a pattern frequently
used within the `TFEL/Math` library which is both verbose and difficult
to read.

#### Example of code size reduction related to compile-time conditionals

The `sigmaeq` function compute the von Mises norm of a symmetric second
order tensor. Depending on the space dimension, some components are null
are shall be neglected. Since the space dimension is known at
compile-time, the correct implementation can be chosen at this stage,
thus avoiding a conditional branch at runtime.

##### Implementation in Version `3.4.1`

The implementation of this function in `TFEL-3.4.1` is illustrated in Listing @lst:sigmaeq_tfel_3.4.0.

~~~~{#lst:sigmaeq_tfel_3.4.0 .cxx caption="Implementation of the sigmaeq function in TFEL-3.4.1"}
template <typename T>
typename std::enable_if<tfel::meta::Implements<T, StensorConceptBase>::cond,
                        StensorNumType<T>>::type
sigmaeq(const T& s) {
  typedef tfel::math::internals::SigmaEqImpl<StensorTraits<T>::dime> Impl;
  return Impl::exe(s);
}
~~~~

The returned type of this function is quite complex and will be
discussed in depth in Section @sec:tfel_math:concepts. In this
section, let us focus on the body of this function which selects a
specialization of the class `SigmaEqImpl` based on the space dimension
and call its static function member `exe`.

Listing @lst:sigmaeq_impl_tfel_3.4.0  shows this specialization in \(1\) dimension.

~~~~{#lst:sigmaeq_impl_tfel_3.4.0 .cxx caption="Implementation of the SigmaEqImpl class in TFEL-3.4.1"}
namespace internals {

  struct SigmaEqImplBase {
    template <typename T>
    static auto square(const T x) {
      return x * x;
    }
  };

  template <unsigned short N>
  struct SigmaEqImpl;

  template <>
  struct SigmaEqImpl<1u> : public SigmaEqImplBase {
    template <class T>
    static typename std::enable_if<
        tfel::meta::Implements<T, StensorConceptBase>::cond,
        StensorNumType<T>>::type
    exe(const T& s) {
      typedef StensorNumType<T> NumType;
      typedef tfel::typetraits::base_type<NumType> base;
      constexpr auto cste = base{3} / base{2};
      const auto tr = trace(s) / 3;
      return std::sqrt(cste * (SigmaEqImplBase::square(s(0) - tr) +
                               SigmaEqImplBase::square(s(1) - tr) +
                               SigmaEqImplBase::square(s(2) - tr)));
    }
  };

} // end of namespace internals
~~~~

The specializations in \(2D\) and \(3D\) are of course very similar. In
fine, the whole implementation is hardly readable and verbose with about
80 lines.

##### Implementation in Version `4.0`

In constrast, the use of the `if constexpr` construct of `C++-17` leads
to this aknowledgly much more compact and readable code:

~~~~{.cxx #lst:sigmaeq_tfel_4.0.1 .cxx caption="Implementation of the sigmaeq function in TFEL-4.0"}
template <typename StensorType>
std::enable_if_t<implementsStensorConcept<StensorType>(),
                 numeric_type<StensorType>>
sigmaeq(const StensorType& s) {
  using real = base_type<numeric_type<StensorType>>;
  constexpr auto N = getSpaceDimension<StensorType>();
  constexpr auto one_third = real(1) / 3;
  constexpr auto cste = real(3) / 2;
  static_assert((N == 1) || (N == 2) || (N == 3), "invalid space dimension");
  auto square = [](const auto x) { return x * x; };
  const auto tr = one_third * trace(s);
  if constexpr (N == 1u) {
    return std::sqrt(cste * (square(s(0) - tr) +  //
                             square(s(1) - tr) +  //
                             square(s(2) - tr)));
  } else if constexpr (N == 2u) {
    return std::sqrt(cste * (square(s(0) - tr) +  //
                             square(s(1) - tr) +  //
                             square(s(2) - tr) + square(s(3))));
  } else if constexpr (N == 3u) {
    return std::sqrt(cste * (square(s(0) - tr) +  //
                             square(s(1) - tr) +  //
                             square(s(2) - tr) +  //
                             square(s(3)) + square(s(4)) + square(s(5))));
  }
}  // end of sigmaeq
~~~~

### Concepts {#sec:tfel_math:concepts}

Concepts are used in the library to relate objects having the same
mathematical nature, such as symmetric second order tensors. 

Indeed, several data structures in the library matches the concept of
symmetric second order tensors:

- The `stensor` data structure defines a symmetric second order tensor
  usually allocated on the stack.
- Following the expression template technique, data structures
  representing an operation that will result to a symmetric second order
  tensor if evaluated.
- Views to externally allocated memory areas (see Section
  @sec:tfel_math_3.x:view).

Support for concepts is not part of the `C++-17` standard and has only
been introduced in the `C++-20` standard. Hence, the support of concepts
currently rely on a hand crafted machinery which has been simplified
using `C++-17` facilities and prepared for its elimination when the
library will be ported to `C++-20`.

Requirements on template arguments are currently based on the `SFINAE`
rule and the `std::enable_if` facility, which takes as first template
argument a boolean value which states if the template parameters of the
function satisfies the requirements and as second argument giving the
result of the function. To illustrate this, one may consider the
declaration of the `sigmaeq` function:

~~~~{.cxx}
template <typename StensorType>
std::enable_if_t<implementsStensorConcept<StensorType>(),
                 numeric_type<StensorType>>
sigmaeq(const StensorType&);
~~~~

In this example, the `implementsStensorConcept` function can be called
at compile-time to check if the type `StensorType` is a symmetric second
order tensor. If not, this function is not considered by the compiler.

Several functions similar to `implementsStensorConcept` has been
introduced in `TFEL-4.0` to facilitate the transition to `C++-20`
built-in support of concepts.

> **Declaration in the future `TFEL-5.0`**
>
>
> With `C++-20`, the declaration of the `sigmaeq` will probably be
> much simplier and is expected to look as follows:
>
> ~~~~{.cxx}
> auto sigmaeq(const StensorConcept auto&);
> ~~~~

### Need of a higher level of abstraction and genericity

#### Basic assignement operators, scaling operators

A mathematical object must implement some basic assignement operators
such as:

- The standard assignement which is implemented using `operator=`.
- The `operator+=` operator
- The `operator-=` operator

Those operators must be valid for any mathematical object assignable to
the current object or to an object representing an operation whose
result is assignable to this object (see the expression template
technique described in Section @sec:tfel_math:expression_templates).
For example, assigning a strain tensor to a stress tensor is not allowed
because a strain value can be assigned to a stress value.

A mathematical object must implement scaling operators allowing to
mulply (`operator*=`) and divide (`operator/=`) every components of the
object by a scalar value. This operation is only valid if the result of
the operation of multiplying the component by the scalar value is
assignable to the component. For example, scaling a strain tensor by a
stress value is not allowed (because the result of the multiplication of
a strain value by a stress value is a stress value which is not
assignable to a strain value).

Those operators were implemented once per supported mathematical object
(vector, symmetric tensors, etc.) in dedicated base classes such as:

~~~~{.cxx}
`tvector_base`, `tmatrix_base`, `stensor_base`, `tensor_base`,
`st2tost2_base`, `t2tot2_base`, `st2tot2_base`, `t2tost2_base`
~~~~

The use of base class allowed to factorize code between the class
implementing the mathematical objects and surrogate classes acting like
those mathematical objects like views (see Section
@sec:tfel_math:views).

Nevertheless, implementing such operators increased the total code size
and were extra-work which prevented introduction of new mathematical
objects.

This was required by the lack of a common way to access individual
components of a mathematical object. Providing such access is one of the
driving force for the re-design of the `TFEL/Math` library.

#### Views {#sec:tfel_math_3.x:view}

Views are an important feature of the library which allows to map an
externally allocated memory area to mathematical objets. The concept of
views is fully described in Section @sec:tfel_math:views.

In previous versions, views were implemented as lightweight classes
`which were introduced as needed. With time, a myriad of such classes
were introduced:

~~~~{.cxx}
`ConstTMatrixView`, `tmatrix_column_view`,
`tmatrix_const_column_view`, `tmatrix_const_row_view`,
`tmatrix_const_submatrix_view`, `tmatrix_row_view`,
`tmatrix_submatrix_view` `TMatrixView`, `ConstST2toST2View`,
`ST2toST2View`, `ST2toT2FromTinyMatrixView2`,
`ST2toT2FromTinyMatrixView`, `ST2toT2View`, `ConstStensorView`,
`StensorFromTinyVectorView`, `StensorView`, `ConstT2toST2View`,
`T2toST2FromTinyMatrixView2`, `T2toST2FromTinyMatrixView`,
`T2toST2View`, `T2toT2FromTinyMatrixView2`, `T2toT2FromTinyMatrixView`,
`T2toT2View`, `TensorFromTinyVectorView`, `TensorView`,
`ConstTVectorView`, `TinyVectorFromTinyVectorView`,
`TinyVectorOfStensorFromTinyVectorView`,
`TinyVectorOfTinyVectorFromTinyVectorView`,
`TVectorFromTinyVectorView` `TVectorView`, TensorViewFromStensor`,
TensorFromTinyMatrixRowView`, TensorFromTinyMatrixColumnView2`,
MatrixViewFromTensor`, TensorFromTinyMatrixRowView2`,
TensorFromTinyMatrixColumnView`, VectorFromTinyMatrixColumnView2`,
VectorFromTinyMatrixRowView2`, VectorFromTinyMatrixRowView`,
VectorFromTinyMatrixColumnView`, StensorFromTinyMatrixColumnView`,
StensorFromTinyMatrixRowView`, StensorFromTinyMatrixColumnView2`,
StensorFromTinyMatrixRowView2`
~~~~

Multiplying such utility classes became a maintenability burden and was
also a reason to limit the number of mathematical objects supported by
the library and in `MFront`.

Moreover, those classes were not designed to handle quantities
appropriately (see Section @sec:tfel_math:quantities), i.e. one
could not map math objets with units.

All those classes are now remplaced by the two classes `View` and
`ViewsArray` which heavily relies on the notion of indexing policy
described in Section @sec:tfel_math:indexing_policies and which
correctly handles quantities. Thoses classes are described in depth in
Section @sec:tfel_math:views.

### Support for compile-time operations (`constexpr`)

The `constexpr` keywords has been introduced in `C++-11`. They can
qualify functions and member functions. However, they were rarely used
in the `TFEL/Math` library in previous versions because of important
restrictions put by the `C++-11` standard and brittle compiler support,
as least when `TFEL-3.0` was released. Those restrictions were relaxed
by `C++-14` and `C++-17` standards.

Declaring most functions `constexpr` allow to test them at compile-time
which is incredibly confortable: basically, if a unit-test based on the
evaluation of `constexpr` function compiles, then the test does not even
have to be run.

### Error handling and `noexcept` correctness.

The default way of reporting errors in the `TFEL/Math` library was
exceptions thrown using one of the `tfel::raise` functions.

In hindsight, it appears that relying on exceptions was in most cases a
bad design choice[^exceptions].

[^exceptions]: It shall be clearly stated that this statement only
  concerns the `TFEL/Math` library.

Most errors can be grabbed at compile-time by the compiler or are just
avoided by design. For example, checking if the indices passed to the
access operator to a symmetric tensors are valid is mostly useless as:

- The access operator is hardly ever used by the end user and the
  library provide all the tensorial operations required to work at a
  higher level of abstraction closed to the mathematical expression.
- Most out of bounds access can be detected by the compiler at
  compile-time as most mathematical objects are based on stacked
  allocated arrays whose size is known at compile-time.

As runtime-time checks may prevent optimisations, such tests were
inhibited in release builds and only enabled in debug builds. In this
case, it seems pretty clear that aborting the execution is a much better
choice than throwing an exception. This is the purpose of the
`reportContractViolation` function.

As a consequence, most functions and methods of the `TFEL/Math` library
are marked as `noexcept` which may allow more aggressive optimisation by
the compiler.

## Overview of the new design of the TFEL/Math library

This overview of Version 4.0 of the `TFEL/Math` library mostly focuses
on two important features:

- The `TFEL/Math` library provides a linear algebra engine with
  mathematical objects and operations on those objets required to
  express the constitutive equations in an efficient and natural manner,
  i.e. as close as possible to the mathematical expressions. Section
  @sec:tfel_math:math_objects is devoted to describe this linear
  algebra engine.
- The `TFEL/Math` library provides efficient and robust non linear
  solvers for small sized problems described in Section
  @sec:tfel_math:non_linear_solvers.

### Mathematical objects {#sec:tfel_math:math_objects}

The mathematical objects provided by the `TFEL/Math` library are based
on scalar values which can be standard `C++` numeric types (`float`,
`double` or `long double`), or so-called quantities, i.e. an object
describing a value with an unit. The later allows to distinguish
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

The main concepts behing the implementation of the mathematical objects
are described in Section @sec:tfel_math:tensorial_objects.

#### Quantities {#sec:tfel_math:quantities}

Quantities were introduced in `TFEL/Math` following ideas of the book D.
Abrahams and A. Gurstovoy [@abrahams_cxx_2004] to allow dimensional
analysis at compile time. Basically, a quantity is the combinaison of a
value and an unit. The type of the value is called the base type of the
quantity.

The unit is encrypted in a type called `Unit` so that the `C++`
type-system can be used to detect at compile-time (without
runtime-checks) if operations are legal. According to the International
System of Units (SI), an unit is decomposed along \(7\) basic units
which are the mass (kg), the length (l), the time (s), the electric
current (A), the temperature (K), the luminous intensity (cd) and the
amount of substance (mole). To be able to describe fractional unit, such
as the fracture toughness which is has the unit of a stress times the
square root of a length, the `Unit` class is parametrized by \(14\)
integers. The `TFEL/Math` library provides some convenient aliases for
the most common units.

The `NoUnit` unit is a special case where all those integers are set to
0 for the numerator parts and 1 (by convention) for the denomiator
parts.

##### The `qt` class

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

##### The `qt_ref` and `const_qt_ref` classes

The library also provides two wrapper classes called `qt_ref` and
`const_qt_ref` which allows to wrap a value into an object which acts
respectively as an mutable an immutable quantity, as illustrated in the
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

While a bit contrieved, this example shows that the `qt_ref` class can
also be used in a `constexpr` context.

The `qt_ref` and `const_qt_ref` classes are parametrized by two template
parameters which describe respectively the unit and a numeric base type.

The `qt_ref` and `const_qt_ref` classes are essentially used to build views of
mathematical objects from a raw memory area, as described in Section
@sec:tfel_math:views.

##### The `Quantity` class

For the sake of simplicity, the `qt`, `qt_ref` and `const_qt_ref` were
described as classes. This is not actually the case in the current
implementation which defines them through type aliases to a more general
`Quantity` class which is parametrized by three template arguments: the
unit, the basic numeric type and a third argument describing how the
value associated with the quantity is handled.

##### Operations on quantities

Common operations on quantities, such as additions, substraction,
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

#### A generic framework to build tensorial-like objects {#sec:tfel_math:tensorial_objects}

This section first introduces the notion of array policy and indexing
policy which describes respectively how data are handled (stored) and
how those data are accesssed.

Common objects of rank \(1\) are then defined, including tiny vectors,
symmetric tensors and unsymmetric tensors. Higher order objects are
defined recursively as derivative of objects of lower ranks.

##### Description of the `ArrayPolicy` concept

Array policies describes how data are handled and accessed. For the
latter, the description of the access partern is delegated to an
indexing policy which will be described later in Section
@sec:tfel_math:indexing_policies. Thus, this section mostly focuses
on how array policies describe how the data are handled.

A class matching the `ArrayPolicy` concept must provide:

- An alias to a type called `value_type` which corresponds to the
  logical type hold by the array (see below).
- An alias to a type called `const_value_type` which corresponds to a
  constant value to a an object of the logical type hold by the array.
- An alias to a type called `storage_type` which is the type actually
  stored in memory.
- An alias to a type called `reference` which is the type returned by
  the non-constant access operators of the array. A object of the
  `reference` type must be convertible and assignable to a `value_type`
  object and provide the following operators:
  - `operator=(const other_type&)`
  - `operator+=(const other_type&)`
  - `operator-=(const other_type&)`
  - `operator*=(const other_type&)`
  - `operator/=(const other_type&)`

  where `other_type` denotes for each operator a type for which the
  considered operator in the `value_type` class is valid.
- An alias to a type calle `const_reference` which is the type returned
  by the constant access operators of the array. A object of the
  `const_reference` type must be convertible and assignable to a
  `value_type` object.
- A static function called `make_const_reference` which turns a a
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

##### Standard array policies

The `StandardArrayPolicy` class is based in the following statements:

- `storage_type` is the same as `value_type`.
- `reference` is the same as as `value_type&`.
- `const_reference` is the same as as `const value_type&`.
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

##### View array policies

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

#### Description of the `IndexingPolicy` concept {#sec:tfel_math:indexing_policies}

A class matching the `IndexingPolicy` concept must provide:

- An alias to a type called `size_type` to the type used to index data.
- An alias to a type called `RowMajorIndexingPolicy` which can be used
  to import data from raw `C` pointers.
- A static member data called `arity` of type `size_type` which
  describes the number of indices required to describe the object.
- A static boolean data member called `hasFixedSizes`. If this data
  member is `true`, the class must be stateless, i.e. empty.
- A static boolean data member called `areDataContiguous` which states
  if the data are continous
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

##### An example of indexing policy

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
at compile time), `st2tost2`, `t2tot2`, etc.. and views to those objects
in a matrix.

##### Higher order indexing policies for fixed-size mathematical objects

Let us consider an mathematical object \(o_{3}\) resulting from the
derivation of an mathematical object \(o_{1}\) of arity \(a_{1}\) with
respect to an mathematical object \(o_{2}\) of arity \(a_{2}\).
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

#### Low rank mathematical objects

The library is based on a few low rank mathematical objects:

- `tvector`: a tiny vector of fixed size. This template class is
  parametrized by the size of the vector and the value hold.
- `stensor`: a symmetric tensor. This template class is parametrized by
  the space dimension and the scalar type.
- `tensor`: a unsymmetric tensor. This template class is parametrized by
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

This notations has the property that the contracted product of two
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

#### Higher order objects defined as derivatives

The library provides a generic class called `FixedSizeArrayDerivative`
which allows to create higher order objects as being the derivative of
two objects of lowest ranks.

This class is currently very limited but will be extended in future
versions of the library.

##### The case of fourth order tensors

Fourth order tensors can be defined as derivatives of two tensors or as
linear mappings from the second order tensors to second order tensors.

As there are two kinds of second order tensors (i.e. symmetric and non
symmetric tensors), there are four kinds of fourth order tensors defined
in the `TFEL/Math` library, which satisfy the following concepts:

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

- The space dimension (`1`, `2` or `3`).
- The type used to perform the computation.

##### The `tmatrix` case

A fixed size matrix, implemented by the `tmatrix` class can be seen as
the derivative of a tiny tensor with respect to a tiny vecor.

##### The `derivative_type` metafunction

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

#### Expressions templates {#sec:tfel_math:expression_templates}

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


##### The `eval` function

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

#### Views {#sec:tfel_math:views}

Views allows to map memory area to mathematical objets.

> **Typical usage of views in `MFront`**
>
> A typical usage of views is given by the example of the integration of
> a behaviour using an implicit scheme. In such a scheme, a non linear
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

##### The `map` function

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

##### The `map_array` function

The `map_array` returns an object which acts like a fixed size of
mathematical objects. It takes one template argument which describes an
arry of of mathematical objects. This template argument is used to
determine the number of object mapped and the kind of object mapped.

It can be used as follows:

~~~~{.cxx}
auto a = map_array<tvector<2u, stensor<2u, double>>>(ptr);
~~~~

where `ptr` is a pointer to a suitable memory location.

##### The `map_derivative` function

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

### Solvers for fixed size non linear systems {#sec:tfel_math:non_linear_solvers}

The `TFEL/Math` library provides several non linear solvers which are
mainly used by `MFront` implicit schemes. Those solvers mostly targets
systems of small size and focuse on robustness and flexibility. Each
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

which solves the non linear system:

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

#### The `TinyNonLinearSolverBase` class

All the available solvers are based on the `TinyNonLinearSolverBase`
which provides two main methods `solveNonLinearSystem` and
`solveNonLinearSystem2` for the end users and many methods that can be
overloaded to customize the behaviour of the algorithm.

In pratice, the `NewtonRaphsonSolver` class presented in the previous
example inherits from `TinyNewtonRaphson` `Solver<2u, double,
NewtonRaphsonSolver>` which itself inherits from
`TinyNonLinearSolverBase<2u, double, NewtonRaphsonSolver>`.

While a bit contrieved, this design ensures that all algorithms share
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

##### Some data members of the `TinyNonLinearSolverBase`  class

The `TinyNonLinearSolverBase` class provides the following data members:

- `fzeros`: residual vector.
- `zeros`: current estimate of the unknowns.
- `delta_zeros`: last computed correction.
- `iter`: current iteration number.
- `is_delta_zeros_defined`: boolean stating if an increment of the
  unknowns has already been computed.
- `epsilon`: criterion value
- `iterMax`: maximum number of iterations.

It is worth metionning that a few variables must be initalized, by the
base class before calling this method, such as:

- The initial estimate of the unknowns. The method `processNewEstimate`
  must be called must be called one this initial estimate is known.
- The values `epsilon` and `iterMax`, as well as other numerical
  parameters required by the core algorithm and declared in derived
  class.

The `iter` and `has_delta_zeros` members are automatically initialized
at the beginning of the `solveNonLinearSystem` method. This method also
calls the `processNewEstimate` method.

##### The `solveNonLinearSystem2` method {#sec:tfel_math:TinyNonLinearSolverBase:solveNonLinearSystem2}

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
and or that can be use to enhance directly affects the performance and
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
  dependant on the current estimate of the unknowns.
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
informations about the state of the resolution:

- `reportInvalidResidualEvaluation` which is called method when the
  evaluation of the residual failed or when the norm of the residual is
  not finite.
- `reportStandardIteration`: which is called once the residual is known.

Note that other methods for reporting the current status of the
algorithm, such as `reportFailure` and `reportSuccess` are also
available but are called in the `solveNonLinearMethod`.

> **Usage in `MFront`**
> 
> It is worth hilighting how the implicit DSL's provided by `MFront`
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

##### The `solveNonLinearSystem` method {#sec:tfel_math:TinyNonLinearSolverBase:solveNonLinearSystem}

As highlighted by Figure
@fig:tfel_math:TinyNonLinearSolverBase:solveNonLinearSystem2, the
`solveNonLinearSystem2` methods may fail for many reasons. A very common
one is that the current estimate of the unknowns are unphysical, leading
to a failure in the evaluation of the residual.

The `solveNonLinearSystem` method implements a simple algorithm which
can be seen as a hand crafted line search method which greatly improve
the robustness of the non linear solvers. When used correctly, this
method may also be used to increase the performances of the non linear
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
initial guess by zero. This choice may seem arbitrary but does makes
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

> **Pratical importance**
>
> While very simple, the strategy described in this section is in
> practice extremly powerful and can be used to easily build very
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
> - If one considers a plasic behaviour yield surface with sharp
>   corners, such as the approximations of the Mohr-Coulomb
>   criterion given by Abbo and Sloan [@Abbo1995;@Nagel2016], the
>   solver may fail because the flow direction may oscillate
>   between too iterations. Too solve this issue, we can simply
>   reject iterations that lead to a large change in the flow
>   direction.
>
> The reader may find an example of such algorithms in the case of a
> perfect plastic behaviour based on the Hosford stress criterion
> on this page of the `MFront` gallery: 
> <http://tfel.sourceforge.net/hosford.html>

An important caveat to this strategy is link to the use of an active-set
method to describe multi-surface plasticity. The active-set method makes
*a priori* assumptions on which plastic mechanisms are active and solve
  the non-linear equations with those assumptions. After convergence,
  those assumptions are checked and some mechanisms may be activated or
  desactivated and the non linear solver is restarted. The activation of
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

# New MFront features

## Improvements to implicit DSLs

### New keywords

####  The `@ProcessNewCorrection` keyword {#sec:tfel-4.0:mfront:implicit:ProcessNewCorrection}

The `@ProcessNewCorrection` keyword introduces a code block called when
a new correction of the increments of the integration variables is
available.

This method can be used to:

- Limit the amplitude of the correction (see also the
  `setMaximumIncrementValuePerIteration` method).
- Implement a line-search algorithm.
- Implement the decondensation step when some integration variables were
  eliminated by static condensation.

This increment is stored in an array called `delta_zeros`. The
`delta_zeros` variable is not meant to be used directly by the users and
views to the corrections of the increments of the integration variables
are automatically declared in this code block.

Let `v` be an integration variable, then the variable `delta_dv` is a
view to the correction of the increment of this variable. If unicode
notations are used, let `υ` be the symbolic name of `v`, then `δΔv` is
an alias for `delta_dv`.

The increments of the integration variables are not updated at this
stage of the resolution algorithm.

##### Example

The following code limits the amplitude of the correction given to the
increment of the elastic strain:

~~~~{.cpp}
@ProcessNewCorrection{
 constexpr const real δΔεᵉˡ_m = 1.e-4;
 const auto e = abs(δΔεᵉˡ);
 if(e > δΔεᵉˡ_m){
   δΔεᵉˡ *= e / δΔεᵉˡ_m;
 }
}
~~~~

####  The `@ProcessNewEstimate` keyword

The `@ProcessNewEstimate` keyword introduces a code block called after
the update of the increments of the integration variables.

This method may be used to compute local variables dependent on the
updated value of the integration variables.

For example, `MFront` may define or update this code block to evaluate
material properties dependent on the value of the state variable (for
example, a Young modulus depending of the porosity), if any.

# Issues fixed

## Issue:#277: [material properties] Support for quantities

The `UseQt` keyword now allows to turn on support for quantities in
material properties for interfaces that supports it.

All interfaces delivered by `MFront` have proper support for quantities.

For more details, see : <https://sourceforge.net/p/tfel/tickets/277/>

## Issue #276: Support for quantities in `TFEL/PhysicalConstants`

The `PhysicalConstants` class now have an additional boolean template
parameter stating if quantities are to be used. For backward
compatibility, this boolean value is `false` by default.

The inline variables in the `tfel::constants` now also have a similar
template parameter.

For more details, see : <https://sourceforge.net/p/tfel/tickets/276/>

## Issue #275: [material properties] Define standard MFront scalar types

For consistency with behaviours, aliases to many scalar types are now
automatically defined in material properties, such as:

- `temperature`, `strain`, etc...

A complete list of those aliases can be found on [this page](mfront-types.html).

For more details, see : <https://sourceforge.net/p/tfel/tickets/275/>

## Issue #160: Add the ability to change the linear solver used by the Implicit DSL

Changing the linear solver can now be done by defining an user defined
algorithm. The general framework to a new linear solver is documented
here: <http://tfel.sourceforge.net/tfel-math.html>

For more details, see : <https://sourceforge.net/p/tfel/tickets/160/>

# References
