---
title: The `Implicit` domain specific language
author: Thomas Helfer
date: 2020
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tabPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tns}[1]{{\underset{\tilde{}}{\mathbf{#1}}}}
\newcommand{\transpose}[1]{{#1^{\mathop{T}}}}

\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\tenseur{\varepsilon}^{\mathrm{el}}}
\newcommand{\tepsilonvp}{\tenseur{\varepsilon}^{\mathrm{vp}}}
\newcommand{\tdepsilonvp}{\tenseur{\dot{\varepsilon}}^{\mathrm{vp}}}
\newcommand{\tepsilonth}{\tenseur{\varepsilon}^{\mathrm{th}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sderiv}[2]{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial #2^{2}}}}
\newcommand{\derivtot}[2]{{\displaystyle \frac{\displaystyle \mathrm{d}#1}{\displaystyle \mathrm{d}#2}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}

![Description of the steps of the implicit scheme and the associated code blocks.](img/ImplicitDSL.svg ""){width=100%}

# Description

Let \(\vec{Y}\) be a vector holding all the integration variables[^1].
The evolution of \(\vec{Y}\) is assumed to satisfy the following
differential equation:

\[
\vec{\dot{Y}}=G\paren{\vec{Y},t}
\]

where \(t\) is not meant to describe an explicit dependency to the time
but rather a placeholder for variables whose evolutions are known
(those variables are called external state variables in `MFront`).

[^1]: In `MFront`, the integration variables refers to variables which
  are part of the implicit systems, i.e. variables the increments of
  which are the solutions of the implicit system. A state variable,
  declared with `@StateVariable` keyword, is saved from one step to the
  other and is automatically added to the list of integration variables.
  An auxiliary state variable, declared with `@AuxiliaryStateVariable`
  keyword, is also saved from one step to the other, but is not added to
  the list of the integration variables. The `@IntegrationVariable`
  keyword allows to append a variable to the integration variables but
  its value wil not be saved from one step to the other.

The following notations are used:

- \(\bts{\vec{Y}}\) denotes the value of the integration variables at
  the beginning of the time step.
- \(\Delta\,\vec{Y}\) denotes the increment of the integration
  variables.
- \(\ets{\vec{Y}}\) denotes the value of the integration variables at
  the end of the time step:
  \[
  \ets{\vec{Y}}=\bts{\vec{Y}}+\Delta\,\vec{Y}
  \]

An implicit scheme turns this ordinary differential equation into a non
linear system of equations whose unknowns are is increment
\(\Delta\,Y\) over a time step \(\Delta\,t\):

\[
\Delta\,\vec{Y}-G\paren{\bts{\vec{Y}}+\theta\,\Delta\,\vec{Y},t+\theta\,\Delta\,t}\,\Delta\,t=0
\]

The increment \(\Delta\,\vec{Y}\) of the state variables satisfies the
following implicit system:

\[
F\paren{\Delta\,\vec{Y},\Delta\,\tepsilonto}=0
\]{#eq:tangent_operator:implicit_system}

Equation @eq:tangent_operator:implicit_system implicitly defines \(\Delta\,\vec{Y}\) as
an implicit function of the increment of the increment of the strain
tensor \(\Delta\tepsilonto\) and may be rewritten as:

\[
\vec{F}\paren{\Delta\,\vec{Y}\paren{\Delta\,\tepsilonto},\Delta\,\tepsilonto}=0
\]{#eq:tangent_operator:implicit_system2}

# Available algorithms

The following algorithms are available:

- `NewtonRaphson`
- `NewtonRaphson_NumericalJacobian`
- `PowellDogLeg_NewtonRaphson`
- `PowellDogLeg_NewtonRaphson_NumericalJacobian`
- `Broyden`
- `PowellDogLeg_Broyden`
- `Broyden2`
- `LevenbergMarquardt`
- `LevenbergMarquardt_NumericalJacobian`

## Notes about updating auxiliary state variable or local variables in the `Integrator` code blocks when the numerical evaluation of the jacobian is requested

In some cases, it is convenient to update auxiliary state variables
in the `@Integrator` code block, rather than computing them in
the `@UpdateAuxiliaryStateVariables` code block which is only called
once the convergence is reached.

However, if the jacobian matrix is computed numerically (at least
partially), such updates could be wrong, because they can be based
on the perturbated values of the unknowns. 

In `TFEL 3.1`, this can be circumvented by testing the value of the
`perturbatedSystemEvaluation` boolean value as follows:

~~~~{.cpp}
// let av be an auxiliary state variable
@AuxiliaryStateVariable StrainStensor av;

@Integrator{
  // put updated value of av in a temporary variable
  const auto av_ = eval(...);  
  ...
  definition of the implicit system
  ...
  if(!perturbatedSystemEvaluation){
    // update auxiliary state variables
    av = av_;
  }
} // end of @Integrator
~~~~

In many cases, rather than updating auxiliary variables during the
Newton iterations, it can be more pratical to compute its increment,
defined in by local variable and to update the auxiliary variable in
the `@UpdateAuxiliaryStateVariables` code block. The previous trick can
be used in this case in a straightforward manner.

# Computation of the consistent tangent operator

## Computation of the consistent tangent operator for small strain behaviours {#sec:tangent_operator:small_strain_behaviours}

In this section, a small strain behaviour is considered.

The increment \(\Delta\,\vec{Y}\) of the state variables satisfies the
following implicit system:

\[
F\paren{\Delta\,\vec{Y},\Delta\,\tepsilonto}=0
\]{#eq:tangent_operator:small_strain:implicit_system}

Equation @eq:tangent_operator:small_strain:implicit_system implicitly defines \(\Delta\,\vec{Y}\) as
an implicit function of the increment of the increment of the strain
tensor \(\Delta\tepsilonto\) and may be rewritten as:

\[
\vec{F}\paren{\Delta\,\vec{Y}\paren{\Delta\,\tepsilonto},\Delta\,\tepsilonto}=0
\]{#eq:tangent_operator:small_strain:implicit_system2}


The stress tensor \(\tsigma\) are assumed to be an explicit function of
the integration variables \(\ets{\vec{Y}}\) at the end of the time step
and the total strain \(\ets{\tepsilonto}\) at the end of time step:

\[
\tsigma\paren{\ets{\vec{Y}}, \ets{\tepsilonto}}
\]

### Formal derivation of the consistent tangent operator

The consistent tangent operator is thus given by:

\[
\derivtot{\tsigma}{\Delta\,\tepsilonto}=
\deriv{\tsigma}{\Delta\,\tepsilonto}+
\deriv{\tsigma}{\Delta\,\vec{Y}}\,\deriv{\Delta\,\vec{Y}}{\Delta\,\tepsilonto}
\]

By differentiation:

\[
\derivtot{\vec{F}}{\Delta\,\tepsilonto}=
\deriv{\vec{F}}{\Delta\,\vec{Y}}\,\derivtot{\Delta\,\vec{Y}}{\Delta\,\tepsilonto}+
\deriv{\vec{F}}{\Delta\,\tepsilonto}=0
\]

\(\deriv{\vec{F}}{\Delta\,\vec{Y}}\) is the jacobian \(J\) of the implicit system.

Hence,

\[
\derivtot{\Delta\,\vec{Y}}{\Delta\,\tepsilonto}
=-J^{-1}\,\deriv{\vec{F}}{\Delta\,\tepsilonto}
\]

Finally, the consistent tangent operator is formally given by:

\[
\derivtot{\tsigma}{\Delta\,\tepsilonto}=
\deriv{\tsigma}{\Delta\,\tepsilonto}-
\deriv{\tsigma}{\Delta\,\vec{Y}}\,J^{-1}\,\deriv{\vec{F}}{\Delta\,\tepsilonto}
\]{#eq:tangent_operator:consistent_tangent_operator}

### Discussion {#sec:tangent_operator:discussion}

Equation @eq:tangent_operator:consistent_tangent_operator may be
extented almost directly to generalised behaviours. However, direct
usage of Equation @eq:tangent_operator:consistent_tangent_operator has
several practical disadvantages:

- The matrix \(\deriv{\tsigma}{\Delta\,\vec{Y}}\) is usually sparse. For
  example, most small strain behaviours are based on the Hooke law,
  which means that the stress \(\tsigma\) only depends on the elastic
  strain \(\tepsilonel\). In many behaviours, the elastic strain
  \(\tepsilonel\) are only a small subset of the integration
  variables \(\vec{Y}\).
- It requires to invert the jacobian matrix.
- The matrix \(\deriv{\vec{F}}{\Delta\,\tepsilonto}\) is also usually sparse.
  For example, the \(\Delta\,\tepsilonto\) commonly only appears in the
  implicit equation describing the split of the strain.

Hence, a computation of the consistent tangent based on Equation
@eq:tangent_operator:consistent_tangent_operator may imply a significant
performance hit which can be avoided in many most common cases, as
discussed in the following section.

### Simplication of the Equation @eq:tangent_operator:consistent_tangent_operator in common cases

Equation @eq:tangent_operator:consistent_tangent_operator can be
simplified if the following assumptions are made:

- The integration variable \(\vec{Y}\) is decomposed as:
  \[
  \vec{Y}=
  \begin{pmatrix}
    \Delta\,\tepsilonel\\
    \Delta\,\vec{z}
  \end{pmatrix}
  \]
  where \(\vec{z}\) is a sub-vector of \(\vec{Y}\) containing the other
  state variables. The implicit system is also decomposed in a similar
  manner:
  \[
  \vec{F}=
  \begin{pmatrix}
    f_{\tepsilonel}\\
    f_{\vec{z}}
  \end{pmatrix}
  \]
- The implicit equation \(f_{\tepsilonel}\) associated with the elastic
  strain holds the split of the strain, which means that is has the form:
  \[
  f_{\tepsilonel}=\Delta\tepsilonel-\Delta\tepsilonto+...
  \]
- The strain increment \(\Delta\tepsilonto\) only appears in
  \(f_{\tepsilonel}\). Thus, \(\deriv{\vec{F}}{\Delta\,\tepsilonto}\)
  has the form:
  \[
  \deriv{\vec{F}}{\Delta\,\tepsilonto}=
  \begin{pmatrix}
    1& 0 & 0 & 0& 0 &0 \\
    0& 1& 0 & 0& 0 &0 \\
    0& 0 & 1 & 0& 0 &0 \\
    0& 0 & 0 & 1& 0 &0 \\
    0& 0 & 0 & 0& 1 &0 \\
    0& 0 & 0 & 0& 0 &1\\
    0& 0 & 0 & 0& 0 &0 \\
    \vdots & \vdots & \vdots & \vdots & \vdots & \vdots\\
    0& 0 & 0 & 0& 0 &0 \\
  \end{pmatrix}
  \]{#eq:tangent_operator:common_cases:dF_ddeto}
- The stress tensor only depends on the elastic strain \(\tepsilonel\)
  through the Hooke law:
  \[
  \ets{\tsigma}=\ets{\tenseurq{D}}\,\colon\,\ets{\tepsilonel}
  \]{#eq:tangent_operator:small_strain:common_case:hooke_law}
  The expression of the consistent tangent operator is then:
  \[
  \derivtot{\tsigma}{\Delta\,\tepsilonto}=\ets{\tenseurq{D}}\,\colon\,\derivtot{\Delta\,\tepsilonel}{\Delta\,\tepsilonto}
  \]{#eq:tangent_operator:small_strain:common_case:consistent_tangent_operator}

Those assumptions are more or the less the basis upon which various
bricks in `MFront` are built
([`StandardElasticity`](http://tfel.sourceforge.net/HookeStressPotential.html),
[`StandardElastoViscoplasticity`](http://tfel.sourceforge.net/StandardElastoViscoPlasticityBrick.html),
[`DDIF2`](http://tfel.sourceforge.net/DDIF2Brick.html)).

Thanks to Equation @eq:tangent_operator:common_cases:dF_ddeto, one sees
that the product \(-J^{-1}\,\deriv{\vec{F}}{\Delta\,\tepsilonto}\) only
contains the the \(6\) first columns of the \(J^{-1}\). This allows
identifying \(\derivtot{\Delta\,\tepsilonel}{\Delta\,\tepsilonto}\) with
the \(6\times 6\) upper-left submatrx of \(J^{-1}\). Let
\(J_{\tepsilonel}^{-1}\) this submatrix:

\[
  J_{\tepsilonel}^{-1}=\derivtot{\Delta\,\tepsilonel}{\Delta\,\tepsilonto}
\]

Thanks to Equation
@eq:tangent_operator:small_strain:common_case:consistent_tangent_operator,
the consistent tangent operator finally reads:

\[
\derivtot{\tsigma}{\Delta\,\tepsilonto}=\ets{\tenseurq{D}}\,\cdot\,J_{\tepsilonel}^{-1}
\]{#eq:tangent_operator:small_strain:common_cases}

#### The `getPartialJacobianInvert` method {#sec:tangent_operator:getPartialTangentOperator}

The `getPartialJacobianInvert` method allows the computation of the
\(J_{\tepsilonel}^{-1}\) tensors as follows:

~~~~{.cxx}
Stensor4 iJe;
getPartialJacobianInvert(iJe);
~~~~

The `getPartialJacobianInvert` method assumes that the jacobian matrix
is decomposed (using the `LU` algorithm with partial pivoting in the
current version of `TFEL`), which is guaranteed in the
`@TangentOperatorBlock`. So \(J_{\tepsilonel}^{-1}\) is computed by
solving \(6\) linear systems. In particular, the jacobian matrix is not
inverted.

A typical computation of the consistent tangent operator is as follows:

~~~~{.cxx}
    StiffnessTensor De;
    Stensor4 iJe;
    computeElasticStiffness<N,Type>::exe(De,lambda,mu);
    getPartialJacobianInvert(iJe);
    Dt = De * Je;
~~~~

This code is loosely the one generated by the `StandardElastoViscoplasticy` brick.

#### Extensions to more general class of behaviours

The `getPartialJacobianInvert` may take several arguments which will
give the derivatives of the other integration variables.

For example, the following code can be used to compute the consistent
tangent operator of a behaviour in which the stress computed using a
standard damaging law of the form
\(\ets{\tsigma}=\paren{1-\ets{d}}\,\ets{\tenseurq{D}}\,\colon\,\ets{\tepsilonel}\)[^2]\(\mbox{}^{,}\)[^3]:

~~~~{.cxx}
    StiffnessTensor De;
    Stensor4 iJe;
    Stensor iJd;
    computeUnalteredElasticStiffness<N,Type>::exe(De, lambda, mu);
    getPartialJacobianInvert(iJe, iJd);
    Dt = (1 - d) * De * iJe - ((De * eel) ^ iJd);
~~~~

[^2]: This assumes that the damage \(d\) is the second integration variables.
[^3]: This code uses the fact the state variables have been updated to
  their values at the end of the time step before the call to
  `@TangentOperator` code block.

## Extension to generalised behaviours{#sec:tangent_operator:generalised_behaviours}

The idea is to extend this method for generalised behaviours.

### Discussion

In Section @sec:tangent_operator:discussion, we discussed various
drawbacks of a direct use of Equation
@eq:tangent_operator:consistent_tangent_operator.

The equivalent of the consistent tangent operator for generalised
behaviours are so called tangent operator blocks which can be:

- the derivatives of one of the thermodynamic force with respect to the
  increment of a gradient.
- the derivative of one of the thermodynamic force with respect to the
  increment of an external state variable gradient.
- the derivative of a state variable with respect to the increment of a
  gradient.
- the derivative of a state variable with respect to the increment of an
  external state variable.

In the spirit of the Section @sec:tangent_operator:small_strain_behaviours,
the computation of those derivatives boils down to computing the
derivatives of the increment of the state variables \(\Delta\,\vec{Y}\)
with respect to the increment of the considered variable \(X\) (\(X\)
can thus be a gradient or an external state variable).

As the derivative \(\deriv{F}{\Delta\,X}\) can not be assumed to be
sparse, two solutions can be considered:

- extend the `getPartialJacobianInvert` method.
- have access to the blocks of the invert of the jacobian matrix.

In both cases, \(\deriv{F}{\Delta\, X}\) matrix must be computed. The next
paragraph is devoted to this computation.

### Computation of the \(\deriv{F}{\Delta\, X}\) matrix.

The \(\deriv{F}{\Delta\, X}\) matrix can be decomposed by blocks:

\[
\deriv{F}{\Delta\, X}
=
\begin{pmatrix}
\deriv{f_{y_{1}}}{\Delta\, X} \\
\vdots \\\
\deriv{f_{y_{n}}}{\Delta\, X} \\
\end{pmatrix}
\]{#eq:tangent_operator:dF_dX}

where \(\paren{y_{1},\cdots\,y_{n}}\) denotes the set of integration
variables, i.e.:

\[
\vec{Y}=\begin{pmatrix}
y_{1}\\
\vdots\\
y_{n}
\end{pmatrix}
\quad\text{and}\quad
\vec{F}=\begin{pmatrix}
f_{y_{1}}\\
\vdots\\
f_{y_{n}}
\end{pmatrix}
\]

The advantage of the Decomposition @eq:tangent_operator:dF_dX is that
each blocks \(\deriv{f_{y_{i}}}{\Delta\, X}\) is a tensorial object that can be
computed using the facilities offered by `TFEL/Math` library

If one of the block \(\deriv{f_{y_{i}}}{\Delta\, X}\) appears in a
`@TangentOperator` code block, `MFront` automatically defines the matrix
\(\deriv{F}{\Delta\, X}\) as an hidden variable and **initializes it to
zero**. **This does mean that only non zero derivatives must be defined
by the user**

For a standard integration variable, a view called
\(\deriv{f_{y_{i}}}{\Delta\, X}\) is also automatically defined. For
array of integration variables, a local function object
\(\deriv{f_{y_{i}}}{\Delta\, X}\) is defined. This mechanism is similar
to the one used to access the jacobian by block.

### The `getIntegrationVariablesDerivatives_X` local function objects

When required in a `@TangentOperator` code block (i.e. when used), the
`getIntegrationVariablesDerivatives_X` is defined as a local function
object which may the blocks of \(-J^{-1}\,\deriv{F}{\Delta\, X}\)

### Block decomposition of the invert of the jacobian

The invert of the jacobian \(J\) can be decomposed as:

\[
J^{-1}=
\begin{pmatrix}
J^{-1}_{\paren{f_{y_{1}}, y_{1}}}&\cdots&J^{-1}_{\paren{f_{y_{1}}, y_{n}}}\\
\vdots & \ddots &\vdots \\
J^{-1}_{\paren{f_{y_{n}}, y_{1}}}&\cdots&J^{-1}_{\paren{f_{y_{n}}, y_{n}}}\\
\end{pmatrix}
\]

When required in the `@TangentOperator` code block (i.e. when used), for
each pair of state variables \(y\), \(z\), a variable called `iJ_x_z` is
defined:

- If \(y\) and \(z\) are not arrays, `iJ_x_z` is directly a view to the
  corresponding block of the invert of the jacobian.
- If \(y\) and/or \(z\) are arrays, `iJ_x_z` is a function object taking
  indices as arguments and returning a view to the corresponding block
  of the invert of the jacobian.

For example:

~~~~{.cxx}
@TangentOperator{
 // iJ_eel_p is the block corresponding to:
 // 1. the elastic strain (row)
 // 2. the equivalent viscoplastic strain
}
~~~~

## Applications

### A first example of a viscoplastic behaviour coupled with heat transfer

#### Description of the behaviour

Let us consider the following simple isotropic viscoplastic behaviour.

The total strain \(\tepsilonto\) is split into an elastic part
\(\tepsilonel\), a thermal strain \(\tepsilonth\) and a viscoplastic
part \(\tepsilonvp\):

\[
\Delta\,\tepsilonto=\Delta\,\tepsilonel+\Delta\,\tepsilonth+\Delta\,\tepsilonvp
\]

The thermal strain is given by:

\[
\tepsilonth=\alpha\paren{T}\,\paren{T-T_{\mathrm{ref}}}
\]

where:

- \(T\) is the temperature, assumed to be an external state variable.
- \(T_{\mathrm{ref}}\) is a reference temperature.

The stress tensor \(\tsigma\) is related to the elastic strain by the Hooke law:

\[
\tsigma=\lambda\paren{T}\,\trace{\tepsilonel}\,\tenseur{I}+2\,\mu\paren{T}\,\tepsilonel
\]

The viscoplastic strain rate is given by the famous Norton-Hoff law:

\[
\tdepsilonvp = \dot{p}\,\tenseur{n} = A\,\exp\paren{-\Frac{T_{a}}{T}}\,\sigmaeq^{E}\,\tenseur{n}
\]

where:

- \(\sigmaeq\) is the equivalent von Mises stress.
- \(\tenseur{n}\) is the von Mises normal.
- \(A\), \(T_{a}\) and \(E\) are material parameters.
- \(p\) is the equivalent plastic strain.

The integration variables are the elastic strain \(\tepsilonel\) and the
equivalent plastic strain \(p\).

The tangent operator blocks to be computed are
\(\deriv{\tsigma}{\Delta\,\tepsilonto}\) and
\(\deriv{\tsigma}{\Delta\,T}\).

The computation is of \(\deriv{\tsigma}{\Delta\,\tepsilonto}\) readily
follows Section @sec:tangent_operator:getPartialTangentOperator and will
not be described here.

#### Implicit scheme

\[
\left\{
\begin{aligned}
f_{\tepsilonel}&=\Delta\,\tepsilonel+\Delta\,\tepsilonth+\Delta\,p\,\mts{\tenseur{n}}-\Delta\tepsilonto\\
f_{p}&=\Delta\,p-A\,\exp\paren{-\Frac{T_{a}}{\mts{T}}}\,\paren{\mts{\sigmaeq}}^{E}
\end{aligned}
\right.
\]

### Computation of \(\deriv{F}{\Delta T}\)

\[
\left\{
\begin{aligned}
\deriv{f_{\tepsilonel}}{\Delta T}&=\deriv{\Delta\tepsilonth}{\Delta T}=\deriv{\ets{\tepsilonth}}{\Delta T}=
\paren{\alpha\paren{\ets{T}}+\left.\derivtot{\alpha}{T}\right|_{\ets{T}}\,\paren{\ets{T}-T_{\mathrm{ref}}}}\,\tenseur{I}\\
\deriv{f_{p}}{\Delta T}&=-A\,\theta\,\Frac{T_{a}}{\mts{T}^{2}}\,\exp\paren{-\Frac{T_{a}}{\mts{T}}}\,\paren{\mts{\sigmaeq}}^{E}
\end{aligned}
\right.
\]

~~~~{.cxx}
∂fεᵉˡ∕∂ΔT = ...;
∂fp∕∂ΔT = ...;
~~~~

### Computing \(\deriv{\tsigma}{\Delta\,T}\) using `getIntegrationVariablesDerivatives_T`

~~~~{.cxx}
@TangentOperatorBlock{
  ∂fεᵉˡ∕∂ΔT = ...;
  ∂fp∕∂ΔT = ...;
  auto ∂Δεᵉˡ∕∂ΔT = Stensor{};
  getIntegrationVariablesDerivatives_T(∂Δεᵉˡ∕∂ΔT);
  const auto λ =...;
  const auto μ =...;
  const auto ∂λ∕∂T =...;
  const auto ∂μ∕∂T =...;
  const auto De = λ  ⋅ (I₂ ⊗ I₂) + 2 ⋅ μ ⋅ I₄;  
  const auto ∂De∕∂T = ∂λ∕∂T  ⋅ (I₂ ⊗ I₂) + 2 ⋅ ∂μ∕∂T ⋅ I₄;  
  ∂σ∕∂ΔT = De ⋅ ∂Δεᵉˡ∕∂ΔT + ∂De∕∂T ⋅ εᵉˡ;
}
~~~~

### Computing \(\deriv{\tsigma}{\Delta\,T}\) using the block decomposition of the invert of jacobian

~~~~{.cxx}
@TangentOperatorBlock{
  const auto λ =...;
  const auto μ =...;
  const auto De = λ  ⋅ (I₂ ⊗ I₂) + 2 ⋅ μ ⋅ I₄;  
  // computation of ∂σ∕∂ΔT
  ∂fεᵉˡ∕∂ΔT = ...;
  ∂fp∕∂ΔT = ...;
  const auto ∂Δεᵉˡ∕∂ΔT = -(iJ_fεᵉˡ_εᵉˡ ⋅ ∂fεᵉˡ∕∂ΔT + iJ_fεᵉˡ_p ⋅ ∂fp∕∂ΔT);
  const auto ∂λ∕∂T =...;
  const auto ∂μ∕∂T =...;
  const auto ∂De∕∂T = ∂λ∕∂T  ⋅ (I₂ ⊗ I₂) + 2 ⋅ ∂μ∕∂T ⋅ I₄;  
  ∂σ∕∂ΔT = De ⋅ ∂Δεᵉˡ∕∂ΔT + ∂De∕∂T ⋅ εᵉˡ;
}
~~~~

