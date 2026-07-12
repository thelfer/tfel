The `@LinearSystemSolver` keyword is used to select the linear sytem
solver. It is followed by the name of solver. Currently, two solvers
are supported:

- `Default`. This uses the solvers provided natively by the `TFEL/Math`
  library. As implied by its name, this solver is used by default.
- `TDLS` (or equivalently `TinyDeviceCallableLinearSolver`). See
  <https://trsxvz.github.io/TDLS/getting_started.html> for details. If
  `TFEL` is not compiled with support of the `TDLS` library selecting
  this solver falls back the the `Default` Solver.

The name of the solver can optionnaly be followed by a dictionary
allowing to parametrize the solver.

## Supported Parameters

### Parameters of the `Default` Linear Solver

The `Default` Linear Solver does not accept any parameter.

### Parameters of the `TDLS` Linear Solver

The following options are supported:

- `tile_size`: the matrix is processed as a grid of `tile_size x
  tile_size` register tiles. This is the main performance axis of the
  solvers - tune it per system dimension. The `tile_size` can be a
  strictly positive integer or a string. In `tile_size` is a string, it
  is interpreted as a integer formula whose variable can be any integer
  constant (see the `@IntegerConstant` keyword), `N`, `TVectorSize`,
  `StensorSize` or `TensorSize`. This formula must evaluate to a
  strictly positive integer for all hypotheses supported by the
  behaviour.
- `schedule`: expected values are either `left_looking` (or equivalently
  `left`, `left-looking` or `LeftLooking`) and `right_looking` (or
  equivalently `right`, `right-looking` or `RightLooking`). This
  parameter defines the elimination schedule of the tiled factorization.
- `out_of_tile_search_strategy`: expected values are either
  `first_acceptable` (or equivalently `first-acceptable` or
  `FirstAcceptable`) or `full_scan` (or equivalently `full-scan` or
  `FullScan`).
- unroll_inner_loop: expected values are either `true` or `false`. If
  `true`, loops indexing register tiles carry a forced-unroll pragma -
  the guard that keeps tiles in registers on GPU backends, where a
  rolled loop indexes the tile dynamically and demotes it to slow local
  memory. If `false`, no unroll pragma anywhere - faster compiles, GPU
  performance not guaranteed.

If a parameter is not defined, the default values are inherited for the
`TiledLUppDefaultConfig` class defined by the `TDLS` library. See the
[API reference](https://trsxvz.github.io/TDLS/api/index.html) for
details. At the time of writing this documentation, those default values
are:

- `3` for `tile_size`
- `right_looking` for `schedule`
- `first_acceptable` for `out_of_tile_search_strategy`
- `true` for `unroll_inner_loop`

## Example

~~~~ {#LinearSystemSolver .cpp}
@LinearSystemSolver TDLS{
  tile_size: 4,
  unroll_inner_loop: false
};
~~~~

~~~~ {#LinearSystemSolver .cpp}
@LinearSystemSolver TDLS{
  tile_size: "StensorSize"
};
~~~~
