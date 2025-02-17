% Implementation of Cailletaud's beta rule for homogenization
% Helfer Thomas/Martin Antoine
% February 10, 2025

\newcommand{\D}{\mathrm{d}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\mathbf{#1}}}
\newcommand{\dbldot}{\mathbin{\mathord{:}}}
\newcommand{\reals}{\mathbb R}

\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\dtsigma}{\Delta{\underline{\sigma}}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}

\newcommand{\tepsilonto}{\underline{\epsilon}^{\mathrm{to}}}
\newcommand{\tepsilon}{\underline{\epsilon}}
\newcommand{\tepsilonel}{\underline{\epsilon}^{\mathrm{el}}}
\newcommand{\tEel}{\underline{E}^{\mathrm{el}}}
\newcommand{\dtEel}{\Delta\underline{E}^{\mathrm{el}}}

\newcommand{\tepsilonvp}{\underline{\epsilon}^{\mathrm{vp}}}
\newcommand{\dtepsilonvp}{\Delta\underline{\epsilon}^{\mathrm{vp}}}
\newcommand{\tdepsilonvp}{\underline{\dot{\epsilon}}^{\mathrm{vp}}}
\newcommand{\tdbeta}{\underline{\dot{\beta}}}
\newcommand{\tbeta}{\underline{\beta}}
\newcommand{\dtbeta}{\Delta{\underline{\beta}}}

\newcommand{\Frac}[2]{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}

\newcommand{\bts}[1]{\left.#1\right|_{t}}
\newcommand{\mts}[1]{\left.#1\right|_{t+\theta\,\Delta\,t}}
\newcommand{\ets}[1]{\left.#1\right|_{t+\Delta\,t}}

This tutorial explains how to implement Cailletaud's $\beta$-rule [@cailletaud_pilvin_1994]
in the context of elasto-viscoplastic composites using an arbitrary local flow function.
In this tutorial, a local behaviour of Meric-Cailletaud type [@meric_single_1991] is proposed.

Here, the implementation of this homogenization scheme requires the derivative of the local flow function.
This derivative is obtained via the integration of a local `Behaviour`, which
gives the local viscoplastic strain-rate as a function of the local stress.
Hence, in `MFront` implementation, this `Behaviour` sees the local stress as a
`gradient`, and the viscoplastic strain-rate as a `thermodynamic_force`, which is unusual.
Finally, local flow rules are used in the global behaviour of the composite as `BehaviourVariable`.

We first recall Cailletaud's $\beta$-rule and present the non-linear system derived from it.
We then implement it in `MFront` and explain how to implement the local flow rules as `Behaviour`.

# Theoretical background

## Cailletaud's beta rule

Cailletaud's $\beta$-rule was proposed in the context of elasto-viscoplastic composites for which
strain partition is assumed:
\begin{aligned}
  \tepsilonto=\tepsilonel+\tepsilonvp \\
\end{aligned}
where the strain $\tepsilonel$ is given by Hooke's law:
\begin{aligned}
  \tepsilonel=\tenseurq S\dbldot\tsigma \\
\end{aligned}
where $\tenseurq S$ is a fourth-rank compliance tensor (which will be supposed uniform in the sequel)
and $\tsigma$ is the stress field; and  where the viscoplastic strain
$\tepsilonvp$ is given by the following flow rule:
\begin{aligned}
  \tdepsilonvp=f(\tsigma) \\
\end{aligned}
where $f$ is a non-linear function.

We consider then a composite made of $N$ phases indexed by $i$ which checked the above rules.

The macroscopic strain field is defined as
\begin{aligned}
\tenseur E= \sum_\alpha f_i \tepsilonto_i \\
\end{aligned}
and is given as a parameter.

In this context, the proposition of Cailletaud is to
introduce an internal variable $\tenseur\beta_i$ on each phase,
whose evolution is given by
\begin{aligned}
 \tdbeta_i= \tdepsilonvp_i-D\,||\tdepsilonvp_i||\,\tdbeta_i \\
\end{aligned}
where $D\in\reals$ is a parameter and
\begin{aligned}
  ||\tdepsilonvp_i||=\sqrt{\dfrac 23 \tdepsilonvp_i\dbldot\tdepsilonvp_i} \\
\end{aligned}
The stress is also given by the localisation relation:
\begin{aligned}
 \tsigma_i= \tenseurq C\dbldot\tenseur E^{\mathrm{el}} + c\,(\tenseur B-\tenseur\beta_i) \\
\end{aligned}
where $\tenseurq C=\tenseurq S^{-1}$, $c$ is a real parameter, $\tenseur B$ is a macroscopic
variable:
\begin{aligned}
\tenseur B= \sum_i f_i\tenseur\beta_i\\
\end{aligned}
and $\tenseur E^{\mathrm{el}}$ is
\begin{aligned}
\tenseur E^{\mathrm{el}}= \sum_i f_i\tepsilonel_i\\
\end{aligned}

## Resolution of the non-linear system

We discretize the time interval and look for the increment of the variables. The non-linear system
to solve is then:

$\left\{
\begin{aligned}
&\dtEel-\Delta\tenseur E+\sum_{i=1}^{N}f_i\,\Delta\tepsilonvp_i = 0\\
&\Delta\tepsilonvp_i - \Delta t\,f(\tsigma_i+\theta\Delta\tsigma_i) = 0\qquad\forall i\\
&\Delta\tenseur\beta_i - \Delta\tepsilonvp_i+D\,||\Delta\tepsilonvp_i||\,(\tenseur\beta_i+\theta\Delta\tenseur\beta_i)=0\qquad\forall i\\
  \end{aligned}\right.$

whose unknowns are $\dtEel$, $\Delta\tepsilonvp_i$, and $\Delta \tenseur\beta_i$,
and the following variables are introduced:
$\left\{
\begin{aligned}
&\Delta\tsigma_i = \tenseurq C\dbldot\Delta\tenseur E^{\mathrm{el}}  + c\,(\Delta\tenseur B-\Delta\tenseur\beta_i) \qquad\forall i\\
&\Delta\tenseur B = \sum_{i=1}^{N}f_i\,\Delta\tenseur\beta_i\\
  \end{aligned}\right.$
  
Note that the local total and elastic strains can be computed via
$\left\{
\begin{aligned}
&\tepsilonto_i = \tepsilonel_i+\tepsilonvp_i \qquad\forall i\\
&\tepsilonel_i = \tenseurq S\dbldot\tsigma_i\qquad \forall i\\
  \end{aligned}\right.$

To solve this non-linear problem in `MFront`, we use
the `Implicit` `DSL` ([see doc](./implicit-dsl.html)). We only have to precise the residues and the Jacobian matrix.
The residues are:

$\left\{
\begin{aligned}
&f_{\tEel}=\dtEel-\Delta\tenseur E+\sum_{i=1}^{N}f_i\,\Delta\tepsilonvp_i\\
&f_{\tepsilonvp_i} =\Delta\tepsilonvp_i - \Delta t\,f(\tsigma_i+\theta\Delta\tsigma_i) \qquad \forall i\\
&f_{\tbeta_i} =\Delta\tenseur\beta_i - \Delta\tepsilonvp_i+D\,||\Delta\tepsilonvp_i||\,(\tenseur\beta_i+\theta\Delta\tenseur\beta_i) \qquad \forall i\\
    \end{aligned}\right.$
    
 We see the presence of the term $f(\tsigma_i+\theta\Delta\tsigma_i)$ which will be provided by an auxiliary `Behaviour` in `MFront`.
The jacobian matrix is given by
  
 $\left\{
\begin{aligned}
&\deriv{f_{\tEel}}{\dtEel} = \tenseurq I\\
&\deriv{f_{\tEel}}{\Delta\tepsilonvp_j} = f_j\,\tenseurq I\qquad \forall j\\
&\deriv{f_{\tEel}}{\dtbeta_j} = \tenseurq 0\qquad \forall j\\
&\deriv{f_{\tepsilonvp_i}}{\dtEel} = -\theta\,\Delta t\,\mts{\deriv{f}{\tsigma}}\dbldot\tenseurq C \qquad \forall i\\
&\deriv{f_{\tepsilonvp_i}}{\dtepsilonvp_j} = \delta_{ij}\,\tenseurq I\qquad\forall i,j\\
&\deriv{f_{\tepsilonvp_i}}{\Delta \tenseur\beta_j} = c\,(\delta_{ij}-f_j)\theta \Delta t\,\mts{\deriv{f}{\tsigma}}\qquad \forall i,j\\
&\deriv{f_{\tbeta_i}}{\dtEel} = \tenseurq 0\qquad \forall i\\
&\deriv{f_{\tbeta_i}}{\dtepsilonvp_j} = -\delta_{ij}\,\tenseurq I+D\,(\tenseur\beta_i+\theta\Delta\tenseur\beta_i)\otimes\deriv{||\Delta\tepsilonvp_i||}{\dtepsilonvp_j}\qquad \forall i,j\\
&\deriv{f_{\tbeta_i}}{\dtbeta_j} = \left(1+\theta\,D\,||\Delta\tepsilonvp_i||\right)\,\delta_{ij}\,\tenseurq I \qquad \forall i,j\\
  \end{aligned}\right.$
  
  and $\mts{\deriv{f}{\tsigma}}$ is a notation for $\deriv{f}{\tsigma}(\tsigma_i+\theta\Delta\tsigma_i)$. This term will be provided as the tangent operator of the auxiliary `Behaviour`, in `MFront`.
  
  Moreover, $\deriv{||\dtepsilonvp_i||}{\dtepsilonvp_j}=\frac 23\delta_{ij}\Frac{\dtepsilonvp_i}{||\dtepsilonvp_i||}$. This last term must be regularized when $||\dtepsilonvp_i||=0$.

## Consistent tangent operator

We can also compute the tangent operator with the method already explained
in the [Implicit DSL](./implicit-dsl.html) documentation. We define $\Delta Y$ such as
\begin{aligned}
\Delta G_i = (\dtepsilonvp_i, \dtbeta_i)\\
\Delta Y = (\dtEel,\Delta G_1,...,\Delta G_N)\\
\end{aligned}
and $H_i=(f_{\tepsilonvp_i},f_{\tbeta_i})$ and $F=(f_{\tEel},H_1,...,H_N)$ so that
\begin{aligned}
  F(\Delta\tenseur E,\Delta Y(\Delta\tenseur E))&=0\\
 \end{aligned}
 Hence, as explained in the documentation, $\deriv{\dtEel}{\Delta\tenseur E}$ can be retrieved
 from the $6\times 6$ upper-left submatrix of $\mathbf{J}^{-1}$ where $\mathbf{J}=\deriv{F}{\Delta Y}$ is the Jacobian
 of the system.
 
 We can then compute $\deriv{\dtEel}{\Delta\tenseur E}$ and obtain the tangent operator from the following equation:
 \begin{aligned}
  \deriv{\Sigma}{\tenseur E}=\tenseurq C\dbldot\deriv{\dtEel}{\Delta\tenseur E}
 \end{aligned}
 
# Implementation in MFront

The implementation requires the computation of the local flow rule.
This derivative is obtained via the integration of a local `Behaviour`, which
gives the local viscoplastic strain-rate as a function of the local stress.
Hence, in `MFront` implementation, this `Behaviour` sees the local stress as a
`gradient`, and the viscoplastic strain-rate as a `thermodynamic_force`, which is unusual.
Finally, local flow rules are used in the global behaviour of the composite as `BehaviourVariable`.

All files `MericCailletaud.mfront`, `BetaRule.mfront` and `BetaRule.mtest` can be downloaded [here](./downloads/BetaRule.zip).

## Implementation of the flow functions and its derivatives

In order to implement Cailletaud's $\beta$-rule with arbitrary local flow functions, we implement
these flow functions as `Behaviour` in auxiliary `.mfront` files.
This is a bit unusual because the `gradients` of these local `Behaviour` are the local stresses,
and its `thermodynamic_force` are the viscoplastic strain rates. It will lead to a syntax like `b1.deto=sig_1`
in the sequel.

The flow rule chosen for the example is of Méric-Cailletaud type, and is described and
implemented [here](./MericCailletaudSingleCrystalPlasticity.html). The `Behaviour` that
we proposed, which represents the flow rule, differs a little from this previous tutorial
because the `thermodynamic_force` returned by the `Behaviour` is the viscoplastic strain-rate.

First, there is no more elastic strain in this local `Behaviour` and
the `@Brick StandardElasticity` block must be removed.

The only integration variables are the viscoplastic slips $\gamma_i$.
The equivalent visco-plastic slips $p_i$ and the backstrain $\alpha_i$
are conserved as `AuxiliaryStateVariable`:

~~~~ {#MericCailletaud .cpp .numberLines}
@StateVariable strain g[Nss];
g.setEntryName("ViscoplasticSlip");

@AuxiliaryStateVariable strain p[Nss];
p.setEntryName("EquivalentViscoplasticSlip");

@AuxiliaryStateVariable strain a[Nss];
a.setEntryName("BackStrain");
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The shear stresses $\tau_i$ are obtained via the projection of the stress
on each slip system. But now, this stress is no more than the `gradient` of
the `Behaviour`, named `eto`. This leads to the following definition for
`tau`:

~~~~ {#MericCailletaud .cpp .numberLines}
const auto tau = (eto+theta*deto) | ss.mus[i];
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

instead of:

~~~~ {#MericCailletaud .cpp .numberLines}
const auto tau = sig | ss.mus[i];
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

as it was [initially implemented](./MericCailletaudSingleCrystalPlasticity.html).

The `@UpdateAuxiliaryStateVariables` block remains the same as in the initial
implementation. However, the block `@ComputeFinalStress` must be added.
This final stress `sig` is in reality the viscoplastic strain-rate $\tdepsilonvp$,
which is computed by summing the slip rates along each slip system.

~~~~ {#MericCailletaud .cpp .numberLines}
@ComputeFinalStress{
  using size_type = unsigned short;
  const auto& ss = MericCailletaudSlipSystems<real>::getSlipSystems();
  sig=Stensor{real{}};
  for (size_type i = 0; i != Nss; ++i) {
    sig+=dg[i]*ss.mus[i]/dtemps;
  }
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We note here the presence of `dtemps` which permits to return a strain rate.
It is not really important here because the `Behaviour` could also return
an increment of strain.

Finally, we need the `@TangentOperator` block, which provides the derivative
$\deriv{f}{\tsigma}$ needed for the global `Behaviour`.

~~~~ {#MericCailletaud .cpp .numberLines}
@TangentOperator{
  using size_type = unsigned short;
  const auto& ss = MericCailletaudSlipSystems<real>::getSlipSystems();
  Dt=Stensor4{real{}};
  auto iJ=matrix;
  for (size_type i = 0; i != Nss; ++i) {
    auto dgi_dsig=Stensor{real{}};
    for (size_type j = 0; j != Nss; ++j) {
      dgi_dsig+=dv_[j]*iJ(i,j)*ss.mus[j];
    }
    Dt+= ss.mus[i]^dgi_dsig;
  }
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Implementation of Cailletaud's beta rule

The implementation of the global `Behaviour` is straightforward.
For the example, we assume that the composites is made of only 2 phases.
The `@BehaviourVariable` is used to call the local flow rules:

~~~~ {#BetaRule .cpp .numberLines}
@BehaviourVariable b1 {
  file: "MericCailletaud.mfront",
  variables_suffix: "1",
  store_gradients: false,
  store_thermodynamic_forces: false,
  external_names_prefix: "FirstPhase",
  shared_external_state_variables: {".+"}
};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We note that the `gradients` and `thermodynamic_forces` are not
stored because the `StateVariable` `evp1` (resp. `evp2`) are
defined, and because the `AuxiliaryStateVariable` `sig_1` (resp. `sig_2`)
are defined. 

...



Here are the residues:

~~~~ {#BetaRule .cpp .numberLines}
  //residues
  fevp1 = devp1-dt*f_sigm_1;
  fevp2 = devp2-dt*f_sigm_2;
  fbeta1 = dbeta1-devp1+DD*ndevp_1*beta_mts_1;
  fbeta2 = dbeta2-devp2+DD*ndevp_2*beta_mts_2;
  feel = deel-deto+ (1-f)*devp1+f*devp2;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

and the terms of the analytical jacobian:

~~~~ {#BetaRule .cpp .numberLines}
  //jacobian matrix
  dfeel_ddevp1 = (1-f)*Id;
  dfeel_ddevp2 = f*Id;
  
  dfevp1_ddbeta1 = c*f*theta*dt*df_dsigm_1;
  dfevp1_ddbeta2 = -c*f*theta*dt*df_dsigm_1;
  dfevp2_ddbeta1 = -c*(1-f)*theta*dt*df_dsigm_2;
  dfevp2_ddbeta2 = -c*(f-1)*theta*dt*df_dsigm_2;
   
  dfevp1_ddeel = -theta*dt*df_dsigm_1*C;
  dfevp2_ddeel = -theta*dt*df_dsigm_2*C;
  
  dfbeta1_ddevp1 = -Id + 2*DD/3*Stensor4(beta_mts_1^devp1)/max(ndevp_1,eeps);
  dfbeta2_ddevp2 = -Id + 2*DD/3*Stensor4(beta_mts_2^devp2)/max(ndevp_2,eeps);
  
  dfbeta1_ddbeta1 =Id+ theta*DD*ndevp_1*Id;
  dfbeta2_ddbeta2 =Id+ theta*DD*ndevp_2*Id;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

...

The computation of the stress and of the tangent operator is straightforward:

~~~~ {#BetaRule .cpp .numberLines}
@ComputeFinalStress{
  sig += C*deel;
}

@TangentOperator{
  Dt=C*iJ_eel_eel;
  auto Id=Stensor4::Id();
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


# Results

We used `MTest` to carry out
simulations of an uniaxial tensile test
(the file is available in the [archive](downloads/BetaRule.zip)).

.....

![Hey]

<!-- Local IspellDict: english -->
