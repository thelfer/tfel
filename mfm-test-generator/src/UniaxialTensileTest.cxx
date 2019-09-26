/*!
 * \file   UniaxialTensileTest.cxx
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

#include <ostream>
#include "MFront/MFrontLogStream.hxx"
#include "MFMTestGenerator/TestCaseParameters.hxx"
#include "MFMTestGenerator/UniaxialTensileTest.hxx"

namespace mfmtg {

  UniaxialTensileTest::UniaxialTensileTest(const TestCaseParameters& p)
      : TestCaseBase(p) {
    //     if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL2) {
    //       mfront::getLogStream() << "UniaxialTensileTest::generate: "
    //                              << "treatment of test '"
    //                              << get<std::string>(p, "name") << "'
    //                              begins\n";
    //     }
    //
    //     if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL2) {
    //       mfront::getLogStream() << "UniaxialTensileTest::generate: "
    //                              << "treatment of test '"
    //                              << get<std::string>(p, "name") << "'
    //                              ends\n";
    //     }
  }  // end of UniaxialTensileTest::UniaxialTensileTest

  void UniaxialTensileTest::generate() const {
  }  // end of UniaxialTensileTest::generate

  UniaxialTensileTest::~UniaxialTensileTest() = default;

}  // end of namespace mfmtg

