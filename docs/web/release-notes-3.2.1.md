% Release notes of the 3.2.1 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2018

Version `3.2.1` is mainly a bug fix version of the `3.2` series.

It has been released simultaneously with Versions
[`3.0.4`](release-notes-3.0.4.html) and
[`3.1.4`](release-notes-3.1.4.html) and inherits from the improvements
made in those versions (See the associated release notes for a complete
description).

The first section announces releases of projects strongly linked with
`TFEL/MFront`. The solved issues described in Section "Tickets fixed" at
the end of this document are relative to the `3.2` branch.

Version `3.2.1` can be downloaded:

- on `SourceForge`: <https://sourceforge.net/projects/tfel/files/>
- on `github`: <https://github.com/thelfer/tfel>

A `Spack` recipe is also available (see
<https://spack.readthedocs.io/en/latest/>).

# Associated projects release

The following associated projects have been released simultaneously:

- Version `1.0.1` of the `MGIS` project (MFront Generic Interface
  Support). This project aims at providing tools (functions, classes,
  bindings, etc...) to handle behaviours written using the `MFront`
  generic interface. Those tools are meant to be used by (FEM, FFT,
  etc.) solver developers. Though quite recent, this project already has
  been integrated in various projects developed internally at `CEA` and
  also in some noteworthy external projects, including the `XPer` solver
  (see @perales_xper_2017) and the
  [`OpenGeoSys`](https://www.opengeosys.org/) platform
 (see @kolditz_opengeosys:_2012). See
  <https://github.com/thelfer/MFrontGenericInterfaceSupport> for
  details.
- Version `1.0.1` of the `MFrontGallery` project which aims at gathering
  high quality, thoroughly tested, well written, efficient and
  ready-to-use `MFront` implementations of various material properties,
  behaviours and models. See <https://github.com/thelfer/MFrontGallery>
  for details.
- Version `1.0.1` of the `tfel-editor` project which aims to provide an
  `EMACS`-like text editor with major modes dedicated to `MFront` and
  `MTest` file editing (including syntax highlighting and keyword
  completion). See <https://github.com/thelfer/tfel-editor> for details.
- Version `1.0.1` of the `tfel-plot` project. See
  <https://github.com/thelfer/tfel-plot> for details.

# Tickets fixed

## Ticket #166: The offset in material properties associated with the rotation matrix is not taken into account propertly in the `Ansys` interface

For more details, see: <https://sourceforge.net/p/tfel/tickets/166/>

## Ticket #165: Integration failure is not always properly handled in the `Ansys` interface

For more details, see: <https://sourceforge.net/p/tfel/tickets/165/>

## Ticket #164: Prediction is not handled properly in the generic interface

There was a error in the generated code, so the type of computation was
not handled properly:

~~~~
const auto bp = K[0]<0.5;
~~~~

shall be (as documented):

~~~~
const auto bp = K[0]<-0.5;
~~~~

For more details, see: <https://sourceforge.net/p/tfel/tickets/164/>

## Ticket #161: Buffer overflow in initial stress computation in the `generic` interface when using a strain measure

For more details, see: <https://sourceforge.net/p/tfel/tickets/161/>

## Ticket #153: `SignalManager.cxx` does not compile under `Mac OS`

The issue is related to the fact that `sigfillset` is a macro under `Mac OS`.

For more details, see: <https://sourceforge.net/p/tfel/tickets/153/>

## Ticket #150: Option specification for additonal keywords or variable substitution is inconsistent under Windows

Options under Windows are allowed to be specified with `'/'` in place
of `'--'` (both specifications are accepted). However, this did not
work for additonal keywords or variable substitution.

The following example failed:

~~~~
$ mfront /@CompareToNumericalJacobian=true ...
~~~~

For more details, see: <https://sourceforge.net/p/tfel/tickets/150/>

## Ticket #149: Ability to specify the path the python library at runtime

The following environment variable can now be used to customize how `python` modules for material properties are build:

- `PYTHON_INCLUDE_PATH`: path to the `python` header files.
- `PYTHON_LIBRARY_PATH`: path to the `python` library.
- `PYTHON_LIBRARY`: name of the `python` library.

For more details, see: <https://sourceforge.net/p/tfel/tickets/149/>

## Ticket #142: get energies (elastic, plastic...) in the `generic` interface

Two outputs have been added in the `MFront_GB_State` structure:

- the stored energy, as computed by the `@InternalEnergy` keyword
- the dissipated energy, as computed by the `@DissipatedEnergy` keyword

The `MGIS` project has been updated to match this evolution.

For more details, see: <https://sourceforge.net/p/tfel/tickets/142/>

## Ticket #141: Output `SSE`, `SPD` and `SCD` in `Abaqus/Standard`

The `Abaqus/Standard` solver defines three optional outputs:

- `SSE`: the elastic energy.
- `SPD`: the energy dissipated by plasticity.
- `SCD`: the energy dissipated by viscoplasticity.

On the `MFront` side, the behaviour can optionally compute the stored
and dissipated energies through the `@InternalEnergy` and
`@DissipatedEnergy` keywords. This means that `MFront` behaviours does
not distinguish the energies dissipated by plasticity and
viscoplasticity. The choice made in version `3.2.1` is to store the
energy computed by the `@InternalEnergy` keyword in `SSE` and to store
the energy computed by the `@DissipatedEnergy` keyword in `SPD`.

For more details, see: <https://sourceforge.net/p/tfel/tickets/141/>

# References

