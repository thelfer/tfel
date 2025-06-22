/*!
 * \file   Evolution.hxx
 * \brief
 * \author Thomas Helfer
 * \date   30/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFM_TEST_GENERATOR_EVOLUTION_HXX
#define LIB_MFM_TEST_GENERATOR_EVOLUTION_HXX

#include <map>
#include <string>
#include "TFEL/Utilities/GenTypeBase.hxx"

namespace mfmtg {

  /*!
   * \brief a simple structure used to describe an evolution defined in an
   * external file.
   */
  struct EvolutionFromFile {
    //! \brief a simple alias
    using Values = tfel::utilities::GenType<unsigned int, std::string>;
    //! \brief name of the file
    std::string file;
    //! \brief description of times
    Values times;
    //! \brief description of the values
    Values values;
  };  // end of EvolutionFromFile

  //! \brief types used to describe an evolution
  using EvolutionTypes = tfel::meta::GenerateTypeList<double,
                                                      std::map<double, double>,
                                                      EvolutionFromFile>::type;

  //! \brief various ways of storing an evolution
  struct Evolution : tfel::utilities::GenTypeBase<EvolutionTypes> {
    //! a simple alias
    using GenTypBase = tfel::utilities::GenTypeBase<EvolutionTypes>;
    //! inheriting constructors
    using GenTypBase::GenTypBase;
    //! inheriting assignement operator
    using GenTypBase::operator=;
  };  // end of struct Evolution

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_GENERATOR_EVOLUTION_HXX */
