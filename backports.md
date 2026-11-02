# Backport of PR #1268 fixes onto rliv-4.2

This file is the working ledger of the campaign that backports the
fixes collected in
[PR #1268](https://github.com/thelfer/tfel/pull/1268) (Backport of
defect fixes from rliv-5.1, itself PR #1267: master PRs #1096 - #1263)
from the 5.0.x branch `backrports-from-5.1` onto this 4.2.x working
copy. It is used to restart the work in case of interruption.

## Objective and constraints

- Source: commit `2ca7f3814` of branch `backrports-from-5.1` in the
  5.0.x working copy
  `/home/th202608/codes/tfel/tfel-5.0.x/src/rliv-5.0`; the authoritative
  change set is `git diff 418aaa0ff 2ca7f3814` (612 files,
  +10529/-4690), saved at `/tmp/opencode/bp42/pr1268.patch`.
  Per-file review diffs are at `/tmp/opencode/bp42/perfile/*.diff`
  (path separators replaced by `%`), the file/status list is
  `/tmp/opencode/bp42/files-status.txt`.
- Target: this tree (`/home/th202608/codes/tfel/tfel-4.2.x/src/rliv-4.2`),
  branch `backrports-from-5.0`, based on `origin/rliv-4.2` plus the
  commits `5076c75fc..667b5f7fd` which already backported a first
  batch of rliv-5.0 fixes (43 files of source commit `07a160f18`).
- Hard constraints: stay on the current branch; no commits, no staging,
  no pushes; everything lives in the working tree; never use
  `reset --hard` or `stash`.
- Verification gate: the build tree `/home/th202608/codes/tfel/tfel-4.2.x/src/build`
  (`cmake . && make -j32 check`), logs saved under `/tmp/opencode/bp42/`.

## Phase status

- [x] Phase 0: generate patch, per-file diffs, classification.
- [x] Phase A: all 439 clean files applied (0 failures).
- [x] Phase B: the 21 new files added; registrations verified:
  `StiffnessTensorTest`, `rk_final_time`, `rungekuttaelasticity[-rk4]`
  (mtest + `install_mfront_data`), `castemptest(elasticity-cubic)`,
  `test_TestLauncher`; the `references/*.ref` files are referenced by
  path (`@top_srcdir@`) from the `.ptest`/`.mtest` files and need no
  registration. `Curve.cxx` / `L2Comparison.cxx` registration comes
  with the manual merge of `tfel-check/src/CMakeLists.txt` (Phase C).
  `backports.md` and `backports-summary.md` of the source commit are
  not backported (campaign artifacts).
- [x] Phase C: merge the 90 manual files (see "Phase C decisions"):
  - the blobs of the source commits were fetched from the 5.0.x
    working copy (`git fetch <path> backrports-from-5.1 rliv-5.0`,
    FETCH_HEAD=2ca7f3814), which enables `git apply --3way` and
    diffing 4.2 against the 5.0 base;
  - 24 files merged cleanly with `git apply --3way`
    (`/tmp/opencode/bp42/3way-ok.txt`); verified with
    `/tmp/opencode/bp42/audit.py` (all lines added by the PR are
    present in the working tree for every one of the 24 files);
  - 65 files with conflict markers (`/tmp/opencode/bp42/3way-conflict.txt`)
    resolved by hand (see "Phase C decisions"); no conflict marker
    remains anywhere in the tree;
  - `docs/mfront/Model/GlobalParameter.md` (deletion) failed the
    3-way merge; handled manually.
  - Resolution rule (as in the 5.1->5.0 campaign): the true change
    is the minimal hunk of `/tmp/opencode/bp42/perfile/<file>.diff`
    (5.0 base -> 5.0 after PR); conflicts are resolved by applying
    that minimal change to the 4.2 code, not by taking either
    conflict side. The 5.0 final state of any file can be shown with
    `git show 2ca7f3814:<path>` and the 5.0 base with
    `git show 418aaa0ffab:<path>`.
- [x] Phase D: triaged the 38 missing-path files (pybind11 bindings
  have no 4.2 counterpart; 4.2 uses boost bindings); see
  "Phase D decisions".
- [x] Phase E: build/test gate green. Plain `make -j32` of all default
  targets succeeded (`/tmp/opencode/bp42/build4.log`, 0 errors), then
  `make -j32 check` in the build tree
  `/home/th202608/codes/tfel/tfel-4.2.x/src/build` was fully green:
  `100% tests passed, 0 tests failed out of 20274`
  (`/tmp/opencode/bp42/check4.log`, 611 s). Clean-pass corrections
  needed after the first builds are listed under "Phase C decisions"
  (C++17 downgrades of `ThreadedTaskResult`/`qt`, the
  `tfel::utilities::starts_with` include, and the 5.x brace-init
  adaptations in `StressCriteriaTest`, `invert2` and `rk_final_time`).
- [x] Follow-up audit: searched every touched file for the lines the
  PR removes that are still present; 0 hits
  (`/tmp/opencode/bp42/removed-lines-report.txt`).

## Phase C decisions

The true change of each manual file is the minimal per-file diff of
`/tmp/opencode/bp42/perfile/`; conflicts were resolved by applying
that change to the 4.2 code. 5.x-only mechanisms never entered:
`areRuntimeChecksDisabled` (runtime-checks-disabled feature, absent
from 4.2), the `OutputStatus` return-status plumbing,
`BehaviourMaterialProperty::getExternalName` (4.2 keeps `name`),
C++20 syntax (`requires`, `std::remove_cvref_t`, `Concept auto`;
4.2 builds with C++17).

C++20 downgrades (files that applied cleanly but had to be adapted):

- `include/TFEL/System/ThreadedTaskResult.{hxx,ixx}`: the constrained
  variadic constructor (fix: `result(std::in_place,
  std::forward<Args>(args)...)`, replacing the constructor set that
  allowed copy-construction through the template) was rewritten with
  `std::enable_if` + `std::disjunction`/`std::is_same` over
  `std::decay_t<Args>`; the out-of-line definition uses
  `template <typename... Args, typename>`;
- `include/TFEL/Math/qt.hxx`: the new constrained copy-assignment of
  `Quantity` (write-through for reference ownership policies) is
  inexpressible in C++17 (SFINAE does not apply to non-template
  assignment operators) and was dropped; the added defaulted copy /
  move constructors and the `square_root` fix (`power<1,2>(q)`
  instead of `power<1,2>(q.getValue())`, which keeps the unit of the
  result) were kept; `tests/Math/qt.cxx` passes;
- `tests/Material/StressCriteriaTest.cxx`: brace-init adapted to the
  4.2 API (no variadic array constructors): `T{{...}}` with double
  literals everywhere (int elements are narrowing for
  `std::initializer_list<double>`).

Notable per-file conflict resolutions:

- root `CMakeLists.txt`: kept `CMAKE_CXX_STANDARD 17`, adopted the
  `CMAKE_CXX_STANDARD_REQUIRED ON` fix;
- `check_all.sh`: kept the 4.2 mingw relative paths;
- `mfront/src/AmitexInterface.cxx`: adopted the `++j` loop-increment
  fix; kept `pm.name`;
- `mfront/src/BehaviourCodeGeneratorBase.cxx`: kept the sfeh
  `dl0_l0/dl1_l0` restructure, bounds-check overloads,
  `tfel::utilities::starts_with` uses and IntegrationData output
  dedup; skipped the fsarray IntegrationData members and the
  array-derivative (`>&>`) fixes (5.x-only array machinery);
- `mfront/src/RungeKuttaCodeGeneratorBase.cxx`: took the 5.0 final
  state (AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL no longer
  time-interpolated); the static-modifier fix was already present;
- `mfront-query/src/QueryHandlerBase.cxx`: rebuilt from HEAD with the
  `--*-dsl-option` keyword renames + aliases, the empty-name fix and
  the rule-output rewrite;
- `mfront/src/OutputStatus.h`, `OverridableImplementation.cxx`: the
  5.x OutputStatus mechanism is not backported;
- `mfront/src/CppMaterialPropertyInterface.cxx`: adopted only the
  `tfel::raise_if` split (separate errno and non-finite-result
  messages, `strerror(mfront_errno)`); its 5.x wrapper guard is
  pre-existing machinery, not part of the PR;
- `mfront/src/OctaveMaterialPropertyInterface.cxx`: adopted the
  interpreter-parameter fix for the generated `_compute` (its body
  calls `global_varval`, which needs it), the removal of the stray
  duplicated `"OCTAVE_OUT_OF_BOUNDS_POLICY");` fragment and the
  missing `WARNING` branch in the UPPER case, the message-quoting
  fix, and `writeLawSymbol(out, name, mpd.law)`; reverted the PR's
  `return_status` parameter (OutputStatus plumbing; the 4.2
  call sites keep the 5-argument signature and the
  `return -nbr;`/`return nbr;` semantics);
- `mfront/src/AnsysInterface.cxx`: material-property example list
  split at `mps.second.getValueForModellingHypothesis(h)` with the
  orthotropic axes inserted, using `pm->name` (no
  `getExternalName()` in 4.2); also adopted: the
  `orthotropicAxesOffset` traits fix, the interface-restriction
  warnings, the isotropic offsets in the property-offset computation,
  the numerical-tangent fixes (`nD[i*(*NTENS)+j]`, `&sigb[0]`,
  declared loop index), `std::swap(deto[4],deto[5])`, the `CSE`
  argument, the stress swap before `convertToCauchyStress` and the
  `transpose(Dt)` calls, and the `*_getOutOfBoundsPolicy()` /
  `sfeh` arguments (both exist in 4.2 generated code and
  `AnsysData`);
- `mfront/src/CastemInterface.cxx`: the 5.x-only
  `writeGibianeMappingComments` overloads are not in the PR change
  set; dropped;
- `mfront/src/CastemModelInterface.cxx`: adopted the TEMP/DTEMP
  dereference, the declared-name bound checks (`vn` lambda fix), the
  brace-initialised `static constexpr` parameters and the
  anonymous-namespace comment fix; the guard-wrapped variants of the
  bound-check lambdas are 5.x machinery, not the PR;
- `mfront/src/GenericBehaviourInterface.cxx`: took the 5.0 final
  state of the gradient increments (useQt-aware scalar paths,
  `tfel::math::map<dv.type>` differences replacing
  `fsalgo::transform<std::minus<real>>`; `tfel::math::map` and
  `BehaviourDescription::useQt` exist in 4.2);
- `mfront/src/IsotropicBehaviourDSLBase.cxx`: error prefix corrected
  to `IsotropicBehaviourDSLBase::completeVariableDeclaration`;
- `mfront/src/MaterialPropertyDSL.cxx`: typo fix, body now ends with
  a newline (interfaces may append a preprocessor directive);
- `mfront/include/MFront/DianaFEA/DianaFEATangentOperator.hxx`: took
  the 5.0 final state of both plane-stress computers (`icste` =
  1/sqrt(2) and `one_half` factors); the 4.2 code did not compile
  (undeclared `cste`/`two` in the symmetric computer) and had wrong
  factors/indices in the general computer; the hard-coded
  TRIDIMENSIONAL specialisations are correct (4.2 only supports that
  hypothesis for this interface);
- `src/Material/FiniteStrainBehaviourTangentOperator.cxx`: typo fix
  (`gradident`) + `default: break;`;
- `mfront/src/ImplicitCodeGeneratorBase.cxx`: `starts_with` calls use
  `tfel::utilities::starts_with` (C++17; added the
  `TFEL/Utilities/StringAlgorithms.hxx` include).

Sub-agent batches (full per-file justifications in
`/tmp/opencode/bp42/reports/`):

- tests/Math batch (7 files): `st2tost2` needs the library fix
  (`isqrt2` in `ConvertT2toST2ToST2toST2Expr.hxx`),
  `stensor_eigenvalues` needs `is_negligible` (`<=` in
  `include/FSES/Utilities.ixx`), `stensor_eigenvectors` needs the
  Jacobi NaN reporting kept in `StensorEigenSolver.hxx`; the
  `HARARIEIGENSOLVER` specialisations were dropped (enumerator absent
  from 4.2); the `tmatrix.cxx` and `view.cxx` hunks target 5.x-only
  APIs and were skipped;
- TFEL-headers batch (18 files): all merged marker-free and
  `g++ -std=c++17 -fsyntax-only` clean;
- docs batch (18 files + `GlobalParameter.md` deletion + its
  `docs/mfront/Makefile.am` reference): see
  `/tmp/opencode/bp42/reports/docs-batch-1.md`;
  `IsotropicStrainHardeningMisesCreep-keywords.md` stays an intact
  4.2 page (only the 6 DSL-example fixes mirrored, no regeneration).

## Phase D decisions

All 38 files of `/tmp/opencode/bp42/cls-missing.txt` have no 4.2
counterpart; verified none needs a mirrored fix:

- `.github/workflows/doxygen.yml`: 4.2 has only the
  `build_win_mingw.yml` and `cmake.yml` workflows;
- the 13 `bindings/python/pybind11/...` files: 4.2 uses boost-python
  bindings under `bindings/python/<module>`; e.g. the pybind11
  `st2tost2` fix (tuple index + `&&`->`||` bounds check) targets a
  `__getitem__` lambda the boost binding does not have (it forwards
  to `operator()`, whose bounds checks are fixed in the library); the
  Anderson-acceleration array-policy fixes are pybind11-only;
- the 15 `docs/web/...-keywords.md` files: 5.x renamed/regenerated
  keyword pages (4.2 equivalents are named `Default*-DSL-keywords.md`
  etc.); the underlying DSL example fixes were mirrored by the docs
  batch; `docs/web/libraries_usage.md` appeared after 4.2;
- `include/TFEL/Math/Stensor/Internals/HarariEigenSolver.{hxx,ixx}`:
  HarariEigenSolver appeared after 4.2;
- `include/TFEL/Math/Matrix/TMatrixTMatrixExpr.hxx`: 4.2 has no
  TMatrix-expression files; the related test hunks
  (`tests/Math/matrix.cxx`, `tmatrix7.cxx`) target 5.x-only APIs and
  were skipped;
- `mfront-doc/src/MaterialPropertyDocumentationGenerator.cxx`: 4.2
  only has `BehaviourDocumentationGenerator.cxx`;
- `mfront/include/MFront/DataInterpolationUtilities.hxx`,
  `mfront/src/BehaviourInterfaceBase.cxx`,
  `mfront/src/StrainRateSensitiveIsotropicHardeningRule.cxx`,
  `mfront/src/TFELMFrontStaticConfig.cmake.in`: files introduced
  after 4.2;
- `mfront/tests/behaviours/DigitSeparatorTest.mfront`: new 5.x test
  (5.x registration machinery); not added.

## Classification

Computed with `/tmp/opencode/bp42/classify.sh` (git apply --check with
and without -R); lists in `/tmp/opencode/bp42/cls-*.txt`:

- clean: 439
- already applied: 22 (mostly from commit 667b5f7fd)
- manual: 90
- new files: 23 (2 excluded: backports.md, backports-summary.md)
- missing path: 38

## Deletions performed by the PR

- `docs/mfront/Model/GlobalParameter.md`: content moved into the
  keyword documentation.
- `src/Utilities/ArgumentParserBase.cxx`: definitions moved to
  `src/Utilities/Argument.cxx` in 5.0; to be checked against 4.2.
- `tfel-check/include/TFEL/Check/TestLauncherV1.hxx` and
  `tfel-check/src/TestLauncherV1.cxx`: removed by the tfel-check
  rework; their references in `tfel-check/src/CMakeLists.txt` are
  removed by the PR (that file is in the manual list).

## Notes and adaptations

(kept up to date during the campaign)

## Known pre-existing untracked files

`AGENTS.md`, `clean`, `diff-5.0.txt`, `licence.sh`, `script.sh`
(unrelated to the campaign; `diff-5.0.txt` records the previous
backport batch).
