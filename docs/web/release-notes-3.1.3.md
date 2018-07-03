% Release notes of the 3.1.3 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2018

This is mainly a bug fix version of the `3.1` series. All tickets
solved are described below.

# Tickets fixed

## Ticket #139: Invalid system flags when using the official `castem.h` header for `Cast3M 2018`

The `castem.h` header relies on the definition of some system flags to
work appropriately (in particular to define the `cast_int` typedef).
This file changed in `Cast3M 2018`. The system flags defined by
`TFEL/MFront` (`-DSUN -DUNIX64` or `-DSUN -DUNIX32`) were no more
appropriate under `Linux` and had to be changed to `-DLINUX64` or
`-DLINUX32`.

For more details, see: <https://sourceforge.net/p/tfel/tickets/139/>

## Ticket #138: Profiling is broken when one of the following code blocks is defined: `@APrioriTimeStepScalingFactor`, `@APosterioriTimeStepScalingFactor`, `@AdditionalConvergenceChecks`

For more details, see: <https://sourceforge.net/p/tfel/tickets/138/>

## Ticket #136: Removing unwanted output statements in windows

The integration errors are reported on the standard output, since the
`Cast3M` interface does not have any output for error messages.

This can be cumbersome, in particular when driving the simulation from
the behaviour.

The `Cast3M` interface now checks if the `CASTEM_DISPLAY_ERROR_MESSAGE`
variable environment is defined. If it is set to any value different
from `true`, no error message is displayed.

In `bash`:

~~~~
export CASTEM_DISPLAY_ERROR_MESSAGE=true
~~~~

In the `Windows` shell:

~~~~
set CASTEM_DISPLAY_ERROR_MESSAGE=true
~~~~

For more details, see: <https://sourceforge.net/p/tfel/tickets/136/>

## Ticket #135: Declaration error of 'material_properties_nb' in `CyranoBehaviourHandler` with the `cyrano` interface

The default version of the `CyranoTraits` class did not provide the
`material_properties_nb` and `internal_state_variables_nb` members.

As a consequence, the trouble appeared when the behaviour did not
support the `AxisymmtricalGeneralisedPlaneStress` modelling
hypotheses, which is the main modelling hypothesis used in
`Cyrano`. This case was not tested.

For more details, see: <https://sourceforge.net/p/tfel/tickets/135/>

## Ticket #134: The `MFRONT_COMPILING` flag is not defined when using the `CMake` generator

This bug leads to the failure of the compilation of `MFront` files based
on other `MFront` files (such as material properties) on `Windows` with
the `Visual` `C++` compiler.

For more details, see: <https://sourceforge.net/p/tfel/tickets/134/>
