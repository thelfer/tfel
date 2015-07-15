The `@Function` keyword defines a computational part of the
model. This keyword is followed by a string defining the name of this
part of the computation.

### Example

~~~~{.cpp}
@Function compute
{
  const real coef1 = 8.e-3;
  const real coef2 = 4.e-2;
  const real p_    = 0.5*(p+p_1);
  s = s_1 + coef1*exp(coef2-p_)*(Bu-Bu_1);
} // end of function compute
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
