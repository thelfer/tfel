The `@PostProcessing` keyword introduces a code block that can be used
to perform computations independently of the behaviour integration.

The outputs of post-processings are stored in so-called *post-processing
variables*. See the `@PostProcessingVariable` for details.

Post-processings are typically meant to be called at the end of a time
step, when the equilibrium has been reached.

Because post-processings are called independently of the behaviour
integration step, special care to the meaning of the variables must be
taken:

- The thermodynamic forces will have their values at the end of the time
  step.
- The state variables and auxiliary state variables will have their
  values at the end of the time step. The associated increments, if
  defined, will be null.

The values of the thermodynamic forces, state variables, auxiliary state
variables at the beginning of the time step are available in a special
data structure named `initial_state`.

Concerning material properties, they have their values at the end of the
time step as usual.

For the gradients and external state variables have their values at the
end of the time step. Their values at the beginning of the time step are
avaiable in the `initial_state` data structure (if defined). Their
increments have their usual values.

> **About initialisation of local variables**
>
> The code block defined by the `@InitLocalVariables` code block
> shall be called before the execution of the post-processing.
> However, this code block will be called with the thermodynamic forces, 
> state variables, auxiliary state variables at the end of the time step.

## Example of usage

The following code defines a post-processing computing the principal
strain at the end of the time step:

~~~~{.cxx}
//! principal strains
@PostProcessingVariable tvector<3u,strain> εᵖ;
εᵖ.setEntryName("PrincipalStrain");
//! compute the principal strain
@PostProcessing PrincipalStrain {
  εᵖ = eto.computeEigenValues();
}
~~~~