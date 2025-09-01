/*!
 * \file   CrystalStructure.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09 juin 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Material/CrystalStructure.hxx"

void declareCrystalStructure(pybind11::module_&);

void declareCrystalStructure(pybind11::module_& m) {
  using tfel::material::CrystalStructure;
  pybind11::enum_<CrystalStructure>(m, "CrystalStructure")
      .value("Cubic", CrystalStructure::Cubic)
      .value("FCC", CrystalStructure::FCC)
      .value("HCP", CrystalStructure::HCP);
}
