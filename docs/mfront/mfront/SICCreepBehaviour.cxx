@Parser IsotropicMisesCreep;
@Behaviour SiCCreep;
@Author É.Brunon;
@Date 06 / 12 / 07;

@Description {
  Matériaux RCG - T et RCG -
      R Un point sur le carbure de Silicium NT SESC / LIAC 02 -
      024 ind 0 de décembre 2002 J.M.ESCLEINE
 §8.4.2 Page 34
}

@ExternalStateVar real flux;

@StaticVar real A = 4.4e3;
@StaticVar real B = 76.0e3;
@StaticVar real a = 1.0e-37;

@LocalVar real AF1;
@LocalVar real AF3;

@InitLocalVars {
  AF1 = A * exp(-B / (T + theta * dT));
  AF3 = a * (flux + theta * dflux);
}

@FlowRule {
  df_dseq = AF1 + AF3;
  f = seq * df_dseq;
}
