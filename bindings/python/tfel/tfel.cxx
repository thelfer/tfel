/*!
 * \file   tfel.cxx
 * \brief
 * \author Thomas Helfer
 * \date   18 août 2017
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>

void declareConfig(pybind11::module_&);
void declarePhysicalConstants(pybind11::module_&);

PYBIND11_MODULE(_tfel, m) {
  declareConfig(m);
//  declarePhysicalConstants(m);
}
