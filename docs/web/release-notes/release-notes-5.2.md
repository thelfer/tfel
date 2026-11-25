---
title: Release notes of the 5.2 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer, Antoine Martin
date: 2026
lang: en-EN
numbersections: true
documentclass: article
from: markdown+tex_math_single_backslash
geometry:
  - margin=2cm
papersize: a4
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}


This page describes the new functionalities of Version 5.2 of the
`TFEL` project.

# Highlights

## Support for the `TDLS` library (Tiny Device-callable Linear Solvers)

The `TDLS` library provides direct linear solvers for small dense
systems, callable on `CPU`s as well as inside `GPU` kernels, and
designed for maximum GPU performance:

- <https://github.com/trsxvz/tdls>
- [API reference](https://trsxvz.github.io/TDLS/api/index.html)

When compiling `TFEL`, one must explicitly enable support for the `TDLS`
library, see below.

## Compatibility modes

The behaviour of `MFront` and the results of the code it generates are
now governed by a compatibility mode, which is a version number. A
development which does not fix an explicit bug, but changes the results
of the `TFEL` libraries or the results or the behaviour of `MFront` and
of the assets it generates, such as a new default value of an option or
a new default behaviour of a working keyword, is associated with the
version which makes it the default. Users who need the previous
behaviour restore it by selecting an older compatibility mode.

Version 5.2 provides two compatibility modes:

- `5.2.0`, the values and behaviours of Version 5.2,
- `future`, which introduces the changes which shall be the default of
  the next major version.

By default, no compatibility mode is declared: the generated code uses
the entities and the algorithms of the version of the `TFEL` libraries
which compiles it. Declaring a compatibility mode makes the results of
the generated code independent of this version.

The compatibility mode of a file is defined by the `compatibility_mode`
option of its domain specific language, which is available for all the
DSLs, whereas the `--default-compatibility-mode` command line argument declares
the mode of all the files treated by a run of `mfront` which do not
declare their own mode:

~~~~{.bash}
$ mfront --obuild --interface=generic --default-compatibility-mode=5.2.0 Norton.mfront
~~~~

The compatibility mode of an asset which declares one is recorded in its
generated code as the `<entry point>_compatibility_mode` symbol, which is
returned by the `ExternalLibraryManager::getCompatibilityMode` method,
available both in `C++` and in `Python`.

The values of the physical constants are the first entities to be
versioned this way. The `TFEL/PhysicalConstants` library now provides
one set of constants per compatibility mode, in namespaces named after
the version which introduced them (`tfel::v5_2_0::`, `tfel::v6_0_0::`,
...), whereas the names used by the previous versions,
`tfel::PhysicalConstants` and the variables of `tfel::constants`, keep
referring to the values of the current version. The current values of
Version 5.2 are those of the `5.2.0` mode, so the physical constants of
version 5.2 are those of version 5.1: the use of the values of the
`CODATA` 2022 adjustment with all the digits of the `double`
representation is postponed to version 6.0, where it is available
through the `6.0.0` set of constants.

Compatibility modes are described in the pages devoted to
[compatibility modes in the `TFEL` libraries](tfel-compatibility-modes.html)
and to
[compatibility modes in `MFront`](mfront-compatibility-modes.html).

# Known incompatibilities

## Issues affecting end users' results

- Formulas of material properties or brick options given as strings in
  `MFront` files (see Issue #1103): `1/2` was compiled as an integer
  division and gave `0` (also in Version 5.1.1).
- In-place division of the arrays and views of `TFEL/Math` (tensors,
  vectors, matrices) by an integer, as in `s /= 2;` (see Issue #1097):
  every component was set to zero (also in Version 5.1.1).
- `RungeKutta` DSLs, `rk54` (the default) and `rk4` algorithms (see
  Issues #1095 and #1109): the final stress used the stiffness tensor
  computed by the behaviour at the middle of the time step. The last
  stage of `rk4` was evaluated at that date (also in Version 5.1.1).
- `RungeKutta` DSLs (see Issue #1430): the expressions substituted in
  `@ComputeStress` for the temperature, the external state variables and
  the gradients lacked parentheses. In the final stress,
  `200e9 - 1e8 * T` was evaluated as `200e9 - 1e8 * T + dT`.
- `generic` interface, plane stress and axisymmetrical generalised plane
  stress (see Issue #1285): when the solver gives `1` as axial
  deformation gradient, as `MTest` and `mfem-mgis` do, the Cauchy stress
  (with the Green-Lagrange strain) or the Piola-Kirchhoff stresses (with
  `AxialDeformationGradient`) were wrong by a factor of about two.
- `RungeKutta` DSLs, `@UpdateAuxiliaryStateVariables` (see Issue #1347):
  the gradients, the external state variables and the outputs of models
  have their current values, not those at the beginning of the time
  step. Blocks using `T + dT` must be modified.
- Isotropic DSLs (see Issue #1107): when several external state
  variables are declared in the same statement, only the value at the
  middle of the time step of the last one was initialised (also in
  Version 5.1.1).
- Models (see Issue #1344): an output updated by a compound assignment,
  such as `+=`, was uninitialised when the model is called by a
  behaviour. With several functions, each reads the values at the
  beginning of the time step, and only one can modify a given output.
- `@ComputeThermalExpansion` with options and thermal expansion options
  of the stress potentials (see Issue #1303): the `ReferenceTemperature`
  declared by an external `MFront` file was replaced by 293.15 K.
- `@RotationMatrix<Euler>` of `MTest` now gives the rotation described
  by the Euler angles in the convention of Bunge, as the polycrystal
  classes of `MFront` (see Issue #1270): the rotation matrix was the
  Bunge matrix instead of its transpose, i.e. the inverse rotation was
  applied.
- Formulas (see Issues #1103 and #1209): `&&` has precedence over `||`,
  and integer formulas are evaluated from left to right (`2*3/4` gave
  `0`). The derivatives of `log10` and of formulas whose variables are
  not in alphabetical order are fixed (also in Version 5.1.1).
- The `**` operator of the formulas read by `MTest`, `MFront` and
  `tfel-check` is right-associative (see Issue #1349): `a**b**c` is
  `a**(b**c)` and was `(a**b)**c`.
- `generic` interface (see Issue #1137): in a rotated frame, the
  deformation gradient of the orthotropic behaviours based on a strain
  measure was rotated as a symmetric tensor (also in Version 5.1.1).
- `generic` interface, finite strain behaviours (see Issues #951 and
  #1050): the stress is converted when the integration succeeds with
  unreliable results, and the stress returned with a prediction operator
  is the one at the beginning of the time step (also in Version 5.1.1).
- `generic` interface (see Issue #1050): the post-processings executed
  before the update of the state read the internal state variables at
  the beginning of the time step (also in Version 5.1.1).
- `generic` interface, orthotropic behaviours having several gradients
  (see Issue #1051): the functions rotating arrays of gradients,
  thermodynamic forces and tangent operator blocks wrote the variables
  following the first one at a wrong position.
- `MTest` assembles the equations associated with the off-diagonal
  components of the deformation gradient of finite strain behaviours
  with the off-diagonal stresses, and applies the imposed off-diagonal
  Cauchy stresses to both of their rows (see Issue #1290): computations
  leaving one of these components free, which failed or converged to a
  wrong state, converge to the right state, and the number of
  iterations of the other ones may change.
- `MTest` (see Issue #1260): in plane stress, the thermal strain of the
  orthotropic behaviours which only declare `ThermalExpansion1` and
  `ThermalExpansion2`, as the `castem` ones, was silently null.
- `MTest` (see Issue #1111): with a dynamic time step scaling and no
  `@MinimalTimeStep`, the last time step of a period could end after it,
  and any `@Event` activated every constraint (also in Version 5.1.1).
- `MTest` (see Issue #1315): the external state variables declared after
  a tensorial one are read at the right position in `@Test`,
  `@UserDefinedPostProcessing` and the `PTest` extractors.
- `PTest` (see Issue #1313): the contact pressure of a mandrel no longer
  contributes to the end cap effect.
- The parameters of a material property are local to the library
  defining it (see Issue #1323): two libraries of the same process
  defining a material property of the same name could share them.
- `generic` interface, Hencky strain, plane stress and axisymmetrical
  generalised plane stress (see Issue #1326): the Piola-Kirchhoff
  stresses and the tangent operators use the axial stretch computed by
  the behaviour, not the one given by the solver.
- `MTest` with `@DynamicTimeStepScaling` (see Issues #1313 and #1351):
  the `generic` behaviours may increase the time step, and the tests and
  post-processings are only evaluated on the accepted time steps.
- Material properties and models (see Issues #953, #1068 and #1187): the
  default values of the parameters and the bounds are written with 14
  significant digits in the generated sources, instead of 6 (also in
  Version 5.1.1).
- Behaviours based on the Hencky strain (see Issue #1349): the stresses
  and the tangent operators may change when two principal stretches are
  almost equal. `LogarithmicStrainHandler::eps` is now a relative
  threshold.
- `castem`, `aster`, `abaqus`, `ansys` and `dianafea` interfaces (see
  Issue #1099): the shear components of the stress-free expansions were
  wrongly subtracted from the strain, and `ansys` halved the shear
  stresses at the beginning of the time step (also in Version 5.1.1).
- `abaqus` and `ansys` interfaces, plane stress (see Issue #1307): the
  axial stress-free expansion (thermal expansion, swelling) of small
  strain behaviours was subtracted from the in-plane shear strain.
- `abaqus` interface (see Issue #1432): the rotation `DROT` applied by
  `Abaqus/Standard` to the stress before the call is cancelled for the
  finite strain behaviours and the `FiniteRotationSmallStrain` and
  `MieheApelLambrechtLogarithmicStrain` strategies.
- `abaqusexplicit` interface (see Issues #1274 and #1431): the results
  of orthotropic behaviours using the `MFront` orthotropy management
  policy change. In 3D, the inverse rotation was applied: only symmetric
  rotation matrices are unaffected. In 2D, the second material axis and
  the axial strain of plane stress were also read at wrong positions.
- `ansys` interface (see Issues #1093 and #1307): the finite strain
  behaviours read the deformation gradients in plane stress, which were
  left uninitialised, and the internal and dissipated energies computed
  by the behaviour are returned.
- `ansys` interface, finite strain strategies in 3D (see Issue #1119):
  with `MieheApelLambrechtLogarithmicStrain`, the XZ and YZ strain
  increments were exchanged; with `FiniteRotationSmallStrain`, a part of
  the XY Green-Lagrange strain was halved (also in Version 5.1.1).
- The `aster`, `ansys`, `dianafea`, `cyrano` and `zmat` interfaces
  evaluate the `@APrioriTimeStepScalingFactor` and
  `@APosterioriTimeStepScalingFactor` blocks, which were ignored (see
  Issue #1307): the time steps rejected by those blocks were accepted.
- `abaqusexplicit` interface (see Issue #1431): in plane stress, the
  `MieheApelLambrechtLogarithmicStrain` strategy converts the stress at
  the beginning of the time step with the right axial stretch. The
  energies are incremented from their previous values.
- `calculix` interface (see Issue #1125): the rotation matrix of
  cylindrical orientations was not orthogonal, and the Cauchy stress at
  the beginning of the increment of the finite strain behaviours used
  the deformation gradient at its end (also in Version 5.1.1).
- `calculix` interface, `MieheApelLambrechtLogarithmicStrain` strategies
  (see Issues #1258 and #1326): the dual stress of orthotropic
  behaviours is rotated back to the global frame before its conversion,
  and the `MieheApelLambrechtLogarithmicStrainII` strategy reads the
  dual stress in the state variables at the beginning of the increment.
- Other interfaces (see Issues #1117, #1157 and #1177): in 2D, `aster`
  wrongly rotated the deformation gradient of orthotropic finite strain
  behaviours and `dianafea` read six components of the strain. `comsol`,
  and the `castem` models whose temperature is not the first input, read
  the temperature out of bounds (also in Version 5.1.1).
- `zmat` interface (see Issue #1205): the elements of an array of non
  symmetric tensors were stored with the size of a symmetric tensor.
- `MTest` (see Issues #1129, #1161 and #1185): the results change for
  `castem_umat` behaviours with external state variables, orthotropic
  `castem` finite strain behaviours in plane stress, `dianafea`
  behaviours in 3D, and for the Piola-Kirchhoff stresses of rotated
  orthotropic `generic` behaviours (also in Version 5.1.1).
- `MTest` (see Issue #1307): the results change for the orthotropic
  `abaqus` behaviours having a thermal expansion in plane stress, in a
  rotated frame, and for the `ansys` finite strain behaviours using the
  stress at the beginning of the time step in 3D.
- Bricks (see Issues #1055, #1123 and #1131): the `Barlat`,
  `Cazacu 2001` and `Orthotropic Cazacu 2004` flow criteria used the
  coefficients of the stress criterion, `DDIF2` its first non constant
  fracture stress or energy in all directions. The broken state tests of
  the isotropic `Hooke` potential were swapped (also in Version 5.1.1).
- Generated code (see Issues #1137 and #1151): `@Relocation` was ignored
  in plane strain, in plane stress and when computed by an external
  model. The isotropic hardening rules of the
  `MultipleIsotropicMisesFlows` DSL used the equivalent plastic strains
  at the beginning of the time step (also in Version 5.1.1).
- HCP crystals (see Issue #1135): some families of slip systems, such as
  the pyramidal `<-1-123>{11-22}` one, have 6 systems instead of 3 (also
  in Version 5.1.1).
- `StandardElastoViscoPlasticity` brick (see Issues #1309 and #1340):
  when the flow is not active, the static recovery of the `DRS`
  kinematic hardening rule is applied (the back strain was frozen), and
  the values saved by `save_stress_criterion` and
  `save_yield_surface_radius` change.
- `StandardElastoViscoPlasticity` brick (see Issues #1101 and #1113):
  the nucleation models take all the inelastic flows into account, not
  only the first one, and the Gurson-Tvergaard-Needleman stress is no
  longer negative under hydrostatic compression (also in Version 5.1.1).
- `StandardElastoViscoPlasticity` brick (see Issues #1324 and #1336):
  the porosity was doubled when its estimate was negative, and the
  Gurson-Tvergaard-Needleman and Rousselier-Tanguy-Besson stresses
  change for null or very small porosities.
- `StandardElastoViscoPlasticity` brick (see Issue #1309): with the
  staggered scheme, the porosity was never updated when a flow without
  isotropic hardening rule sets `save_porosity_increase`.
- `MultipleIsotropicMisesFlows` DSL (see Issue #1340): the isotropic
  hardening rules of a flow use the `theta` parameter of this flow, not
  the one of the behaviour.
- `Implicit` DSLs (see Issue #1303): the material properties depending
  on a state variable having a normalisation factor are evaluated with
  the increment of this variable, not with its normalised increment.
- `ScalarDamage` stress potential in axisymmetrical generalised plane
  stress (see Issue #1256): the axial stress was `(1 - d)` times the
  imposed one.
- Altered stiffness tensors in axisymmetrical generalised plane stress
  (see Issue #1121): the hoop component was condensed instead of the
  axial one (also in Version 5.1.1).
- `TFEL/Math` (see Issues #1109, #1115, #1133 and #1147): the derivative
  of the negative part, `st2tost2::convert`, `st2tot2::tpld`,
  `CubicRoots`, `TinyMatrixInvert`, a rebuilt `Kriging` and the final
  time of the `RungeKutta54` and `RungeKutta42` solvers are fixed (also
  in Version 5.1.1).
- Eigenvalues of 3D symmetric tensors (see Issues #1115 and #1153): the
  eigenvalues and eigenvectors were wrongly sorted when two eigenvalues
  are equal, and `FSESQLEIGENSOLVER` and `FSESHYBRIDEIGENSOLVER`
  returned `NaN` for uniaxial states (also in Version 5.1.1).
- `symmetric_product(s1, s2)` returns half of `s1 * s2 + s2 * s1`, as
  documented (see Issue #998): the factor `1/2` was missing (also in
  Version 5.1.1).
- `TFEL/Math` (see Issue #1433): `convertToTangentModuli` halved the
  shear columns, and `det` of `st2tost2` and `t2tot2` objects ignored
  the row exchanges of the LU decomposition.
- `computeDeterminantSecondDerivative` of a non-symmetric tensor now
  returns the second derivative of the determinant in 2D and 3D (see
  Issue #1262): the rows of the off-diagonal components differ from the
  previous results, which were the derivative of the adjugate (also in
  Version 5.1.1).
- `TFEL/Math` (see Issues #1203, #1317 and #1321): `RungeKutta2::exe`
  and `RungeKutta4::exe` overshot the final time, nested calls of an
  external function in a formula used wrong arguments, and the
  Levenberg-Marquardt minimisers ignored `setMultiplicationFactor`.
- The self-consistent scheme of a `ParticulateMicrostructure` no longer
  normalises the localisation tensors of the inclusions: the homogenised
  stiffness changes. The localisation tensor of the matrix returned by
  `computeDilute` is no longer the identity.
- In the homogenisation schemes of `TFEL/Material`, the elasticity of an
  anisotropic `OrientedDistribution`, given in the local basis of its
  inclusions, is rotated to the global basis (see Issue #1299): it was
  used as a global elasticity, so that the orientation of the elasticity
  of these phases was ignored. `computeEllipsoidLocalisationTensor` and
  `computePlaneStrainLocalisationTensor` with an isotropic matrix rotate
  the elasticity of the inclusion as well, as documented. The results of
  the dilute, Mori-Tanaka and self-consistent schemes with such phases
  change.
- Homogenisation (see Issues #1121 and #1153): the Hill tensor of an
  ellipsoid whose two largest semi-axes are equal and the plane strain
  Hill tensor of a rotated ellipse in an anisotropic matrix were wrong.
  The Hashin-Shtrikman bounds of the shear modulus were too narrow for
  phases which are not well ordered (also in Version 5.1.1).
- `MetallicCFCGenericSlidingSystemsInteractionMatrix` (see Issue #1056):
  every call of `getInteractionMatrix` and `getSecondInteractionMatrix`
  returned the matrix built by the first one.
- `SlipSystemsDescription::getClimbTensors` and the `--climb-tensors`
  queries of `mfront-query` (see Issue #1437): the tensors are built on
  the slip direction, as in the generated behaviours, not on the normal
  to the slip plane.
- In the python bindings of `MTest`, `istates[i]` returns a reference to
  the state of the integration point, not a copy (see Issue #1163): the
  values set through it were lost, and a state kept in a variable now
  follows the computation.
- In the python bindings, `ExternalBehaviourDescription.evtypes`
  returned the types of the internal state variables, and
  `mfront.umatxx.Behaviour.isIsotropic` and `isOrthotropic` the type of
  the behaviour instead of its symmetry (see Issue #1139, also in
  Version 5.1.1).
- `tfel-check` (see Issues #1032 and #1209): the column formulas such as
  `'$2+2*$1'` were evaluated with exchanged columns, and a `NaN` in a
  compared column made the comparison succeed (also in Version 5.1.1).
- Eigen solvers of 3D symmetric tensors (see Issue #1254):
  `FSESANALYTICALEIGENSOLVER`, `FSESCUPPENEIGENSOLVER` and
  `HARARIEIGENSOLVER` gave wrong or inaccurate eigenvectors for equal or
  close eigenvalues. The default solver changes within its accuracy.
- `computeEshelbyTensor` (see Issue #1349): the results change for
  elongated ellipsoids having two close semi-axes, as the test of
  axisymmetry is now relative to the smallest semi-axis.

## Fixes improving convergence

- Jacobians and tangent operators (see Issues #1101, #1107, #1113,
  #1119, #1123, #1125, #1131, #1133, #1157, #1159, #1177, #1189, #1209
  and #1301): fixes in the `StandardElastoViscoPlasticity` brick, the
  stress criteria, the `Implicit` DSLs and the `ansys`, `abaqus`,
  `aster`, `castem` and `comsol` interfaces (also in Version 5.1.1).
- `Implicit` DSLs (see Issues #1303 and #1334): the variables having a
  normalisation factor have the right increments in `@TangentOperator`
  and the right maximum increment per iteration. The correction of an
  iteration includes the projection on the physical bounds.
- The tiny non-linear solvers (and the `Implicit` DSLs) evaluate the
  estimate given by the last allowed correction (see Issue #1288): an
  integration that converges with exactly `iterMax` corrections now
  succeeds, and a failing one evaluates its residual once more.
- The isotropic DSLs accept a convergence reached at the last allowed
  iteration, which was reported as a failure (see Issue #1199).
- `LogarithmicStrainHandler` (see Issue #1292): in 3D, the tangent
  moduli of the behaviours based on the Hencky strain are fixed when two
  eigenvalues of the right Cauchy-Green tensor are equal and the dual
  stress is not coaxial with the strain. The stresses are unchanged.
- The Broyden algorithms of the `Implicit` DSLs (`Broyden`,
  `PowellDogLeg_Broyden` and `Broyden2`) update the jacobian, or its
  inverse, again (see Issue #1295): since `TFEL-4.0`, the update was
  hidden by the `updateOrCheckJacobian` method generated in every
  behaviour. The number of iterations of the behaviours using them
  changes, and so do their results within the convergence criterion.
  In `TinyBroydenSolver` and `TinyPowellDogLegBroydenSolver`
  (respectively `TinyBroyden2Solver`), the update is performed by the
  `updateJacobian` (respectively `updateJacobianInvert`) method, which
  replaces their `updateOrCheckJacobian` method.
- `Implicit` DSLs, inverse of the jacobian (see Issues #1303 and #1494):
  the `iJ_X_Y` blocks are fixed when a block of the same row is not
  used, and `getPartialJacobianInvert` for a tensorial variable which is
  not the first one.
- The `FiniteStrainSingleCrystal` brick takes into account, in the
  jacobian and the consistent tangent operator, the derivatives of the
  normalisation of the inverse of the plastic deformation gradient
  increment (see Issues #1309 and #1359). These derivatives were
  explicitly neglected, as numerical tests showed (or at least seemed to
  show) that they were negligible. The residual is unchanged, so the
  results only change within the convergence criteria, unless the
  sub-stepping changes: the number of local and global iterations
  changes, and decreases significantly when the deformation gradient is
  not fully imposed. The boolean option
  `plastic_deformation_gradient_normalisation_corrections` of the brick,
  which is `true` by default, allows users to easily test if this
  correction affects their results: setting it to `false` restores the
  previous derivatives.
- `castem` interface with `@CastemUseTimeSubStepping` (see Issue #1157):
  unless `@CastemDoSubSteppingOnInvalidResults` is `true`, a successful
  integration asking for a smaller time step no longer triggers the
  local sub-stepping (also in Version 5.1.1).
- Consistent tangent operators (see Issues #1207, #1307 and #1309): the
  non symmetric operators of the `castem` interface were transposed in
  1D and with `FiniteRotationSmallStrain`. Those of the `ansys`
  interface with `FiniteRotationSmallStrain` in 3D and of the `DDIF2`
  brick with a non null `AngularCoordinate` are fixed.
- `MTest` (see Issues #1260, #1313 and #1371): the first iterate of the
  `UAnderson` acceleration algorithm, the elastic stiffness of the
  `castem` behaviours with the generic plane stress algorithm and the
  initial stiffness of the `Abaqus/Explicit` behaviours in a rotated
  frame are fixed.

## Fixes affecting the user experience

- The integer values read by `CxxTokenizer::readInt` and
  `CxxTokenizer::readUnsignedInt`, i.e. the integers of the `MTest`,
  `MFront` and `tfel-check` files (numbers of iterations and of
  sub-steps, columns, array sizes of the DSLs, ...), shall be written
  without decimal point nor exponent: `3.0`, `3.` or `3e0` are now
  rejected (see Issue #1326).
- Integers written with a decimal point or an exponent were truncated:
  `1e2` was read as `1` (see Issue #1326). Negative values of unsigned
  integers and out of range values are now rejected (see Issue #1201).
- The array sizes of the variables of `MFront` and the integer
  formulas evaluated by `IntegerEvaluator` shall only contain integers:
  a number with a decimal point or an exponent, such as `2.5` (which was
  truncated to `2`), `1e1` or `2.0`, is now rejected (see Issue #1321).
- Inputs that were silently ignored are now rejected (see Issue #1256):
  the `overriding_parameters` option of the model DSLs, `@Bounds` and
  `@PhysicalBounds` on gradients and thermodynamic forces, bounds on a
  component of a scalar variable, and the `Ksf` option of the
  `HarmonicSumOfNortonHoffViscoplasticFlows` inelastic flow.
- The `calculix` and `comsol` interfaces reject the behaviours declaring
  a stress-free expansion, such as `@ComputeThermalExpansion` or
  `@Swelling` (see Issue #1338): the results were wrong. With
  `CalculiX`, the `*EXPANSION` keyword shall be used.
- `mfront-query`, `mfront-doc`, `mtest`, `mfm` and `mfm-test-generator`
  exit with the status `1` on an input error, instead of aborting (see
  Issue #1211). The `--no-terminate-handler` command line argument
  restores the former behaviour.
- On Linux, the libraries built by `mfront --obuild` contain the
  directory of the libraries of `TFEL` in their run path, unless
  `--set-rpath=false` is given (see Issue #1077).
- The `env.sh` script no longer adds an empty entry to `LD_LIBRARY_PATH`
  and `PYTHONPATH` when they are not set (see Issue #1251): the current
  directory is no longer searched for libraries and python modules.
- The negative values of unsigned parameters in the
  `<Behaviour>-parameters.txt` files are rejected, as are the constants
  followed by a suffix, such as `0.3f`, in `@ElasticMaterialProperties`,
  `@ComputeThermalExpansion`, `@ComputeStiffnessTensor` and
  `@HillTensor` (see Issues #1319 and #1436).
- Models (see Issues #617, #1305, #1355 and #1358): the `generic` and
  `castem` interfaces check the bounds of the outputs, the `castem`
  models follow the `CASTEM_OUT_OF_BOUNDS_POLICY` environment variable,
  and the models read the `<ClassName>-parameters.txt` file of the
  current directory.
- `is_delta_zeros_defined`, `fzeros_1`, `inv_jacobian`, `levmar_first`
  and the names of the tangent operator blocks are reserved names in the
  `Implicit` DSLs (see Issues #1303 and #1355), as are `fsscb_c`,
  `fsscb_iinv_dFp_t` and `fsscb_mu` with the `FiniteStrainSingleCrystal`
  brick (see Issue #1309).
- Normalisation factors are rejected on the integration variables
  handled by the bricks, such as `eel` or the equivalent plastic strains
  (see Issue #1347): the bricks ignored them, which led to wrong
  results.
- The `save_strain` and `save_stress` options of `@StrainMeasure`, when
  set to `false`, no longer add the `StrainMeasure` and `DualStress`
  auxiliary state variables (see Issue #1330): the number of internal
  state variables of such behaviours changes.
- Bounds on the first component of a tensorial variable, such as
  `eel(0)`, were checked on all its components (see Issue #1256).
- `MaterialLaw` DSL (see Issue #1319): the physical bounds deduced from
  the glossary are also declared, and thus checked, when `@UnitSystem`
  follows `@Function`.
- `MTest` (see Issue #1260): an incomplete set of orthotropic thermal
  expansion coefficients is now an error (except `ThermalExpansion1` and
  `ThermalExpansion2` in plane stress).
- `MTest` and `PTest` reject the parameters of the acceleration
  algorithms which are not integers between 0 and 65535, the bare name
  of a tensorial external state variable in `@Test`, and
  `@MandrelRadiusEvolution` with the `TightPipe` radial loading (see
  Issues #1197, #1315 and #1351).
- In the text files read by `MTest` and `tfel-check`, a `#` character
  starts a comment anywhere in a line (see Issue #1201).
- Failures of the behaviour (see Issues #1205, #1258 and #1307): the
  `dianafea` interface stops the computation instead of returning
  silently, the `comsol` interface returns `3` instead of `-1`, and the
  `abaqusexplicit` interface reports the exceptions. The `zmat`
  interface checks the bounds of the variables.
- In the `Makefile` generated by `mfront`, `$(LDFLAGS)` ends the link
  command, and the default warning flags are not added to a user-defined
  `CXXFLAGS` or `CFLAGS` (see Issues #962 and #1054).
- `PTest` writes the oxidation lengths in their own columns of the
  output file (see Issue #1161, also in Version 5.1.1).
- `mfront-query` takes into account the
  `--dsl-material-property-option`, `--dsl-behaviour-option` and
  `--dsl-model-option` options and the associated `...-options-file`
  ones, which were ignored (see Issue #1191, also in Version 5.1.1).
- `tfel-config --tfel-home` returns the installation path of `TFEL`, not
  the name of the environment variable (see Issue #1048, also in
  Version 5.1.1).
- In the python bindings, the `None` value of the `OutOfBoundsPolicy`
  enumeration is renamed `None_`, as `None` is a reserved keyword in
  python (see Issue #1072).
- In the python bindings, `mtest.makeConstraintOptions` takes keyword
  arguments, not a tuple and a dictionary (see Issue #897, also in
  Version 5.1.1).
- In the python bindings, the `source` attribute of the entry points of
  `mfront.database` returns the source file, not the type, now given by
  `type` (see Issue #1181, also in Version 5.1.1).
- In the python bindings, the argument of
  `TargetsDescription.getLibrary` is named `n` instead of `library` (see
  Issue #1073).
- `tfel-check` (see Issue #892): two columns of two files are compared
  by `@Test {'reference.res' : 2} {'results.res' : 3};`. The form
  `@Test {{'reference.res' : 2}, {'results.res' : 3}};`, formerly the
  only one accepted, is rejected (also in Version 5.1.1).
- `tfel-check` applies the output check of a command declared with
  `shall_fail` to the output of the failed command (see Issue #1342),
  and runs once the `.check` files of a directory reachable through
  several paths (see Issue #1321).
- With the `icpx` compiler, `TFEL` and the libraries built by
  `mfront --obuild` are compiled with `-fno-fast-math` unless
  `enable-fast-math` is set (see Issue #1416): the results may change at
  the level of rounding errors.
- With `MinGW`, the python modules are installed in
  `lib/python<X.Y>/site-packages` instead of
  `bin/python<X.Y>/site-packages` (see Issue #955).

## Internal incompatibilities

- `cmake` 3.20 or later is now required to build `TFEL`.
- Most single-argument constructors of the public classes of `TFEL`,
  `MFront` and `MTest` are `explicit` (see Issue #1010): implicit
  conversions, such as `mtest::ConstantEvolution e = 1.;`, no longer
  compile.
- The `size_type` of the fixed-size objects of `TFEL/Math` (`tvector`,
  `stensor`, `st2tost2`...), returned by `size`, is `unsigned int`
  instead of `unsigned short` (see Issue #905).
- The `TFEL_VISIBILITY_EXPORT`, `TFEL_VISIBILITY_IMPORT` and
  `TFEL_VISIBILITY_LOCAL` macros, and the macros built on them, expand
  to standard attributes (see Issue #873): they must start a
  declaration, as in `TFEL_VISIBILITY_EXPORT static void f();`.
- The misspelt names of functions of `TFEL/Math` and of stress measures
  of `MTest` (`Kirchoff` for `Kirchhoff`) are deprecated and will be
  removed in version 6.0 (see Issue #1483).
- The `CoalescedView` and `ConstCoalescedView` views of `TFEL/Math`, the
  `MappableMathObjectUsingCoalescedViewConcept`,
  `MappableMutableMathObjectUsingCoalescedViewConcept` and
  `MappableImmutableMathObjectUsingCoalescedViewConcept` concepts and the
  `TFEL/Math/Array/CoalescedView.hxx` header are deprecated: `Coalesced`
  is replaced by `Scattered` in those names (see Section
  @sec:tfel:5.2:tfel_math:views). The former names will be removed in
  version 6.0.
- The type identifiers exported by `MFront` for the variables whose
  type is the derivative of a tensorial type alias (such as
  `StressStensor`, `Stensor`, `TVector` or `Tensor`) with respect to a
  non-scalar type follow the encoding described in the documentation:
  for instance, `derivative_type<StressStensor, StrainStensor>` is
  exported as `268`, like `Stensor4`, instead of `76`, which `MTest` and
  `MGIS` decode as `derivative_type<stensor<1, real>, real>`. Behaviours
  declaring such variables must be generated again (see Issue #1332).
- The symbols exported for the bounds of the elements of array variables
  are named `..._mfront_index_<i>_LowerBound` instead of
  `..._mfront_index_<i>__LowerBound`, as `MGIS` expects (see Issue
  #1205).
- Quantities (see Issues #1003 and #1353): `qt`, `qt_ref` and
  `const_qt_ref` are classes deriving from `Quantity`, not aliases. The
  views arrays of quantities map a storage of the underlying numeric
  type: mapping a storage of quantities no longer compiles.
- Units of the quantities (see Issues #1001 and #1013): `GenerateUnit`,
  the `Unit` class template and the `TFEL/Math/Quantity/Unit.ixx` header
  are removed, and `UnitConcept` moves to the `tfel::math::unit`
  namespace. User-defined units must be rewritten.
- `Evaluator::getVariablesNames` returns the names of the variables
  ordered by their positions (the order given to the constructor, or the
  order of their first appearance in the expression), instead of the
  alphabetical order (see Issue #1209).
- The Levenberg-Marquardt minimisers (`LevenbergMarquardt`,
  `FSLevenbergMarquardt`) reject the steps that increase the cost (see
  Issue #1288): the iterates of every fit change.
- A `YoungNuModuli`, `KGModuli` or `LambdaMuModuli` object can no longer
  be assigned from another format of isotropic moduli (see Issue #1203):
  these assignments compiled and did nothing.
- The `TFEL_TESTS_ASSERT` macro throws at the first failed assertion
  (see Issue #1024). The new `TFEL_TESTS_CHECK` macro reports the
  failure and continues the test, as before.
- `computeSelfConsistent` (`computeSelfConsistentScheme` in python)
  takes a `Polycrystal`. For a `ParticulateMicrostructure`,
  `computeAsymmetricSelfConsistent` shall be used: its second argument
  is a relative tolerance instead of a number of iterations.
- `ParticulateMicrostructure::replaceMatrixPhase` is replaced by
  `changeElasticityOfMatrixPhase`, and the `stiffness` data member of
  the phases by the `getElasticityOfPhase` method.
- A `ParticulateMicrostructure` stores copies of the distributions given
  to `addInclusionPhase` and can no longer be copied (see Issues #946
  and #978): `changeElasticityOfInclusionPhase` and
  `changeFractionOfInclusionPhase` shall be used to modify them.
- The interfaces of `MFront` must implement a method named `setOptions`.
- The static method `get` of the
  `GlobalDomainSpecificLanguageOptionsManager` class has been removed.
  The global options passed to domain specific languages are now handled
  by the `ConfigurationManager` class, which derives from the
  `GlobalDomainSpecificLanguageOptionsManager` class and whose unique
  instance is returned by `ConfigurationManager::get()`.
- Code extending `MFront` (see Issues #1334 and #1340):
  `computeElasticLimit` and `computeElasticLimitAndDerivative` of
  `IsotropicHardeningRule` take the expression of `theta` as a new
  argument, and the generated `computeNumericalJacobian` and
  `updateOrCheckJacobian` methods return a boolean.
- The following installed headers are removed (see Issues #1205 and
  #1264):
  `IsotropicLinearFourierHookLaw.hxx`, `IsotropicLinearFourierLaw.hxx`,
  `ThermalBehaviour.hxx`, `ThermalBehaviourData.hxx` and
  `ThermoMechanicalBehaviour.hxx` in `TFEL/Material`,
  `TFEL/Math/Parser/EvaluatorProxy.hxx` and
  `MFront/Cyrano/CyranoGenericPlaneStressHandler.hxx`.
- The boolean argument of `CxxTokenizer::isValidIdentifier` now states,
  as documented, if `C++` keywords are allowed (see Issue #964): its
  meaning has been inverted. With the default value (`true`), `C++`
  keywords are now considered as valid identifiers (also in Version
  5.1.1).
- The value of a raw string literal token given by `CxxTokenizer` holds the
  whole literal, from the `R` delimiter to the closing `"` included, and its
  offset is that of the `R`, as for a quoted string (see Issue #1390): the
  value was the contents of the literal only and the offset that of those
  contents, so that the consumers which stripped the first and last characters
  of the value of the string tokens, as the parsers of `MFront`, `MTest` and
  `tfel-check` did, truncated the raw string literals. Those parsers read the
  right contents now: `@Date R"(23/11/06)";` gave the date `3/11/0` and
  `@Author R"(Helfer "Thomas")";` the author `elfer "Thomas"`. Code which
  reads the contents of a string token shall use the new
  `tfel::utilities::getContents` function rather than removing the first and
  last characters of the value.
- `CxxTokenizer::stripComments` gathers all the comments of a line, separated
  by a space character, instead of keeping only the last one (see Issue
  #1392): the comments of a line holding `//! first` and `//! second` are
  `first second`, not `second`. The empty comments bring nothing and give no
  separator, so that a line whose only comment is empty gives no entry in the
  comments of the tokenizer.
- Tokenizing a file line by line, with a call to `CxxTokenizer::parseString`
  per line, gives the same tokens as giving the whole contents to a single call
  (see Issue #1396): a file whose last line is terminated by an end of line
  character gave an additional empty line, which is not a line of the file, so
  that the value of a comment or of a raw string literal which is not closed at
  the end of the file held an additional end of line character, and the last
  line of a file which is not terminated by an end of line character was
  tokenized twice. The line numbers are unaffected.
- The offset of a comment token which has no contents on the line it starts on
  is that of the delimiter which opens the comment (see Issue #1391): it was
  the offset reached after the delimiters and the spaces which would have
  surrounded the contents, so that it pointed past the end of the line for an
  empty `C++` style comment, as `a; //`, and beyond the opening delimiter for an
  empty `C` style comment or for a comment opened at the end of a line, as the
  header comments of the `MFront` files starting with `/*!`. The flags, the
  lines and the values of the tokens are unaffected.
- When the `keepCommentBoundaries` option of `CxxTokenizer` is enabled,
  the offsets of the tokens which follow a comment on the same line are
  no longer shifted by the size of the delimiter opening the comment
  (see Issue #1407).
- `tfel::utilities::tokenize` no longer returns an empty first token for
  a string starting with the delimiter, and `replace_all` with a start
  position keeps the characters preceding it (see Issue #1201).
- With quantities, the units of `PhysicalConstants::ElementaryCharge`
  and `FaradayConstant` are `A.s` and `A.s/mol` instead of `A/s` and
  `A/s/mol` (see Issue #1175, also in Version 5.1.1).
- The `enable-sanitize-options` option is no longer a boolean option: it
  now takes the list of the sanitizers to enable, e.g.
  `-Denable-sanitize-options="address;undefined"`. Enabling all the
  sanitizers at once was indeed meaningless as most sanitizers cannot be
  combined (for instance `address`, `thread` and `memory`). Each
  sanitizer can also be enabled individually using its dedicated
  `enable-<name>-sanitizer` option, see below.
- The `Conan` recipe and the `Homebrew` patches are no longer provided,
  and `tfel-config --oflags` no longer returns `-O2 -DNDEBUG` when
  `TFEL` is built in `Debug` mode with `gcc`, `nvc++` or MSVC (see Issue
  #1239).
- With `TFEL_APPEND_VERSION` or `TFEL_VERSION_FLAVOUR`, the static
  libraries are named `lib<name>-<suffix>.a` instead of
  `lib<name>-{TFEL_SUFFIX}.a` (see Issue #1183, also in Version 5.1.1).
- On Windows, `tfel::system::dirSeparator` returns the backslash (see
  Issues #947 and #948, also in Version 5.1.1).

# Improvements to the build system

## New `cmake` options

- `enable-tdls`: adds support for the Tiny Device-callable Linear
  Solvers (`TDLS`) library. `OFF` by default.
- `enable-tdls-as-default-linear-system-solver-in-mfront`: use `TDLS`
  as the default linear solver. Requires `enable-tdls` to be `ON`.
  `OFF` by default.
- `enable-pybind11-stubgen`: generate the stub files (`.pyi` files) of the
  python modules built using `pybind11` and install them. Those files are
  used by python development environments to provide code completion and
  type checking. This option has no effect if the python bindings are not
  enabled or if the `pybind11-stubgen` tool is not found. `ON` by default.
- `enable-libfuzzer-tests`: configure the `libFuzzer` based analyses of
  the `tfel::utilities::CxxTokenizer` class. `OFF` by default. Those
  analyses can only be built with `clang` and are neither built nor run
  by `make check`, see the `build-cxxtokenzier-libfuzzer-tests` and
  `cxxtokenzier-libfuzzer-tests` targets described below.

## Sanitizers and hardening options

The support for sanitizers has been modularised: the `enable-sanitize-options`
option takes the list of the sanitizers to enable (a comma may be used as a
separator instead of a semicolon), for instance:

~~~~{.bash}
$ cmake <srcdir> -Denable-sanitize-options="address;undefined"
~~~~

The available sanitizers depend on the compiler and on its version. The
`address`, `thread` and `memory` sanitizers cannot be used simultaneously.

Each sanitizer also has a dedicated option, whose name is obtained from the
sanitizer name: `enable-<name>-sanitizer`, e.g.
`-Denable-bounds-strict-sanitizer=ON`. Such options can be combined with the
`enable-sanitize-options` option: the enabled sanitizers are the sanitizers
listed in the `enable-sanitize-options` option and the sanitizers whose
dedicated option is enabled. A dedicated option is also defined for
`integer` with `gcc` and for `bounds-strict` with `clang`, which those
compilers do not support: enabling it issues a warning and the sanitizer
is ignored.

The following sanitizers are declared for `gcc` (the sanitizers corresponding
to a subset of the undefined behaviour sanitizer can be enabled
individually):

- `address`, `thread`, `leak`, `hwaddress`,
- `undefined` and its subsets: `alignment`, `bool`, `bounds`,
  `bounds-strict`, `builtin`, `enum`, `float-cast-overflow`,
  `float-divide-by-zero`, `null`, `object-size`, `return`, `shift`,
  `shift-base`, `shift-exponent`, `signed-integer-overflow`, `vptr`,
- `pointer-compare`, `pointer-subtract` (the address sanitizer and the link
  time optimization are required and added automatically),
- hardening flags, which can be combined with the sanitizers above:
  `fstack-check`, `fstack-protector-all`, `fno-omit-frame-pointer`.

The following sanitizers are declared for `clang`:

- `address`, `thread`, `memory` (the memory sanitizer also enables
  `-fsanitize-memory-track-origins=2`), `leak`, `hwaddress`,
- `cfi`, `cfi-icall`, `safe-stack`, `shadow-call-stack`, `dataflow`,
- `undefined` and its subsets: `alignment`, `bool`, `bounds`, `enum`,
  `float-cast-overflow`, `float-divide-by-zero`, `integer`,
  `unsigned-integer-overflow`, `null`, `object-size`, `return`, `shift`,
  `shift-base`, `shift-exponent`, `signed-integer-overflow`, `vptr`,
- `pointer-compare`, `pointer-subtract` (the address sanitizer and the link
  time optimization are required and added automatically),
- hardening flags, which can be combined with the sanitizers above:
  `address-use-after-scope` (`-fsanitize-address-use-after-scope`),
  `fstack-check`, `fstack-protector-all`, `fno-omit-frame-pointer`.

Some sanitizers require specific settings to be fully effective (the link
time optimization for `cfi` and `shadow-call-stack`, instrumentation of all
the linked code for `memory` and `safe-stack`, etc.), as described in the
`gcc` and `clang` documentations.

Note that the `vptr` check, which is part of the `undefined` sanitizer,
may lead to pathologically long compilation times with `gcc` for some
files of `MFront`, which implement heavy template code: it takes more
than ninety minutes to compile
`mfront/src/GenericMaterialPropertyInterfaceBase.cxx` with
`-fsanitize=undefined` (`-O2`), whereas a few minutes are needed when the
`vptr` check is disabled. This check can be excluded by listing
individually the sanitizers of interest, using the corresponding
dedicated options (`alignment`, `bool`, `bounds`, `builtin`, `enum`,
etc.) instead of the `undefined` sanitizer.

When at least one sanitizer is enabled, the `-fno-omit-frame-pointer` flag is
added automatically to ease the interpretation of the reports emitted by the
sanitizers.

## User-defined `cmake` variables

- `generic-parallel-configurations`: list of configurations to be
  supported. See next paragraph for details and the list of supported
  configurations.
- `TFEL_GXX_COMPILER`: path to the `g++` compiler.
- `TFEL_CLANGXX_COMPILER`: path to the `clang++` compiler.
- `TFEL_ICPX_COMPILER`: path to Intel's `icpx` compiler.
- `TFEL_NVHPC_COMPILER`: path to NVIDIA's `nvhpc` compiler.
- `TFEL_ACPP_COMPILER`: path to AdaptiveCpp's `acpp` compiler.
- `TFEL_CLANG_CUDA_PATH`: path to the `CUDA` toolkit used by `clang` in
  the `cuda-clang` and `hip-clang` configurations (by default, the
  toolkit of `nvcc`). A version of `clang` only supports the versions of
  `CUDA` released before it.
- `tdls_DIR`: directory where to find the `cmake` files installed by the
  Tiny Device-callable Linear Solvers (`TDLS`) library.

## Supported parallel configurations

The following configurations are supported for parallel computations:

- `cuda-nvcc`: configuration associated with the `CUDA` parallel
  programming model and NVIDIA's `nvcc` compiler
- `cuda-clang`: configuration associated with the `CUDA` parallel
  programming model and LLVM's `clang` compiler
- `hip-hipcc`:  configuration associated with the `HIP` parallel
  programming model and AMD's `hipcc` compiler
- `hip-clang`:  configuration associated with the `HIP` parallel
  programming model and LLVM's `clang` compiler
- `stlpar-parunseq-gcc`: configuration associated with `C++` standard
  parallel algorithms using the parallel unsequenced execution policy
  and the `gcc` compiler.
- `stlpar-parunseq-clang`: configuration associated with `C++` standard
  parallel algorithms using the parallel unsequenced execution policy
  and the `clang` compiler.
- `stlpar-parunseq-icpx`: configuration associated with `C++` standard
  parallel algorithms using the parallel unsequenced execution policy
  and the `icpx` compiler.
- `stlpar-parunseq-nvhpc-gpu`: configuration associated with `C++`
  standard parallel algorithms using the parallel unsequenced execution
  policy and NVIDIA's `nvhpc` compiler with offloading on the GPU.
- `sycl-default-icpx`: configuration associated with the `SYCL`
  programming model and Intel's `icpx` compiler.
- `sycl-default-acpp`: configuration associated with the `SYCL`
  programming model and AdaptiveCpp's `acpp` compiler.
- `sycl-gpu-acpp`: configuration associated with the `SYCL`
  programming model and AdaptiveCpp's `acpp` compiler, with offloading
  on the GPU.

## Add a `build-tests` target

In previous versions, tests were compiled and run using the `check`
target, which internally calls `ctest` to run the tests. One
drawback of this approach is that `ctest` could not be run in parallel.

The `build-tests` target (which is a dependency of the `check` target)
only compiles the tests. To run the tests, one may still call the
`check` target or call directly `ctest`.

If the `build-tests` target has not been called, the `check` target
works as in previous versions.

## Generation of the stubs of the python modules

When the python bindings are enabled and the `pybind11-stubgen` tool is
found, the stub files (`.pyi` files) associated with the python modules
are generated by the `python-stubs` target, which is built along with the
project. The generated files are installed next to the python modules they
describe. Such files are used by python development environments to provide
code completion and type checking.

A `py.typed` marker is installed in the packages holding stubs, as type
checkers such as `mypy` only use the stubs of a package holding this marker
(see PEP 561). The arguments of the methods of the `mfront`,
`mfront.database` and `mtest` modules now have names, and can be passed by
keyword.

The `python-stubs` target can be triggered explicitly:

```
make python-stubs
```

## Fuzz testing the tokenizer with `libFuzzer`

The `tfel::utilities::CxxTokenizer` class, which `MFront` uses to read
the files of its domain specific language, is now covered by coverage
guided fuzz testing, gathered in the `additional-tests/fuzzing`
directory of the sources. Those analyses are configured by the
`enable-libfuzzer-tests` option, which is `OFF` by default, and can
only be built with `clang`.

Two targets are dedicated to them:

- `build-cxxtokenzier-libfuzzer-tests` builds the fuzzing executables,
- `cxxtokenzier-libfuzzer-tests` builds them and runs the analyses, for
  the duration given by the `libfuzzer-tests-max-total-time` option.

The fuzzing executables are excluded from the default target and no
analysis is registered with `ctest`, so that neither `make all` nor
`make check` builds or runs them: the duration of a fuzzing analysis is
not bounded a priori. The corpus of an analysis and the inputs it
generates which lead to a failure are kept in the build tree, so that a
long campaign is a sequence of runs sharing the same corpus.

The `mfront-fuzzing` analysis tokenizes `MFront` files with the options
`MFront` itself uses, and checks the contract of the produced tokens
beyond the absence of any sanitizer report. The `all` value of the
`mfront-fuzzing-database` option uses all the `MFront` files of
`mfront/tests/behaviours` as the database of the analysis.

For more details, see the documentation on [fuzz testing the
tokenizer](testing-fuzzing.html).

# New features in `TFEL/Utilities`

## Contents of the string tokens

The `getContents` function returns the contents of the literal held by a
`Token`: its value without the double quotes of a quoted string, without the
single quotes of a character literal and without the `R"delim(` prefix and the
`)delim"` suffix of a raw string literal. The `isRawString` and
`getRawStringDelimiter` functions tell if a token holds a raw string literal
and give its delimiter. Those functions allow a consumer to read the contents
of a string token without relying on the flag of the token only, as removing
the first and last characters of the value truncates a raw string literal (see
Issue #1390).

## Documented contract of the tokens

The contract of the tokens given by `CxxTokenizer` is documented now, on the
`Token` class for the value, the line and the offset of a token, and on the
`CxxTokenizer` class for the exceptions: the value of a number does not hold
the digit separators of the `C++14` integer literals, whereas the offsets
remain those of the file, so that a source code can be rewritten from the
offsets of its tokens (see Issue #1393); a `C` style comment and a raw string
literal which are not closed at the end of a file are accepted,
and leave the state of the tokenizer open, contrary to a quoted string
and a character literal, which are rejected (see Issue #1395). Checking that
the tokens describe the file they come from is what the fuzz testing analysis
of the tokenizer does, see above.

# New `TFEL/Config` features

The `TFEL/Config` library is now documented on [this
page](tfel-config.html).

## TFEL's default substitutions

The `getTFELDefaultSubstitutions` function returns some default
substitutions used by `MFront` when reading configuration files and
`tfel-check` when analysing `.check` files. The keys listed below are
delimited by the `@` character in those files and in the map returned
by this function, e.g. `@TFEL_CXX_STANDARD@`.

### General substitutions

- `TFEL_CXX_STANDARD`: the `C++` standard used by the project.

### Supported compilers

- `TFEL_GXX_COMPILER`: path to the `g++` compiler. The value associated
  with this key is determined as follows, by order of priority:

  1. by passing the variable `TFEL_GXX_COMPILER` during the invocation of
  `cmake`.
  2. by using `g++` as the main compiler to compile `TFEL`
  3. by searching `g++` using `cmake`'s `find_program` function.
- `TFEL_CLANGXX_COMPILER`: path to LLVM's `clang++` compiler. The value
  associated with this key is determined as follows, by order of
  priority:

  1. by passing the variable `TFEL_CLANGXX_COMPILER` during the invocation of
  `cmake`,
  2. by using `clang++` as the main compiler to compile `TFEL`,
  3. by searching `clang++` using `cmake`'s `find_program` function.
- `TFEL_ICPX_COMPILER`: path to Intel's `icpx` compiler. The value
  associated with this key is determined as follows, by order of
  priority:

  1. by passing the variable `TFEL_ICPX_COMPILER` during the invocation of
  `cmake`,
  2. by using `icpx` as the main compiler to compile `TFEL`,
  3. by searching `icpx` using `cmake`'s `find_program` function.
- `TFEL_ACPP_COMPILER`: path to AdaptiveCpp's `acpp` compiler. The value
  associated with this key is determined as follows, by order of
  priority:

  1. by passing the variable `TFEL_ACPP_COMPILER` during the invocation of
  `cmake`,
  2. by searching `acpp` using `cmake`'s `find_program` function.
- `TFEL_NVHPC_COMPILER`: path to NVIDIA's `nvhpc` compiler. The value
  associated with this key is determined as follows, by order of
  priority:

  1. by passing the variable `TFEL_NVHPC_COMPILER` during the invocation of
  `cmake`,
  2. by using `nvhpc` as the main compiler to compile `TFEL`,
  3. by searching `nvc++` using `cmake`'s `find_program` function.
- `TFEL_NVCC_COMPILER`: path to NVIDIA's `nvcc` as found by
  `FindCUDAToolkit`.
- `TFEL_HIPCC_COMPILER`: path to AMD's `hipcc` compiler. The value
  associated with this key is determined as follows, by order of
  priority:

  1. by passing the variable `TFEL_HIPCC_COMPILER` during the invocation
    of `cmake`,
  2. by searching `hipcc` using `cmake`'s `find_program` function.

### Substitutions related to the `HIPCC` compiler

- `TFEL_HIPCC_FLAGS`: additional flags to be passed to the `hipcc`
  compiler.
- `TFEL_HIP_INCLUDE_DIR`: path to the `HIP` headers.

### Substitutions related to the `CUDA` platform

- `TFEL_CUDA_PATH`: path to NVIDIA's `CUDA` toolkit as found by
  `CMake`'s `FindCUDAToolkit` module.
- `TFEL_CUDA_LIBRARY_DIR`: path to the libraries of NVIDIA's `CUDA`
  toolkit as found by `CMake`'s `FindCUDAToolkit` module.
- `TFEL_CLANG_CUDA_PATH`: path to the `CUDA` toolkit used by `clang`
  (`cuda-clang` and `hip-clang` configurations).
- `TFEL_CLANG_CUDA_LIBRARY_DIR`: path to the libraries of this toolkit.

## The `--cmake-prefix-path` option of `tfel-config`

`--cmake-prefix-path` returns the path to the `CMake` configuration
files provided by the project:

~~~~{.bash}
$ tfel-config --cmake-prefix-path
/home/th202608/codes/tfel/master/install-python-3.10.12/share/tfel/cmake
~~~~

# New `TFEL/Math` features

## Views of tensorial objects whose components are not stored contiguously {#sec:tfel:5.2:tfel_math:views}

### Views of data separated by a constant stride

The `View` class, available in previous versions, allows interpreting a
contiguous memory area as a tensorial object. The `ScatteredView` class,
formerly named `CoalescedView` (see below), allows creating a tensorial
object where each component is referred to by its own pointer.

The `StridedView` class assumes that each component is separated by a
constant stride in memory, so that it only has to store one pointer and
the value of the stride.

Such views are typically used on GPUs, where coalesced memory accesses
are preferable.

~~~~{.cxx}
int values[8] = {1, 10, 2, 20, 3, 30, 4, 40};
auto s1 = map_strided<stensor<2u, int>>(values, 2);
~~~~

### Views named after the way they address the components

The views of tensorial objects whose components are not stored
contiguously are renamed after the way they address the components:
`ScatteredView` (formerly `CoalescedView`) stores one pointer per
component, `StridedView` (formerly `StridedCoalescedView`) stores a
pointer to the first component and the stride between two successive
components, and `StridedViewsArray` replaces
`StridedCoalescedViewsArray`. The `TFEL/Math/Array/ScatteredView.hxx`,
`TFEL/Math/Array/StridedView.hxx` and
`TFEL/Math/Array/StridedViewsArray.hxx` headers replace the headers named
after the former names.

These classes make no assumption on the layout of the data: whether
memory accesses are coalesced on a GPU depends on this layout and on the
mapping of the threads to the data.

The `map`, `map_strided`, `map_strided_array` and
`map_derivative_strided` functions are unchanged. The former names are
deprecated aliases and the former headers include the new ones; they
will be removed in version 6.0.

### Scattered views of derivatives

The `map_derivative_scattered` function creates a view of a derivative
from the pointers to the components of a matrix. It is the counterpart
of the `map_derivative_strided` function for a matrix whose components
are not separated by a constant stride (see the
[documentation of `TFEL/Math`](tfel-math.html#scattered-views-of-derivatives)
for details).

## Walpole basis

The Walpole basis relative to transverse isotropic fourth-order tensors (`st2tost2`)
is introduced with the `struct` `TransverseIsotropicWalpoleBasis`, member of
`tfel::math`.

## Refactoring of quantities

Quantities were deeply refactored to:

- improve maintainability (code is much more compact),
- allow creating surrogate classes for quantities,
- improve error messages,
- reduce size of the generated symbols.

## Adjugate of a tensor and its derivative

The `computeAdjugateDeterminantDerivative` function returns the
adjugate of a tensor, \(\det\left(F\right)\,F^{-1}\), i.e. the
transpose of the derivative of the determinant returned by
`computeDeterminantDerivative`. The
`computeAdjugateDeterminantSecondDerivative` function returns its
derivative, which `computeDeterminantSecondDerivative` returned in 2D
and 3D before Issue #1262 (it now returns the second derivative of the
determinant).

~~~~{.cxx}
const auto adj = computeAdjugateDeterminantDerivative(F);
const auto dadj = computeAdjugateDeterminantSecondDerivative(F);
~~~~

## Piola's and Nanson's transformations

### Piola's transformation

The `computePiolaTransformOfIdentityMatrixInReferenceConfiguration`
function returns the Piola transformation of the identity matrix of the
current configuration, i.e. the matrix
\(\det\left(F\right)\,F^{-1}\,F^{-T}\) in the reference configuration.

The `computePiolaTransform` function returns the Piola transformation
\(\det\left(F\right)\,F^{-1}\,q\) of a vector \(q\) of the current
configuration, such as the heat flux. The `computeInversePiolaTransform`
function returns the vector of the current configuration from its Piola
transformation.

~~~~{.cxx}
const auto Q = computePiolaTransform(q, F);
const auto q2 = computeInversePiolaTransform(Q, F);
~~~~

### Nanson's transformation

The `computeNansonTransform` function returns the vector
\(\det\left(F\right)\,F^{-T}\,N\) given by Nanson's formula. If \(N\)
is an oriented surface element of the reference configuration, the
result is the oriented surface element of the current configuration.
The `computeInverseNansonTransform` function returns the vector of the
reference configuration.

~~~~{.cxx}
const auto n = computeNansonTransform(N, F);
const auto N2 = computeInverseNansonTransform(n, F);
~~~~

### Derivatives

Strongly coupled resolutions need the derivatives of those
transformations. The `computePiolaTransformAndDerivatives`,
`computeInversePiolaTransformAndDerivatives`,
`computeNansonTransformAndDerivatives` and
`computeInverseNansonTransformAndDerivatives` functions return a tuple
made of the transformed vector, its derivative with respect to the
initial vector and its derivative with respect to the deformation
gradient.

~~~~{.cxx}
const auto [Q, dQ_dq, dQ_dF] = computePiolaTransformAndDerivatives(q, F);
~~~~

The derivative with respect to the vector is a `tmatrix`. The
derivative with respect to the deformation gradient is of type
`derivative_type<tvector<N, T>, tensor<N, real>>`. A `tmatrix` can now
be multiplied by such a derivative, which allows applying the chain
rule:

~~~~{.cxx}
// q depends on F through dq_dF
const auto dQ_dF_total = dQ_dF + dQ_dq * dq_dF;
~~~~

Those functions are declared in the
`TFEL/Math/Tensor/PiolaTransformation.hxx` header.

### Space dimensions one and two

In space dimensions one and two, those functions only describe the in
plane behaviour:

- the out of plane stretch, given by the third component of the
  deformation gradient, is taken into account: the result is the in plane
  block of the result obtained in the three dimensional case,
- the out of plane component of the vector on which the transformation is
  applied is taken equal to zero: the out of plane component of the
  result is null.

In space dimension one, only the diagonal components of the deformation
gradient are described. The result of
`computePiolaTransformOfIdentityMatrixInReferenceConfiguration` is
therefore \(F_{22}\,F_{33}/F_{11}\). In space dimension two, the missing
out of plane component of this matrix is
\(\det\left(F\right)/F_{33}^{2}\), where \(\det\left(F\right)\) is the
determinant of the two dimensional deformation gradient.

# New `TFEL/Material` features

## Finite strain heat transfer

For isotropic or orthotropic materials described by Fourier's law in the
current configuration, the
`computeThermalConductivityMatrixInReferenceConfiguration` functions are
available to compute the thermal conductivity matrix in the reference
configuration using Piola's transformation. Those functions can also be
used to compute the derivative of the thermal conductivity matrix with
respect to the temperature.

### Example of usage

~~~~{.cxx}
const auto K =
  computeThermalConductivityMatrixInReferenceConfiguration(k, F);
~~~~

where `k` is the thermal conductivity and `F` is the deformation
gradient.

### Space dimensions one and two

In space dimension two, the orthotropic version of the function takes the
two in plane thermal conductivities as arguments and returns the in plane
block of the thermal conductivity matrix:

~~~~{.cxx}
const auto K =
  computeThermalConductivityMatrixInReferenceConfiguration(k1, k2, F);
~~~~

The out of plane thermal conductivity \(k_3\) is not used: the out of
plane component of the three dimensional result,
\(\det\left(F\right)\,k_3/F_{33}^{2}\), is indeed of no use in a two
dimensional computation. The out of plane stretch is nevertheless taken
into account, as for Piola's transformation.

In space dimension one, only the diagonal components of the deformation
gradient are described and the isotropic version of the function is also
suitable for orthotropic materials: only the thermal conductivity in the
first direction is used and the result is
\(F_{22}\,F_{33}\,k_1/F_{11}\).

### Heat flux in the reference configuration and its derivatives

The `computeHeatFluxInReferenceConfigurationAndDerivatives` function
returns the heat flux \(Q=-K\,\nabla T\) in the reference
configuration, its derivative with respect to the temperature gradient
and its derivative with respect to the deformation gradient, which
strongly coupled resolutions need. The thermal conductivity matrix
\(K\) must be given by one of the
`computeThermalConductivityMatrixInReferenceConfiguration` functions.

~~~~{.cxx}
const auto K =
  computeThermalConductivityMatrixInReferenceConfiguration(k, F);
const auto [Q, dQ_dgT, dQ_dF] =
  computeHeatFluxInReferenceConfigurationAndDerivatives(K, gT, F);
~~~~

## Homogenization

The following methods of the `ParticulateMicrostructure` objects are able
to modify the microstructure:
 
 - `changeElasticityOfMatrixPhase`
 - `changeElasticityOfInclusionPhase`
 - `changeFractionOfInclusionPhase`

The `UserDefinedDistributionOfSpheroids` class allows defining
a distribution of spheroids defined by two orientation
tensors. This distribution can be used in a `ParticulateMicrostructure`
and the associated homogenization schemes can be used with
these microstructures.

The `Polycrystal` object has been added to deal with
homogenization schemes which do not assume that there is a matrix phase.
An object `Grain` was created for this class of microstructures.

The `computeSelfConsistent` function (`computeSelfConsistentScheme` in
the `Python` bindings) has been modified in order to be
more consistent with the literature. This scheme now takes a `Polycrystal`
in which no matrix phase exists. For the `ParticulateMicrostructure`,
the function `computeAsymmetricSelfConsistent`
(`computeAsymmetricSelfConsistentScheme` in the `Python` bindings) has
been introduced, which corresponds to a variant of the Self-Consistent
scheme for which the matrix phase plays a particular role.

In the homogenization schemes relative to the `ParticulateMicrostructure`,
the computation of the derivatives of the homogenized stiffness tensor
w.r.t. the isotropic moduli is available. It can be obtained
by using the attributes `.derivative_of_homogenized_stiffness_wrt_kr`
and `.derivative_of_homogenized_stiffness_wrt_mur` of the
`HomogenizationScheme` object (only for isotropic phases).

The second moments of strains considering a Hashin-Shtrikman scheme
are available. A new [tutorial](PonteCastaneda1992.html)
shows how to use them in the implementation of the Ponte-Castaneda
variational bound (1992), within the framework of non-linear elasticity.

# New features in `TFEL/System`

## Improvements in `ProcessManager`

- New overloads of the `createProcess` and `execute` methods allow
  specifying the directory where a command shall be executed.
- `ProcessManager` now works properly in a multithreaded context.

## Improvements in `SignalManager`

- `SignalManager` now works properly in a multithreaded context.

# New features in `MFront`

## The `@LinearSystemSolver` keyword

The `@LinearSystemSolver` keyword is used to select the linear system
solver. It is followed by the name of the solver. Currently, two solvers
are supported:

- `Default`. This uses the solvers provided natively by the `TFEL/Math`
  library. As implied by its name, this solver is used by default.
- `TDLS` (or equivalently `TinyDeviceCallableLinearSolver`). See
  <https://trsxvz.github.io/TDLS/main/getting_started.html> for details. If
  `TFEL` is not compiled with support for the `TDLS` library, selecting
  this solver falls back to the `Default` solver and a warning is
  reported. The parameters of the `TDLS` solver are checked, but not
  used. The `safe` option disables this warning, as in
  `@LinearSystemSolver<safe> TDLS;`.

The name of the solver can optionally be followed by a dictionary
giving the parameters of the solver.

### Supported parameters

#### Parameters of the `Default` linear solver

The `Default` linear solver does not accept any parameter.

#### Parameters of the `TDLS` linear solver

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
- `relative_pivot_threshold`: relative threshold of the pivot choice
  inside the diagonal tile, in `]0, 1]`. The row in place keeps the
  pivot when its magnitude reaches both this fraction of the largest
  in-tile candidate and the out-of-tile search threshold; otherwise the
  largest in-tile candidate wins. This threshold governs the in-tile
  choice only: the out-of-tile search keeps its absolute threshold. A
  value of `1` takes the largest in-tile candidate. This option requires
  a version of `TDLS` later than 0.2.0.
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
- `0.1` for `relative_pivot_threshold` (`TDLS` 0.2.0 takes the largest
  in-tile candidate, as a threshold of `1`)
- `std::numeric_limits<double>::min()` for `singular_pivot_threshold`
  (or `std::numeric_limits<float>::min()` for single precision)

### Examples

~~~~{.cpp}
@LinearSystemSolver TDLS{
  tile_size: 4,
  unroll_inner_loop: false
};
~~~~

~~~~{.cpp}
@LinearSystemSolver TDLS{
  tile_size: "StensorSize"
};
~~~~

## Configuration file

`MFront` now allows defining options in a configuration file given
through the `--configuration-file` command line argument as follows:

~~~~{.bash}
$ mfront --configuration-file=config.json --interface=generic-parallel UO2_ShearModulus.mfront
~~~~

The configuration file follows an extended `JSON`-like syntax. It contains
the following sections:

- `dsl_options`
- `material_property_dsl_options`
- `behaviour_dsl_options`
- `model_dsl_options`
- `interfaces_options`
- `material_property_interfaces_options`
- `behaviour_interfaces_options`
- `model_interfaces_options`
- `compilation_options`
- `linking_options`

None of those sections is required. All those sections must introduce a
dictionary.

> **Note**
>
> The options given in sections `dsl_options`,
> `material_property_dsl_options`, `behaviour_dsl_options` and
> `model_dsl_options` may also be given in dedicated configuration
> files, respectively specified by the `--dsl-options-file`,
> `--material-property-dsl-options-file`, `--behaviour-dsl-options-file`,
> and `--model-dsl-options-file` command line arguments introduced in
> Version 4.1.

### Example of configuration file

~~~~{.json}
interfaces_options: {
  generic-parallel: {
    backend: {stlpar: {execution_policy: "par_unseq"}}
  }
}
~~~~

### The `interfaces_options` section

The `interfaces_options` section associates the name of an interface
with a dictionary of options passed to this interface, as in the previous
example. The `material_property_interfaces_options`,
`behaviour_interfaces_options` and `model_interfaces_options` sections
have the same syntax, but their options are only passed to the
interfaces dedicated to material properties, behaviours and models
respectively.

### The `compilation_options` section

The `compilation_options` section associates a language (`c`,
`cxx`, `cuda` or `hip`) with a dictionary of options, as follows:

~~~~{.json}
compilation_options : {
  cxx : {
    compiler: "nvc++",
    compilation_flags: "-O2 -stdpar=gpu -std=c++20 -march=native -gpu=sm_89" 
  }
}
~~~~

#### The `compiler` option

The `compiler` option specifies the compiler used for the given
language. The environment variables `CC`, `CXX`, `CUDA_COMPILER` and
`HIP_COMPILER` take precedence over this option.

#### The `compilation_flags` option

The `compilation_flags` option specifies the compilation flags used for
the given language, as a string or as an array of strings. The
environment variables `CFLAGS`, `CXXFLAGS`, `CUDAFLAGS` and `HIPFLAGS`
take precedence over this option.

#### The `preprocessor_flags` option

The `preprocessor_flags` option specifies the preprocessor flags used
for the given language, as a string or as an array of strings. Those
flags are written before the compilation flags.

### The `linking_options` section

The `linking_options` section defines the options passed to the linker.

#### The `linker_flags` option

The `linker_flags` option specifies the flags passed to the linker, as a
string or as an array of strings. The environment variable `LDFLAGS`
takes precedence over this option.

## Passing options to interfaces on the command line

### First syntax

~~~~{.bash}
$ mfront --obuild --interface='generic-parallel{backend:{"stlpar": {execution_policy:"par_unseq"}}}' UO2_ShearModulus.mfront
~~~~

### Second syntax

~~~~{.bash}
$ mfront --obuild --interface='{generic-parallel:{backend:"stlpar"{execution_policy:"par_unseq"}}}' UO2_ShearModulus.mfront
~~~~

## The `generic-parallel` interface for material properties

The `generic-parallel` interface generates functions evaluating a
material property on a batch of points, on CPUs or on GPUs. See [this
page for details](generic-parallel-material-property-interface.html).

### Available backends

- `stlpar`: parallel algorithms of the C++ standard library, on CPUs or on
  NVIDIA GPUs with `nvc++ -stdpar=gpu`;
- `cuda` and `hip`;
- `sycl`.

### Generated functions

The functions generated for a material property `S` are (version 1 of
those functions, `MFRONT_GPMP_API_VERSION` macro, declared in
`MFront/GenericParallelMaterialProperty/MaterialProperty.h`):

~~~~{.cxx}
int S_generic_parallel(mfront_gpmp_OutputStatus* const,
                       const mfront_gpmp_BatchData* const);
int S_generic_parallel_report(mfront_gpmp_OutputStatus* const,
                              const mfront_gpmp_Statistics* const);
~~~~

- the data of a batch (`mfront_gpmp_BatchData`) give the number of
  points, the output and the arguments (pointer and stride between two
  points), the out of bounds policy, the optional statuses of the points,
  the optional statistics and the execution policy (see the section on
  the execution policies below). Contiguous arrays are treated as a
  special case by the backends.
- the names of the functions end with `_generic_parallel`, and the names
  of the material properties can't contain `_generic_parallel`. A library
  generated by this interface is recognized by the
  `<name>_generic_parallel_api_version` symbol, next to the
  `<name>_generic_parallel_backend`,
  `<name>_generic_parallel_memory_space` and
  `<name>_generic_parallel_execution_policy` symbols.
- the evaluation at each point is the one of the `generic` interface:
  same checks, same status at each point (`0`, `1`, `-1` or `-4`, written
  in the optional array of the statuses of the points), same retained
  check. The output is `NaN` where the evaluation fails, and the computed
  value where it is not finite.
- the output status (`mfront_gpmp_OutputStatus`) gives the numbers of
  points where the evaluation failed and where a bound is violated with
  the warning policy, and the first point of each kind. Its status is the
  error of the batch, otherwise the status of the first point where the
  evaluation failed, otherwise the status of the first point where the
  result is unreliable. The errors of the batch are `-5` (invalid
  arguments), `-6` (invalid parameters) and `-7` (runtime error of the
  backend, including the exceptions thrown on the host by its runtime).
  An empty batch is accepted without any check.
- the kernels only update the statistics of the evaluation when an event
  occurs at a point: with atomic operations (`cuda`, `hip` and `sycl`
  backends), the cost of the error reporting is null when the evaluation
  succeeds. The `stlpar` backend merges the statuses of the points by a
  reduction.
- with statistics given by the calling code, the output status only
  reports the errors of the batch, and the result is given by the
  `<name>_generic_parallel_report` function once the computations are
  completed. If the execution policy requests an asynchronous evaluation,
  the backends with a queue may return before the end of the
  computations.
- the `stlpar` and `sycl` backends have a `device_stack_size` option. The
  `cuda` and `hip` backends increase the stack size of the threads if the
  kernel requires it.
- the automatic data transfers of the `sycl` backend also copy the
  statuses of the points and the statistics.
- the default number of threads per block of the `cuda` and `hip`
  backends is `128`, rounded up to a multiple of the warp size.
- the libraries of several backends for the same material property can be
  loaded in the same process: each library has its own parameters.
- the generated code mostly depends on the material property. The
  backend selects the header and the expression of its primitive
  (`MFront/GenericParallel/GPU.hxx`, `ParallelSTL.hxx` or `SYCL.hxx`),
  the function of the runtime which is called and the symbols describing
  the backend. The generated code holds a function object evaluating
  the material property at one point, the report function and the
  function of batches, which calls the runtime of the interface
  (`MFront/GenericParallelMaterialProperty/Evaluate.hxx`) with this
  primitive. The checks, the launch of the kernels and the report of the
  statistics are written once, in those installed headers, and shared by
  all the backends.

### Execution policies

The generated functions receive an execution policy (`execution_policy`
member of the data of a batch), or a null pointer for the default policy
of the backend (default stream). The policies are `C++` classes of the
`mfront::generic_parallel` namespace, one per backend, declared in the
headers of `MFront/GenericParallel`:

- `CUDABackendExecutionPolicy` and `HIPBackendExecutionPolicy`, which hold
  a stream;
- `SyclBackendExecutionPolicy`, which holds a queue. The `sycl` backend has
  no default policy;
- `ParallelSTLBackendExecutionPolicy`.

All the policies have two common options (`BackendExecutionCommonPolicy`):
`preserve_stack_size` (the stack size of the threads of the device is
never modified) and `asynchronous` (the function may return once the
computations are submitted). An asynchronous computation requires
statistics. A policy of another backend is refused. A policy is created by
a function taking its options, which can be given by designated
initializers:

~~~~{.cxx}
#include "MFront/GenericParallel/CUDABackendExecutionPolicy.hxx"
const auto policy = mfront::generic_parallel::makeCUDABackendExecutionPolicy(
    {.common_options = {.asynchronous = true}, .stream = stream});
d.statistics = statistics;
d.execution_policy = &policy;
~~~~

Each generated library exports the identifier of the policy it expects
(`<name>_generic_parallel_execution_policy` symbol). In `C`, the type of
the policies is opaque (`mfront_gp_AbstractBackendExecutionPolicy`, see
`MFront/GenericParallel/BackendExecutionPolicy.h`): a `C` code can only
select the default policy. The base class of the policies,
`AbstractBackendExecutionPolicy`, is defined by the new
`MFrontGenericParallelRuntime` library (`--mfront-generic-parallel-runtime`
option of `tfel-config`), which must be linked by the codes creating
policies. The generated libraries do not need it. Its table of virtual
functions starts with `getIdentifier` with the ABI of Visual Studio and
with the Itanium ABI: a policy created by a program built by Visual
Studio can be checked by a library generated by a `TFEL` built with
MinGW, and the reverse.

## Improvements to the `Cast3M` interface

### Improved generation of `Gibiane` input file

The `Cast3M` interface generates automatically an example of `Gibiane`
input file for each `MFront` behaviour. This example is
meant to be used as a template and does not pretend to be usable without
modification by the user. For instance, previous versions of `MFront`
did not guarantee that the names of the variables (limited to `4`
characters) were unique. Version 5.2 improves this. To illustrate this,
one may compare the output of Version 5.1 and Version 5.2:

~~~~
* Version prior to 5.2
statev = 'MOTS' 'EERR' 'EEZZ' 'EETT' 'EERZ' 'KHRR' 'KHZZ' 'KHTT' 'KHRZ' 'KHRR' 'KHZZ' 'KHTT' 'KHRZ' 'P';
* Version 5.2
statev = 'MOTS' 'EERR' 'EEZZ' 'EETT' 'EERZ' 'KHRR' 'KHZZ' 'KHTT' 'KHRZ' 'V0' 'V1' 'V2' 'V3' 'P';
~~~~

In Version 5.1, the variable name `KHRR` is declared twice. `Cast3M`
does not generate an error although duplicated names lead to erroneous
results. Version 5.2 avoids this pitfall.

## The `plastic_deformation_gradient_normalisation_corrections` option of the `FiniteStrainSingleCrystal` brick

The `FiniteStrainSingleCrystal` brick normalises the inverse of the
plastic deformation gradient increment by the cubic root of its
determinant. The derivatives of this normalisation with respect to the
plastic slips were explicitly neglected in the jacobian and in the
consistent tangent operator, as numerical tests showed (or at least
seemed to show) that they were negligible.

These derivatives are now taken into account by default (see Issue
#1359). The results are not affected, up to the convergence criteria, but
the convergence is, when the deformation gradient is not fully imposed:
the global iterations recover their quadratic convergence. The boolean
option `plastic_deformation_gradient_normalisation_corrections` allows
neglecting them:

~~~~{.cpp}
@Brick FiniteStrainSingleCrystal{
  plastic_deformation_gradient_normalisation_corrections : false
};
~~~~

The same derivatives are explicitly neglected in the
`FiniteStrainSingleCrystal`, `FiniteStrainSingleCrystal2` and
`FiniteStrainSingleCrystal_NumericalJacobian` test behaviours. Versions
taking them into account are provided by the
`FiniteStrainSingleCrystal_WithCorrections`,
`FiniteStrainSingleCrystal2_WithCorrections` and
`FiniteStrainSingleCrystal_NumericalJacobian_WithCorrections` test
behaviours.

## Generalised porous stress criteria in the `StandardElastoViscoPlasticity` brick {#sec:tfel:5.2:mfront:generalised_porous_stress_criteria}

The `StandardElastoViscoPlasticity` brick provides two new stress
criteria coupled with the porosity evolution:

- `GeneralisedGursonTvergaardNeedleman`,
- `GeneralisedRousselierTanguyBesson`.

They are the `GursonTvergaardNeedleman1982` and
`RousselierTanguyBesson2002` stress criteria in which the von Mises
stress is replaced by an equivalent stress chosen by the user.

This equivalent stress is given by the `equivalent_stress` option. It
must be a stress criterion of the brick with a deviatoric normal, not
coupled with the porosity evolution (`Mises`, `Hill`, `Hosford`,
`Barlat`, etc.).

~~~~{.cxx}
@Brick StandardElastoViscoPlasticity{
  stress_potential : "Hooke" {young_modulus : 200e9, poisson_ratio : 0.3},
  inelastic_flow : "Plastic" {
    criterion : "GeneralisedGursonTvergaardNeedleman" {
      q_1 : 1.5,
      q_2 : 1.0,
      f_c : 0.01,
      f_r : 0.1,
      equivalent_stress : "Hosford" {a : 8}
    },
    isotropic_hardening : "Linear" {R0 : 150e6}
  }
};
~~~~

The original criteria are unchanged. The generalised criteria give the
same results if the equivalent stress is the von Mises stress.

Those criteria are described on [this
page](StandardElastoViscoPlasticityBrick-PorousPlasticity.html).

The associated functions of the `TFEL/Material` library are declared in
the `TFEL/Material/GeneralisedGursonTvergaardNeedlemanStressCriterion.hxx`
and `TFEL/Material/GeneralisedRousselierTanguyBessonStressCriterion.hxx`
headers.

# New features in `mfront-query`

## `--compute-thermal-expansion` and `--compute-stress-free-expansion` queries

The queries `--compute-thermal-expansion` and
`--compute-stress-free-expansion` are now available.

# New features in `tfel-check`

## Parallelization

The commands described in different `.check` files can now be run in
distinct processes.

Note that the commands described in a `.check` file are still run
sequentially.

By default, as in previous versions, only one process is used.

### Specifying the number of jobs run simultaneously

The command line argument `--jobs` (`-j`) allows specifying the number
of jobs run simultaneously.

If no value is given to `--jobs`, the number of available cores
is used, if the system provides this information.

#### Limit on the number of jobs

By default, this number must not exceed the number of available
cores. This limit can be discarded by
passing `--discard-jobs-limit=true` to `tfel-check`.

### Caution

The user must take care that the commands described in `.check` files
can be run in parallel:

- the jobs shall not exhaust memory.
- the jobs shall not access the same resources concurrently.

A typical example of conflict would be to have two `.check` files in the
same directory that call `mfront` to build some shared libraries: the
two `MFront` processes may concurrently write files in the `src` and
`include` directories, leading to undefined behaviour.

Another example would be to use `@CleanDirectories` and `@CleanFiles`
commands inappropriately and erase data used by commands of other
`.check` files.

## Defining environment variables in configuration files

Configuration files can now contain an `environment_variables` section,
which introduces a map defining a set of environment variables.

### Example of usage

~~~~{.cxx}
environment_variables: {"CXXFLAGS": "-Wall"};
~~~~

### Caution

- An environment variable can only be defined once in configuration
  files
- If an environment variable is defined in a configuration file and in a
  test file (through the `@Environment` keyword), the latter definition
  is used.

## Controlling clean up of generated files and directories

The `--clean-up` command line argument allows choosing whether the
generated files and directories listed by the `@CleanFiles` and
`@CleanDirectories` keywords will be cleaned (this is the case by
default) or not in case of success.

~~~~{.bash}
$ tfel-check --clean-up=false
~~~~

The `--discard-clean-up-on-failure` command line argument allows
choosing whether the generated files and directories listed by the
`@CleanFiles` and `@CleanDirectories` keywords will be cleaned or not
(this is the case by default) in case of failure.

~~~~{.bash}
$ tfel-check --discard-clean-up-on-failure=false
~~~~

## Timeout for commands

Commands now have a `timeout` option to limit their duration:

~~~~{.cpp}
@Command "sleep 10"{
 timeout: 1
};
~~~~

This feature requires the `timeout` utility to be available.

# Documentation

## New coding guidelines page

A new page describes the coding conventions of the `C` and `C++`
sources of the project, for new developers as well as for coding
agents: [coding guidelines](coding-guidelines.html).

## New compatibility modes pages

Two new pages describe how compatibility modes are implemented in the
`TFEL` libraries ([compatibility modes in the `TFEL`
libraries](tfel-compatibility-modes.html)) and how they are used by
`MFront` ([compatibility modes in
`MFront`](mfront-compatibility-modes.html)).

# Continuous integration

## GitHub Actions

GitHub Actions have been updated to support a wider range of
configurations, including testing based on `Visual Studio 2022` and
`Visual Studio 2026`.

# Acknowledgements

The authors are grateful to the many contributors to the `TFEL/MFront`
project. This research was conducted in the framework of the PLEIADES
project, which was supported financially by the CEA (Commissariat à
l’Énergie Atomique et aux Énergies Alternatives), EDF (Électricité de
France) and Framatome. Homogenization developments were conducted within
the framework of the AnoHonA ANR project (n° AAPG2023).

# Issues fixed

## Issue 1494: [mfront] getPartialJacobianInvert: out of bounds rows for a tensorial integration variable which is not the first one

For a tensorial integration variable which is not an array, the rows of
the block returned by the `getPartialJacobianInvert` function object
were indexed by the position of the variable among the integration
variables. When such a variable follows another one, the block was
written out of its bounds and left unset. The rows are now indexed from
zero, as they are for the arrays of tensorial variables.

For more details, see <https://github.com/thelfer/tfel/issues/1494>

## Issue 1483: Misspellings of Kirchhoff and Piola

`Kirchoff`, `Kirschoff`, `Piolo` and `Piolay` are replaced by the right
spellings in the names of the functions of `TFEL/Math`, the options of
`MTest`, the tests and the documentation. The former names of the
functions
`convertCauchyStressDerivativeToFirstPiolaKirchoffStressDerivative`,
`convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative`
and `convertFirstPiolaKirchoffStressDerivativeToKirchhoffStressDerivative`
are kept as deprecated functions, and `MTest` still accepts the
`SecondPiolaKirchoffStress` and `FirstPiolaKirchoffStress` stress measures
with a deprecation message. They will be removed in version 6.0.

For more details, see <https://github.com/thelfer/tfel/issues/1483>

## Issue 969: GPU coalescence/broadcast loss when projectors/identities are accessed with warp-divergent runtime index

The `Id`, `IxI`, `K`, `M` and `J` methods of the `st2tost2` class, the
`Id`, `IxI`, `K` and `transpose_derivative` methods of the `t2tot2`
class and the `Id` methods of the `stensor`, `tensor` and `tmatrix`
classes have a new overload which takes the indices of a component and
returns this component without building the object:

~~~~{.cxx}
// same value as Stensor4::M()(i, j)
const auto m = Stensor4::M(i, j);
~~~~

The component has no unit: its type is the base type of the numeric
type of the object.

On GPUs, those overloads avoid storing and reading a table when an
index is only known at run time. The existing methods are unchanged.

For more details, see <https://github.com/thelfer/tfel/issues/969>

## Issue 1414: [mfront] generic-parallel material properties: generated code on shared runtime headers

The code generated by the `generic-parallel` interface for material
properties only evaluates the material property at one point. The checks
of a batch, the launch of the kernels and the report of the statistics
are written once, in installed headers shared by all the backends. The
API of the generated functions is unchanged.

For more details, see <https://github.com/thelfer/tfel/issues/1414>

## Issue 1451: [doc] Keywords cited in the documentation which do not exist (`@StiffnessTensor`, `@ElasticProperties`, `@Parameters`)

The description of the `@OrthotropicBehaviour` keyword now refers to
the `@ComputeStiffnessTensor` and `@ElasticMaterialProperties` keywords.
The example of the manual construction of the interaction matrix of the
single crystal page now uses the `@Parameter` keyword and gives default
values.

For more details, see <https://github.com/thelfer/tfel/issues/1451>

## Issue 1440: [tfel-material] HCP slip systems: the c/a ratio is fixed to the ideal value

The c/a ratio of the `HCP` crystal structure can now be given through
the `c_over_a_ratio` option of the `@CrystalStructure` keyword:

~~~~{.cpp}
@CrystalStructure HCP{c_over_a_ratio : 1.593};
~~~~

The default value is unchanged: it is the one of the ideal structure,
\(\sqrt{8/3}\). The ratio is an argument of a new constructor of the
`tfel::material::SlipSystemsDescription` and `numodis::HCP` classes.

For more details, see <https://github.com/thelfer/tfel/issues/1440>

## Issue 431: Extend the GursonTvergaardNeedleman model to account for other equivalent stresses

The `GeneralisedGursonTvergaardNeedleman` stress criterion of the
`StandardElastoViscoPlasticity` brick allows choosing the equivalent
stress used by the Gurson-Tvergaard-Needleman criterion. The same
extension is available for the Rousselier-Tanguy-Besson criterion
through the `GeneralisedRousselierTanguyBesson` stress criterion.

See Section @sec:tfel:5.2:mfront:generalised_porous_stress_criteria for
details.

For more details, see <https://github.com/thelfer/tfel/issues/431>

## Issue 440: There is no tangent operator for DSL MultipleIsotropicMisesFlows

The behaviours generated by the `MultipleIsotropicMisesFlows` DSL now
provide the consistent tangent operator, whatever the number and the
kind of the flows (`Plasticity`, `Creep`, `StrainHardeningCreep`).
Requesting this operator previously made the integration fail.

This operator is derived from the implicit equations solved by the DSL.
For a single flow, it is the one of the `IsotropicPlasticMisesFlow`,
`IsotropicMisesCreep` and `IsotropicStrainHardeningMisesCreep` DSLs.

The consistent tangent operator is symmetric, unless a specific value of
\(\theta\) is given to one of the flows. In this case, the behaviour is
declared as having a non symmetric tangent operator.

For more details, see <https://github.com/thelfer/tfel/issues/440>

## Issue 617: [mfront] add support for parameter override through a text file for Model DSL

The parameters of a model written with the `Model` domain specific
language can now be modified through an external file with the
`generic`, `castem` and `castem21` interfaces, as the parameters of
behaviours and material properties.

For a model whose class name is `Swelling`, the file is named
`Swelling-parameters.txt` and is read in the current directory at the
first call of the model. Each line gives the name of a parameter and its
value:

~~~~{.txt}
GrowthRate 3e-2
~~~~

The name of this file is returned by the new `--parameters-file` query
of `mfront-query` for models.

This feature is controlled by the `parameters_initialization_from_file`
DSL option, which was already accepted by the `Model` domain specific
language but had no effect.

For more details, see <https://github.com/thelfer/tfel/issues/617>

## Issue 796: Remarks on the documentation of the StandardElastoViscoPlasticity Brick of MFront

The description of the inelastic flows of the
[`StandardElastoViscoPlasticity` brick](StandardElastoViscoPlasticityBrick.html)
now uses the same notations for every flow. The inelastic strain rate is
always written \(\dot{p}\,\tenseur{n}\), where \(\dot{p}\) is the rate
of the equivalent strain and \(\tenseur{n}\) is the flow direction, and
\(f\) always denotes the yield function. Each flow is then described by
the expression of \(\dot{p}\). A section describing this general form
has been added.

For more details, see <https://github.com/thelfer/tfel/issues/796>

## Issue 641: [Miscellaneous] Is a library a MFront library?

`mfm` displayed nothing when its argument was not an existing file, for
instance `Behaviour.dll` in place of `libBehaviour.dll` for a library
built with `MinGW`. Such an argument is now reported on the standard
error stream, unless the `quiet` verbosity level is selected:

~~~~{.bash}
$ mfm Behaviour.dll
mfm: 'Behaviour.dll' is neither a regular file nor a directory
~~~~

The files which can't be analysed are still silently ignored.

For more details, see <https://github.com/thelfer/tfel/issues/641>

## Issue 954: [msys2] the environment scripts are not the good ones (use `.sh.in`, not `.bat.in` or `.ps1.in`)

With a `MinGW` compiler, such as the ones provided by `MSYS2`, only the
`env.bat` and `env.ps1` environment scripts were installed, although the
`MSYS2` environments are used through the `bash` shell.

The `env.sh` and `env.fish` scripts are now also installed in this case,
in the `share/tfel/env` directory. The `env.bat` and `env.ps1` scripts
are still installed, as the binaries may also be used from the `cmd` and
`PowerShell` shells.

For more details, see <https://github.com/thelfer/tfel/issues/954>

## Issue 955: [msys2] `python` modules are not installed in the right places: one shall follow Unix conventions

With a `MinGW` compiler, such as the ones provided by `MSYS2`, the
`Python` modules were installed in the
`bin/python<major>.<minor>/site-packages` directory, which is the
directory used on `Windows` with `Visual Studio`. The `Python`
interpreter of `MSYS2` follows the Unix directory layout and does not
look for modules in this directory.

The `Python` modules are now installed in the
`lib/python<major>.<minor>/site-packages` directory in this case. The
`TFEL_PYTHON_SITE_PACKAGES_DIR` `cmake` variable still allows choosing
another directory.

For more details, see <https://github.com/thelfer/tfel/issues/955>

## Issue 1411: [TFEL/PhysicalConstants] Use CODATA 2022 for 6.0 and the `future` compatibility mode of 5.2.0

The values of the physical constants of the `6.0.0` compatibility mode,
which are those of the next major version, are now the `CODATA` 2022
internationally recommended values, in place of the `CODATA` 2014 ones.
Those values are available through the `tfel::v6_0_0` and `tfel::future`
namespaces in `C++`, through the `tfel.v6_0_0` and `tfel.future`
submodules in `Python` and through the `future` compatibility mode in
`MFront`.

The values used by default, which are those of the `5.2.0` compatibility
mode, are unchanged.

The `Fortran` bindings are versioned as well. The
`TFELPHYSICALCONSTANTS-v6_0_0.INC` and `TFELPHYSICALCONSTANTS-future.INC`
include files and the `TFEL_PHYSICAL_CONSTANTS_V6_0_0` and
`TFEL_PHYSICAL_CONSTANTS_FUTURE` modules expose the `CODATA` 2022 values.
The `TFELPHYSICALCONSTANTS.INC` include file and the
`TFEL_PHYSICAL_CONSTANTS` module are equivalent to the
`TFELPHYSICALCONSTANTS-v5_2_0.INC` include file and to the
`TFEL_PHYSICAL_CONSTANTS_V5_2_0` module: their values are those of the
previous versions.

The initial guess of the gas equation of state solver of `MTest` now
uses the molar gas constant of the `TFEL/PhysicalConstants` library, in
place of a hard-coded value.

For more details, see <https://github.com/thelfer/tfel/issues/1411>

## Issue 1417: Investigate tests failures with `gcc` and `-ffast-math`

With the `-ffast-math` flag, the compiler assumes that no `NaN` and no
infinite value exist, treats the subnormal numbers as zeros and may
evaluate both branches of a test. The following points are fixed:

- `tfel-check` detects the `NaN` in the results.
- `MTest` detects the non finite results in the analytical tests and in
  the comparisons to reference files.
- `tfel::utilities::convert<double>` and
  `tfel::utilities::convert<long double>` reject the numbers which
  overflow.
- The Jacobi eigen solver (`FSESJACOBIEIGENSOLVER`) returns `NaN` for a
  tensor holding a `NaN`, as the other eigen solvers.
- The default 2D kriging model can be evaluated at one of its data
  points when the floating-point exceptions are enabled.

The tolerances of the `IsotropicCazacuYieldTest`,
`HosfordSecondDerivativeTest` and `IsotropicModuli` tests and the
criterion on the stresses of the `HyperbolicSineTest2` `MTest` test are
now consistent with the accuracy of the algorithms they rely on.

For more details, see <https://github.com/thelfer/tfel/issues/1417>

## Issue 1416: Fix tests failures with intel `icpx` 2024

The `icpx` compiler enables by default a fast floating-point model
(`-fp-model=fast`), which allows value-unsafe optimisations and flushes
the subnormal numbers to zero. The `-fno-fast-math` flag is now passed
to this compiler when the `enable-fast-math` option is not set, as
already done for `gcc`. The `StringAlgorithms`,
`LinearHomogenizationSchemes` and `IsotropicModuli` tests failed in
`Release` mode without this flag.

Those tests no longer depend on the floating-point model. They also
pass with the default model of `icpx` and with the `-ffast-math` flag of
`gcc`, `clang` and `icpx`:

- The `tfel::utilities::convert` function detects that a floating-point
  number overflows from the magnitude of the value returned by
  `std::strtod` or `std::strtold`, in place of testing if this value is
  infinite. A number which overflows is thus also rejected when the
  rounding mode is not `to nearest`: it was previously converted to the
  largest finite number.
- The `StringAlgorithms` test compares the representations of the
  converted values.
- The `LinearHomogenizationSchemes` test uses a relative tolerance.

For more details, see <https://github.com/thelfer/tfel/issues/1416>

## Issue 1387: [TFEL/Utilities] tests `CxxTokenizer` with `libFuzzer`

The `tfel::utilities::CxxTokenizer` class is now covered by `libFuzzer`
based analyses, which are enabled by the `enable-libfuzzer-tests`
option, and by the `CxxTokenizerContractTest` unit test, which checks
the contract of the tokens for the lexical constructs that were not
covered by the existing tests of the tokenizer: raw string literals,
empty comments, comments of a line and numbers with digit separators.

For more details, see <https://github.com/thelfer/tfel/issues/1387>

## Issue 1379: [mfront] Chaboche 2012 kinematic hardening rule: are the differences with the literature for m != 1 intended?

For more details, see <https://github.com/thelfer/tfel/issues/1379>

## Issue 932: [mtest] Document modeling of pipes

For more details, see <https://github.com/thelfer/tfel/issues/932>

## Issue 1376: [mfront-doc] Formula of the Chaboche 2012 kinematic hardening rule differs from the code for m != 1

For more details, see <https://github.com/thelfer/tfel/issues/1376>

## Issue 1359: [mfront] FiniteStrainSingleCrystal: corrections of the derivatives of the normalised plastic deformation gradient increment, test behaviours and option of the brick

For more details, see <https://github.com/thelfer/tfel/issues/1359>

## Issue 1273: [tfel-check] Warning when `@Precision` with one value keeps the second precision of a previous `@Precision`

For more details, see <https://github.com/thelfer/tfel/issues/1273>

## Issue 1371: [mtest] Generalised plane strain elastic stiffness used with the generic plane stress algorithm of the castem wrappers

For more details, see <https://github.com/thelfer/tfel/issues/1371>

## Issue 1332: [mfront] type identifiers of the derivatives of tensorial type aliases (derivative_type<StressStensor, StrainStensor>) are decoded as another type by MTest and MGIS

For more details, see <https://github.com/thelfer/tfel/issues/1332>

## Issue 1330: [docs] StrainMeasure options, MTest prediction policies and keywords, implicit DSL tangent operator, tfel-check options, crystal plasticity, acceleration algorithms, cyrano generalised plane stress

For more details, see <https://github.com/thelfer/tfel/issues/1330>

## Issue 1326: [mfront] behaviour interfaces: Hencky strain in plane stress, calculix orthotropic logarithmic strain, MTest files of abaqus and ansys, cyrano NTENS, castem and abaqus examples, negative unsigned values of the DSLs

For more details, see <https://github.com/thelfer/tfel/issues/1326>

## Issue 1324: [tfel-material] GTN equivalent stress for small porosities, Norton-Hoff harmonic sum derivative, criteria with quantities, Schmid factors of cubic crystals, documentation of criteria and nucleation models

For more details, see <https://github.com/thelfer/tfel/issues/1324>

## Issue 1321: [tfel-math, tfel-system] formula parsers, raw streams, recursiveFind, ThreadPool, Python tfel.system: non-integer sizes, missing derivatives, nested external functions, ln and H in C++, pointers, short writes, symbolic links

For more details, see <https://github.com/thelfer/tfel/issues/1321>

## Issue 1319: [mfront] RungeKuttaFiniteStrain with the deformation gradient, truncated files, command-line keywords of mtest, unsigned parameters, MaterialLaw bounds, Python mfront module, empty configuration options, tfel-config of the CMake generator

For more details, see <https://github.com/thelfer/tfel/issues/1319>

## Issue 1317: [tfel-math] Include guard of runtime_array, assignment from views arrays, strided indexing, Runge-Kutta final time and null step, scalarNewtonRaphson last iterate, QRDecomp of wide matrices, Levenberg-Marquardt with one parameter, abs of vector expressions

For more details, see <https://github.com/thelfer/tfel/issues/1317>

## Issue 1313: [mtest] Prediction of finite strain behaviours, derivative types with a scalar side, plane stress messages, time step scaling of generic behaviours, UAnderson, PTest mandrel, mfm-test-generator

For more details, see <https://github.com/thelfer/tfel/issues/1313>

## Issue 1311: [mfront] Material properties given by formulas or external laws do not compile with `@UseQt`

For more details, see <https://github.com/thelfer/tfel/issues/1311>

## Issue 1309: [mfront] Bricks: staggered porosity scheme, ScalarDamage plane stress, DDIF2 tangent operator, FiniteStrainSingleCrystal derivatives, saved criteria, Data extrapolation, untested components

For more details, see <https://github.com/thelfer/tfel/issues/1309>

## Issue 1307: [mfront] UMAT-like interfaces: Ansys finite strain shear components, plane stress expansions, time step scaling factors, ZMAT bounds, Ansys energies, Abaqus/Explicit exceptions

For more details, see <https://github.com/thelfer/tfel/issues/1307>

## Issue 1305: [mfront] Material property and model interfaces: validator symbols, octave static parameters, castem bounds and models, gnuplot `@Graph`, java with MSVC, bounds of model outputs, out of bounds policy of generic models

For more details, see <https://github.com/thelfer/tfel/issues/1305>

## Issue 1303: [mfront] Implicit DSLs: inverse jacobian blocks, normalisation factors, reference temperature of external files, UserDefined and Broyden2 algorithms, reserved names

For more details, see <https://github.com/thelfer/tfel/issues/1303>

## Issue 1301: Fixes of the 5.2 review: incomplete fixes and regressions in merged code

For more details, see <https://github.com/thelfer/tfel/issues/1301>

## Issue 1299: TFEL/Material homogenisation: the stiffness of oriented anisotropic inclusions and grains, given in their local basis, is used as a global stiffness

For more details, see <https://github.com/thelfer/tfel/issues/1299>

## Issue 1295: [tfel-math] The Broyden solvers never update the jacobian in MFront behaviours

For more details, see <https://github.com/thelfer/tfel/issues/1295>

## Issue 1292: [tfel-material] LogarithmicStrainHandler: wrong tangent moduli in 3D when two eigenvalues of C are equal

For more details, see <https://github.com/thelfer/tfel/issues/1292>

## Issue 1290: [mtest] Off-diagonal deformation gradient equations and imposed off-diagonal stresses at finite strain, Broyden test behaviours overwriting the decomposed jacobian, implicitmonocrystal tests running MonoCrystal, elastic stiffness of the castem generic plane stress algorithm

For more details, see <https://github.com/thelfer/tfel/issues/1290>

## Issue 1288: [tfel-math] Adaptive Gauss-Kronrod integration with quantities, last correction of the tiny non-linear solvers, Levenberg-Marquardt steps increasing the cost, ExternalLibraryManager library path, contract violations aborting Python

For more details, see <https://github.com/thelfer/tfel/issues/1288>

## Issue 1285: [mfront] generic interface, finite strain in plane stress: the Green-Lagrange and standard finite strain families add the axial deformation gradient to the one of the caller

For more details, see <https://github.com/thelfer/tfel/issues/1285>

## Issue 1283: [mfront] MTest files generated on failure by the generic interface cannot be replayed in plane stress, and in plane strain at finite strain

For more details, see <https://github.com/thelfer/tfel/issues/1283>

## Issue 1281: [mfront] The generated Makefile adds the calling directory, not src, to LD_LIBRARY_PATH

For more details, see <https://github.com/thelfer/tfel/issues/1281>

## Issue 1274: [mfront] abaqusexplicit, MFront orthotropy policy: the integration applies the inverse rotation, second axis read beyond the orientation in 2D

For more details, see <https://github.com/thelfer/tfel/issues/1274>

## Issue 1270: [mtest] `@RotationMatrix<Euler>` applies the inverse rotation, convention of `@RotationMatrix` undocumented, rotation matrix of the generic interface described by its transpose

For more details, see <https://github.com/thelfer/tfel/issues/1270>

## Issue 1264: [tfel] Installed headers that cannot be compiled, 2D microstructure-based homogenisation, Windows installation pages

For more details, see <https://github.com/thelfer/tfel/issues/1264>

## Issue 1262: [tfel-math] Second derivative of the determinant of a non-symmetric tensor, const scalar views of quantities, truncated exponent in the derivative of a**b

For more details, see <https://github.com/thelfer/tfel/issues/1262>

## Issue 1260: [mtest] Abaqus/Explicit stiffness in a rotated frame, incomplete orthotropic thermal expansion, tfel-check second precision, failed tangent operator comparison, overwritten JUnit reports

For more details, see <https://github.com/thelfer/tfel/issues/1260>

## Issue 1258: [mfront] generic MTest files of finite strain behaviours, cpptest and gnuplot interfaces, calculix MieheApelLambrechtLogarithmicStrainII strategy, dianafea failures and orthotropic behaviours, abaqus glue lock on Windows

For more details, see <https://github.com/thelfer/tfel/issues/1258>

## Issue 1256: [mfront] Model overriding parameters, bounds on main variables and on components, ScalarDamage in axisymmetrical generalised plane stress, keyword pages, Ksf of the harmonic sum flow, brick base classes exports, mfront-doc/mfront-query global options, MFrontLock

For more details, see <https://github.com/thelfer/tfel/issues/1256>

## Issue 1254: [tfel-math] Eigen solvers of symmetric tensors: wrong eigenvectors for equal or close eigenvalues, Cardano eigenvalues of syevc3, derivatives of isotropic functions of quantities, ConvertLogarithmicStrainTangentOperator

For more details, see <https://github.com/thelfer/tfel/issues/1254>

## Issue 1251: [cmake] Installed executables, libraries and Python modules without RUNPATH on Linux, madnex library not found, empty entries added by env.sh

For more details, see <https://github.com/thelfer/tfel/issues/1251>

## Issue 1242: [tests] Twenty-one C++ unit tests check no result

For more details, see <https://github.com/thelfer/tfel/issues/1242>

## Issue 1241: [ptest] Thirty of the 42 PTest pipe tests compare nothing

For more details, see <https://github.com/thelfer/tfel/issues/1241>

## Issue 1239: [cmake] Obsolete packaging files, TFEL_OPTIMISATION_FLAGS0, Debug flags, checks that never act, Cast3M 2026 header, minimum CMake version, exports of libTFELMFront

For more details, see <https://github.com/thelfer/tfel/issues/1239>

## Issue 1237: [cmake] Compiler printed by mfront --version, installed headers of the isotropic DSLs and of TFELCheck, dependencies of the installed CMake packages, static TFELMFrontDatabase package, CPack components

For more details, see <https://github.com/thelfer/tfel/issues/1237>

## Issue 1235: [tests] Copy-paste slips and vacuous checks in unit tests

For more details, see <https://github.com/thelfer/tfel/issues/1235>

## Issue 1233: [mfront] MFront DSL unit tests accept any exception

For more details, see <https://github.com/thelfer/tfel/issues/1233>

## Issue 1231: [tests] Commented-out cases in the Cazacu, LocalDataStructure, CxxTokenizerOffset and eigenvectors unit tests

For more details, see <https://github.com/thelfer/tfel/issues/1231>

## Issue 1229: [mfront] Expected-failure tests that fail for an unrelated reason

For more details, see <https://github.com/thelfer/tfel/issues/1229>

## Issue 1227: [mfront] Comparisons of the finite strain single crystal tests are commented out

For more details, see <https://github.com/thelfer/tfel/issues/1227>

## Issue 1225: [mtest] The fanderson and uanderson tests do not select their acceleration algorithm

For more details, see <https://github.com/thelfer/tfel/issues/1225>

## Issue 1223: [mtest] Thirty test files give the same variable twice in a `@Test` map

For more details, see <https://github.com/thelfer/tfel/issues/1223>

## Issue 1221: [mfront] Logarithmic strain shear tests: the shear-FZY-3D tests impose FZX, and the transposed cases compare too few stresses

For more details, see <https://github.com/thelfer/tfel/issues/1221>

## Issue 1219: [mfront] Strain tolerance of the axisymmetric shear tests of orthotropic elasticity

For more details, see <https://github.com/thelfer/tfel/issues/1219>

## Issue 1217: [tfel-glossary] Types, units and short descriptions of some glossary entries

For more details, see <https://github.com/thelfer/tfel/issues/1217>

## Issue 1215: [mfront] Installed GenericData and LSDYNA explicit headers, undefined declarations of the code generator and specific targets, madnex search paths documentation, pedantic bounds of parameters, castem21 keyword restrictions

For more details, see <https://github.com/thelfer/tfel/issues/1215>

## Issue 1213: [mfront] `@RequireThermalExpansionCoefficientTensor` is unusable: missing alias, documented syntax, castem finite strain, stype of the other interfaces

For more details, see <https://github.com/thelfer/tfel/issues/1213>

## Issue 1211: [tfel-system] Undefined SignalManager and NUMODIS declarations, MTestParser and ProcessManagerCommand constructors, mtest reference file generator, terminate handler of the tools, BurnUp glossary name in the docs

For more details, see <https://github.com/thelfer/tfel/issues/1211>

## Issue 1209: [tfel-math] Variable positions of Evaluator in diff() and tfel-check, fses::sort, vector::copy, GenType output, GTN flow stress, Mohr-Coulomb second derivative, eigen solvers on GPUs

For more details, see <https://github.com/thelfer/tfel/issues/1209>

## Issue 1207: [mfront] Logarithm of the stress-free expansion stretch (abaqus, ansys, calculix, lsdyna), castem tangent operators in 1D and with FiniteRotationSmallStrain, cyrano hypothesis identifier, repeated `@Test` keys

For more details, see <https://github.com/thelfer/tfel/issues/1207>

## Issue 1205: [mfront] comsol error codes and initialisation, aster orthotropic cohesive zone models, bounds symbols of array elements, cyrano plane stress header, zmat arrays of tensors

For more details, see <https://github.com/thelfer/tfel/issues/1205>

## Issue 1203: [tfel-math] Isotropic moduli assignment, stensor products on GPUs, Gauss-Kronrod half-infinite error estimate, QR with more rows than columns, Levenberg-Marquardt factor, physical constants digits

For more details, see <https://github.com/thelfer/tfel/issues/1203>

## Issue 1201: [tfel-utilities] CxxTokenizer numbers and integers, string algorithms, TextData comments and infinities, ProcessManager and SignalManager children, tfel-doc --split

For more details, see <https://github.com/thelfer/tfel/issues/1201>

## Issue 1199: [mfront] Code block option ".+", `@PhysicalBounds` on array elements, thermal expansion options, external files registered twice, isotropic DSLs last iteration, bricks and generic behaviours with quantities

For more details, see <https://github.com/thelfer/tfel/issues/1199>

## Issue 1197: [mtest] Acceleration algorithm parameters, Anderson method orders above 22, castem_umat optional material properties, mtest Python options

For more details, see <https://github.com/thelfer/tfel/issues/1197>

## Issue 1195: [mfront] madnex search paths: file extension, behaviours and models, knowledge type; madnex export of shared dependencies

For more details, see <https://github.com/thelfer/tfel/issues/1195>

## Issue 1193: [mtest] End of input in the MTest and PTest parsers, ansys input file example of orthotropic behaviours, misleading interface messages

For more details, see <https://github.com/thelfer/tfel/issues/1193>

## Issue 1191: [mfront-query] Names of the DSL options in the help, strain measure output, specific targets rules and help texts

For more details, see <https://github.com/thelfer/tfel/issues/1191>

## Issue 1189: [mfront] Cazacu Pipe convention, StrainRateSensitive derivative, `@Initialize`, command line keywords, StringArray defaults, array bounds, log stream, fortran03/python/octave/java interfaces and parser messages

For more details, see <https://github.com/thelfer/tfel/issues/1189>

## Issue 1187: [mfront] castem logarithmic strain helpers, stensor absolute value, DSLBase helpers, FSES return codes, TestDescription constructor, exported constants precision, tfel-check columns and ABAQUSEXPLICIT_NTHREADS

For more details, see <https://github.com/thelfer/tfel/issues/1187>

## Issue 1185: [mtest] dianafea wrapper shear order, tfel-check JUnit escaping, raw stream readers, Python docstrings, documentation links and tutorial examples

For more details, see <https://github.com/thelfer/tfel/issues/1185>

## Issue 1183: [cmake] Static library names, test dependencies, C++ standard, static MFront package, environment scripts, Doxygen install, installed MFront headers and exports

For more details, see <https://github.com/thelfer/tfel/issues/1183>

## Issue 1181: [python] mfront.database entry points, mtest initial values and depths, tfel.material imports, mfm-test-generator, mfm --filter-by-type and MTest files on failure

For more details, see <https://github.com/thelfer/tfel/issues/1181>

## Issue 1179: [mfront] Wrong or misleading error messages and documentation of DSLs, tools, material property interfaces, stress criteria, MTest bindings and tfel-check

For more details, see <https://github.com/thelfer/tfel/issues/1179>

## Issue 1177: [mfront] ansys, comsol, dianafea, castem, aster and cyrano interfaces: material properties layout, tangent operator conventions, 2D sizes, plane stress checks, Gibiane examples and messages

For more details, see <https://github.com/thelfer/tfel/issues/1177>

## Issue 1175: [installation] Doxygen deployment, keyword descriptions, installation pages, maintainer scripts, generic interface and math docs, quantity tests option, physical constants and glossary generators

For more details, see <https://github.com/thelfer/tfel/issues/1175>

## Issue 1173: [mfront] abaqus and amitex input examples, cyrano header, ansys/lsdyna/dianafea example files and zmat interface

For more details, see <https://github.com/thelfer/tfel/issues/1173>

## Issue 1171: [mfront-query] File extensions of mfront.write and mtest.write, mfront-query lookups, bounds and specific targets, mfront-doc descriptions and constants, InelasticFlowBase include

For more details, see <https://github.com/thelfer/tfel/issues/1171>

## Issue 1169: [tfel-tests] tfel-doc output, log and colors, TFEL/Tests XML output and exports, copy-paste slips, JUnit report names, TextData::skipLines, FSAlgorithm loop and strided iterator assignment

For more details, see <https://github.com/thelfer/tfel/issues/1169>

## Issue 1167: [tfel-utilities] CxxTokenizer, Data arrays, reserved keywords, systemCall::copy, wfstream, TFEL/System messages and tfel-check end of file and empty_output

For more details, see <https://github.com/thelfer/tfel/issues/1167>

## Issue 1165: [tfel-math] Headers and parser: parenthesised conditions, stensor setComponent/getComponent, isqrt3 with MSVC, square_root of quantities, GPU attribute, ATLAS binding, HasRandomAccessIterator, self-contained material headers

For more details, see <https://github.com/thelfer/tfel/issues/1165>

## Issue 1163: [python] Bindings: CurrentStateVector, tfel.math operators, versioned packages, CMakeGenerator, return value policies, Hill tensors, DSLFactory and mergeTargetsDescription

For more details, see <https://github.com/thelfer/tfel/issues/1163>

## Issue 1161: [mtest] MTest and PTest: ptest keywords help, array component names, orthotropic generic stress, mistral defaults, PTest output and jacobian, algorithm names, installed and exported classes

For more details, see <https://github.com/thelfer/tfel/issues/1161>

## Issue 1159: [tfel-material] Finite strain tangent operators, logarithmic strain handler, Hosford/Barlat with quantities, bounds checks, Gauss-Kronrod, broyden, parser and LU messages

For more details, see <https://github.com/thelfer/tfel/issues/1159>

## Issue 1157: [mfront] castem, aster, abaqus, ansys and abaqusexplicit interfaces: LogarithmicStrain1D, sub-stepping, 2D finite strain fallbacks, tangent operator layouts and numerical comparison

For more details, see <https://github.com/thelfer/tfel/issues/1157>

## Issue 1155: [mfront] DSLs and generated code: modelling hypotheses intersection, braceless assignments, output operators, brick symmetries, code blocks, error messages and debug comments

For more details, see <https://github.com/thelfer/tfel/issues/1155>

## Issue 1153: [tfel-math] Quantities and views: qt_ref assignment, change_basis, positive parts, runtime matrix views, arrays of views, tmatrix products, FSES NaN, Hashin-Shtrikman shear bounds, Anderson weights

For more details, see <https://github.com/thelfer/tfel/issues/1153>

## Issue 1151: [mfront] Generated code: integration variable derivatives, tangent operator blocks, generic gradients, relocation model, brick arrays, MultipleIsotropicMisesFlows, behaviour variables and ℝ₊ bounds

For more details, see <https://github.com/thelfer/tfel/issues/1151>

## Issue 1149: [mtest] Initial elastic stiffness of Abaqus/Explicit behaviours: halved shear terms and 3D shear columns

For more details, see <https://github.com/thelfer/tfel/issues/1149>

## Issue 1147: [tfel-math] Formula parsers, kriging and LUSolve: copy and arguments of diff, Cast3M functions without argument, integer division by zero, rebuilt and empty kriging, work vector size

For more details, see <https://github.com/thelfer/tfel/issues/1147>

## Issue 1145: [tfel-math] Headers: abs of t2tost2, st2tost2 conversion in 1D, eigen tensors of quantities, tmatrix, array derivatives, copy, strided maps and indexing policies

For more details, see <https://github.com/thelfer/tfel/issues/1145>

## Issue 1143: [mfront] DSLs: bricks, behaviour variables, `@TangentOperator`, initialisation values, targets.lst, overriding increments and bounds

For more details, see <https://github.com/thelfer/tfel/issues/1143>

## Issue 1141: [tfel-utilities] tfel-check shall_fail and directory configurations, rstream::getLine, CxxTokenizer, Data reading and comparison, ArgumentParser

For more details, see <https://github.com/thelfer/tfel/issues/1141>

## Issue 1139: [python] Bindings: evtypes, ST2toST2 subscripts, homogenisation bounds, setRotationMatrix, setStrainMeasure, umatxx, Anderson algorithms and Data

For more details, see <https://github.com/thelfer/tfel/issues/1139>

## Issue 1137: [mfront] Generated code: amitex template, arrays of state variables and gradients, profiling, `@Relocation`, rotation functions and MTest files on failure

For more details, see <https://github.com/thelfer/tfel/issues/1137>

## Issue 1135: [tfel-material] NUMODIS and slip systems: Schmid factors, installed headers, exports, HCP families and symmetries

For more details, see <https://github.com/thelfer/tfel/issues/1135>

## Issue 1133: [tfel-math] Powell dog leg solvers, scalar Newton-Raphson, TinyMatrixSolve/Invert, CubicRoots, runtime_array output and Fischer-Burmeister derivatives

For more details, see <https://github.com/thelfer/tfel/issues/1133>

## Issue 1131: [mfront] Hooke stress potentials: plane stress jacobian, generic_prediction_operator option and broken state tests

For more details, see <https://github.com/thelfer/tfel/issues/1131>

## Issue 1129: [mtest] Wrappers of the generic, aster, castem, ansys, abaqusexplicit and castem_umat behaviours

For more details, see <https://github.com/thelfer/tfel/issues/1129>

## Issue 1127: [tfel-system] rmdir following symbolic links, mkdir, binary_read, ThreadPool, ThreadedTaskResult, pipes and ELF symbols

For more details, see <https://github.com/thelfer/tfel/issues/1127>

## Issue 1125: [mfront] Tangent operators of cohesive zone models (castem, aster, MTest) and calculix interface

For more details, see <https://github.com/thelfer/tfel/issues/1125>

## Issue 1123: [mfront] StandardElastoViscoPlasticity brick: flow identifiers, flow criteria coefficients, theta factors and hardening rules

For more details, see <https://github.com/thelfer/tfel/issues/1123>

## Issue 1121: [tfel-material] Hill tensors, orientation averages, Plate convention and altered stiffness tensors in axisymmetrical generalised plane stress

For more details, see <https://github.com/thelfer/tfel/issues/1121>

## Issue 1119: [mfront] ansys and abaqusexplicit interfaces: finite strain strategies, plane stress, packaging and parallelisation

For more details, see <https://github.com/thelfer/tfel/issues/1119>

## Issue 1117: [mfront] Interfaces of material properties and models: exports, one-line functions, bounds, temperature of castem models and comsol

For more details, see <https://github.com/thelfer/tfel/issues/1117>

## Issue 1115: [tfel-math] Eigenvalue sorting, Harari solver, positive/negative parts, convert, tpld and fromRotationMatrix on GPU

For more details, see <https://github.com/thelfer/tfel/issues/1115>

## Issue 1113: [mfront] StandardElastoViscoPlasticity brick: nucleation, porosity name, porosity jacobian and porous stress criteria

For more details, see <https://github.com/thelfer/tfel/issues/1113>

## Issue 1111: [mtest] Crash without behaviour, time step at the end of a period, events, NaN results and cubic pipe elements

For more details, see <https://github.com/thelfer/tfel/issues/1111>

## Issue 1109: [mfront] Runge-Kutta DSLs and solvers: rk4 last stage, external models, error estimates and final time

For more details, see <https://github.com/thelfer/tfel/issues/1109>

## Issue 1107: [mfront] Normalisation factors, physical bounds and MultipleIsotropicMisesFlows in the implicit and isotropic DSLs

For more details, see <https://github.com/thelfer/tfel/issues/1107>

## Issue 1105: [docs] Examples rejected by mfront and mtest, wrong keyword names and broken links

For more details, see <https://github.com/thelfer/tfel/issues/1105>

## Issue 1103: [tfel-math] Crash, operator precedence, derivative and integer literal errors of the formula parsers

For more details, see <https://github.com/thelfer/tfel/issues/1103>

## Issue 1101: [tfel-material] Wrong second derivatives, NaN and single precision build errors of several stress criteria

For more details, see <https://github.com/thelfer/tfel/issues/1101>

## Issue 1099: [mfront] Wrong shear components of stress-free expansions in several behaviour interfaces

For more details, see <https://github.com/thelfer/tfel/issues/1099>

## Issue 1097: [tfel-math] `x /= 2` sets every component of arrays and views to zero

For more details, see <https://github.com/thelfer/tfel/issues/1097>

## Issue 1095: [mfront] `rk54` and `rk4` compute the final stress with the stiffness tensor of the middle of the time step

For more details, see <https://github.com/thelfer/tfel/issues/1095>

## Issue 1093: [5.2] Regressions since TFEL 5.1

For more details, see <https://github.com/thelfer/tfel/issues/1093>

## Issue 1074: [mfront] generic-parallel material properties: API of the generated functions

For more details, see <https://github.com/thelfer/tfel/issues/1074>

## Issue 1072: Add support for `pybind11_stubgen`

The `SpecificTargetDescription` class is now available in the `mfront`
python module, so that the `specific_targets` member of the
`TargetsDescription` class can be read. The libraries exported to `CMake`
require the `C++20` standard, which is thus enabled in the projects using
them.

For more details, see <https://github.com/thelfer/tfel/issues/1072>

## Issue 1089: Add debug mode + hardening in the test matrix

The sanitizers options have been modularised to allow the continuous
integration to enable sanitizers individually.

For more details, see <https://github.com/thelfer/tfel/issues/1089>

## Issue 1038: [tfel-check] don't execute cleaning tasks when a test fails

For more details, see <https://github.com/thelfer/tfel/issues/1038>

## Issue 1037: [tfel-check] add a `--clean` option to select whether or not cleaning is done

For more details, see <https://github.com/thelfer/tfel/issues/1037>

## Issue 1036: [tfel-check] Add timeout option

For more details, see <https://github.com/thelfer/tfel/issues/1036>

## Issue 1023: [tfel-config] add an option `--cmake-prefix-path`

For more details, see <https://github.com/thelfer/tfel/issues/1023>

## Issue 1016: [mfront] Variables have the same name in the `dgibi` file generated by `MFront`, for using the FEM program `Cast3M`

For more details, see <https://github.com/thelfer/tfel/issues/1016>

## Issue 1014: [cmake] Add a `build-tests` target
 
For more details, see <https://github.com/thelfer/tfel/issues/1014>

## Issue 1013: [tfel-math] Refine `QuantityConcept` to take mutability into account

For more details, see <https://github.com/thelfer/tfel/issues/1013>

## Issue 1012: [github-actions] Add testing of `TFEL` with `madnex`

For more details, see <https://github.com/thelfer/tfel/issues/1012>

## Issue 1010: [clang-tidy] fix warnings related to the google-explicit-constructor check

For more details, see <https://github.com/thelfer/tfel/issues/1010>

## Issue 1006: [github-actions] Add tests based on `tfel-check`

For more details, see <https://github.com/thelfer/tfel/issues/1006>

## Issue 1004: [github-actions] Add support actions based on Visual Studio

For more details, see <https://github.com/thelfer/tfel/issues/1004>

## Issue 1003: [tfel-math] Improve error messages when using quantities

For more details, see <https://github.com/thelfer/tfel/issues/1003>

## Issue 1001: [tfel-math] Introduce the `QuantityConcept` class to allow surrogate classes to be treated as quantities

For more details, see <https://github.com/thelfer/tfel/issues/1001>

## Issue 995: [tfel-check] Adding `@Environment` section in configuration file

For more details, see <https://github.com/thelfer/tfel/issues/995>

## Issue 987: `StridedCoalescedView` has no exposed data pointer and stride

For more details, see <https://github.com/thelfer/tfel/issues/987>

## Issue 926: [tfel-math] Add `TFEL_HOST_DEVICE` to `LeftCauchyGreenTensorDerivativeExpr` and `RightCauchyGreenTensorDerivativeExpr` classes

For more details, see <https://github.com/thelfer/tfel/issues/926>

## Issue 925: [tfel-math] Introduce the `StressStensorConcept` and `NoUnitStensorConcept` concepts and siblings for orther tensorial objects

For more details, see <https://github.com/thelfer/tfel/issues/925>

## Issue 922: [tfel-math] Improve `convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchhoffStressDerivative`

For more details, see <https://github.com/thelfer/tfel/issues/922>

## Issue 890: [mfront, homogenization] Add Ponte Castaneda 1992 non linear scheme

For more details, see <https://github.com/thelfer/tfel/issues/890>

## Issue 905: [TFEL/Math] Pointer-heavy CoalescedView

For more details, see <https://github.com/thelfer/tfel/issues/905>

## Issue 853: [tfel-check] Have the possibility to launch multiple call to `tfel-check` simultaenously in a CI context

For more details, see <https://github.com/thelfer/tfel/issues/853>
