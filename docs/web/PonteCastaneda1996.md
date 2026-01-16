---
title: Ponte-Castaneda second-order estimates for homogenization of non-linear behaviours
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
\newcommand{\dtot}{{\mathrm{d}}}
\newcommand{\paren}[1]{\left(#1\right)}
\newcommand{\nom}[1]{\textsc{#1}}
\renewcommand{\div}{\mathrm{div}}


We present here an implementation of Ponte-Castaneda second-order estimates [@castaneda_exact_1996].
It deals with the homogenization of a composite whose phases are governed by one potential.
It can be used in non-linear elasticity, but an example was also treated in [@bornert_second-order_2001]
for the case of a polycrystalline material. Here the idea is to show that many implementations
are possible in `MFront`. If the user wants to improve the accuracy of the homogenized response,
he may want to use morphological tensors, for example. Note that it will also increase the cost of
the integration.

This tutorial first recalls the methodology from [@castaneda_exact_1996], and then presents
different kinds of possible implementations. A particular implementation is given in MFront.

# Ponte-Castaneda second-order estimates

## The non-linear homogenization problem

The variational approach used by Ponte-Castaneda is based on one potential $\phi$ which is of the form:
\begin{aligned}
    \phi(\tepsilon)= \sum_{r=1}^{N}\chi_r\,\phi_r (\tepsilon)
  \end{aligned}
where $N$ is the number of phases and $\chi_r$ is characteristic function of phase $r$. The local
potential $\phi_r$ is a non-linear function of the strain $\tepsilon$, but it can
be also a function of the strain rate $\dot{\tepsilon}$.

For isotropic materials, the potential is of the form:
 \begin{aligned}
  \phi_r(\tepsilon)=\Frac92 k_r\, \varepsilon_m^2+f_r (\epsiloneq^2)
  \end{aligned}
  where
\begin{aligned}
    \epsiloneq= \sqrt{\Frac23\tepsilon^d\dbldot\tepsilon^d}\qquad\text{where}\quad \tepsilon^d = \tepsilon - \Frac13 \trace{\tepsilon}\tenseur{1} \qquad\text{and}\quad \varepsilon_m=\Frac13 \trace{\tepsilon}
  \end{aligned}
 
For polycrystalline materials, the potential is of the form:

...


In the variational approach, the homogenization problem is equivalent to a minimization problem:
\begin{aligned}
    \overline{\phi}\left(\overline{\tepsilon}\right)=\underset{\tepsilon\in\mathcal K(\overline{\tepsilon})}{\textrm{min}} \langle \phi(\tepsilon)\rangle
  \end{aligned}
  where the space of kinematically admissible fields $\mathcal K(\overline{\tepsilon})$ is introduced depending on the boundary conditions used, and it makes intervene the loading $\overline{\tepsilon}$ (which will be our total strain $\texttt{eto}$). The $\langle.\rangle$ denotes the average on the representative volume element. Besides, $\Phi^{\mathrm{eff}}$ is the effective energy.
  
## The second-order estimates

Ponte-Castaneda's idea is to make a second-order Taylor development around a given "reference" strain $\tepsilon^r$ in phase $r$:
\begin{aligned}
    \phi_r(\tepsilon)\approx\phi_r(\tepsilon^r)+\tsigma_r(\tepsilon^r)\dbldot\left(\tepsilon-\tepsilon^r\right)+\dfrac12\left(\tepsilon-\tepsilon^r\right)\dbldot\tenseurq L_r\left(\tepsilon^r\right)\dbldot\left(\tepsilon-\tepsilon^r\right)
  \end{aligned}
  where
  \begin{aligned}
    \tsigma_r(\tepsilon^r)=\deriv{\phi_r}{\tepsilon}\left(\tepsilon^r\right)\qquad\text{and}\qquad\tenseurq L_r\left(\tepsilon^r\right)=\derivdeux{\phi_r}{\tepsilon}\left(\tepsilon^r\right)
  \end{aligned}
  give a "tangent" estimation of the stress $\tsigma_r$ around $\tepsilon^r$:
    \begin{aligned}
    \tsigma_r(\tepsilon)\approx\tsigma_r(\tepsilon^r)+\tenseurq L_r\left(\tepsilon^r\right)\dbldot\left(\tepsilon-\tepsilon^r\right)
  \end{aligned}
  and can be viewed as the behaviour of a so-called "thermoelastic comparison composite".
  
  Hence, by noting $\tepsilon=\overline{\tepsilon}+\tepsilon'$, where $\tepsilon'$ is the fluctuating part of $\tepsilon$, we obtain
  \begin{aligned}
    \phi_r(\tepsilon)\approx v_r\left(\overline{\tepsilon}\right)+\tenseur\tau^r\dbldot\tepsilon'+\dfrac12\tepsilon'\dbldot\tenseurq L_r\left(\tepsilon^r\right)\dbldot\tepsilon'
  \end{aligned}
  with
  \begin{aligned}
    &v_r\left(\overline{\tepsilon}\right)=\phi_r(\tepsilon^r)+\tsigma_r(\tepsilon^r)\dbldot\left(\overline{\tepsilon}-\tepsilon^r\right)+\dfrac12\left(\overline{\tepsilon}-\tepsilon^r\right)\dbldot\tenseurq L_r\left(\tepsilon^r\right)\dbldot\left(\overline{\tepsilon}-\tepsilon^r\right)\\
    &\tenseur\tau^r=\tsigma_r(\tepsilon^r)+\tenseurq L_r\left(\tepsilon^r\right)\dbldot\left(\overline{\tepsilon}-\tepsilon^r\right)
  \end{aligned}
  
  After that, we notice that for each $\tepsilon\in\Ka\left(\overline{\tepsilon}\right)$ corresponds a unique $\tepsilon'$ in $\Ka\left(\tenseur 0\right)$, so that the effective energy $\overline{\phi}$ is estimated by
  \begin{aligned}
    \overline{\phi}\left(\overline{\tepsilon}\right)\approx\sum_r c_r\,v_r\left(\overline{\tepsilon}\right)+\underset{\tepsilon'\in\mathcal K(0)}{\textrm{min}}\sum_r c_r\, \langle \tenseur\tau^r\dbldot\tepsilon'+\dfrac12\tepsilon'\dbldot\tenseurq L_r\left(\tepsilon^r\right)\dbldot\tepsilon'\rangle_r
  \end{aligned}
  where $c_r$ is the volume fraction of the phase $r$, and $\langle.\rangle_r$ denotes the average on phase $r$. Of course, the last term in previous expression corresponds to the effective energy of a linear thermoelastic composite. This effective energy is also shown to be (see Eq. (5.32) of [@ponte_castaneda_nonlinear_1998]):
  \begin{aligned}
  \overline{\phi}\left(\overline{\tepsilon}\right)\approx\sum_r c_r\,\left[\phi_r\left(\tepsilon^r\right)+\dfrac12 \tsigma_r\left(\tepsilon^r\right)\dbldot\left(\overline{\tepsilon}-\tepsilon^r\right)\right]
  \end{aligned}
  
The last question is the choice of the reference strain $\tepsilon^r$. A discussion in [@castaneda_exact_1996] leads to the simple expression:
\begin{aligned}
\tepsilon^r = \overline{\tepsilon}+\langle\tepsilon'\rangle_r
\end{aligned}
  where $\tepsilon'$ here stands for the solution of the minimization problem above, that is, the strain field solution of the homogenization of the thermoelastic composite. This solution can be estimated by a mean-field homogenization scheme (or another more accurate method).
  
## Macroscopic stress and tangent operator

The macroscopic stress $\overline{\tsigma}$ can be obtained by derivation of the estimated effective energy. It gives (see Eq. (5.30) of [@ponte_castaneda_nonlinear_1998]):
  \begin{aligned}
    \overline{\tsigma}\approx\sum_r c_r\,\left[\tsigma_r\left(\tepsilon^r\right)+\dfrac12 \langle\left(\tepsilon-\tepsilon^r\right)\dbldot\tenseurt N_r\dbldot\left(\tepsilon-\tepsilon^r\right)\rangle_r\dbldot\deriv{\tepsilon^r}{\overline{\tepsilon}}\right]
  \end{aligned}
where
\begin{aligned}
\tenseurt N_r = \deriv{\tenseurq L_r}{\tepsilon}\left(\tepsilon^r\right)
\end{aligned}

The tangent operator is given by
\begin{aligned}
    \dfrac{\mathrm{d}\overline{\tsigma}}{\mathrm{d}\overline{\tepsilon}}=\tenseurq C_0^{\mathrm{eff}}+\dfrac{\mathrm{d}\tenseurq C_0^{\mathrm{eff}}}{\mathrm{d}\overline{\tepsilon}}
  \end{aligned}
  Here, the derivative of $\tenseurq C_0^{\mathrm{eff}}$ w.r.t. $\overline{\tepsilon}$ can be computed by derivating the Hashin-Shtrikman
  moduli w.r.t. the secant moduli $\mu_0^r$ and the derivatives of these moduli w.r.t. $\overline{\tepsilon}$. However, it is tedious and in the implementation, we see that the convergence of the Newton Raphson algorithm is good if we only retain the first term \(\tenseurq C_0^{\mathrm{eff}}\) in the tangent operator.


## Summary

The resolution consists in
\[
 \text{Find}\quad\tepsilon^r=\overline{\tepsilon}+\langle\tepsilon'\rangle_r
\]
where \(\tepsilon'\) is the strain field solution of a homogenization problem of the form:
\begin{aligned}
&\div\left[ \tenseurq L_r\left(\tepsilon^r\right)\dbldot\tepsilon'+\tenseur\tau^r\right]=0,\qquad\tepsilon'\in\Ka\left(\tenseur 0\right)\\
&\tenseurq L_r\left(\tepsilon^r\right)=\derivdeux{\phi_r}{\tepsilon}\left(\tepsilon^r\right)\qquad\text{and}\qquad\tenseur\tau^r=\deriv{\phi_r}{\tepsilon}\left(\tepsilon^r\right)+\tenseurq L_r\left(\tepsilon^r\right)\dbldot\left(\overline{\tepsilon}-\tepsilon^r\right)
\end{aligned}
 
 
# Possible implementations



# Implementation in MFront

In the application of the implementation, we take the example of the following behaviour:
  \begin{aligned}
    w_r(\tepsilon)=\dfrac{9}{2}k_r\,\varepsilon_{m}^2+\dfrac{\sigma_r^0}{n+1}\,\varepsilon_{eq}^{n+1}
  \end{aligned}
  where $n$ is between $0$ and $1$, which means that $f_r(e)=\dfrac{\sigma_r^0}{n+1}\,e^{\frac{n+1}2}$ (note that the function
  $f_r$ is effectively concave relatively to $e$, because $0\lt n\leq 1$).
  
  Moreover, we make the choice of purely elastic inclusions. Hence, only the matrix is non-linear and has a
  secant modulus.
  
The unknowns of our non-linear problem are the $e^r$. The secant moduli $\mu_0^r$ can be computed by derivation of the function $f_r$ which is given by the behaviour. In the precise case detailed above, we have
\begin{aligned}
\deriv{f_r}{e}(e)=\dfrac{\sigma_r^0}{2}\,e^{\frac{n-1}2}\qquad\text{and}\quad\mu_0^r=\dfrac{\sigma_r^0}{3}\,\left(e^r\right)^{\frac{n-1}2}
\end{aligned}
Then, the effective energy of the linear comparison composite can be computed via a mean-field scheme. In our implementation, we proposed to use the Hashin-Shtrikman bound. The $e^r$ must then cancel the following residue:
\begin{aligned}
r_{e^r}=e^r - \Frac2{3\,c_r}\deriv{W_0^{\mathrm{eff}}}{\mu_0^r}
\end{aligned}
which means that we only need the derivative of the Hashin-Shtrikman bound. This will be given by the `tfel::material::homogenization` functionalities.

## Details of implementation

All the files are available [here](./downloads/PonteCastaneda1992.zip)

For the jacobian, we adopt the `Numerical Jacobian`, which
means that the beginning of the `mfront` file reads:

~~~~ {#Begin .cpp .numberLines}
@DSL ImplicitII;
@Behaviour PC_VB_92;
@Author Martin Antoine;
@Date 12 / 12 / 25;
@Description{"Ponte Castaneda variational bounds for homogenization of non-linear elasticity (one potential), based on second-moments computation"};
@UseQt true;
@Algorithm NewtonRaphson_NumericalJacobian;
@PerturbationValueForNumericalJacobianComputation 1e-10;
@Epsilon 1e-12;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Moreover, the following libraries will be needed for computation
of the stress and secant modulus:

~~~~ {#Begin .cpp .numberLines}
@TFELLibraries {"Material"};
@Includes{
#include "TFEL/Material/IsotropicModuli.hxx"
#include "TFEL/Material/HomogenizationSecondMoments.hxx"
#include "TFEL/Material/LinearHomogenizationBounds.hxx"
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We define two state variables which are the averages (on each phase) of the squares
of the equivalent strains, and correspond to the $e^r$:

~~~~ {#Begin .cpp .numberLines}
@StateVariable real e_0;
e_0.setEntryName("MatrixSquaredEquivalentStrain");
@StateVariable real e_r;
e_r.setEntryName("InclusionSquaredEquivalentStrain");
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The secant moduli are computed by derivation of function $f_r$, which is detailed above. Hence

~~~~ {#Integrator .cpp .numberLines}
@Integrator {
//secant modulus/////////////////////////////////
const auto po = std::pow(max(e_0+de_0,real(1e-10)),0.5*(n_-1));
const auto mu0 = sig0/3.*po;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The second moment is given by the function `computeMeanSquaredEquivalentStrain`
(see [documentation](tfel-material.html#second-moments-of-the-strains)):

~~~~ {#Integrator .cpp .numberLines}
//second moments/////////////////////////////////
const auto em2 = tfel::math::trace(eto+deto)/3.;
const auto ed = tfel::math::deviator(eto+deto);
const auto eeq2 = 2./3.*(ed|ed);
using namespace tfel::material;
const auto kg0 = KGModuli<stress>(k_m,mu0);
const auto kgr = KGModuli<stress>(kar,mur);
using namespace tfel::material::homogenization::elasticity;
const auto eeq2_ = computeMeanSquaredEquivalentStrain(kg0,fr,kgr,em2,eeq2);
const auto eeq20=std::get<0>(eeq2_);
const auto eeq2r=std::get<1>(eeq2_);
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Now we can write the residues:

~~~~ {#Integrator .cpp .numberLines}
//residues/////////////////////////////////////
fe_0 = e_0+de_0 - eeq20;
fe_r = e_r+de_r - eeq2r;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There is no jacobian because we use the `NumericalJacobian`. The macroscopic stress
is given by the Hashin-Shtrikman lower bound (indeed, the lower bound is better suited
for soft matrix and rigid inclusions):

~~~~ {#Integrator .cpp .numberLines}
//sigma//////////////////////////////////////
vector<real> tab_f={1-fr,fr};
vector<stress> tab_k = {k_m,kar};
vector<stress> tab_mu = {mu0,mur};
const auto HSB=computeIsotropicHashinShtrikmanBounds<3,stress>(tab_f,tab_k,tab_mu);
const auto LB=std::get<0>(HSB);
const auto kHS=std::get<0>(LB);
const auto muHS=std::get<1>(LB);
Chom=3*kHS*J+2*muHS*K;
sig = Chom*(eto+deto);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

(Note that `Chom` is the effective elastic tensor, defined here as
a local variable).
Afterwards, we can give the tangent operator:

~~~~ {#tangent .cpp .numberLines}
@TangentOperator{
Dt=Chom;
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Results

We then use `MTest` to simulate a strain-imposed test.

MTest file is the following:

~~~~ {#PC_VB_92 .mtest .numberLines}
@ModellingHypothesis 'Tridimensional';
@Behaviour<Generic> 'src/libBehaviour.so' 'PC_VB_92';
@ExternalStateVariable 'Temperature' {0 : 1000,1:1000};

@MaterialProperty<constant> 'sig0'             1.e9;
@MaterialProperty<constant> 'n_'               0.05;
@MaterialProperty<constant> 'k_m'               1.e9;
@MaterialProperty<constant> 'kar'               1.e15;
@MaterialProperty<constant> 'mur'               1.e15;
@MaterialProperty<constant> 'fr'               0.25;

@ImposedStrain 'EXX' {0 : 0, 100 : 0.8660254};
@ImposedStrain 'EYY' {0 : 0, 100 : -0.8660254};
@ImposedStrain 'EZZ' {0 : 0, 100 : 0};
@ImposedStrain 'EXY' {0 : 0, 100 : 0};
@ImposedStrain 'EXZ' {0 : 0, 100 : 0};
@ImposedStrain 'EYZ' {0 : 0, 100 : 0};
@Times {0.,10 in 100};
~~~~~~~~~~~~~~

The macroscopic stress $\sigma_{xx}$ is represented below as a function
of the macroscopic strain $\varepsilon_{xx}$: 

![Macroscopic stress as function of the macroscopic strain](./img/PC_VB_92.png)

We also plotted the behaviour of the matrix when there is no inclusions (it gives
an idea of the reinforcement given by the inclusions).

<!-- Local IspellDict: english -->
