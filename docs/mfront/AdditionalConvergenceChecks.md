The `@AdditionalConvergenceChecks` keyword is meant to introduce a
code block returning stating if convergence has been reached. More
precisely, this code block is meant to modify a boolean variable
called `converged`. This boolean is `true` if the standard convergence
criterion has been reached, `false` otherwise.

One possible usage of this code block is multi-surfaces' plasticity
treated by activating or desactivating statuses.

## Example

Consider a two surfaces plastic behaviour. To handle it, we will need
two arrays of boolean:

- the first one tells if the ith surface is activable during the time
  step.
- the second one gives the current status of the ith surface: if the
  corresponding status is set to `true`, this surface is active.

~~~~
@Brick StandardElasticity; // to have computeElasticPrediction

@LocalVariable bool activable[2];
@LocalVariable bool status[2];

@Prediction{
  auto sigel = computeElasticPrediction();
  for(unsigned short i=0;i!=2;++i){
	activable[i] = ...
  }
} // end of @Prediction

@Integrator{
  for(unsigned short i=0;i!=2;++i){
    if(status[i]){
      ...
    }
  }
} // end of @Integrator

@AdditionalConvergenceChecks{
  for(unsigned short i=0;i!=2;++i){
    if(!activable[i]){continue;}
    // update status[i]
	...
  }
}

~~~~
