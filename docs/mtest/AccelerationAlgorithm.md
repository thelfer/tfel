The `@AccelerationAlgorithm` keyword is used to give the name of the
acceleration algorithm used to accelerate the convergence toward
equilibrium.

The following algorithms are available:

- `Cast3M`: the acceleration algorithm of the resolution procedures of
  the `Cast3M` finite element solver.
- `Secant`: the secant method.
- `Steffensen`: the Steffensen method.
- `IronsTuck`: the Irons-Tuck method.
- `CrossedSecant`: the crossed secant (or crossed \(1\)-\(\delta\))
  method, often called Aitken relaxation.
- `AlternateSecant`: the alternate secant (or alternate
  \(1\)-\(\delta\)) method.
- `CrossedDelta2`: the crossed \(\delta^{2}\) method.
- `AlternateDelta2`: the alternate \(\delta^{2}\) method.
- `Crossed2Delta`: the crossed \(2\)-\(\delta\) method.
- `Crossed2Deltabis`: a variant of the crossed \(2\)-\(\delta\) method,
  which may not converge to the solution.
- `Alternate2Delta`: the alternate \(2\)-\(\delta\) method.
- `UAnderson` and `FAnderson`: two variants of the Anderson
  acceleration algorithm.

The parameters of these algorithms are given by the
`@AccelerationAlgorithmParameter` keyword:

- all the algorithms but `UAnderson` and `FAnderson` accept the
  `AccelerationTrigger` parameter, the number of the first iteration at
  which the acceleration is applied. Its default value is \(4\) for
  `Cast3M`, \(3\) for `Secant`, `Steffensen`, `CrossedDelta2` and
  `AlternateDelta2`, and \(2\) for the other algorithms. Its minimal
  value is \(3\) for `Cast3M`, `Secant` and `Steffensen`, and \(2\) for
  the other algorithms.
- `Cast3M` also accepts the `AccelerationPeriod` parameter, the number
  of iterations between two accelerations (\(2\) by default).
- `UAnderson` and `FAnderson` accept the `MethodOrder` parameter, the
  maximum number of previous iterates used (\(4\) by default), and the
  `AccelerationPeriod` parameter: the acceleration is applied once
  every `AccelerationPeriod` iterations (\(2\) by default). Both
  parameters must be between \(1\) and \(255\).

## Example:

~~~~~{.cpp}
@AccelerationAlgorithm 'Secant';
~~~~~
