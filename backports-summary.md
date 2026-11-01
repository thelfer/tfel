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

## 2. Outcome

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

## 3. Method

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
   the minimal PR diffs (see section 5 for the hard cases).
5. Phase D triaged the 40 "missing path" files: 13 python binding
   fixes were portable through a path remap, 1 was hand-merged, and
   the remaining 26 were documented as not applicable (section 6).
6. Phase E iterated the build/test gate until green (section 7).
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

## 4. What was backported (area overview)

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
- `bindings/python/pybind11` (12 files): see section 6.1.
- `docs` (about 90 files): documentation typo and content fixes that
  exist in the 5.0 documentation structure.
- miscellaneous: CI workflow fixes, `cmake/modules`, `check_all.sh`.

## 5. Notable adaptations to the 5.0 structure

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

## 6. Python bindings and other path-mapped files

### 6.1 pybind11 bindings

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
  `mtest/__init__.py.in`, `tfel/st2tost2.cxx` (section 5).

Not applicable (absent from 5.0): `mfront/database/MFrontDatabase.cxx`,
`tfel/material/LinearHomogenizationBounds.cxx`,
`tfel/material/__init__.py.in` and `__init__.py.version.in` (the
homogenisation python API was created after 5.0). The boost bindings
in `bindings/python/boost` were deliberately not touched: 5.1 dropped
them, and the glossary divergence (hand-written pybind `Glossary.cxx`
versus generated boost bindings) was documented rather than ported.

### 6.2 Files with no 5.0 counterpart at all

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

## 7. Fixes deliberately not backported (5.1-only context)

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
  the homogenisation docs/bindings sections (section 6),
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

## 8. Test content that had to be rejected

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

## 9. Gate iteration history

- gate-check.log: baseline attempt on the untouched tree; aborted
  (EXIT=2) because it ran while the tree still contained conflict
  markers; no clean baseline number is available, so every later
  failure was attributed manually.
- gate-check-2.log: 2 failures, both 5.1 unit-system test content
  (`division_by_scalar`, `qt`, `qt5` then `stensor9`,
  `ST2toST2ChangeBasis`); fixed per section 8.
- gate-check-3.log: 1 failure, `AsterInterface.cxx` (lambda capturing
  `k` while the parameter is named `key`); fixed per section 5.
- gate-check-4.log: EXIT=0, 20583/20583 tests passed.

## 10. Final state and follow-up hints

- Branch: `backrports-from-5.1`, untouched history; everything lives
  in the working tree (unstaged modifications and untracked new
  files), which is exactly the mode in which the campaign was run and
  which makes it easy to stage selectively later:
  `git diff f2fc5f698 pr-1267` remains the reference to replay any
  part of the work, and `git status` plus `backports.md` describe the
  rest.
- Suggested commit organisation if this is ever turned into commits:
  one commit per upstream theme (the original campaign's `per-pr.json`
  in `/home/th202608/codes/tfel/tfel-5.1.x/backport-artifacts/` gives
  the 58 PR messages and the 5.1 deviations), plus one commit for the
  5.0-specific adaptations (DSLUtilities bounds overloads,
  reportWarning port, `dl0_l0` renames, pybind11 path remap) so they
  can be reviewed independently.
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

## 11. Statistics

| item | value |
| --- | --- |
| files in the source change set | 581 |
| applied cleanly | 474 |
| merged manually | 47 |
| new files added | 19, of which 2 (`qt5.cxx`, `division_by_scalar.cxx`) had to be dropped again as 5.1 unit-system tests (section 8); 17 remain, all registered and passing |
| already applied | 1 |
| missing-path files triaged | 40 (14 portable to pybind11, 26 not applicable) |
| final diff vs HEAD | 525 files, +7231/-2973 |
| test suite | 20583 tests, 100% pass |
