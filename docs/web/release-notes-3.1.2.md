% Release notes of the 3.1.2 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2018

This is mainly a bug fix version of the `3.1` series. All tickets
solved are described below.

# Tickets fixed

## Ticket #123: Implementation mistakes in various behaviours based on Implicit DSL and using a numerical jacobian (use of perturbated values to update auxiliary state variables)

Various behaviours in the MFront' tests base used local variables to
compute the increment of auxiliary state variables (in order to safe
some computational times and reduce the implementation size). This trick
is quite often used with analytical jacobian but can cause various
problems with numerical jacobian if the consistent tangent operator is
requested (in this case, the system is perturbated once after
convergence to get the upper part of the inverse of the jacobian, as
needed by the `getPartialJacobianInvert` method).

The issue can be circumvented using the `perturbatedSystemEvaluation`
flag introduced in `TFEL` `3.1.1` (see
<http://tfel.sourceforge.net/release-notes-3.1.1.html> and
<https://sourceforge.net/p/tfel/tickets/111/> for details ).

The concerned implementations were the following:

- `mfront/tests/behaviours/MonoCrystal.mfront`
- `mfront/tests/behaviours/MonoCrystalNewtonRaphson.mfront`
- `mfront/tests/behaviours/MonoCrystal_DD_CC_Irradiation_NumericalJacobian.mfront`
- `mfront/tests/behaviours/MonoCrystal_DD_CFC_Irradiation.mfront`
- `mfront/tests/behaviours/MonoCrystal_DD_CFC_NumericalJacobian.mfront`

For more details, see: <https://sourceforge.net/p/tfel/tickets/123/>

## Ticket #122: Missing file in the distributed tar ball (`mfront-query/include/CMakeLists`)

The file `mfront-query/include/CMakeLists.txt` was not included in the distributed tar ball.
This led to a warning when compiling with `cmake`, but it did not hinder the compilation.

For more details, see: <https://sourceforge.net/p/tfel/tickets/122/>

## Ticket #121: Modifying the reference values read in a file does not work in MTest

In `MTest`, the following lines was not parsed appropriately:

~~~~{.cpp}
@Test<file> @reference_file@ {
  'SXX' : '$8*1.e6'
} 1.e3;
~~~~

For more details, see: <https://sourceforge.net/p/tfel/tickets/122/>

## Ticket #120: Direct call to Cast3M' umat behaviours in `MTest` is broken

Since `TFEL` `3.1`, `MTest` is able to call raw Cast3M' umat behaviours
(not written using `MFront`). However, the handling of external state
variables was broken.

For more details, see: <https://sourceforge.net/p/tfel/tickets/120/>

## Ticket #119:	Material laws are not declared in the `@AdditionalConvergenceChecks` code block

In the generation of the `additionalConvergenceChecks` method, the
material laws were not declared.

For more details, see: <https://sourceforge.net/p/tfel/tickets/119/>

## Ticket #118: The `BehaviourDescription::areMaterialPropertiesDependantOnStateVariables` method always returns `true`

The
`BehaviourDescription::areMaterialPropertiesDependantOnStateVariables`
method always returned `true`. This bugs led to unnecessary computations
but did not affect the results.

For more details, see: <https://sourceforge.net/p/tfel/tickets/118/>

## Ticket #117: Generated source fails to compile if one ask MTest file generation with array of internal state variables

A `using namespace std;` statement has been removed in 3.1.0 in the generated code, which led to a compile time error (`ostringstream` undeclared) when:

- generating a `MTest` file on failure
- there is an array of internal state variables (this case was not tested)

A new test case has been introduced for testing all the cases.

For more details, see: <https://sourceforge.net/p/tfel/tickets/117/>

## Ticket #115: The stress free expansion handler shall be a `nullptr` for true finite strain behaviours

For more details, see: <https://sourceforge.net/p/tfel/tickets/115/>
