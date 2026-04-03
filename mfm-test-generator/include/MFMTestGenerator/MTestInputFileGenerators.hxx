/*!
 * \file   mfm-test-generator/include/MFMTestGenerator/MTestFileGenerators.hxx
 * \brief
 * \author Thomas Helfer
 * \date   25/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFMTESTGENERATOR_MTESTFILEGENERATORS_HXX
#define LIB_MFMTESTGENERATOR_MTESTFILEGENERATORS_HXX

namespace mfmtg {

  // forward declaration
  struct AbstractTestCase;

  namespace mtest {

    void generateUniaxialTensileTest(const AbstractTestCase&);

    void generateClosedPipeTest(const AbstractTestCase&);

  }  // end of namespace mtest

}  // end of namespace mfmtg

#endif /* LIB_MFMTESTGENERATOR_MTESTFILEGENERATORS_HXX */
