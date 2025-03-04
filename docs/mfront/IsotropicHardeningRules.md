The `@IsotropicHardeningRules` keywords introduces the isotropic
hardening rule associated with a flow rule as the sum of predefined
hardening rules.

The following predefined hardening rules are
available: `Data`, `Linear`, `Power`, `StrainRateSensitive`, `Swift`,
`UserDefined`, and `Voce`. Those rules are documented in the
`StandardElastoViscoPlasticity` brick.

When an isotropic hardening rule is associated with a flow rule (using
the `flow_id` parameter), two variables are accessible in the code block
defining the flow rule:

- \(R\) which stands for the yield radius surface,
- \(dR_dp\) which stands for the derivative of the yield radius surface
  with respect to the equivalent plastic strain.

## Example

~~~~{.cxx}
@IsotropicHardeningRules {
  flow_id : 0,
  isotropic_hardening : "Voce" {R0 : 125e6, Rinf : 500e6, b : 20},
  isotropic_hardening : "Linear" {R0 : 25e6}
};
~~~~

## Notes

- For DSLs supporting only one flow rule, the `flow_id` parameter is
  optional and shall be equal to zero if specified.
- In the case of the `IsotropicPlasticMisesFlow` DSL, the following
  flow rule is implicitly defined (if the user does specify it):

  ~~~~{.cxx}
  @FlowRule {
    f = seq - R;
    df_dseq = 1;
    df_dp = -dR_dp;
  }
  ~~~~
