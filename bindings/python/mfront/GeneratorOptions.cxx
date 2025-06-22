
/*!
 * \file   GeneratorOptions.cxx
 * \brief
 * \author Thomas Helfer
 * \date   11/07/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MFront/GeneratorOptions.hxx"

void declareGeneratorOptions(pybind11::module_&);

void declareGeneratorOptions(pybind11::module_& m) {
  using namespace mfront;
  pybind11::enum_<GeneratorOptions::OptimisationLevel>(
      m, "GeneratorOptionsOptimisationLevel")
      .value("Level0", GeneratorOptions::OptimisationLevel::LEVEL0)
      .value("Level1", GeneratorOptions::OptimisationLevel::LEVEL1)
      .value("Level2", GeneratorOptions::OptimisationLevel::LEVEL2);

  pybind11::class_<GeneratorOptions>(m, "GeneratorOptions")
      .def_readwrite("olevel", &GeneratorOptions::olevel,
                     "the optimisation level");
}  // end of declareGeneratorOptions
