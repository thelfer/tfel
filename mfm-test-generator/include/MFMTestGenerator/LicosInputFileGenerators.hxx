/*!
 * \file   mfm-test-generator/include/MFMTestGenerator/LicosFileGenerators.hxx
 * \brief
 * \author Thomas Helfer
 * \date   02/10/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFMTESTGENERATOR_LICOSFILEGENERATORS_HXX
#define LIB_MFMTESTGENERATOR_LICOSFILEGENERATORS_HXX

namespace mfmtg {

  // forward declaration
  struct AbstractTestCase;

  namespace licos {

    void generateUniaxialTensileTest(const AbstractTestCase&);

  }  // end of namespace licos

}  // end of namespace mfmtg

#endif /* LIB_MFMTESTGENERATOR_LICOSFILEGENERATORS_HXX */
