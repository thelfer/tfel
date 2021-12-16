% Implementation of a multi-surface, compressible and perfect plastic behaviour using the Drucker-Prager yield criterion and a cap
% Thomas Helfer
% 21/11/2017

\newcommand{\absvalue}[1]{{\left|#1\right|}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tdepsilonto}{\tenseur{\dot{\varepsilon}}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\tenseur{\varepsilon}^{\mathrm{el}}}
\newcommand{\tepsilonvis}{\tenseur{\varepsilon}^{\mathrm{vis}}}
\newcommand{\tdepsilonvis}{\tenseur{\dot{\varepsilon}}^{\mathrm{vis}}}
\newcommand{\tepsilonp}{\tenseur{\varepsilon}^{\mathrm{p}}}
\newcommand{\tdepsilonp}{\tenseur{\dot{\varepsilon}}^{\mathrm{p}}}
\newcommand{\tdepsilonpi}{\tenseur{\dot{\varepsilon}}^{\mathrm{p,i}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\sigmaH}{\sigma_{H}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sderiv}[2]{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial #2^{2}}}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}
\newcommand{\transpose}[1]{{#1^{\mathrm{T}}}}

This paper is dedicated to the implementation a multi-surface,
compressible, perfect plastic behaviour.

# Description

## Elasticity

The elasticity is assumed linear and isotropic, i.e. given by the
Hooke law:
\[
\tsigma=\lambda\,\trace{\tepsilonel}\,\tenseur{I}+2\,\mu\,\tepsilonel 
\]
where \(\lambda\) and \(\mu\) are the first and second Lamé parameters.

## Plasticity

- The first surface is based on the Drucker-Prager yield criterion defined as follows:
  \[
  \sigmaeq^{\mathrm{DP}}=q+\tan\paren{\beta}\,p
  \]
  where:
    - \(p\) is the hydrostatique pressure:
      \[
      p = \Frac{1}{3}\,\trace{\tsigma}
      \]
    - \(q\) is the von Mises norm of the stress tensor \(\tsigma\):
      \[
      q = \sigmaeq = \sqrt{3\,J_{2}} = \sqrt{\Frac{3}{2}\,\tenseur{s}\,\colon\,\tenseur{s}}
      \]
      where \(\tenseur{s}\) is the deviatoric part of the stress tensor and \(J_{2}\)
      the second invariant of \(\tenseur{s}\).
  The plastic surface is defined using a constant yield stress
  \(R_{0}^{\mathrm{DP}}\):
  \[
  \sigmaeq^{\mathrm{DP}}-R_{0}^{\mathrm{DP}}=0
  \]
  This surface is a line in the \(\paren{p,q}\) plane.
- The second surface forms a so-called cap which closes the first
  plastic surface. This plastic surface is based on a criterion close the Green one
  (See @fritzen_computational_2013 and [this page](greenplasticity.html)
  for details):
  \[
  \sigmaeq^{c}=\sqrt{\paren{p-p_{a}}^{2}+\paren{R\,q}^{2}}
  \]
  Again, the plastic surface is defined using a constant yield stress
  \(R_{0}^{c}\), as follows:
  \[
  \sigmaeq^{c}-R_{0}^{c}=0
  \]  
  This surface is an ellipse in the \(\paren{p,q}\)
  plane. The material parameter \(p_{a}\) marks the transition between the two
  plastic surface. For the two surface to intersect at \(p_{a}\),
  the following relation must hold:
  \[
  R_{0}^{c}=R\,\paren{R_{0}^{\mathrm{DP}}-p_{a}\,\tan{\beta}}
  \]
  The material constants associated with the cap are
  \(p_{a}\) and the ellipse exentricity \(R\). Here, we prefer
  another material constant, denoted \(p_{b}\), which stands for
  the minimal pressure allowed. \(p_{b}\) is related to \(R\) by:
  \[
  R = \Frac{p_{a} - p_{b}}{d - p_{a} \,\tan\paren{\beta}};
  \]

![Yield surface based on the Drucker-Prager criterion and a cap. The
point associated with the `MFront` key has been obtained with the
implementation described in this page (see below for the material
constants used)](img/DruckerPragerCap.svg "Yield surface based on the
Drucker-Prager criterion and a cap"){width=80%}

> **Note**
>
> Here, the definition of \(p\) differs from some authors who use the opposite.
> With our definition, \(p\) is negative in tri-axial compression
> (\(\trace{\tsigma}<0\)).

Both criterion \(\sigmaeq^{\mathrm{DP}}\) and \(\sigmaeq^{c}\)
are homogeneous function of degree 1.

In the following, the flow rule is assumed associated. For each
mechanism \(i\), the plastic strain rate \(\tdepsilonpi\) is given by:
\[
\tdepsilonpi=\dot{p}^{i}\,\deriv{\sigmaeq^{i}}{\tsigma}=\dot{p}^{i}\,\tenseur{n}^{i}
\]
where \(\dot{p}^{i}\) is the rate of the equivalent plastic strain
\(p^{i}\). The expression of the normals \(\tenseur{n}^{i}\) is given
below.

### Derivatives of the stress criteria

#### Derivatives of the invariants

In the following, we will need the first and second derivatives of \(p\)
and \(q\) with respect to \(\tsigma\). One may refer to
[this page](tensors.html) for more details.

The first derivative of \(p\) with respect to \(\tsigma\) is:
\[
\deriv{p}{\tsigma} = \Frac{1}{3}\,\tenseur{I}
\]

The first derivative of \(q\) with respect to \(\tsigma\) will be
denoted \(\tenseur{n}_{q}\) and is given by:
\[
\deriv{q}{\tsigma} = \tenseur{n}_{q} = \Frac{3}{2\,q}\tenseur{s}
\]

The second derivative of \(q\) with respect to \(\tsigma\) can be
computed as follows:
\[
\sderiv{q}{\tsigma} =
\Frac{1}{q}\paren{\tenseurq{M}-\tenseur{n}_{q}\,\otimes\,\tenseur{n}_{q}}
\]
where \(\tenseurq{M}\) is given by:
\[
\tenseurq{M}=\Frac{3}{2}\,\paren{\tenseurq{I}-\Frac{1}{3}\,\tenseur{I}\,\otimes\,\tenseur{I}}
\]

### Derivatives of \(\sigmaeq^{\mathrm{DP}}\)

Using the previous results, the first derivative of
\(\sigmaeq^{\mathrm{DP}}\) with respect to \(\tsigma\) will be denoted
\(\tenseur{n}^{\mathrm{DP}}\) is:

\[
\deriv{\sigmaeq^{\mathrm{DP}}}{\tsigma} =
\tenseur{n}^{\mathrm{DP}}=
\tenseur{n}_{q}+\Frac{\tan\paren{\beta}}{3}\,\tenseur{I}
\]

The second derivative of \(\sigmaeq^{\mathrm{DP}}\) with respect to
\(\tsigma\) is:
\[
\sderiv{\sigmaeq^{\mathrm{DP}}}{\tsigma} =
\deriv{\tenseur{n}^{\mathrm{DP}}}{\tsigma}=
\sderiv{q}{\tsigma} = 
\Frac{1}{q}\paren{\tenseurq{M}-\tenseur{n}_{q}\,\otimes\,\tenseur{n}_{q}}
\]

### Derivatives of \(\sigmaeq^{c}\)

The first derivative of \(\sigmaeq^{\mathrm{DP}}\) with respect to
\(\tsigma\) will be denoted \(\tenseur{n}^{c}\) and can be computed as
follows:

\[
\deriv{\sigmaeq^{\mathrm{DP}}}{\tsigma} =
\tenseur{n}^{c}=
\Frac{1}{\sigmaeq^{c}}\paren{
\Frac{\paren{p-p_{a}}}{3}\tenseur{I}+R^{2}\,q\,\tenseur{n}_{q}
}
\]

The second derivative of
\(\sigmaeq^{c}\) with respect to \(\tsigma\) is:
\[
\sderiv{\sigmaeq^{c}}{\tsigma}=
\deriv{\tenseur{n}^{c}}{\tsigma}=
\Frac{1}{\sigmaeq^{c}}\paren{
\Frac{1}{9}\,\tenseur{I}\,\otimes\,\tenseur{I}+
R^{2}\,\tenseur{n}_{q}\,\otimes\,\tenseur{n}_{q}+
R^{2}\,\paren{\tenseurq{M}-\tenseur{n}_{q}\,\otimes\,\tenseur{n}_{q}}
-\tenseur{n}_{c}\,\otimes\,\tenseur{n}_{c}
}
\]

# Integration scheme

The behaviour is integrated by an implicit \(\theta\)-scheme. This
scheme requires to choose the state variables and to associate to each
variable an equation.

## Choice of the state variables

Three state variables are introduced:

- the elastic strain \(\tepsilonel\).
- the equivalent plastic strain \(p^{\mathrm{DP}}\).
- the equivalent plastic strain \(p^{c}\) associated with the cap.

The elastic strain is automatically defined by the `Implicit` domain
specific language.

\(p^{\mathrm{DP}}\) and \(p^{c}\)could be considered as an integration
variables, but, for post-processing purposes, we choose to keep it as a
state variables.

## Implicit scheme

Let \(\bts{\vec{Y}}\) be a vector containing the values of the state
variables at the beginning of the time step and \(\Delta\,\vec{Y}\) a
vector holding the values of unknown increment of those state variables.
Here, \(\vec{Y}\) and \(\Delta\,\vec{Y}\) have the the following form:

\[
\bts{\vec{Y}} =
\begin{pmatrix}
\bts{\tepsilonel} \\
\bts{p^{\mathrm{DP}}} \\
\bts{p^{c}} \\
\end{pmatrix}
\quad
\Delta\,\vec{Y}=
\begin{pmatrix}
\Delta\,\bts{\tepsilonel} \\
\Delta\,\bts{p^{\mathrm{DP}}} \\
\Delta\,\bts{p^{c}} \\
\end{pmatrix}
\]

The principle of implicit schemes is to discretize the constitutive
equations so that the increment of the unknowns \(\Delta\,\vec{Y}\)
satisfy the following equation:
\[
\vec{F}\paren{\Delta\,\vec{Y}}=0
\]
where \(\vec{F}\) is a vectorial function.

This (non linear) equation is solved iteratively by the Newton-Raphson
method. This method requires the jacobian \(\deriv{\vec{F}}{\vec{Y}}\)
to be defined.

In the following, \(\vec{F}\) will be decomposed as follows:
\[
\vec{F}\paren{\Delta\,\vec{Y}}=
\begin{pmatrix}
f_{\tepsilonel} \\
f_{p^{\mathrm{DP}}} \\
f_{p^{c}} \\
\end{pmatrix}
\]

The jacobian will be decomposed by blocks as follows:
\[
\deriv{\vec{F}}{\Delta\,\vec{Y}}=
\begin{pmatrix}
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel} &
\deriv{f_{\tepsilonel}}{\Delta\,p^{\mathrm{DP}}} &
\deriv{f_{\tepsilonel}}{\Delta\,p^{c}} \\
\deriv{f_{p^{\mathrm{DP}}}}{\Delta\,\tepsilonel} &
\deriv{f_{p^{\mathrm{DP}}}}{\Delta\,p^{\mathrm{DP}}} &
\deriv{f_{p^{\mathrm{DP}}}}{\Delta\,p^{c}} \\
\deriv{f_{p^{c}}}{\Delta\,\tepsilonel} &
\deriv{f_{p^{c}}}{\Delta\,p^{\mathrm{DP}}} &
\deriv{f_{p^{c}}}{\Delta\,p^{c}} \\
\end{pmatrix}
\]

In the following, we will use the following notation:
\[
\mts{\vec{Y}}=\bts{\vec{Y}}+\theta\,\Delta\,\vec{Y}
\]
where \(\theta\) is a numerical parameter. 

By extension, the value of every function \(f\) of the state variables
that is evaluated with \(\mts{\vec{Y}}\) as argument is denoted
\(\mts{f}\). For example, the stress tensor \(\mts{\tsigma}\) is
evaluated as follows
\[
\mts{\tsigma}=\mts{D}\,\colon\,\mts{\tepsilonel}
\]
where \(\mts{D}\) is the evaluation of the stiffness tensor at
\(t+\theta\,\Delta\,t\). In the example treated here, the stiffness
tensor is constant.

## Status

The integration scheme will be based on a set of three implicit equations:

- The first equation is associated with split of the strain.
- The second equation is associated with the first plastic mechanism
  (Drucker-Prager).
- The third equation is associated with the second plastic mechanism
  (cap).

The last two equations depend on whether the associated mechanism is
assumed active or not. For each systems, a first guess will be made
based on an elastic prediction of the stress, as discussed in the next
paragraph. After a solution to the implicit systems will be found, the
validity of those two assumptions will be checked. If one of the
assumption is false, the resolution will be restarted by making the
opposite assumption.

In pratice, the activation of a plastic mechanism is associated to a
boolean value.

## Elastic prediction

First, an elastic prediction of the stress \(\tsigma^{\mathrm{tr}}\)
is made (The following expression is not valid in plane stress
hypothesis, see below):
\[
\tsigma^{\mathrm{tr}}=\lambda\,\trace{\bts{\tepsilonel}+\theta\,\Delta\,\tepsilonto}\,\tenseur{I}+2\,\mu\,\paren{\bts{\tepsilonel}+\theta\,\Delta\,\tepsilonto}
\]

- If the predicted stress is inside the elastic domain, no plastic
  flow occurs.
- Otherwise, the material state at the end of the time step lies on
  at least one of the yield surfaces.

As describe in the previous paragraph, this elastic prediction is used
initialize the status associated with each mechanism.

## Equation associated with the elastic strain

The equation associated with the evolution of the elastic strain is
given by the split of strain:
\[
f_{\tepsilonel}=\Delta\,\tepsilonel-\Delta\,\tepsilonto+
\Delta\,p^{\mathrm{DP}}\,\mts{\tenseur{n}^{\mathrm{DP}}}+
\Delta\,p^{c}\,\mts{\tenseur{n}^{c}}
\]

The derivatives of this equation with respect to
\(\Delta\,\tepsilonel\), \(\Delta\,p^{\mathrm{DP}}\) and
\(\Delta\,p^{c}\) are given by:

\[
\left\{
\begin{aligned}
\deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel} &=
\tenseurq{I}
+\theta\,\Delta\,p^{\mathrm{DP}}\,\deriv{\mts{\tenseur{n}^{\mathrm{DP}}}}{\mts{\tsigma}}\,\colon\,\mts{\tenseurq{D}}
+\theta\,\Delta\,p^{c}\,\deriv{\mts{\tenseur{n}^{c}}}{\mts{\tsigma}}\,\colon\,\mts{\tenseurq{D}}\\
\deriv{f_{\tepsilonel}}{\Delta\,p^{\mathrm{DP}}} &= \mts{\tenseur{n}^{\mathrm{DP}}}\\
\deriv{f_{\tepsilonel}}{\Delta\,p^{c}} &= \mts{\tenseur{n}^{c}}\\
\end{aligned}
\right.
\]

where \(\mts{\tenseurq{D}}\) is the stiffness tensor. For an isotropic
material, \(\mts{\tenseurq{D}}\) is equal to:

\[
\mts{\tenseurq{D}}=\mts{\lambda}\,\tenseur{I}\,\otimes\,\tenseur{I}+2\,\mts{\mu}\,\tenseurq{I}
\]

## Equation associated with the Drucker-Prager plastic flow

If the Drucker-Prager mechanism is active, the material must be on the
yield surface. Thus, the equation associated with the mechanism is:

\[
f_{p^{\mathrm{DP}}} = \Frac{1}{\mts{E}}\,\paren{\mts{\sigmaeq^{\mathrm{DP}}}-R_{0}^{\mathrm{DP}}}
\]

Here, the Young modulus \(E\) has been used to normalize the equation.

The derivatives of this equation with respect to
\(\Delta\,\tepsilonel\), \(\Delta\,p^{\mathrm{DP}}\) are given by:

\[
\left\{
\begin{aligned}
\deriv{f_{p^{\mathrm{DP}}}}{\Delta\,\tepsilonel} &=
\Frac{\theta}{E}\,\mts{\tenseur{n}^{\mathrm{DP}}}\,\colon\,\mts{\tenseurq{D}}\\
\deriv{f_{p^{\mathrm{DP}}}}{\Delta\,p^{\mathrm{DP}}} &= 0\\
\end{aligned}
\right.
\]

If the Drucker-Prager mechanism is not active, the equation assoaciated
with \(p^{\mathrm{DP}}\) is simply:

\[
f_{p^{\mathrm{DP}}}=\Delta\,p^{\mathrm{DP}}
\]

## Equation associated with the Drucker-Prager plastic flow

If the cap mechanism is active, the material must be on the
yield surface. Thus, the equation associated with the mechanism is:

\[
f_{p^{c}} = \Frac{1}{\mts{E}}\,\paren{\mts{\sigmaeq^{c}}-R_{0}^{c}}
\]

Here, the Young modulus \(E\) has been used to normalize the equation.

The derivatives of this equation with respect to
\(\Delta\,\tepsilonel\), \(\Delta\,p^{c}\) are given by:

\[
\left\{
\begin{aligned}
\deriv{f_{p^{c}}}{\Delta\,\tepsilonel} &= \Frac{\theta}{E}\,\mts{\tenseur{n}^{c}}\,\colon\,\mts{\tenseurq{D}}\\
\deriv{f_{p^{c}}}{\Delta\,p^{c}} &= 0\\
\end{aligned}
\right.
\]

If the cap mechanism is not active, the equation assoaciated
with \(p^{c}\) is simply:

\[
f_{p^{c}}=\Delta\,p^{c}
\]

# Implementation

## Metadata

The beginning of the file gives some information about the behaviour:

- the integration scheme used, selected by the `@DSL` keyword.
- the name of the behaviour, introduced by the `@Behaviour` keyword.
- the author of the implementation (`@Author`).
- a small description of the behaviour (`@Description`).

~~~~{.cpp}
@DSL Implicit;
@Behaviour DruckerPragerCap;
@Author Thomas Helfer;
@Description{
  A simple implementation of a perfect
  plasticity behaviour using the
  Drucker-Prager yield criterion
  closed by a cap.
};
~~~~

## Supported modelling hypothesis

Thanks to the `StandardElasticity` brick, all the modelling hypotheses
can be supported. The following statement, starting with the
`@ModellingHypotheses`, enables all the modelling hypotheses:

~~~~{.cpp}
@ModellingHypotheses {".+"};
~~~~

## The standard elasticity brick

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

The usage of the `StandardElasticity` is introduced as follows:

~~~~{.cpp}
@Brick StandardElasticity;
~~~~

## Numerical parameters

The following part of file give some default values for numerical
parameters used by the integration algorithm:

~~~~{.cpp}
@Epsilon 1.e-16;
@Theta 1;
~~~~

## State variables

The elastic strain is automatically declared the `StandardElasticity`
brick. The associated variable is `eel`.

The following statement introduces an array of two states variables named
`p`:

~~~~{.cpp}
@StateVariable strain p[2];
p.setGlossaryName("EquivalentPlasticStrain");
~~~~

The first entry `p[0]` will be associated to the Drucker-Prager
mechanism. The second entry `p[1]` will be associated to the cap
mechanism.

## Statuses

~~~~{.cpp}
@LocalVariable bool bpl[2];
~~~~

The first entry `bpl[0]` will be associated to the Drucker-Prager
mechanism. The second entry `bpl[1]` will be associated to the cap
mechanism.

## Local variables

Local variables are helpers variables that are accessible in each code
blocks during the all the integration of the behaviour:

~~~~{.cpp}
@LocalVariable real tg;
@LocalVariable real R;
~~~~

## Material constants

### Material constants associated with the elastic part of the behaviour

~~~~{.cpp}
@ElasticMaterialProperties {150e9,0.3};
~~~~

### Material constants associated with the plastic parts of the behaviour

The material constants associated with Drucker-Prager yield surface are
\(\beta\) and \(R_{0}^{\mathrm{DP}}\). In the following,
\(R_{0}^{\mathrm{DP}}\), will be called `d`.

~~~~{.cpp}
@Parameter d    = 150e6;
@Parameter beta = 0.6;
~~~~

~~~~{.cpp}
@Parameter pa   = -10e6;
@Parameter pb   = -75e6;
~~~~

## Initialisation of the local variables

The `@InitLocalVariables` block is called once at the very beginning of
the behaviour integration.

~~~~{.cpp}
@InitLocalVariables {
  tg = tan(beta);
  R = (pa - pb) / (d - pa * tg);
}
~~~~

## Elastic prediction, initial value of the statuses

The `@Predictor` code block is called just before the beginning of the
Newton-Raphson algorihtm.

~~~~{.cpp}
@Predictor{
  const auto sigel = computeElasticPrediction();
  // Drucker-Prager
  const auto prel = trace(sigel) / 3;
  const auto qel = sigmaeq(sigel);
  bpl[0] = (qel + prel * tg - d > 0) && (prel > pa);
  // Cap
  const auto seq_c = sqrt(power<2>(prel - pa) + power<2>(R * qel));
  bpl[1] = (seq_c > (pa - pb)) && (prel <= pa);
}
~~~~

The `computeElasticPrediction` method, introduced by the
`StandardElasticity` brick computes an elastic prediction of stress.
This method takes the modelling hypothesis into account when required
(plane stress, generalised plane stress).

## Implicit system

The implicit system is build inside the `@Integrator` code block:

~~~~{.cpp}
@Integrator {
~~~~

We first declare some useful variables:

~~~~{.cpp}
  constexpr const auto id = Stensor::Id();
  constexpr const auto id4 = Stensor4::Id();
  constexpr const auto id_x_id = Stensor4::IxI();
  constexpr const auto M = Stensor4::M();
  const auto seps =  real(1.e-12) * young;
~~~~

Those variables are declared `constexpr` to allow the compiler to
evaluate them at compile-time. Those variables are declared `const` so
that their values can't be changed.

The following code tests if at least one mechanism is active.

~~~~{.cpp}
  if ((bpl[0]) || (bpl[1])) {
~~~~

If at least one mechanism is active, the following code computes \(p\),
\(q\) and their derivatives:

~~~~{.cpp}
    const auto pr = trace(sig) / 3;
    const auto q = sigmaeq(sig);
    const auto iq = 1 / max(q,seps);
    const auto nq = eval(3 * deviator(sig) * (iq / 2));
    const auto dnq = eval((M - (nq ^ nq)) * iq);
~~~~

We now treat the case where the Drucker-Prager mechanism is active.

~~~~{.cpp}
    if (bpl[0]) {
      const auto n   = eval(nq+(tg/3)*id);
      feel += dp[0]*n;
      fp(0) = (q+pr*tg-d)/young;
      // jacobian
      dfeel_ddeel += 2*mu*theta*dp[0]*dnq;
      dfeel_ddp(0) = n;
      dfp_ddp(0,0) = 0;
      dfp_ddeel(0) = (2*mu*theta/young)*n+(tg*theta*lambda/young)*id;
    }
~~~~

The following code describes the implicit equations to be solved if the
cap mechanism is active.

~~~~{.cpp}
    if (bpl[1]) {
      const auto seq_c = sqrt(power<2>(pr - pa) + power<2>(R * q));
      const auto iseq_c = 1 / max(seq_c,seps);
      const auto n = eval(((pr - pa) * (id / 3) + R * R * q * nq) * iseq_c);
      const auto dn = (Stensor4::IxI() / 9 + R * R * M - (n ^ n)) * iseq_c;
      const auto De = 2 * mu * id4 + lambda * id_x_id;
      feel += dp[1] * n;
      fp(1) = (seq_c - R * (d - pa * tg)) / young;
      // jacobian
      dfeel_ddeel += theta * dp[1] * dn * De;
      dfeel_ddp(1) = n;
      if(std::abs(seq_c)<seps){
        dfp_ddp(1, 1) = 1;
      } else {
        dfp_ddp(1, 1) = 0;
      }
      dfp_ddeel(1) = theta * (n | De) / young;
    }
~~~~

The following braces closes the test which was used to check that at
least one mechanism is active.

~~~~{.cpp}
  } // end of if ((bpl[0]) || (bpl[1]))
~~~~

Finally, the `@Integrator` code block is closed:

~~~~{.cpp}
} // end of @Integrator
~~~~

## A posterio checks: validation of the assumptions

The `@AdditionalConvergenceChecks` code block has been introduced in
`TFEL-3.1` to have a better control on the convergence of the
Newton-Raphson algorithm. In particular, this block can be used to
change the status associated with each mechanism.

Here, we will change the status associated with a mechanism once the
Newton-Raphson algorithm has converged (this is indicated by the
`converged` boolean variable).

If a mechanism is active, we check that \(\Delta\,p_{i}\) is positive.
Otherwise, the mechanism is desactivated and the `converged` flag is set
to `false`.

If a mechanism is not active, we check that the final solution remain
below the yield surface. Otherwise, the mechanism is activated and the
`converged` flag is set to `false`.

~~~~{.cpp}
@AdditionalConvergenceChecks {
  if (converged){
    if (bpl[0]) {
      if (dp[0] < 0) {
        // desactivating this system
        converged = bpl[0] = false;
      }
    } else {
      const auto pr = trace(sig) / 3;
      const auto q = sigmaeq(sig);
      if (q + pr * tg > d) {
        converged = false;
        bpl[0] = true;
      }
    }
    if (bpl[1]) {
      if (dp[1] < 0) {
        // desactivating this system
        converged = bpl[1] = false;
      }
     } else {
       const auto pr = trace(sig) / 3;
       const auto q = sigmaeq(sig);
       const auto seq_c = sqrt(power<2>(pr - pa) + power<2>(R * q));
       if ((seq_c > pa - pb) && (pr < pa)) {
         converged = false;
         bpl[1] = true;
       }
     }
  }
}
~~~~

# Evolution of the porosity

In this paragraph, we will discuss how the evolution of the porosity of
the material du to the plastic flow can be added.

The porosity \(f\) is defined as the ratio between the current mass
density \(\rho\) and the theoretical density \(\rho^{\mathrm{th}}\):
\[
f=1-\Frac{\rho}{\rho^{\mathrm{th}}}
\]

The mass conservation reads:
\[
\rho\,J=\rho_{0}
\]
where \(\rho_{0}\) is the initial density and \(J\) the change of
volume.

Thus, by derivation with respect to time:
\[
\dot{\rho}\,J+\rho\,\dot{J}=0\quad\Rightarrow\quad\Frac{\dot{\rho}}{\rho}=-\Frac{\dot{J}}{J}
\]

In the Hencky strain measure is used (see the `@StrainMeasure` keyword),
the following relation holds:
\[
J=\exp\paren{\trace{\tepsilonto}}
\]
Hence,
\[
\dot{\rho}=-\rho\,\trace{\tdepsilonto}
\]

Finally, the evolution of the porosity is:
\[
\dot{f}=\paren{1-f}\,\trace{\tdepsilonto}
\]

Following @chaboche_endommagement_2001, the elasic part of the strain is neglected and the following evolution of \(f\) is finally kept:
\[
\dot{f}=\paren{1-f}\,\trace{\tdepsilonp}
\]

> **Dependency of the elastic material properties with the porosity**
> 
> For simplicity, we don't treat here the case where the elastic
> properties depend on the porosity. Indeed, in this case, the usage of
> the `StandardElasticity` brick is not more justified. In pratice,
> however, neglecting the additional terms du to such a dependency may
> work quite well.

Assuming no dependency of elastic material properties with the porosity,
two cases can be distinguished:

- The yield criteria and the isotropic hardening depend on the porosity.
  In this case, the simpliest approach is to consider the porosity as a
  new state variable and to add the appropriate derivatives of the yield
  surface in the definition of the jacobian of the implicit system.
- The yield criteria and the isotropic hardening do not depend on the
  porosity. In this second case, the porosity can be treated as an
  auxiliary state variable.

In some cases, the second case may also be appropriate even if the yield
criteria and the isotropic hardening depend on the porosity, but the
evolution of the porosity during the time step has a negligible impact.

## Porosity as a state variable

The discretization of the evolution of the porosity leads to the
following equation:
\[
f_{f} =
\Delta\,f-\paren{1-\mts{f}}\,\paren{
\Delta\,p^{\mathrm{DP}}\,\trace{\mts{\tenseur{n}^{\mathrm{DP}}}}
+
\Delta\,p^{c}\,\trace{\mts{\tenseur{n}^{c}}}
}
\]

The derivatives of the \(f_{p}\) are trivial:
\[
\left\{
\begin{aligned}
\deriv{f_{f}}{\Delta\,f}&=1+
\theta\,
\paren{
\Delta\,p^{\mathrm{DP}}\,\trace{\mts{\tenseur{n}^{\mathrm{DP}}}}
+
\Delta\,p^{c}\,\trace{\mts{\tenseur{n}^{c}}}
}\\
&+\paren{1-\mts{f}}\,
\paren{
\Delta\,p^{\mathrm{DP}}\,\trace{\deriv{\mts{\tenseur{n}^{\mathrm{DP}}}}{\Delta\,f}}
+
\Delta\,p^{c}\,\trace{\deriv{\mts{\tenseur{n}^{c}}}{\Delta\,f}}
}\\
\deriv{f_{f}}{\Delta\,p^{\mathrm{DP}}}&=
-\paren{1-\mts{f}}\,
\trace{\mts{\tenseur{n}^{\mathrm{DP}}}}\\
\deriv{f_{f}}{\Delta\,p^{c}}&=
-\paren{1-\mts{f}}\,
\trace{\mts{\tenseur{n}^{c}}}\\
\deriv{f_{f}}{\Delta\,\tepsilonel}&=
-\paren{1-\mts{f}}\,
\tenseur{I}\,\colon\,
\paren{
\Delta\,p^{\mathrm{DP}}\,\deriv{\tenseur{n}^{\mathrm{DP}}}{\Delta\,\tepsilonel}+
\Delta\,p^{\mathrm{c}}\,\deriv{\tenseur{n}^{c}}{\Delta\,\tepsilonel}
}\\
\end{aligned}
\right.
\]

In some case, neglecting the trace of
\(\deriv{\mts{\tenseur{n}^{\mathrm{DP}}}}{\Delta\,f}\) and
\(\deriv{\mts{\tenseur{n}^{c}}}{\Delta\,f}\) can be a very good approximation.

## Porosity as an auxiliary state variable

The evolution of the porosity can be computed once the plastic
increments are known, as follows:

\[
\Delta\,f = \Frac{\paren{1-\bts{f}}\,\trace{\Delta\,\tepsilonp}}{1+\theta\,\trace{\Delta\,\tdepsilonp}}
\]

with:
 
\[
\trace{\Delta\,\tepsilonp}=
\Delta\,p^{\mathrm{DP}}\,\trace{\mts{\tenseur{n}^{\mathrm{DP}}}}+
\Delta\,p^{c}\,\trace{\mts{\tenseur{n}^{c}}}
\]

# References

<!-- Local IspellDict: english -->
