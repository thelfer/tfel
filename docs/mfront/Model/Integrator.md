The `@Integrator` keyword defines the computational part of the model.
It is equivalent to the `Function` keyword except that it does not
except a function name.

## Example

~~~~{.cpp}
@Integrator {
  const real coef1 = 8.e-3;
  const real coef2 = 4.e-2;
  const real p_    = 0.5*(p+p_1);
  s = s_1 + coef1*exp(coef2-p_)*(Bu-Bu_1);
}
~~~~
