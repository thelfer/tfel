/*!
 * \file   LibraryInformation.cxx
 * \brief
 * \author Thomas Helfer
 * \date   13 avril 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>
#include "TFEL/System/LibraryInformation.hxx"

void declareLibraryInformation();

void declareLibraryInformation() {
  using tfel::system::LibraryInformation;
  std::vector<std::string> (LibraryInformation::*ptr1)() =
      &LibraryInformation::symbols;
  std::vector<std::string> (LibraryInformation::*ptr2)(const std::string&) =
      &LibraryInformation::symbols;
  boost::python::class_<LibraryInformation>("LibraryInformation",
                                            boost::python::init<std::string>())
      .def("sections", &LibraryInformation::sections)
      .def("symbols", ptr1)
      .def("symbols", ptr2);
}
