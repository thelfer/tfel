% How to implement an isotropic viscoplastic behaviour combining isotropic hardening and  multiple kinematic hardenings following an Armstrong-Frederic evolution of the back stress
% Thomas Helfer
% 27/01/2017

\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}
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
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sderiv}[2]{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial #2^{2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}

This article shows how to implement an isotropic viscoplastic
behaviour combining isotropic hardening and multiple kinematic
hardenings following an Armstrong-Frederic evolution of the back
stress.

Such an example illustrates:

- The usage of `StandardElasticity` brick (see
  [this page](BehaviourBricks.html)).

<!-- The whole implementation is available -->
<!-- [here](./gallery/plasticity/OrthotropicLinearHardeningPlasticity.mfront). -->

# Description of the behaviour

The behaviour is described by a standard split of the strain
\(\tepsilonto\) in an elastic and a plastic parts, respectively
denoted \(\tepsilonel\) and \(\tepsilonvis\):

\[
\tepsilonto=\tepsilonel+\tepsilonvis
\]

## Elastic behaviour

The stress \(\tsigma\) is related to the the elastic strain
\(\tepsilonel\) by a the standard Hooke behaviour:

\[
\tsigma = \lambda\,\trace{\tepsilonel}\,\tenseur{I}+2\,\mu\,\tepsilonel
\]

## Viscoplastic behaviour

The viscoplastic behaviour follows a standard viscoplastic behaviour:
\[
\tdepsilonvis=\paren{\Frac{F}{K}}^{m}\,\tenseur{n}
\]

where \(F\) is the yield surface defined below and \(\tenseur{n}\) is
the normal to \(F\).

The yield surface is defined by:
\[
F\paren{\tsigma,\tenseur{X}_{i},p}=\paren{\tsigma-\sum_{i=1}^{N}\tenseur{X}_{i}}_{\mathrm{eq}}-R\paren{p}
\]

where \(R\paren{p}\) describes the isotropic hardening as a function
of the equivalent viscoplastic strain \(p\) and where the \(N\)
tensors \(\tenseur{X}_{i}\) (i\in\left[1,N\right]) are backstresses
describing the kinematic hardening.

## Evolution of the isotropic hardening

The isotropic hardening is defined by:
\[
R\paren{p}=R_{\infty} + \paren{R_{0}-R_{\infty}}\,\exp\paren{-b\,p}
\]

## Evolution of the kinematic hardenings

\[
\tenseur{X}_{i}=\Frac{2}{3}\,C_{i}\,\tenseur{a}_{i}
\]

The evolution of the kinematic variables \(\tenseur{a}_{i}\) follows
the Armstrong-Frederic rule:

\[
\tenseur{\dot{a}}_{i}=\tdepsilonvis-g[i]\,\tenseur{a}_{i}\,\dot{p}
\]

# Implementation

<!-- ## Plastic flow -->

<!-- The plastic part of the behaviour is described by the following yield -->
<!-- surface: -->
<!-- \[ -->
<!-- f\paren{\sigmaH,p} = \sigmaH-\sigma_{0}-R\,p -->
<!-- \] -->

<!-- where \(\sigmaH\) is the Hill stress defined below, \(p\) is the -->
<!-- equivalent plastic strain. \(\sigma_{0}\) is the yield stress and -->
<!-- \(R\) is the hardening slope. -->

<!-- The Hill stress \(\sigmaH\) is defined using the fourth order Hill -->
<!-- tensor \(H\): -->
<!-- \[ -->
<!-- \sigmaH=\sqrt{\tsigma\,\colon\,\tenseurq{H}\colon\,\tsigma} -->
<!-- \] -->

<!-- The plastic flow is assumed to be associated, so the flow direction -->
<!-- \(\tenseur{n}\) is given by \(\deriv{f}{\tsigma}\): -->

<!-- \[ -->
<!-- \tenseur{n} = \deriv{f}{\tsigma} = \Frac{1}{\sigmaH}\,\tenseurq{H}\,\colon\,\tsigma -->
<!-- \] -->

<!-- # Integration algorithm -->

<!-- The previous constitutive equations will be integrated using a -->
<!-- standard implicit scheme. -->

<!-- ## Plastic loading case -->

<!-- ### Implicit system -->

<!-- Assuming a plastic loading, the system of equations to be solved is: -->
<!-- \[ -->
<!-- \left\{ -->
<!-- \begin{aligned} -->
<!-- 	\Delta\,\tepsilonel-\Delta\,\tepsilonto+\Delta\,p\,\tenseur{n} &= 0 \\ -->
<!-- 	f\paren{\mts{\sigmaH},\mts{p}} &= 0 \\ -->
<!-- \end{aligned} -->
<!-- \right. -->
<!-- \] -->

<!-- where \(\mts{X}\) is the value of \(X\) at \(t+\theta\,\Delta\,t\), -->
<!-- \(\theta\) being a numerical parameter. In the following, the first -->
<!-- (tensorial) equation is noted \(f_{\tepsilonel}\) and the second -->
<!-- (scalar) equation is noted \(f_{p}\). -->

<!-- In practice, it is physically sound to make satisfy exactly the yield -->
<!-- condition at the end of the time step (otherwise, stress extrapolation -->
<!-- can lead to stress state outside the yield surface and spurious -->
<!-- oscillations can also be observed). This leads to the choice -->
<!-- \(\theta=1\). -->

<!-- ### Computation of the jacobian -->

<!-- The jacobian \(J\) of the implicit system can be decomposed by blocks: -->
<!-- \[ -->
<!-- J= -->
<!-- \begin{pmatrix} -->
<!-- \deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel} & \deriv{f_{\tepsilonel}}{\Delta\,p} & \\\\ -->
<!-- \deriv{f_{p}}{\Delta\,\tepsilonel} & \deriv{f_{p}}{\Delta\,p} \\ -->
<!-- \end{pmatrix} -->
<!-- \] -->

<!-- The expression of the previous terms is given by: -->

<!-- \[ -->
<!-- \left\{ -->
<!-- \begin{aligned} -->
<!-- \deriv{f_{\tepsilonel}}{\Delta\,\tepsilonel} &= \tenseur{I} + \Frac{\theta\,dp}{\sigmaH}\,\paren{\tenseurq{H}-\tenseur{n}\,\otimes\,\tenseur{n}}\,\tenseurq{D} \\ -->
<!-- \deriv{f_{\tepsilonel}}{\Delta\,p} &= \tenseur{n}\\ -->
<!-- \deriv{f_{p}}{\Delta\,\tepsilonel} &= -\theta\,\tenseur{n}\,\colon\,\tenseurq{D}\\ -->
<!-- \deriv{f_{p}}{\Delta\,p}           &= -R\,\theta -->
<!-- \end{aligned} -->
<!-- \right. -->
<!-- \] -->

<!-- ## Elastic loading case -->

<!-- Assuming an elastic loading, the system of equations to be solved is -->
<!-- trivially: -->
<!-- \[ -->
<!-- \left\{ -->
<!-- \begin{aligned} -->
<!-- 	\Delta\,\tepsilonel-\Delta\,\tepsilonto &= 0 \\ -->
<!-- 	\Delta\,p &= 0 \\ -->
<!-- \end{aligned} -->
<!-- \right. -->
<!-- \] -->

<!-- The jacobian associated with this system is the identity matrix. -->

<!-- # Implementation -->

<!-- In the plastic loading case, the system of equations to be solved in -->
<!-- *non-linear*. We choose the Newton-Raphson algorithm which an -->
<!-- analytical jacobian. This algorithm is the *most efficient* in -->
<!-- pratice. -->

<!-- ## Preamble -->

<!-- The implementation begins with the choice of the `Implicit` domain -->
<!-- specific language (dsl): -->

<!-- ~~~~{.cpp} -->
<!-- @DSL Implicit; -->
<!-- ~~~~ -->

<!-- Note that this dsl automatically declares the elastic strain `eel` as -->
<!-- a state variable. -->

<!-- As discussed before, we explicit state that that a fully implicit -->
<!-- integration will be used by default: -->

<!-- ~~~~{.cpp} -->
<!-- @Theta 1; -->
<!-- ~~~~ -->

<!-- The choice can be dynamically choice par modifying the `theta` -->
<!-- parameter. -->

<!-- The stopping criterion is chosen low, to ensure the quality of the -->
<!-- consistent tangent operator (the default value, \(10^{-8}\) is enough -->
<!-- to ensure a good estimation of the state variable evolution, but is -->
<!-- not enough to have a proper estimation of the consistent tangent -->
<!-- operator): -->

<!-- ~~~~{.cpp} -->
<!-- @Epsilon 1e-14; -->
<!-- ~~~~ -->

<!-- We then declare the behaviour to be orthotropic. We choose the `pipe` -->
<!-- orthotropic convention to have a consistent definition of the elastic -->
<!-- stiffness tensor and hill tensor for all modelling hypotheses (axes -->
<!-- are automatically permuted for plane modelling hypotheses): -->

<!-- ~~~~{.cpp} -->
<!-- @OrthotropicBehaviour<Pipe>; -->
<!-- ~~~~ -->

<!-- We then declare that we want to support all the modelling hypotheses: -->

<!-- ~~~~{.cpp} -->
<!-- @ModellingHypotheses {".+"}; -->
<!-- ~~~~ -->

<!-- The support of plane stress modelling hypotheses are handled by the -->
<!-- `StandardElasticity` brick and will not be discussed here. -->

<!-- ### Usage of the `StandardElasticity` brick -->

<!-- To implement this behaviour, we will use the `StandardElasticity` -->
<!-- brick which provides: -->

<!-- - Automatic computation of the stress tensor at various stages of the -->
<!--   behaviour integration. -->
<!-- - Automatic computation of the consistent tangent operator. -->
<!-- - Automatic support for plane stress and generalized plane stress -->
<!--   modelling hypotheses (definitions of the axial strain as an -->
<!--   additional state variables and the associated equation enforcing the -->
<!--   plane stess condition). -->
<!-- - Automatic addition of the standard terms associated with the elastic -->
<!--   strain state variables. -->

<!-- This behaviour brick is fully described [here](BehaviourBricks.html). -->

<!-- The usage of the `StandardElasticity` is declared as follows: -->

<!-- ~~~~{.cpp} -->
<!-- @Brick StandardElasticity; -->
<!-- ~~~~ -->

<!-- ### Elastic stiffness tensor -->

<!-- The elastic stiffness tensor \(D\) is defined using -->
<!-- `@ComputeStiffnessTensor` keyword by giving the elastic material -->
<!-- properties as constants: -->

<!-- ~~~~{.cpp} -->
<!-- @ComputeStiffnessTensor<UnAltered> { -->
<!--   // YoungModulus1 YoungModulus2 YoungModulus3 -->
<!--   7.8e+10,2.64233e+11,3.32e+11, -->
<!--   // PoissonRatio12 PoissonRatio23 PoissonRatio13 -->
<!--   0.13,0.24,0.18, -->
<!--   // ShearModulus12 ShearModulus23 ShearModulus13 -->
<!--   4.8e+10,1.16418e+11,7.8e+10 -->
<!-- }; -->
<!-- ~~~~ -->

<!-- This computed stiffness is stored in a variable `D`. A second variable -->
<!-- `D_tdt` is also introduced. As the material properties are constants, -->
<!-- `D_tdt` is an alias to `D`. -->

<!-- The elastic material properties can be changed at runtime time by -->
<!-- modifying the following parameters: `YoungModulus1`, -->
<!-- `YoungModulus2`,`YoungModulus3`, `PoissonRatio12`, `PoissonRatio23`, -->
<!-- `PoissonRatio13`, `ShearModulus12`, `ShearModulus23` and -->
<!-- `ShearModulus13`. -->

<!-- Rather than constants, one can also use correlations implemented in -->
<!-- seperate `MFront` files. This allows to take into account the -->
<!-- dependency of the material properties with the temperature for -->
<!-- example. In this case, the variable `D` contains the stiffness tensor -->
<!-- at \(t+\theta\,\Delta\,t\) and the variable `D_tdt` contains the -->
<!-- stiffness tensor at \(t+\Delta\,t\). -->

<!-- Another possibility is to use the `@RequireStiffnessTensor` -->
<!-- keyword. In this case, the elastic material properties must be -->
<!-- computed by the calling solver at the end of the time step (and -->
<!-- furnished to the mechanical behaviours through hidden material -->
<!-- properties). -->

<!-- ### Hill tensor -->

<!-- For the computation of the Hill tensor, we make use of the -->
<!-- `@HillTensor` keyword: -->

<!-- ~~~~{.cpp} -->
<!-- @HillTensor H {0.371,0.629,4.052,1.5,1.5,1.5}; -->
<!-- ~~~~ -->

<!-- ### Variable declarations -->

<!-- #### State variables -->

<!-- As recalled earlier, the state variable `eel` is automatically -->
<!-- declared by the `Implicit` dsl. -->

<!-- The equivalent plastic strain state variable `p` is declared as: -->

<!-- ~~~~{.cpp} -->
<!-- @StateVariable real p; -->
<!-- ~~~~ -->

<!-- We then associate the appropriate glossary name to this variable: -->

<!-- ~~~~{.cpp} -->
<!-- p.setGlossaryName("EquivalentPlasticStrain"); -->
<!-- ~~~~ -->

<!-- #### Parameters -->

<!-- The definition of yield surface introduce two material coefficients -->
<!-- \(\sigma_{0}\) and \(R\) that we declare as parameters: -->

<!-- ~~~~{.cpp} -->
<!-- @Parameter s0 = 150e6; -->
<!-- s0.setGlossaryName("YieldStress"); -->
<!-- @Parameter R  = 150e9; -->
<!-- R.setEntryName("HardeningSlope"); -->
<!-- ~~~~ -->

<!-- The `YieldStress` is an entry of the glossary (see -->
<!-- [here](glossary.html)). The `HardeningSlope` name is not declared in -->
<!-- the glossary name (yet) and is then associated to the \(R\) variable -->
<!-- with the `setEntryName` methode. -->

<!-- #### Local variable -->

<!-- To select the implicit system associated either with the elastic or -->
<!-- plastic loading case, we introduce a boolean local variable `b`. -->

<!-- ~~~~{.cpp} -->
<!-- @LocalVariable bool b; // if true, plastic loading -->
<!-- ~~~~ -->

<!-- ## Initialisation of the local variables, determination of the loading case -->

<!-- Before solving the implicit system, the code block introduced by the -->
<!-- `@InitLocalVariables` keyword is executed. For this behaviour, this -->
<!-- block will select either the elastic or plastic loading case. -->

<!-- We first make an *elastic prediction* of the stress at the end of the -->
<!-- time step. We use the `computeElasticPrediction` method introduced by -->
<!-- the `StandardElasticity` brick. This method takes into account the -->
<!-- modelling hypothesis, which is mandatory for plane stress modelling -->
<!-- hypotheses. We then make an elastic prediction of the Hill equivalent -->
<!-- stress and check whether or not this elastic prediction is inside the -->
<!-- elastic domain. The latter information is stored in the boolean value -->
<!-- `b` which will be `false` (no plastic loading) if the loading is -->
<!-- elastic. -->

<!-- ~~~~{.cpp} -->
<!-- @InitLocalVariables{ -->
<!--   const auto s   = computeElasticPrediction(); -->
<!--   const auto seq = sqrt(s|H*s); -->
<!--   b = seq-s0-R*p > 0; -->
<!-- } -->
<!-- ~~~~ -->

<!-- ## Implicit system and jacobian -->

<!-- Finally, we describe how the implicit system and the computation of -->
<!-- the jacobian is written in a code block introduced by the -->
<!-- `@Integrator` keyword.  -->

<!-- We use the following facts: -->

<!-- - The equations of implicit system are initialized to the state -->
<!--   variables increments (i.e. `feel` is initialized to `deel`). -->
<!-- - The jacobian \(J\) is initialized to the identity -->
<!--   (i.e. `dfeel_ddeel` is initialized to the identity tensor). -->
<!-- - The increment of the total strain is automatically deduced from -->
<!--   `feel` by the `StandardElasticity` brick. -->

<!-- Apart from those facts, the code is an almost direct translation of -->
<!-- the mathematical expression described in previous sections and boils -->
<!-- down to the following lines of code: -->

<!-- ~~~~{.cpp} -->
<!-- @Integrator{ -->
<!--   if(b){ -->
<!--     const auto seq = sqrt(sig|H*sig); -->
<!--     const auto iseq = 1/(max(seq,real(1.e-10*D(0,0)))); -->
<!--     const auto n = iseq*H*sig; -->
<!--     // elasticity -->
<!--     feel        += dp*n; -->
<!--     dfeel_ddeel += theta*dp*iseq*(H-(n^n))*D; -->
<!--     dfeel_ddp    = n; -->
<!--     // plasticity -->
<!--     fp           = (seq-s0-R*(p+theta*dp))/D(0,0); -->
<!--     dfp_ddp      = -theta*(R/D(0,0)); -->
<!--     dfp_ddeel    =  theta*(n|D)/D(0,0); -->
<!--   } -->
<!-- } -->
<!-- ~~~~ -->

<!-- Local IspellDict: english -->

