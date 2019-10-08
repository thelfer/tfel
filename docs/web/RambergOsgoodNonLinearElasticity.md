% Non linear elasticity of the Ramberg-Osgood type
% Thomas Helfer, Duc Vinh Nguyen
% 10/11

\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\tenseur{\varepsilon}^{\mathrm{el}}}
\newcommand{\tepsilonin}{\tenseur{\varepsilon}^{\mathrm{in}}}
\newcommand{\tdepsilonto}{\tenseur{\dot{\varepsilon}}^{\mathrm{to}}}
\newcommand{\tdepsilonel}{\tenseur{\dot{\varepsilon}}^{\mathrm{el}}}
\newcommand{\tdepsilonin}{\tenseur{\dot{\varepsilon}}^{\mathrm{in}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\sigmaH}{\sigma_{H}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\derivtot}[2]{{\displaystyle \frac{\displaystyle \mathrm{d} #1}{\displaystyle \mathrm{d} #2}}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\epsiloneq}{\varepsilon_{\mathrm{eq}}}
\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}

In fracture mechanics, it is classical to replace the plastic behaviour
by an equivalent non linear elastic behaviour.

This document describes the implementation of non linear elastic
behaviour of the Ramberg-Osgood type using the `Default` domain specific
language of `MFront`.

The full implementation is available [here](gallery/nonlinearelasticity/RambergOsgoodNonLinearElasticity.mfront).

> **Note**
>
> This implementation is compatible with `MFront-3.3` and uses unicode
> characters (see [this page](unicode.html) for details).

# Description of the behaviour

The behaviour is based on the following relationship between the strain
\(\tepsilonto\) and the stress \(\tsigma\):

\[
\tepsilonto = \Frac{1}{3\,K}\,\trace{\tsigma}\,\tenseur{I}+
\Frac{\sigmaeq}{3\,\mu}\,\tenseur{n}+
\alpha\,\paren{\Frac{\sigmaeq}{\sigma_{0}}}^{n}\,\tenseur{n}
\]{#eq:sigeto}

where:

- \(K\) is the bulk modulus.
- \(\mu\) is the shear modulus.
- \(\sigma_{0}\), \(\alpha\) and \(n\) are material properties
  describing the pseudo-plastic part of the behaviour
- \(\tenseur{s} = \tsigma-\Frac{\trace{\tsigma}}{3}\,\tenseur{I}\) is
  the deviatoric part of the stress tensor.
- \(\sigmaeq\) is the von Mises equivalent stress.
- \(\tenseur{n}=\Frac{3\,\tenseur{s}}{2\,\sigmaeq}\) is the von Mises
  normal.

# Algorithm

## Computation of the stress{#sec:StressComputation}

Knowing the strain \(\tepsilonto\), integration of the behaviour, which
boils down to the computation of the stress \(\tsigma\), requires the
inversion of Equation @eq:sigeto.

The projection of this equation on the volumetric axis yields:

\[
\trace{\tsigma}=K\,\trace{\tepsilonto}
\]

Taking the deviatoric part of Equation @eq:sigeto yields:

\[
 \tenseur{s}^{e}= \paren{\Frac{\sigmaeq}{3\,\mu}+
 \alpha\,\paren{\Frac{\sigmaeq}{\sigma_{0}}}^{n}}\,\tenseur{n} = 
 f\paren{\sigmaeq}\,\tenseur{n}
\]{#eq:sigeto_dev}

Contracting each side of his equations with itself leads to:

\[
 \tenseur{s}^{e}\,\colon\,\tenseur{s}^{e} =
 f^{2}\paren{\sigmaeq}\,
 \tenseur{n}\,\colon\,\tenseur{n} = 
 \Frac{3}{2}\,f^{2}\paren{\sigmaeq}
\]

where we used the classical result:
\(\tenseur{n}\,\colon\,\tenseur{n}=\Frac{3}{2}\).

Finally, \(\sigmaeq\) satisfies:

\[
 f\paren{\sigmaeq}=\epsiloneq
\]{#eq:sigmaeq}

with
\(\epsiloneq=\sqrt{\Frac{2}{3}\,\tenseur{s}^{e}\,\colon\,\tenseur{s}^{e}}\).

The scalar Equation @eq:sigmaeq allows resolving iteratively
\(\sigmaeq\) using a standard Newton-Raphson algorithm. Let
\(\sigmaeq^{(i)}\) be the current estimate of \(\sigmaeq\), the next
estimate is given by:

\[
\sigmaeq^{(i+1)}=\sigmaeq^{(i)}-\Frac{f\paren{\sigmaeq^{(i)}}}{\derivtot{f}{\sigmaeq}\paren{\sigmaeq^{(i)}}}
\]

The initial estimation is choosen as follows:

\[
\sigmaeq^{(0)}=\sigma_{0}\,\sqrt[n]{\Frac{\epsiloneq}{\alpha}}
\]

Once \(\sigmaeq\) is known, Equations @eq:sigeto_dev and @eq:sigmaeq
allow computing the normal \(\tenseur{n}\):

\[
\tenseur{n}=
\Frac{1}{f\paren{\sigmaeq}}\,\tenseur{s}^{e}=
\Frac{1}{\epsiloneq}\,\tenseur{s}^{e}
\]

The following relation allows computing the stress:

\[
\tsigma 
=K\,\trace{\tepsilonto}\,\tenseur{I}+\tenseur{s}
=K\,\trace{\tepsilonto}\,\tenseur{I}+\Frac{2\,\sigmaeq}{3}\tenseur{n}
=K\,\trace{\tepsilonto}\,\tenseur{I}+\Frac{2\,\sigmaeq}{3\,\epsiloneq}\,\tenseur{s}^{e}
\]

Finally:
\[
\tsigma=K\,\trace{\tepsilonto}\,\tenseur{I}+\sigmaeq\,\tenseur{n}^{e}
\]{#eq:sigma}
with \(\tenseur{n}^{e}=\Frac{2}{3\,\epsiloneq}\,\tenseur{s}^{e}\).

## Computation of the tangent operator

The following relations will be useful:
\[
\begin{aligned}
\derivtot{\epsiloneq}{\tepsilonto}&=\tenseur{n}^{e}\\
\derivtot{\tenseur{n}^{e}}{\tepsilonto}&=
\Frac{1}{\epsiloneq}\paren{\Frac{2}{3}\,\tenseurq{P}-\tenseur{n}^{e}\,\otimes\,\tenseur{n}^{e}}\\
\end{aligned}
\]
with \(\tenseurq{P}=\tenseurq{I}-\Frac{1}{3}\,\tenseur{I}\,\otimes\,\tenseur{I}\)

Equation @eq:sigma allows computing the tangent operator as follows:

\[
\derivtot{\tsigma}{\tepsilonto}=
K\,\tenseur{I}\,\otimes\,\tenseur{I}+
\derivtot{\sigmaeq}{\epsiloneq}\,\tenseur{n}^{e}\,\otimes\,\tenseur{n}^{e}+
\sigmaeq\,\derivtot{\tenseur{n}^{e}}{\tepsilonto}
\]{#eq:tangentoperator}

\(\derivtot{\sigmaeq}{\epsiloneq}\) is obtained by derivation of
Equation @eq:sigmaeq as follows:

\[
f\paren{\sigmaeq\paren{\epsiloneq}}=\epsiloneq \Rightarrow
\derivtot{\sigmaeq}{\epsiloneq}=\Frac{1}{\deriv{f}{\sigmaeq}}
\]{#eq:dseq_deeq}

# Implementation

## Choice of domain specific language

While not mandatory (the `@DSL` keyword can be place anywhere in the
file), its is convenient to start the implementation by declaring the
domain specific language to be used. The `Default` DSL, which does not
set any particular integration scheme, is the most appropriate here:

~~~~{.cxx}
@DSL Default;
~~~~

## Name of the behaviour

The `@Behaviour` keyword is used to give the name of the behaviour.

~~~~{.cxx}
@Behaviour RambergOsgoodNonLinearElasticity;
~~~~

## Metadata

The following instructions give some information about the author, the
date and some description of the behaviour.

~~~~{.cxx}
@Author L. Gelebart/V. Duc Nguyen;
@Description {
  "Ramberg-Osgood model (non-(linear elastic formulation)"
  "Eps = S:Sig +β.σ_eq^n. N"
  "with:"
  " - N = (3/2) Sig_dev / σ_eq"
  " - σ_eq = rac((3/2)σ_dev:σ_dev))"
  " - β = α⋅σ₀/E"
}
~~~~

## Material properties

This behaviour is meant to be generic, which means that the specific
characteristics of the material are to be given by the calling solver.

Here, we declare \(5\) material properties:

- Two properties associated with the elastic part of the behaviour: the
  Young modulus \(E\) and the Poisson ratio \(\nu\). Although this set
  of elastic properties is not the most convenient for this specific
  case (the bulk modulus and the shear modulus are more appropriate), it
  is compatible with the requirement of the `Cast3M` solver.
- Three properties associated with the plastic part of the behaviour:
  \(n\), \(\alpha\) and the \(\sigma_{0}\).

~~~~{.cxx}
@MaterialProperty stress E;
E.setGlossaryName("YoungModulus");
@MaterialProperty real ν;
ν.setGlossaryName("PoissonRatio");
@MaterialProperty real n;
@MaterialProperty real α;
α.setEntryName("alpha");
@MaterialProperty stress σ₀;
σ₀.setGlossaryName("YieldStrength");
~~~~

Here, we use the subset of UTF-8 characters supported by `MFront` (See
[this page](unicode.html) for details) to define the variables. Note
that it is important to associate so called external names to those
variables using one of the `setEntryName` and `setGlossaryName` methods:
it is highly recommended that the external names only contains `ASCII`
characters.

## A numerical parameter

The following numerical parameter will be used as threshold to avoid
division by zero.

~~~~{.cxx}
//! Numerical threshold 
@Parameter real e_ε = 1e-12;
e_ε.setEntryName("NumericalThreshold");
~~~~

## Local variables

The following local variables are used:

~~~~{.cxx}
//! the shear modulus
@LocalVariable stress μ;
//! the bulk modulus
@LocalVariable stress K;
//! an helper variable
@LocalVariable real β;
//! von Mises equivalent stress
@LocalVariable stress σₑ;
/*!
 * derivative of the von Mises equivalent stress
 * with respect to the equivalent strain
 */
@LocalVariable stress ∂σₑ∕∂εₑ;
//! the equivalent strain
@LocalVariable strain εₑ;
//! inverse of the equivalent strain
@LocalVariable real iεₑ;
//! normal associated with the deviatoric part of the strain
@LocalVariable StrainStensor ne;
~~~~

Local variables are available in each code blocks.

## Initialisation of local variables

The initialisation of local variables is devoted to the
`@InitLocalVariables`.

~~~~{.cxx}
@InitLocalVariables {
  μ = computeMu(E, ν);
  K = E / (3 ⋅ (1 - 2 ⋅ ν));
  β = α ⋅ σ₀ / E;
  σₑ = stress{};
  ∂σₑ∕∂εₑ = stress{};
}
~~~~

Note that we use here the symbol `⋅` in place of `*` as the
multiplication sign. Both are equivalent.

The statement `σₑ = stress{};` initializes `σₑ` to a default value
(which is `0`).

## Prediction operator

The prediction operator can used by `code_aster` to initialize the
algorithm used to find the equilibrium. Here we only implement an
elastic prediction operator.

~~~~{.cxx}
@PredictionOperator{
  const auto P = I₄ - (I₂ ⊗ I₂) / 3;
  Dt = K ⋅ (I₂ ⊗ I₂) + 2 ⋅ μ ⋅ P;
}
~~~~

where:

- `I₄` is the fourth order identity tensor (of type `st2tost2`, i.e. a
  fourth order tensor which transforms a symmetric tensor to a a
  symmetric tensor).
- `I₂` is the second order identity symmetric tensor
- `⊗` is the tensorial product. It is equivalent to the `^` characters.

## The behaviour integration

The behaviour integration is `@Integrator`.

~~~~{.cxx}
@Integrator{
~~~~

The integrator part is divided in three parts:

- The first part computes some useful variables.
- The second part deals with the computation of the von Mises stress
  `σₑ` and its derivative `∂σₑ∕∂εₑ`.
- The last part computes the stress tensor.

## First part of the computation

The first part computes the strain at the end of the time step, its
deviatoric part, the equivalent strain and the normal.

~~~~{.cxx}
  const auto ε = εᵗᵒ + Δεᵗᵒ;
  const auto se = deviator(ε);
  εₑ = sqrt(2 ⋅ (se | se) / 3);
  iεₑ = 1 / max(εₑ, e_ε);
  ne = 2 ⋅ se ⋅ (iεₑ / 3);
~~~~

### Computation of the von Mises stress and its derivative

The following code computes the von Mises stress and its derivative:

~~~~{.cxx}
  if (εₑ < e_ε) {
    σₑ = 3 ⋅ μ ⋅ εₑ;
    ∂σₑ∕∂εₑ = 3 ⋅ μ;
  } else {
    const auto e_σ = E ⋅ e_ε;
    const auto i3μ = 1 / (3 ⋅ μ);
    auto f = real{};
    auto fidf = [&]() {
      const auto rσₑⁿ = pow(σₑ / σ₀, n);
      const auto iσₑ = 1 / max(e_σ, σₑ);
      f = σₑ ⋅ i3μ + β ⋅ rσₑⁿ - εₑ;
      ∂σₑ∕∂εₑ = 1 / max(i3μ + n ⋅ β ⋅ rσₑⁿ ⋅ iσₑ, i3μ ⋅ e_ε);
    };
    σₑ = σ₀ ⋅ pow(εₑ / β, 1 / n);
    auto iter = int{};
    fidf();
    while (abs(f) > e_ε) {
      fidf();
      σₑ -= f ⋅ ∂σₑ∕∂εₑ;
      if (++iter > 20) {
        throw(DivergenceException());
      }
    }
  }
~~~~

If the equivalent strain is below the numerical threshold, a solution
corresponding to an elastic behaviour is returned.

In the other case, a local Newton algorithm is used to determine `σₑ`
and `∂σₑ∕∂εₑ`, as described by Section @sec:StressComputation. Except
for the usage of a lambda function called `ifdf`, the code is quite easy
to read. A lambda function can be seen as a local function which allows
to factorise the code computing the residual and its derivative. The
syntax `[&]` means that the lambda function of has access to all the
variables of the surrounding scope by references. Note that we used
Equation @eq:dseq_deeq to identify \(\Frac{1}{\derivtot{f}{\sigmaeq}}\)
to `∂σₑ∕∂εₑ`.

### Computation of the stress

The computation of the stress is a direct translation of Equation
@eq:sigma:

~~~~{.cxx}
  σ = K ⋅ trace(ε) ⋅ I₂ + σₑ ⋅ ne;
~~~~

### Finalisation

The `@Integrator` code block ends with a final curly brace.

~~~~{.cxx}
} // end of @Integrator
~~~~

# Computation of the tangent operator

The following block compute the tangent operator using Equation
@eq:tangentoperator:

~~~~{.cxx}
@TangentOperator{
  const auto P = I₄ - (I₂ ⊗ I₂) / 3;
  if ((smt == ELASTIC) || (smt == SECANTOPERATOR)) {
    Dt = K ⋅ (I₂ ⊗ I₂) + 2 ⋅ μ ⋅ P;
  } else {
    if (εₑ < e_ε) {
      Dt = K ⋅ (I₂ ⊗ I₂) + 2 ⋅ μ ⋅ P;
    } else {
      Dt = K ⋅ (I₂ ⊗ I₂) + ∂σₑ∕∂εₑ ⋅ (ne ⊗ ne) +
           σₑ ⋅ iεₑ ⋅ (2 ⋅ P / 3 - (ne ⊗ ne));
    }
  }
}
~~~~

# Examples of usage

## Uniaxial tensile test

![Result of an uniaxial tensile test](./img/RambergOsgoodNonLinearElasticity-UniaxialTensileTest.png "Result of an uniaxial tensile test")