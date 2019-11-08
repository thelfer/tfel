@Parser    MultipleIsotropicMisesFlows;
@Material  NbZrC;
@Behaviour Creep;
@Author    E. Brunon;
@Date      05/05/09;

@LocalVar real A;
@ExternalStateVar real dpa;
dpa.setGlossaryName("IrradiationDamage");

@InitLocalVars{
  A = 7.1687e-31*exp(-4.1722e+04/(T+theta*dT));
}

// Fluage Thermique
@FlowRule Creep{
  real tmp = A*pow(seq,3.9);
  df_dseq  = 4.9*tmp;
  f        = seq*tmp;
}

// Fluage d'irradiation
@FlowRule Creep{
  df_dseq = 0.5e-12*ddpa/dt;
  f       = df_dseq*seq;
}
