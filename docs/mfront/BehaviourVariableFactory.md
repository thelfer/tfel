The `@BehaviourVariableFactory` keyword allows to define an object able
to create a behaviour variable.

## Example of usage

~~~~{cxx}
@BehaviourVariableFactory f1 {
  file: "BehaviourVariablePlasticity.mfront",
  variables_suffix: "1",
  external_names_prefix: "FirstPhase",
  store_gradients: true,
  store_thermodynamic_forces: true,
  shared_external_state_variables: {".+"}
};

@Integrator{
  auto b1 = f1.make();
  ...
}
~~~~
