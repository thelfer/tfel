/*!
 * \file   StructureCurrentState.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08/12/2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include "MTest/CurrentState.hxx"
#include "MTest/StructureCurrentState.hxx"

static tfel::math::vector<mtest::CurrentState>& get_istates(
    mtest::StructureCurrentState& scs) {
  return scs.istates;
}

void declareStructureCurrentState();

void declareStructureCurrentState() {
  using namespace boost::python;
  class_<mtest::StructureCurrentState>("StructureCurrentState")
      .add_property("istates",
                    make_function(&get_istates, return_internal_reference<>()));
}  // end of declareStructureCurrentState
