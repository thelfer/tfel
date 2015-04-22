% Frequently asked questions
% Helfer Thomas
% October 15, 2014

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
