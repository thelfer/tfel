/*!
 * \file  bindings/python/tfel/TestResult.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 20 sept. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <boost/python.hpp>

#include "TFEL/Tests/TestResult.hxx"

void declareTestResult();

void declareTestResult() {
  using namespace boost;
  using namespace boost::python;
  using namespace tfel::tests;
  class_<TestResult>("TestResult");

}  // end of declareExternalLibraryManager
