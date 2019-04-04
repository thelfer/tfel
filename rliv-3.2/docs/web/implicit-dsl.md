% The `Implicit` domain specific language
% Thomas Helfer
% 8/02/2018

# Implicit schemes

## Available algorithms

### Notes about updating auxiliary state variable or local variables in the `Integrator` code blocks when the numerical evaluation of the jacobian is requested

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

