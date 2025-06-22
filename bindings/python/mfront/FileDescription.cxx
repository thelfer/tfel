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

#include <boost/python.hpp>
#include "MFront/FileDescription.hxx"

void declareFileDescription() {
  using namespace boost::python;
  using namespace mfront;
  class_<mfront::FileDescription>("FileDescription")
      .def_readwrite("fileName", &mfront::FileDescription::fileName)
      .def_readwrite("authorName", &mfront::FileDescription::authorName)
      .def_readwrite("date", &mfront::FileDescription::date)
      .def_readwrite("description", &mfront::FileDescription::description);
}
