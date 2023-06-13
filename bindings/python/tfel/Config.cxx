/*!
 * \file   bindings/python/tfel/Config.cxx
 * \brief
 * \author Thomas Helfer
 * \date   02/04/2023
 */

#include <string>
#include <boost/python.hpp>
#include "TFEL/Config/GetTFELVersion.h"

void declareConfig();

void declareConfig() {
  using namespace boost::python;
  def("getTFELVersion", ::getTFELVersion);
}  // end of declareConfig