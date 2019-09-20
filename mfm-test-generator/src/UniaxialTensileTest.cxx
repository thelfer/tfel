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

  UniaxialTensileTest::UniaxialTensileTest() = default;

  void UniaxialTensileTest::generate(const TestCaseParameters& p,
                                     const std::vector<std::string>&) const {
    if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL2) {
      mfront::getLogStream() << "UniaxialTensileTest::generate: "
                             << "treatment of test '" << p.name << "' begins\n";
    }

    if (mfront::getVerboseMode() >= mfront::VERBOSE_LEVEL2) {
      mfront::getLogStream() << "UniaxialTensileTest::generate: "
                             << "treatment of test '" << p.name << "' ends\n";
    }
  }  // end of UniaxialTensileTest::generate

  UniaxialTensileTest::~UniaxialTensileTest() = default;

}  // end of namespace mfmtg

