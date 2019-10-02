% Release notes of the 3.2.2 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2018

Version `3.2.2` is mainly a bug fix version of the `3.2` series.

This version was released along with Version `3.1.5` and inherits from
all the fixes of this version.

# New MFront Features

## New algorithm to treat damage in the `DDIF2` stress potential

Two improvements were made to the numerical treatment of the `DDIF2`
stress potential:

- The `DDIF2` damage behaviour is now treated by default using an
  algorithm based on statuses. In each damage directions, the damage
  state is kept constant during the Newton iterations. Once converged,
  the consistency of the damage state with the solution found is tested.
  If the state is inconsistent, the iterations are restarted with a new
  state.
- The implicit scheme is now divided in two steps by default. In the
  first step, the time step is set to zero before the prediction stage.
  This is meant to filter all viscoplastic flows and the convergence is
  thus performed only on the damage state (unless another
  rate-independent mechanisms is considered). Once converged on the
  damage state, the time step is reset to its original value and the
  implicit resolution is restarted.

The `DDIF2` stress potential (and thus the DDIF2 brick) has two new
options:

- `use_status_algorithm` (boolean value, true by default). If false, the algorithm used
  in previous version is used.
- `first_converge_on_damage` (boolean value, true by default). If false,
  the time step is not set to zero at the prediction stage.

# Issues solved

## Ticket #187: Conversions in `Number::getCxxFormula` is biased by rounding errors

`Number::getCxxFormula` did rely on `std::to_string` which only has 6
significant digits. As an example, the output of `std::to_string(1e-14)`
is `0.000000`.

To overcome this issue, the `Number` structure now holds the original
string representation of the number, as given by the user.

For more details, see: <https://sourceforge.net/p/tfel/tickets/187/>

## Ticket #186: "Less than" conditional is not correctly translated to `C++` by `tfel::math::Evaluator`

The `tfel::math::Evaluator` class allows to translate a formula in `C++` since Version 3.2.

Operators `<` and `<=` were not correctly implemented, so the following
formula `a<b ? 0 : 1` was translated `a>b ? 0: 1` in `C++`.

For more details, see: <https://sourceforge.net/p/tfel/tickets/186/>

## Ticket #182: Jacobian error in the SwiftIsotropicHardeningRule

For more details, see: <https://sourceforge.net/p/tfel/tickets/182/>

## Ticket #174: Using formulae with the Voce hardening rule is broken

For more details, see: <https://sourceforge.net/p/tfel/tickets/174/>

## Ticket #173: The axial component of the deformation gradient was not correctly updated in the `generic` interface in plane stress when using the GreenLagrange strain measure

For more details, see: <https://sourceforge.net/p/tfel/tickets/173/>

# References

