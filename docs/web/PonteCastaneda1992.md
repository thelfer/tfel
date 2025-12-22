% Ponte-Castaneda variational bound for homogenization in non-linear elasticity
% Martin Antoine
% January, 5 2026

\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tenseurq}[1]{\underline{\mathbf{#1}}}
\newcommand{\tns}[1]{\underset{\tilde{}}{\mathbf{#1}}}
\newcommand{\transpose}[1]{#1^{\mathop{T}}}

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


We present here an implementation of Ponte-Castaneda variational bound in non-linear elasticity [@castaneda_new_1992].
It deals with the homogenization of a composite made of a matrix and spherical inclusions. The two phases have
a non-linear elastic behaviour.
The variational bound from Ponte-Castaneda is based on the computation of
the strain second-moments.

This tutorial first recalls the scheme, and presents the implementation.

# Ponte-Castaneda variational bound

## The non-linear homogenization problem

The variational approach used by Ponte-Castaneda is based on the elastic potential $w$ which is of the form:
\begin{aligned}
    w(\tepsilon)= \sum_{r=1}^{N}\chi_r\,w_r (\tepsilon)
  \end{aligned}
where $N$ is the number of phases and $\chi_r$ is characteristic function of phase $r$.
The local potential $w_r$ is a non-linear function of the strain $\tepsilon$, but more precisely
it is a non-linear function of the `second-moment' of the strain $\te=\Frac 12\tepsilon\otimes\tepsilon$.
Here we focus on potentials of the form
 \begin{aligned}
  w_r(\tepsilon)=\Frac92 k_r\, \varepsilon_m^2+f_r (\epsiloneq^2)
  \end{aligned}
  where
\begin{aligned}
    \epsiloneq= \sqrt{\Frac23\tepsilon^d\dbldot\tepsilon^d}\qquad\text{where}\quad \tepsilon^d = \tepsilon - \Frac13 \trace{\tepsilon}\tenseur{1} \qquad\text{and}\quad \varepsilon_m=\Frac13 \trace{\tepsilon}
  \end{aligned}
 
In the variational approach, the homogenization problem is equivalent to a minimization problem:
\begin{aligned}
    \tepsilon=\underset{\tepsilon\in\mathcal K(\overline{\tepsilon})}{\textrm{argmin}} \langle w(\tepsilon)\rangle
  \end{aligned}
  where the space of kinematically admissible fields $\mathcal K(\overline{\tepsilon})$ is introduced depending on the boundary conditions used, and it makes intervene the loading $\overline{\tepsilon}$ (which will be our total strain $\texttt{eto}$). The $\langle.\rangle$ denotes the average on the representative volume element.
  
## The variational bound

Ponte-Castaneda's idea is to use the dual function $f_r^*$ of $f_r$, under an hypothesis of concavity of $f_r$ (note that $w_r$ is convex relatively to $\tepsilon$, but the hypothesis is: $f_r$ concave relatively to $\epsiloneq$).
\begin{aligned}
    w_r(\tepsilon)=\underset{\mu_0^r(\tenseur x)}{\min} \left\{\Frac92 k_r\, \varepsilon_m^2+\Frac32 \mu_0^r(\tenseur x)\, \epsiloneq^2-f_r^*(\mu_0^r(\tenseur x))\right\}
  \end{aligned}
  where the dual function $f_r^*$ is defined as
  \begin{aligned}
    f_r^*(\mu_0)=\underset{e}{\min} \left\{\mu_0 e-f_r(e)\right\}
  \end{aligned}
  
Considering uniform per phase shear moduli $\mu_0^r$ (also called the 'secant moduli'), and after a few manipulations, Ponte-Castaneda arrives at the following bound on the effective energy $W^{\mathrm{eff}}(\overline{\tepsilon})$:
\begin{aligned}
    W^{\mathrm{eff}}(\overline{\tepsilon})=\underset{\tepsilon\in\mathcal K(\overline{\tepsilon})}{\textrm{min}} \langle w(\tepsilon)\rangle \leq \overline{w}(\overline{\tepsilon})=\underset{\mu_0^r}{\min} \left\{W_0^{\mathrm{eff}}(\overline{\tepsilon})-\sum_r c_r\, f_r^*(\mu_0^r)\right\}
  \end{aligned}
 where $c_r$ is the volume fraction of phase $r$ and $W_0^{\mathrm{eff}}(\overline{\tepsilon})$ is the effective energy relative to a 'linear comparison composite' which is heterogeneous, and whose elastic moduli are the $k_r$ and the $\mu_0^r$.  

  The stationarity conditions associated to the minimization on $\mu_0^r$ are:
  \begin{aligned}
    \deriv{W_0^{\mathrm{eff}}}{\mu_0^r} = c_r\,\deriv{f_r^*}{\mu_0^r}
  \end{aligned}

which can be shown to be equivalent to
\begin{aligned}
    \mu_0^r= \Frac23 \deriv{f_r}{e}\left(\langle \epsiloneq^2\rangle_r\right)
  \end{aligned}
  where $\epsiloneq$ is relative to $\tepsilon$ solution of the homogenization problem:
\begin{aligned}
    \mathrm{div}\left(3k_r\,\varepsilon_m\,\tenseur 1+2\mu_0^r\,\tepsilon^d\right)=0,\qquad\tepsilon\in\mathcal K(\overline{\tepsilon})
  \end{aligned}
  
  Besides, noting that
  \begin{aligned}
    W_0^{\mathrm{eff}}(\overline{\tepsilon})=\sum_r c_r\,\left(\Frac92 k_r\,\langle\varepsilon_m^2\rangle_r+\Frac32 \mu_0^r\langle \epsiloneq^2\rangle_r\right)
  \end{aligned}
  it follows that
\begin{aligned}
    \langle \epsiloneq^2\rangle_r= \Frac{2}{3c_r}\deriv{W_0^{\mathrm{eff}}}{\mu_0^r}
  \end{aligned}
  
## Summary

The resolution hence consists in 
\begin{gather}
 \text{Find}\,e^r=\langle \epsiloneq^2\rangle_r\,\text{such that}\quad\langle \epsiloneq^2\rangle_r= \Frac{2}{3c_r}\deriv{W_0^{\mathrm{eff}}}{\mu_0^r}\quad\text{where}\,W_0^{\mathrm{eff}}\,\text{is the effective energy of a linear comparison composite}\\
 \text{whose elastic moduli are}\,(k_r,\,\mu_0^r)\,\text{such that}\quad\mu_0^r= \Frac23\deriv{f_r}{e}\left(\langle \epsiloneq^2\rangle_r\right).
  \end{gather}

 In the application of the implementation, we take the example of the following behaviour:
  \begin{aligned}
    w_r(\tepsilon)=\dfrac{9}{2}k_r\,\varepsilon_{m}^2+\dfrac{\sigma_r^0}{n+1}\,\varepsilon_{eq}^{n+1}
  \end{aligned}
  where $n$ is between $0$ and $1$.
 
# Implementation in MFront

The unknowns of our non-linear problem are the $e^r$. The secant moduli $\mu_0^r$ can be computed with the local behaviours.
The effective energy of the linear comparison composite can be computed via a mean-field scheme. In our implementation, we proposed
to use the Hashin-Shtrikman bound. The $e^r$ must then cancel the following residue:
\begin{aligned}
r_{e^r}=c_r\,e^r - \deriv{W_0^{\mathrm{eff}}}{\mu_0^r}
\end{aligned}

## Computation of the secant moduli

The secant moduli are computed by derivation of function $f_r$.

~~~~ {#Integrator .cpp .numberLines}
@ProvidesTangentOperator;

@Integrator{
  b1.deto=deto;
  
  constexpr auto b1_smflag = TangentOperatorTraits<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR>::STANDARDTANGENTOPERATOR;
  const auto r1 = b1.integrate(b1_smflag,CONSISTENTTANGENTOPERATOR);
  StiffnessTensor Dt1 = b1.getTangentOperator();
 
  b2.deto=deto;
  
  constexpr auto b2_smflag = TangentOperatorTraits<MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR>::STANDARDTANGENTOPERATOR;
  const auto r2 = b2.integrate(b2_smflag,CONSISTENTTANGENTOPERATOR);
  StiffnessTensor Dt2 = b2.getTangentOperator();
  
  updateAuxiliaryStateVariables(b1);
  updateAuxiliaryStateVariables(b2);

  sig = f * sig1 + (1 - f) * sig2;
  if (computeTangentOperator_) {
    Dt = f * Dt1 + (1 - f) * Dt2;
  }
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


## Computation and derivation of the effective energy of the linear comparison composite

We do not need the effective energy

## Stress and tangent operator

Finally, the computation of macroscopic stress is given
by Taylor scheme (average of local stresses)
at line $19$, and the macroscopic tangent operator
is computed in the sequel.



# Results

We then use `MTest` to simulate a uniaxial tensile test.

MTest file (`Taylor.mtest`) is the following:

~~~~ {#Taylor_mtest .mtest .numberLines}
@ModellingHypothesis 'Tridimensional';
@Behaviour<Generic> 'src/libBehaviour.so' 'Taylor';
@MaterialProperty<constant> 'FirstPhaseYoungModulus'     150.e9;
@MaterialProperty<constant> 'FirstPhasePoissonRatio'       0.35;
@MaterialProperty<constant> 'H1'                50.e9;
@MaterialProperty<constant> 's01'               200.e6;
@MaterialProperty<constant> 'SecondPhaseYoungModulus'     90.e9;
@MaterialProperty<constant> 'SecondPhasePoissonRatio'       0.3;
@MaterialProperty<constant> 'H2'                30.e9;
@MaterialProperty<constant> 's02'               50.e6;
@MaterialProperty<constant> 'FirstPhaseFraction' 0.1;
@ExternalStateVariable 'Temperature' 293.15;
@ImposedStrain 'EXX' {0 : 0, 1 : 3e-3};
@Times {0, 1 in 200};
~~~~~~~~~~~~~~

Macroscopic and local stresses are represented
below as functions of the uniform axial strain:

![Local and macroscopic stresses as function of the uniform axial strain, uniaxial tensile test, Taylor scheme](./img/Taylor_test.png)

We can see, as expected, that the macroscopic stress
is an average of the local stresses. When the lowest yield
stress is reached, the corresponding phase becomes plastic.
The macroscopic tangent module is then reduced, which has repercussions
on the macroscopic stress.
The other phase becomes plastic later. Even if its evolution remains
elastic, we can see that its axial stress is also
impacted by the plastic transition of the other phase, because of the
non-axial plastic strains appeared in this latter phase.

<!-- Local IspellDict: english -->
