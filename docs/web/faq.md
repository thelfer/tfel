% Frequently asked questions
% Helfer Thomas
% October 15, 2014

# My newly implemented behaviour is not converging, what can I do ?

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

## Large values of the residual

In this case, you may want to print some of your variables to see what
is happening. If the large values appears due to unrealistic
prediction of the stresses, in particular at the second iteration, the
`Implicit` scheme allows you to limit the `Newton` steps or use more
robust algorithms (`PowellDogLeg`, `LevenbergMarquardt`). Otherwise,
you must check your units.

## Divergence spurious oscillations in the residual

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

## `NaN` propagation.

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

# What are the variable types available in `MFront`

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
to be able to express the nature of the variable :

`real`, `frequency`, `stress`, `length`, `time`, `strain`,
`strainrate`, `temperature`, `energy_density`,
`thermalexpansion`, `massdensity`

For vector values, `MFront` introduces these `typedef` :

`TVector`,`DisplacementTVector`, `ForceTVector`

For symmetric tensor values, `MFront` also introduces many different
`typedef` :

`Stensor`, `StressStensor`, `StressRateStensor`, `StrainStensor`,
`StrainRateStensor`

Finally, for tensor values, `MFront` introduces these `typedef` :

`Tensor`, `DeformationGradientTensor`

For the moment, distinction between those various types is only
informative. We hope to introduce more severe tests in future versions
of `MFront` so that we won't be able to add a `StressStensor` and a
`StrainStensor`. The `TFEL` library already provides the mandatory
types to do that.

You can also directly use to types provided by the `TFEL`
library. The most interesting ones for the end user are :

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

# What are the differences between the `Stensor`, `StressStensor` and `StrainStensor`

The difference between those types is currently purely informative :
the user can use these types to improve the readability of their code
which is strongly encouraged.

The `TFEL` library has support for quantities (number associated with
units) which allows to checks for the consistency of operations at
compile-time (no cost at runtime). However, support for this feature
has not been enabled in `MFront` yet : for the moment, we only have
introduced the associated types.

# In which order are the blocks given by the user evaluated ?

The following figure shows how the various blocks defined by the user
may be used when using the `Implicit` domain specific language:

![Use order of the code blocks defined by the user](img/ImplicitDSL.svg
 "Use order of the code blocks defined by the user")

<!-- Local IspellDict: english -->
