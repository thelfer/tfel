% Frequently asked questions
% Helfer Thomas
% December 18, 2017

# Getting help

There are various ways of getting help, including this FAQ.

The main source is the `TFEL` website:
<http://tfel.sourceforge.net/>. In particular, one may want to read
the pages dedicated to:

- the documentation (see also the various entries in the FAQ below):
  <http://tfel.sourceforge.net/documentation.html>
- the installation process (see also the dedicated entry in the FAQ
  below): <http://tfel.sourceforge.net/documentation.html>

If none of the resources available is satisfying, one may want to use:

- the `TFEL` forums: <https://sourceforge.net/p/tfel/discussion>
- the user mailing list:
  [tfel-discuss@lists.sourceforge.net](mailto:tfel-discuss@lists.sourceforge.net)
- the `TFEL` official mail adress to contact the developpers:
  [tfel-contact@cea.fr](tfel-contact@cea.fr).

We recommend using the forums and the mailing list for general
questions, as our answers can hopefully also be of any help to other
users.

# Documentation

There are various kind of documents available, covering a wide range
of questions. This section describes some of them, but documentation
about specific part of the `TFEL` project, such as `MFront`, is
described in the associated sections.

## TFEL/Math

### Documentation about operations on tensors

Available operations on tensors are described
[here](tensors.html). This page is not complete, so you may want to
read the `doxygen` documentation of the project.

# Installation

This section deals with installing `MFront` from sources. Consider
using binary packages of your distribution, when available, if those
versions were compiled with the appropriate interfaces for your
specific needs.

## Documentation

The installation process is fully described in the following pages:

- [install.html](install.hml): this document describes the generic
  installation procedure for `TFEL` from the sources on
  posix-compliant systems. Please note that the main systems on which
  `TFEL` was developed is `Linux`. Extensive testing on other
  posix-compliant operating systems, notably `FreeBSD`, is lacking,
  although compilation and unit testing is known to work.
- A page dedicated to `FreeBSD` is available
  [here](install-freebsd.html).
