---
title: Mechanical behaviours in MFront
author: Thomas Helfer, Jérémy Hure, Mohamed Shokeir
date: 15/10/2014
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\mathbf{#1}}}
\newcommand{\tns}[1]{\underset{\tilde{}}{\mathbf{#1}}}
\newcommand{\transpose}[1]{#1^{\mathop{T}}}

\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}

\newcommand{\epsilonth}{\epsilon^{\mathrm{th}}}

\newcommand{\tepsilonto}{\underline{\epsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\underline{\epsilon}^{\mathrm{el}}}
\newcommand{\tepsilonth}{\underline{\epsilon}^{\mathrm{th}}}

\newcommand{\tepsilonvis}{\underline{\epsilon}^{\mathrm{vis}}}
\newcommand{\tdepsilonvis}{\underline{\dot{\epsilon}}^{\mathrm{vis}}}

\newcommand{\tepsilonp}{\underline{\epsilon}^{\mathrm{p}}}
\newcommand{\tdepsilonp}{\underline{\dot{\epsilon}}^{\mathrm{p}}}

\newcommand{\talpha}{\underline{\alpha}}
\newcommand{\tdalpha}{\underline{\dot{\alpha}}}
\newcommand{\txi}{\underline{\xi}}
\newcommand{\tdxi}{\underline{\dot{\xi}}}

\newcommand{\tDq}{\underline{\mathbf{D}}}
\newcommand{\trace}[1]{\mathrm{tr}\paren{#1}}
\newcommand{\Frac}[2]{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\dtot}{{\mathrm{d}}}
\newcommand{\paren}[1]{\left(#1\right)}
\newcommand{\nom}[1]{\textsc{#1}}
\newcommand{\bts}[1]{\left.#1\right|_{t}}
\newcommand{\mts}[1]{\left.#1\right|_{t+\theta\,\Delta\,t}}
\newcommand{\ets}[1]{\left.#1\right|_{t+\Delta\,t}}

# Overview

Versatility of general purpose mechanical solver mainly relies on its
ability to let the user define the material behaviour. `MFront`
provides a high level language to write mechanical behaviours.

It can be compared to the `ZebFront` tool developped by the Centre des
Matériaux de Mines ParisTech as part of the Zset software
[see @foerch_polymorphic_1997;@besson_object-oriented_1998-1;@northwest_numerics_and_modeling_inc._zebfront_2014]. One
major difference between `ZebFront` and `MFront` is the programming
techniques used: `ZebFront` mostly relies on object oriented
techniques where `MFront` relies on generic programming leading to
almost orthogonal design choices.

Three kind of mechanical behaviours are currently considered with
`MFront`: small and finite strain behaviours and cohesive zone
models.

## Mechanical behaviour role

We now precise the role of the mechanical behaviours in standard
displacement-based finite element solver
[see @zienkiewicz_finite_1977;@besson_mecanique_2001;@edf_algorithme_2013]. For
the sake of simplicity, we only treat the case of small strain
behaviours for the rest of this document.

At each time step, the following resolution procedure is used:

1. a prediction of the displacement is made. Such a prediction may use
  the derivate of the stress tensor with respect of the strain tensor
  \(\deriv{\tsigma}{\tepsilonto}\) or an approximation of it. This
  prediction step will not be discussed in this article but can also
  be handled by behaviour implementations made with `MFront`;
2. an iterative process similar to the Newton-Raphson algorithm used
  to find a displacement that will satisfy the mechanical
  equilibrium. Given an estimation of the displacement at the end the
  time step, one computes at each integration point an estimation of
  the increment of the deformation tensor \(\Delta\,\tepsilonto\). The
  mechanical behaviour is then called and provides an associated
  estimation of the stress tensor \(\tsigma\) and the values of some
  internal state variables \(Y_{i}\). In the solver requires it, the
  mechanical behaviour may also provide an estimation of the tangent
  consistant operator \(\deriv{\Delta\,\tsigma}{\Delta\,\tepsilonto}\)
  (see @simo_consistent_1985) which is used to estimate a more
  accurate displacement field.

A mechanical behaviour can thus be viewed as functional:

\[
\paren{\ets{\tsigma},\ets{Y_{i}},\deriv{\Delta\,\tsigma}{\Delta\,\tepsilonto}}=
\mathcal{F}\paren{\bts{\tsigma},\bts{Y_{i}},\Delta\,\tepsilonto,\Delta\,t,\ldots}
\]

The dots \(\ldots\) means that the behaviour may also depend of
external state variables evolutions with time, namely the temperature,
the irradiation damage, and so on.

## Isotropic $J_{2}$ plastic/viscoplastic behaviours. Example of finite strain pre- and post-processing

\(4\) domain specific languages address the case of small strain
isotropic \(J_{2}\) plastic and/or viscoplastic behaviours which are
of common use and for which efficient implicit scalar radial return
mapping algorithms exist (see @simo_computational_1998).

The following listing shows how a simple plastic behaviour can be
implemented:

~~~~ {#Plasticity .cpp .numberLines}
@DSL IsotropicPlasticMisesFlow; //< domain specific language
@Behaviour Plasticity;             //< name of the behaviour
@Parameter  H  = 22e9;             //< hardening slope
@Parameter s0 = 200e6;             //< elasticity limit
@FlowRule{                         //< flow rule
  f       = seq-H*p-s0;
  df_dseq = 1;
  df_dp   = -H;
}
~~~~

The plastic behaviour is governed by the following yield
surface (see @besson_mecanique_2001;@chaboche_mecanique_2009):

\[
f\paren{\sigmaeq,p}=\sigmaeq-H\,p-\sigma_{0} \leq 0
\]

where \(\sigmaeq\) is the Von Mises stress, \(p\) the equivalent
plastic strain, \(R\) the isotropic hardening slope and \(\sigma_{0}\)
the initial elasticity limit.

The generated code represent a total amount of \(1\,512\) lines of
`̀C++` code and provides:

1. optimised implementations of the behaviour for various modelling
  hypotheses (axisymmetrical generalised plane strain, plane strain,
  plane stress, generalised plane strain, axisymmetry, tridimensional)
  thanks to template metaprogramming and template specialisations. An
  small overview of the programming techniques used can be found
  in the description of the [TFEL/Math library](tfel-math.html).
2. the computation of the prediction operator;
3. meta-data about the required material properties, the number of
  states variables, etc... that can be retrieved dynamically (For the
  sake of simplicity, no glossary name was specified in this example).
  This mechanism is typically used by solvers based on the [`MGIS`
  libray](https://thelfer.github.io/mgis/web/index.html) appropriately
  call the behaviour;
4. the computation of a tangent matrix operator (various choice are
  possible: elastic, secant, consistent);
5. dynamically loadable functions allowing the user to change various
  parameters of the behaviour (the convergence criterion of the
  implicit algorithm, the \(\theta\) parameter of the implicit
  algorithm, the hardening slope \(H\) and the initial elasticity
  limit \(\sigma_{0}\), etc.). Those functions by-pass the standard
  behaviour call and are an extremely light-weight manner to
  dynamically modify a behaviour (almost no runtime cost).

### Local divergence

Local divergence of the implicit algorithm can be handled through an
appropriate substepping procedure. This feature is not enabled by
default, but appropriate keywords gives to the end user explicit
control on this procedure.

### Finite strain strategies

If not handled directly by the calling code, appropriate pre- and
post-processings allowing the use of small strain behaviours in finite
strain computations can be generated. Two lagrangian finite strain
strategies are currently available:

1. finite rotations, small strains\cite{edf_loi_2013}. This
  method allows the re-use a behaviour whose material parameters \(H\)
  and \(\sigma_{0}\) were identified through small strain computations
  in the context of finite rotations without any
  re-identification. The physical meaning of the pre- and
  post-processing stages are discussed by
  Doghri (see @doghri_mechanics_2000);
2. lagrangian logarithmic strains as proposed by Miehe, Apel and
  Lambrecht (see @miehe_anisotropic_2002;edf_modeles_2013).  The use
  of the logarithmic strains has several advantages:
   i.  it preserves the small-strain classical meaning of the variables,
       which is truly appreciated by engineers;
   ii. it may can also be used for arbitrary complex models (kinematic
       hardening, initial or induced anisotropy, etc.).

The following figure shows how our example can be used to model a
notched specimen under a tensile test:

![Von Mises stress](img/LogarithmicStrainPlasticity-ssna303.svg "Von Mises stress")
		   
In this case, the material parameters \(H\) and \(\sigma_{0}\) of the
behaviour must be identified on tests implying finite strains. As an
additional remark, the results found using logarithmic strains were
remarkably closed to those obtained by the classical formulation based
on an \(F_{e}\,F_{p}\) decomposition proposed by Simo and Miehe (see
@simo_associative_1992) (that was also implemented using `MFront`).

## Generic domain specific languages

Apart from the domain specific languages dealing with isotropic
\(J_{2}\) plastic and viscoplastic behaviours presented in the
previous paragraph, `MFront` also provides several general-purpose
domain specific languages:

1. the `Default` domain specific language allows the user the
  write its own integration algorithm. This is very useful for
  explicit behaviours such as the classical \nom{Tvergaard} cohesive
  zone model [see @tvergaard_effect_1990].
2. the `Runge-Kutta` domain specific language allows the user
  to write the constitutive equations given as a system of ordinary
  time differential equations. Using those algorithms is generally
  less efficient than using implicit integration to be
  described. Various algorithms are however available.
3. the `Implicit` domain specific language allows the user to
  perform the local integration using an implicit algorithm. An
  introduction to those algorithms is given in the next
  paragraph.

### Example of a cohesive zone model

The implementation of the Tvergaard cohesive zone model using the
`Default` domain specific language is given below:

~~~~ {#Tvergaard .cpp .numberLines}
@DSL DefaultCZM;     // domain specific language
@Behaviour Tvergaard;         // name of the behaviour
@MaterialProperty stress kn;  // normal stiffness
@MaterialProperty stress ks;  // tangential elastic stiffness
@MaterialProperty stress smax;// maximal stress
@MaterialProperty real delta; // maximal normal opening displacement
@StateVariable real d;        // damage variable
@Integrator{
  const real C = real(27)/real(4);
  t_t = ks*(u_t+du_t);        // tangential behaviour
  if(u_n+du_n<0){             // normal behaviour in compression
    t_n = kn*(u_n+du_n);
  } else {                    // normal behaviour in traction
    const real rod = (u_n+du_n)/delta;   // reduced opening displacement
    const real d_1 = d;             // previous damage
    d   = min(max(d,rod),0.99);     // damage indicator
    const real K1 = C*smax/delta;   // initial stiffness
    const real K  = K1*(1-d)*(1-d); // secant stiffness
    t_n = K*(u_n+du_n);
  }
} // end of @Integrator
~~~~

Details about the computation of the consistent tangent operator were
eluded. The opening displacement \(\vec{u}\) is automatically
decomposed into the normal opening displacement \(u_{n}\) and its
tangential opening displacement \(\vec{u}_{t}\)

### Explicit algorithm example


The implementation of a generalisation of the Norton
creep law for anisotropic materials is given below:

~~~~ {#OrthotropicCreep .cpp .numberLines}
@DSL    RungeKutta;              // domain specific language
@Behaviour OrthotropicCreep;     // name of the behaviour
@OrthotropicBehaviour;           // treating an orthotropic behaviou
@RequireStiffnessTensor;         // requires the stiffness tensor to be computed
@StateVariable Stensor evp;      // viscoplastic strain
@StateVariable strain p;         // Equivalent viscoplastic strain
@ComputeStress{                  /* stress computation */
  sig = D*eel;
}
@Derivative{                     /* constitutive equations */
  st2tost2<N,real> H;            // Hill Tensor
  H = hillTensor<N,real>(0.371,0.629,4.052,1.5,1.5,1.5);
  stress sigeq = sqrt(sig|H*sig);  // equivalent Hill stress
  if(sigeq>1e9){                   // automatic sub-stepping
    return false;
  }
  Stensor  n(real(0));                // flow direction
  if(sigeq > 10.e-7){
    n    = H*sig/sigeq;
  }
  dp   = 8.e-67*pow(sigeq,8.2); // evolution of p
  devp = dp*n;                  // evolution of the viscoplastic strains
  deel = deto - devp;           // evolution of the elastic strains
}
~~~~

Integration is performed in the material referential. The elastic
strain state variable \(\tepsilonel\) is automatically declared. For
each state variable `Y`, its time derivative `dY` is automatically
declared.

## Implicit integration

This section provides a very coarse overview of the `Implicit` domain
specific language. The reader can refer to [this
page](implicit-dsl.html) for a comprehensive description.

If the evolution of the state variables, grouped into a single vector
\(Y\) whose components \(Y_{i}\) may be scalars or symmetric tensors,
is given by the following system of differential equations:
\[
\dot{Y}=G\paren{Y,t} \quad\quad \Leftrightarrow\quad\quad
\left\{
\begin{aligned}
\dot{Y}_{0} &= g_{Y_{0}}\paren{Y,t}\\
&\ldots \\
\dot{Y}_{i} &= g_{Y_{i}}\paren{Y,t}\\
&\ldots \\
\dot{Y}_{N} &= g_{Y_{N}}\paren{Y,t}\\
\end{aligned}
\right.
\]

where the dependency with respect to time stands for the evolution of
some external state variables and the evolution of strains (for small
strain behaviours) which are supposed to evolve *linearly* during the
time step.

The integration of this ordinary differential equation over a time
step \(\Delta\,t\) using an implicit algorithm leads to the (generally
non-linear) system of equations [see @besson_numerical_2004]:
\[
F\paren{\Delta\,Y}=0\quad \Leftrightarrow\quad
\left\{
\begin{aligned}
f_{Y_{0}}&=\Delta\,Y_{0}-\Delta\,t\,g_{y_{0}}\paren{\mts{Y},t}&=0 \\
&\ldots \\
f_{Y_{i}}&=\Delta\,Y_{i}-\Delta\,t\,g_{y_{i}}\paren{\mts{Y},t}&=0 \\
&\ldots \\
f_{Y_{N}}&=\Delta\,Y_{N}-\Delta\,t\,g_{y_{N}}\paren{\mts{Y},t}&=0 \\
\end{aligned}
\right.
\]

where the unknowns are the state variables increments
\(\Delta\,Y_{i}\), and we introduced the following notation:
\[
  \mts{Y}=Y+\theta\,\Delta\,Y
\]

Algorithms used to solve this system of equations may require the
jacobian matrix \(J\) of \(F\) which can be computed by
blocks [see @besson_mecanique_2001]:
\[
  J=\deriv{F}{\Delta\,Y}=
  \begin{pmatrix}
    \deriv{f_{y_{1}}}{\Delta\,y_{1}} & \ldots & \ldots & \ldots & \ldots \\
    \vdots & \vdots & \vdots & \vdots & \vdots \\
    \vdots & \vdots & \deriv{f_{y_{i}}}{\Delta\,y_{j}} & \vdots & \vdots \\
    \vdots & \vdots & \vdots & \vdots & \vdots \\
    \ldots & \ldots & \ldots & \ldots & \deriv{f_{y_{N}}}{\Delta\,y_{N}} \\
  \end{pmatrix}
\]

### Time independent mechanisms

For state variable associated with time-independent mechanisms, the
implicit equation shall impose that the system lies on the yield
surface when plastic loading occurs.

### Available algorithms

Several algorithms are available to solve the previous implicit system:

- `NewtonRaphson` is the standard Newton-Raphson algorithm:
  \[
  \Delta\,Y^{\paren{n+1}}=\Delta\,Y^{\paren{n}}-J^{-1}\,.\,F\paren{\Delta\,Y^{\paren{n}}}
  \]
  The user must explicitly compute the jacobian matrix, which constitutes
  the main difficulty of this method. For debugging purposes, `MFront` may
  generate the comparison of each block of the jacobian matrix with a
  numerical approximation.
- `NewtonRaphson_NumericalJacobian` is a variation of the standard
  Newton-Raphson algorithm using a jacobian matrix computed by a second
  order finite difference. Writing behaviour implementations using this
  algorithm is as easy as using the domain specific languages based on
  `RungeKutta` algorithm. It can be considered as a first step toward an
  implicit implementation with an analytical jacobian matrix.
- `Broyden` algorithms which do not require to computation of the
  jacobian matrix: these algorithms update an approximation of the
  jacobian matrix (first Broyden algorithm) or its inverse (second
  Broyden algorithm) at each iteration. The first Broyden algorithm can
  sometimes be interesting as one may compute analitically some part of
  the jacobian matrix and let the algorithm compute the other parts. If
  the computation of those other parts takes a significant amount of CPU
  time, this algorithm can in same cases outperfom the Newton-Raphson
  algorithm.
- `PowellDogLeg_XX` algorithm, where `XX` is one of the previous
  algorithm. Those trust-region algorithms implements the classical
  Powell dogleg method [see @chen_modification_1981] to improve the
  robustness of the resolution.

### Consistent tangent operator

For most small strain behaviours, algorithms providing the jacobian
matrix \(J\) of the implicit system have a significant advantage: the
consistent tangent operator
\(\deriv{\Delta\,\sigma}{\Delta\,\tepsilonto}\) can be computed almost
automatically with only a small numerical cost.

### Example

The Norton creep law can be implemented as follows:

~~~~ {#ImplicitNorton .cpp .numberLines}
@DSL Implicit;
@Behaviour ImplicitNorton;
@Brick StandardElasticity;

@MaterialProperty stress young; /* mandatory for castem */
young.setGlossaryName("YoungModulus");
@MaterialProperty real nu;    /* mandatory for castem */
nu.setGlossaryName("PoissonRatio");

@LocalVariable stress lambda,mu;

@StateVariable real    p;
@PhysicalBounds p in [0:*[;

/* Initialize Lame coefficients */
@InitLocalVariables{
  using namespace tfel::material::lame;
  lambda = computeLambda(young,nu);
  mu = computeMu(young,nu);
} // end of @InitLocalVars

@Integrator{
  const real A = 8.e-67;
  const real E = 8.2;
  const auto seq = sigmaeq(sig);
  const auto tmp = A*pow(seq,E-1.);
  const auto df_dseq = E*tmp;
  const auto iseq = 1/max(seq,real(1.e-8)*young);
  const auto n    = eval(3*deviator(sig)*iseq/2);
  feel += dp*n;
  fp   -= tmp*seq*dt;
  // jacobian
  dfeel_ddeel += 2.*mu*theta*dp*iseq*(Stensor4::M()-(n^n));
  dfeel_ddp    = n;
  dfp_ddeel    = -2*mu*theta*df_dseq*dt*n;
} // end of @Integrator
~~~~~~~~~

# References

<!-- Local IspellDict: english -->
