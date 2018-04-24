% How to implement an hyperviscoelastic behaviour in MFront
% Thomas Helfer
% 15/12/2016

\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tns}[1]{{\underset{\tilde{}}{\mathbf{#1}}}}
\newcommand{\transpose}[1]{{#1^{\mathop{T}}}}

\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sderiv}[2]{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial #2^{2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}

\newcommand{\C}{\tenseur{C}}
\newcommand{\S}{\tenseur{S}}
\newcommand{\H}{\tenseur{H}}
\newcommand{\iJb}{\bar{c}_{I_{3}}}
\newcommand{\diJb}{\deriv{\iJb}{I_{3}}}
\newcommand{\sdiJb}{\sderiv{\iJb}{I_{3}}}

This article shows how to implement hyperviscoelastic behaviours in
`MFront`.

The hyperelastic behaviour is left unspecified. We only assume that
one can compute \(\S^{\infty}\) and its derivative
\(\deriv{\S^{\infty}}{\C}\).

The reader can refer to the following pages describing the
implementation of various hyperelastic behaviours:

- [Signorini](signorini.html)
- [Ogden](ogden.html)

The formalism used by most hyperelastic behaviours is described in
[this page](hyperelasticity.html).

An example of implementation based on the Signorini hyperelastic
behaviour is available here:
[SignoriniHyperViscoelasticity.mfront](./gallery/hyperviscoelasticity/SignoriniHyperViscoelasticity.mfront)

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
>
>~~~~{.bash}
>CXXFLAGS="`tfel-config --oflags` -std=c++11" mfront --obuild --interface=aster SignoriniHyperViscoelasticity-tfel-2.0.3.mfront`
>~~~~
>
> An implementation where those changes were made is available
> [here](./gallery/hyperviscoelasticity/SignoriniHyperViscoelasticity-tfel-2.0.3.mfront)

# Computation of the Cauchy stress

The hyperelastic behaviour is supposed to decouple the volumetric
stress \(\S^{v\,\infty}\) and the isochoric stress \(S^{i\,\infty}\):
\[
\S^{\infty}=\S^{v\,\infty}+\S^{i\,\infty}
\]

The second Piola-Kirchhoff stress is given by:

\[
\S = \S^{\infty}+\sum_{i=1}^{N}\H_{i}
\]

where \(\H_{i}\) are the overstresses.

## Evolution of the overstresses

The algorithm used to update \(\H_{i}\) is the following:

\[
\ets{\H_{i}}=\exp\paren{-\Frac{dt}{\tau_{i}}}\bts{\H_{i}}+g_{i}\,\tau_{i}\paren{1-\exp\paren{-\Frac{dt}{\tau_{i}}}}\Frac{\paren{\ets{S^{i\,\infty}}-\bts{S^{i\,\infty}}}}{dt}
\]

## Computation of second Piola-Kirchhoff stress

~~~~{.cpp}
// updating the viscoelastic stresses and computing the Second
// Piola-Kirchhoff stress
StressStensor S = Sv+Se;
if(dt>0){
  for(unsigned short i=0;i!=Nv;++i){
    const auto dtr = dt/tau[i];
    e[i] = exp(-dtr);
    H[i] = e[i]*H[i]+g[i]*(1-e[i])/dtr*(Se-Se_1);
    S += H[i];
  }
}
~~~~

## Computation of the Cauchy stress

The function `convertSecondPiolaKirchhoffStressToCauchyStress`
converts the second Piola Kirchhoff stress to the Cauchy stress using
the deformation gradient. It is used as follows:

~~~~{.cpp}
sig = convertSecondPiolaKirchhoffStressToCauchyStress(S,F1);
~~~~

# Computation of the consistent tangent operator

Assuming that \(\deriv{\S^{v\,\infty}}{\C}\) and
\(\deriv{\S^{i\,\infty}}{\C}\) are known, the expression of the
consistent tangent operator is:

\[
\begin{aligned}
\deriv{\S^{v\,\infty}}{\C} &= \deriv{\S^{v\,\infty}}{\C}+\deriv{\S^{i\,\infty}}{\C}+ \sum_{i=1}^{N}\deriv{\ets{\H_{i}}}{\C}\\
&= \deriv{\S^{v\,\infty}}{\C}+
   \deriv{\S^{i\,\infty}}{\C}\,\underbrace{\paren{1+\sum_{i=1}^{N}g_{i}\,\tau_{i}\,\Frac{\paren{1-\exp\paren{-\Frac{dt}{\tau_{i}}}}}{dt}}}_{c}
\end{aligned}
\]

This expression can be implemented in a straightforward manner:

~~~~{.cpp}
auto c = real(1);
if(dt>0){
  for(unsigned short i=0;i!=Nv;++i){
    const auto dtr = dt/tau[i];
    c += g[i]*(1-e[i])/dtr;
  }
}
dS_dC = dSv_dC+c*dSi_dC;
~~~~

<!-- Local IspellDict: english -->
