/*!
 * \file   SolverWorkSpace.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include "MTest/SolverWorkSpace.hxx"

void declareSolverWorkSpace();

void declareSolverWorkSpace() {
  boost::python::class_<mtest::SolverWorkSpace, boost::noncopyable>(
      "SolverWorkSpace");
}
