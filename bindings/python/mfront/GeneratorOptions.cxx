/*!
 * \file   GeneratorOptions.cxx
 * \brief
 * \author HELFER Thomas 202608
 * \date   11/07/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include "MFront/GeneratorOptions.hxx"

void declareGeneratorOptions();

void declareGeneratorOptions() {
  using namespace boost::python;
  using namespace mfront;

  enum_<GeneratorOptions::OptimisationLevel>(
      "GeneratorOptionsOptimisationLevel")
      .value("Level0", GeneratorOptions::OptimisationLevel::LEVEL0)
      .value("Level1", GeneratorOptions::OptimisationLevel::LEVEL1)
      .value("Level2", GeneratorOptions::OptimisationLevel::LEVEL2);

  class_<GeneratorOptions>("GeneratorOptions")
      .def_readwrite("olevel", &GeneratorOptions::olevel,
                     "the optimisation level");

}  // end of declareGeneratorOptions
