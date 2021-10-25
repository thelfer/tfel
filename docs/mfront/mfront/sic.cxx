@Parser IsotropicMisesCreep;
@Behaviour SiC;
@Author Helfer Thomas;
@Date 22 / 01 / 06;

@Interface umat;

@Coef real young;
@Coef real nu;
@Coef real alpha;

@ExternalStateVar real flux;

@LocalVar real AF1;
@LocalVar real AF3;

@InitLocalVars {
  real T_tdt = T + theta * dT;
  real AF1 = 5.084957636e-8f * exp(-442.e3f / (8.314f * (T_tdt)));
  real AF3 = 0.4e-37f * flux;
}

@FlowRule {
  real tmp = pow(seq, real(0.36));
  f = seq * (AF1 * tmp + AF3);
  df_dseq = 1.36f * AF1 * tmp;
}
