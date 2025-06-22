/*!
 * \file   OutOfBoundsPolicy.cxx
 * \brief
 * \author Thomas Helfer
 * \date   02 sept. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>

#include "TFEL/Material/OutOfBoundsPolicy.hxx"

void declareOutOfBoundsPolicy();

void declareOutOfBoundsPolicy() {
  using namespace boost::python;
  using namespace tfel::material;
  enum_<OutOfBoundsPolicy>("OutOfBoundsPolicy")
      .value("Strict", Strict)
      .value("Warning", Warning)
      .value("None", None);
}
