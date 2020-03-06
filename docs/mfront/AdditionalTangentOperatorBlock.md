The `@AdditionalTangentOperatorBlock` keyword allows adding a tangent
operator block to the list of available tangent operator blocks.

The tangent operator blocks can be:

- the derivative of a flux with respect to a gradient.
- the derivative of a flux with respect to an external state variable.
- the derivative of a state variable with respect to an external state
  variable.

## Example

~~~~{.cpp}
@Gradient TemperatureGradient gT;
gT.setGlossaryName("TemperatureGradient");

@Flux HeatFlux j;
j.setGlossaryName("HeatFlux");

@AdditionalTangentOperatorBlock dj_ddT;
~~~~
