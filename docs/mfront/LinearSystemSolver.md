The `@LinearSystemSolver` keyword is used to select the linear system
solver. It is followed by the name of the solver. Currently, two solvers
are supported:

- `Default`. This uses the solvers provided natively by the `TFEL/Math`
  library. As implied by its name, this solver is used by default.
- `TDLS` (or equivalently `TinyDeviceCallableLinearSolver`). See
  <https://trsxvz.github.io/TDLS/getting_started.html> for details. If
  `TFEL` is not compiled with support for the `TDLS` library, selecting
  this solver falls back to the `Default` solver.

The name of the solver can optionally be followed by a dictionary
allowing to parametrize the solver.

## Supported Parameters

### Parameters of the `Default` Linear Solver

The `Default` Linear Solver does not accept any parameter.

### Parameters of the `TDLS` Linear Solver

The following options are supported:

- `tile_size`: the matrix is processed as a grid of `tile_size x
  tile_size` register tiles. This is the main performance axis of the
  solvers. Tune it per system dimension. The `tile_size` can be a
  strictly positive integer or a string. If `tile_size` is a string, it
  is interpreted as an integer formula whose variables can be any integer
  constant (see the `@IntegerConstant` keyword), `N`, `TVectorSize`,
  `StensorSize` or `TensorSize`. This formula must evaluate to a
  strictly positive integer for all hypotheses supported by the
  behaviour.
- `schedule`: expected values are either `left_looking` (or equivalently
  `left`, `left-looking` or `LeftLooking`) or `right_looking` (or
  equivalently `right`, `right-looking` or `RightLooking`). This
  parameter defines the elimination schedule of the tiled factorization.
- `out_of_tile_search_strategy`: expected values are either
  `first_acceptable` (or equivalently `first-acceptable` or
  `FirstAcceptable`) or `full_scan` (or equivalently `full-scan` or
  `FullScan`). With `first_acceptable`, the search below the tile stops
  at the first candidate whose corrected magnitude reaches the
  out-of-tile search threshold. With `full_scan`, all the rows below
  the tile are scanned and the best corrected candidate wins.
- `unroll_inner_loop`: expected values are either `true` or `false`. If
  `true`, loops indexing register tiles carry a forced-unroll pragma,
  the guard that keeps tiles in registers on GPU backends, where a
  rolled loop indexes the tile dynamically and demotes it to slow local
  memory. If `false`, no unroll pragma anywhere, for faster compiles,
  GPU performance not guaranteed.
- `out_of_tile_search_threshold`: acceptable-pivot threshold of the
  out-of-tile search. An in-tile pivot candidate whose magnitude reaches
  this value is accepted without looking outside the tile; below it, the
  search extends to the rows under the tile (out-of-tile pivoting) and
  the best candidate wins.
- `singular_pivot_threshold`: the factorization is declared singular
  when even the best candidate of the out-of-tile recovery stays below
  this threshold. This option must be set together with the
  `out_of_tile_search_threshold` option and must not exceed it.
- `out_of_tile_search_diagnostics`: expected values are either `true`
  or `false`. If `true`, an integer member `mfront_tdls_oot_count`,
  readable in user code blocks, reports the number of out-of-tile
  searches of the last linear solve. If `false` (the default), the
  generated code is unchanged.

If a parameter is not defined, the default values are inherited from the
`TiledLUppConfig` class defined by the `TDLS` library. See the
[API reference](https://trsxvz.github.io/TDLS/api/index.html) for
details. At the time of writing this documentation, those default values
are:

- `3` for `tile_size`
- `right_looking` for `schedule`
- `first_acceptable` for `out_of_tile_search_strategy`
- `true` for `unroll_inner_loop`
- `1e-10` for `out_of_tile_search_threshold` (or `1e-4` for single
  precision)
- `std::numeric_limits<double>::min()` for `singular_pivot_threshold`
  (or `std::numeric_limits<float>::min()` for single precision)

## Examples

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
