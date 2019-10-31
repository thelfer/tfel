% The `Implicit` domain specific language
% Thomas Helfer
% 8/02/2018

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
\newcommand{\Frac}[2]{{{\displaystyle \frac{\displaystyle #1}{\displaystyle #2}}}}
\newcommand{\deriv}[2]{{\displaystyle \frac{\displaystyle \partial #1}{\displaystyle \partial #2}}}
\newcommand{\sderiv}[2]{{\displaystyle \frac{\displaystyle \partial^{2} #1}{\displaystyle \partial #2^{2}}}}
\newcommand{\dtot}{{{\mathrm{d}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}


# Description

Let \(\vec{Y}\) be a vector satisfying the following ordinary
differential equation:

\[
\vec{\dot{Y}}=G\paren{\vec{Y},t}
\]

where \(t\) is not meant to describe an explicit dependency to the time
but rather a placeholder for variables whose evolutions are known
(those variables are called external state variables in `MFront`).

An implicit scheme turns this ordinary differential equation into a non
linear system of equations whose unknowns are is increment
\(\Delta\,Y\) over a time step \(\Delta\,t\):

\[
\Delta\,\vec{Y}-G\paren{\bts{\vec{Y}}+\theta\,\Delta\,\vec{Y},t+\theta\,\Delta\,t}\,\Delta\,t=0
\]

# Available algorithms

The following algorithms are available:

- `NewtonRaphson`
- `NewtonRaphson_NumericalJacobian`
- `PowellDogLeg_NewtonRaphson`
- `PowellDogLeg_NewtonRaphson_NumericalJacobian`
- `Broyden`
- `PowellDogLeg_Broyden`
- `Broyden2`
- `LevenbergMarquardt`
- `LevenbergMarquardt_NumericalJacobian`

## Notes about updating auxiliary state variable or local variables in the `Integrator` code blocks when the numerical evaluation of the jacobian is requested

In some cases, it is convenient to update auxiliary state variables
in the `@Integrator` code block, rather than computing them in
the `@UpdateAuxiliaryStateVariables` code block which is only called
once the convergence is reached.

However, if the jacobian matrix is computed numerically (at least
partially), such updates could be wrong, because they can be based
on the perturbated values of the unknowns. 

In `TFEL 3.1`, this can be circumvented by testing the value of the
`perturbatedSystemEvaluation` boolean value as follows:

~~~~{.cpp}
// let av be an auxiliary state variable
@AuxiliaryStateVariable StrainStensor av;

@Integrator{
  // put updated value of av in a temporary variable
  const auto av_ = eval(...);  
  ...
  definition of the implicit system
  ...
  if(!perturbatedSystemEvaluation){
    // update auxiliary state variables
    av = av_;
  }
} // end of @Integrator
~~~~

In many cases, rather than updating auxiliary variables during the
Newton iterations, it can be more pratical to compute its increment,
defined in by local variable and to update the auxiliary variable in
the `@UpdateAuxiliaryStateVariables` code block. The previous trick can
be used in this case in a straightforward manner.

