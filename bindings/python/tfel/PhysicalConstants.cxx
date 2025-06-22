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

#include <boost/python.hpp>
#include "TFEL/Config/TFELConfig.hxx"

void declarePhysicalConstants();

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

void declarePhysicalConstants() {
  boost::python::class_<PhysicalConstants>("PhysicalConstants")
      .def_readonly("AtomicMassConstant", PhysicalConstants::AtomicMassConstant,
                    "Atomic mass constant (kg)")
      .def_readonly("mu", PhysicalConstants::AtomicMassConstant,
                    "Atomic mass constant (kg)")
      .def_readonly("AvogadroConstant", PhysicalConstants::AvogadroConstant,
                    "Avogadro constant (mol-1)")
      .def_readonly("Na", PhysicalConstants::AvogadroConstant,
                    "Avogadro constant (mol-1)")
      .def_readonly("BoltzmannConstant", PhysicalConstants::BoltzmannConstant,
                    "Boltzmann constant (J.K-1)")
      .def_readonly("kb", PhysicalConstants::BoltzmannConstant,
                    "Boltzmann constant (J.K-1)")
      .def_readonly("ConductanceQuantum", PhysicalConstants::ConductanceQuantum,
                    "Conductance quantum (S)")
      .def_readonly("G0", PhysicalConstants::ConductanceQuantum,
                    "Conductance quantum (S)")
      .def_readonly("ElectricConstant", PhysicalConstants::ElectricConstant,
                    "Electric constant (F.m-1)")
      .def_readonly("e0", PhysicalConstants::ElectricConstant,
                    "Electric constant (F.m-1)")
      .def_readonly("ElectronMass", PhysicalConstants::ElectronMass,
                    "Electron mass (kg)")
      .def_readonly("me", PhysicalConstants::ElectronMass, "Electron mass (kg)")
      .def_readonly("ElectronVolt", PhysicalConstants::ElectronVolt,
                    "Electron volt (J)")
      .def_readonly("eV", PhysicalConstants::ElectronVolt, "Electron volt (J)")
      .def_readonly("ElementaryCharge", PhysicalConstants::ElementaryCharge,
                    "Elementary charge (C)")
      .def_readonly("e", PhysicalConstants::ElementaryCharge,
                    "Elementary charge (C)")
      .def_readonly("FaradayConstant", PhysicalConstants::FaradayConstant,
                    "Faraday constant (C.mol-1)")
      .def_readonly("F", PhysicalConstants::FaradayConstant,
                    "Faraday constant (C.mol-1)")
      .def_readonly("FineStructureConstant",
                    PhysicalConstants::FineStructureConstant,
                    "Fine-structure constant")
      .def_readonly("a", PhysicalConstants::FineStructureConstant,
                    "Fine-structure constant")
      .def_readonly("MolarGasConstant", PhysicalConstants::MolarGasConstant,
                    "Molar gas constant (J.mol-1.K-1)")
      .def_readonly("R", PhysicalConstants::MolarGasConstant,
                    "Molar gas constant (J.mol-1.K-1)")
      .def_readonly("StefanBoltzmannConstant",
                    PhysicalConstants::StefanBoltzmannConstant,
                    "Stefan-Boltzmann constant (W.m-2.K-4)")
      .def_readonly("s", PhysicalConstants::StefanBoltzmannConstant,
                    "Stefan-Boltzmann constant (W.m-2.K-4)");

}  // end of declarePhysicalConstants
