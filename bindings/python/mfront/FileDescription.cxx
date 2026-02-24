/*!
 * \file   bindings/python/mfront/FileDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date   04 mai 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MFront/FileDescription.hxx"

void declareFileDescription(pybind11::module_&);

void declareFileDescription(pybind11::module_& m) {
  using namespace mfront;
  pybind11::class_<mfront::FileDescription>(m, "FileDescription")
      .def_readwrite("fileName", &mfront::FileDescription::fileName)
      .def_readwrite("authorName", &mfront::FileDescription::authorName)
      .def_readwrite("date", &mfront::FileDescription::date)
      .def_readwrite("description", &mfront::FileDescription::description);
}
