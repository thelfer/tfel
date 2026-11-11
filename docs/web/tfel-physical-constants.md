---
title: The `TFEL/PhysicalConstants` library
author: Thomas Helfer
date: 17/08/2021
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

# Description of the library

The `TFEL/PhysicalConstants` library is header-only which provides:

- A template class called `PhysicalConstants` in the namespace `tfel`. This class defined a constexpr static data member per physical constant. This class has two template parameters:
    - `NumericType`: which is the floatting point number type
      to be used to define the physical constants.
    - `use_quantities`: which states if quantities (i.e. numbers
       with units) shall be used.
      to be used to define the physical constants.
- A set of template constexpr inline variables defined in the
  namespace `tfel::constants`. Those variables have the same
  template parameters than the `PhysicalConstants class`

The values of the physical constants are extracted from: <http://physics.nist.gov/cuu/Constants/index.html>

The values of the physical constants depend on the compatibility
modes: those of the current version of `TFEL` are those exposed by
the names without suffix, whereas those of the next major version
are those of the `tfel::future` and `tfel::constants::future`
namespaces, as described in the page devoted to
[compatibility modes in the `TFEL` libraries](tfel-compatibility-modes.html).

## Bindings

### `Fortran` bindings

A include file named `TFELPHYSICALCONSTANTS.INC` is provided for `fortran77`. This include declares the physical constants as `real64` parameters.

A module named `TFEL_PHYSICAL_CONSTANTS` is provided for `fortran95`. This module exposes the physical constants as `real64` parameters.

The `Fortran` bindings are not versioned: they expose the values of
the next major version of `TFEL`.

### `Python` bindings

The `tfel` module exposes the class `PhysicalConstants` which declares the physical constants as read-only attributes. The constants of a given compatibility mode are exposed by a submodule of the same name.

# Physical constants

- AtomicMassConstant (mu, \(kg\)): Atomic mass constant
- AvogadroConstant (Na, \(mol^{-1}\)): Avogadro constant
- BoltzmannConstant (kb, \(J.K^{-1}\)): Boltzmann constant
- ConductanceQuantum (G0, \(S\)): Conductance quantum
- ElectricConstant (e0, \(F.m^{-1}\)): Electric constant
- ElectronMass (me, \(kg\)): Electron mass
- ElectronVolt (eV, \(J\)): Electron volt
- ElementaryCharge (e, \(C\)): Elementary charge
- FaradayConstant (F, \(C.mol^{-1}\)): Faraday constant
- FineStructureConstant (a): Fine-structure constant
- MolarGasConstant (R, \(J.mol^{-1}.K^{-1}\)): Molar gas constant
- StefanBoltzmannConstant (s, \(W.m^{-2}.K^{-4}\)): Stefan-Boltzmann constant
