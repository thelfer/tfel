# Backport campaign rliv-5.0 -> rliv-4.2: summary of the backported PRs

Date: 2026-11-01. Branch `backrports-from-5.0`, on top of `rliv-4.2`.
Task: apply to this 4.2.x tree the fixes collected in
[PR #1268](https://github.com/thelfer/tfel/pull/1268) (Backport of defect
fixes from rliv-5.1, itself
[PR #1267](https://github.com/thelfer/tfel/pull/1267): master PRs #1096 -
#1263). The authoritative change set is `git diff 418aaa0ff 2ca7f3814` on
the 5.0.x branch `backrports-from-5.1` (612 files, +10529/-4690); the
branch already carried a first batch (`667b5f7fd`, the fixes of
`07a160f18`). Companion file: `backports.md` (per-file working ledger and
merge decisions).

## 1. Final state and evidence

| | |
|---|---|
| PRs processed | 58 / 58 (none skipped wholesale), the same list as section 2 of PR #1268 |
| Working tree | 528 changed tracked paths (+4903/-3960, including the 4 deletions performed by the change set itself) plus 21 new test/data files, all registered |
| File-level classification | 439 applied cleanly; 22 already applied (by the first batch `667b5f7fd`); 90 merged manually (24 with `git apply --3way`, audited line by line, and 66 resolved by hand); 21 new files; 4 deletions; 38 missing-path files documented as not applicable |
| Final validation | `cmake . && make -j32 check` in the configured build tree: `100% tests passed, 0 tests failed out of 20274` (611 s) |
| Follow-up audit | every line the change set removes was searched for in the touched files: 0 hits |
| C++17 conformance | 4.2 still builds with C++17: the diff introduces no `requires`, `std::remove_cvref_t`, `Concept auto`, `std::span`/`std::ranges`/`std::format` nor `<=>` (verified by grep over the full diff) |

## 2. Source pull requests

The fixes come from the 58 master pull requests listed below, the same
list as section 2 of
[PR #1268](https://github.com/thelfer/tfel/pull/1268) (itself section 2.1
of [PR #1267](https://github.com/thelfer/tfel/pull/1267)). PR #1268
documents how each of them landed on rliv-5.0; the present branch applies
that rliv-5.0 state to 4.2, with the deviations recorded in section 4 and
in `backports.md`.

| PR | Subject |
|---|---|
| [#1096](https://github.com/thelfer/tfel/pull/1096) | [mfront] Final stiffness tensor of the rk54 and rk4 algorithms |
| [#1098](https://github.com/thelfer/tfel/pull/1098) | [tfel-math] In-place division of arrays and views by an integer |
| [#1100](https://github.com/thelfer/tfel/pull/1100) | [mfront] Shear components of stress-free expansions in several behaviour interfaces |
| [#1102](https://github.com/thelfer/tfel/pull/1102) | [tfel-material] Second derivatives, NaN and single precision of several stress criteria |
| [#1104](https://github.com/thelfer/tfel/pull/1104) | [tfel-math] Crash, operator precedence, derivative and integer literals of the formula parsers |
| [#1106](https://github.com/thelfer/tfel/pull/1106) | [docs] Examples rejected by mfront and mtest, wrong keyword names and broken links |
| [#1108](https://github.com/thelfer/tfel/pull/1108) | [mfront] Normalisation factors, physical bounds and MultipleIsotropicMisesFlows in the implicit and isotropic plasticity |
| [#1110](https://github.com/thelfer/tfel/pull/1110) | [mfront] Runge-Kutta DSLs and solvers: rk4 last stage, external models, error estimates and final time |
| [#1112](https://github.com/thelfer/tfel/pull/1112) | [mtest] Crash without behaviour, time step at the end of a period, events, NaN results and cubic pipe elements |
| [#1114](https://github.com/thelfer/tfel/pull/1114) | [mfront] StandardElastoViscoPlasticity brick: nucleation, porosity name, porosity jacobian and porous stress |
| [#1116](https://github.com/thelfer/tfel/pull/1116) | [tfel-math] Eigenvalue sorting, Harari solver, positive/negative parts, convert, tpld and fromRotationMatrix |
| [#1118](https://github.com/thelfer/tfel/pull/1118) | [mfront] Interfaces of material properties and models: exports, one-line functions, bounds, temperature of cases |
| [#1120](https://github.com/thelfer/tfel/pull/1120) | [mfront] ansys and abaqusexplicit interfaces: finite strain strategies, plane stress, packaging and parallelisation |
| [#1122](https://github.com/thelfer/tfel/pull/1122) | [tfel-material] Hill tensors, orientation averages, Plate convention and altered stiffness tensors in axisymmetry |
| [#1124](https://github.com/thelfer/tfel/pull/1124) | [mfront] StandardElastoViscoPlasticity brick: flow identifiers, flow criteria coefficients, theta factors |
| [#1126](https://github.com/thelfer/tfel/pull/1126) | [mfront] Tangent operators of cohesive zone models (castem, aster, MTest) and calculix interface |
| [#1128](https://github.com/thelfer/tfel/pull/1128) | [tfel-system] rmdir following symbolic links, mkdir, binary_read, ThreadPool, ThreadedTaskResult, pipes, ELF |
| [#1130](https://github.com/thelfer/tfel/pull/1130) | [mtest] Wrappers of the generic, aster, castem, ansys, abaqusexplicit and castem_umat behaviours |
| [#1132](https://github.com/thelfer/tfel/pull/1132) | [mfront] Hooke stress potentials: plane stress jacobian, generic_prediction_operator option, broken state |
| [#1134](https://github.com/thelfer/tfel/pull/1134) | [tfel-math] Powell dog leg solvers, scalar Newton-Raphson, TinyMatrixSolve/Invert, CubicRoots, runtime_array |
| [#1136](https://github.com/thelfer/tfel/pull/1136) | [tfel-material] NUMODIS and slip systems: Schmid factors, installed headers, exports, HCP families, symmetries |
| [#1138](https://github.com/thelfer/tfel/pull/1138) | [mfront] Generated code: amitex template, arrays of state variables and gradients, profiling, @Relocation, rotation |
| [#1140](https://github.com/thelfer/tfel/pull/1140) | [python] Bindings: evtypes, ST2toST2 subscripts, homogenisation bounds, setRotationMatrix, setStrainMeasure |
| [#1142](https://github.com/thelfer/tfel/pull/1142) | [tfel-utilities] tfel-check shall_fail and directory configurations, rstream::getLine, CxxTokenizer, Data reads |
| [#1144](https://github.com/thelfer/tfel/pull/1144) | [mfront] DSLs: bricks, behaviour variables, @TangentOperator, initialisation values, targets.lst, overriding |
| [#1146](https://github.com/thelfer/tfel/pull/1146) | [tfel-math] Headers: abs of t2tost2, st2tost2 conversion in 1D, eigen tensors of quantities, tmatrix, array decls |
| [#1148](https://github.com/thelfer/tfel/pull/1148) | [tfel-math] Formula parsers, kriging and LUSolve: copy and arguments of diff, Cast3M functions without arguments |
| [#1150](https://github.com/thelfer/tfel/pull/1150) | [mtest] Initial elastic stiffness of Abaqus/Explicit behaviours: halved shear terms and 3D shear columns |
| [#1152](https://github.com/thelfer/tfel/pull/1152) | [mfront] Generated code: integration variable derivatives, tangent operator blocks, generic gradients, relocation |
| [#1154](https://github.com/thelfer/tfel/pull/1154) | [tfel-math] Quantities and views: qt_ref assignment, change_basis, positive parts, runtime matrix views, arrays |
| [#1156](https://github.com/thelfer/tfel/pull/1156) | [mfront] DSLs and generated code: modelling hypotheses intersection, braceless assignments, output operators |
| [#1158](https://github.com/thelfer/tfel/pull/1158) | [mfront] castem, aster, abaqus, ansys and abaqusexplicit interfaces: LogarithmicStrain1D, sub-stepping, 2D finite strain |
| [#1160](https://github.com/thelfer/tfel/pull/1160) | [tfel-material] Finite strain tangent operators, logarithmic strain handler, Hosford/Barlat with quantities |
| [#1162](https://github.com/thelfer/tfel/pull/1162) | [mtest] MTest and PTest: ptest keywords help, array component names, orthotropic generic stress, mistral defaults |
| [#1166](https://github.com/thelfer/tfel/pull/1166) | [tfel-math] Headers and parser: parenthesised conditions, stensor setComponent/getComponent, isqrt3 with MSVC |
| [#1168](https://github.com/thelfer/tfel/pull/1168) | [tfel-utilities] CxxTokenizer, Data arrays, reserved keywords, systemCall::copy, wfstream, TFEL/System messages |
| [#1170](https://github.com/thelfer/tfel/pull/1170) | [tfel-tests] tfel-doc output, log and colors, TFEL/Tests XML output and exports, copy-paste slips, JUnit reports |
| [#1172](https://github.com/thelfer/tfel/pull/1172) | [mfront-query] File extensions of mfront.write and mtest.write, mfront-query lookups, bounds, specific targets |
| [#1174](https://github.com/thelfer/tfel/pull/1174) | [mfront] abaqus and amitex input examples, cyrano header, ansys/lsdyna/dianafea example files, zmat interface |
| [#1176](https://github.com/thelfer/tfel/pull/1176) | [installation] Doxygen deployment, keyword descriptions, installation pages, maintainer scripts, generic interfaces |
| [#1178](https://github.com/thelfer/tfel/pull/1178) | [mfront] ansys, comsol, dianafea, castem, aster and cyrano interfaces: material properties layout, tangent operators |
| [#1180](https://github.com/thelfer/tfel/pull/1180) | [mfront] Wrong or misleading error messages and documentation of DSLs, tools, material property interfaces |
| [#1182](https://github.com/thelfer/tfel/pull/1182) | [python] mfront.database entry points, mtest initial values and depths, tfel.material imports, mfm-test-generator |
| [#1184](https://github.com/thelfer/tfel/pull/1184) | [cmake] Static library names, test dependencies, C++ standard, static MFront package, environment scripts, Doxygen |
| [#1186](https://github.com/thelfer/tfel/pull/1186) | [mtest] dianafea wrapper shear order, tfel-check JUnit escaping, raw stream readers, Python docstrings |
| [#1188](https://github.com/thelfer/tfel/pull/1188) | [mfront] castem logarithmic strain helpers, stensor absolute value, DSLBase helpers, FSES return codes, TestData |
| [#1190](https://github.com/thelfer/tfel/pull/1190) | [mfront] Cazacu Pipe convention, StrainRateSensitive derivative, @Initialize, command line keywords, StringArray |
| [#1192](https://github.com/thelfer/tfel/pull/1192) | [mfront-query] Names of the DSL options in the help, strain measure output, specific targets rules and help text |
| [#1194](https://github.com/thelfer/tfel/pull/1194) | [mtest] End of input in the MTest and PTest parsers, ansys input file example of orthotropic behaviours |
| [#1196](https://github.com/thelfer/tfel/pull/1196) | [mfront] madnex search paths: file extension, behaviours and models, knowledge type; madnex export of shared dirs |
| [#1210](https://github.com/thelfer/tfel/pull/1210) | [choice] [tfel-math] Variable positions of Evaluator in diff() and tfel-check, fses::sort, vector::copy, GenType |
| [#1218](https://github.com/thelfer/tfel/pull/1218) | [tfel-glossary] Types, units and short descriptions of some glossary entries |
| [#1243](https://github.com/thelfer/tfel/pull/1243) | [choice] [tests] Unit tests: checks that need reference values (tensor2, stensor9, test_model, aster memo-nrad) |
| [#1244](https://github.com/thelfer/tfel/pull/1244) | [choice] [mfront] Reference file for the finitestrainsinglecrystal2 tests |
| [#1246](https://github.com/thelfer/tfel/pull/1246) | [choice] [ptest] Reference profiles of the orthotropic pipe tests |
| [#1248](https://github.com/thelfer/tfel/pull/1248) | [choice] [tests] Checks of the kriging and parser unit tests |
| [#1250](https://github.com/thelfer/tfel/pull/1250) | [choice] [tests] Check of the process unit test |
| [#1263](https://github.com/thelfer/tfel/pull/1263) | [choice] [tfel-math] Second derivative of the determinant of a non-symmetric tensor, const scalar views of quantities |

## 3. What was backported

### 3.1 Area overview

Same distribution as the rliv-5.0 landing (section 5 of PR #1268),
modulo the 4.2 deviations of section 4:

- `include/TFEL` (~105 files): math library fixes (stensor
  `setComponent`/`getComponent`, `stensor.ixx`, eigenvalue helpers, the
  `is_negligible` boundary case in FSES, `isqrt2` in the
  `t2tost2`/`st2tost2` conversions, `Data`/`DataMap` improvements,
  string algorithms, `ThreadedTaskResult` constructor fix, etc.);
- `mfront` (src + include): the bulk of the DSL fixes (`DSLBase`,
  `BehaviourDSLCommon`, `BehaviourDescription`, `DSLUtilities`,
  implicit/isotropic DSL bases, flow rules, bricks, code generators)
  and the interface handlers (Abaqus, Ansys, Aster, CalculiX, DianaFEA,
  AbaqusExplicit, Castem, Comsol, Cyrano, Zmat...), plus the
  material-property interfaces (Cpp, Octave) and `mfront-query`;
- `mtest` (~60 files): parser fixes, scheme and behaviour fixes, new
  ptest references for the orthotropic castem tests,
  `castemelasticity-cubic.ptest`;
- `tests` and `src` (~60 files): corrected unit-test expectations
  (`stensor9` mpmath reference values, exponentials of tensors), parser
  regressions, the new `StiffnessTensorTest`, `StressCriteriaTest`,
  `rk_final_time`;
- `tfel-check`: `Curve`/`L2Comparison` extraction, `TestLauncher`
  rework (`TestLauncherV1` deleted), `test_TestLauncher` unit tests;
- `docs` (~90 files): documentation typo and content fixes existing in
  the 4.2 documentation structure;
- miscellaneous: `cmake/modules`, `check_all.sh`,
  `.github/workflows/cmake.yml`.

### 3.2 New files (21, all registered in the `CMakeLists.txt` of their directory)

- tests: `tests/Material/StiffnessTensorTest.cxx`,
  `tests/Material/StressCriteriaTest.cxx`,
  `tests/Math/RungeKutta/rk_final_time.cxx`;
- behaviours: `mfront/tests/behaviours/RungeKuttaElasticity.mfront`,
  `RungeKuttaElasticity_rk4.mfront`,
  `generic/rungekuttaelasticity.mtest`, `generic/rungekuttaelasticity-rk4.mtest`;
- ptest: `mtest/tests/ptest/castem/castemelasticity-cubic.ptest`,
  `mtest/tests/ptest/references/orthotropic-*.ref` (7 files),
  `mfront/tests/behaviours/references/finitestrainsinglecrystal2-001.ref`;
- tfel-check: `include/TFEL/Check/Curve.hxx`, `L2Comparison.hxx`,
  `src/Curve.cxx`, `src/L2Comparison.cxx`,
  `tests/unitTest/test_TestLauncher.cxx`.

### 3.3 Notable adaptations to the 4.2 structure

The full list is in `backports.md` ("Phase C decisions"); the main ones:

- per-component bounds checks (`writeBoundsChecks` /
  `writePhysicalBoundsChecks` overloads taking the bounds as argument)
  live in 4.2's `DSLUtilities` (5.1's `CodeGeneratorUtilities` split is
  not adopted);
- the interface keyword portability warnings (`check_interface_restriction`)
  were added to the Abaqus, Ansys, Aster and CalculiX interfaces
  honouring each file's parameter naming (`k` vs `key`);
- the `AnsysInterface.cxx` material-property example list uses
  `pm->name` (5.x's `getExternalName()` accessor does not exist in 4.2);
- `mfront-query`: the `--*-dsl-option` keyword renames were rebuilt
  with back-compat aliases on the 4.2 `QueryHandlerBase.cxx`;
- the DianaFEA plane-stress tangent operators took the 5.0 final state
  (`icste`/`one_half` factors); the 4.2 code did not compile (undeclared
  `cste`/`two`); the hard-coded `TRIDIMENSIONAL` specialisations are
  correct for 4.2, which only supports that hypothesis for this
  interface.

## 4. What was not backported

### 4.1 C++20 downgrades (4.2 builds with C++17)

- `include/TFEL/System/ThreadedTaskResult.{hxx,ixx}`: the constrained
  variadic constructor was rewritten with `std::enable_if` +
  `std::disjunction` over `std::decay_t` (the `std::in_place`
  forwarding fix is kept);
- `include/TFEL/Math/qt.hxx`: the new constrained copy-assignment of
  `Quantity` is inexpressible in C++17 (SFINAE does not apply to
  non-template assignment operators) and was dropped; the `square_root`
  fix (`power<1,2>(q)`, which keeps the unit of the result) is kept;
- error-message and generator fixes using `std::string::starts_with`
  were written with `tfel::utilities::starts_with`.

### 4.2 5.x mechanisms not imported

`areRuntimeChecksDisabled`, the `OutputStatus` return-status plumbing,
`getExternalName`, the fsarray IntegrationData members and the `>&>`
array-derivative machinery are 5.x features absent from 4.2; the hunks
wrapping the backported fixes in them were reduced to the fixes
themselves.

### 4.3 Paths with no 4.2 counterpart (38 files)

- the 13 `bindings/python/pybind11/...` files: 4.2 uses boost-python
  bindings under `bindings/python/<module>`; e.g. the pybind11
  `st2tost2` subscript fix targets a lambda the boost binding does not
  have (it forwards to `operator()`, whose bounds checks are fixed in
  the library); the boost bindings were deliberately not touched;
- the 15 `docs/web/...-keywords.md` files: 5.x renamed/regenerated
  keyword pages; the underlying DSL example fixes were mirrored onto
  the 4.2 pages by the docs batch;
- `.github/workflows/doxygen.yml`, `docs/web/libraries_usage.md`,
  `HarariEigenSolver.{hxx,ixx}`, `TMatrixTMatrixExpr.hxx`,
  `MaterialPropertyDocumentationGenerator.cxx`,
  `DataInterpolationUtilities.hxx`, `BehaviourInterfaceBase.cxx`,
  `StrainRateSensitiveIsotropicHardeningRule.cxx`,
  `TFELMFrontStaticConfig.cmake.in`: introduced after 4.2;
- `mfront/tests/behaviours/DigitSeparatorTest.mfront`: new 5.x test.

### 4.4 Test hunks skipped

- `HARARIEIGENSOLVER` specialisations of the stensor eigen tests
  (enumerator absent from 4.2);
- hunks of `tests/Math/tmatrix.cxx`, `view.cxx`, `matrix.cxx`,
  `tmatrix7.cxx` targeting 5.x-only APIs;
- brace-init of new tests adapted to the 4.2 array API
  (`T{{...}}`, no variadic array constructors).

### 4.5 Deletions performed by the change set (kept deleted)

- `docs/mfront/Model/GlobalParameter.md` (content moved into the
  keyword documentation);
- `src/Utilities/ArgumentParserBase.cxx` (definitions moved to
  `src/Utilities/Argument.cxx`);
- `tfel-check/include/TFEL/Check/TestLauncherV1.hxx` and
  `tfel-check/src/TestLauncherV1.cxx` (tfel-check rework).

## 5. Validation record

- `cmake . && make -j32` (all default targets): 0 errors;
- `make -j32 check`: `100% tests passed, 0 tests failed out of 20274`
  (611 s);
- follow-up audit (lines removed by the change set but still present in
  touched files): 0 hits;
- new tests contributed by the campaign run green:
  `generic-rungekuttaelasticity*_mtest` (five time-discretisation
  strategies), `rk_final_time`, `StiffnessTensorTest`,
  `StressCriteriaTest`, `castemelasticity-cubic_ptest`,
  `mfront-RungeKuttaElasticity-generic`, the `tfel-check`
  `test_TestLauncher` unit tests.

## 6. Statistics

| item | value |
|---|---|
| files in the source change set | 612 |
| applied cleanly | 439 |
| already applied (first batch `667b5f7fd`) | 22 |
| merged manually | 90 (24 three-way + audited, 66 by hand) |
| new files added | 21 |
| deletions kept | 4 |
| missing-path files triaged | 38 (all not applicable) |
| diff of this campaign vs `667b5f7fd` | 528 tracked files (+4903/-3960) + 21 new files |
| test suite | 20274 tests, 100% pass |
