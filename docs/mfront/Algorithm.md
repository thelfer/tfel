The `@Algorithm` keyword is used to select a numerical algorithm.

The set of available algorithms depends on the domain specific
language. As the time of writting this notice, the following
algorithms are available:

- `euler`, `rk2`, `rk4`, `rk42` , `rk54` and `rkCastem` for the
  `Runge-Kutta` dsl.
- `NewtonRaphson`, `NewtonRaphson_NumericalJacobian,`
  `PowellDogLeg_NewtonRaphson`,
  `PowellDogLeg_NewtonRaphson_NumericalJacobian`, `Broyden`,
  `PowellDogLeg_Broyden`, `Broyden2`, `LevenbergMarquardt`,
  `LevenbergMarquardt_NumericalJacobian` for implicit dsls.

## Example (Runge-Kutta dsl)

~~~~{.cpp}
@Algorithm rk54;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Example (Implicit dsl)

~~~~{.cpp}
@Algorithm NewtonRaphson;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

