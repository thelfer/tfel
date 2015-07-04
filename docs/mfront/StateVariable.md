The `StateVariable` keyword introduces one or several new state
variables. It is followed by a type name and the name(s) of the
variable(s) declared, separated by commas.

### Example

~~~~{.cpp}
// scalar state variable
@StateVariables strain p;
// symmetric tensors state variable
@StateVariables StrainStensor evp,evp2;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
