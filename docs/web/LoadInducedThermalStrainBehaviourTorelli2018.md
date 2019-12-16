% How to implement Torelli 2018' load induced thermal strain behaviour
% Thomas Helfer, Giacomo Torelli
% 08/12/2019

\newcommand{\absvalue}[1]{{\left|#1\right|}}
\newcommand{\ppos}[1]{{\left<#1\right>_{+}}}
\newcommand{\pneg}[1]{{\left<#1\right>_{-}}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\D}{\ets{\tenseurq{D}}}
\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\tenseur{\varepsilon}^{\mathrm{el}}}
\newcommand{\lits}{\mathrm{LITS}}
\newcommand{\telits}{\tenseur{\varepsilon}^{\lits}}
\newcommand{\tdelits}{\tenseur{\dot{\varepsilon}}^{\lits}}

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

This page shows how to implement a behaviour describing the load induced
thermal strain (LITS) phenomena in concrete following the work of
@torelli_confinement-dependent_2018.

This implementation is available [here](gallery/viscoplasticity/LoadInducedThermalStrain_Torelli2018.mfront).

# Description

The behaviour is described by a standard split of the strain
\(\tepsilonto\) in an elastic and a plastic parts, respectively
denoted \(\tepsilonel\) and \(\telits\):

\[
\tepsilonto=\tepsilonel+\telits
\]{#eq:eel}

## Elastic behaviour

The stress \(\tsigma\) is related to the the elastic strain
\(\tepsilonel\) by a the standard Hooke behaviour:

\[
\tsigma = \lambda\,\trace{\tepsilonel}\,\tenseur{I}+2\,\mu\,\tepsilonel
\]{#eq:Hooke}

## Load induced thermal strain

The evolution of the load induced thermal strain \(\telits\) is given by:

\[
\tdelits=
\Frac{\eta\paren{\tsigma}\,\beta\paren{T}}{\sigma_{u0}}
\left[
  -\nu_{\lits}\trace{\pneg{\tsigma}}\,\tenseur{I}
  +\paren{1-\nu_{\lits}}\,\pneg{\tsigma}
\right]\,\dot{T}_{e}
\]{#eq:delits}

where:

- \(\pneg{\sigma}\) is the negative part of the stress tensor
  \(\tsigma\). This negative part is computed as follows:
  \[
    \pneg{\sigma} = \sum_{I=1}^{3}\pneg{\sigma_{I}}\,\tenseur{n}_{I}
  \]
  where \(\sigma_{I}\) is the Ith eigenvalue of \(\tsigma\) and 
  \(\tenseur{n}_{I}\) the associated eigen tensor.
- \(\eta\paren{\tsigma} = 1+C_{m}\paren{\tsigma}\,\gamma\). \(\eta\)
  describes the effect of the stress triaxiality.
- \(C_{m}\paren{\tsigma}=\Frac{-\trace{\pneg{\tsigma}}}{\sqrt{\pneg{\tsigma}\,\colon\,\pneg{\tsigma}}}\).
  \(C_{m}\) is the triaxiality index linked to the principal stresses.
- \(\sigma_{u0}\), \(\nu_{\lits}\) and \(\gamma\) are material
  parameters. More precisely \(\sigma_{u0}\) is the compressive strength
  of the material and \(\gamma\) is a material parameter calibrated to
  experimental data for the appropriate temperature range.
- \(\beta\paren{T}\) is a four order polynomial of the temperature
  describing uniaxial temperature-LITS behaviour observed in the
  literature:
  \[
    \beta\paren{T}=\beta_{0}+\beta_{1}\,T+\beta_{2}\,T^{2}+\beta_{3}\,T^{3}+\beta_{4}\,T^{4}
  \]

\(\dot{T}_{e}\) is the effective temperature rate defined as follows:

\[
\dot{T}_{e}=
\left\{
\begin{aligned}
\dot{T} &\quad\text{if}\quad T=T_{\mathrm{max}}\\
0 &\quad\text{if}\quad T<T_{\mathrm{max}}\\
\end{aligned}
\right.
\]

where \(T_{\mathrm{max}}\) is the maximal temperature seen by the
material over time.

The negative part of the stress \(\pneg{\sigma}\) is an isotropic
function of the stress. `TFEL/Math`, the mathematical library behind
`MFront`, offers dedicated functions to compute its value and its
derivative \(\deriv{\pneg{\sigma}}{\tsigma}\).

# Implicit scheme

Equation @eq:delits can be discretized using a standard implicit scheme
as follows:

\[
\Delta\,\telits=C\,\eta\paren{\mts{\tsigma}}\,\tenseur{s}
\]{#eq:felits}

where:

- \(\theta\) is a numerical parameter (\(\theta \in \left[0:1\right]\).
- \(\mts{\tsigma}\) is evaluated by Equation @eq:Hooke:
  \[
    \mts{\tsigma}=\mts{\lambda}\,\trace{\mts{\tepsilonel}}+
                  2\,\mts{\mu}\,\mts{\tepsilonel}+
  \]
- \(\mts{\tepsilonel}=\bts{\tepsilonel}+\theta\,\Delta\,\tepsilonel\)
- \(\mts{T}=\bts{T}+\theta\,\Delta\,T\)
- \(C=\Frac{\beta\paren{\mts{T}}\,\Delta\,T_{e}}{\sigma_{u0}}\)
- \(\tenseur{s}= -\nu_{\lits}\trace{\pneg{\mts{\tsigma}}}\,\tenseur{I}+\paren{1-\nu_{\lits}}\,\pneg{\mts{\tsigma}}\)

In the previous expressions, \(\Delta\,T_{e}\) is the effective
temperature increment.

Equation @eq:felits shows that \(\Delta\,\telits\) is known if
\(\Delta\,\tepsilonel\) is known.

Using Equation @eq:eel in its increment form thus leads to the following
non linear equation whose only unknows is \(\Delta\,\tepsilonel\):

\[
f_{\tepsilonel}=\Delta\,\tepsilonel+\Delta\,\telits\paren{\Delta\,\tepsilonel}-\Delta\,\tepsilonto=\tenseur{0}
\]{#eq:feel}

This non linear equation will be solved by a standard Newton-Raphson
algorithm, which requires the jacobian
\(\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel}\):

\(\deriv{\eta}{\Delta\,\tepsilonel}\) is computed as follows:

\[
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel}
=\tenseur{I}+\deriv{\Delta\,\telits}{\Delta\,\tepsilonel}
=\tenseur{I}+
C\,\left[
\tenseur{s}\,\otimes\,\deriv{\eta}{\Delta\,\tepsilonel}+
\eta\paren{\mts{\tsigma}}\,\deriv{s}{\Delta\,\tepsilonel}
\right]
\]

\[
\begin{aligned}
\deriv{\eta}{\Delta\,\tepsilonel}
&=\gamma\,\deriv{C_{m}}{\mts{\tsigma}}\,\deriv{\mts{\tsigma}}{\Delta\,\tepsilonel}\\
&=\theta\,\gamma\,\deriv{C_{m}}{\mts{\tsigma}}\,\colon\,\mts{\tenseurq{D}}\\
&=\theta\,\gamma\,\deriv{C_{m}}{\pneg{\mts{\tsigma}}}\,\colon\,\deriv{\pneg{\mts{\tsigma}}}{\mts{\tsigma}}\,\colon\,\mts{\tenseurq{D}}\\
\end{aligned}
\]{#eq:deta_dsig}

where \(\mts{\tenseurq{D}}\) is the elastic stiffness tensor at the middle of the time step:

\[
\mts{\tenseurq{D}}=\mts{\lambda}\,\tenseur{I}\,\otimes\,\tenseur{I}+2\,\mu\,\tenseurq{I}
\]

> **Note**
>
> The explicit expression of
> \(\deriv{\pneg{\mts{\tsigma}}}{\mts{\tsigma}}\) is not given here
> as it will be computed by an build-in function of the `TFEL/Math`
> library.

\(\deriv{C_{m}}{\pneg{\mts{\tsigma}}}\) is computed as follows:
\[
\begin{aligned}
\deriv{C_{m}}{\pneg{\mts{\tsigma}}}
&=\deriv{}{\pneg{\mts{\tsigma}}}\paren{\Frac{-\trace{\pneg{\mts{\tsigma}}}}{\sqrt{\pneg{\mts{\tsigma}}\,\colon\,\pneg{\mts{\tsigma}}}}}\\
&=\Frac{-\tenseur{I}}{\sqrt{\pneg{\mts{\tsigma}}\,\colon\,\pneg{\mts{\tsigma}}}}+\trace{\pneg{\mts{\tsigma}}}\,\deriv{}{\pneg{\mts{\tsigma}}}\paren{\Frac{-1}{\sqrt{\pneg{\mts{\tsigma}}\,\colon\,\pneg{\mts{\tsigma}}}}}\\
&=\Frac{-\tenseur{I}}{\sqrt{\pneg{\mts{\tsigma}}\,\colon\,\pneg{\mts{\tsigma}}}}\,\tenseur{I}+\trace{\pneg{\mts{\tsigma}}}\,\Frac{\pneg{\mts{\tsigma}}}{\paren{\pneg{\mts{\tsigma}}\,\colon\,\pneg{\mts{\tsigma}}}^{3/2}}\,
\end{aligned}
\]

Finally, \(\deriv{s}{\Delta\,\tepsilonel}\) is given by:

\[
\begin{aligned}
\deriv{s}{\Delta\,\tepsilonel}&=
\deriv{s}{\pneg{\mts{\tsigma}}}\,\colon\,\deriv{\pneg{\mts{\tsigma}}}{\mts{\tsigma}}\,\colon\,\deriv{\mts{\tsigma}}{\Delta\,\tepsilonel}\\
&\theta\,\deriv{s}{\pneg{\mts{\tsigma}}}\,\colon\,\deriv{\pneg{\mts{\tsigma}}}{\mts{\tsigma}}\,\colon\,\tenseurq{D}\\
\end{aligned}
\]

where:

\[
\deriv{s}{\pneg{\mts{\tsigma}}}=
-\nu_{\lits}\,\tenseur{I}\,\otimes\,\tenseur{I}+\paren{1-\nu_{\lits}}\,\tenseurq{I}\\
\]

# Implementation

## Choice of the domain specific language

The domain specific language suitable for implementing an implicit
scheme is called `Implicit`. The `@DSL` keyword allows specifying the
domain specific language to be used:

~~~~{.cxx}
@DSL Implicit;
~~~~

## Choice of the behaviour name

The `@Behaviour` allows giving a name the behaviour:

~~~~{.cxx}
@Behaviour LoadInducedThermalStrain_Torelli2018;
~~~~

Defining the name of behaviour is required.

## Metadata

The `@Author`, `@Date` and `@Description` keywords allows adding
metadata to the behaviour (respectively the name of the author of the
implementation, the date at which the implementation was written, a
short description), as follows:

~~~~{.cxx}
@Author Giacomo Torelli;
@Date 25 / 03 / 2018;
@Description {
  "A confinement-dependent load-induced thermal strain "
  "constitutive model for concrete subjected "
  "to temperatures up to 500 °C"

  "Torelli, Giacomo and Mandal, Parthasarathi and Gillie, "
  "Martin and Tran, Van-Xuan"

  "International Journal of Mechanical Sciences"

  "http://www.sciencedirect.com/science/article/pii/S0020740317337372"
}
~~~~

Those metadata, though recommended, are optional.

## Use of the `StandardElasticity` brick

The behaviour treated in this document allows using the
`StandardElasticity` brick. This brick, described in [this
page](BehaviourBricks.html), provides:

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

~~~~{.cxx}
@Brick StandardElasticity;
~~~~

As the `StandardElasticity` brick provides support for all modelling
hypotheses, we can declare that your behaviour indeed supports all
modelling hypotheses:

~~~~{.cxx}
@ModellingHypotheses{".+"};
~~~~

## Choice of the numerical parameter

The implicit scheme requires three numerical parameters:

- the \(\theta\) parameter (default value is \(1/2\)).
- the maximum number of iterations (default value is \(100\)).
- the stopping criteria (default value is \(10^{-8}\)). This may be
  sufficient in most cases, but not for the computation of the
  consistent tangent operator by the `StandardElasticity` brick.

~~~~{.cxx}
@Theta 0.5;
@IterMax 100;
@Epsilon 1.e-16;
~~~~

> **Note**
> 
> Those values are automatically generates parameters that can be
> changed at runtime: the names of those parameters are respectively
> `theta`, `iterMax` and `epsilon`.

## Thermoelastic properties, computation of the thermal strain

The `@ComputeStiffnessTensor` keyword allows computing the elastic
stiffness tensor in a variable called `D`. In this case, the `UnAltered`
version of the stiffness tensor is required ("unaltered" means that the
modelling hypothesis was not taken into account).

~~~~{.cxx}
@ComputeStiffnessTensor<UnAltered>{47000, 0.25};
~~~~

The `@ComputeThermalExpansion` keyword allows computing the thermal
strain. It is followed by the mean thermal expansion coefficient.

~~~~{.cxx}
@ComputeThermalExpansion 10e-6;
~~~~

The thermal strain is automatically removed to the total strain and the
total strain increment.

## Parameters

The following code declares the various parameters used by the
constitutive equations:

~~~~{.cxx}
@Parameter gamma = 1.5;
@Parameter sigmultimate = 50.;
@Parameter nulits = 0.50;
@Parameter tcrit = 0.;
@Parameter b[5] = {2.7031065533E-05, -1.0209170592E-06, 6.1200423753E-9, //
                   -1.2632648735E-11, 6.9158539621E-15};
~~~~

## Auxiliary state variables

By default, the `Implicit` `DSL` declares the elastic strain as a state
variable, i.e. a variable which is part of the implicit system and whose
value is saved from one time step to the other.

We choose to declare two auxiliary state variables, the load induced
thermal strain \(\elits\) and the maximum value of the temperature seen
by the material \(T_{\mathrm{max}}\). Those variables are not part of
the implicit system but their values are saved from one time step to the
other.

Saving maximum value of the temperature seen by the material is
required, but its evolution is independent of the mechanics.

Saving the load induced thermal strain is only useful for
post-processing. It could be removed for saving some space in memory.

Auxiliary state variables are introduced by the
`@AuxiliaryStateVariable` keyword, as follows:

~~~~{.cxx}
@AuxiliaryStateVariable StrainStensor elits;
elits.setEntryName("LoadInducedThermaStrain");
@AuxiliaryStateVariable temperature Tmax;
Tmax.setEntryName("MaximalValueOfTheTemperature");
~~~~

## Local Variables

Local variables are available in every code blocks. Here we will
introduce two local variables:

- the increment of the load induced thermal strain, that will be used to
  update the load induced thermal strain at the end of the time step
- the creep coefficient which allows computing the thermal dependency
  before the implicit resolution.

~~~~{.cxx}
//! LITS increment
@LocalVariable StrainStensor delits;
//! Creep coefficient
@LocalVariable real C;
~~~~

## Initialization of the local variables

The `@InitLocalVariables` code block is called before the implicit
resolution. It is used here to compute the `C` variable and update the
`Tmax` auxiliary variable.

~~~~{.cxx}
@InitLocalVariables {
  Tmax = max(max(tcrit, Tmax), T);
  const auto T_ = T + theta * dT;
  const auto beta = b[0] + T_ * (b[1] + T_ * (b[2] + T_ * (b[3] + T_ * b[4])));
  const auto dTe = max(T + dT - max(tcrit, Tmax), temperature(0));
  C = (beta / (-sigmultimate)) * dTe;
}
~~~~

## Implicit system

The implicit system is defined in the code block introduced by the
`@Integrator` code block.

This code block is called at each iteration of the implicit system,
after the stress \(\mts{\tsigma}\) at the middle of the time step has
been computed and stored in the `sig` variable (this is handled by the
`StandardElasticity` brick).

~~~~{.cxx}
@Integrator {
~~~~

We first define some useful constants.

~~~~{.cxx}
  constexpr const stress eeps = 1.e-12;
  const stress seps = eeps * young;
  constexpr const auto id = Stensor::Id();
  constexpr const auto esolver = StressStensor::FSESJACOBIEIGENSOLVER;
~~~~

Then the negative part of the stress \(\pneg{\mts{\tsigma}}\) and its
derivative with respect to the stress
\(\deriv{\pneg{\mts{\tsigma}}}{\mts{\tsigma}}\) are computed using the
`computeIsotropicFunctionAndDerivative` method of the `stensor` class.

> **Notes**
> 
> A scalar function \(f\) defines an isotropic function of a symmetric tensor
> \(\tenseur{a}\) as follows:
> 
> \[
>     f{\tenseur{a}} = \sum_{I=1}^{3}\pneg{a_{I}}\,\tenseur{n}_{I}
> \]
>
> where \(a_{I}\) are the eigenvalues of \(\tenseur{a}\) and
> \(\tenseur{n}_{I}\) its eigentensors.

This function requires two scalar functions defining the isotropic
function and its derivative. Those functions are called `np` and `dnp`
here:

~~~~{.cxx}
  // negative part
  const auto np = [](const stress x) -> stress { return x <= 0 ? x : 0; };
  const auto dnp = [&seps](const stress x) -> real {
    return std::abs(x) < seps ? 0.5 : ((x < 0) ? 1 : 0);
  };
~~~~

Then we call the `computeIsotropicFunctionAndDerivative` method:

~~~~{.cxx}
  auto sn = StressStensor{};
  auto dsn = Stensor4{};
  std::tie(sn, dsn) =
      sig.template computeIsotropicFunctionAndDerivative<esolver>(np, dnp,
                                                                  seps * 0.1);
~~~~

The previous expression can be simplified in `C++-17`:

~~~~{.cxx}
  const auto [sn,dsn] = 
      sig.template computeIsotropicFunctionAndDerivative<esolver>(np, dnp,
                                                                  seps * 0.1);
~~~~

> **About the choice of the eigensolver**
>
> Here we used a the Jacobi algorithm as implemented in @kopp_efficient_2008.
> This algorithm is lower but more accurate than the default one
> (see [here](tensors.html) for an indepth comparison of available algorithms.

The next step is to compute \(\mts{\eta}\) and its derivative
\(\deriv{\mts{\eta}}{\mts{sig}}\) (See Equation @eq:deta_dsig):

~~~~{.cxx}
  //----- Multiaxial correction coefficient
  const auto sn_eq = sqrt(sn | sn);
  const auto isn_eq = 1 / max(seps, sn_eq);
  const auto cm = -trace(sn) * isn_eq;
  const auto dcm_dsig = eval((-isn_eq * id + trace(sn) * power<3>(isn_eq) * sn) | dsn);
  const auto eta = 1 + (cm - 1) * gamma;
  const auto deta_dsig = gamma * dcm_dsig;
~~~~

We are now able to compute the increment \(\Delta\,\telits\) and its derivative:

~~~~{.cxx}
  const auto se = eval((1 + nulits) * sn - nulits * trace(sn) * id);
  delits = C * eta * se;
  const auto dse_dsig = (1 + nulits) * dsn - nulits * ((id ^ id) * dsn);
  const auto ddelits_dsig = C * (se ^ deta_dsig) + C * eta * dse_dsig;
~~~~

Finally, we form the implicit equation associated with the elastic
strain and its derivative:

~~~~{.cxx}
  feel += delits;
  dfeel_ddeel += theta * ddelits_dsig * D;
} // end of @Integrator
~~~~

Note that we used the fact that `feel` is implicitly initialized to
\(\Delta\,\tepsilonel-\Delta\,\tepsilonto\).

## Update of the auxiliary state variables

The `@UpdateAuxiliaryStateVariables` code block is called after the
implicit resolution, after the computation of the stress at the end of
the time step and after the update of the state variables (here the
elastic strain).

We use the last estimate of the its increment to update the `elits`
variable:

~~~~{.cxx}
@UpdateAuxiliaryStateVariables {
  e_lits += delits;
}
~~~~

# References

