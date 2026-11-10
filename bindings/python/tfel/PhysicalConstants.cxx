/*!
 * \file   bindings/python/tfel/PhysicalConstants.cxx
 * \brief
 * \author Thomas Helfer
 * \date 18/08/2017
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

#ifndef TFEL_VERSION_MAJOR
#error "TFEL_VERSION_MAJOR must be defined"
#endif

void declarePhysicalConstants(pybind11::module_& m);

struct TFEL_VISIBILITY_LOCAL PhysicalConstants_v5_2_0 {
  static const double AtomicMassConstant;
  static const double AvogadroConstant;
  static const double BoltzmannConstant;
  static const double ConductanceQuantum;
  static const double ElectricConstant;
  static const double ElectronMass;
  static const double ElectronVolt;
  static const double ElementaryCharge;
  static const double FaradayConstant;
  static const double FineStructureConstant;
  static const double MolarGasConstant;
  static const double StefanBoltzmannConstant;
};

const double PhysicalConstants_v5_2_0::AtomicMassConstant = double(1.66054e-27);
const double PhysicalConstants_v5_2_0::AvogadroConstant = double(6.02214e+23);
const double PhysicalConstants_v5_2_0::BoltzmannConstant = double(1.38065e-23);
const double PhysicalConstants_v5_2_0::ConductanceQuantum = double(7.74809e-05);
const double PhysicalConstants_v5_2_0::ElectricConstant = double(8.85419e-12);
const double PhysicalConstants_v5_2_0::ElectronMass = double(9.10938e-31);
const double PhysicalConstants_v5_2_0::ElectronVolt = double(1.60218e-19);
const double PhysicalConstants_v5_2_0::ElementaryCharge = double(1.60218e-19);
const double PhysicalConstants_v5_2_0::FaradayConstant = double(96485.3);
const double PhysicalConstants_v5_2_0::FineStructureConstant =
    double(0.00729735);
const double PhysicalConstants_v5_2_0::MolarGasConstant = double(8.31446);
const double PhysicalConstants_v5_2_0::StefanBoltzmannConstant =
    double(5.67037e-08);

struct TFEL_VISIBILITY_LOCAL PhysicalConstants_v6_0_0 {
  static const double AtomicMassConstant;
  static const double AvogadroConstant;
  static const double BoltzmannConstant;
  static const double ConductanceQuantum;
  static const double ElectricConstant;
  static const double ElectronMass;
  static const double ElectronVolt;
  static const double ElementaryCharge;
  static const double FaradayConstant;
  static const double FineStructureConstant;
  static const double MolarGasConstant;
  static const double StefanBoltzmannConstant;
};

const double PhysicalConstants_v6_0_0::AtomicMassConstant =
    double(1.66053904e-27);
const double PhysicalConstants_v6_0_0::AvogadroConstant =
    double(6.022140857e+23);
const double PhysicalConstants_v6_0_0::BoltzmannConstant =
    double(1.38064852e-23);
const double PhysicalConstants_v6_0_0::ConductanceQuantum =
    double(7.748091731e-05);
const double PhysicalConstants_v6_0_0::ElectricConstant =
    double(8.854187817e-12);
const double PhysicalConstants_v6_0_0::ElectronMass = double(9.10938356e-31);
const double PhysicalConstants_v6_0_0::ElectronVolt = double(1.6021766208e-19);
const double PhysicalConstants_v6_0_0::ElementaryCharge =
    double(1.6021766208e-19);
const double PhysicalConstants_v6_0_0::FaradayConstant = double(96485.33289);
const double PhysicalConstants_v6_0_0::FineStructureConstant =
    double(0.0072973525664);
const double PhysicalConstants_v6_0_0::MolarGasConstant = double(8.3144598);
const double PhysicalConstants_v6_0_0::StefanBoltzmannConstant =
    double(5.670367e-08);

static void declarePhysicalConstants_v5_2_0(pybind11::module_& m) {
  pybind11::class_<PhysicalConstants_v5_2_0> w(m, "PhysicalConstants",
                                               "Physical constants");
  w.def_readonly_static("AtomicMassConstant",
                        &PhysicalConstants_v5_2_0::AtomicMassConstant)
      .def_readonly_static("mu", &PhysicalConstants_v5_2_0::AtomicMassConstant)
      .def_readonly_static("AvogadroConstant",
                           &PhysicalConstants_v5_2_0::AvogadroConstant)
      .def_readonly_static("Na", &PhysicalConstants_v5_2_0::AvogadroConstant)
      .def_readonly_static("BoltzmannConstant",
                           &PhysicalConstants_v5_2_0::BoltzmannConstant)
      .def_readonly_static("kb", &PhysicalConstants_v5_2_0::BoltzmannConstant)
      .def_readonly_static("ConductanceQuantum",
                           &PhysicalConstants_v5_2_0::ConductanceQuantum)
      .def_readonly_static("G0", &PhysicalConstants_v5_2_0::ConductanceQuantum)
      .def_readonly_static("ElectricConstant",
                           &PhysicalConstants_v5_2_0::ElectricConstant)
      .def_readonly_static("e0", &PhysicalConstants_v5_2_0::ElectricConstant)
      .def_readonly_static("ElectronMass",
                           &PhysicalConstants_v5_2_0::ElectronMass)
      .def_readonly_static("me", &PhysicalConstants_v5_2_0::ElectronMass)
      .def_readonly_static("ElectronVolt",
                           &PhysicalConstants_v5_2_0::ElectronVolt)
      .def_readonly_static("eV", &PhysicalConstants_v5_2_0::ElectronVolt)
      .def_readonly_static("ElementaryCharge",
                           &PhysicalConstants_v5_2_0::ElementaryCharge)
      .def_readonly_static("e", &PhysicalConstants_v5_2_0::ElementaryCharge)
      .def_readonly_static("FaradayConstant",
                           &PhysicalConstants_v5_2_0::FaradayConstant)
      .def_readonly_static("F", &PhysicalConstants_v5_2_0::FaradayConstant)
      .def_readonly_static("FineStructureConstant",
                           &PhysicalConstants_v5_2_0::FineStructureConstant)
      .def_readonly_static("a",
                           &PhysicalConstants_v5_2_0::FineStructureConstant)
      .def_readonly_static("MolarGasConstant",
                           &PhysicalConstants_v5_2_0::MolarGasConstant)
      .def_readonly_static("R", &PhysicalConstants_v5_2_0::MolarGasConstant)
      .def_readonly_static("StefanBoltzmannConstant",
                           &PhysicalConstants_v5_2_0::StefanBoltzmannConstant)
      .def_readonly_static("s",
                           &PhysicalConstants_v5_2_0::StefanBoltzmannConstant);
}

static void declarePhysicalConstants_v6_0_0(pybind11::module_& m) {
  pybind11::class_<PhysicalConstants_v6_0_0> w(m, "PhysicalConstants",
                                               "Physical constants");
  w.def_readonly_static("AtomicMassConstant",
                        &PhysicalConstants_v6_0_0::AtomicMassConstant)
      .def_readonly_static("mu", &PhysicalConstants_v6_0_0::AtomicMassConstant)
      .def_readonly_static("AvogadroConstant",
                           &PhysicalConstants_v6_0_0::AvogadroConstant)
      .def_readonly_static("Na", &PhysicalConstants_v6_0_0::AvogadroConstant)
      .def_readonly_static("BoltzmannConstant",
                           &PhysicalConstants_v6_0_0::BoltzmannConstant)
      .def_readonly_static("kb", &PhysicalConstants_v6_0_0::BoltzmannConstant)
      .def_readonly_static("ConductanceQuantum",
                           &PhysicalConstants_v6_0_0::ConductanceQuantum)
      .def_readonly_static("G0", &PhysicalConstants_v6_0_0::ConductanceQuantum)
      .def_readonly_static("ElectricConstant",
                           &PhysicalConstants_v6_0_0::ElectricConstant)
      .def_readonly_static("e0", &PhysicalConstants_v6_0_0::ElectricConstant)
      .def_readonly_static("ElectronMass",
                           &PhysicalConstants_v6_0_0::ElectronMass)
      .def_readonly_static("me", &PhysicalConstants_v6_0_0::ElectronMass)
      .def_readonly_static("ElectronVolt",
                           &PhysicalConstants_v6_0_0::ElectronVolt)
      .def_readonly_static("eV", &PhysicalConstants_v6_0_0::ElectronVolt)
      .def_readonly_static("ElementaryCharge",
                           &PhysicalConstants_v6_0_0::ElementaryCharge)
      .def_readonly_static("e", &PhysicalConstants_v6_0_0::ElementaryCharge)
      .def_readonly_static("FaradayConstant",
                           &PhysicalConstants_v6_0_0::FaradayConstant)
      .def_readonly_static("F", &PhysicalConstants_v6_0_0::FaradayConstant)
      .def_readonly_static("FineStructureConstant",
                           &PhysicalConstants_v6_0_0::FineStructureConstant)
      .def_readonly_static("a",
                           &PhysicalConstants_v6_0_0::FineStructureConstant)
      .def_readonly_static("MolarGasConstant",
                           &PhysicalConstants_v6_0_0::MolarGasConstant)
      .def_readonly_static("R", &PhysicalConstants_v6_0_0::MolarGasConstant)
      .def_readonly_static("StefanBoltzmannConstant",
                           &PhysicalConstants_v6_0_0::StefanBoltzmannConstant)
      .def_readonly_static("s",
                           &PhysicalConstants_v6_0_0::StefanBoltzmannConstant);
}

struct PhysicalConstants_future : PhysicalConstants_v6_0_0 {};

#if TFEL_VERSION_MAJOR < 6
struct PhysicalConstants : PhysicalConstants_v5_2_0 {};
#else
struct PhysicalConstants : PhysicalConstants_v6_0_0 {};
#endif

void declarePhysicalConstants(pybind11::module_& m) {  
  auto m_v5_2_0 =
      m.def_submodule("v5_2_0", "Compatibility module with version 5.2.0.");
  auto m_v6_0_0 =
      m.def_submodule("v6_0_0", "Compatibility module with version 6.0.0.");
  declarePhysicalConstants_v5_2_0(m_v5_2_0);
  declarePhysicalConstants_v6_0_0(m_v6_0_0);
  //
  auto m_future = m.def_submodule(
      "future", "Compatibility module with unpublished versions of TFEL.");
  pybind11::class_<PhysicalConstants_future, PhysicalConstants_v6_0_0> w_future(
      m_future, "PhysicalConstants", "Physical constants");
#if TFEL_VERSION_MAJOR < 6
  pybind11::class_<PhysicalConstants, PhysicalConstants_v5_2_0>
      w(m, "PhysicalConstants", "Physical constants");
#else
  pybind11::class_<PhysicalConstants, PhysicalConstants_v6_0_0>
      w(m, "PhysicalConstants", "Physical constants");
#endif
}  // end of declarePhysicalConstants
