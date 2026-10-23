/*!
 * \file   LibraryInformation.cxx
 * \brief
 * \author Thomas Helfer
 * \date 13/04/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "TFEL/System/LibraryInformation.hxx"

void declareLibraryInformation(pybind11::module_&);

void declareLibraryInformation(pybind11::module_& m) {
  using tfel::system::LibraryInformation;
  std::vector<std::string> (LibraryInformation::*ptr1)() =
      &LibraryInformation::symbols;
  std::vector<std::string> (LibraryInformation::*ptr2)(const std::string&) =
      &LibraryInformation::symbols;
  pybind11::class_<LibraryInformation>(
      m, "LibraryInformation",
      "Class that is capable of extracting different\n"
      "information from a library or binary file.\n"
      "Currently understands ELF, MACH-O and PE formats on all the\n"
      "platforms.")
      .def(pybind11::init<std::string>(),
           pybind11::arg("f"),
           "Opens file with specified path and prepares for\n"
           "information extraction\n"
           "f: path to the binary file from which the info must be extracted\n"
           "b: throw an exception if this file format is not supported by OS")
      .def("sections", &LibraryInformation::sections,
           "List of sections that exist in binary file")
      .def("symbols", ptr1,
           "List of all the exportable symbols from all the\n"
           "sections that exist in binary file")
      .def("symbols", ptr2,
           pybind11::arg("n"),
           "The list of symbols from the specified section\n"
           "n: name of the section from which symbol names must be returned");
}
