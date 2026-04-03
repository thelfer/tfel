/*!
 * \file  bindings/python/tfel/TextData.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 13 mars 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/Utilities/TextData.hxx"

void declareTextData(pybind11::module_&);

void declareTextData(pybind11::module_& m) {
  using namespace tfel::utilities;

  std::vector<double> (TextData::*getColumn)(const Token::size_type) const =
      &TextData::getColumn;

  pybind11::class_<TextData>(m, "TextData")
      .def(pybind11::init<std::string>())
      .def(pybind11::init<std::string, std::string>())
      .def("getColumn", getColumn);

}  // end of declaretvector
