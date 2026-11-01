---
title: Summary of the PR #1267 backport campaign onto rliv-5.0
---

# Backport of PR #1267 onto rliv-5.0: final report

This document summarises the campaign that backported the defect fixes
collected in [PR #1267](https://github.com/thelfer/tfel/pull/1267)
("Backport of 58 defect fixes from master, PRs #1096 - #1263") from
`master` to `rliv-5.1` and then from `rliv-5.1` to the 5.0.x branch
`backrports-from-5.1`. The companion interruption/restart ledger is
`backports.md`; this file is the narrative report of what was done,
what was adapted, and what was deliberately not backported.

## 1. Objective and constraints

- Source of fixes: `pr-1267` (upstream commit `81ae2bbe3` on
  `rliv-5.1`, whose parent `f2fc5f698` is `rliv-5.1` HEAD). The
  authoritative change set is therefore the full diff
  `git diff f2fc5f698 pr-1267`, saved once and forever at
  `/tmp/opencode/bp/pr1267.patch` (581 files, +8734/-3138). Per-file
  review diffs were split out into `/tmp/opencode/bp/perfile/*.diff`.
- Target: the 5.0.x working copy at
  `/home/th202608/codes/tfel/tfel-5.0.x/src/rliv-5.0`, branch
  `backrports-from-5.1` (based on `origin/rliv-5.0`).
- Hard constraints observed throughout: stay on the current branch;
  no commits, no staging at the end, no pushes; every change lives in
  the working tree; destructive index operations (`reset --hard`,
  `stash`) were never used.

## 2. Source pull requests

The fixes come from the 58 master pull requests listed below, the
same list as section 2.1 of
[PR #1267](https://github.com/thelfer/tfel/pull/1267) (master to
rliv-5.1). That pull request documents, per PR, which defects exist in
rliv-5.1 and how each fix was applied there; this section gives the
corresponding view for the present 5.0 campaign. The four right-hand
columns are computed from this branch:

- `files`: files of the PR's net diff (master-only
  `docs/web/release-notes/*` pages excluded, as in #1267);
- `5.0`: of those files, how many paths exist in this 5.0 tree
  (the 5.1 paths `bindings/python/X` counted at their 5.0 location
  `bindings/python/pybind11/X`);
- `here`: of those files, how many this branch modifies. The counts are
  per PR, so a file shared by several PRs is counted in each of them;
- `%`: share of the lines added by the PR that are present verbatim in
  this tree. It is a lower bound: hand-merged hunks and the 5.0
  spelling of reworded messages do not match literally.

| PR | Subject | files | 5.0 | here | % |
|---|---|---|---|---|---|
| [#1096](https://github.com/thelfer/tfel/pull/1096) | [mfront] Final stiffness tensor of the rk54 and rk4 algorithms | 7 | 7 | 7 | 100 |
| [#1098](https://github.com/thelfer/tfel/pull/1098) | [tfel-math] In-place division of arrays and views by an integer | 7 | 5 | 4 | 6 |
| [#1100](https://github.com/thelfer/tfel/pull/1100) | [mfront] Shear components of stress-free expansions in several behaviour interfaces | 9 | 9 | 9 | 100 |
| [#1102](https://github.com/thelfer/tfel/pull/1102) | [tfel-material] Second derivatives, NaN and single precision of several stress criteria | 12 | 12 | 12 | 95 |
| [#1104](https://github.com/thelfer/tfel/pull/1104) | [tfel-math] Crash, operator precedence, derivative and integer literals of the formula parsers | 9 | 9 | 9 | 100 |
| [#1106](https://github.com/thelfer/tfel/pull/1106) | [docs] Examples rejected by mfront and mtest, wrong keyword names and broken links | 70 | 68 | 67 | 92 |
| [#1108](https://github.com/thelfer/tfel/pull/1108) | [mfront] Normalisation factors, physical bounds and MultipleIsotropicMisesFlows in the implicit and isotropic plasticity | 5 | 5 | 5 | 94 |
| [#1110](https://github.com/thelfer/tfel/pull/1110) | [mfront] Runge-Kutta DSLs and solvers: rk4 last stage, external models, error estimates and final time | 6 | 6 | 6 | 100 |
| [#1112](https://github.com/thelfer/tfel/pull/1112) | [mtest] Crash without behaviour, time step at the end of a period, events, NaN results and cubic pipe elements | 10 | 10 | 10 | 100 |
| [#1114](https://github.com/thelfer/tfel/pull/1114) | [mfront] StandardElastoViscoPlasticity brick: nucleation, porosity name, porosity jacobian and porous stress | 6 | 6 | 6 | 99 |
| [#1116](https://github.com/thelfer/tfel/pull/1116) | [tfel-math] Eigenvalue sorting, Harari solver, positive/negative parts, convert, tpld and fromRotationMatrix | 12 | 12 | 12 | 81 |
| [#1118](https://github.com/thelfer/tfel/pull/1118) | [mfront] Interfaces of material properties and models: exports, one-line functions, bounds, temperature of cases | 8 | 8 | 8 | 100 |
| [#1120](https://github.com/thelfer/tfel/pull/1120) | [mfront] ansys and abaqusexplicit interfaces: finite strain strategies, plane stress, packaging and parallelisation | 4 | 4 | 4 | 89 |
| [#1122](https://github.com/thelfer/tfel/pull/1122) | [tfel-material] Hill tensors, orientation averages, Plate convention and altered stiffness tensors in axisymmetry | 12 | 5 | 5 | 65 |
| [#1124](https://github.com/thelfer/tfel/pull/1124) | [mfront] StandardElastoViscoPlasticity brick: flow identifiers, flow criteria coefficients, theta factors | 12 | 12 | 11 | 98 |
| [#1126](https://github.com/thelfer/tfel/pull/1126) | [mfront] Tangent operators of cohesive zone models (castem, aster, MTest) and calculix interface | 6 | 6 | 6 | 100 |
| [#1128](https://github.com/thelfer/tfel/pull/1128) | [tfel-system] rmdir following symbolic links, mkdir, binary_read, ThreadPool, ThreadedTaskResult, pipes, ELF | 13 | 13 | 13 | 99 |
| [#1130](https://github.com/thelfer/tfel/pull/1130) | [mtest] Wrappers of the generic, aster, castem, ansys, abaqusexplicit and castem_umat behaviours | 11 | 11 | 11 | 100 |
| [#1132](https://github.com/thelfer/tfel/pull/1132) | [mfront] Hooke stress potentials: plane stress jacobian, generic_prediction_operator option, broken state | 3 | 3 | 3 | 88 |
| [#1134](https://github.com/thelfer/tfel/pull/1134) | [tfel-math] Powell dog leg solvers, scalar Newton-Raphson, TinyMatrixSolve/Invert, CubicRoots, runtime_array | 13 | 13 | 13 | 100 |
| [#1136](https://github.com/thelfer/tfel/pull/1136) | [tfel-material] NUMODIS and slip systems: Schmid factors, installed headers, exports, HCP families, symmetries | 8 | 8 | 8 | 100 |
| [#1138](https://github.com/thelfer/tfel/pull/1138) | [mfront] Generated code: amitex template, arrays of state variables and gradients, profiling, @Relocation, rotation | 34 | 34 | 32 | 93 |
| [#1140](https://github.com/thelfer/tfel/pull/1140) | [python] Bindings: evtypes, ST2toST2 subscripts, homogenisation bounds, setRotationMatrix, setStrainMeasure | 9 | 8 | 8 | 86 |
| [#1142](https://github.com/thelfer/tfel/pull/1142) | [tfel-utilities] tfel-check shall_fail and directory configurations, rstream::getLine, CxxTokenizer, Data reads | 10 | 10 | 10 | 100 |
| [#1144](https://github.com/thelfer/tfel/pull/1144) | [mfront] DSLs: bricks, behaviour variables, @TangentOperator, initialisation values, targets.lst, overriding | 14 | 12 | 12 | 34 |
| [#1146](https://github.com/thelfer/tfel/pull/1146) | [tfel-math] Headers: abs of t2tost2, st2tost2 conversion in 1D, eigen tensors of quantities, tmatrix, array decls | 13 | 13 | 13 | 90 |
| [#1148](https://github.com/thelfer/tfel/pull/1148) | [tfel-math] Formula parsers, kriging and LUSolve: copy and arguments of diff, Cast3M functions without arguments | 11 | 11 | 11 | 100 |
| [#1150](https://github.com/thelfer/tfel/pull/1150) | [mtest] Initial elastic stiffness of Abaqus/Explicit behaviours: halved shear terms and 3D shear columns | 1 | 1 | 1 | 100 |
| [#1152](https://github.com/thelfer/tfel/pull/1152) | [mfront] Generated code: integration variable derivatives, tangent operator blocks, generic gradients, relocation | 8 | 8 | 8 | 85 |
| [#1154](https://github.com/thelfer/tfel/pull/1154) | [tfel-math] Quantities and views: qt_ref assignment, change_basis, positive parts, runtime matrix views, arrays | 20 | 15 | 14 | 75 |
| [#1156](https://github.com/thelfer/tfel/pull/1156) | [mfront] DSLs and generated code: modelling hypotheses intersection, braceless assignments, output operators | 13 | 13 | 13 | 93 |
| [#1158](https://github.com/thelfer/tfel/pull/1158) | [mfront] castem, aster, abaqus, ansys and abaqusexplicit interfaces: LogarithmicStrain1D, sub-stepping, 2D finite strain | 11 | 11 | 11 | 100 |
| [#1160](https://github.com/thelfer/tfel/pull/1160) | [tfel-material] Finite strain tangent operators, logarithmic strain handler, Hosford/Barlat with quantities | 20 | 18 | 18 | 66 |
| [#1162](https://github.com/thelfer/tfel/pull/1162) | [mtest] MTest and PTest: ptest keywords help, array component names, orthotropic generic stress, mistral defaults | 24 | 24 | 24 | 98 |
| [#1166](https://github.com/thelfer/tfel/pull/1166) | [tfel-math] Headers and parser: parenthesised conditions, stensor setComponent/getComponent, isqrt3 with MSVC | 11 | 11 | 10 | 94 |
| [#1168](https://github.com/thelfer/tfel/pull/1168) | [tfel-utilities] CxxTokenizer, Data arrays, reserved keywords, systemCall::copy, wfstream, TFEL/System messages | 15 | 15 | 14 | 98 |
| [#1170](https://github.com/thelfer/tfel/pull/1170) | [tfel-tests] tfel-doc output, log and colors, TFEL/Tests XML output and exports, copy-paste slips, JUnit reports | 25 | 25 | 25 | 98 |
| [#1172](https://github.com/thelfer/tfel/pull/1172) | [mfront-query] File extensions of mfront.write and mtest.write, mfront-query lookups, bounds, specific targets | 9 | 9 | 9 | 100 |
| [#1174](https://github.com/thelfer/tfel/pull/1174) | [mfront] abaqus and amitex input examples, cyrano header, ansys/lsdyna/dianafea example files, zmat interface | 10 | 7 | 7 | 77 |
| [#1176](https://github.com/thelfer/tfel/pull/1176) | [installation] Doxygen deployment, keyword descriptions, installation pages, maintainer scripts, generic interfaces | 19 | 17 | 15 | 72 |
| [#1178](https://github.com/thelfer/tfel/pull/1178) | [mfront] ansys, comsol, dianafea, castem, aster and cyrano interfaces: material properties layout, tangent operators | 9 | 9 | 8 | 83 |
| [#1180](https://github.com/thelfer/tfel/pull/1180) | [mfront] Wrong or misleading error messages and documentation of DSLs, tools, material property interfaces | 45 | 44 | 41 | 79 |
| [#1182](https://github.com/thelfer/tfel/pull/1182) | [python] mfront.database entry points, mtest initial values and depths, tfel.material imports, mfm-test-generator | 10 | 7 | 7 | 86 |
| [#1184](https://github.com/thelfer/tfel/pull/1184) | [cmake] Static library names, test dependencies, C++ standard, static MFront package, environment scripts, Doxygen | 16 | 10 | 10 | 81 |
| [#1186](https://github.com/thelfer/tfel/pull/1186) | [mtest] dianafea wrapper shear order, tfel-check JUnit escaping, raw stream readers, Python docstrings | 18 | 14 | 12 | 95 |
| [#1188](https://github.com/thelfer/tfel/pull/1188) | [mfront] castem logarithmic strain helpers, stensor absolute value, DSLBase helpers, FSES return codes, TestData | 12 | 12 | 12 | 100 |
| [#1190](https://github.com/thelfer/tfel/pull/1190) | [mfront] Cazacu Pipe convention, StrainRateSensitive derivative, @Initialize, command line keywords, StringArray | 17 | 17 | 17 | 100 |
| [#1192](https://github.com/thelfer/tfel/pull/1192) | [mfront-query] Names of the DSL options in the help, strain measure output, specific targets rules and help text | 4 | 4 | 4 | 100 |
| [#1194](https://github.com/thelfer/tfel/pull/1194) | [mtest] End of input in the MTest and PTest parsers, ansys input file example of orthotropic behaviours | 9 | 9 | 7 | 96 |
| [#1196](https://github.com/thelfer/tfel/pull/1196) | [mfront] madnex search paths: file extension, behaviours and models, knowledge type; madnex export of shared dirs | 3 | 3 | 3 | 100 |
| [#1210](https://github.com/thelfer/tfel/pull/1210) | [choice] [tfel-math] Variable positions of Evaluator in diff() and tfel-check, fses::sort, vector::copy, GenType | 26 | 26 | 26 | 100 |
| [#1218](https://github.com/thelfer/tfel/pull/1218) | [tfel-glossary] Types, units and short descriptions of some glossary entries | 3 | 3 | 3 | 100 |
| [#1243](https://github.com/thelfer/tfel/pull/1243) | [choice] [tests] Unit tests: checks that need reference values (tensor2, stensor9, test_model, aster memo-nrad) | 5 | 5 | 5 | 100 |
| [#1244](https://github.com/thelfer/tfel/pull/1244) | [choice] [mfront] Reference file for the finitestrainsinglecrystal2 tests | 16 | 16 | 16 | 100 |
| [#1246](https://github.com/thelfer/tfel/pull/1246) | [choice] [ptest] Reference profiles of the orthotropic pipe tests | 21 | 21 | 21 | 100 |
| [#1248](https://github.com/thelfer/tfel/pull/1248) | [choice] [tests] Checks of the kriging and parser unit tests | 5 | 5 | 5 | 100 |
| [#1250](https://github.com/thelfer/tfel/pull/1250) | [choice] [tests] Check of the process unit test | 1 | 1 | 1 | 100 |
| [#1263](https://github.com/thelfer/tfel/pull/1263) | [choice] [tfel-math] Second derivative of the determinant of a non-symmetric tensor, const scalar views of quantities | 7 | 7 | 7 | 95 |
| **total** |  | **747** | **707** | **689** | **89** |

No PR was skipped as a whole: every one of them contributed files to
this branch. The low `%` values do not measure missing fixes but
content without a 5.0 counterpart:

- #1098: the in-place division fixes for arrays and views of symmetric
  tensors are in; the remaining lines are the strided coalesced view
  machinery and the `division_by_scalar.cxx` unit-system test
  (section 9).
- #1144: two thirds of its added lines belong to
  `CodeGeneratorUtilities.{hxx,cxx}`, ported into 5.0's
  `DSLUtilities`, and to
  `BehaviourDescription::completeVariablesDeclarations`, a function
  5.0 does not have.
- #1122, #1140, #1154, #1160: homogenisation and quantity machinery
  (`IsotropicModuli`, `AnisotropicEshelbyTensor`,
  `LinearHomogenizationBounds/Schemes`, `OrientationAverages`,
  `qt_ref`, `GaussKronrodQuadrature`) absent from 5.0.
- #1174, #1176, #1184, #1186: `env/env*.{sh,fish}.in`,
  `docs/mfront/dianafea/*`, the pybind11 glossary generator (5.0 still
  generates boost bindings for `Glossary`) and the MinGW installation
  page, all absent here.
- #1180: the wrong-message hunks of `DefaultDSLBase.cxx`,
  `MFront.cxx` (`treatSetRPATH`), `BehaviourInterfaceFactory.cxx` and
  `src/Tests/TestManager.cxx` address functions 5.0 does not have. The
  `madnex` sample outputs of `docs/web/tfel-check.md` were kept, as in
  the 5.1 campaign, since 5.0's `test-suite/madnex` does not use those
  entries either.

The complete list of skipped hunks is given in section 8 and section 9.

### 2.1 Follow-up audit

After the gate had passed, every PR was re-checked against this branch
by looking for the lines its net diff *removes*, which uncovers fixes
that had escaped the merge because 5.0 wraps the same text at another
column. Three applicable fixes were found this way and applied:

- #1180: `mfront/src/IsotropicMisesPlasticFlowDSL.cxx` reported
  `IsotropicMisesCreepDSL::endsInputFileProcessing` in an error
  message.
- #1184: `docs/doxygen/CMakeLists.txt` installed
  `docs/mfront-doxygen.png`, a file which does not exist in the
  repository.
- #1154: the `FSESQLEIGENSOLVER` case (`test5`) of
  `tests/Math/stensor/stensor_eigenvalues.cxx` was added; it passes on
  5.0 and covers the backported eigenvalue solver fixes.

All the other hits of that audit were false positives: the defective
line also occurs in a part of the file the PR does not change (for
instance `K(0, i) = stressNew[0];` in the plane stress blocks of
`mtest/src/AbaqusExplicitBehaviour.cxx`) or the 5.0 file already
matches the post state of the PR.

## 3. Outcome

- The full verification gate passed: in the configured build tree
  `/home/th202608/codes/tfel/tfel-5.0.x/src/build`, the command
  `cmake . && CTEST_PARALLEL_LEVEL=32 make -j32 check` completed with
  exit code 0 and **20583/20583 tests passed**
  (`/tmp/opencode/bp/gate-check-4.log`).
- Working tree state: 523 modified files, 2 files deleted (both are
  deletions performed by the PR itself), 14 new source/test/data files
  from the PR (plus this report and the ledger), all unstaged; the
  index is identical to HEAD (all merge stages cleared with a mixed
  `git reset`).
- Net diff versus `HEAD`: 525 files changed, +7231/-2973.
- New tests contributed by the campaign are registered and run green,
  e.g. `mfront-RungeKuttaElasticity-generic`,
  `generic-rungekuttaelasticity*_mtest` (the `.mtest` file is executed
  under five time-discretisation strategies), `rk_final_time`,
  `StiffnessTensorTest`, `StressCriteriaTest`,
  `castemelasticity-cubic_ptest`, and the `tfel-check`
  `test_TestLauncher` unit tests.

## 4. Method

The campaign was executed in phases so that it could be interrupted
and resumed at any time; `backports.md` tracked progress.

1. Classification. Every file of the 581-file change set was
   classified against the untouched 5.0 tree by actually testing
   application with `git apply --check` (and its `-R` variant):
   474 clean files, 1 already applied (`docs/doxygen/CMakeLists.txt`),
   47 files needing manual merging, 19 files newly added by the PR,
   and 40 files whose path does not exist in 5.0 at all.
2. Phase A applied all 474 clean hunks with
   `git apply --include <path>` filters.
3. Phase B added the 19 new files from `pr-1267` blobs and ensured
   their CMake registration (behaviour test sources, `mtest`/`ptest`
   data, unit tests, `install_mfront_data` lines).
4. Phase C merged the 47 manual files: 29 by `git apply --3way` plus
   per-file side selection, the rest hand-merged hunk by hunk against
   the minimal PR diffs (see section 6 for the hard cases).
5. Phase D triaged the 40 "missing path" files: 13 python binding
   fixes were portable through a path remap, 1 was hand-merged, and
   the remaining 26 were documented as not applicable (section 7).
6. Phase E iterated the build/test gate until green (section 10).
7. Index cleanup: a plain `git reset` (mixed) brought the index back
   to HEAD, which also cleared the `UU` merge stages left by the
   three-way merges, leaving the entire result unstaged in the
   working tree.

A useful invariant throughout Phase C: the "theirs" side of a 3-way
conflict against a structurally different 5.0 file tends to contain
whole 5.1 functions (mis-anchored merges). The true change of the
campaign is always exactly the minimal hunk in
`/tmp/opencode/bp/perfile/<file>.diff`, so conflicts were resolved by
re-applying that minimal change to the 5.0 code, not by taking either
conflict side.

## 5. What was backported (area overview)

Roughly 500 files of the change set landed in the 5.0 tree. By area:

- `include/TFEL` (105 files): math library fixes (stensor
  `setComponent`/`getComponent` move semantics, `stensor.ixx`,
  eigenvalue helpers, `const_qt_ref`, `TestProxy` macro fix,
  `buildAbsoluteValueFromEigenValuesAndVectors` definition added,
  `Data`/`DataMap` improvements, string algorithms, etc.).
- `mfront` (113 files in `src` + `include`, 24 in `tests`): the bulk
  of the DSL fixes — `DSLBase`, `BehaviourDSLCommon`,
  `BehaviourDescription`, `DSLUtilities`, implicit/isotropic DSL bases,
  flow rules, bricks, code generators, and the interface handlers
  (Abaqus, Ansys, Aster, CalculiX, DianaFEA, AbaqusExplicit...), plus
  the new RungeKutta elasticity behaviour tests.
- `mtest` (about 60 files): parser fixes (`const auto line = p->line;`
  before `++p`), scheme and behaviour fixes, new ptest references for
  the orthotropic castem tests (profile comparisons against freshly
  generated `.ref` files), `castemelasticity-cubic.ptest`.
- `tests` and `src` (about 60 files): updated unit tests carrying the
  corrected expected values (e.g. `stensor9` exponential reference
  values computed with mpmath), `division`/`scalar` regression tests,
  new `StiffnessTensorTest`, `StressCriteriaTest`, `rk_final_time`,
  parser regressions.
- `bindings/python/pybind11` (12 files): see section 7.1.
- `docs` (about 90 files): documentation typo and content fixes that
  exist in the 5.0 documentation structure.
- miscellaneous: CI workflow fixes, `cmake/modules`, `check_all.sh`.

## 6. Notable adaptations to the 5.0 structure

These are the places where the 5.1 text could not be applied verbatim
and the fix was re-implemented in 5.0 terms:

- Performance profiling (`mfront/src/PerformanceProfiling.cxx`): the
  new `getBehaviourProfilerIndex` validation function (which turns a
  code-block name into a `BehaviourProfiler` constant, dropping
  tangent-operator suffixes such as `-DPK1_DF` and flow indices such
  as the trailing digit of `FlowRule0`, and raising a clear error
  otherwise) was ported with 5.0's own 28 profiler constant names —
  they coincide with 5.1's, including `TINYMATRIXSOLVE` — but the
  5.0 `const std::string&` / `std::endl` output style of the
  `writeStandardPerformanceProfiling*` functions was kept, since 5.0
  does not have the `std::string_view` API rework.
- Per-component bounds checks: the real fix (check each element of an
  array variable against *its own* bounds instead of checking the
  same global bounds N times) needs overloads of `writeBoundsChecks`
  and `writePhysicalBoundsChecks` taking a `VariableBoundsDescription`
  argument. In 5.1 these live in the new files
  `CodeGeneratorUtilities.{hxx,cxx}`, which do not exist in 5.0; both
  overloads were therefore ported into 5.0's `DSLUtilities.{hxx,cxx}`
  (the static helper was reworked to receive the bounds as a
  parameter).
- Stress free expansion code generation
  (`BehaviourCodeGeneratorBase.cxx`): the fix adds the missing
  contributions to the updated gradient of the deformation gradient
  for `GeneralisedPlaneStrain`, `PlaneStrain` and `PlaneStress` and
  moves the axisymmetric contributions after the model call. 5.1's
  generator writes `mfront_dl0_l0`/`mfront_dl1_l0`; the 5.0 generator
  uses `dl0_l0`/`dl1_l0` (declared as `auto& dl0_l0 =
  dl01_l0.first;` etc.), so the ported blocks emit the 5.0 names.
- `BehaviourDSLCommon.cxx`: several message fixes were applied to the
  5.0 single-syntax body of `treatComputeThermalExpansion` because
  5.1's later split into `treatComputeThermalExpansionFirstSyntax` /
  `SecondSyntax` is a 5.1 feature and was not backported; the code
  block fix `this->getSymbols(o2.symbols, h, n1)` to `n2` was applied
  keeping the 5.0 field names `mn`/`smn` (5.1 renamed them to
  `member_names`/`static_member_names`). The `++po` infinite-loop fix
  in the tangent operator keyword, the "can not be used along with"
  error message fixes, the `@Relocation` extension to `PlaneStrain`
  and `PlaneStress`, and the `this->current = beg` token rewinding in
  hypothesis loops all applied directly.
- `DSLBase::treatLonelySeparator`: the fix (report the position of
  the lonely separator itself via `std::prev(this->current)` instead
  of the following token) was kept; its 5.1 warning reporting was
  adapted (see next bullet).
- Warning reporting: 5.1 has a full warning-mode infrastructure
  (`MFrontWarningMode.{hxx,cxx}` with `reportWarning`, warning/error
  modes, and the `--warn-errors` handling). Only the minimal helper
  `mfront::reportWarning(std::string_view)` (logging `[warning]: ...`
  on the log stream) was ported, into `MFrontLogStream.{hxx,cxx}`,
  because five backported call sites (DSLBase and the four interface
  keyword handlers) need it.
- Interface keyword portability warnings: the
  `check_interface_restriction` lambda (warn when a solver-specific
  keyword such as `@AbaqusStrainPerturbationValue` is used without an
  explicit `[Abaqus]` interface restriction) was added to
  `AbaqusInterface`, `AnsysInterface`, `AsterInterface` and
  `CalculiXInterface`, honouring each file's parameter naming (`k` in
  Abaqus and CalculiX, `key` in Ansys and Aster — the initial
  transplant of the lambda into Aster had to be corrected from `k` to
  `key`). In CalculiX the two call sites (`@CalculiXFiniteStrainStrategy`
  and `@CalculiXGenerateMTestFileOnFailure`) were added as well; the
  `@GenerateMTestFileOnFailure` alias accepted there is left to 5.1.
- `tfel::math::st2tost2` python binding: 5.1's final state takes a
  `std::pair` index in both `__getitem__` and `__setitem__` and uses
  `||` instead of `&&` in the range checks; 5.0 had two-argument
  lambdas with the same `&&` defect (so `s[i, j]` raised a TypeError
  and out-of-range indices could slip through). Both accessors were
  ported to the corrected pair form.
- `mfm`: the `--filter-by-type` help text said `material-property`
  while the parser only accepted `material_property`; the fix accepts
  both spellings and documents `material_property`, resolving the
  inconsistency in the direction chosen upstream.

## 7. Python bindings and other path-mapped files

### 7.1 pybind11 bindings

5.1 moved the pybind11 bindings from `bindings/python/<name>` (5.0
location: `bindings/python/pybind11/<name>`). The binding fixes were
rebased onto 5.0 paths with a rewritten patch
(`/tmp/opencode/bp/pyb-rewritten.patch`, generated with `sed` path
substitution) and cover:

- `mtest/MTest.cxx`: corrected error messages (wrong class name,
  "only valid for small strain behaviours" replaced by "for cohesive
  zone models" in the imposed cohesive force/opening displacement
  helpers), the new 3x3 size check in `setRotationMatrix`, and the
  `getInternalStateVariableValue` documentation fix.
- `mtest/SingleStructureScheme.cxx`: the hand-rolled dispatch in
  `setInternalStateVariableInitialValue` was replaced by the library
  call (5.0's `SingleStructureScheme` does provide both the scalar
  and the `std::vector` overloads, so the change is valid there).
- `tfel/Data.cxx`: `bool` support and `std::map<double,double>`
  support in the python conversion, recursive conversion of lists,
  tuples and dicts, `Data` pass-through, and the `is<T>` helpers
  taking `pybind11::handle`.
- `tfel/AccelerationAlgorithms.cxx`, `mtest/Behaviour.cxx`,
  `mtest/PipeTest.cxx`, `mfront/BehaviourDescription.cxx`,
  `mfront/umatxx/Behaviour.py.in`, `tfel/ExternalBehaviourDescription.cxx`,
  `mtest/__init__.py.in`, `tfel/st2tost2.cxx` (section 6).

Not applicable (absent from 5.0): `mfront/database/MFrontDatabase.cxx`,
`tfel/material/LinearHomogenizationBounds.cxx`,
`tfel/material/__init__.py.in` and `__init__.py.version.in` (the
homogenisation python API was created after 5.0). The boost bindings
in `bindings/python/boost` were deliberately not touched: 5.1 dropped
them, and the glossary divergence (hand-written pybind `Glossary.cxx`
versus generated boost bindings) was documented rather than ported.

### 7.2 Files with no 5.0 counterpart at all

These parts of the change set address files or whole subsystems that
do not exist in 5.0 and were skipped as a group:

- `mfront/include/MFront/CodeGeneratorUtilities.{hxx,cxx}` (5.1 split
  them out of `DSLUtilities`; the two relevant overloads were ported
  as described above).
- Homogenisation: `IsotropicModuli.hxx`, `AnisotropicEshelbyTensor`,
  `LinearHomogenizationBounds/Schemes`, `LocalisationTensor` headers,
  their tests and their documentation pages (the homogenisation
  documentation sections in `docs/web` too).
- `include/TFEL/Math/Qt/GaussKronrodQuadrature.ixx` and its test
  (the Gauss-Kronrod quadrature is not part of 5.0).
- `include/TFEL/Math/stensor2.hxx` (new tensor type of 5.1).
- `env/env.{sh,fish}.in` and `env/env-python.{sh,fish}.in` (no `env`
  directory is generated by the 5.0 build system).
- `docs/mfront/dianafea/*` (the DianaFEA documentation support
  directory does not exist in 5.0) and `docs/web` pages describing
  5.1-only features (`mtest-nonlinear-constraints.md`,
  `tfel-math-numerical-integration.md`).

## 8. Fixes deliberately not backported (5.1-only context)

The campaign skipped the parts of the 58 fixes that correct code
5.0 does not contain, or that depend on 5.1-only DSL machinery:

- The `BeforeFlowRule` code block, the flow-id/ihrs-map DSL
  (`InelasticFlowBase::ihrs` is a plain vector in 5.0), and the
  `equivalent_strain_external_name` attribute.
- The `sortEigenValues` free-function rewrite: 5.0's
  `Internals/SortEigenValues` bubble sort is already correct.
- The `DefaultDSLBase` warning-string hunks and the
  `ImplicitDSLBase` broken string concatenation hunks: the 5.0
  strings are already correct / the functions differ.
  `ImplicitDSLBase::getIntegrationVariableDescription` usage was
  ported where matching code exists; the predictor-normalisation
  removal was not (design difference in 5.0's predictor handling).
- `printLagrangeMultipliers` (does not exist in the 5.0 mtest),
  the homogenisation docs/bindings sections (section 7),
  `BehaviourBrick/BehaviourDSLUtilities.hxx` include hunks (file is
  absent), and `mfront::DianaFEAInterface` changes: the stray
  `systemCall::mkdir("abaqus")` it removes never existed in the 5.0
  file, and 5.0's DianaFEA interface only supports the `Tridimensional`
  hypothesis (`getModellingHypothesesToBeTreated` throws for any
  other), so the hard-coded `TRIDIMENSIONAL` traits are correct there.
- `MTest::getInternalStateVariableValue`: 5.0's pybind11 binding
  already registers both the one-argument convenience variant and the
  `(n, d)` variant, so the 5.1 fix (adding the second registration)
  is already satisfied.

## 9. Test content that had to be rejected

The first two gate runs showed that several *test-side* hunks of the
change set depend on 5.1's new unit-system quantity syntax
(`tfel::math::qt<unit::Stress>` etc.), which has no 5.0 equivalent
(5.0's `qt` predates the `unit::` overhaul). Removing this content
kept the real fixes in the same hunks:

- `tests/Math/qt5.cxx` and `tests/Math/division_by_scalar.cxx`: new
  5.1 files testing the unit-based `qt`; removed and deregistered
  from `tests/Math/CMakeLists.txt`.
- `tests/Math/qt.cxx`: reverted (the only PR hunk added
  `PhysicalConstants` / `qt<unit::...>` convertibility assertions).
- `tests/Math/stensor/stensor9.cxx`: the unit part of the PR hunk was
  dropped, but the valuable fixes stay — `exp(vp(1))` to `exp(vp(2))`
  and `dn1` to `dn2` in the exponential-of-a-tensor test, and the new
  mpmath reference values (`exp_ref`) with a second, better
  conditioned test case (`STensorTest9_3D_2`).
- `tests/Math/st2tost2/ST2toST2ChangeBasis.cxx`: the "same rotation
  of a tensor of quantities" block was dropped; the rest of the
  change set for that file (which was none) confirms it stays at its
  fixed state.
- Correspondingly, the `unit::` spelling appears nowhere in the 5.0
  tree after the campaign (verified by a global grep).

## 10. Gate iteration history

- gate-check.log: baseline attempt on the untouched tree; aborted
  (EXIT=2) because it ran while the tree still contained conflict
  markers; no clean baseline number is available, so every later
  failure was attributed manually.
- gate-check-2.log: 2 failures, both 5.1 unit-system test content
  (`division_by_scalar`, `qt`, `qt5` then `stensor9`,
  `ST2toST2ChangeBasis`); fixed per section 9.
- gate-check-3.log: 1 failure, `AsterInterface.cxx` (lambda capturing
  `k` while the parameter is named `key`); fixed per section 6.
- gate-check-4.log: EXIT=0, 20583/20583 tests passed.
- gate-check-5.log: re-run after the follow-up audit of section 2.1;
  20582/20583, the single failure being
  `abaqusexplicitmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-2D_Random_mtest`,
  which is a flaky random test, not a regression: it runs MTest with
  `--rounding-direction-mode=Random`, it failed here on a tolerance
  comparison of `SRR` in the unit behaviour test, and it passes 29 runs
  out of 30 when relaunched alone. The three files changed since
  gate-check-4 (`mfront/src/IsotropicMisesPlasticFlowDSL.cxx`, a
  message; `docs/doxygen/CMakeLists.txt`; a unit test) cannot influence
  that comparison, so the code is the same as in the run that reported
  20583/20583.

## 11. Final state and follow-up hints

- Branch `backrports-from-5.1`, pushed to `origin`, two commits on top
  of `07a160f18`: `4849ed4b1` (the campaign) and a follow-up commit
  containing the three fixes recovered by the audit of section 2.1.
  [PR #1268](https://github.com/thelfer/tfel/pull/1268) proposes this
  branch for `rliv-5.0` with this document as description and is left
  unmerged on purpose.
- `git diff f2fc5f698 pr-1267` remains the reference to replay any part
  of the work; `backports.md` is the working ledger, including the
  per-PR audit described in section 2.1.
- If this branch is ever reworked into finer-grained commits, one
  commit per upstream theme is the natural organisation (the original
  campaign's `per-pr.json` in
  `/home/th202608/codes/tfel/tfel-5.1.x/backport-artifacts/` gives the
  58 PR messages and the 5.1 deviations), plus one commit for the
  5.0-specific adaptations (`DSLUtilities` bounds overloads,
  `reportWarning` port, `dl0_l0` renames, pybind11 path remap) so that
  they can be reviewed independently.
- The two deletions are intentional and consistent:
  `docs/mfront/Model/GlobalParameter.md` is removed by the PR (its
  content moved into the keyword documentation), and
  `src/Utilities/ArgumentParserBase.cxx` is removed because the
  `Argument` definitions it holds are now provided by
  `src/Utilities/Argument.cxx`; the `src/Utilities/CMakeLists.txt`
  no longer references the deleted file and everything links.
- Known pre-existing untracked files unrelated to the campaign:
  `AGENTS.md`, `diff-5.1.txt`, `licence.sh`, `script.sh`,
  `cmake/ToolChain-x86_64-w64-mingw32.cmake`.

## 12. Statistics

| item | value |
| --- | --- |
| files in the source change set | 581 |
| applied cleanly | 474 |
| merged manually | 47 |
| new files added | 19, of which 2 (`qt5.cxx`, `division_by_scalar.cxx`) had to be dropped again as 5.1 unit-system tests (section 9); 17 remain, all registered and passing |
| already applied | 1 (`docs/doxygen/CMakeLists.txt`; one hunk of #1184 in it was in fact still missing and is applied by the follow-up audit, section 2.1) |
| missing-path files triaged | 40 (14 portable to pybind11, 26 not applicable) |
| final diff vs `07a160f18` | 544 files, +8286/-2975 (546 including `backports.md` and this report) |
| test suite | 20583 tests, 100% pass |
