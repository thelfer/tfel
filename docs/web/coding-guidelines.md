---
title: Coding guidelines
author: Thomas Helfer
date: 22/10/2026
lang: en-EN
---

This page gathers the coding conventions used in the `TFEL`/`MFront`
sources. It is written for **new developers** and for **coding
agents**, which are expected to produce code indistinguishable from
the existing sources.

The conventions described here apply to the `C` and `C++` sources of
all the components of the project (`TFEL`, `MFront`, `MTest`,
`TFELCheck`, the various solver interfaces and the bindings). They
complement the general contribution process described in the
[contributing guidelines](contributing.html).

Rules stated with **shall** are mandatory. When a rule is only
partially satisfied by the historical sources, this is explicitly
mentioned: a coding agent shall apply the rule to the code it writes
or modifies, but shall not rename unrelated entities as a side
effect.

# Quick reference

| Topic | Rule |
|---|---|
| Formatting | `clang-format` with the `.clang-format` file of the repository, `./format.sh` before submitting |
| File name | same name as the main class it declares, `UpperCamelCase` |
| File header | doxygen comment describing the file followed by the copyright statement |
| Include order | standard headers, then third-party headers, then `TFEL` headers, then headers of the current library, then own header |
| Types (classes, structures, aliases, enumerations) | `UpperCamelCase` |
| Methods and free functions | `camelCase` |
| Data members and variables | `snake_case` |
| Template parameters | `UpperCamelCase` (or a single letter for generic parameters) |
| Macros and enumerators | `ALL_UPPER_CASE`, prefixed by the library name for macros |
| `static constexpr` constants | `UpperCamelCase`, with a short lowercase alias for physical constants |
| Entity kinds | `struct` (never `class`), public members first |
| Data members and methods | always qualified by `this->` |
| Documentation | every member, function and method documented with `doxygen` |
| Error handling | `tfel::raise`, `tfel::raise_if`, exception classes, no macro |
| Closing comments | `}  // end of <name>` on namespaces, types, functions and enumerations |

# Tooling

## Formatting

