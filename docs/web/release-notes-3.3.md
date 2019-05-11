% New functionalities of the 3.3 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2019

The page declares the new functionalities of the 3.3 version of the
`TFEL` project.

# Improvements to `MTest`

## Ability to choose  the stress measure used in input/output and the consistent tangent operator returned for finite strain behaviours through the `generic` interface

The following options can be passed to the behaviour:

- `stress_measure`. The value of this option must be either
  `CauchyStress` (or `CAUCHY`), `SecondPiolaKirchoffStress` (or `PK2`)
  or `FirstPiolaKirchoffStress` (or `PK1`).
- `tangent_operator`. The value of this option must be either:
    - `DCAUCHY_DF` (or `DSIG_DF`): the derivative of the Cauchy stress
	  with respect to the deformation gradient
    - `DS_DEGL`: the derivative of the second Piola-Kirchoff stress
      with respect to the Green-Lagrange strain
	- `DPK1_DF`: the derivative of the first Piola-Kirchoff
      stress with respect to the deformation gradient is returned

~~~~{.cxx}
@Behaviour<generic> 'src/libBehaviour.so' 'LogarithmicStrainElasticity2' {
  stress_measure : PK2,
  tangent_operator : DPK1_DF
};
~~~~

The output of the behaviour is still the Cauchy stress. The stress
measure on input and input are converted from or to the Cauchy
stress.

The non linear algorithm still relies the derivative of the Cauchy
stress with respect to the deformation gradient: the tangent operator
returned by the behaviour is automatically converted to this
derivative.

# Tickets solved during the development of this version

## Ticket #172: Support for dynamic choice of the consistent tangent operator of finite strain behaviours using the `generic` interface

The `generic` interface now allows to specify, for finite strain
behaviours (including the behaviours based on the Green-Lagrange and
the Hencky strain measure):

- the stress measure used on input/output
- the consistent tangent operator expected.

If `K` is the array used to store the consistent tangent operator,
`K[0]` still represent the kind of integration to be performed.

For finite strain behaviours `K[1]` must now hold the stress measure used:

- if `K[1]` < 0.5, the Cauchy stress is used
- if 0.5 < `K[1]` < 1.5, the second Piola-Kirchoff stress is used
- if 1.5 < `K[1]` < 2.5, the first Piola-Kirchoff stress is used

`K[2]` must hold the consitent tangent operator returned by the behaviour:

- if `K[2]`<0.5, the derivative of the Cauchy stress with respect
  to the deformation gradient is returned
- if 0.5<`K[2]`<1.5, the derivative of the second Piola-Kirchoff
  stress with respect to the Green-Lagrange strain
  is returned
- if 1.5<`K[2]`<2.5, the derivative of the first Piola-Kirchoff
  stress with respect to the deformation gradient is returned

For more details, see: <https://sourceforge.net/p/tfel/tickets/172/>

## Ticket #171: Add the derivative of the first Piola-Kirchoff stress as a possible consistent tangentor for finite strain behaviours

For more details, see: <https://sourceforge.net/p/tfel/tickets/171/>
