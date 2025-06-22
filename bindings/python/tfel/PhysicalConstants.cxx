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

namespace tfel {
  struct TFEL_VISIBILITY_EXPORT PhysicalConstants {
    //! Atomic mass constant (kg)
    static const double mu;
    //! Avogadro constant (mol-1)
    static const double Na;
    //! Boltzmann constant (J.K-1)
    static const double kb;
    //! Conductance quantum (S)
    static const double G0;
    //! Electric constant (F.m-1)
    static const double e0;
    //! Electron mass (kg)
    static const double me;
    //! Electron volt (J)
    static const double eV;
    //! Elementary charge (C)
    static const double e;
    //! Faraday constant (C.mol-1)
    static const double F;
    //! Fine-structure constant
    static const double a;
    //! Molar gas constant (J.mol-1.K-1)
    static const double R;
    //! Stefan-Boltzmann constant (W.m-2.K-4)
    static const double s;
  };

  const double PhysicalConstants::mu = double(1.66054e-27);
  const double PhysicalConstants::Na = double(6.02214e+23);
  const double PhysicalConstants::kb = double(1.38065e-23);
  const double PhysicalConstants::G0 = double(7.74809e-05);
  const double PhysicalConstants::e0 = double(8.85419e-12);
  const double PhysicalConstants::me = double(9.10938e-31);
  const double PhysicalConstants::eV = double(1.60218e-19);
  const double PhysicalConstants::e = double(1.60218e-19);
  const double PhysicalConstants::F = double(96485.3);
  const double PhysicalConstants::a = double(0.00729735);
  const double PhysicalConstants::R = double(8.31446);
  const double PhysicalConstants::s = double(5.67037e-08);

}  // end of namespace tfel
void declarePhysicalConstants() {
  boost::python::class_<tfel::PhysicalConstants>("PhysicalConstants")
      .def_readonly("mu", tfel::PhysicalConstants::mu,
                    "Atomic mass constant (kg)")
      .def_readonly("Na", tfel::PhysicalConstants::Na,
                    "Avogadro constant (mol-1)")
      .def_readonly("kb", tfel::PhysicalConstants::kb,
                    "Boltzmann constant (J.K-1)")
      .def_readonly("G0", tfel::PhysicalConstants::G0,
                    "Conductance quantum (S)")
      .def_readonly("e0", tfel::PhysicalConstants::e0,
                    "Electric constant (F.m-1)")
      .def_readonly("me", tfel::PhysicalConstants::me, "Electron mass (kg)")
      .def_readonly("eV", tfel::PhysicalConstants::eV, "Electron volt (J)")
      .def_readonly("e", tfel::PhysicalConstants::e, "Elementary charge (C)")
      .def_readonly("F", tfel::PhysicalConstants::F,
                    "Faraday constant (C.mol-1)")
      .def_readonly("a", tfel::PhysicalConstants::a, "Fine-structure constant")
      .def_readonly("R", tfel::PhysicalConstants::R,
                    "Molar gas constant (J.mol-1.K-1)")
      .def_readonly("s", tfel::PhysicalConstants::s,
                    "Stefan-Boltzmann constant (W.m-2.K-4)");

}  // end of declarePhysicalConstants
