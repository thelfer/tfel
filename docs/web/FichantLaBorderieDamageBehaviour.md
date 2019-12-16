% How to implement the Fichant-La Borderie damage behaviour
% Thomas Helfer, Alexandre Gagnant
% 12/2019

\newcommand{\absvalue}[1]{{\left|#1\right|}}
\newcommand{\ppos}[1]{{\left<#1\right>_{+}}}
\newcommand{\pneg}[1]{{\left<#1\right>_{-}}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\D}{\ets{\tenseurq{D}}}
\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tepsilontop}{\ppos{\tenseur{\varepsilon}^{\mathrm{to}}}}
\newcommand{\eeq}{\varepsilon^{\mathrm{to}}_{\mathrm{eq}}}
\newcommand{\tsigmae}{\underline{\sigma}^{\mathrm{eff}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\sigmaH}{\sigma_{H}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\derivtot}[2]{{\displaystyle \frac{\displaystyle \mathrm{d} #1}{\displaystyle \mathrm{d} #2}}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}

<!--
pandoc -f markdown-markdown_in_html_blocks+tex_math_single_backslash+grid_tables --filter pandoc-crossref  --mathjax --email-obfuscation=javascript --default-image-extension=svg FLB.md -o FLB.pdf
-->

This document describes how to implement the Fichant-La Borderie damage
behaviour.

The Fichant-La Borderie (FLB) model is an extension of the Mazars model,
which:

- simplifies the number of input parameters defining the tensile and
  compressive asymmetry.
- features an energy based regularisation method, thus allowing a
  fracture finite energy in a finite element implementation

![The Fichant La Borderie damage behaviour under uniaxial tension and compression](./img/FichantLaBorderieDamageBehaviourUniaxialTensileTest.png "")

The implementation is available [here](gallery/damage/FichantLaBorderieDamageBehaviour.mfront)

# Description of the Fichant-La Borderie damage law

## Equivalent strain

The equivalent strain \(\eeq\) is defined as:

\[
\eeq=\sqrt{\tepsilontop\,\colon\,\tepsilontop}=
\sqrt{\sum_{I=1}^{3}\ppos{\varepsilon^{\mathrm{to}}_{I}}^2}
\]

where \(\tepsilontop\) is the positive part of the total strain and
\(\left(\varepsilon^{\mathrm{to}}_{I}\right)_{i \in [1:3]}\) are its
eigenvalues.

> **Derivative of the equivalent strain**
>
> The following expression of the derivative of the equivalent
> strain with respect to the total strain will be useful:
>
> \[
> \derivtot{\eeq}{\tepsilonto}=\Frac{1}{\eeq}\,\tepsilontop\,\colon\,\derivtot{\tepsilontop}{\tepsilonto}
> \]{#eq:deeq}

## Damage evolution

The damage evolution is a function of the equivalent strain \(\eeq\):

- Under a given threshold \(\epsilon_{0}\), no damage occurs.
- Once this threshold is reached, the damage \(d\) is defined as
  follows:
\[
d=1-\Frac{\epsilon_{0}}{\eeq}\,\exp\paren{B_{t}\,\paren{\epsilon_{0}-\eeq}}
\]{#eq:d}

However, the latter expression must be modified to take into account the
irreversibility of the damage evolution. Let \(\bts{d}\) be the value of
the damage at the beginning of the time step and \(\ets{d}\) its value
at the end of the time step, \(\ets{d}\) is determined as follows:

\[
\ets{d}=\max\paren{\bts{d},1-\Frac{\epsilon_{0}}{\ets{\eeq}}\,\exp\paren{B_{t}\,\paren{\epsilon_{0}-\ets{\eeq}}}}
\]

where \(\ets{\eeq}\) is the value of the equivalent strain at the end of
the time step.

> **Derivatives of the damage**
>
> In case of a growing damage, the derivative of the damage with respect to the 
> equivalent plastic strain is:
> 
> \[
> \derivtot{\ets{d}}{\ets{\eeq}}=\Frac{\epsilon_{0}}{\ets{\eeq}}\exp\paren{B_{t}\,\paren{\epsilon_{0}-\ets{\eeq}}}\left(B_{t}+\Frac{1}{\ets{\eeq}}\right)
> \]{#eq:dd}
>
> Combined with Equation @eq:deeq, Equation @eq:dd allows computing the
> derivative of the damage with respect to the total strain.

## Computation of the stress

The effective stress tensor \(\tsigmae\) is computed using the standard Hooke law:
\[
\ets{\tsigmae}=\D\,\colon\,\ets{\tepsilonto}
\]

The behaviour is assumed isotropic, so that the stiffness tensor \(\D\)
can be related to the first and second Lamé coefficients, denoted
respectively \(\lambda\) and \(\mu\), as follows:

\[
\D=\ets{\lambda}\,\tenseur{I}\,\otimes\,\tenseur{I}+2\,\ets{\mu}\,\tenseurq{I}
\]

> **Note**
>
> The restriction to isotropy has an important pratical consequence:
> \(\tsigmae\) and \(\tepsilonto\) have the same egein basis.

The stress tensor \(\tsigma\) is then computed as follows:
\[
\ets{\tsigma}=\paren{1-\ets{d}}\,\ppos{\ets{\tsigmae}}+\paren{1-\ets{d}^{a}}\,\pneg{\ets{\tsigmae}}
\]

> **Derivative of the stress with respect to the total strain**
> 
> In case of damage growth, the derivative of the stress with respect to
> the total strain is given by:
> \[
> \begin{aligned}
> \derivtot{\ets{\tsigma}}{\ets{\tepsilonto}}&=
> \left[\paren{1-\ets{d}}\,    \derivtot{\ppos{\ets{\tsigmae}}}{\ets{\tsigmae}}+
>       \paren{1-\ets{d}^{a}}\,\derivtot{\pneg{\ets{\tsigmae}}}{\ets{\tsigmae}}\right]\,\colon\,\D \\
> &- \ppos{\ets{\tsigmae}}\,\otimes\,\derivtot{\ets{d}}{\ets{\tepsilonto}}
>  - a\,\ets{d}^{a-1}\,\pneg{\ets{\tsigmae}}\,\otimes\,\derivtot{\ets{d}}{\ets{\tepsilonto}}
> \end{aligned}
> \]{#eq:Dt}
>
> This expression gives the so-called consistent tangent operator.

# Implementation

## Choice of the domain specific language

The Fichant-La Borderie damage behaviour does not require any specific
integration scheme as the damage evolution is an explicit function of
the total strain. The `Default` domain specific language is suitable for
treating that kind of behaviours.

~~~~{.cxx}
@DSL Default;
~~~~

## Name of the behaviour

The `@Behaviour` keyword defines the name of the behaviour:

~~~~{.cxx}
@Behaviour FichantLaBorderieDamageBehaviour;
~~~~

## Metadata

The implementation starts by defining somme metadata associated with the
behaviour:

- the date.
- the names of the authors.
- a short description.

~~~~{.cxx}
@Date 7/12/2019;
@Author A. Gangnant, T. Helfer;
@Description{
  "Implementation of the Fichant-La Borderie damage behaviour"
}
~~~~

## Material properties

### Elastic material properties

Most solvers uses the Young modulus and the Poisson ratio to
characterize the elasticity of an isotropic material. Here we adopt this
convention by defining them as two material properties named `young` and
`nu` and associate those variables with the appropriate glossary names:

~~~~{.cxx}
@MaterialProperty stress young;
young.setGlossaryName("YoungModulus");
@MaterialProperty real nu;
nu.setGlossaryName("PoissonRatio");
~~~~

> **About glossary names**
>
> Associating a glossary name is a way to define the so-called external
> name of this variable, i.e. the name that will appear on the calling
> solver side. It also allows `MFront` to make assumptions about
> the real meaning of those variables.
> 
> The case of the `Cast3M` solver is a noteworthy example. This solver
> requires \(4\) elastic properties to be defined for its own use in
> the case of an isotropic material properties: the Young modulus, the
>  Poisson ratio, the density and the thermal expansion coefficient.
> Normally, the `MFront` material properties are appended to those
> four properties. However, by associating the `YoungModulus` and the
> `PoissonRatio` glossary names to the `young` and `nu` material
> properties respectively, `MFront` will identify them with the
> ones required by `Cast3M`. 
>
> For details about the glossary names, the reader may refer to the
> [following page](glossary.html).

### Material properties associated with damage

The damage evolution given by Equation @eq:d requires three material
properties \(Bt\), \(e_{0}\) and \(a\).

~~~~{.cxx}
@MaterialProperty real Bt;
@MaterialProperty strain e0;
@MaterialProperty real a;
~~~~

## State variable

The only state variable of this behaviour is the damage variable \(d\)
which we declare as follows:

~~~~{.cxx}
@StateVariable real d;
~~~~

For post-processing purposes, we associate this variable with the
`Damage` glossary name.

~~~~{.cxx}
d.setGlossaryName("Damage");
~~~~

## Local variables

In `MFront`, a local variable are variables in each code blocks. They
are usually evaluated once for all in the `@InitLocalVariables` code
block which is called before the behaviour integration (as defined by
the `@Integrator` code block) or before the computation of the
prediction operator (as defined in the `@PredictionOperator` code
block).

In the following, we will use two local variables `lambda` and `mu`
associated respectively with the first and second Lamé coefficients.

~~~~{.cxx}
@LocalVariable stress lambda, mu;
~~~~

## Initialization of the local variables

The initialization of the local variables is performed in the
`@InitLocalVariables` code block, as follows:

~~~~{.cxx}
@InitLocalVariables {
  lambda = computeLambda(young, nu);
  mu = computeMu(young, nu);
}
~~~~

Here, we use two functions provided by the `TFEL/Material` library:
`computeLambda` and `computeMu`.

## Prediction operator

The prediction operator is used by the `Code_Aster` finite element
solver at the beginning of each new time step. It is also used by the
`Abaqus/Explicit` solver to retrieve the elastic properties at the
packaging step.

~~~~{.cxx}
@PredictionOperator {
  static_cast<void>(smt);
  Dt = lambda * Stensor4::IxI() + 2 * mu * Stensor4::Id();
}
~~~~

Here, we only provide the elastic prediction operator. Thus, we
discarded the `smt` variable by casting it to `void` to avoid a compiler
warning.

## Behaviour integration

The behaviour integration is performed in the `@Integrator` code block
which is meant to:

- update the damage state variable.
- compute the stress at the end of the time step.
- compute the consistent tangent operator if required.

> **Note**
>
> The computation of the consistent tangent operator is usually
> performed in the `@TangentOperator` code block. However, in the
> case of the Fichant-La Borderie behaviour, this would lead to
> an unneccessary complexity as a lot of local variables would
> have been required to store the results of complex computations
> required in both the `@Integrator` and the `@TangentOperator`
> code blocks.
>
> To state that the consistent tangent operator is indeed
> computed in the `@Integrator` code block, one shall use
> the `@ProvidesTangentOperator` keyword. Note that this
> keyword explicitly states that the consistent tangent operator
> is *not* symmetric. See the `@ProvidesSymmetricTangentOperator`
> keyword to state that the consistent tangent operator
> is symmetric.

~~~~{.cxx}
@ProvidesTangentOperator;
@Integrator{
  // a simple alias for the identity
  constexpr const auto id = Stensor::Id();
~~~~

At the beginning of this block, we define two constant used in the
following to decide if two eigenvalues are equal:

- The first constant `eeps` is used when dealing a tensor of the strain
  type.
- The second constant `seps` is used when dealing a tensor of the stress
  type.

~~~~{.cxx}
  constexpr const strain eeps = 1.e-12;
  const auto seps = eeps * young;
~~~~

Then we define two local functions called `pp` and
`square_pp`:

- `pp` computes the positive part of a scalar.
- `square_pp` computes the square of the positive part of a scalar.

~~~~{.cxx}
  // positive part
  const auto pp = [](const real x) { return x > 0 ? x : 0; };
  // square of the posititve part
  auto square_pp = [](const strain v) { return v > 0 ? v * v : 0; };
~~~~

> **Note**
> 
> In `C++`, those local functions are called `lambda` functions.

We now compute the total strain and stores it in a variable called `e`:

~~~~{.cxx}
  // total strain
  const auto e = eval(eto + deto);
~~~~

where `eto` and `deto` are respectively the total strain at the
beginning of the time step and its increment over the time step.

Tensorial operations in the `TFEL/Math` library are lazy, which means
that their evaluation is delayed until it is really needed. This
technique is called `expression template` in `C++` and is based on the
definition of intermediate objects which hold the operation to be
performed.

The `eval` function is used to force the evaluation of this operation.
This is required for the computation of the eigenvalues and the
eigenvectors which is done by calling the `computeEigenVectors` tensor
of the `stensor` class.

In `C++-17`, this can be called as follows:

~~~~{.cxx}
  // eigen values and eigen tensors of the total strain
  const auto [e_vp, m] = 
    e.template computeEigenVectors<Stensor::FSESJACOBIEIGENSOLVER>();
~~~~

In previous `C++` version, one must use a more verbose code:

~~~~{.cxx}
  // eigen values and eigen tensors of the total strain
  auto e_vp = tvector<3u, strain>{};
  auto m = tmatrix<3u, 3u, strain>{};
  e.template computeEigenVectors<Stensor::FSESJACOBIEIGENSOLVER>(e_vp, m);
~~~~

Here, we must make some important comments:

- We use the Jacobi eigen solver which is very accurate but is more
  expensive than the default eigen solver.
- The computation of the equivalent strain only requires the eigenvalues
  of the total strain. We also compute its eigen vectors because they
  also are the eigen vectors of the effective stress and its positive
  part.

The computation of the equivalent strain is then straightforward:

~~~~{.cxx}
  const auto e_eq = sqrt(square_pp(e_vp[0]) + //
                         square_pp(e_vp[1]) + //
                         square_pp(e_vp[2]));
~~~~

We are now able to compute the damage at the end of the time step as
follows:

~~~~{.cxx}
  // update the damage, taking irreversibility into account
  const auto Cd = (e0 / e_eq) * exp(Bt * (e0 - e_eq));
  const auto d_p = (e_eq > e0) ? 1 - Cd : 0;
  const auto bp = d_p > d;
  if (bp) {
    d = d_p;
  }
~~~~

The previous lines also defines two additional variables:

- `Cd` which is used to compute the expression
  \(\Frac{\epsilon_{0}}{\ets{\eeq}}\exp\paren{B_{t}\,\paren{\epsilon_{0}-\ets{\eeq}}}\)
  only once.
- `bp` which is a boolean stating if the damage increases over the time
  step. This boolean will be used in the computation of the consistent
  tangent operator.

The next two lines computes the effective stress:

~~~~{.cxx}
  // effective stress at the end of the time step
  const auto l_tr_e = lambda * trace(e);
  const auto s = eval(l_tr_e * id + 2 * mu * e);
~~~~

The next lines computes the positive part of the effective stress:

~~~~{.cxx}
  // positive part of the effective stress
  const auto s_vp = tvector<3u, real>{l_tr_e + 2 * mu * e_vp[0],  //
                                      l_tr_e + 2 * mu * e_vp[1],  //
                                      l_tr_e + 2 * mu * e_vp[2]};
  const auto sp = StressStensor::computeIsotropicFunction(pp, s_vp, m);
~~~~

Those lines uses the `computeIsotropicFunction` which takes the function
computing the positive part, the eigenvalues of the effective stress and
its eigen vectors.

Finally, the final stress is computed:

~~~~{.cxx}
  // function of the damage to simplify expressions
  const auto d_a = pow(d, a);
  const auto fpd = (1 - d);
  const auto fpn = (1 - d_a);
  // final stress
  sig = (fpd - fpn) * sp + fpn * s;
~~~~

#### Computation of the tangent operator

If one computes the tangent operator in the `@Integrator` code block,
one shall check if this operator was requested using the
`computeTangentOperator_` boolean value.

~~~~{.cxx}
  if (computeTangentOperator_) {
~~~~

Next, we define a local function called `dpp` which computes the
derivative of the positive part:

~~~~{.cxx}
    // derivative of the positive part
    const auto dpp = [&seps](const stress x) {
      return std::abs(x) < seps ? 0.5 : ((x < 0) ? 0 : 1);
    };
~~~~

Various kind of tangent operator can be computed. To see which one was
requested by the calling solver, one must check the value of the `smt`
variable (`smt` stands "stiffness matrix type"):

The first case corresponds to the `ELASTIC` value:

~~~~{.cxx}
    if (smt == ELASTIC) {
      Dt = lambda * Stensor4::IxI() + 2 * mu * Stensor4::Id();
~~~~

The second case corresponds to the `SECANTOPERATOR` value. This case
also corresponds to the consistent tangent operator was no damage
increase occurs (i.e. the `bp` variable' value is `false`):

~~~~{.cxx}
    } else if ((smt == SECANTOPERATOR) || (!bp)) {
      // elastic stiffness
      const auto De = lambda * Stensor4::IxI() + 2 * mu * Stensor4::Id();
      // derivative of the positive part of the stress
      const auto dsp =
          StressStensor::computeIsotropicFunctionDerivative(pp, dpp, s_vp, m, seps * 0.1);
      Dt = ((fpd * dsp + fpn * (Stensor4::Id() - dsp)) + eeps * Stensor4::Id()) * De;
~~~~

This code uses the `computeIsotropicFunctionDerivative` which computes
\(\deriv{\ppos{\tsigma}}{\tsigma}\). We also added the `eeps *
Stensor4::Id()` to avoid singular stiffness matrices and add some
coercivity to the global problem.

The last case corresponds to the `CONSISTENTANGENTOPERATOR` value in
case of damage increase

~~~~{.cxx}
    } else {
      // elastic stiffness
      const auto De = lambda * Stensor4::IxI() + 2 * mu * Stensor4::Id();
      // derivative of the positive part of the stress
      const auto dsp =
          StressStensor::computeIsotropicFunctionDerivative(pp, dpp, s_vp, m, seps * 0.1);
      // derivative with respect to the damage
      const auto dfpd_dd = -1;
      const auto id = 1 / max(eeps, d);
      const auto dfpn_dd = -a * d_a * id;
      // positive part of the total strain
      const auto ep = StrainStensor::computeIsotropicFunction(pp, e_vp, m);
      // derivative of the damage
      const auto dd_deq = Cd * (Bt + 1 / e_eq);
      const auto dep_de =
          StrainStensor::computeIsotropicFunctionDerivative(pp, dpp, e_vp, m, eeps * 0.1);
      const auto deq_dep = ep / e_eq;
      const auto dd_de = dd_deq * deq_dep * dep_de;
      Dt = ((fpd * dsp + fpn * (Stensor4::Id() - dsp)) + eeps * Stensor4::Id()) * De  //
           + (((dfpd_dd - dfpn_dd) * sp + dfpn_dd * s) ^ dd_de);
    }
  }
}
~~~~


<!--
\[
\
\]
  const auto e_vp = e.computeEigenValues();
  const auto e_eq = sqrt(square_pp(e_vp[0]) + square_pp(e_vp[1]) + square_pp(e_vp[2]));
  // effective stress at t+theta*dt
  const auto de = (e_eq > e0) ? max(d, 1 - (e0 / e_eq) * exp(Bt * (e0 - e_eq))) : d;
  // function of the damage to simplify expressions
  const auto fpd = (1 - de);
  const auto fpn = (1 - pow(de, a));
  // effective stress at the end of the time step
  const auto s = eval(lambda * trace(e) * id + 2 * mu * e);
  // positive part
  const auto pp = [](const real x) { return x > 0 ? x : 0; };
  // derivative of the positive part
  const auto dpp = [&seps](const real x) { return std::abs(x) < seps ? 0.5 : ((x < 0) ? 0 : 1); };
  const auto sp_dsp =
      s.template computeIsotropicFunctionAndDerivative<stensor_common::FSESANALYTICALEIGENSOLVER>(
          pp, dpp, seps * 0.1);
  const auto& sp = sp_dsp.first;
  const auto& dsp = sp_dsp.second;  // derivative of sp with respect to s
  const auto sn = s - sp;
  // final stress
  sig = fpd * sp + fpn * sn;
-->