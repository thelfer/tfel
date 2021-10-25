@ComputeStress {
  sig = D * eel;
}

@Derivative {
  /* coefficients d'orthotropie */
  real Hrr = ...;
  real Htt = ...;
  real Hzz = ...;
  real Hrt = ...;
  real Hrz = ...;
  real Htz = ...;
  /* tenseur de Hill */
  st2tost2<N, real> H = hillTensor<N, real>(Hzz, Hrr, Htt, Hrz, Hrt, Htz);
  real sigeq = sqrt(sig | H * sig);
  if (sigeq > 1.e9) {
    return false;
  }
  Stensor n(0.);
  if (sigeq > 10.e-7) {
    n = H * sig / sigeq;
  }
  /* Système différentiel */
  dp = 0.;
  devp = dp * n;
  deel = deto - devp;
}
