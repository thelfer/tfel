The `@Function` keyword defines a computational part of the
model. This keyword is followed by a string defining the name of this
part of the computation.

The functions are executed in the order of their declaration. An
output can only be modified by one function.

An output whose first modification in a function is a compound
assignment (`+=`, `-=`, `*=` or `/=`) starts from its value at the
beginning of the time step: its depth must be at least one (see the
`setDepth` method).

## Example

~~~~{.cpp}
@Function compute
{
  const real coef1 = 8.e-3;
  const real coef2 = 4.e-2;
  const real p_    = 0.5*(p+p_1);
  s = s_1 + coef1*exp(coef2-p_)*(Bu-Bu_1);
} // end of function compute
~~~~
