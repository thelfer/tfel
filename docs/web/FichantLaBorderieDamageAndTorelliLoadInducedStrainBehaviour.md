% Coupling the Fichant-La Borderie damage behaviour and Torelli' load induced strain behaviour
% Thomas Helfer
% 16/12/2020

\newcommand{\absvalue}[1]{{\left|#1\right|}}
\newcommand{\ppos}[1]{{\left<#1\right>_{+}}}
\newcommand{\pneg}[1]{{\left<#1\right>_{-}}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\D}{\ets{\tenseurq{D}}}
\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\tenseur{\varepsilon}^{\mathrm{el}}}
\newcommand{\tepsilonelp}{\ppos{\tenseur{\varepsilon}^{\mathrm{el}}}}
\newcommand{\eeq}{\varepsilon^{\mathrm{el}}_{\mathrm{eq}}}
\newcommand{\lits}{\mathrm{LITS}}
\newcommand{\telits}{\tenseur{\varepsilon}^{\lits}}
\newcommand{\tdelits}{\tenseur{\dot{\varepsilon}}^{\lits}}

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

This document describes how to build a behaviour of concrete describing
the damage evolution following Fichant-La Borderie and the load induced
thermal strain (`LITS`) following Torelli et al
(@torelli_confinement-dependent_2018).

We strongly advice readers to refer to the following documents before
continuing:

- The implementation of the Fichant-La Borderie damage behaviour is
  detailled [here](FichantLaBorderieDamageBehaviour.html).
- The implementation of the Fichant-LaBorderie damage behaviour is
  detailled [here](LoadInducedThermalStrainBehaviourTorelli2018.html).

# Description of the coupled behaviour

The behaviour is described by a standard split of the strain
\(\tepsilonto\) in an elastic and a viscoplastic parts, respectively
denoted \(\tepsilonel\) and \(\telits\):

\[
\tepsilonto=\tepsilonel+\telits
\]{#eq:eel}

## Damage behaviour

The stress \(\tsigma\) is related to the the elastic strain
\(\tepsilonel\) by the Fichant-La Borderie damage behaviour:

\[
\tsigma = f_{FLB}\paren{d,\tepsilonel}
\]{#eq:FLB}

where \(d\) is a scalar damage variable. See [this
page](FichantLaBorderieDamageBehaviour.html) for details.

### Damage evolution

The damage evolution is a function of the equivalent elastic strain
\(\eeq\), which will be defined hereafter:

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

where \(\ets{\eeq}\) is the value of the equivalent elastic strain at
the end of the time step.

### Equivalent strain

The equivalent elastic strain \(\eeq\) is defined as:

\[
\eeq=\sqrt{\tepsilonelp\,\colon\,\tepsilonelp}=
\sqrt{\sum_{I=1}^{3}\ppos{\varepsilon^{\mathrm{el}}_{I}}^2}
\]

where \(\tepsilonelp\) is the positive part of the elastic strain and
\(\left(\varepsilon^{\mathrm{el}}_{I}\right)_{i \in [1:3]}\) are its
eigenvalues.

### Computation of the stress

The stress tensor \(\tsigma\) is then computed as follows:
\[
\ets{\tsigma}=\paren{1-\ets{d}}\,\ppos{\ets{\tsigmae}}+\paren{1-\ets{d}^{a}}\,\pneg{\ets{\tsigmae}}
\]

where the effective stress tensor \(\tsigmae\) is computed using the
standard Hooke law:

\[
\ets{\tsigmae}=\D\,\colon\,\ets{\tepsilonel}
\]

The behaviour is assumed isotropic, so that the stiffness tensor \(\D\)
can be related to the first and second Lamé coefficients, denoted
respectively \(\lambda\) and \(\mu\), as follows:

\[
\D=\ets{\lambda}\,\tenseur{I}\,\otimes\,\tenseur{I}+2\,\ets{\mu}\,\tenseurq{I}
\]


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

# Implicit algorithm

Following the implementation of Torelli' LITS behaviour (see
[here](LoadInducedThermalStrainBehaviourTorelli2018.html) for details),
the implicit scheme can be reduced to one non linear equation whose only
unknown is the elastic strain increment:

\[
f_{\tepsilonel}=\Delta\,\tepsilonel+\Delta\,\telits\paren{\Delta\,\tepsilonel}-\Delta\,\tepsilonto=\tenseur{0}
\]{#eq:feel}

Computing the jacobian \(\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel}\)
is straightforward:

\[
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel}=\tenseurq{I}+\deriv{\Delta\,\telits}{\mts{\tsigma}}\,\colon\,\deriv{\mts{\tsigma}}{\Delta\,\tepsilonel}
\]

The derivative \(\deriv{\Delta\,\telits}{\mts{\tsigma}}\) has been
computed on [this
page](LoadInducedThermalStrainBehaviourTorelli2018.html) while the
derivative \(\deriv{\mts{\tsigma}}{\Delta\,\tepsilonel}\) is computed on
[this page](FichantLaBorderieDamageBehaviour.html).

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
@Behaviour FichantLaBorderieDamageAndTorelliLoadInducedThermalStrain;
~~~~

Defining the name of behaviour is required.


## Metadata

The `@Author`, `@Date` and `@Description` keywords allows adding
metadata to the behaviour (respectively the name of the author of the
implementation, the date at which the implementation was written, a
short description), as follows:

~~~~{.cxx}
@Author Thomas Helfer;
@Date 15 / 12 / 2019;
@Description {
  "Implementation of a behaviour combining Fichant-La Borderie' "
  "description of damage and Torelli'LITS"
}
~~~~

## Numerical parameters

We specify two numerical parameters impacting the implicit scheme:

- the \(\theta\) parameter (default value is \(1/2\)). In pratice, we
  set to \(1\) to indicate that the resolution is purely implicit, but
  we won't use this value in the implementation.
- the stopping criteria (default value is \(10^{-8}\)). This may be
  sufficient in most cases, but not for the computation of the
  consistent tangent operator.

~~~~{.cxx}
@Epsilon 1.e-14;
@Theta 1;
~~~~

## Thermoelastic properties, computation of the thermal strain

The `@ElasticMaterialProperties` keyword allows defining the material
properties. When those are defined, the `Implicit` `DSL` automatically
defines the first and second Lamé coefficients in local variables
called:

- `lambda` and `mu` at time \(t+\theta\,\Delta\,t\)
- `lambda_tdt` and `mu_tdt` at time \(t+\Delta\,t\)

~~~~{.cxx}
@ElasticMaterialProperties{47000, 0.25};
~~~~

The `@ComputeThermalExpansion` keyword allows computing the thermal
strain. It is followed by the mean thermal expansion coefficient.

~~~~{.cxx}
@ComputeThermalExpansion 10e-6;
~~~~

~~~~{.cxx}
// parameters of the Fichant-La Borderie part
@Parameter Bt = 3690.070983;
@Parameter e0 = 1.03e-04;
@Parameter a = 3;
~~~~

~~~~{.cxx}
// parameters of the LITS part
@Parameter gamma = 1.5;
@Parameter sigmultimate = 50.;
@Parameter nulits = 0.50;
@Parameter tcrit = 0.;
@Parameter b[5] = {2.7031065533E-05, -1.0209170592E-06, 6.1200423753E-9,  //
                   -1.2632648735E-11, 6.9158539621E-15};
~~~~


~~~~{.cxx}
@AuxiliaryStateVariable real d;
d.setGlossaryName("Damage");
@AuxiliaryStateVariable StrainStensor elits;
elits.setEntryName("LoadInducedThermaStrain");
@AuxiliaryStateVariable temperature Tmax;
Tmax.setEntryName("MaximalValueOfTheTemperature");
~~~~

~~~~{.cxx}
//! LITS increment
@LocalVariable StrainStensor delits;
//! Creep coefficient
@LocalVariable real C;
~~~~

~~~~{.cxx}
@LocalVariable StiffnessTensor dsig_ddeel;
@LocalVariable real d_p;
@LocalVariable StiffnessTensor De;
~~~~

~~~~{.cxx}
@InitLocalVariables {
  Tmax = max(max(tcrit, Tmax), T);
  const auto T_ = T + theta * dT;
  const auto beta = b[0] + T_ * (b[1] + T_ * (b[2] + T_ * (b[3] + T_ * b[4])));
  const auto dTe = max(T + dT - max(tcrit, Tmax), temperature(0));
  C = (beta / (-sigmultimate)) * dTe;
  De = lambda_tdt * Stensor4::IxI() + 2 * mu_tdt * Stensor4::Id();
} // end of @InitLocalVariables
~~~~

~~~~{.cxx}
@Integrator {
  constexpr const auto id = Stensor::Id();
  constexpr const auto esolver = StressStensor::FSESJACOBIEIGENSOLVER;
  constexpr const stress eeps = 1.e-12;
  const stress seps = 1.e-12 * young;
~~~~

~~~~{.cxx}
  // positive part
  const auto pp = [](const real x) { return x > 0 ? x : 0; };
  // derivative of the positive part
  const auto dpp = [&eeps](const real x) { return std::abs(x) < eeps ? 0.5 : ((x < 0) ? 0 : 1); };
  // square of the posititve part
  auto square_ppos = [](const strain& v) { return v > 0 ? v * v : 0; };
  // elastic strain at the midle of the time step
  const auto e = eval(eel + deel);
  // eigen values and eigen tensors of the elastic strain
  auto e_vp = tvector<3u, strain>{};
  auto m = tmatrix<3u, 3u, strain>{};
  e.template computeEigenVectors<esolver>(e_vp, m);
  // update the damage
  const auto e_eq = sqrt(square_ppos(e_vp[0]) + square_ppos(e_vp[1]) + square_ppos(e_vp[2]));
  // effective stress at t+dt
  const auto Cd = (e0 / e_eq) * exp(Bt * (e0 - e_eq));
  d_p = (e_eq > e0) ? 1 - Cd : 0;
  const auto bp = d_p > d;
  const auto de = bp ? d_p : d;
  // derivative with respect to the damage
  const auto dde_ddeel = [&]() -> Stensor {
    if (!bp) {
      return Stensor(real(0));
    }
    // positive part of the total strain
    const auto ep = StrainStensor::computeIsotropicFunction(pp, e_vp, m);
    // derivative of the damage
    const auto dde_deq = Cd * (Bt + 1 / e_eq);
    const auto dep_ddeel =
        StrainStensor::computeIsotropicFunctionDerivative(pp, dpp, e_vp, m, eeps * 0.1);
    const auto deq_dep = ep / e_eq;
    return dde_deq * deq_dep * dep_ddeel;
  }();
  // function of the damage to simplify expressions
  const auto de_a = pow(de, a);
  const auto fpd = (1 - de);
  const auto fpn = (1 - de_a);
  // effective stress at the end of the time step
  const auto l_tr_e = lambda_tdt * trace(e);
  const auto s = eval(l_tr_e * id + 2 * mu_tdt * e);
  const auto s_vp = tvector<3u>{l_tr_e + 2 * mu_tdt * e_vp[0],  //
                                l_tr_e + 2 * mu_tdt * e_vp[1],  //
                                l_tr_e + 2 * mu_tdt * e_vp[2]};
  const auto sp = StressStensor::computeIsotropicFunction(pp, s_vp, m);
  const auto dsp = StressStensor::computeIsotropicFunctionDerivative(pp, dpp, s_vp, m, seps * 0.1);
  const auto sn = eval(s - sp);
  const auto dsn = eval(Stensor4::Id() - dsp);
  // final stress
  sig = fpd * sp + fpn * sn;
  // derivative of the stress
  dsig_ddeel = ((fpd - fpn) * dsp + fpn * Stensor4::Id()) * De;
  if (bp) {
    const auto ide = 1 / max(eeps, de);
    const auto dfpd_dd = -1;
    const auto dfpn_dd = -a * de_a * ide;
    dsig_ddeel += dfpd_dd * (sp ^ dde_ddeel) + dfpn_dd * (sn ^ dde_ddeel);
  }
~~~~

~~~~{.cxx}
  // LITS part
  const auto sn_eq = sqrt(sn | sn);
  const auto isn_eq = 1 / max(seps, sn_eq);
  const auto cm = -trace(sn) * isn_eq;
  const auto dcm_dsig = eval((-isn_eq * id + trace(sn) * power<3>(isn_eq) * sn) | dsn);
  const auto eta = 1 + (cm - 1) * gamma;
  const auto se = eval((1 + nulits) * sn - nulits * trace(sn) * id);
  delits = C * eta * se;
  const auto deta_dsig = gamma * dcm_dsig;
  const auto dse_dsig = (1 + nulits) * dsn - nulits * ((id ^ id) * dsn);
  const auto ddelits_dsig = C * (se ^ deta_dsig) + C * eta * dse_dsig;
~~~~


~~~~{.cxx}
  // elasticity
  feel += delits;
  dfeel_ddeel += ddelits_dsig * dsig_ddeel;
} // end of @Integrator
~~~~

~~~~{.cxx}
@UpdateAuxiliaryStateVariables {
  d = max(d, d_p);
  elits += delits;
}
~~~~

~~~~{.cxx}
@TangentOperator {
  if (smt == ELASTIC) {
    Dt = D;
  } else if (smt==CONSISTENTTANGENTOPERATOR){
    Stensor4 ddeel_ddeto;
    getPartialJacobianInvert(ddeel_ddeto);
    Dt = dsig_ddeel * ddeel_ddeto;
  }
}
~~~~

# References