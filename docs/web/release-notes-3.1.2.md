% Release notes of the 3.1.2 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2018

This is mainly a bug fix version of the `3.1` series. All tickets
solved are described below.

# Tickets fixed

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
