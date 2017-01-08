% Implementation of the Signorini hyperelastic behaviour
% T. Helfer
% 01/01/2017

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

\newcommand{\C}{\tenseur{C}}
\newcommand{\S}{\tenseur{S}}
\newcommand{\iJb}{\bar{c}_{I_{3}}}
\newcommand{\diJb}{\deriv{\iJb}{I_{3}}}
\newcommand{\sdiJb}{\sderiv{\iJb}{I_{3}}}

This page describe how to implement the Signorini hyperelastic
behaviour (see @edf_loi_2013).

The whole implementation is available here:
[Signorini.mfront](./gallery/hyperelasticity/Signorini.mfront)

> **Compatibility with previous versions of `TFEL`**
>
> This implementation presented here is based on the version \(3.0\)
> of `TFEL`. However, only a few adaptations are required for used
> with previous versions:
>
> - the first line of the file must be replace by: `@Parser DefaultFiniteStrainParser;`
> - the `power` function is not available. It can be replaced by
>   explicit multiplications.
> - the `computeDeterminantSecondDerivative` is not available. It can
>   be replaced by the following code:
>   `Stensor4::dsquare(C)-(C^id)-I1*Stensor4::Id()+(id^dI2_dC)`
> - the use of the `auto` keyword is only permitted by the `C++11`
>   standard, which is default for `TFEL-3.0`. The user is left with
>   two choices:
>     - replace `auto` with explicit types: `real` for scalars,
>       `Stensor` for symmetric tensors, `Stensor4` for linear forms
>       on symmetric tensors ("fourth order tensors"). However, in
>       this case, the implementation can be less efficient as it
>       leads to an explicit evaluation of intermediate results.
>     - specify the appropriate flag to the `C++` compiler. Assuming
>       that the current shell is `bash` and that the compiler is
>       `g++`, this can be done by invoking mfront as follows:
>       `CXXFLAGS="`tfel-config --oflags` -std=c++11" mfront --obuild --interface=aster Signorini.mfront`
>
> An implementation where those changes were made is available
> [here](./gallery/hyperelasticity/Signorini-tfel2.mfront)


# Description

The Signorini behaviour is described by the following decoupled
potential:
\[
W\paren{\C}=W^{v}\paren{J}+W^{i}\paren{\bar{I}_{1},\bar{I}_{2}}
\]
where

