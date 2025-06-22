!> \file   bindings/fortran90/TFELPHYSICALCONSTANTS.mod
!> \brief  physical constants from CODATA Internationally recommended values,
!           http://physics.nist.gov/cuu/Constants/index.html
!> \author Thomas Helfer
!> \date   18 août 2017
!> \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights 
!  reserved. 
!  This project is publicly released under either the GNU GPL Licence with linking exception 
!  or the CECILL-A licence. A copy of thoses licences are delivered 
!  with the sources of TFEL. CEA or EDF may also distribute this 
!  project under specific licensing conditions.

module TFEL_PHYSICAL_CONSTANTS
  use,intrinsic :: ISO_FORTRAN_ENV
  implicit none
  !> Atomic mass constant (kg)
  real (real64), parameter :: ATOMICMASSCONSTANT = 1.6605390399999999D-27
  !> Atomic mass constant (kg)
  real (real64), parameter :: MU = 1.6605390399999999D-27
  !> Avogadro constant (mol-1)
  real (real64), parameter :: AVOGADROCONSTANT = 6.0221408570000002D+23
  !> Avogadro constant (mol-1)
  real (real64), parameter :: NA = 6.0221408570000002D+23
  !> Boltzmann constant (J.K-1)
  real (real64), parameter :: BOLTZMANNCONSTANT = 1.3806485199999999D-23
  !> Boltzmann constant (J.K-1)
  real (real64), parameter :: KB = 1.3806485199999999D-23
  !> Conductance quantum (S)
  real (real64), parameter :: CONDUCTANCEQUANTUM = 7.7480917309999997D-05
  !> Conductance quantum (S)
  real (real64), parameter :: G0 = 7.7480917309999997D-05
  !> Electric constant (F.m-1)
  real (real64), parameter :: ELECTRICCONSTANT = 8.8541878170000005D-12
  !> Electric constant (F.m-1)
  real (real64), parameter :: E0 = 8.8541878170000005D-12
  !> Electron mass (kg)
  real (real64), parameter :: ELECTRONMASS = 9.1093835599999998D-31
  !> Electron mass (kg)
  real (real64), parameter :: ME = 9.1093835599999998D-31
  !> Electron volt (J)
  real (real64), parameter :: ELECTRONVOLT = 1.6021766208000001D-19
  !> Electron volt (J)
  real (real64), parameter :: EV = 1.6021766208000001D-19
  !> Elementary charge (C)
  real (real64), parameter :: ELEMENTARYCHARGE = 1.6021766208000001D-19
  !> Elementary charge (C)
  real (real64), parameter :: E = 1.6021766208000001D-19
  !> Faraday constant (C.mol-1)
  real (real64), parameter :: FARADAYCONSTANT = 96485.332890000005D0
  !> Faraday constant (C.mol-1)
  real (real64), parameter :: F = 96485.332890000005D0
  !> Fine-structure constant
  real (real64), parameter :: FINESTRUCTURECONSTANT = 0.0072973525663999998D0
  !> Fine-structure constant
  real (real64), parameter :: A = 0.0072973525663999998D0
  !> Molar gas constant (J.mol-1.K-1)
  real (real64), parameter :: MOLARGASCONSTANT = 8.3144597999999998D0
  !> Molar gas constant (J.mol-1.K-1)
  real (real64), parameter :: R = 8.3144597999999998D0
  !> Stefan-Boltzmann constant (W.m-2.K-4)
  real (real64), parameter :: STEFANBOLTZMANNCONSTANT = 5.6703669999999997D-08
  !> Stefan-Boltzmann constant (W.m-2.K-4)
  real (real64), parameter :: S = 5.6703669999999997D-08
endmodule TFEL_PHYSICAL_CONSTANTS
