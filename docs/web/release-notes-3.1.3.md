% Release notes of the 3.1.3 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2018

This is mainly a bug fix version of the `3.1` series. All tickets
solved are described below.

# Tickets fixed

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
