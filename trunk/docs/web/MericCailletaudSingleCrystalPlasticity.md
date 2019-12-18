% How to implement the Méric-Cailletaud single crystal behaviour
% Thomas Helfer, Alexandre Bourceret
% 25/10/2019

\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}
\newcommand{\incr}[1]{\Delta\,#1}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tns}[1]{{\underset{\tilde{}}{\mathbf{#1}}}}
\newcommand{\transpose}[1]{{#1^{\mathop{T}}}}

\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\tenseur{\varepsilon}^{\mathrm{el}}}
\newcommand{\tepsilonvis}{\tenseur{\varepsilon}^{\mathrm{vis}}}
\newcommand{\tdepsilonvis}{\tenseur{\dot{\varepsilon}}^{\mathrm{vis}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\sigmaH}{\sigma_{H}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sderiv}[2]{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial #2^{2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\range}[1]{\paren{{#1}_{i}}_{i\in[1:N]}}
\newcommand{\sign}[1]{\mathop{sgn}\paren{#1}}

This article shows how to implement the Méric-Cailletaud single crystal
viscoplastic behaviour in `MFront`. Such an example illustrates:

- The usage of `StandardElasticity` brick (see
  [this page](BehaviourBricks.html)).
- The usage of the functionnalities introduces by the `TFELNumodis`
  library, as described [here](singlecrystal.html). In particular, we
  will automatically let `MFront` generate the slips systems and the
  interaction matrix.
- The notion of integration variables and auxiliary state variables.

> **Advices**
>
> This kind of behaviour is rather complex and requires lot of care to
> get it work properly in any solver. We highly recommend that you follow
> these steps when implementing such behaviour:
> 
> 1. Validate the definition of your material orientation on a simple
>     orthotropic behaviour.
> 2. Validate the definition of the slips systems and the interaction
>      matrix using `mfront-query` as advocated on [this
>      page](singlecrystal.html).
> 3. Start validating the behaviour without isotropic hardening nor
>      kinematic hardening, then add the isotropic hardening and finally
>      the kinematic hardenings.
> 4. Start by using a numerical jacobian, then compute each terms of the
>     jacobian analytically (see the `@NumericallyComputedJacobianBlocks`
>     keyword to compute one block at a time).
> 5. Watch out that most implementation of the Méric-Calletaud uses a very 
>     high value for the stress exponent (typically \(70\) to \(100\)) which
>     may lead to many divergences of a standard Newton-Raphson
>     algorithm. This issue is discussed at the end of this document.

The implementations described in this document are available here:

- [`MericCailletaudSingleCrystalViscoPlasticityNumericalJacobian.mfront`](gallery/viscoplasticity/MericCailletaudSingleCrystalViscoPlasticityNumericalJacobian.mfront):
  the implementation at small strain with a numerical jacobian
- [`MericCailletaudSingleCrystalViscoPlasticity.mfront`](gallery/viscoplasticity/MericCailletaudSingleCrystalViscoPlasticity.mfront):
  the implementation at small strain with an analytical jacobian
- [`MericCailletaudFiniteStrainSingleCrystalViscoPlasticity.mfront`](gallery/finitestrainsinglecrystal/MericCailletaudFiniteStrainSingleCrystalViscoPlasticity.mfront):
  the implementation at finite strain with an analytical jacobian.

# Description of the behaviour

The theoretical frame which allow the monocrystalline model was
introduced in the 70. Monocrystalline behaviour describes the slip of
the crystallographic structure throught the slip systems of the
considerated crystal. Slips systems are defined using two vectors: the normal of the slip plan and the slip direction. 

<--!
From each slip systems the symmetric part of the Schmid tensor
tenseur{m}_{i} is defined. Due to this tensor which describes the slips
systems in 3 dimensions, the monocrystalline behaviour law can only be
express in 3 dimensions models. The Schmid tensor allows the orientation
of the constraint tensor on each slip system, which is the resolved
shear stress \tau_{i}. The inelastic flow is defined by dot{\gamma}_{i}
and depends on the chosen potential. The cumulated viscoplastic strain
is defined by dot{p}_{i} which is the absolute value of the inelastic
flow. The hardening R_{i} is calculated for each slip systems,and it can
be described by the sum of all the interactions each slip system has
experienced with all the others slips systems. Interactions are
describes using the interaction matrix which represents the geometrical
relation between the two slip systems which are interacting. Finally,
the defined viscoplastic strain rate \tepsilonvis is a sum of all the
hardenings o
-->

The behaviour is described by a standard decomposition of the strain
\(\tepsilonto\) in an elastic and a viscoplastic component, respectively
denoted \(\tepsilonel\) and \(\tepsilonvis\):

\[
\tepsilonto=\tepsilonel+\tepsilonvis
\]

## Elastic behaviour

The stress \(\tsigma\) is related to the the elastic strain
\(\tepsilonel\) by a the orthotropic elastic stiffness
\(\tenseurq{D}\):

\[
\tsigma = \tenseurq{D}\,\colon\,\tepsilonel
\]


## Viscoplastic behaviour

In the crystal frame, the viscoplastic strain rate is the summation of
the slips along each slip systems, as follows:

\[
\tdepsilonvis=\sum_{i=1}^{N}\dot{\gamma}_{i}\,\tenseur{m}_{i}
\]

where \(\range{\tenseur{m}}\) are the symmetric part of
the orientation tensors, defined by:

\[
\tenseur{m}_{i}=\Frac{1}{2}\,\paren{\vec{n}_{i}\,\otimes\,\vec{b}_{i}+\vec{b}_{i}\,\otimes\,\vec{n}_{i}}
\]

with \(\vec{n}_{i}\) and \(\vec{b}_{i}\) being respectively the
normal to the slip plane and the slip direction. \(\vec{n}_{i}\) and
\(\vec{b}_{i}\) being orthogonal, the viscoplastic flow is
incompressible [^1].

[^1]: The trace of \(\tenseur{m}_{i}\) is proportional to \(\vec{n}_{i}\,\cdot\,\vec{b}_{i}\)

The computation of the dissipated power yields:

\[
\tsigma\,\colon\,\tdepsilonvis=
\sum_{i=1}^{N}\dot{\gamma}_{i}\,\tsigma\,\colon\,\tenseur{m}_{i}=
\sum_{i=1}^{N}\dot{\gamma}_{i}\,\tau_{i}
\]

This equation shows that that the split rate is \(\dot{\gamma}_{i}\) is
conjugated to the resolved shear stress
\(\tau_{i}=\tsigma\,\colon\,\tenseur{m}_{i}\).

The Méric-Cailletaud behaviour is a scalar equivalent of the Norton-Hoff
behaviour with isotropic and kinematic hardening along each slip plane,
i.e. the split rate \(\dot{\gamma}_{i}\) is expressed as:

\[
\left\{
\begin{aligned}
\dot{\gamma}_{i}&=\dot{p}_{i}\,\sign{\tau_{i}-x_{i}}\\
\dot{p}_{i}&=\left\langle\Frac{\left|\tau_{i}-x_{i}\right|-R_{i}-\tau_{0}}{K}\right\rangle^n
\end{aligned}
\right.
\]{#eq:vp}

where
\(p_{i}=\displaystyle\int_{t_{0}}^{t}\left|\dot{\gamma}_{i}\right|\,\dtot\,t\)
is the equivalent viscoplastic slip.

The isotropic hardening rule couples the slip every systems as follows:

\[
R_{i}=Q\,\sum_{j=1}^{N}h_{ij}\,\paren{1-\exp\paren{-b\,p_{j}}}
\]{#eq:R}

where \(h\) is the so-called interaction matrix

The kinematic hardening follows a scalar equivalent of the
Armstrong-Frederick law:

\[
\left\{
\begin{aligned}
x_{i}&=C\,\alpha_{i} \\
\dot{\alpha}_{i}&=\dot{\gamma}_{i}-D\,\alpha_{i}\,\dot{p}_{i}
\end{aligned}
\right.
\]{#eq:x}

# Integration algorithm{#sec:integration:algorithm}

The behaviour will be integrated using an implicit scheme as described
[here](implicit-dsl.html).

## Implicit system by direct discretization

*A priori*, The unknowns are the increments of the elastic strain
  \(\Delta\,\tepsilonel\), of the plastic slips
  \(\range{\Delta\,\gamma}\), of the equivalent plastic slips
  \(\range{\Delta\,p}\) and of the back-strains
  \(\range{\Delta\,\alpha}\). After a straightforward discretization,
  the following system of non equations are to be solved:

\[
\left\{
\begin{aligned}
f_{\tepsilonel}  &= \Delta\,\tepsilonel-\Delta\,\tepsilonto+\sum_{i=1}^{N}\Delta\,\gamma_{i}\,\tenseur{m}_{i}&=0 \\
f_{\gamma_{i}} &= \Delta\,\gamma_{i}-\Delta\,p_{i}\,\mathop{sgn}\paren{\mts{\tau_{i}}-\mts{x_{i}}}&=0 \\
f_{p_{i}} &= \Delta\,p_{i}-\Delta\,t\,\left\langle\Frac{\left|\mts{\tau_{i}}-\mts{x_{i}}\right|-\mts{R_{i}}-\tau_{0}}{K}\right\rangle^n &=0\\
f_{\alpha_{i}}&= \Delta\,\alpha_{i}-\Delta\,\gamma_{i}+D\,\mts{\alpha_{i}}\,\Delta\,p_{i}&=0
\end{aligned}
\right.
\]{#eq:F}

where :

- \(\mts{\tau_{i}}=\mts{\tsigma}\,\colon\,\tenseur{m}_{i}\) with
  \(\mts{\tsigma}=\tenseurq{D}\,\colon\,\mts{\tepsilonel}\) and
  \(\mts{\tepsilonel}=\bts{\tepsilonel}+\theta\,\incr{\tepsilonel}\)
- \(\mts{x_{i}}=C\,\mts{\alpha_{i}}\) with
  \(\mts{\alpha}=\bts{\alpha}+\theta\,\incr{\alpha}\)
- \(\displaystyle\mts{R_{i}}=Q\,\sum_{j=1}^{N}h_{ij}\,\paren{1-\exp\paren{-b\,\mts{p_{j}}}}\)
  with \(\mts{p}=\bts{p}+\theta\,\incr{p}\)

This number of unknowns of System @eq:F is egal to \(6+3\,N_{ss}\) where
\(N_{ss}\) is the number of slip systems and \(6\) is the number of
component of the elastic strain.

## Reduction of the number of unknowns

We now show how to reduce the size of this system to \(6+N_{ss}\).

> **Notes**
>
> Reducing the number of unknowns has two main impacts:
>
> - The size of the linear system to be solved at each
>   time step is greatly reduced

> - When using a numerical jacobian, the number of evaluations of the
>   implicit system is reduced from \(13+12\,N_{ss}\) to \(13+2\,Nss\)
>   per iteration of the Newton algorithm (finite centered difference).
>   If \(Nss\) is \(12\), as in our example, the number of evaluations
>   is thus \(37\) rather than \(157\) !

First, the relationship between \(\Delta\,p_{i}\) and
\(\Delta\,\gamma_{i}\) is trivial:

\[
\Delta\,p_{i}=\left|\Delta\,\gamma_{i}\right|
\]

One may choose one of them and eliminate the other for the system. In
the following, we keep \(\Delta\,\gamma_{i}\).

It shall be noted that:

- We only need \(\Delta\,\gamma_{i}\) as an unknown of the implicit
  system but do not need to save it. In `MFront`, such variables are
  called *integration variables* and are declared through the
  `@IntegrationVariable` keyword. Note that one may save the values of
  \(\Delta\,\gamma_{i}\) only by changing its declaration from
  `@IntegrationVariable` to `@StateVariable`, which can be usefull for
  debugging purposes or for post-processing: this possibility is the
  main reason to keep \(\Delta\,\gamma_{i}\) rather than
  \(\Delta\,p_{i}\).
- We need to keep the save the values of \(p_{i}\) for one step to the
  other. In `MFront`, such variables are called *auxiliary state
  variables* and are declared through the `@AuxiliaryStateVariable`
  keyword.

Second, \(f_{\alpha_{i}}\) can be used to express \(\Delta\,\alpha_{i}\) as a
function of \(\Delta\,p_{i}\) and \(\Delta\,\gamma_{i}\) :

\[
\begin{aligned}
\Delta\,\alpha_{i}&=
\Frac{\Delta\,\gamma_{i}-D\,\bts{\alpha_{i}}\,\Delta\,p_{i}}{1+D\,\theta\,\Delta\,p_{i}}=
\Frac{\Delta\,\gamma_{i}-D\,\bts{\alpha_{i}}\,\left|\Delta\,\gamma_{i}\right|}{1+D\,\theta\,\left|\Delta\,\gamma_{i}\right|}\\
&=\paren{1-D\,\bts{\alpha_{i}}\,\sign{\Delta\,\gamma_{i}}}\,\Frac{\Delta\,\gamma_{i}}{1+D\,\theta\,\sign{\Delta\,\gamma_{i}}\,\Delta\,\gamma_{i}}
\end{aligned}
\]{#eq:alpha}

Finally, the implicit system to be solved is:

\[
\left\{
\begin{aligned}
f_{\tepsilonel}  &= \Delta\,\tepsilonel-\Delta\,\tepsilonto+\sum_{i=1}^{N}\Delta\,\gamma_{i}\,\tenseur{m}_{i}&=0 \\
f_{\gamma_{i}}&=\Delta\,\gamma_{i}-\Delta\,t\,v\paren{f}\,\sign{\tau_e}
\end{aligned}
\right.
\]{#eq:F2}

with:

- \(v\paren{f}=\left\langle\Frac{f}{K}\right\rangle^n\)
- \(f=\left|\tau_e\right|-R_{i}-\tau_0\)
- \(\tau_e=\mts{\tau_{i}}-\mts{x_{i}}\)

The latter notations have been introduced to ease the analytical
computation of the jacobian, see Section @sec:J.

# Implementation using a numerical jacobian

## Choice of the domain specific language

The implementation begins with the choice of the `Implicit` domain
specific language (dsl):

~~~~{.cpp}
@DSL Implicit;
~~~~

Note that this dsl automatically declares the elastic strain `eel` as
a state variable.

## Metadata

The following code declares the name of the behaviour, the names of the
authors, and the date.

~~~~{.cpp}
@Behaviour MericCailletaudSingleCrystalViscoPlasticityNumericalJacobian;
@Author Thomas Helfer, Alexandre Bourceret;
@Date 17 / 10 / 2019;
~~~~

## Supported modelling hypothesis

This behaviour is only valid in \(3D\). The following line restricts the
behaviour to be only usable in \(3D\):

~~~~{.cpp}
@ModellingHypothesis Tridimensional;
~~~~

## Material symmetry

The behaviour describes an orthotropic material:

~~~~{.cpp}
@OrthotropicBehaviour;
~~~~

> **Note**
> 
> As the behaviour is only valid in \(3D\), there is no need to specify any
> orthotropic axises convention (see the `@OrthotropicBehaviour`
> keyword for details).

## Numerical parameters

The following lines declare that we want to use a standard
Newton-Raphson algorithm with a numerically evaluated jacobian. The
perturbation value used to evaluate the jacobian is chosen equal to
\(10^{-7}\) which is a reasonnable value.

~~~~{.cpp}
@Algorithm NewtonRaphson_NumericalJacobian;
@PerturbationValueForNumericalJacobianComputation 1.e-7;
~~~~

The stopping criterion is chosen low, to ensure the quality of the
consistent tangent operator (the default value, \(10^{-8}\) is enough
to ensure a good estimation of the state variable evolution, but is
not enough to have a proper estimation of the consistent tangent
operator):

~~~~{.cpp}
@Epsilon 1e-14;
~~~~

We explicit state that a fully implicit integration will be used by
default:

~~~~{.cpp}
@Theta 1;
~~~~

This value can be changed at runtime by modifying the `theta`
parameter.

### Usage of the `StandardElasticity` brick

To implement this behaviour, we will use the `StandardElasticity`
brick which provides:

- Automatic computation of the stress tensor at various stages of the
  behaviour integration.
- Automatic computation of the consistent tangent operator.
- Automatic support for plane stress and generalized plane stress
  modelling hypotheses (The axial strain is defined as an additional
  state variable and the associated equation in the implicit system is
  added to enforce the plane stess condition).
- Automatic addition of the standard terms associated with the elastic
  strain state variable.

This behaviour brick is fully described [here](BehaviourBricks.html).

The usage of the `StandardElasticity` is declared as follows:

~~~~{.cpp}
@Brick StandardElasticity{
  young_modulus1 : 208000,
  young_modulus2 : 208000,
  young_modulus3 : 208000,
  poisson_ratio12 : 0.3,
  poisson_ratio23 : 0.3,
  poisson_ratio13 : 0.3,
  shear_modulus12 : 80000,
  shear_modulus23 : 80000,
  shear_modulus13 : 80000
};
~~~~

> **Note**
> 
> Here, we chose to declare the elastic material properties as part of
> the brick declaration. This pratice has been introduced in `TFEL-3.2`
> with the development of the `StandardElastoViscoPlasticity`brick.
> The material properties can also be computed using a formula or
> an external `MFront` files. 
>
> The elastic material properties can also be introduced by:
>
> - using the `@ComputeStiffnessTensor`.
> - using the `@RequireStiffnessTensor`, which requires the calling
>   solver to provide the stiffness tensor. For most interfaces, it is means
>   that additional material properties are required.

The declaration of the elastic material properties automatically defines
the variables `D` and `D_tdt` which respectively holds the stiffness
tensor at the middle of the time step (at \(t+\theta\,\incr{t}\) and the
stiffness at the end of the time step (at \(t+\incr{t}\).

### Definition of the crystal structure, the slip systems and the interaction matrix

The following lines, based on the `TFELNumodis` library introduced in
`TFEL 3.1.0` defines the the crystal structure, the slip systems and the
interaction matrix:

~~~~{.cpp}
@CrystalStructure FCC;
@SlidingSystem<0, 1, -1>{1, 1, 1};
@InteractionMatrix{1, 1, 0.6, 1.8, 1.6, 12.3, 1.6};
~~~~

Those keywords are fully documented on [this page](singlecrystal.html).
We highly recommend that the user take the time to understandand how they work.

In this particular case, the family `<0, 1, -1>{1, 1, 1}` generates
\(12\) slips systems by symmetry. The generated slip systems and their
associated indexes can be retrieved by the `mfront-query` tool:

~~~~{.bash}
$ mfront-query MericCailletaudSingleCrystalViscoPlasticityNumericalJacobian.mfront --slip-systems-by-index
- 0: [0,1,-1](1,1,1)
- 1: [1,0,-1](1,1,1)
- 2: [1,-1,0](1,1,1)
- 3: [0,1,1](1,1,-1)
- 4: [1,0,1](1,1,-1)
- 5: [1,-1,0](1,1,-1)
- 6: [0,1,-1](1,-1,-1)
- 7: [1,0,1](1,-1,-1)
- 8: [1,1,0](1,-1,-1)
- 9: [0,1,1](1,-1,1)
- 10: [1,0,-1](1,-1,1)
- 11: [1,1,0](1,-1,1)
~~~~

The interation matrix, which is non symmetric, has \(7\) independent
coefficients. The precise structure of this interaction matrix can be
retrieved thank to the `mfront-query` tool as follows:

~~~~{.bash}
$ mfront-query MericCailletaudSingleCrystalViscoPlasticityNumericalJacobian.mfront --interaction-matrix
| 0 1 1 2 3 4 5 6 6 2 4 3 |
| 1 0 1 3 2 4 4 2 3 6 5 6 |
| 1 1 0 6 6 5 4 3 2 3 4 2 |
| 2 3 4 0 1 1 2 4 3 5 6 6 |
| 3 2 4 1 0 1 6 5 6 4 2 3 |
| 6 6 5 1 1 0 3 4 2 4 3 2 |
| 5 6 6 2 4 3 0 1 1 2 3 4 |
| 4 2 3 6 5 6 1 0 1 3 2 4 |
| 4 3 2 3 4 2 1 1 0 6 6 5 |
| 2 4 3 5 6 6 2 3 4 0 1 1 |
| 6 5 6 4 2 3 3 2 4 1 0 1 |
| 3 4 2 4 3 2 6 6 5 1 1 0 |
with:
- coefficient '0': 1
- coefficient '1': 1
- coefficient '2': 0.6
- coefficient '3': 1.8
- coefficient '4': 1.6
- coefficient '5': 12.3
- coefficient '6': 1.6
~~~~

Those instructions also automatically defines :

- an integer constant called `Nss` which holds the number of slip
  systems (\(12\) here).
- an class which contains the slip systems and the interaction matrix.
  The name of this class is the name of the behaviour followed by
  `SlipSystems`. In this example, this class' name is thus:
  `MericCailletaudSingleCrystalViscoPlasticityNumericalJacobianSlipSystems`.
  This class provides the `getSlipSystems` methods which returns the
  unique instance of this class (singleton).

### Material parameters

The following block of code defines the various material parameters used
in the constitutive equations associated with the plastic slips:

~~~~{.cpp}
@Parameter n = 10.0;
@Parameter K = 25.;
@Parameter tau0 = 66.62;
@Parameter Q = 11.43;
@Parameter b = 2.1;
@Parameter d = 494.0;
@Parameter C = 14363;
~~~~

### Integration variables, state variables and auxiliary state variables

As described earlier (see Section @sec:integration:algorithm), the
viscoplastic slips \(\range{\gamma}\) are declared as integration
variables as follows:

~~~~{.cpp}
@IntegrationVariable strain g[Nss];
g.setEntryName("ViscoplasticSlip");
~~~~

As discussed before, declaring the viscoplastic slips as state variables
would save their values from one step to the other which can be
convenient for debugging and/or post-processing.

The equivalent viscoplastic slips and the back-strains are declared as
auxiliary state variables:

~~~~{.cpp}
@AuxiliaryStateVariable strain p[Nss];
p.setEntryName("EquivalentViscoplasticSlip");

@AuxiliaryStateVariable strain a[Nss];
a.setEntryName("BackStrain");
~~~~

## Implicit system

The `@Integrator` block allows the definition of the implicit system. We
divided it into three sections:

- a preamble which retrieves the object containing the slip systems
  and the interatction matrix.
- a section which precomputes the exponentials appearing in the the
  isotropic hardening of each slip systems
- a section which updates the implicit equation `feel` associated with
  elasticity and which defines the implicits equations `fg` associated
  with the plastic slips.

### Preamble

~~~~{.cpp}
@Integrator {
  using size_type = unsigned short;
  const auto &ss =
      MericCailletaudSingleCrystalViscoPlasticityNumericalJacobianSlipSystems<
          real>::getSlipSystems();
  const auto& m = ss.him;
~~~~

The first line defines a type alias: after this `size_type` is a short
hand for the `unsigned short` integer type used as the index type by
loops on the slip systems.

The second line retrieves the uniq instance of the
`MericCailletaudSingleCrystalViscoPlasticityNumericalJacobianSlipSystems`
class. The `const` keywords means that this variable is immutable. The
`auto` keyword means that the type of the `ss` variable is deduced by
the compiler. The ampersand character `&` means that `ss` is a
reference, i.e. the object returned by the `getSlipSystems` method is
not copied.

The  object referenced by the`ss` variable contains two data members:

- `ss.him` which contains the hardening interaction matrix defined by
  the `@InteractionMatrix` keyword.
- `ss.mus` which contains the symmetric parts of the orientations
  tensors defined by the `@SlidingSystem` keyword.

The last line defines `m` as an immutable reference the the interaction
matrix.

### Precomputation of the exponentials

In order to precompute the exponentials appearing in the isotropic
hardening of each slip systems, we first compute declare an array of
size `Nss` called `exp_bp`. We then loop over each slip systems.

~~~~{.cpp}
  // precomputing the exponentials used to compute the isotropic hardening of
  // each slip systems
  real exp_bp[Nss];
  for (size_type i = 0; i != Nss; ++i) {
      const auto p_ = p[i] + theta * abs(dg[i]);
      exp_bp[i] = exp(-b * p_);
  }
~~~~

### Definition of the implicit equations

Befor defining the implicit equations, it is important to remember a few
conventions of the `Implicit` DSL and of the `StandardElasticity` brick:

- The implicit equations are initialized by the `Implicit` dsl to the
  current estimate of the increment of the associated variable.
- The implicit equation `feel` associated with the elastic strain is
  initialized to \(\incr{\tepsilonel}-\incr{\tepsilonto}\) by the
  `StandardElasticity` brick.
- Before each evaluation of the implicit equations, i.e. before each
  call to the code declared in the `Integrator` block, the stress tensor
  is automatically computed by the `StandardElasticity` brick at
  \(t+\theta\,\incr{t}\) using the current estimate of the elastic
  increment and the result is stored in the `sig` variable.

Thus, the evaluation of the implicit equations are mostly a loop over
the slip systems which is meant to update `feel` and define the implicit
equation for the considered system.

This loop has three main parts:

1. Computing the yield stress by taking into account the contributions
  of the other slip systems (see Equation @eq:R).
2. Computing the back-strain increment and the back stress using
  Equations @eq:alpha and @eq:x.
3. Computing the plastic slip rate using the resolved shear stress (see
  Equation @eq:vp).

Finally, the implementation of the implicit equations closely mimics System @eq:F2.

~~~~{.cpp}
  // loop over the slip systems
  for (size_type i = 0; i != Nss; ++i) {
    // isostropic hardening
    auto r = tau0;
    for (size_type j = 0; j != Nss; ++j) {
      r += Q * m(i, j) * (1 - exp_bp[j]);
    }
    // back strain increment and kinematic hardening
    const auto da =  //
        (dg[i] - d * a[i] * abs(dg[i])) / (1 + theta * d * abs(dg[i]));
    const auto x = C * (a[i] + theta * da);
    // resolved shear stress
    const auto tau = sig | ss.mus[i];
    const auto f = max(abs(tau - x) - r, stress(0));
    const auto sgn = tau - x > 0 ? 1 : -1;
    fg[i] -= dt * pow(f / K, n) * sgn;
    feel += dg[i] * ss.mus[i];
  }
}
~~~~

## Auxiliary state variables update

Once the Newton-Raphson algorithm converged, one need to update the
auxiliary state variables as follows:

~~~~{.cpp}
@UpdateAuxiliaryStateVariables {
  using size_type = unsigned short;
  for (size_type i = 0; i != Nss; ++i) {
    p[i] += abs(dg[i]);
    const auto da =  //
        (dg[i] - d * a[i] * abs(dg[i])) / (1 + theta * d * abs(dg[i]));
    a[i] += da;
  }
}
~~~~

## Improving the robustness of the  Newton-Raphson algorithm

When the Norton exponent \(n\) is reasonnable, the standard
Newton-Raphson algorithm works pretty good. However, in practice, this
exponent is usually very high, typically between \(70\) and \(100\)
which almost imposes \(f=\left|\tau_e\right|-R_{i}-\tau_0\) to be equal
or lower to \(K\).

One simple trick to handle this condition is to reject Newton steps that
would lead to values of \(f\) too high. This can simply be done by
inserting the following test after the definition of `f`:

~~~~{.cpp}
    if (f > 1.1 * K) {
      return false;
    }
~~~~

# Implementation with an analytical jacobian

The implementation of the behaviour with an analytical jacobian is quite similar.

## Jacobian {#sec:J}

To solve System @eq:F2, one must compute its jacobian \(J\), which can be decomposed by blocks:

\[
J=
\begin{pmatrix}
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel} &
\dots&
\deriv{f_{\tepsilonel}}{\Delta\,\gamma_{i}} & 
\dots&
\deriv{f_{\tepsilonel}}{\Delta\,\gamma_{j}} &
\dots \\
\vdots&\vdots&\vdots&\vdots&\vdots&\vdots\\
\deriv{f_{\gamma_{i}}}{\Delta\,\tepsilonel} &
\dots&
\deriv{f_{\gamma_{i}}}{\Delta\,\gamma_{i}} &
\dots&
\deriv{f_{\gamma_{i}}}{\Delta\,\gamma_{j}} &
\dots \\
\vdots&\vdots&\vdots&\vdots&\vdots&\vdots\\
\deriv{f_{\gamma_{j}}}{\Delta\,\tepsilonel} &
\dots&
\deriv{f_{\gamma_{j}}}{\Delta\,\gamma_{i}} &
\dots&
\deriv{f_{\gamma_{j}}}{\Delta\,\gamma_{j}} &
\dots\\
\vdots&\vdots&\vdots&\vdots&\vdots&\vdots\\
\end{pmatrix}
\]

\(\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel}\) and
\(\deriv{f_{\tepsilonel}}{\Delta\,\gamma_{i}}\) are trivial:

\[
\left\{
\begin{aligned}
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel}&=\tenseurq{I}\\
\deriv{f_{\tepsilonel}}{\Delta\,\gamma_{i}}&=\tenseur{m}_{i}\\
\deriv{f_{\tepsilonel}}{\Delta\,\gamma_{j}}&=\tenseur{m}_{j}\\
\end{aligned}
\right.
\]

The other derivatives are tedious to compute, but not really difficult:
\[
\left\{
\begin{aligned}
\deriv{f_{\gamma_{i}}}{\Delta\,\tepsilonel}
&= -\Delta\,t\deriv{v}{f}\,\deriv{f}{\tau_e}\,\deriv{\tau_e}{\mts{\tau_{i}}}\,\deriv{\mts{\tau_{i}}}{\mts{\tsigma}}\,\colon\,\deriv{\mts{\tsigma}}{\Delta\,\tepsilonel}\,\sign{\tau_e}\\
&= -\Delta\,t\deriv{v}{f}\,\theta\,\tenseur{m}_{i}\,\colon\,\tenseurq{D}\\
\deriv{f_{\gamma_{i}}}{\Delta\,\gamma_{i}}
&= 1-\Delta\,t\deriv{v}{f}\,
\left[
\deriv{f}{\tau_e}\,\deriv{\tau_e}{\mts{x_{i}}}\,\deriv{\mts{x_{i}}}{\mts{\alpha_{i}}}\,\deriv{\mts{\alpha_{i}}}{\Delta\,\alpha_{i}}\,\deriv{\Delta\,\alpha_{i}}{\Delta\,\gamma_{i}}
+
\deriv{f}{R_{i}}\,\deriv{R_{i}}{\Delta\,\gamma_{i}}
\right]\,\sign{\tau_e}\\
&= 1+\Delta\,t\deriv{v}{f}\,
\left[
C\,\theta\,\deriv{\Delta\,\alpha_{i}}{\Delta\,\gamma_{i}}
+
\sign{\tau_e}\,\deriv{R_{i}}{\Delta\,\gamma_{i}}
\right]\\
\deriv{f_{\gamma_{i}}}{\Delta\,\gamma_{j}}
&= \Delta\,t\deriv{v}{f}\,\deriv{R_{i}}{\Delta\,\gamma_{j}}\,\sign{\tau_e}
\end{aligned}
\right.
\]

with:

- \(\deriv{v}{f}=
\left\{
\begin{aligned}
0 &\quad\text{if}\quad&f \leq 0&\\
n \, \Frac{v}{f}&\quad\text{if}\quad&f > 0&\\
\end{aligned}
\right.
\)
- \(\deriv{\Delta\,\alpha_{i}}{\Delta\,\gamma_{i}}=
\Frac{1-D\,\bts{\alpha_{i}}\,\sign{\Delta\,\gamma_{i}}}{\left(1+D\,\theta\,\sign{\Delta\,\gamma_{i}}\,\Delta\,\gamma_{i}\right)^2}
\)
- \(\deriv{R_{i}}{\Delta\,\gamma_{j}}=Q\,h_{ij}\,b\,\exp\paren{-b\,\mts{p_{j}}}\,\theta\,\mathop{sgn}\paren{\Delta\,\gamma_{j}}\)
- \(\deriv{R_{i}}{\Delta\,\gamma_{i}}=Q\,h_{ii}\,b\,\exp\paren{-b\,\mts{p_{i}}}\,\theta\,\mathop{sgn}\paren{\Delta\,\gamma_{i}}\)

## Modification of the previous implementation

From the previous file, one must:

- change the name of the behaviour.
- change the name of the classe containing the slip systems to reflect
  the change of name of the behaviour.
- change the name of the algorithm (or simply remove the associated line
  as the Newton-Raphson algorithm with analytical jacobian is the
  default algorithm) and remove the definition of the perturbation
  value.
- change the implementation of the `@Integrator` block to add the
  computation of the jacobian blocks.

Again, it is important to recall the convention of the `Implicit` DSL
concerning the initialization of the jacobian is to set it to identity.

The new `@Integrator` block is now:

~~~~{.cpp}
@Integrator {
  using size_type = unsigned short;
  constexpr const auto eeps = 1.e-12;
  const auto seps = eeps * D(0, 0);
  const auto& ss = //
      MericCailletaudSingleCrystalViscoPlasticitySlipSystems<real>::getSlipSystems();
  const auto& m = ss.him;
  real exp_bp[Nss];
  for (size_type i = 0; i != Nss; ++i) {
      const auto p_ = p[i] + theta * abs(dg[i]);
      exp_bp[i] = exp(-b * p_);
  }
  for (size_type i = 0; i != Nss; ++i) {
    const auto tau = sig | ss.mus[i];
    auto r = tau0;
    for (size_type j = 0; j != Nss; ++j) {
      r += Q * m(i, j) * (1 - exp_bp[j]);
    }
    const auto da =  //
        (dg[i] - d * a[i] * abs(dg[i])) / (1 + theta * d * abs(dg[i]));
    const auto x = C * (a[i] + theta * da);
    const auto f = max(abs(tau - x) - r, stress(0));
    if (f > 1.1 * K) {
      return false;
    }
    const auto sgn = tau - x > 0 ? 1 : -1;
    // elasticity
    feel += dg[i] * ss.mus[i];
    dfeel_ddg(i) = ss.mus[i];
    // viscoplasticity
    const auto v = pow(f / K, n);
    const auto dv = n * v / (max(f, seps));
    fg[i] -= dt * pow(f / K, n) * sgn;
    dfg_ddeel(i) = -dt * dv * (ss.mus[i] | D);
    const auto sgn_gi = dg(i) > 0 ? 1 : -1;
    const auto dda_ddg =
        (1 - d * a[i] * sgn_gi) / (power<2>(1 + theta * d * abs(dg[i])));
    dfg_ddg(i, i) += dt * dv * C * theta * dda_ddg;
    for (size_type j = 0; j != Nss; ++j) {
      const auto sgn_gj = dg(j) > 0 ? 1 : -1;
      const auto dr = Q * m(i, j) * theta * b * exp_bp[j] * sgn_gj;
      dfg_ddg(i, j) += dt * dv * dr * sgn;
    }
  }
}
~~~~

## Benchmarks

: Results of MericCailletaudSingleCrystalViscoPlasticity profiling : 8msecs 600musecs 753nsecs {#tbl:analytical_jacobian}

+:-----------------------------------------:+:--------------------------------------------:+
| Integrator                                | 8msecs 82musecs 148nsecs (8082148 ns)        |
+-------------------------------------------+----------------------------------------------+
| Integrator::ComputeThermodynamicForces    | 512musecs 422nsecs (512422 ns)               |
+-------------------------------------------+----------------------------------------------+
| Integrator::ComputeFdF                    | 2msecs 177musecs 565nsecs (2177565 ns)       |
+-------------------------------------------+----------------------------------------------+
| Integrator::TinyMatrixSolve               | 3msecs 846musecs 171nsecs (3846171 ns)       |
+-------------------------------------------+----------------------------------------------+
| ComputeFinalThermodynamicForces           | 25musecs 395nsecs (25395 ns)                 |
+-------------------------------------------+----------------------------------------------+
| ComputeTangentOperator                    | 334musecs 361nsecs (334361 ns)               |
+-------------------------------------------+----------------------------------------------+
| UpdateAuxiliaryStateVariables             | 23musecs 852nsecs (23852 ns)                 |
+-------------------------------------------+----------------------------------------------+

: Results of MericCailletaudSingleCrystalViscoPlasticityNumericalJacobian profiling : 69msecs 631musecs 952nsecs {#tbl:numerical_jacobian}

+:-----------------------------------------:+:--------------------------------------------:+
| Integrator                                | 69msecs 277musecs 664nsecs (69277664 ns)     |
+-------------------------------------------+----------------------------------------------+
| Integrator::ComputeThermodynamicForces    | 12msecs 83musecs 980nsecs (12083980 ns)      |
+-------------------------------------------+----------------------------------------------+
| Integrator::ComputeFdF                    | 30msecs 512musecs 471nsecs (30512471 ns)     |
+-------------------------------------------+----------------------------------------------+
| Integrator::TinyMatrixSolve               | 2msecs 538musecs 618nsecs (2538618 ns)       |
+-------------------------------------------+----------------------------------------------+
| ComputeFinalThermodynamicForces           | 16musecs 989nsecs (16989 ns)                 |
+-------------------------------------------+----------------------------------------------+
| ComputeTangentOperator                    | 225musecs 267nsecs (225267 ns)               |
+-------------------------------------------+----------------------------------------------+
| UpdateAuxiliaryStateVariables             | 15musecs 801nsecs (15801 ns)                 |
+-------------------------------------------+----------------------------------------------+

# Extenstion to finite strain

To extend the implementation to finite strain, we will use the
`FiniteStrainSingleCrystal` brick which is described
[here](finitestrainsinglecrystal.html).

In pratice, thanks to this brick, only a very limited number of changes
are required:

- The name of DSL must changed to `ImplicitFiniteStrainDSL`.
- The `StandardElasticity` brick must be changed in `FiniteStrainSingleCrystal`.
- The declaration of the the plastic slips must be removed, they are
  declared as state variables by the brick.
- The declaration of the variable `ss` must be removed as it is
  automatically declared by the brick.
- The definition of the resolved shear stress must be changed to:
  
  ~~~~{.cxx}
  const auto tau = M | ss.mu[i];
  ~~~~
  
  where `M` is the (unsymmetric) Mandel stress tensor and `ss.mu` are an
  array that contains the (unsymmetric) orientation tensors. `M` is
  automatically computed by the `FiniteStrainSingleCrystal` brick.
- The update of the implicit equation associated with the elastic strain must
  be removed. This is handled by the `FiniteStrainSingleCrystal` brick.
- The derivative of the implicit equations associated with the plastic slips
  with respect the the elastic strain must be changed as follows: 
  
  ~~~~{.cxx}
  dfg_ddeel(i) = -dt * dv * theta * (ss.mu[i] | dM_ddeel);
  ~~~~
  
  where `dM_ddeel` stands for the derivative of the Mandel stress with respect
  to the elastic strain. `dM_ddeel` is automatically computed by the brick.

# References
