/*!
 * \file   TestCaseParameters.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   17/09/2019
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFMTestGenerator/TestCaseParameters.hxx"

namespace mfmtg{

  TestCaseParameters::TestCaseParameters() = default;

  TestCaseParameters::TestCaseParameters(const TestCaseParameters&) = default;

  TestCaseParameters::TestCaseParameters(TestCaseParameters&&) = default;

  TestCaseParameters& TestCaseParameters::operator=(const TestCaseParameters&) =
      default;

  TestCaseParameters& TestCaseParameters::operator=(TestCaseParameters&&) =
      default;

  TestCaseParameters::~TestCaseParameters() = default;

} // end of namespace mfmtg