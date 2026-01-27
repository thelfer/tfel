---
title: Ponte-Castaneda second-order estimates for homogenization of a viscoplastic polycrystal
author: Antoine Martin
date: 15/01/2026
lang: en-EN
link-citations: true
colorlinks: true
geometry:
  - margin=2cm
papersize: a4
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\mathbf{#1}}}
\newcommand{\tenseurt}[1]{\underline{\underline{\underline{#1}}}}
\newcommand{\tns}[1]{\underset{\tilde{}}{\mathbf{#1}}}
\newcommand{\transpose}[1]{#1^{\mathop{T}}}
\newcommand{\Ka}{\mathcal K}
\newcommand{\Sa}{\mathcal S}
\newcommand{\tsigma}{\tenseur{\sigma}}
\newcommand{\dbldot}{:}
\newcommand{\tepsilon}{\tenseur{\varepsilon}}
\newcommand{\te}{\tenseurq{e}}
\newcommand{\sigmaeq}{\sigma_{\mathrm{eq}}}
\newcommand{\epsiloneq}{\varepsilon_{\mathrm{eq}}}

\newcommand{\tepsilonto}{\underline{\epsilon}^{\mathrm{to}}}
\newcommand{\tepsilonel}{\underline{\epsilon}^{\mathrm{el}}}

\newcommand{\trace}[1]{\mathrm{tr}\paren{#1}}
\newcommand{\Frac}[2]{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\derivdeux}[2]{{\displaystyle \frac{\displaystyle \partial^2 #1}{\displaystyle \partial #2^2}}}
\newcommand{\pderiv}[2]{{\dfrac{\partial #1}{\partial #2}}}
\newcommand{\pderivdeux}[2]{{\dfrac{\partial^2 #1}{\partial #2^2}}}
\newcommand{\derivderiv}[3]{{\displaystyle \frac{\displaystyle \partial^2 #1}{\displaystyle \partial #2\,\displaystyle \partial #3}}}
\newcommand{\dtot}{{\mathrm{d}}}
\newcommand{\paren}[1]{\left(#1\right)}
\newcommand{\nom}[1]{\textsc{#1}}
\renewcommand{\div}{\mathrm{div}}


We present here an implementation of Ponte-Castaneda second-order estimates [@castaneda_exact_1996]
for the homogenization of a viscoplastic polycrystal, example also treated in [@bornert_second-order_2001].
The procedure can also be used in non-linear elasticity, providing that the behaviour is governed
by only one potential.

Here, the idea is to show that many implementations of that procedure are possible in `MFront`.
We particularly present the possibility to take into account the morphology of the polycrystal
by using interaction tensors, which are provided before the integration. It results in better
estimates than classical ones. Note that the resulting non-linear system involved
couples the unknowns, and hence increases the cost of the integration.
We also show that we can use a self-consistent scheme, making use of
`tfel::material::homogenization`.

This tutorial first presents the homogenization problem, recalls Ponte-Castaneda's methodology
from [@castaneda_exact_1996], presents different kinds of possible implementations,
and show the details of the `mfront` file.

# The viscoplastic polycrystal

## Behaviour

We consider a polycrystalline material, which means that each phase $r$ is associated to a crystal with
corresponding slip systems $\tenseur \mu_k^r$ ($1\leq k\leq K$). The strain rate in each crystal $r$ is given by
  \begin{aligned}
    \dot{\tepsilon}=\sum_k \dot{\gamma}_k^r\, \tenseur \mu_k^r\qquad\text{with}\quad \tenseur\mu_k^r = \dfrac12\left(\tenseur n_k^r\otimes\tenseur m_k^r + \tenseur m_k^r\otimes\tenseur n_k^r\right)
  \end{aligned}
  where $\dot{\gamma}_k^r$ is the shear strain rate on the $k^{th}$ slip system of crystal $r$, and is given as a
  function of the shear stress $\tau_k^r=\tsigma\dbldot\tenseur \mu_k^r$ by means of a potential $\psi_k^r$:
  \begin{aligned}
  \dot{\gamma}_k^r= \deriv{\psi_k^r}{\tau}\left(\tau_k^r\right)
  \end{aligned}
  The expressions above show that on phase (or crystal) $r$:
  \begin{aligned}
  \dot{\tepsilon}=\deriv{\psi_r}{\tsigma}\left(\tsigma\right)\qquad\text{with}\quad\psi_r \left(\tsigma\right)=\sum_k \psi_k^r\left(\tau_k^r\right)
  \end{aligned}
  
  Now, the behaviour of the polycrystal is governed by a potential $\psi$:
  \begin{aligned}
    \dot{\tepsilon}=\deriv{\psi_r}{\tsigma}\left(\tsigma\right)\qquad\psi(\tsigma)= \sum_{r=1}^{N}\chi_r\,\psi_r (\tsigma)
  \end{aligned}
where $N$ is the number of phases (or crystals) and $\chi_r$ is characteristic function of phase $r$.
 
 In all the sequel, we just note $\tepsilon$ for $\dot{\tepsilon}$.
  
## The non-linear homogenization problem

We impose a strain rate $\overline{\tepsilon}$ to the polycrystal and look for the solution $\tepsilon,\tsigma$ such that:

\begin{aligned}
 &\div\,\tsigma=\tenseur 0\\
 &\tepsilon\in\Ka(\overline{\tepsilon})\\
 &\tepsilon = \deriv{\psi}{\tsigma}\left(\tsigma\right)
\end{aligned}

where we introduced the space of kinematically admissible fields $\mathcal K(\overline{\tepsilon})$, depending on the boundary conditions used (in the implementation, we work with periodic boundary conditions).
The effective energy of the composite is then defined as the following average on the
entire Representative Volume Element (RVE):
\begin{aligned}
\overline{\phi}\left(\overline{\tepsilon}\right)= \langle\phi\left(\tepsilon\right)\rangle
\end{aligned}
where $\tepsilon$ is the solution of the above problem, $\langle.\rangle$ denotes the average
on the RVE, and $\phi$ is the primal potential,
defined as the Legendre-Fenchel transform of $\psi$:
\begin{aligned}
\phi\left(\tepsilon\right)=\underset{\tsigma}{\mathrm{max}}\left\{\tsigma\dbldot\tepsilon-\psi\left(\tsigma\right)\right\}
\end{aligned}

# Ponte-Castaneda second-order estimates

## The variational approach

 The homogenization problem is equivalent to a maximization problem:
 \begin{aligned}
    \overline{\phi}\left(\overline{\tepsilon}\right)=\underset{\tsigma\in\Sa}{\textrm{max}} \left\{ \langle\tsigma\rangle\dbldot\overline{\tepsilon} - \langle\psi(\tsigma)\rangle\right\}
  \end{aligned}
  where $\Sa$ is the set of divergence-free fields.
  
  Note that a dual minimization problem can also be solved to find the effective energy
\begin{aligned}
    \overline{\phi}\left(\overline{\tepsilon}\right)=\underset{\tepsilon\in\mathcal K(\overline{\tepsilon})}{\textrm{min}} \langle \phi(\tepsilon)\rangle
  \end{aligned}
  However, because $\phi$ is known only through the Legendre-Fenchel transform of $\psi$, it will be more
  easy to use the maximization problem above.
  
## The second-order estimates

Ponte-Castaneda's idea is to make a second-order Taylor development of each $\psi_r$ around a given "reference" stress $\tsigma^r$ in phase $r$:
\begin{aligned}
    \psi_r(\tsigma)\approx\psi_r(\tsigma^r)+\tepsilon_r(\tsigma^r)\dbldot\left(\tsigma-\tsigma^r\right)+\dfrac12\left(\tsigma-\tsigma^r\right)\dbldot\tenseurq M_r\left(\tsigma^r\right)\dbldot\left(\tsigma-\tsigma^r\right)
  \end{aligned}
  where
  \begin{aligned}
    \tepsilon_r(\tsigma^r)=\deriv{\psi_r}{\tsigma}\left(\tsigma^r\right)\qquad\text{and}\qquad\tenseurq M_r\left(\tsigma^r\right)=\derivdeux{\psi_r}{\tsigma}\left(\tsigma^r\right)
  \end{aligned}
  give a "tangent" estimation of the strain $\tepsilon_r$ around $\tsigma^r$:
    \begin{aligned}
    \tepsilon_r(\tsigma)\approx\tepsilon_r(\tsigma^r)+\tenseurq M_r\left(\tsigma^r\right)\dbldot\left(\tsigma-\tsigma^r\right)
  \end{aligned}
  and can be viewed as the behaviour of a so-called "thermoelastic comparison composite". We hence obtain
  \begin{aligned}
    \psi_r(\tsigma)\approx v_r\left(\tsigma^r\right)+\tenseur e^r\left(\tsigma^r\right)\dbldot\tsigma+\dfrac12\tsigma\dbldot\tenseurq M_r\left(\tsigma^r\right)\dbldot\tsigma
  \end{aligned}
  with
  \begin{aligned}
    &v_r\left(\tsigma^r\right)=\psi_r(\tsigma^r)-\tepsilon_r(\tsigma^r)\dbldot\tsigma^r+\dfrac12\tsigma^r\dbldot\tenseurq M_r\left(\tsigma^r\right)\dbldot\tsigma^r\\
    &\tenseur e^r\left(\tsigma^r\right)=\tepsilon_r(\tsigma^r)-\tenseurq M_r\left(\tsigma^r\right)\dbldot\tsigma^r
  \end{aligned}
  
  Afterwards, the effective energy is found by the maximization problem:
  \begin{aligned}
    \overline{\phi}\left(\overline{\tepsilon}\right)=\underset{\tsigma\in\Sa}{\textrm{max}} \sum_r c_r\,\left[\langle\tsigma\rangle_r\dbldot\left(\overline{\tepsilon}-\tenseur e^r\right)-v_r-\dfrac12\langle\tsigma\dbldot\tenseurq M_r\dbldot\tsigma\rangle_r\right]
  \end{aligned}
  where $c_r$ is the volume fraction of the phase $r$, and $\langle.\rangle_r$ denotes the average on phase $r$.
  The effective energy is hence given by
  \begin{aligned}
    \overline{\phi}\left(\overline{\tepsilon}\right)\approx P\left(\overline{\tepsilon},\tsigma^r\right) -\sum_r c_r\,v_r\qquad\text{with}\quad P\left(\overline{\tepsilon},\tsigma^r\right)=\underset{\tsigma\in\Sa}{\textrm{max}}\sum_r c_r\, \langle \tenseur d^r\dbldot\tsigma-\dfrac12\tsigma\dbldot\tenseurq M_r\dbldot\tsigma\rangle_r
  \end{aligned}
  where $\tenseur d^r=\overline{\tepsilon}-\tenseur e^r$. Of course, the term $P\left(\overline{\tepsilon},\tsigma^r\right)$ in previous expression corresponds to the effective energy of a linear thermoelastic composite. The maximization leads to a linear problem, so that the average on phase $r$ of the solution $\tsigma$ is given by
  \begin{aligned}
  \langle\tsigma\rangle_r = \tenseurq A_r\left(\tsigma^1,...,\tsigma^N\right)\dbldot\overline{\tepsilon} + \sum_s \tenseurq B_{rs}\left(\tsigma^1,...,\tsigma^N\right)\dbldot\tenseur e^s\left(\tsigma^s\right)
  \end{aligned}
  where $\tenseurq A_r\left(\tsigma^1,...,\tsigma^N\right)$ and $\tenseurq B_{rs}\left(\tsigma^1,...,\tsigma^N\right)$ can be obtained by a homogenization procedure (mean-field scheme, FFT...).
  
The last question is the choice of the reference stresses $\tsigma^r$ ($1\leq r\leq N$). A discussion in [@castaneda_exact_1996] leads to the simple assumption that these reference stresses are the averages of the stress in the thermoelastic composite:
\begin{aligned}
\tsigma^r = \langle\tsigma\rangle_r
\end{aligned}
  where $\tsigma$ here stands for the stress solution of the thermoelastic problem mentioned above.
  
## Macroscopic stress and tangent operator

The above maximization on the set of statically admissible (divergence-free) fields leads to the condition that the field $\tenseur d^r-\tenseurq M_r\dbldot\tsigma$ belongs to $\Ka\left(\tenseur 0\right)$. Hence, by multiplying this field by $\tsigma$, which is divergence-free, and integrating over the entire volume, we obtain (apply Hill's lemma):
\begin{aligned}
\sum_r c_r\,\langle\left(\tenseur d^r-\tenseurq M_r\dbldot\tsigma\right)\dbldot\tsigma\rangle_r=0
\end{aligned}
which leads to
\begin{aligned}
\sum_r c_r\, \langle \tenseur d^r\dbldot\tsigma-\dfrac12\tsigma\dbldot\tenseurq M_r\dbldot\tsigma\rangle_r=\dfrac12\sum_r c_r\,  \tenseur d^r\dbldot\langle\tsigma\rangle_r=\dfrac12\sum_r c_r\,  \tenseur d^r\dbldot\tsigma^r
\end{aligned}

Hence, using the expressions of $v_r$, $\tenseur e^r$ and $\tenseur d^r$ we obtain (see also expression (B.7) in [@bornert_second-order_2001])
\begin{aligned}
    \overline{\phi}\left(\overline{\tepsilon}\right)\approx\sum_r c_r\,\left[\dfrac12\tsigma^r\dbldot\left(\tepsilon_r+\overline{\tepsilon}\right)-\psi_r\left(\tsigma^r\right)\right]
  \end{aligned}
  The macroscopic stress hence can be obtained by derivation w.r.t. $\overline{\tepsilon}$:
  \begin{aligned}
\overline{\tsigma}\approx\dfrac12\sum_r c_r\,\left[\tsigma^r+\left(\overline{\tepsilon}-\tepsilon_r+\tsigma^r\dbldot\tenseurq M_r\right)\dbldot\deriv{\tsigma^r}{\overline{\tepsilon}}\right]
  \end{aligned}
  
  The tangent operator is obtained by derivation:
  \begin{aligned}
    \dfrac{\mathrm{d}\overline{\tsigma}}{\mathrm{d}\overline{\tepsilon}}\approx\sum_r c_r\,\left\{\deriv{\tsigma^r}{\overline{\tepsilon}}+\dfrac12\left[\left(\deriv{\tsigma^r}{\overline{\tepsilon}}\right)^T\dbldot\left(\tsigma^r\dbldot\tenseurt O_r\right)\dbldot\deriv{\tsigma^r}{\overline{\tepsilon}}+\left(\overline{\tepsilon}-\tepsilon_r+\tsigma^r\dbldot\tenseurq M_r\right)\dbldot\derivdeux{\tsigma^r}{\overline{\tepsilon}}\right]\right\}
  \end{aligned}
  where $\tenseurt O_r=\deriv{\tenseurq M_r}{\tsigma^r}$.

In the expressions of the macroscopic stress and tangent operator, we can compute the term $\deriv{\tsigma^r}{\overline{\tepsilon}}$ by means of the jacobian matrix, also done and explained in the documentation of the [Implicit DSL](https://thelfer.github.io/tfel/web/implicit-dsl.html#computation-of-the-consistent-tangent-operator). See below for more details.

# Summary and possible implementations

## Summary

The resolution consists in
\[
 \text{Find}\quad\tsigma^r\quad\text{such that}\quad\tsigma^r=\langle\tsigma\rangle_r
\]
where \(\tsigma\) is the stress field solution of a homogenization problem of the form:
\begin{aligned}
&\div\,\tsigma=\tenseur 0\\
&\left(\tenseur e^r+\tenseurq M_r\dbldot\tsigma\right)\in\Ka\left(\overline{\tepsilon}\right)\\
&\tenseurq M_r=\derivdeux{\psi_r}{\tsigma}\left(\tsigma^r\right)\qquad\text{and}\qquad\tenseur e^r=\deriv{\psi_r}{\tsigma}\left(\tsigma^r\right)-\tenseurq M_r\dbldot\tsigma^r
\end{aligned}

 
## Possible implementations

The iterative resolution of the non-linear system can be summarized as

\[
\underset{\substack{\\ \\ \\ \Downarrow\\ \\ \\\text{analytic / finite difference}\\ \\ \\\text{\texttt{@BehaviourVariable} / directly provided}}}{\tenseurq M_r= \pderivdeux{\psi_r}{\tsigma}\left(\tsigma^r\right),\quad\tenseur e^r=\deriv{\psi_r}{\tsigma}\left(\tsigma^r\right)-\tenseurq M_r\dbldot\tsigma^r}\qquad\rightarrow\qquad \underset{\substack{\\ \\ \\ \Downarrow\\ \\ \\\text{mean-field scheme / morphological tensors}\\ \\ \\\texttt{tfel::material::homogenization}}}{\tsigma^r = \tenseurq A_r\dbldot\overline{\tepsilon} + \sum_s \tenseurq B_{rs}\dbldot\tenseur e^s}
\]

The first step of the resolution consists in computing the moduli $\tenseurq M_r$ and free strains $\tenseur e^r$. It can be done analytically (as below) or via finite difference or automatic differentiation. Moreover, for both strategies, we could use a `BehaviourVariable`, defining the local behaviour in an external file (in our example, the potential is to simple to do that).

The second step is the computation of the tensors $\tenseurq A_r$ and $\tenseurq B_{rs}$. This can be achieved by means of mean-field schemes (the `namespace` `tfel::material::homogenization` provides some localisation tensors). Another approach is to use morphological tensors. The two approaches are shown below in the next subsection.

We note that the equation relative to the second step is the non-linear equation to solve and will hence correspond to our residue.
The macroscopic stress and the tangent operator must also be computed with the formulas shown above.

## Computation of tensors Ar and Brs

### Computation via morphological tensors

In fact, the thermoelastic problem can be rewritten as a Lippmann-Schwinger equation (see [@willis_bounds_1977;@castaneda_effect_1995] for the primal version). Let us write $\tenseur\tau=\tsigma-\tenseurq L_0\dbldot\tepsilon$ with $\tepsilon=\tenseur e+\tenseurq M\dbldot\tsigma\in\Ka\left(\overline{\tepsilon}\right)$, we have, because, $\div\,\tsigma=0$,
\begin{aligned}
\tepsilon=\overline{\tepsilon}-\tenseurq \Gamma_0\left(\tenseur\tau\right)
\end{aligned}
where $\tenseurq \Gamma_0$ is the Green operator associated to the elasticity $\tenseurq L_0$ (this elasticity must be chosen by the user). It gives
\begin{aligned}
\tsigma=\tenseurq L_0\dbldot\overline{\tepsilon}-\tenseurq L_0\dbldot\tenseurq \Gamma_0\left(\tenseur\tau\right)+\tenseur\tau
\end{aligned}
and hence (due to the expressions of $\tenseur\tau$ and $\tepsilon$)
\begin{aligned}
\tsigma=\tenseurq L_0\dbldot\overline{\tepsilon}-\tenseurq L_0\dbldot\tenseurq \Gamma_0\left(\tsigma-\tenseurq L_0\dbldot\left(\tenseur e^r+\tenseurq M_r\dbldot\tsigma\right)\right)+\tsigma-\tenseurq L_0\dbldot\left(\tenseur e^r+\tenseurq M_r\dbldot\tsigma\right)
\end{aligned}

\begin{aligned}
\tsigma+\tenseurq \Delta_0\left(\tenseurq\delta\tenseurq M_r\dbldot\tsigma\right)=\tenseurq L_0\dbldot\overline{\tepsilon}-\tenseurq \Delta_0\left(\tenseur e^r\right)
\end{aligned}
with $\tenseurq\delta\tenseurq M_r=\tenseurq M_r-\tenseurq M_0$, $\tenseurq M_0=\tenseurq L_0^{-1}$, and $\tenseurq \Delta_0=\tenseurq L_0-\tenseurq L_0\dbldot\tenseurq \Gamma_0\tenseurq L_0$. We will not resolve this equation exactly, but we consider its variational formulation and consider an approximation of the stress field, piece-wise uniform. It is exactly equivalent to apply the dual version of Hashin-Shtrikman principle.
\begin{aligned}
\langle\tsigma\rangle_r + \sum_s\tenseurq \Delta_{rs}\dbldot\tenseurq\delta\tenseurq M_s\dbldot\langle\tsigma\rangle_s= \tenseurq L_0\dbldot\overline{\tepsilon} -\sum_s\tenseurq \Delta_{rs}\dbldot\tenseur e^s
\end{aligned}
where
\begin{aligned}
\tenseurq \Delta_{rs} = \sum_j\langle\,\tenseurq\Delta_0(\chi_s\,\tenseur s_j)\otimes\tenseur s_j\,\rangle_r
\end{aligned}
is what we call a morphological tensor or an interaction tensor, and can be computed by FFT or FEM before the `mfront` integration. This tensor makes intervene the Green operator $\tenseurq \Gamma_0$, relative to the elasticity $\tenseurq L_0$, and a basis of symmetric second-order tensors $(\tenseur s_1,...,\tenseur s_d)$ ($d=6$ in 3D).
Hence, the $\langle\tsigma\rangle_r$ 's are solution of the following linear system:
\begin{aligned}
\sum_{s=1}^N\,\left[\delta_{rs}\,\tenseurq I + \tenseurq \Delta_{rs}\dbldot\tenseurq\delta\tenseurq M_s\right]\dbldot\langle\tsigma\rangle_s=\tenseurq L_0\dbldot\overline{\tepsilon} -\sum_s\tenseurq \Delta_{rs}\dbldot\tenseur e^s
\end{aligned}
where $\delta_{rs}$ is Krönecker symbol and $\tenseurq I$ is the fourth-order identity tensor.
Depending on the number of phases $N$, the solution of this system is more or less costly. The solution directly gives
the tensors $\tenseurq A_r$ and $\tenseurq B_{rs}$ (compare with the expressions above).

### Computation via a self-consistent scheme

We restart here with the equation
\begin{aligned}
\tepsilon=\overline{\tepsilon}-\tenseurq \Gamma_0\left(\tenseur\tau\right)
\end{aligned}
which can be discretized (considering again piece-wise uniforms fields and a variational framework) as
\begin{aligned}
\langle\tepsilon\rangle_r-\overline{\tepsilon}=-\sum_s\tenseurq \Gamma_{rs}\dbldot\langle\tenseur\tau\rangle_s
\end{aligned}
with $\tenseurq\Gamma_{rs}=\sum_j\langle\,\tenseurq\Gamma_0(\chi_s\,\tenseur s_j)\otimes\tenseur s_j\,\rangle_r$.
A classical approximation (see Eq. (3.17) of [@castaneda_effect_1995]) is to consider that $\tenseurq\Gamma_{rs}=\delta_{rs}\tenseurq P_r-c_s\,\tenseurq P_0$, with $\tenseurq P_r=\tenseurq\Gamma_{rr}$ the Hill tensor associated to $\tenseurq L_0$ and the inclusion shape of phase $r$, and $\tenseurq P_0$ the Hill tensor associated to $\tenseurq L_0$ and a spherical inclusion. This is when considering an isotropic distribution of inclusions (otherwise $\tenseurq P_0$ must be modified). For our polycrystal, we can also consider that $\tenseurq P_r=\tenseurq P_0$ so that
\begin{aligned}
\langle\tepsilon\rangle_r-\overline{\tepsilon}=-\tenseurq P_0\dbldot\langle\tenseur\tau\rangle_r+\tenseurq P_0\dbldot\langle\tenseur\tau\rangle
\end{aligned}
and due to the expression of $\tenseur\tau$:
\begin{aligned}
\left[\tenseurq P_0^{-1}-\tenseurq L_0\right]\dbldot\left(\langle\tepsilon\rangle_r-\overline{\tepsilon}\right)=-\left[\langle\tsigma\rangle_r-\langle\tsigma\rangle\right]
\end{aligned}
and noting $\tenseurq M_0^*=\left[\tenseurq P_0^{-1}-\tenseurq L_0\right]^{-1}$, with $\tepsilon=\tenseurq M\dbldot\tsigma+\tenseur e$, and $\langle\tsigma\rangle_r=\tsigma^r$
\begin{aligned}
\left(\tenseurq M_r\dbldot\tsigma^r+\tenseur e^r-\overline{\tepsilon}\right)=-\tenseurq M_0^*\dbldot\left[\tsigma^r-\langle\tsigma\rangle\right]
\end{aligned}
which is
\begin{aligned}
\tsigma^r=\left(\tenseurq M_r+\tenseurq M_0^*\right)^{-1}\dbldot\left[\overline{\tepsilon}-\tenseur e^r+\tenseurq M_0^*\dbldot\langle\tsigma\rangle\right]
\end{aligned}
and by taking the average
\begin{aligned}
\langle\tsigma\rangle=\langle\left(\tenseurq M+\tenseurq M_0^*\right)^{-1}\rangle\dbldot\left[\overline{\tepsilon}+\tenseurq M_0^*\dbldot\langle\tsigma\rangle\right]-\langle\left(\tenseurq M+\tenseurq M_0^*\right)^{-1}\dbldot\tenseur e\rangle
\end{aligned}
and then
\begin{aligned}
\langle\tsigma\rangle=\left[\tenseurq I-\langle\left(\tenseurq M+\tenseurq M_0^*\right)^{-1}\rangle\dbldot\tenseurq M_0^*\right]^{-1}\dbldot\left[\langle\left(\tenseurq M+\tenseurq M_0^*\right)^{-1}\rangle\dbldot\overline{\tepsilon}-\langle\left(\tenseurq M+\tenseurq M_0^*\right)^{-1}\dbldot\tenseur e\rangle\right]
\end{aligned}
Now, the self-consistent approach consists in choosing $\tenseurq M_0$ such that
\begin{aligned}
\tenseurq M_0=\left\{\left[\tenseurq I-\langle\left(\tenseurq M+\tenseurq M_0^*\right)^{-1}\rangle\dbldot\tenseurq M_0^*\right]^{-1}\dbldot\langle\left(\tenseurq M+\tenseurq M_0^*\right)^{-1}\rangle\right\}^{-1}
\end{aligned}
This is also
\begin{aligned}
\tenseurq M_0=\langle\left(\tenseurq M+\tenseurq M_0^*\right)^{-1}\rangle^{-1}-\tenseurq M_0^*
\end{aligned}
This equation can be solved by an iterative procedure, and this is exactly what does the function `computeSelfConsistent`
from the library `tfel::material::homogenization`.
Afterwards, noting $\tenseurq M_0=\tenseurq M_{\mathrm{SC}}$, we have
\begin{aligned}
\langle\tsigma\rangle=\tenseurq M_{\mathrm{SC}}^{-1}\dbldot\overline{\tepsilon}-\tenseurq M_{\mathrm{SC}}^{-1}\dbldot\left[\langle\left(\tenseurq M+\tenseurq M_{\mathrm{SC}}^*\right)^{-1}\rangle\right]^{-1}\dbldot\langle\left(\tenseurq M+\tenseurq M_{\mathrm{SC}}^*\right)^{-1}\dbldot\tenseur e\rangle
\end{aligned}
which is, because $\tenseurq M_{\mathrm{SC}}+\tenseurq M_{\mathrm{SC}}^*=\langle\left(\tenseurq M+\tenseurq M_{\mathrm{SC}}^*\right)^{-1}\rangle^{-1}$,
\begin{aligned}
\langle\tsigma\rangle=\tenseurq M_{\mathrm{SC}}^{-1}\dbldot\overline{\tepsilon}-\left[\tenseurq I+\tenseurq M_{\mathrm{SC}}^{-1}\dbldot\tenseurq M_{\mathrm{SC}}^*\right]\dbldot\langle\left(\tenseurq M+\tenseurq M_{\mathrm{SC}}^*\right)^{-1}\dbldot\tenseur e\rangle
\end{aligned}
and then
\begin{aligned}
&\tenseurq A_r=\left(\tenseurq M_r+\tenseurq M_{\mathrm{SC}}^*\right)^{-1}\dbldot\left(\tenseurq I + \tenseurq M_{\mathrm{SC}}^*\dbldot\tenseurq M_{\mathrm{SC}}^{-1}\right)\\
&\tenseurq B_{rs}=-\left(\tenseurq M_r+\tenseurq M_{\mathrm{SC}}^*\right)^{-1}\dbldot\left(\delta_{rs}\tenseurq I +\tenseurq M_{\mathrm{SC}}^*\dbldot{}^t\tenseurq A_s\right)
\end{aligned}
In fact, because $\langle\tepsilon\rangle_r = \tenseurq M_r\dbldot\tsigma^r + \tenseur e^r$, we have also
\begin{aligned}
\langle\tepsilon\rangle_r = \mathcal A_r\dbldot\overline{\tepsilon}+ \tenseur e^r +\sum_s \tenseurq M_r\dbldot\tenseurq B_{rs}\dbldot\tenseur e^s 
\end{aligned}
where $\mathcal A_r=\tenseurq M_r\dbldot\tenseurq A_r$ is the strain localisation tensor,
also provided by the function `computeSelfConsistent` (see below for details).

# Implementation in MFront

All the files are available in the [`MFrontGallery`](https://github.com/thelfer/MFrontGallery/tree/master/generic-behaviours/homogenization) project. We also provide in [this archive](./downloads/PonteCastaneda1996.zip)
a `python` script to generate the morphological tensors.

ARCHIVE  AND MFRONTGALLERY !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

## Example used for the implementation

  The geometry of our polycrystal is generated with [`merope`](https://github.com/MarcJos/Merope/),
  with a Voronoi tesselation.
  This geometry is saved as an array with an integer representing the number of the phase (or crystal).
  The image below shows an example of such a polycrystal. There are here 10 crystals. The orientations
  and volume fractions are given in the file `polycrystal.csv`. The geometry is used to
  compute the morphological tensors (see the `python` script).
  
  ![Geometry of the polycrystal](./img/polycrystal.png){width=50%}
  
  The potential $\psi_k^r$ will be taken of the form
  \begin{aligned}
  \psi_k^r \left(\tau\right)= \dfrac{\dot{\gamma}_0\tau_0^{k,r}}{n+1}\left(\dfrac{|\tau|}{\tau_0^{k,r}}\right)^{n+1}
  \end{aligned}
  where the strain rate $\dot{\gamma}_0$ and the creep exponent $n\geq 1$ will be chosen identical for all $k,r$, and the resolved shear stress $\tau_0^{k,r}$ depends on $k,r$.

The derivatives of the potential are
 \begin{aligned}
  &\deriv{\psi_r}{\tsigma}=\sum_k \deriv{\psi_k^r}{\tau}\left(\tau_k^r\right)\,\tenseur\mu_k^r=\sum_k\mathrm{sgn}(\tau)\,\dot{\gamma}_0\left(\dfrac{|\tau|}{\tau_0^{k,r}}\right)^{n}\tenseur\mu_k^r\\
  &\derivdeux{\psi_r}{\tsigma}=\sum_k \derivdeux{\psi_k^r}{\tau}\left(\tau_k^r\right)\,\tenseurq N_k^r=\sum_k \dfrac{n\,\dot{\gamma}_0}{\tau_0^{k,r}}\left(\dfrac{|\tau|}{\tau_0^{k,r}}\right)^{n-1}\tenseurq N_k^r,\qquad\text{with}\quad\tenseurq N_k^r=\tenseur \mu_k^r\otimes\tenseur \mu_k^r
  \end{aligned}
  and then defining $\overline{\tau}_k^r=\tsigma^r\dbldot\tenseur \mu_k^r$ where $\tsigma^r$ is the reference stress, we have
  \begin{aligned}
  &\tenseurq M_r=\sum_k \dfrac{n\,\dot{\gamma}_0}{\tau_0^{k,r}}\left(\dfrac{|\overline{\tau}_k^r|}{\tau_0^{k,r}}\right)^{n-1}\tenseurq N_k^r\\
  &\tenseur e^r=\sum_k\left(\mathrm{sgn}(\overline{\tau}_k^r)-n\right)\,\dot{\gamma}_0\left(\dfrac{|\overline{\tau}_k^r|}{\tau_0^{k,r}}\right)^{n}\tenseur\mu_k^r
  \end{aligned}

## Details of implementation

### Headers

For the jacobian, we adopt the `Numerical Jacobian`, so that
the beginning of the `mfront` file reads:

~~~~ {#Begin .cpp .numberLines}
@DSL ImplicitII;
@Behaviour PC_SOE_96;
@Author Martin Antoine;
@Date 26 / 01 / 26;
@Description{"Ponte Castaneda second-order estimates for homogenization of a viscoplastic polycrystal, based on Green interaction tensors"};
@UseQt false;
@Algorithm NewtonRaphson_NumericalJacobian;
@PerturbationValueForNumericalJacobianComputation 1e-10;
@Epsilon 1e-12;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Moreover, we need to include a header file which contains our interaction tensors.
This file must be present in a repository `extra-headers/TFEL/Material/`
located at the same place as the `.mfront` file. In our implementation,
we generate a polycrystal and compute the tensors via FFT (see the `python` script).

We will also need to handle the slip systems, and hence include `ExtendedPolyCrystalsSlidingSystems.hxx`.
This header is supposed to be in the same repository as `tensors.hxx`.

~~~~ {#Begin .cpp .numberLines}
@TFELLibraries {"Material"};
@Includes{
#include "../extra-headers/TFEL/Material/tensors.hxx"
#include "../extra-headers/TFEL/Material/ExtendedPolyCrystalsSlidingSystems.hxx"}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Sliding systems

The sliding systems are loaded via an external file (the same
procedure as for the tutorial on the [Berveiller-Zaoui](ExplicitBerveillerZaouiPolyCrystals.html) scheme). 
This is done as follows :

~~~~ {#Begin .cpp .numberLines}
@CrystalStructure HCP;
@SlidingSystems{<1, 1, -2, 0>{1, -1, 0, 0},
                <-2, 1, 1, 3>{1, -1, 0, 1},
                <-2, 1, 1, 0>{0, 0, 0, 1},
                <1, 1, -2, 0>{1, -1, 0, 1}};
                
@Members {
  using PCSlidingSystems =
      ExtendedPolyCrystalsSlidingSystems<Np, PC_SOE_96SlipSystems<real>, real>;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Poperties and variables

We can now define a state variable on each phase, which correspond to
the reference stresses $\tsigma^r$. We also define an auxiliary variable
for $\deriv{\psi}{\tsigma}\left(\tsigma^r\right)$.

~~~~ {#Begin .cpp .numberLines}
@StateVariable Stensor sigma[Np];
sigma.setEntryName("PhaseReferenceStress");
@AuxiliaryStateVariable Stensor dpsi_dsigma[Np];
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Among the different local variables, note the following which
allows to store the morphological tensors:

~~~~ {#Begin .cpp .numberLines}
@LocalVariable std::array<std::array<tfel::math::st2tost2<3u,real>,Np>,Np> DELTA;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note also that `Np` is the number of phases. This number is a constant parameter,
otherwise, the definition of the `LocalVariable DELTA` above would not be possible
like that. However, we want that this parameter can change. Hence, we define it
before like that:

~~~~ {#Begin .cpp .numberLines}
@IntegerConstant Np = @Np@;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

and the parameter will be set at the compilation of the behaviour:

~~~~ {#bash .sh}
mfront --obuild --interface=generic --@Np@=10 PC_SOE_96.mfront
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Tangent modulus and free-strain

There is no difficulty to compute the tangent modulus and free-strain:

~~~~ {#Integrator .cpp .numberLines}
@Integrator {
  using namespace tfel::math;
  Stensor4 M[Np];
  Stensor e[Np];
  for (int r=0;r<Np;r++){
    dpsi_dsigma[r]=Stensor::zero();
    frac[r]=gs.volume_fractions[r];
    for (int k=0;k<Nss;k++){
      const auto Nkr = gs.mus[r][k]^gs.mus[r][k];
      const auto taukr = gs.mus[r][k] | (sigma[r]+dsigma[r]);
      const auto puisn_1 = pow(abs(taukr)/tau0[r], nexp[r]-1);
      const auto fac= nexp[r]*gamma0/tau0[r]*puisn_1;
      M[r]+=fac*Nkr;
      const auto puisn = puisn_1*abs(taukr)/tau0[r];
      const auto sgn= taukr< 0 ? -real(1) : real(1);
      dpsi_dsigma[r]+=sgn*gamma0*puisn*gs.mus[r][k]; 
    }
    e[r]=dpsi_dsigma[r]-M[r]*(sigma[r]+dsigma[r]);
  }
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Morphological tensors

The morphological tensors are computed as follows:

~~~~ {#Integrator .cpp .numberLines}
  tmatrix<6*Np,6*Np,real> MAT;
  tmatrix<6*Np,6*Np,real> G;
  tmatrix<6*Np,6,real> E;
  for (int r=0;r<Np;r++){
    map_derivative<Stensor,Stensor>(E,6*r,0)=L0;
    for (int s=0;s<Np;s++){
      if (true){
        const auto dM = M[r]-M0;
        map_derivative<Stensor,Stensor>(MAT,6*r,6*s) +=DELTA[r][s]*dM;
        map_derivative<Stensor,Stensor>(G,6*r,6*s) -=DELTA[r][s];
      }
    }
    map_derivative<Stensor,Stensor>(MAT,6*r,6*r) +=I;
  }

  TinyMatrixInvert<6*Np,real>::exe(MAT);
  tmatrix<6*Np,6,real> A = MAT*E;
  tmatrix<6*Np,6*Np,real> B = MAT*G;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

### Residues and jacobian

The residues are

~~~~ {#Integrator .cpp .numberLines}
  for (int r=0;r<Np;r++){
    const auto Ar=map_derivative<Stensor,Stensor>(A,6*r,0);
    fsigma[r] = sigma[r]+dsigma[r]-Ar*(eto+deto);
    for (int s=0;s<Np;s++){
      const auto Brs = map_derivative<Stensor,Stensor>(B,6*r,6*s);
      fsigma[r]-=Brs*e[s];
    }
  }
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There is no jacobian because we use the `NumericalJacobian`. Here,
an analytical jacobian would have been tedious (see the expressions
of $\tenseurq A_r$ and $\tenseurq B_{rs}$ !).

### Macroscopic stress

The computation of the macroscopic stress necessitates the computation of $\deriv{\tsigma^r}{\overline{\tepsilon}}$.
To do that, we use a technique widely used in `MFront`.
The idea is to write the non-linear system to solve as:
\begin{aligned}
\tsigma^r - \tenseurq A_r\left(\tsigma^1,...,\tsigma^N\right)\dbldot\overline{\tepsilon} - \sum_s \tenseurq B_{rs}\left(\tsigma^1,...,\tsigma^N\right)\dbldot\tenseur e^s\left(\tsigma^s\right)\equiv \tenseur R_r(\overline{\tepsilon},\tsigma^1,...,\tsigma^N)=0
\end{aligned}
and by derivation of $\tenseur R_r$ w.r.t. $\overline{\tepsilon}$:
\begin{aligned}
-\tenseurq A_r\left(\tsigma^1,...,\tsigma^N\right)+\sum_s\tenseurq J_{rs}\dbldot\deriv{\tsigma^s}{\overline{\tepsilon}}=0
\end{aligned}
where $\tenseurq J_{rs}=\deriv{\tenseur R_r}{\tsigma^s}$ stands for the sub-block $(r,s)$ of the Jacobian. Hence
\begin{aligned}
\deriv{\tsigma^r}{\overline{\tepsilon}}=\sum_s\mathbf {iJ}_{rs}\dbldot\tenseurq A_s\left(\tsigma^1,...,\tsigma^N\right)
\end{aligned}
where $\mathbf {iJ}$ is the inverse of the Jacobian.

The implementation is

~~~~ {#Integrator .cpp .numberLines}
  tmatrix<6*Np,6*Np,real> iJ = 0.000001*ID+jacobian;
  TinyMatrixInvert<6*Np,real>::exe(iJ);
  dsigma_deto=iJ*A;
  sig=Stensor::zero();
  for (int r=0;r<Np;r++){
    const auto dsigma_deto_r = map_derivative<Stensor,Stensor>(dsigma_deto,6*r,0);
    const auto term = M[r]*(sigma[r]+dsigma[r])-dpsi_dsigma[r]+eto+deto;
    const auto term2 = term*dsigma_deto_r+sigma[r]+dsigma[r];
    sig += 0.5*frac[r]*term2;
  }
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Note the use of the matrix `ID` HERE #############

### Tangent operator

As for the tangent operator, due to its complexity, we will only retain the first term in the implementation:
\begin{aligned}
    \dfrac{\mathrm{d}\overline{\tsigma}}{\mathrm{d}\overline{\tepsilon}}\approx\sum_r c_r\,\deriv{\tsigma^r}{\overline{\tepsilon}}
  \end{aligned}
which only requires the computation of $\deriv{\tsigma^r}{\overline{\tepsilon}}$ that
was already computed. Hence, we have

~~~~ {#tangent .cpp .numberLines}
@TangentOperator{
  for (int r=0;r<Np;r++){
    const auto dsigma_deto_r = map_derivative<Stensor,Stensor>(dsigma_deto,6*r,0);
    Dt+=frac[r]*dsigma_deto_r;
  }
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Results

For the tests, we use `mtest`. However, note that the strain we impose
is in fact a strain rate. The idea for us is to keep this strain rate
constant and to make vary the parameters $n^r$ with time.

Indeed, in Ponte-Castaneda's works (but also in Bornert's works and other works),
when all exponents $n_k^r$ are equal to a unique $n$,
the effective stress is shown to be of the form

\begin{aligned}
\overline{\tsigma}_{11}=\tilde{\tau}_0\left(\dfrac{\dot{\tepsilon}_{11}}{\dot{\gamma_0}}\right)^{1/n}
\end{aligned}

VERIFY THE COORDINATES !!!!!!!!!!!!!!!!!!!

which means that for a constant $\dot{\tepsilon}$, where we introduced
an effective resolved shear stress $\tilde{\tau}_0$. Afterwards, these
authors generally plot this $\tilde{\tau}_0$ as a function of the
parameter $n$. Hence, the idea here is to reproduce this
curve by varying $n$ and keeping $\dot{\tepsilon}$ constant.

The `.mtest` file is the following:

~~~~ {#PC_SOE_96 .mtest .numberLines}
@ModellingHypothesis 'Tridimensional';
@Behaviour<Generic> 'src/libBehaviour.so' 'PC_SOE_96';
@ExternalStateVariable 'Temperature' {0 : 1000,1:1000};

@MaterialProperty<function> 'n1' "exp(2*t*2.3026)";
@MaterialProperty<function> 'n2' "exp(2*t*2.3026)";
@MaterialProperty<constant> 'tau1' 20.;
@MaterialProperty<constant> 'tau2' 30.;
@ImposedStrain 'EXX' {0 : -1, 1 : -1};
@ImposedStrain 'EYY' {0 : 0, 1 : 0};
@ImposedStrain 'EZZ' {0 : 0, 1 : 0};
@ImposedStrain 'EXY' {0 : 0, 1 : 0};
@ImposedStrain 'EXZ' {0 : 0, 1 : 0};
@ImposedStrain 'EYZ' {0 : 0, 1 : 0};
@Times {0.,1 in 100};
~~~~~~~~~~~~~~


![Effective resolved shear stress as a function of the parameter $n$](./img/PC_VB_92.png)

FIGURE !!!!!!!!!!!!!!!!!!!!!!!!!

In this figure we plotted the behaviour of a single crystal when it is alone
(to show the heterogeneity), and the two implementations of the second-order estimates
(SOE), with morphological tensors (SOE_T) and with self-consistent scheme (SOE_SC).

<!-- Local IspellDict: english -->
