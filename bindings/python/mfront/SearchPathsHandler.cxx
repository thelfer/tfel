/*!
 * \file   SearchPathsHandler.cxx
 * \brief
 * \author Thomas Helfer
 * \date   21 juin 2017
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
  pybind11 ::class_<SearchPathsHandler>(m, "SearchPathsHandler")
      .def_static("addSearchPaths", &SearchPathsHandler::addSearchPaths,
                  "Add new search paths. Multiple paths are separated by "
                  "commas under unices systems and by semicolons under "
                  "Windows systems")
      .def_static("search", &SearchPathsHandler::search,
                  "search a file and return the path to it if found.")
      .def_static("getSearchPaths", &SearchPathsHandler::getSearchPaths,
                  "return all the registred search paths.");
}  // end of declareSearchPathsHandler
