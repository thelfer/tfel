% Release notes of the 3.3.1 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2020

Version `3.3.1` is mainly a bug fix version of the `3.3` series.

# Tickets fixed

## Ticket #227: Symbolic names are not declared for the tangent operator blocks associated with the derivatives with respect to external state variables

For more details, see: <https://sourceforge.net/p/tfel/tickets/227/>

## Ticket #221: `mfront-query --tangent-operator-blocks` shall return the symbolic name of the tangent operator blocks

For more details, see: <https://sourceforge.net/p/tfel/tickets/221/>

## Ticket #220: Declaring additional tangent operator blocks using symbolic names is broken

For more details, see: <https://sourceforge.net/p/tfel/tickets/220/>

## Ticket #218: The generation of the copy constructor of the `BehaviourData` class is broken when several gradients are defined

For more details, see: <https://sourceforge.net/p/tfel/tickets/218/>

## Ticket #217: Better support of several symmetric tensor gradients

The tangent operator blocks was not defined when several symmetric
tensor gradients were defined.

For more details, see: <https://sourceforge.net/p/tfel/tickets/217/>

## Ticket #216: Computation of the consistent tangent operator in the `mtest::GenericBehaviour` class is broken when several gradients are declared

For more details, see: <https://sourceforge.net/p/tfel/tickets/216/>

## Ticket #215: UTF-8 representations of the tangent operator blocks are not declared

The tangent operator blocks now have proper symbolic representations.

For more details, see: <https://sourceforge.net/p/tfel/tickets/215/>

## Ticket #214: Strings shall not be mangled

Unicode support is based on a mangling used to convert UTF-8 symbols
into ANSYS characters. Strings are no more mangled.

For more details, see: <https://sourceforge.net/p/tfel/tickets/214/>

## Ticket #211: Visual Studio Build Error for DIANAFEA

For more details, see: <https://sourceforge.net/p/tfel/tickets/211/>

## Ticket #198: The `mfront/src/MohrCoulombStressCriterion.cxx` file does not compile in `C++-17`

For more details, see: <https://sourceforge.net/p/tfel/tickets/198/>

