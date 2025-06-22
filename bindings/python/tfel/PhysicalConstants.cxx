/*!
 * \file   bindings/python/tfel/PhysicalConstants.cxx
 * \brief
 * \author Thomas Helfer
 * \date   18 août 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Config/TFELConfig.hxx"

void declarePhysicalConstants(pybind11::module_& m);

struct TFEL_VISIBILITY_LOCAL PhysicalConstants {
  //! Atomic mass constant (kg)
  static const double AtomicMassConstant;
  //! Avogadro constant (mol-1)
  static const double AvogadroConstant;
  //! Boltzmann constant (J.K-1)
  static const double BoltzmannConstant;
  //! Conductance quantum (S)
  static const double ConductanceQuantum;
  //! Electric constant (F.m-1)
  static const double ElectricConstant;
  //! Electron mass (kg)
  static const double ElectronMass;
  //! Electron volt (J)
  static const double ElectronVolt;
  //! Elementary charge (C)
  static const double ElementaryCharge;
  //! Faraday constant (C.mol-1)
  static const double FaradayConstant;
  //! Fine-structure constant
  static const double FineStructureConstant;
  //! Molar gas constant (J.mol-1.K-1)
  static const double MolarGasConstant;
  //! Stefan-Boltzmann constant (W.m-2.K-4)
  static const double StefanBoltzmannConstant;
};

const double PhysicalConstants::AtomicMassConstant = double(1.66054e-27);
const double PhysicalConstants::AvogadroConstant = double(6.02214e+23);
const double PhysicalConstants::BoltzmannConstant = double(1.38065e-23);
const double PhysicalConstants::ConductanceQuantum = double(7.74809e-05);
const double PhysicalConstants::ElectricConstant = double(8.85419e-12);
const double PhysicalConstants::ElectronMass = double(9.10938e-31);
const double PhysicalConstants::ElectronVolt = double(1.60218e-19);
const double PhysicalConstants::ElementaryCharge = double(1.60218e-19);
const double PhysicalConstants::FaradayConstant = double(96485.3);
const double PhysicalConstants::FineStructureConstant = double(0.00729735);
const double PhysicalConstants::MolarGasConstant = double(8.31446);
const double PhysicalConstants::StefanBoltzmannConstant = double(5.67037e-08);

void declarePhysicalConstants(pybind11::module_& m) {
  pybind11::class_<PhysicalConstants>(m, "PhysicalConstants")
      .def_readonly_static("AtomicMassConstant",
                           &PhysicalConstants::AtomicMassConstant)
      .def_readonly_static("mu", &PhysicalConstants::AtomicMassConstant)
      .def_readonly_static("AvogadroConstant",
                           &PhysicalConstants::AvogadroConstant)
      .def_readonly_static("Na", &PhysicalConstants::AvogadroConstant)
      .def_readonly_static("BoltzmannConstant",
                           &PhysicalConstants::BoltzmannConstant)
      .def_readonly_static("kb", &PhysicalConstants::BoltzmannConstant)
      .def_readonly_static("ConductanceQuantum",
                           &PhysicalConstants::ConductanceQuantum)
      .def_readonly_static("G0", &PhysicalConstants::ConductanceQuantum)
      .def_readonly_static("ElectricConstant",
                           &PhysicalConstants::ElectricConstant)
      .def_readonly_static("e0", &PhysicalConstants::ElectricConstant)
      .def_readonly_static("ElectronMass", &PhysicalConstants::ElectronMass)
      .def_readonly_static("me", &PhysicalConstants::ElectronMass)
      .def_readonly_static("ElectronVolt", &PhysicalConstants::ElectronVolt)
      .def_readonly_static("eV", &PhysicalConstants::ElectronVolt)
      .def_readonly_static("ElementaryCharge",
                           &PhysicalConstants::ElementaryCharge)
      .def_readonly_static("e", &PhysicalConstants::ElementaryCharge)
      .def_readonly_static("FaradayConstant",
                           &PhysicalConstants::FaradayConstant)
      .def_readonly_static("F", &PhysicalConstants::FaradayConstant)
      .def_readonly_static("FineStructureConstant",
                           &PhysicalConstants::FineStructureConstant)
      .def_readonly_static("a", &PhysicalConstants::FineStructureConstant)
      .def_readonly_static("MolarGasConstant",
                           &PhysicalConstants::MolarGasConstant)
      .def_readonly_static("R", &PhysicalConstants::MolarGasConstant)
      .def_readonly_static("StefanBoltzmannConstant",
                           &PhysicalConstants::StefanBoltzmannConstant)
      .def_readonly_static("s", &PhysicalConstants::StefanBoltzmannConstant);

}  // end of declarePhysicalConstants
