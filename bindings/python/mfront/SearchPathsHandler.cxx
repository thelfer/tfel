/*!
 * \file   SearchPathsHandler.cxx
 * \brief
 * \author Thomas Helfer
 * \date 21/06/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "MFront/SearchPathsHandler.hxx"

void declareSearchPathsHandler(pybind11::module_&);

void declareSearchPathsHandler(pybind11::module_& m) {
  using mfront::SearchPathsHandler;
  pybind11::class_<SearchPathsHandler>(
      m, "SearchPathsHandler",
      "Structure in charge of:\n"
      "- storing search paths\n"
      "- searching imported mfront file")
      .def_static("addSearchPaths", &SearchPathsHandler::addSearchPaths,
                   pybind11::arg("p"),
                   "Add new search paths.\n"
                   "Multiple paths are separated by commas under unices "
                   "systems and by semicolons under Windows systems.\n"
                   "p: new search paths")
      .def_static("search", &SearchPathsHandler::search,
                   pybind11::arg("f"),
                   "Search a file in the specified paths.\n"
                   "The file is searched using the following conventions:\n"
                   "- search in the current directory.\n"
                   "- search in the search paths from the most recently "
                   "declared\n"
                   "search path to the older ones.\n"
                   "Search paths declared through the MFRONT_INCLUDE_PATH are\n"
                   "stored at the end of the list.\n"
                   "f: file name\n"
                   "Return the full path of the file")
      .def_static("getSearchPaths", &SearchPathsHandler::getSearchPaths,
                  "Return the list of the search paths associated with "
                  "directories");
}
