/*!
 * \file  tfel-doc/src/TestDocumentation.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Utilities/Global.hxx"
#include "TFEL/Utilities/TestDocumentation.hxx"

namespace tfel::utilities {

  TestDocumentation::TestDocumentation() = default;

  TestDocumentation::TestDocumentation(const TestDocumentation&) = default;

  TestDocumentation::TestDocumentation(TestDocumentation&&) = default;

  TestDocumentation& TestDocumentation::operator=(const TestDocumentation&) =
      default;

  TestDocumentation& TestDocumentation::operator=(TestDocumentation&&) =
      default;

  TestDocumentation::~TestDocumentation() = default;

}  // end of namespace tfel::utilities