- \(\C\) is the right Cauchy tensor. The three invariants of \(\C\)
  are defined by:
  \[
  \left\{
  \begin{aligned}
  I_{1} &= \trace{C} \\
  I_{2} &= \Frac{1}{2}\left(\paren{\trace{\C}}^{2}-\trace{\C^{2}}\right) \\
  I_{3} &= \det\paren{\C}
  \end{aligned}
  \right.
  \]
- \(J\) is the determinant of deformation gradient (\(J=\sqrt{I_{3}}\))
- \(\bar{I}_{1}\) and \(\bar{I}_{2}\) are the first and second
  invariants of the modified right Cauchy tensor \(\tenseur{\bar{C}}\)
  defined by: \(\tenseur{\bar{C}}=J^{-2/3}\,\C = I_{3}^{-1/3}\,\C\)

**A general derivation the stress and consistent tangent operator for
this kind of hyperelastic behaviours is described
[here](hyperelasticity.html).** The reader shall refer to this page
for intermediate computations that won't be detailled here.

For our implementation, we choose \(W^{v}\) as follows: 
\[
W^{v}\paren{J}=\Frac{1}{2}\,K\,(J-1)^{2}
\]
where \(K\) is the bulk modulus.

The Signorini behaviour mostly refers to the following choice of
\(W^{i}\):
\[
W^{i}= C_{01}\,\paren{\bar{I}_{1}-3}+2\,C_{02}\,\paren{\bar{I}_{1}-3}^{2}+C_{10}\,\paren{\bar{I}_{2}-3}
\]

The general expression of the second Piola-Kirchhoff stress is:

\[
\S=2\,\deriv{W^{v}}{\C}+2\deriv{W^{i}}{\C}=\S^{v}+\S^{i}
\]

where \(\S^{v}=2\,\deriv{W^{v}}{\C}\) is the
volumetric part of the the second Piola-Kirchhoff stress and
\(\S^{i}=2\,\deriv{W^{i}}{\C}\) is the isochoric
part.

# Implementation 

The implementation of this behaviour is decomposed in two parts:

- The computation of the Cauchy stress.
- The computation of the consistent tangent operator.

We use the `DefaultFiniteStrain` implementation as this behaviour
does not require an integration algorithm.

~~~~~{.cpp}
@DSL DefaultFiniteStrain;
~~~~~

We use parameters for defining the material constants:

~~~~~{.cpp}
@Parameter K   = 2.939e9;
@Parameter C01 = 2.668e6;
@Parameter C02 = 0.446e6;
@Parameter C10 = 0.271e6;
~~~~~

Finally, we use a local variable to store the consistent tangent
operator:

~~~~~{.cpp}
@LocalVariable StiffnessTensor dS_dC;
~~~~~

## Computation of the Cauchy stress

### Computation of the right Cauchy tensor \(\C\), the invariants and the invariants derivatives

The identity tensor is stored in the `id` variable for a shorter and
cleaner code:

~~~~~{.cpp}
const auto id = Stensor::Id();
~~~~~

Then, we compute the determinant of the deformation gradient at the
end of the time step, called `F1`:

~~~~~{.cpp}
const auto J  = det(F1);
~~~~~

Then we compute the right Cauchy tensor \(\C\):

~~~~~{.cpp}
const auto C  = computeRightCauchyGreenTensor(F1);
~~~~~

For the computation of the invariants of \(\C\) and theirs
derivatives, we need to compute \(\C^{2}\). Since the multiplication
of two symmetric tensors leads to a non symmetric tensors, we use the
special function `square` which is a more efficient equivalent of
`syme(C*C)`:

~~~~~{.cpp}
const auto C2 = square(C);
~~~~~

The first invariant \(I_{1}\) is the trace of \(\C\):

~~~~~{.cpp}
const auto I1 = trace(C);
~~~~~

The second invariant \(I_{2}\) is computed as follows:

~~~~~{.cpp}
const auto I2 = (I1*I1-trace(C2))/2;
~~~~~

The derivative of \(\deriv{I_{2}}{\C}\) is equal to
\(\trace{\C}\,\tenseur{I}-\C\), which is readily written as:

~~~~~{.cpp}
const auto dI2_dC = I1*id-C;
~~~~~

> At this stage, \(\deriv{I_{2}}{\C}\) is not evaluated: the
> associated variable `dI2_dC` is an evaluation tree that stores the
> computation that will be performed latter. This is an essential
> optimization techniques used in `MFront` to achieve optimal
> performances. If we wanted or needed to explicitly evaluate, we
> could either use the `eval` function or explicitly precise the type
> of `dI2_dC`.

The derivative \(\deriv{I_{3}}{\C}\) of the third invariant is given
by:
\[
\deriv{I_{3}}{\C} = \C^{2}-I_{1}\,\C+I_{2}\,\tenseur{I}
\]

The computation of the third invariant and its derivatives is
straightforward:

~~~~~{.cpp}
const auto I3      = J*J;
const auto dI3_dC  = C2-I1*C+I2*id;
~~~~~
  
### Computation of the volumetric part of the second Piola Kirchhoff stress

The volumetric part of the second Piola Kirchhoff stress \(\S^{v}\) is
given by:
\[
\S^{v} = \Frac{1}{J}\,\deriv{W^{v}}{J}\,\deriv{I_{3}}{\C}
\]

Here,
\[
\deriv{W^{v}}{J}=K\,(J-1)
\]

We can implement it as follows:

~~~~~{.cpp}
const auto dPv_dJ   = K*(J-1);
const StressStensor Sv = dPv_dJ/J*dI3_dC;
~~~~~

Here, we have explicitly given the type of `Sv` to force its evaluation.

### Computation of the isochoric part of the second Piola Kirchhoff stress

We first compute the invariants of the modified right Cauchy tensor
\(\tenseur{\bar{C}}\) and their derivatives with respect to \(\C\).

\(\bar{I}_{1}\) and \(\bar{I}_{2}\) are related to the first and
second invariants of the right Cauchy tensor by:

\[
\begin{aligned}
  \bar{I}_{1} &= I_{3}^{-1/3}\,I_{1} = \iJb\,I_{1}\\
  \bar{I}_{2} &= I_{3}^{-2/3}\,I_{2} = \iJb^{2}\,I_{2}\\
\end{aligned}
\]

The expression of the derivatives of \(\bar{I}_{1}\) and
\(\bar{I}_{2}\) is detailled [here](hyperelasticity.html).

The computation of all those terms is implemented as follows:

~~~~~{.cpp}
const auto iJb        =  1/cbrt(I3);
const auto iJb2       =  power<2>(iJb);
const auto iJb4       =  iJb2*iJb2;
const auto diJb_dI3   = -iJb4/3;
const auto diJb_dC    = diJb_dI3*dI3_dC;
const auto I1b        =  I1*iJb;
const auto dI1b_dC    =  iJb*id+I1*diJb_dC;
const auto dI2b_dC    = iJb2*dI2_dC+2*I2*iJb*diJb_dC;
~~~~~

By chain rule, we have:
\[
  \S^{i} = 2\,\deriv{W^{i}}{\C}=2\,\deriv{W^{i}}{\bar{I}_{1}}\,\deriv{\bar{I}_{1}}{\C}+2\,\deriv{W^{i}}{\bar{I}_{2}}\,\deriv{\bar{I}_{2}}{\C}
\]

In the case of the Signorini behaviour, the derivatives of \(W^{i}\) are:
\[
\left\{
\begin{aligned}
\deriv{W^{i}}{\bar{I}_{1}} &= C_{01}+2\,C_{02}\,(\bar{I}_{1}-3)\\
\deriv{W^{i}}{\bar{I}_{2}} &= C_{10}\\
\end{aligned}
\right.
\]

The computation of \(\S^{i}\) is:

~~~~{.cpp}
const auto dPi_dI1b    = C01+2*C02*(I1b-3);
const auto dPi_dI2b    = C10;
const StressStensor Si = 2*(dPi_dI1b*dI1b_dC+dPi_dI2b*dI2b_dC);
~~~~

### Computation of the Cauchy stress

The function `convertSecondPiolaKirchhoffStressToCauchyStress`
converts the second Piola Kirchhoff stress to the Cauchy stress using
the deformation gradient. It is used as follows:

~~~~{.cpp}
sig = convertSecondPiolaKirchhoffStressToCauchyStress(Sv+Si,F1);
~~~~

## Computation of the consistent tangent operator

The most direct expression the consistent tangent operator is given by
\(\deriv{\S}{\C}\). We let `MFront` make the appropriate conversion to
the consistent tangent operator expected by the solver.

Here,
\[
\deriv{\S}{\C}=\deriv{\S^{v}}{\C}+\deriv{\S^{i}}{\C}
\]

### Computation of the second derivatives of the invariants of \(\C\) and \(\tenseur{\bar{C}}\)

~~~~{.cpp}
const auto d2I3_dC2 = computeDeterminantSecondDerivative(C);
const auto d2I2_dC2   = (id^id)-Stensor4::Id();
const auto iJb7       =  iJb4*power<3>(iJb);
const auto d2iJb_dI32 = 4*iJb7/9;
const auto d2iJb_dC2  = d2iJb_dI32*(dI3_dC^dI3_dC)+ diJb_dI3*d2I3_dC2;
const auto d2I1b_dC2  = (id^diJb_dC)+(diJb_dC^id)+I1*d2iJb_dC2;
const auto d2I2b_dC2  = 2*iJb*(dI2_dC^diJb_dC)+ iJb2*d2I2_dC2+
                        2*iJb*(diJb_dC^dI2_dC)+
                        2*I2*(diJb_dC^diJb_dC)+
                        2*I2*iJb*d2iJb_dC2;
~~~~

### Computation of the volumetric part of the consistent tangent operator

\[
\begin{aligned}
\deriv{\S^{v}}{\C}
	&=\deriv{}{J}\paren{\Frac{1}{J}\,\deriv{W^{v}}{J}}\,\deriv{J}{I_{3}}\,\deriv{I_{3}}{\C}\otimes\deriv{I_{3}}{\C}+\Frac{1}{J}\,\deriv{W^{v}}{J}\,\sderiv{I_{3}}{\C}\\
	&=\paren{\sderiv{W^{v}}{J}-\Frac{1}{J}\,\deriv{W^{v}}{J}}\,\frac{1}{2\,I_{3}}\,\deriv{I_{3}}{\C}\otimes\deriv{I_{3}}{\C}+\Frac{1}{J}\,\deriv{W^{v}}{J}\,\sderiv{I_{3}}{\C}\\
\end{aligned}
\]

For the Signorini behaviour:
\[
\sderiv{W^{v}}{J}=K
\]

~~~~{.cpp}
const auto d2Pv_dJ2 = K;
const auto dSv_dC  = ((d2Pv_dJ2-dPv_dJ/J)/(2*I3)*(dI3_dC^dI3_dC)
	                  +dPv_dJ/J*d2I3_dC2);
~~~~

### Computation of the isochoric part of the consistent tangent operator

\[
\begin{aligned}
\sderiv{\S^{i}}{C}=&
2\,\sderiv{W^{i}}{\bar{I}_{1}}\,\deriv{\bar{I}_{1}}{\C}\otimes\deriv{\bar{I}_{1}}{\C}+
2\,{\displaystyle \frac{\displaystyle \partial^{2} W^{i}}{\partial \bar{I}_{1}\partial \bar{I}_{2}}}\,\deriv{\bar{I}_{1}}{\C}\otimes\deriv{\bar{I}_{2}}{\C}+
2\,\deriv{W^{i}}{\bar{I}_{1}}\sderiv{\bar{I}_{1}}{\C}+\\
& 2\,\sderiv{W^{i}}{\bar{I}_{2}}\,\deriv{\bar{I}_{2}}{\C}\otimes\deriv{\bar{I}_{2}}{\C}+
2\,{\displaystyle \frac{\displaystyle \partial^{2} W^{i}}{\partial \bar{I}_{1}\partial \bar{I}_{2}}}\,\deriv{\bar{I}_{2}}{\C}\otimes\deriv{\bar{I}_{1}}{\C}+
2\,\deriv{W^{i}}{\bar{I}_{2}}\sderiv{\bar{I}_{2}}{\C}
\end{aligned}
\]

For the Signorini behaviour:
\[
\left\{
\begin{aligned}
\sderiv{W^{i}}{\bar{I}_{1}} &= 2\,C_{02} \\
\sderiv{W^{i}}{\bar{I}_{2}} &= {\displaystyle \frac{\displaystyle \partial^{2} W^{i}}{\partial \bar{I}_{1}\partial \bar{I}_{2}}} =
{\displaystyle \frac{\displaystyle \partial^{2} W^{i}}{\partial \bar{I}_{2}\partial \bar{I}_{1}}}=0
\end{aligned}
\right.
\]

~~~~{.cpp}
const auto d2Pi_dI1b2 = 2*C02;
const auto dSi_dC = 2*(d2Pi_dI1b2*(dI1b_dC^dI1b_dC)+dPi_dI1b*d2I1b_dC2+
	                                               +dPi_dI2b*d2I2b_dC2);
~~~~

# References

<!-- Local IspellDict: english -->
