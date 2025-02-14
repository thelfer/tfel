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

This tutorial explains how to implement Cailletaud's $\beta$-rule in the context of elasto-viscoplastic
composites using an arbitrary local flow rule.
The local flow rules are implemented as `Behaviour` and used in the global `Behaviour` as `BehaviourVariable`.
In this tutorial, a local behaviour of Meric-Cailletaud type is proposed.

# Cailletaud's beta rule

Cailletaud's beta rule was proposed in the context of elasto-viscoplastic composites for which
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

In this context, the proposition of Cailletaud ...REF  is to
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

# Resolution of the non-linear system

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
  
  and $\mts{\deriv{f}{\tsigma}}$ is a notation for $\deriv{f}{\tsigma}(\tsigma_i+\theta\Delta\tsigma_i)$ and
  
 \begin{aligned}
  \deriv{||\dtepsilonvp_i||}{\dtepsilonvp_j}=\frac 23\delta_{ij}\Frac{\dtepsilonvp_i}{||\dtepsilonvp_i||}.
 \end{aligned}

We note that this last term must be regularized when $||\dtepsilonvp_i||=0$.

# Consistent tangent operator

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

All files `MericCailletaud.mfront`, `BetaRule.mfront` and `BetaRule.mtest` can be downloaded [here](./downloads/BetaRule.zip).

## Implementation of the flow rule and its derivatives

In order to implement Cailletaud's beta rule with arbitrary local flow rules, we implement
these flow rules as `Behaviour` in auxiliary `.mfront` files.
This is a bit unusual because the `gradients` of these local `Behaviour` are the local stresses,
and its `thermodynamic_force` are the strain rates. It will lead to a syntax like `b1.deto=sig_1`
in the sequel.






# Results

We used `MTest` to carry out
simulations on material point, simulating an uniaxial tensile test
(the file is available in the [archive](downloads/BetaRule.zip)).

We represent here...

![Hey]

<!-- Local IspellDict: english -->
