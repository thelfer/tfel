The `@AdditionalConvergenceChecks` keyword is meant to introduce a
code block returning stating if convergence has been reached. More
precisely, this code block is meant to modify a boolean variable
called `converged`. This boolean is `true` if the standard convergence
criterion has been reached, `false` otherwise.

One possible usage of this code block is multi-surfaces' plasticity
treated by activating or desactivating statuses, as described by Simo
and Hughes.

# Example

Consider a two surfaces plastic behaviour. We will use an array of
booleans called `statuses`.

~~~~{.cpp}
@Brick StandardElasticity; // to have computeElasticPrediction

@LocalVariable bool statuses[2];

@Prediction{
  // initial status based of the elastic prediction
  auto sigel = computeElasticPrediction();
  for(unsigned short i=0;i!=2;++i){
	statuses[i] = ...
  }
} // end of @Prediction

@Integrator{
  for(unsigned short i=0;i!=2;++i){
    if(statuses[i]){
      ...
    }
  }
} // end of @Integrator
~~~~
