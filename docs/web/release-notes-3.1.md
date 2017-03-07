% New functionalities of the 3.1 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2017

\newcommand{\Frac}[2]{\displaystyle\frac{\displaystyle #1}{\displaystyle #2}}
\newcommand{\paren}[1]{\left(#1\right)}
\newcommand{\deriv}[2]{\Frac{\partial #1}{\partial #2}}
\newcommand{\tenseur}[1]{\underline{#1}}

The page declares the new functionalities of the 3.1 version of
`TFEL`, `MFront` and `MTest`.

# TFEL

## TFEL/Math

### Symmetric tensor eigen values and eigen vectors

The computation of the eigen values and eigen vectors of a symmetric
tensor has been improved in various ways:

- Various overloaded versions of the `computeEigenValues`,
  `computeEigenVectors` and `computeEigenTensors` methods have been
  introduced for more readable usage and compatibility with
  *structured bindings* construct introduced in C++17: the results of
  the computations are returned by value. There is also a new optional
  parameter allowing to sort the eigen values.
- New eigen solvers have been introduced.

#### New overloaded versions of the `computeEigenValues`, `computeEigenVectors` and `computeEigenTensors` methods 

##### Return by value

Various overloaded versions of the `computeEigenValues`,
`computeEigenVectors` and `computeEigenTensors` methods have been
introduced for more readable usage and compatibility with *structured
bindings* construct introduced in C++17: the results of the
computations are returned by value.

For example:

~~~~~{.cpp}
tmatrix<3u,3u,real> m2;
tvector<3u,real>    vp2;
std::tie(vp,m)=s.computeEigenVectors();
~~~~~

Thanks to C++17 *structured bindings* construct, the previous code
will be equivalent to this much shorter and more readable code:

~~~~~{.cpp}
auto [vp,m] = s.computeEigenVectors();
~~~~~

Even better, we could write:

~~~~~{.cpp}
const auto [vp,m] = s.computeEigenVectors();
~~~~~

##### Eigen values sorting

The `computeEigenValues` and `computeEigenVectors` methods now have an
optional argument which specify if we want the eigen values to be
sorted. Three options are available:

- `ASCENDING`: the eigen values are sorted from the lowest to the greatest.
- `DESCENDING`: the eigen values are sorted from the greatest to the lowest.
- `UNSORTED`: the eigen values are not sorted.

Here is how to use it:

~~~~~{.cpp}
tmatrix<3u,3u,real> m2;
tvector<3u,real>    vp2;
std::tie(vp,m)=s.computeEigenVectors(Stensor::ASCENDING);
~~~~~

#### New eigen solvers

The default eigen solver for symmetric tensors used in `TFEL` is based
on analitical computations of the eigen values and eigen vectors. Such
computations are more efficient but less accurate than the iterative
Jacobi algorithm (see [@kopp_efficient_2008;@kopp_numerical_2017]).

With the courtesy of Joachim Kopp, we have introduced the following
algorithms:

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
std::tie(vp,m)=s.computeEigenVectors<Stensor::GTESYMMETRICQREIGENSOLVER>();
~~~~~

|  Algorithm                  |  Failure ratio  | \(\Delta_{\infty}\) |   Times (ns)  |  Time ratio |
|:---------------------------:|:---------------:|:-------------------:|:-------------:|:-----------:|
| `TFELEIGENSOLVER`           |   0.000642      | 3.29e-05            |   250174564	  |	1	  	    |
| `GTESYMMETRICQREIGENSOLVER` |   0             | 1.10e-06            |   359854550   |	1.44	    |
| `FSESJACOBIEIGENSOLVER`     |   0             | 5.62e-07            |   473263841   |	1.89	    |
| `FSESQLEIGENSOLVER`         |   0.000397      | 1.69e-06            |   259080052   |	1.04	    |
| `FSESCUPPENEIGENSOLVER`     |   0.019469      | 3.49e-06            |   274547371   | 1.10	    |
| `FSESHYBRIDEIGENSOLVER`     |   0.076451      | 5.56e-03            |   126689850   |	0.51	    |
| `FSESANALYTICALEIGENSOLVER` |   0.108877      | 1.58e-01            |   127236908   |	0.51  	    |
: Test on \(10^{6}\) random symmetric tensors in single precision (`float`). {#tbl:comp_eigensolvers_float}

|  Algorithm                  |  Failure ratio  | \(\Delta_{\infty}\) |   Times (ns)  |  Time ratio |
|:---------------------------:|:---------------:|:-------------------:|:-------------:|:-----------:|
| `TFELEIGENSOLVER`           |   0.000632      | 7.75e-14            |   252663338   |	1		    |
| `GTESYMMETRICQREIGENSOLVER` |   0             | 2.06e-15            |   525845499   |	2.08	    |
| `FSESJACOBIEIGENSOLVER`     |   0             | 1.05e-15            |   489507133   |	1.94	    |
| `FSESQLEIGENSOLVER`         |   0.000422      | 3.30e-15            |   367599140   |	1.45	    |
| `FSESCUPPENEIGENSOLVER`     |   0.020174      | 5.79e-15            |   374190684   |	1.48	    |
| `FSESHYBRIDEIGENSOLVER`     |   0.090065      | 3.53e-10            |   154911762   |	0.61	    |
| `FSESANALYTICALEIGENSOLVER` |   0.110399      | 1.09e-09            |   157613994   |	0.62	    |
: Test on \(10^{6}\) random symmetric tensors in double precision (`double`). {#tbl:comp_eigensolvers_double}

|  Algorithm                  |  Failure ratio  | \(\Delta_{\infty}\) |   Times (ns)  |  Time ratio |
|:---------------------------:|:---------------:|:-------------------:|:-------------:|:-----------:|
| `TFELEIGENSOLVER`           |   0.000575      |  2.06e-17           |   428333721   |	1		    |
| `GTESYMMETRICQREIGENSOLVER` |   0             |  1.00e-18           |   814990447   |	1.90	    |
| `FSESJACOBIEIGENSOLVER`     |   0             |  5.11e-19           |   748476926   |	1.75	    |
| `FSESQLEIGENSOLVER`         |   0.00045       |  1.83e-18           |   548604588   |	1.28	    |
| `FSESCUPPENEIGENSOLVER`     |   0.009134      |  3.23e-18           |   734707748   |	1.71	    |
| `FSESHYBRIDEIGENSOLVER`     |   0.99959       |  4.01e-10           |   272701749   |	0.64	    |
| `FSESANALYTICALEIGENSOLVER` |   0.999669      |  1.36e-11           |   315243286   |	0.74	    |
: Test on \(10^{6}\) random symmetric tensors in extended precision (`long double`). {#tbl:comp_eigensolvers_long_double}

#### Some benchmarks

We have compared the available algorithm on \(10^{6}\) random
symmetric tensors whose components are in \([-1:1]\).

For a given symmetric tensor, we consider that the computation of the
eigenvalues and eigenvectors failed if:
\[
\Delta_{\infty}=\max_{i\in[1,2,3]}\left\|\tenseur{s}\,\cdot\,\vec{v}_{i}-\lambda_{i}\,\vec{v}_{i}\right\|>10\,\varepsilon
\]
where \(\varepsilon\) is the accuracy of the floatting point considered.

The results of those tests are reported on Tables
@tbl:comp_eigensolvers_float, @tbl:comp_eigensolvers_double and
@tbl:comp_eigensolvers_long_double:

- The standard eigen solver available in previous versions of `TFEL`
  offers a very interesting compromise between accuracy and numerical
  efficiency.
- If very accurate results are required, the `FSESJACOBIEIGENSOLVER`
  eigen solver is a good choice.

### Isotropic functions and Isotropic function derivatives of symmetric tensors

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

Various new overloaded versions of those methods have been introduced
in `TFEL-3.1`. This overloaded methods are meant to:

- allow the user to explicitly give the values of \(f\) or \(df\),
  rather than the functions to compute them. This allows to reduce the
  computational cost of the evaluation of the isotropic function when
  the values of the derivatives can directly be computed from the
  values of \(f\). See the example \(\exp\) example below.
- return the results by value. This allow a much more readable code if
  the *structure bindings* feature of the C++17 standard is available.

To illustrate this new features, assuming that the the *structure
bindings* feature of the C++17 standard is available, one can now
efficiently evaluate the exponential of a symmetric tensor and its
derivative as follows:

~~~~{.cpp}
const auto [vp,m] = s.computeEigenVectors();
const auto evp    = map([](const auto x){return exp(x)},vp);
const auto [f,df] = Stensor::computeIsotropicFunctionAndDerivative(evp,evp,vp,m,1.e-12);
~~~~

# New functionalities of the `MFront` code generator

## Gallery

### Hyperelastic behaviours

The implementation of various hyperelastic behaviours can be found
[here](hyperelasticity.html):

- [Signorini](signorini.html)
- [Ogden](ogden.html)

# New functionalities of `MTest` solver

### Abritrary non linear constraints

Abritrary non linear constraints on driving variables and
thermodynamic forces can now be added using the `@NonLinearConstraint`
keyword.

> **Note**
>
> This keyword can also be used to define linear constraints, although
> the numerical treatment of such a constraint will be sub-optimal. A
> special treatment of such a constraint is planned.

#### Normalisation policy

This keyword must be followed by an option giving the normalisation
policy. The normalisation policy can have one of the following values:

- `DrivingVariable`, `Strain`, `DeformationGradient`,
  `OpeningDisplacement` stating that the constraint is of the order of
  magnitude of the driving variable.
- `ThermodynamicForce`, `Stress`, `CohesiveForce` stating that the
  constraint is of the order of magnitude of the thermodynamic force.

#### Example

~~~~~ {.cpp}
// ensure that the loading is isochoric in 1D
@NonLinearEquation<Strain> 'FRR*FTT*FZZ-1';
~~~~~~~~~~

#### `python` bindings

In the `python` bindings, the `setNonLinearConstraint` method has been
added to the `MTest` class.

This method takes two named arguments:

- `constraint`, the equation to be satified
- `normalisation_policy`. The normalisation policy can have one of the
  following values:
    - `DrivingVariable`, `Strain`, `DeformationGradient`,
	  `OpeningDisplacement` stating that the constraint is of the
	  order of magnitude of the driving variable
	- `ThermodynamicForce`, `Stress`, `CohesiveForce` stating that the
	  constraint is of the order of magnitude of the thermodynamic
	  force

# References

<!-- Local IspellDict: english -->
