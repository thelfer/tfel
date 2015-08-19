The `@ComputeFinalStress` keyword introduces a code block meant to
compute the stress symmetric tensor after the integration.

The code block is called after the update of the state variables. The
auxiliary state variable and the external state variables are not
updated yet.

## Example

~~~~{.cpp}
@ComputeFinalStress{
  const SlidingSystems& ss = SlidingSystems::getSlidingSystems();
  // approximation de l'inverse de \(\Delta\,F_p\)
  inv_dFp = Tensor::Id();
  for(unsigned short i=0;i!=12;++i){
    inv_dFp -= dg[i]*ss.mu[i];
  }
  real J_inv_dFp = det(inv_dFp);
  inv_dFp /= CubicRoots::cbrt(J_inv_dFp);
  // Fe en fin de pas de temps
  Fe  = Fe_tr*inv_dFp;
  // Piola-Kirchhoff II
  S = D*eel;
  // Cauchy
  sig = convertSecondPiolaKirchhoffStressToCauchyStress(S,Fe);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
