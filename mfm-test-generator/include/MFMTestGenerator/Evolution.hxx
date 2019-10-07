/*!
 * \file   Evolution.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   30/09/2019
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFM_TEST_GENERATOR_EVOLUTION_HXX
#define LIB_MFM_TEST_GENERATOR_EVOLUTION_HXX

#include <map>
#include "TFEL/Utilities/GenTypeBase.hxx"


namespace mfmtg {

  //! \brief a simple alias
  struct Evolution
      : tfel::utilities::GenTypeBase<
            tfel::meta::GenerateTypeList<double,
                                         std::map<double, double>>::type> {
    //! a simple alias
    using GenTypBase = tfel::utilities::GenTypeBase<
        tfel::meta::GenerateTypeList<double, std::map<double, double>>::type>;
    //! inheriting constructors
    using GenTypBase::GenTypBase;
  };  // end of struct Evolution

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_GENERATOR_EVOLUTION_HXX */
