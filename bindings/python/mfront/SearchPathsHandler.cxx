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

#include "boost/python.hpp"
#include "MFront/SearchPathsHandler.hxx"

void declareSearchPathsHandler();
void declareSearchPathsHandler() {
  using namespace boost;
  using namespace boost::python;
  using mfront::SearchPathsHandler;
  class_<SearchPathsHandler, noncopyable>("SearchPathsHandler", no_init)
      .def("addSearchPaths", &SearchPathsHandler::addSearchPaths,
           "Add new search paths. Multiple paths are separated by "
           "commas under unices systems and by semicolons under "
           "Windows systems")
      .staticmethod("addSearchPaths")
      .def("search", &SearchPathsHandler::search,
           "search a file and return the path to it if found.")
      .staticmethod("search")
      .def("getSearchPaths", &SearchPathsHandler::getSearchPaths,
           return_value_policy<copy_const_reference>(),
           "return all the registred search paths.")
      .staticmethod("getSearchPaths");

  //  SearchPathsHandler& getSearchPathsHandler();

}  // end of declareSearchPathsHandler
