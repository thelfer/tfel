% Release notes of the 3.0.2 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2017

# Tickets fixed

## Ticket #64: Change the library prefix to "cyg" under cygwin

The default library prefix under cygwin has been changed to `cyg`
(rather than `lib`) to follow the standard conventions of this
environment.

For more details, see: <https://sourceforge.net/p/tfel/tickets/84/>

## Ticket #63: Error cygwin (make)

Compiling `MFront` under `Cygwin` was broken because the `_GNU_SOURCE`
macro was not defined in the following headers:

- `mfront\tests\properties\test-cxx.cxx`
- `src\System\ProcessManager.cxx`
- `src\System\SignalManager.cxx`

For more details, see: <https://sourceforge.net/p/tfel/tickets/83/>

## Ticket #58: Fix initialisation of an array of parameters

For more details, see: <https://sourceforge.net/p/tfel/tickets/58/>

## Ticket #57: More robust detection of python versions: avoid mismatch between the python libraries, the python interpreter and the boost python library

For more details, see: <https://sourceforge.net/p/tfel/tickets/57/>

## Ticket #54: Bug in StensorFromTinyVectorView: standard assignement fails

For more details, see: <https://sourceforge.net/p/tfel/tickets/54/>

## Ticket #53: Add comments handling in parameters txt files

For more details, see: <https://sourceforge.net/p/tfel/tickets/53/>

## Ticket #52: More robust handling of parameters txt file

For more details, see: <https://sourceforge.net/p/tfel/tickets/52/>

<!-- Local IspellDict: english -->
