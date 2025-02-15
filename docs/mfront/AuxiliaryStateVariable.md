The `AuxiliaryStateVariable` keyword introduces one or several new
auxiliary state variables. It is followed by a type name and the
name(s) of the variable(s) declared, separated by commas.

## Example

~~~~{.cpp}
// scalar auxiliary state variable
@AuxiliaryStateVariables strain p;
// symmetric tensors auxiliary state variable
@AuxiliaryStateVariables StrainStensor evp,evp2;
~~~~
