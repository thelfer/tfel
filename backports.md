---
title: Backport campaign PR #1267 to rliv-5.0
---

# Backport campaign: PR #1267 (master fixes via rliv-5.1) → rliv-5.0

Goal: backport as many fixes as possible from
[PR #1267](https://github.com/thelfer/tfel/pull/1267)
("Backport of 58 defect fixes from master (PRs #1096 - #1263)",
commit `81ae2bbe3`, base `f2fc5f698` = rliv-5.1 HEAD) onto the current
branch `backrports-from-5.1` of the 5.0.x tree.

Rules: stay on the current branch; no commits; no pushes; all work stays
in the working tree. This file is the interruption/restart ledger.

## Setup facts

- Working tree: `/home/th202608/codes/tfel/tfel-5.0.x/src/rliv-5.0`
  (branch `backrports-from-5.1` @ `07a160f18`, contains `origin/rliv-5.0`).
- Source of fixes: local ref `pr-1267` (fetched `pull/1267/head`); full
  patch: `git diff f2fc5f698 pr-1267` saved at
  `/tmp/opencode/bp/pr1267.patch` (581 files, +8734/-3138).
- Per-file review diffs: `/tmp/opencode/bp/perfile/*.diff`.
- Build/check: `/home/th202608/codes/tfel/tfel-5.0.x/src/build`
  (`cmake . && CTEST_PARALLEL_LEVEL=<n> make -j<n> check`).
- Reference material: original campaign artifacts in
  `/home/th202608/codes/tfel/tfel-5.1.x/backport-artifacts/` (per-PR
  patches in `prs/PR<nnnn>.patch`, per-PR report `per-pr.json`, guide
  `BACKPORT-GUIDE.md`); 5.1 working copy of the result at
  `/home/th202608/codes/tfel/tfel-5.1.x/src/rliv-5.1` (branch
  `backport-master-fixes`).
- File lists: `/tmp/opencode/bp/cls-{clean,manual,new,missing,already}.txt`
  (classification against the 5.0 working tree, produced by
  `/tmp/opencode/bp/classify.sh` using `git apply --check` / `-R --check`).

## Classification (581 files of PR #1267)

| class | count | meaning / plan |
| --- | --- | --- |
| clean | 474 | hunk applies byte-clean to 5.0 → apply directly |
| already | 1 | `docs/doxygen/CMakeLists.txt` already carries the change |
| manual | 47 | 5.0 text differs → hand-merge hunk by hunk |
| new | 19 | files added by the PR → add + ensure CMake registration |
| missing | 40 | path absent in 5.0 → map to 5.0 counterpart (python bindings moved: 5.1 `bindings/python/X` == 5.0 `bindings/python/pybind11/X`; boost variant checked case-by-case) or document why not applicable |

## Status

- [x] Classification audit (see tables above)
- [x] Baseline `make check` on untouched tree — abandoned: contaminated
  by mid-run edits (log `/tmp/opencode/bp/baseline-check.log`, EXIT=2
  caused by conflict markers); failures must instead be attributed
  case by case against HEAD behaviour
- [x] Phase A: 474 clean files applied
- [x] Phase B: 19 new files added + CMake registrations
- [x] Phase C: 47 manual files merged (29 by `git apply --3way` side
  selection, rest hand-merged; final state has no conflict markers)
- [x] Phase D: 40 "missing" files triaged: 13 pybind11 binding fixes
  ported with path remapping, 1 (st2tost2) hand-merged, 3 binding files
  and 25 other files not applicable (see deviations)
- [x] Deferred small fixes (mfm.cxx, DSLUtilities.cxx, MTestParser.cxx,
  ViscoplasticFlowBase.cxx, IsotropicMisesPlasticFlowDSL.cxx,
  IsotropicBehaviourDSLBase.cxx, check_interface_restriction in
  Abaqus/Ansys/Aster, reportWarning helper port)
- [x] Phase E: final build + `make check` gate passed on 2026-11-01:
  `make -j32 check` EXIT=0, 20583/20583 tests passed
  (`/tmp/opencode/bp/gate-check-4.log`); previous gates
  (gate-check-2/3.log) exposed 5.1-only test content, now removed
- [x] Index cleanup: `git reset` (mixed) executed; index matches HEAD,
  no merge stages, all 523 modifications + 2 PR deletions unstaged,
  new files untracked

## Deviations (fixes skipped or adapted)

Skipped because the fix targets 5.1-only code or features absent in 5.0:

- `BeforeFlowRule` machinery, flow-id/ihrs-map DSL,
  `equivalent_strain_external_name` (InelasticFlowBase, ImplicitDSLBase
  predictor-normalisation hunks).
- `sortEigenValues` free function (5.0 `Internals/SortEigenValues`
  bubble sort is already correct).
- Homogenation documentation/bindings sections;
  `bindings/python/mfront/database/MFrontDatabase.cxx`,
  `bindings/python/tfel/material/LinearHomogenizationBounds.cxx`,
  `bindings/python/tfel/material/__init__.py{.in,.version.in}`
  (homogenisation python API does not exist in 5.0).
- `docs/mfront/dianafea/*` (directory absent in 5.0),
  `docs/web/mtest-nonlinear-constraints.md`,
  `docs/web/tfel-math-numerical-integration.md`, `env/env*.{sh,fish}.in`.
- `mfront/include/MFront/BehaviourBrick/BehaviourDSLUtilities.hxx`
  include hunks (file absent in 5.0), `printLagrangeMultipliers`
  (absent in 5.0 mtest), glossary boost-vs-pybind divergence.
- `include/TFEL/Math/Qt/GaussKronrodQuadrature.ixx`,
  `include/TFEL/Math/stensor2.hxx`, Eshelby/homogenisation
  headers/tests (`IsotropicModuli.hxx`, `AnisotropicEshelbyTensor`,
  `LinearHomogenizationBounds/Schemes`, `LocalisationTensor`),
  `mfront/{include/MFront,src}/CodeGeneratorUtilities.{hxx,cxx}`
  (5.1 reorganisation; its `writeBoundsChecks` overloads were ported
  into 5.0 `DSLUtilities` instead).
- `MTest::getInternalStateVariableValue` second overload (5.0 pybind11
  binding already registers both variants).
- `DianaFEAInterface.cxx`: stray `systemCall::mkdir("abaqus")` absent in
  5.0; hard-coded `TRIDIMENSIONAL` traits are correct because 5.0's
  interface only supports that hypothesis.
- `DefaultDSLBase.cxx` warning-string hunks (code absent in 5.0);
  `ImplicitDSLBase.cxx` broken-string-concatenation hunks (5.0 strings
  are already correct).
- New 5.1 unit-system tests not portable: `tests/Math/qt5.cxx`,
  `tests/Math/division_by_scalar.cxx` removed and deregistered;
  `tests/Math/qt.cxx` reverted; qt/unit fragments removed from
  `tests/Math/stensor/stensor9.cxx` and
  `tests/Math/st2tost2/ST2toST2ChangeBasis.cxx`.

Adapted to 5.0 structure:

- `PerformanceProfiling.cxx`: `getBehaviourProfilerIndex` ported with
  the 28 `BehaviourProfiler` constants of 5.0 (names match, including
  `TINYMATRIXSOLVE`); 5.0 string/stream output style kept.
- `BehaviourDSLCommon.cxx`: dual-syntax
  `treatComputeThermalExpansion{First,Second}Syntax` split is 5.1-only;
  the "can not be used along with" message fix applied to the 5.0
  single-syntax body instead.
- `BehaviourCodeGeneratorBase.cxx`: stress-free-expansion block uses
  the 5.0 symbol names `dl0_l0`/`dl1_l0` (no `mfront_` prefix).
- Bounds checks per component: `writeBoundsChecks` /
  `writePhysicalBoundsChecks` `VariableBoundsDescription` overloads
  ported into 5.0 `DSLUtilities.{hxx,cxx}` (5.1 keeps them in
  `CodeGeneratorUtilities`).
- `reportWarning(std::string_view)`: minimal port into
  `MFrontLogStream.{hxx,cxx}` (5.1 warning-mode infrastructure not
  backported); `check_interface_restriction` lambdas added to
  Abaqus/Ansys/Aster interfaces (Ansys/Aster use parameter `key`,
  Abaqus/CalculiX use `k`); CalculiX call sites added.
- `mfront/src/DSLBase.cxx::treatLonelySeparator`: `std::prev(current)`
  line/offset fix kept, output through ported `reportWarning`.
- pybind11 bindings: 5.1 `bindings/python/{mfm_test_generator,mfront,
  mtest,tfel}` mapped onto 5.0 `bindings/python/pybind11/...`; the
  boost bindings in `bindings/python/boost` were left untouched.
- `tests/Math/view.cxx`: `const_qt_ref<Time, double>` alias used as in
  5.0; no `coalesced_view` test (5.1-only feature).

## Work log

- Phase A: `git apply --include` for all 474 clean files.
- Phase B: 19 new files copied from `pr-1267` blobs; registrations
  added (rungekuttaelasticity tests, `RungeKuttaElasticity.mfront` +
  `_rk4` install lines, `tests_math(rk_final_time)`, etc.).
- Phase C: 47 manual files; conflicts resolved keeping the minimal PR
  hunk from `/tmp/opencode/bp/perfile/<file>.diff` and discarding
  mis-anchored whole-5.1-function "theirs" sides.
- Phase D: pybind11 rewrite `bindings/python/X` →
  `bindings/python/pybind11/X` applied to 13 files
  (`/tmp/opencode/bp/pyb-rewritten.patch`); `st2tost2` pair-index
  fix hand-merged.
- Gate fixes found by build/check iterations: removed 5.1 unit-system
  test content (qt/qt5/division_by_scalar/stensor9/
  ST2toST2ChangeBasis); added `check_interface_restriction` to
  Abaqus/Ansys/Aster/CalculiX; ported `reportWarning`.
- Final state: 523 modified, 2 deleted
  (`docs/mfront/Model/GlobalParameter.md` and
  `src/Utilities/ArgumentParserBase.cxx`, both deleted by the PR; its
  symbols live in `src/Utilities/Argument.cxx`), 14 new files from the
  PR (rungekutta elasticity behaviours/tests, profile references,
  `StiffnessTensorTest`, `StressCriteriaTest`, `rk_final_time`,
  `castemelasticity-cubic.ptest`, `test_TestLauncher.cxx`), all
  registered in CMake and covered by the passing test run
  (20583/20583).

## Post-PR follow-up (audit of the per-PR landing)

After `4849ed4b1` was pushed and PR #1268 opened, every one of the 58
source PRs was re-checked against this branch with two machine audits
(per-PR patches from
`/home/th202608/codes/tfel/tfel-5.1.x/backport-artifacts/prs/`):

- added lines of the PR present in the 5.0 file (lower bound on
  landing; reports low rates for wrapped/hand-merged text);
- removed lines of the PR still present with an unchanged occurrence
  count between `07a160f18` (pre-backport) and the current tree
  (candidates for missed fixes).

The second audit flagged 12 file-per-PR pairs; 9 were false positives
(duplicated text outside the changed block, 5.1-only functions, the
boost-only glossary generator, the `madnex` documentation samples).
Three were applicable and are now applied:

- #1180 `mfront/src/IsotropicMisesPlasticFlowDSL.cxx`: error message
  said `IsotropicMisesCreepDSL::endsInputFileProcessing`.
- #1184 `docs/doxygen/CMakeLists.txt`: stopped installing the
  non-existent `docs/mfront-doxygen.png`.
- #1154 `tests/Math/stensor/stensor_eigenvalues.cxx`: added the
  `FSESQLEIGENSOLVER` case (`test5`), which passes on 5.0.

The per-PR landing table (PR, subject, files, paths existing in 5.0,
files changed here, share of added lines present) is regenerated by
`/tmp/opencode/bp/pr-table.json` and is published as section 2 of
`backports-summary.md`, hence of PR #1268.

## Gate re-run after the audit (gate-check-5)

`CTEST_PARALLEL_LEVEL=32 make -j32 check`: 20582/20583. The single
failure is
`abaqusexplicitmieheapellambrechtlogarithmicstrainelasticity-uniaxialloading-1-2D_Random_mtest`,
a random MTest (`--rounding-direction-mode=Random`) failing on a
tolerance comparison of `SRR`; relaunched alone it passes 29 times out
of 30 (`/tmp/opencode/bp/flake-27.log`). The only code difference with
the gate-check-4 run (20583/20583) is an error message, a unit test and
a doxygen install rule, so this is inherent flakiness of that random
test and not a regression of the campaign.