The sources are formatted with `clang-format` using the
[`.clang-format`](https://github.com/thelfer/tfel/blob/master/.clang-format)
file at the root of the project:

~~~~{.yaml}
Language: Cpp
Standard: Cpp11
BasedOnStyle: Google
ColumnLimit: 80
Cpp11BracedListStyle: true
BinPackParameters: false
NamespaceIndentation: All
SortIncludes: false
~~~~

The main consequences are:

- a hard limit of **80 columns**,
- a **2 space** indentation, and everything inside a namespace is
  indented (`NamespaceIndentation: All`),
- opening braces are on the same line,
- function arguments are either all on the same line or **one per
  line**, aligned on the opening parenthesis (`BinPackParameters:
  false`),
- **`SortIncludes: false`**: the include order is meaningful and is
  maintained manually (see [Including headers](#including-headers)).

Run `./format.sh` before submitting any modification. This script
only formats `*.hxx`, `*.ixx` and `*.cxx` files.

## Static analysis and verification

- `./check_all.sh -j <n>` builds and tests the library in many
  configurations, `./check_all.sh -f` is the fast variant.
- `./cppcheck.sh -j <n>` runs `cppcheck`.
- [`clang-tidy.md`](https://github.com/thelfer/tfel/blob/master/clang-tidy.md)
  lists the `clang-tidy` checks which are disabled, and thus the
  warnings which are not considered as defects.
- `git commit` messages follow the usual conventions: a short
  imperative summary followed by a blank line and a detailed
  description.

# Source files

## Extensions and roles

| Extension | Role |
|---|---|
| `.hxx` | interface: declarations, type aliases, enumerations, `static constexpr` members, function declarations |
| `.ixx` | inline and template definitions, included at the bottom of the corresponding `.hxx` |
| `.cxx` | non template definitions, `static` and anonymous namespace helpers |
| `.h`, `.c` | reserved to the `C` interfaces (see [C sources](#c-sources)) |

The `.hxx`/`.ixx` split is used when the implementation is long: the
`.hxx` stays a readable interface and the `.ixx` holds the
definitions. Both files have the same base name and both have their
own include guard.

The `.ixx` file is included **at the bottom of the `.hxx`, after the
last namespace and before the closing `#endif`**:

~~~~{.cpp}
}  // end of namespace tfel::material

#include "TFEL/Material/Hosford1972YieldCriterion.ixx"

#endif /* LIB_TFEL_MATERIAL_HOSFORD_HXX */
~~~~

A `.cxx` file shall never include a `.ixx` file.

## Directory layout

- `Forward/` sub-directories contain forward declarations only.
- `Internals/` sub-directories contain implementation details which
  are not part of the public interface.
- Private headers of a library are installed with `install_header`
  (see [Build system](#build-system)).

## Naming

A file is named after the main type it declares, in
`UpperCamelCase`: `ModellingHypothesis.hxx`, `BehaviourDescription.cxx`.

Files whose name starts with a lowercase letter are the exception:
they declare types mimicking the standard library (`stensor.hxx`,
`matrix.hxx`, `qt.hxx`, `power.hxx`, `runtime_array.hxx`, ...) or are
executable entry points (`main.cxx`, `mfront.cxx`, `mtest.cxx`,
`mfm.cxx`).

# File header

Every source file and every header shall start by a `doxygen`
comment describing the file and containing the copyright statement:

~~~~{.cpp}
/*!
 * \file   include/TFEL/Material/BoundsCheck.hxx
 * \brief  This file declares the BoundsCheck class.
 * \author Thomas Helfer
 * \date   06/03/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */
~~~~

The tags are always in this order: `\file`, `\brief`, `\author`,
`\date`, `\copyright`.

- `\file` is the path of the file, relative to the root of the source
  tree.
- `\brief` is a one line description of the file. It shall not be
  left empty in new files.
- `\author` is the list of the authors of the file.
- `\date` is the creation date, in the `DD/MM/YYYY` format.
- `\copyright` is the copyright statement, reproduced verbatim. The
  end year of the range is updated by `./licence.sh` for the whole
  tree: the exact text of the statement shall not be altered,
  including the historical spelling of its last lines.

Code generated by `MFront` uses the same header, with the
`\copyright` line replaced by the mention of the generator version.

# Include guards

Headers are protected by an include guard built from the include
path, in all upper case, ending with the extension of the file:

~~~~{.cpp}
#ifndef LIB_TFEL_MATH_FORWARD_STENSOR_HXX
#define LIB_TFEL_MATH_FORWARD_STENSOR_HXX
...
#endif /* LIB_TFEL_MATH_FORWARD_STENSOR_HXX */
~~~~

The closing `#endif` shall always repeat the name of the macro in a
block comment. The same applies to every conditional compilation
block: `#endif /* NDEBUG */`, `#endif /* __cplusplus */`, ...

`#pragma once` is not used. `.cxx` files have no include guard.

# Including headers

## Order

The order of the `#include` directives is meaningful and shall be
preserved:

1. standard headers, in angle brackets,
2. headers of third-party libraries, in quotes,
3. `TFEL` headers,
4. headers of the current library (`MFront`, `MTest`, ...),
5. the header implemented by the current `.cxx` file, last.

`TFEL` headers shall therefore appear **after** the standard headers
and after the headers of the third-party libraries. A blank line
separates the standard headers from the following groups; it is
optional but recommended.

~~~~{.cpp}
#include <cstring>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/BehaviourData.hxx"
~~~~

Since `SortIncludes` is disabled, adding a new include shall not
trigger a reordering of the existing ones.

## Syntax

- Project headers are always included with quotes and with a path
  relative to the root of the `include` directory of the library:
  `#include "TFEL/Math/stensor.hxx"`. Relative paths (`"../Foo.hxx"`)
  and angle brackets are not used for project headers.
- `TFEL/Raise.hxx` is the conventional first project header of a
  `.cxx` file which reports errors.
- Conditional includes are written with `#ifdef HAVE_*` and the
  closing `#endif` comment.
- Headers shared between `C` and `C++` use `#ifdef __cplusplus` to
  include the `C++` or the `C` version of a standard header.

# Namespaces

Namespaces use the `a::b::c` syntax and are always closed by a
comment giving their **full** name:

~~~~{.cpp}
namespace tfel::material {

  ...

}  // end of namespace tfel::material
~~~~

The main namespaces are `tfel::math`, `tfel::material`,
`tfel::system`, `tfel::tests`, `tfel::utilities`, `tfel::typetraits`,
`mfront`, `mfront::bbrick`, `mtest`, `tfel::check` and the namespaces
of the solver interfaces (`abaqus`, `aster`, `ansys`, `castem`,
`calculix`, `dianafea`, `lsdyna`, ...).

Implementation details are put in a sub-namespace named `internals`
or `details`.

`using namespace` is forbidden in headers. It is allowed in functions
of `.cxx` files, where it is the usual way to shorten tests and
implementations:

~~~~{.cpp}
  tfel::tests::TestResult execute() override {
    using namespace tfel::material::moduli_symmetries;
    ...
  }  // end of execute
~~~~

# Naming conventions

## Types

Types, enumerations and type aliases are in `UpperCamelCase`:
`ModellingHypothesis`, `BehaviourDescription`, `IsotropicModuli`,
`StressStensor`, `TestResult`.

Type aliases use `using` and never `typedef`. They are documented by
the conventional comment `//! a simple alias`:

~~~~{.cpp}
    //! a simple alias
    using size_type = std::vector<std::thread>::size_type;
~~~~

## Methods and free functions

Methods and free functions are written in `camelCase`, starting with
a verb:

~~~~{.cpp}
  static bool isModellingHypothesis(const std::string&);
  static std::string toString(const Hypothesis);
  [[nodiscard]] size_type getNumberOfThreads() const;
  TFEL_HOST_DEVICE constexpr StressType computeLambda(const StressType&);
~~~~

The conventional prefixes are `get`/`set`, `is`/`has`, `check`,
`throw`, `compute`, `add`/`insert`/`remove`, `to`.

## Data members and variables

Data members and variables, including function parameters when they
are not trivial, are written in `snake_case`:

~~~~{.cpp}
    //! number of integration points
    const size_type nb_of_integration_points;
    //! name of the modelling hypothesis
    std::string modelling_hypothesis;
    //! lower bound
    double lower_bound = std::numeric_limits<double>::quiet_NaN();
~~~~

Multi-word names use underscores, never internal capitals:
`cblock_begin`, `is_mutable`, `numerical_jacobian_epsilon`,
`include_paths`.

Single word names are preferred for short lived variables and for
parameters whose meaning is given by the `\param` documentation:
`n`, `v`, `s`, `os`, `lb`, `ub`, `eps`.

The historical sources use a mix of `UpperCamelCase` and
`camelCase` for data members, without any prefix (`_`, `m_` and `s_`
are never used). New code and modified code shall use `snake_case`;
existing entities shall not be renamed outside of the scope of the
current change.

## Template parameters

Descriptive template parameters are in `UpperCamelCase`
(`StressStensor`, `NumericType`, `InputIterator`, `ValueType`),
generic ones use a single letter or a short list (`T`, `U`, `F`,
`Op`, `T1`, `T2`). Boolean and concept parameters use lowercase
(`real`, `stress`, `use_qt`).

## Enumerations

Enumerations are in `UpperCamelCase`, their enumerators in
`ALL_UPPER_CASE`. Each enumerator is documented by a trailing `//<!`
comment and the enumeration is closed by a `// end of enum` comment:

~~~~{.cpp}
  /*!
   * \brief list the possible values for the logging facilities
   *        provided by the getVerboseMode/getLogStream functions
   */
  enum VerboseLevel {
    VERBOSE_QUIET = -1,  //<! no output
    VERBOSE_LEVEL0 = 0,  //<! minimal output
    VERBOSE_LEVEL1 = 1,  //<! a simpler output
    VERBOSE_LEVEL2 = 2,  //<! a much detailled output
    VERBOSE_LEVEL3 = 3,  //<! the finer level for standard user
    VERBOSE_DEBUG = 4,   //<! an output adapted for debugging
    VERBOSE_FULL = 5     //<! a very detailled output
  };                     // end of enum VerboseLevel
~~~~

A plain `enum` is preferred to an `enum class` when the enumerators
are used as array indices or have to be visible in the `C` or
`Fortran` interfaces; an `enum class` is used otherwise.

## Macros and constants

Macros are in `ALL_UPPER_CASE` and prefixed by the library name
(`TFEL_`, `MFRONT_`, `MTEST_`, ...). They are documented with a
`\def` block:

~~~~{.cpp}
/*!
 * \def TFEL_PP_JOIN
 * \brief this macro joins its two arguments together.
 * \author Thomas Helfer
 * \date 28/08/2006
 */
#define TFEL_PP_JOIN(X, Y) TFEL_PP_DO_JOIN(X, Y)
~~~~

`static constexpr` constants use a descriptive `UpperCamelCase` name
and, when the constant is a well known physical quantity, a short
lowercase alias (see `TFEL/PhysicalConstants.hxx`).

# Structures and classes

`struct` shall be used in place of `class`: `struct` is used more
than 3 000 times in the sources, whereas `class` is essentially
reserved to template parameters.

**Public members are declared first.** Since `struct` is already
public by default, most types have no `public:` label at all: the
private section is written at the end of the type.

The conventional order of the members of a type is:

1. documentation of the type,
2. `using` aliases,
3. nested types and enumerations,
4. `static constexpr` constants and static string constants,
5. constructors, destructor, copy and move operators,
6. methods,
7. `protected:` section, if any,
8. `private:` section, if any, with private methods first and data
   members last.

~~~~{.cpp}
  /*!
   * \brief structure handling a fixed-size pool of threads
   */
  struct TFELSYSTEM_VISIBILITY_EXPORT ThreadPool {
    //! a simple alias
    using size_type = std::vector<std::thread>::size_type;
    /*!
     * \brief constructor
     * \param[in] n: number of thread to be created
     */
    explicit ThreadPool(const size_type);
    ...
    //! \return the number of threads managed by the pool
    [[nodiscard]] size_type getNumberOfThreads() const;
    //! destructor
    ~ThreadPool();

   private:
    //! enum describing the status of workers
    enum Status { WORKING, IDLE };  // end of enum Status
    //! status of the workers
    std::vector<Status> statuses;
    //! list of available threads
    std::vector<std::thread> workers;
    // the task queue
    std::queue<std::function<void()>> tasks;
  };  // end of struct ThreadPool
~~~~

Non-copyable types explicitly disable the special members rather than
omitting them:

~~~~{.cpp}
    //! \brief copy constructor (disabled)
    TestCase(const TestCase&) = delete;
    //! \brief assignement operator (disabled)
    TestCase& operator=(const TestCase&) = delete;
~~~~

Leaf types are declared `final`.

# The `this` pointer

Data members and methods shall always be qualified by `this->`, even
when the qualification is not required:

~~~~{.cpp}
  void LogarithmicStrainHandlerBase::checkLagrangianSetting() const {
    raise_if(this->s != LAGRANGIAN,
             "LogarithmicStrainHandlerBase::"
             "checkLagrangianSetting: "
             "setting is not set to LAGRANGIAN");
  }  // end of LogarithmicStrainHandlerBase::checkLagrangianSetting
~~~~

This convention makes member accesses immediately visible, avoids
shadowing bugs, and is mandatory in template classes, where the
`this->template` form is required to access dependent members:

~~~~{.cpp}
    this->template test_1<float, false>();
    this->template computeEigenValues<es>(vp0, vp1, vp2, this->v, b);
~~~~

# Documentation

## General rules

All members, functions and methods shall be documented with
`doxygen`, in the `.hxx` file: the `doxygen` configuration
(`Doxyfile.in`) only processes `*.hxx`, `*.ixx` and `*.doxygen` files,
so a `.cxx` file is never documented.

Documentation is written in `C++` comments, using backslash commands
(`\brief`, never `@brief`):

- `/*! ... */` for types, functions and non-trivial members,
- `//! ...` for one line comments: constructors, destructors,
  aliases, getters, data members, enumerators.

`///` and `/** ... */` are not used.

## Conventional forms

~~~~{.cpp}
  /*!
   * \brief compute the Hosford yield stress
   * \tparam StressStensor: type of the stress tensor
   * \tparam HosfordExponentType: type of the Hosford exponent
   * (could be a numeric type or an integer type)
   * \param[in] s: stress tensor
   * \param[in] a: Hosford exponent
   * \param[in] e: criterion used to check if the stress are null
   */
  template <tfel::math::StensorConcept StressStensor,
            typename HosfordExponentType>
  TFEL_HOST_DEVICE HosfordStressType<StressStensor> computeHosfordStress(
      const StressStensor&,
      const HosfordExponentType,
      const HosfordStressType<StressStensor>);
~~~~

- parameters are always annotated with their direction:
  `\param[in]`, `\param[out]`, `\param[in,out]`, and the name is
  followed by a colon. Descriptions of several parameters are
  column-aligned when it helps,
- `\return` is used for the returned value, and is often the only
  tag of a trivial accessor,
- template parameters are documented with `\tparam`,
- limitations of the documented entity are stated with `\pre`,
  `\note`, `\see`.

The following one line comments are conventional and shall be used
consistently:

~~~~{.cpp}
    //! a simple alias
    //! \brief default constructor
    //! \brief copy constructor (disabled)
    //! \brief assignement operator (disabled)
    //! destructor
    //! \return the name of the class
~~~~

Documentation is also expected for private members, private methods
and private data members, whose comment states their role.

# Language conventions

## Constness

`const` is written before the type and is applied to parameters
even when they are passed by value, in the declarations as well as in
the definitions:

~~~~{.cpp}
  static std::string toString(const Hypothesis);
  void checkModellingHypothesis(const Hypothesis) const;
~~~~

Local variables are `const` by default: `const auto` is far more
frequent than `auto`, and `for (const auto& x : ...)` far more
frequent than `for (auto& x : ...)`.

## Attributes

- `[[nodiscard]]` is applied to every non-void function whose result
  cannot be ignored, that is to say essentially all getters and all
  pure queries.
- `[[noreturn]]` is applied to `raise`, `raise_if` and the
  `throw...` helpers.
- `noexcept` is used for everything that cannot throw.
- `override`, `final` and `explicit` are always used where they apply.

Attributes come before the visibility macro and before the return
type: `[[nodiscard]] TFEL_HOST_DEVICE ...`.

## Closing comments

Namespaces, types, functions and enumerations are closed by a comment
giving their name, with two spaces before the comment:

~~~~{.cpp}
  }  // end of computeIsotropicStiffnessTensor
};  // end of struct BehaviourData
}  // end of namespace mfront
}  // end of enum VerboseLevel
~~~~

Control flow blocks (`if`, `for`, `while`) are not annotated.

## Exported symbols

A type or a function belonging to the public interface of a library
is annotated with the visibility macro of the library,
`<LIBRARY>_VISIBILITY_EXPORT`, where the library name is the name of
the `CMake` target (`TFELMATH_VISIBILITY_EXPORT`,
`TFELMATERIAL_VISIBILITY_EXPORT`, `TFELTESTS_VISIBILITY_EXPORT`,
`MFRONT_VISIBILITY_EXPORT`, ...). Such macros are defined in
`TFEL/Config/TFELConfig.hxx` for the `TFEL` libraries and in
`MFront/MFrontConfig.hxx` for the `MFront` libraries. The macro is
written immediately before `struct`, or before the return type of a
function, after the attributes.

`TFEL_VISIBILITY_LOCAL` is used for helpers which are defined in
headers but must not be exported.

Templates are not exported: their definitions are in the `.ixx` files
and the non template helpers they use are marked
`TFEL_VISIBILITY_LOCAL`.

## Portability

Every function which must be usable on a GPU or through a `SYCL`
backend is annotated with `TFEL_HOST_DEVICE` (or `TFEL_HOST`,
`TFEL_DEVICE`), written before the return type and before `constexpr`
(see `TFEL/Config/TFELConfig.hxx`).

The same restriction applies to the code of such functions: no
allocation, no exception, no `STL` container which is not supported
by the targeted backends.

`TFEL_INLINE` and its variants are used instead of `inline` when the
inlining is performance critical.

# Error handling

## Error handling in `TFEL/Math` and `TFEL/Material`

Due to GPU compatibility, usage of exceptions in `TFEL/Math` and
`TFEL/Material` is generally prohibited. There are exceptions for parts
not meant to ported on the GPU, notably `TFEL/Math/Parser`. For
historical reasons and backward compatibility, some usage of exceptions
may still exist in other parts of those libraries.

As a general rule, one shall use the return type to indicate success or
failure: boolean values (`true` for success, `false` for failuer), error
codes, etc..

## Error handling in other parts of the project

There is no error reporting macro. Errors are reported by calling
functions declared in `TFEL/Raise.hxx`:

~~~~{.cpp}
  template <typename Exception = std::runtime_error>
  [[noreturn]] TFEL_VISIBILITY_LOCAL TFEL_INLINE void raise();

  template <typename Exception = std::runtime_error, typename... Args>
  [[noreturn]] TFEL_VISIBILITY_LOCAL TFEL_INLINE void raise(Args&&...);

  template <typename Exception = std::runtime_error, typename... Args>
  TFEL_VISIBILITY_LOCAL TFEL_INLINE void raise_if(const bool, Args&&...);
~~~~

The exception object is built outside of the `throw` statement, so
that the `cert-err60-cpp` rule is satisfied even if the constructor
of the exception throws.

Error messages are written as `<Class>::<method>: <message>`. They
are split into adjacent string literals to respect the 80 columns
limit, and completed by string concatenation when they contain
dynamic parts:

~~~~{.cpp}
    tfel::raise_if(this->behaviour.empty(),
                   "BehaviourDescription::getBehaviourName: "
                   "behaviour name not defined");
    ...
    tfel::raise("BehaviourData::CodeBlocksAggregator::set: " + m);
~~~~

Exception classes are `struct` types, exported and derived from
`TFELException` (`TFEL/Exception/TFELException.hxx`), `MaterialException`
(`TFEL/Material/MaterialException.hxx`), `MathRunTimeException`,
`SystemError`, ... according to the library they belong to.

Out-of-bounds checks are handled by the policy classes of
`TFEL/Material/BoundsCheck.hxx`, which use the `Warning`, `Strict` or
`None` policies of `TFEL/Material/OutOfBoundsPolicy.hxx`.

# Dynamic polymorphism

## Dynamic polymorphism in `TFEL/Math` and `TFEL/Material`

Due to GPU compatibility, usage of dynamic polymorphism in `TFEL/Math`
and `TFEL/Material` is generally prohibited. There are exceptions for parts
not meant to ported on the GPU, notably `TFEL/Math/Parser`.

Static polymorphism, through CRTP (Curiously Recurring Template Pattern,
see @coplien_curiously_1995), is used in many places as a substitute for
dynamic polymorphism.

## Dynamic polymorphism in other parts of the project

Dynamic polymorphism is used extensively in other parts of the project.
The general scheme is as follows:

- The root of a class hierarchy shall be pure virtual. Its name shall
  start with `Abstract` followed by the name of the class hierarchy:
  `AbstractNonLinearSystemSolver` for instance.
- In most cases, a large part of the interface has meaningful defaults.
  Those defaults are then implemented in a class whose name is the name
  of the class hierarchy suffixed by `Base`: `NonLinearSystemSolverBase`
  for instance.
- Leaf classes shall be declared `final`.

Multiple inheritance combined with dynamic polymorphism is discouraged.

### Factories

A hierarchy whose instances are designated by a name, given in an input
file, on the command line or by a script, is associated with a factory.
The factory is declared in `<Name>Factory.hxx`, in the same namespace
as the abstract type, and named after the type it creates:
`DSLFactory`, `BehaviourInterfaceFactory`, `ModelInterfaceFactory`,
`MaterialPropertyInterfaceFactory`, `NonLinearSystemSolverFactory`,
`BehaviourBrickFactory`, `bbrick::InelasticFlowFactory`,
`bbrick::IsotropicHardeningRuleFactory`, `bbrick::StressCriterionFactory`,
`bbrick::StressPotentialFactory`, `bbrick::PorosityNucleationModelFactory`,
`bbrick::StrainRateSensitivityFactorFactory`,
`generic_parallel::material_property::BackendFactory`, ...

All those factories follow the same pattern:

- the factory is a `struct`, exported with the visibility macro of the
  library (`MFRONT_VISIBILITY_EXPORT`) and documented by a one line
  `doxygen` comment: `//! \brief abstract factory for <types>`;
- the factory is a singleton: the unique instance is returned by a
  static function, `getFactory()`, `get<Name>Factory()` or `get()`,
  implemented with a function local static. The constructor is private,
  the destructor is defined in the `.cxx` file, and the copy and move
  operations are deleted;
- the type of the functions which create an instance is defined by a
  `using` alias, usually named `Generator`, which is a `std::function`
  returning a `std::shared_ptr` (or a `std::unique_ptr`) of the
  abstract type. The older sources use a `typedef` of a function
  pointer named `constructor` or `<Type>Creator` instead;
- the registered generators are stored in a `std::map<std::string, ...>`
  data member, `generators` or `constructors`, whose key is the name
  used by the user. `std::less<>` is used as comparator when the
  lookup is performed with a `std::string_view`;
- the registration method (`addGenerator`, `registerSolver`,
  `registerDSLCreator`, `registerInterfaceCreator`,
  `registerAbstractBehaviourBrick`, ...) inserts the generator and
  reports a duplicate name as an error, using `tfel::raise` or
  `tfel::raise_if`;
- the creation method (`generate`, `get`, `getSolver`, `getInterface`,
  `createNewDSL`, ...) looks up the name and raises an error when the
  name is unknown. The error message lists the available names when
  their number is reasonable;
- the available names are returned by a `getRegistred<PluralType>()`
  method, which uses `getKeys` of `MFront/MFrontUtilities.hxx` and is
  used by the error messages, the `--help` output and the completion
  scripts. The misspelling `Registred` is the established spelling of
  the interface and shall be preserved;
- the standard implementations are registered by the private
  constructor of the factory, which only contains calls to the
  registration method with lambdas calling `std::make_shared`.

A factory may be both coarse grained and extensible from the outside:
`BehaviourBrickFactory` is the single factory of all the behaviour
bricks, its constructor registers the generic bricks, and the DSLs
register the bricks they own with `registerAbstractBehaviourBrick`.

~~~~{.cpp}
  //! \brief abstract factory for isotropic hardening rules.
  struct MFRONT_VISIBILITY_EXPORT IsotropicHardeningRuleFactory {
    //! \brief a simple alias
    using Generator = std::function<std::shared_ptr<IsotropicHardeningRule>()>;
    //! \return the uniq instance of the class
    [[nodiscard]] static IsotropicHardeningRuleFactory& getFactory();
    //
    IsotropicHardeningRuleFactory(IsotropicHardeningRuleFactory&&) = delete;
    IsotropicHardeningRuleFactory(const IsotropicHardeningRuleFactory&) =
        delete;
    IsotropicHardeningRuleFactory& operator=(IsotropicHardeningRuleFactory&&) =
        delete;
    IsotropicHardeningRuleFactory& operator=(
        const IsotropicHardeningRuleFactory&) = delete;
    //! \brief return the list of available isotropic hardening rules
    [[nodiscard]] std::vector<std::string> getRegistredIsotropicHardeningRules()
        const;
    /*!
     * \brief add a new generator
     * \param[in] n: name of the generator
     * \param[in] g: generator
     */
    void addGenerator(const std::string&, const Generator&);
    /*!
     * \brief generate a new isotropic hardening rule
     * \param[in] n: name of the isotropic hardening rule
     */
    [[nodiscard]] std::shared_ptr<IsotropicHardeningRule> generate(
        const std::string&) const;

   private:
    //! \brief default constructor
    IsotropicHardeningRuleFactory();
    //! \brief destructor
    ~IsotropicHardeningRuleFactory();
    //! \brief generators
    std::map<std::string, Generator> generators;
  };  // end of struct IsotropicHardeningRuleFactory
~~~~

The implementation of the singleton, of the two methods which
manipulate the map and of the constructor is fixed:

~~~~{.cpp}
  IsotropicHardeningRuleFactory& IsotropicHardeningRuleFactory::getFactory() {
    static IsotropicHardeningRuleFactory i;
    return i;
  }  // end of getFactory

  void IsotropicHardeningRuleFactory::addGenerator(const std::string& n,
                                                   const Generator& g) {
    if (!this->generators.insert({n, g}).second) {
      tfel::raise(
          "IsotropicHardeningRuleFactory::addGenerator: "
          "generator '" +
          n + "' already registred");
    }
  }  // end of addGenerator

  std::shared_ptr<IsotropicHardeningRule>
  IsotropicHardeningRuleFactory::generate(const std::string& n) const {
    const auto p = this->generators.find(n);
    if (p == this->generators.end()) {
      tfel::raise(
          "IsotropicHardeningRuleFactory::generate: "
          "no generator named '" +
          n + "'");
    }
    return p->second();
  }  // end of generate

  IsotropicHardeningRuleFactory::IsotropicHardeningRuleFactory() {
    this->addGenerator("Linear", []() {
      return std::make_shared<bbrick::LinearIsotropicHardeningRule>();
    });
    this->addGenerator("Voce", []() {
      return std::make_shared<bbrick::VoceIsotropicHardeningRule>();
    });
    ...
  }  // end of IsotropicHardeningRuleFactory
~~~~

A factory may store more information than a single generator:
`DSLFactory` stores the aliases and the description of each DSL,
`BehaviourBrickFactory` stores the behaviour types and the integration
schemes for which a brick is valid, the interface factories store the
dependencies of an interface with another one. Those additional
properties are stored in maps keyed by the same name.

### Proxies

A factory is fed by proxies, small template structures whose only role
is to build the generator of a given type and to register it:
`DSLProxy`, `BehaviourInterfaceProxy`, `ModelInterfaceProxy`,
`MaterialPropertyInterfaceProxy` and, in the unit tests framework,
`tfel::tests::TestProxy`, which is instantiated by the
`TFEL_TESTS_GENERATE_PROXY` macro.

A proxy is declared in `<Name>Proxy.hxx`, is not exported, since it is
a header only template, and the definitions of its members are written
in a `<Name>Proxy.ixx` file included at the end of the header:

~~~~{.cpp}
  /*!
   * \brief structure used to register a DSL generator
   * \tparam DSLType: type of the DSL generated
   */
  template <typename DSLType>
  struct DSLProxy {
    //! \brief default constructor
    DSLProxy();
    /*!
     * \brief constructor
     * \param[in] aliases: alternative names for the DSL
     */
    explicit DSLProxy(const std::vector<std::string>&);
    /*!
     * \brief generate an instance of the DSL
     * \param[in] opts: options passed to the DSL constructor
     */
    static std::shared_ptr<AbstractDSL> createDSL(
        const AbstractDSL::DSLOptions&);
  };  // end of struct DSLProxy

}  // end of namespace mfront

#include "MFront/DSLProxy.ixx"
~~~~

The constructor of the proxy registers the generator built by the
static member `createDSL`, which is no more than a `std::make_shared`
of the template parameter. The name of the type is given by a static
`getName` method of the registered type itself, and the overloaded
constructors are used to declare the aliases and the dependencies of
this type:

~~~~{.cpp}
  template <typename DSLType>
  DSLProxy<DSLType>::DSLProxy() {
    auto& f = DSLFactory::getDSLFactory();
    f.registerDSLCreator(DSLType::getName(), &DSLProxy<DSLType>::createDSL,
                         &DSLType::getDescription);
  }  // end of DSLProxy

  template <typename DSLType>
  std::shared_ptr<AbstractDSL> DSLProxy<DSLType>::createDSL(
      const AbstractDSL::DSLOptions& opts) {
    const auto& opts_validator = DSLType::getDSLOptionsValidator();
    opts_validator.validate(opts);
    return std::make_shared<DSLType>(opts);
  }  // end of createDSL
~~~~

A proxy is a plain object, which must be constructed for the
registration to take place. Proxies are created as local objects of an
explicit initialisation function, `initDSLs()` or `initInterfaces()`,
which the executables shall call before using the factory and which
shall be idempotent and thread safe:

~~~~{.cpp}
  void initDSLs() {
    static bool init = false;
    ...
    if (init) {
      return;
    }
    if (!init) {
      DSLProxy<DefaultDSL> proxy1(
          std::vector<std::string>{"Default", "DefaultParser"});
      DSLProxy<IsotropicMisesCreepDSL> proxy2("IsotropicMisesCreepParser");
      ...
      init = true;
    }
  }  // end of initDSLs
~~~~

A global proxy is only used in the interface libraries which are not
built in `MFront` and are loaded on demand, since the main program
cannot know the types they declare:

~~~~{.cpp}
  ModelInterfaceProxy<MFrontGerminalModelInterface> ModelGerminalProxy;
~~~~

A proxy is not necessarily one of the templates above: a type
registered by a library external to `MFront` is registered by a local
structure with a `static` instance:

~~~~{.cpp}
  struct MyStressCriterionProxy {
    MyStressCriterionProxy() {
      auto& f = StressCriterionFactory::getFactory();
      f.addGenerator("MyStressCriterion", []() {
        return std::make_shared<MyStressCriterion>();
      });
    }  // end of MyStressCriterionProxy
  };  // end of struct MyStressCriterionProxy

  static MyStressCriterionProxy proxy;
~~~~

In short, adding a new type to an existing hierarchy amounts to:

1. deriving from the abstract type, or from the `Base` class which
   provides the default implementation of its interface,
2. giving the new type a static `getName` method, which is the name
   used by the proxy to register it,
3. registering a generator in the corresponding factory, either from
   the constructor of the factory itself (types which are always
   available), from a proxy created by an initialisation function
   (interfaces and DSLs), or from a local proxy with a `static`
   instance (types of an external library).

# Unit tests

Unit tests are written with the framework of `TFEL/Tests`, which is
documented [here](TFELTests.html). A test file is a `.cxx` file named
after the tested entity, located in the `tests` directory of the
library, and follows this layout:

~~~~{.cpp}
#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <limits>
#include <cstdlib>
#include <iostream>
#include "TFEL/Config/TFELTypes.hxx"
#include "TFEL/Material/Lame.hxx"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

template <typename T>
static constexpr T my_abs(const T& v) noexcept {
  return v < T(0) ? -v : v;
}

struct LameTest final : public tfel::tests::TestCase {
  LameTest()
      : tfel::tests::TestCase("TFEL/Material", "Lame") {}  // end of LameTest
  tfel::tests::TestResult execute() override {
    this->template test1<float, false>();
    this->template test1<double, false>();
    this->template test1<long double, false>();
    return this->result;
  }  // end of execute

 private:
  //! check the computation of the Lame coefficients
  template <typename NumericType, bool use_qt>
  void test1() {
    using stress =
        typename tfel::config::Types<1u, NumericType, use_qt>::stress;
    using real = typename tfel::config::Types<1u, NumericType, use_qt>::real;
    constexpr auto eps = 10 * std::numeric_limits<stress>::epsilon();
    constexpr auto young = stress{150e9};
    constexpr auto nu = real{0.3};
    constexpr auto l = tfel::material::computeLambda(young, nu);
    constexpr auto l_ref = young * nu / ((1 + nu) * (1 - 2 * nu));
    TFEL_TESTS_STATIC_ASSERT(my_abs(l - l_ref) < eps);
  }  // end of test1
};  // end of struct LameTest

TFEL_TESTS_GENERATE_PROXY(LameTest, "Lame");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("Lame.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}  // end of main
~~~~

The points to note are:

- `NDEBUG` is undefined so that `assert` based checks remain active
  in release builds,
- the test type is `final` and derived from `tfel::tests::TestCase`,
  and is registered with `TFEL_TESTS_GENERATE_PROXY`,
- the checks are made with the `TFEL_TESTS_ASSERT`,
  `TFEL_TESTS_CHECK_EQUAL`, `TFEL_TESTS_CHECK_LT`, ... macros of
  `TFEL/Tests/TestCase.hxx`,
- the `/* coverity [UNCAUGHT_EXCEPT]*/` comment before `main`
  discards a false positive of the static analyser.

`TFEL_TESTS_FUNCTION` is a legacy interface and shall not be used in
new tests.

# C sources

`C` sources are limited to the interfaces which must be `C`
compatible: the `GenericBehaviour` interface, the `getFunction`
helpers, the `ProcessManager` helpers.

The same file header, the same include guards and the same
documentation rules apply. The differences are:

- `extern "C"` blocks are opened and closed by commented
  `#ifdef __cplusplus` / `#endif /* __cplusplus */` pairs,
- portability is obtained through `#ifdef __cplusplus` rather than
  by including a standard `C` header directly,
- names are `snake_case` and are prefixed by the name of the
  interface, in lower case, for instance `mfront_gb_State`,
  `mfront_gb_BehaviourData`, `mfront_gb_real`,
  `processManager_wifexited`,
- aggregates are anonymous `typedef struct` types whose members are
  documented by `//!` comments,
- closing comments use the block comment form:
  `} /* end of processManager_wifexited */`.

`format.sh` does not format `.c` and `.h` files: they shall be
formatted as if `clang-format` had been run on them.

# Build system

The conventions of the build system are as important as the
conventions of the sources, since a new file which is not declared is
not a part of the library.

- `CMake` options are lower case, verb first and dash separated:
  `enable-testing`, `enable-static`, `enable-portable-build`,
  `enable-<solver>`.
- `if` and `endif` are always used with their argument:
  `endif(enable-static)`, never `endif()`.
- Sources are first gathered in a `<Target>_SOURCES` variable, then
  passed to `tfel_library`, `mfront_library` or `mtest_library`.
- A new library duplicates its target properties in an
  `if(enable-static)` block for the `-static` variant.
- Headers are installed one by one, never by directory, using
  `install_header`, `install_mfront_header` or `install_mtest_header`.
- A new library declares its `install(TARGETS ... EXPORT ...)` set
  and its `<Library>Config.cmake.in` file, and is added to the
  package config file of the project.
- Tests are built through a macro (`tests_material`, `tests_math`,
  `test_mfront`, ...) which creates an `EXCLUDE_FROM_ALL` executable
  and adds it to the `build-tests` target. Tests are therefore never
  built by a plain build, but by `make build-tests`, which is what
  the continuous integration does.

# Documentation

The website is built from the markdown files of `docs/web` by
`pandoc`. A new page is added by calling `pandoc_html` in
`docs/web/CMakeLists.txt` and by adding an entry in the menu of
`docs/web/mfront-template.html`.

The markdown conventions of the project are:

- a `YAML` header with at least `title`, `author`, `date` and
  `lang`,
- a blank line after every section title,
- no bold markers inside titles,
- code blocks use tildes: `~~~~{.cpp}` ... `~~~~`,
- `TFEL` and `MFront` are written in code font when they refer to the
  software.

The `doxygen` documentation is generated separately from the headers
(see the `doxygen` workflow of the continuous integration).

# Checklist before submitting

Before submitting a pull request, or a patch generated by a coding
agent, check that:

1. the file header of every new file contains the doxygen description
   of the file and the copyright statement,
2. the headers of the file are ordered as specified (standard
   headers, then third-party headers, then `TFEL` headers, then the
   headers of the current library, then the own header),
3. the file is named after the main type it declares,
4. types are `struct`, public members are declared first,
5. data members and variables are in `snake_case`, methods and free
   functions are in `camelCase`,
6. data members and methods are qualified by `this->`,
7. every member, function and method is documented, in the header,
8. namespaces, types, functions and enumerations are closed by a
   `// end of ...` comment,
9. `./format.sh` has been run,
10. new sources and new tests are declared in the `CMakeLists.txt`
    files,
11. `./check_all.sh -f` passes,
12. new features are described in the relevant release notes file of
    `docs/web/release-notes`.