- The installation on
  [Windows plateform](http://windows.microsoft.com) is described in the
  following pages:
    - [Installation based on `cmake` and `Visual Studio`](install-windows-VisualStudio.html).
    - [Installation based on `cmake` and `MinGW` (as packaged with `Cast3M` `2017`)](install-windows-Cast3M2017.html).
	  This tutorial can easily be adapted to other versions of
	  `MinGW`, without requiring `Cast3M` to be installed.
    - [Installation based on `MSYS`](install-windows-msys.html).
- The creation of binary packages are detailled [here](packages.html). 

## Which build systems shall I use (`autotools` or `cmake`)

We strongly recommend using the `cmake` build system, which is
actively in the development process used for various reasons and which
enables many tests that are not present when using `autotools`.

The `autotools` build system is barely maintained in a functional
state under `LinuX`. This can be used when `cmake` is not available.

## Portable build

By default, the compilation of `mfront` takes advantage of the
specific CPU of the host system (using flags such as `--march=native`
with `clang` and `gcc`, `-xHost` with the `intel` compiler). However,
the binaries generated can not:

- be used to generated redistributable binary packages.
- be installed on a NFS-like shared folder to be shared on a network
  of computers.

In both cases, the execution can fail with an `illegal instruction`
message.

To solve this issue, the `-Denable-portable-build=ON` option (this
option is valid for `cmake`, when using the `autotools`, consider the
`--enable-portable-build` option) has been introduced.

With this option, the `mfront` binary and all the `TFEL` shared
libraries will be "portable" (will not include CPU specific
instructions).

However, most of the times, the shared libraries produced by `MFront`
will be executed on the machine on which they will be used. For this
reasons, the default behaviour of `mfront` is to use flags like
`-march=native` when compiling the libraries (This can be disabled by
selecting `--obuild=level0`).

Thus, we test the availability of this flag whether or not
`-Denable-portable-build=ON` is used.  Thus, a message such as
`--enabling flag 'march=native'` barely states that this option is
supported by the compiler.

The best way to know if this option was taken into account is check
the flags used to compile TFEL, as follows:

~~~~{.sh}
# when using cmake
$ make VERBOSE=1
~~~~

~~~~{.sh}
# when using autotools
$ make V=1
~~~~

Without `-Denable-portable-build=ON`, you shall see the
`--march=native` flag twice: one time as a compiler flag, one time as
part of the definition of the `OPTIMISATION_FLAGS` macro. With
`-Denable-portable-build=ON`, you will see it only once, in the
definition of the `OPTIMISATION`_FLAGS macro.

# MFront

## General questions

### What is a DSL (domain specific language)

`MFront` treats various kind of material knowledge:

- [material properties](material-properties.html) (for instance the
  Young modulus, the thermal conductivity, etc.)
- [mechanical behaviours](behaviours.html).
- [simple point-wise models](models.html), such as material swelling
  used in fuel performance codes.

For mechanical behaviours, various algorithms are available.

In all cases, `MFront` strives to provide the most natural way of
implementing the material knowledge under consideration. In technical
terms, `MFront` provides for each case a *domain specific language*
which is meant to be simple and expressive.

### Supporting new interfaces

`MFront` already supports many interfaces to:

- free or commercial finite element solvers (implicit or explicit)
- mechanical solvers based on Fast Fourier Transform (FFT).

Most solvers offers entry points to add user defined mechanical
behaviours. The most common one is `UMAT`, which is part of the
`Abaqus/Standard` solver. In this case, the process of supporting new
a solver is fairly easy and we are ready to help setting it
up. However, extensive testing can be a long and tedious task: again,
we are ready to help by providing advice, test cases and reference
solutions.

If no such entry point exist, then one may need to modify the
solver. Again, we can provide valuable advice of how to do add support
for user defined behaviours and even provide tight integration with
`MFront` (which can really ease user's life).

## Documentation

### Keywords available

To get the list of all the keywords associated with a given `DSL`, the
`Implicit` keyword for example, just type:

~~~~{.sh}
$ mfront --help-keywords-list=Implicit
~~~~

### Help on a specific keyword

To get help on a specific keyword, the `@StrainMeasure` keyword from
the `Implicit DSL` for example, just type:

~~~~{.sh}
$ mfront --help-keyword=Implicit:@StrainMeasure
~~~~

The help is written using `pandoc`' `markdown`.

### Getting the help on all keywords of a specific DSL

The following command will display the description of all the keywords
provided by the `Implicit DSL`:

~~~~{.sh}
$ mfront --help-keywords=Implicit
~~~~

Using `pandoc`, this can be turned into an web page or a `PDF`
document, as follows:

~~~~{.sh}
$ mfront --help-keywords=Implicit | pandoc -f markdown-markdown_in_html_blocks+tex_math_single_backslash --mathjax -o Implicit.html
$ mfront --help-keywords=Implicit | pandoc -f markdown-markdown_in_html_blocks+tex_math_single_backslash --mathjax -s --toc --toc-depth=1 -o Implicit.pdf
~~~~

## General questions about mechanical behaviours

### Where can I find examples of well written behaviours ?

The gallery has been created for that purpose. Various implementations
of mechanical behaviour are covered in depth, including the
description of the algorithm used. See [this page](gallery.html) for
details.

### My newly implemented behaviour do not converge, what can I do ?

Let us point that, there is no general guidelines, most troubles are
behaviour specific. However, here are some advises to may help
you. Note that those advises are worth considered during the
behaviour implementation, before "real-world tests".

The first thing to do is to identify the trouble.

If your computations are very CPU intensive and if the divergence
appends after a noticeable amount of time, it is worth enabling the
generation of a `MTest` file on failure. This feature is for example
supported by the `castem` (`Cast3M`), `aster` and `cyrano` interfaces.

I thus assume that your are using `MTest`.

You can use `--debug` command line option when compiling the `MFront`
file. This will print some information about convergence at
runtime. For example, it may show:

- large values of the residual.
- a divergence (residual growing) or spurious oscillations in the
  residual.
- `NaN` propagation.

#### Large values of the residual

In this case, you may want to print some of your variables to see what
is happening. If the large values appears due to unrealistic
prediction of the stresses, in particular at the second iteration, the
`Implicit` scheme allows you to limit the `Newton` steps or use more
robust algorithms (`PowellDogLeg`, `LevenbergMarquardt`). Otherwise,
you must check your units.

#### Divergence spurious oscillations in the residual

In the second case, the trouble may be related to your implementation
of the Jacobian matrix (assuming you are using an `Implicit` scheme
with analytical jacobian). In this case, it is worth comparing your
jacobian to a numericall one (see `@CompareToNumericalJaobian`). As
this comparison is CPU intensive, please consider specifying this
keyword in the command line rather than in your implementation to
avoid forgetting removing it in your real-world tests:

~~~~ {.bash}
$ mfront --obuild --interface=castem --@CompareToNumericalJacobian=true norton.mfront
~~~~~~~~~~~~~~~~~~~

Spurious oscillations may also be caused by an ill-conditioned system,
see the `setNormalisationFactor` method.

#### `NaN` propagation.

In this case, you may want to build your `MFront` libraries with
debugging symbols. This can be done by defining the `CXXFLAGS`
environment variable before the behaviour compilation. For example:

~~~~ {.bash}
$ export CXXFLAGS="-g"
$ mfront --obuild --interface=castem norton.mfront
~~~~~~~~~~~~~~~~~~~

or even better

~~~~ {.bash}
$ CXXFLAGS="-g" mfront --obuild --interface=castem norton.mfront
~~~~~~~~~~~~~~~~~~~

In `c-shell`, you must use the following lines:

~~~~ {.bash}
$ setenv CXXFLAGS "-g"
$ mfront --obuild --interface=castem norton.mfront
~~~~~~~~~~~~~~~~~~~

You can then launch `MTest` in the `gdb` debugger like this:

~~~~ {.bash}
$ gdb --args mtest -fpe norton.mtest
~~~~~~~~~~~~~~~~~~~

You must type `r` in `gdb` to start the computations. The `-fpe`
command line option will cause the program to fail a the invalid
operation and `gdb` will show you which line causes the
trouble. Beware that this line may be in the generated code. In this
case, this information will not be useful and you shall return to
manual search of the problem.

### What are the variable types available in `MFront`

For all domain specific languages, `MFront` defines the real
`typedef` which is used to abstract to floating-point type used
by the calling solver. For example, if the calling solver works
in double precision, real will be a `typedef` to `double`. If the
calling solver works in quadruple precision, real will be a
`typedef` to `long double`.

Thus, we do recommend not to use the numerical types defined by the
`C++` language directly.

We now get more specific and **only** deal with *mechanical
behaviours*.

For scalar values, `MFront` introduces many different `typedef`
to be able to express the nature of the variable:

`real`, `frequency`, `stress`, `length`, `time`, `strain`,
`strainrate`, `temperature`, `energy_density`,
`thermalexpansion`, `massdensity`

For vector values, `MFront` introduces these `typedef`:

`TVector`,`DisplacementTVector`, `ForceTVector`

For symmetric tensor values, `MFront` also introduces many different
`typedef`:

`Stensor`, `StressStensor`, `StressRateStensor`, `StrainStensor`,
`StrainRateStensor`

Finally, for tensor values, `MFront` introduces these `typedef`:

`Tensor`, `DeformationGradientTensor`

For the moment, distinction between those various types is only
informative. We hope to introduce more severe tests in future versions
of `MFront` so that we won't be able to add a `StressStensor` and a
`StrainStensor`. The `TFEL` library already provides the mandatory
types to do that.

You can also directly use to types provided by the `TFEL`
library. The most interesting ones for the end user are:

- `tvector<N,Type>` (fixed sized vector)
- `stensor<N,Type>` (symmetric tensor)
- `tensor<N,Type>`  (non symmetric tensor)
- `tmatrix<N,M,Type>` (fixed sized matrix)
- `st2tost2<N,Type>` (linear application changing a symmetric
  tensor to a symmetric tensor)
- `st2tot2<N,Type>` (linear application changing a symmetric
  tensor to a non symmetric tensor)
- `t2tost2<N,Type>` (linear application changing a non
  symmetric tensor to a symmetric tensor)
- `t2tot2<N,Type>` (linear application changing a non
  symmetric tensor to a non symmetric tensor)

where `N` is the size for vectors, the number of rows for matrices and
the spatial dimension for the other types. `M` is the number of
columns for the matrices. `Type` is the underlying numeric type.

### What are the differences between the `Stensor`, `StressStensor` and `StrainStensor`

The difference between those types is currently purely informative:
the user can use these types to improve the readability of their code
which is strongly encouraged.

The `TFEL` library has support for quantities (number associated with
units) which allows to checks for the consistency of operations at
compile-time (no cost at runtime). However, support for this feature
has not been enabled in `MFront` yet: for the moment, we only have
introduced the associated types.

### Orthotropic axes convention

Most finite element solver can't have a uniq definition of the
orthotropic axes for all the modelling hypotheses.

For example, one can define a pipe using the following axes
definition:

- \(\left(rr,zz,tt,...\right)\) in \(3D\), \(2D\) axysymmetric, \(1D\)
  axisymmetric generalised plane strain or generalised plane stress.
- \(\left(rr,tt,zz,...\right)\) in \(2D\) plane stress, strain,
  generalized plane strain.

![The `Pipe` orthotropic axes convention for \(3D\), \(2D\)
  axysymmetric, \(1D\) axisymmetric generalised plane strain or
  generalised plane stress (left) and \(2D\) plane stress, strain,
  generalized plane strain (right)](img/pipe.svg "The `Pipe`
  orthotropic axes convention for \(3D\), \(2D\) axysymmetric, \(1D\)
  axisymmetric generalised plane strain or generalised plane stress
  (left) and \(2D\) plane stress, strain, generalized plane strain
  (right)"){width=50%}

With those conventions, the axial direction is either the second or
the third material axis, a fact that must be taken into account when
defining the stiffness tensor, the Hill tensor(s), the thermal
expansion, etc.

This convention is valid for all modelling hypotheses.

![The `Plate` orthotropic axes convention](img/plate.svg "The `Plate`
  orthotropic axes convention"){width=50%}

If we were to model plates, an appropriate convention is the following:

- The first material axis is the rolling direction
- The second material axis is the in plane direction perpendicular to
  the rolling direction (transverse direction).
- The third material axis is the normal to the plate.

By definition, this convention is only valid for \(3D\), \(2D\) plane
stress, strain and generalized plane strain modelling hypotheses.

## Implementing mechanical behaviours using the `Implicit` DSL

### In which order are the blocks given by the user evaluated ?

The following figure shows how the various blocks defined by the user
may be used when using the `Implicit` domain specific language:

![Use order of the code blocks defined by the user](img/ImplicitDSL.svg
 "Use order of the code blocks defined by the user")

<!-- Local IspellDict: english -->
