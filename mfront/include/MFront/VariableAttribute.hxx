/*!
 * \file  mfront/include/MFront/VariableAttribute.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 19 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_VARIABLEATTRIBUTE_HXX
#define LIB_MFRONT_VARIABLEATTRIBUTE_HXX

#include <string>
#include <vector>
#include "TFEL/Metaprogramming/GenerateTypeList.hxx"
#include "TFEL/Utilities/GenTypeBase.hxx"

namespace mfront {

  //! \brief types that can be stored in a variable attribute
  using VariableAttributeTypes =
      tfel::meta::GenerateTypeList<bool,
                                   unsigned short,
                                   int,
                                   double,
                                   std::string,
                                   std::vector<double>,
                                   std::vector<std::string>>::type;
  //! \brief a class used to store a variable attribute
  using VariableAttribute =
      tfel::utilities::GenTypeBase<VariableAttributeTypes>;

}  // end of namespace mfront

#endif /* LIB_MFRONT_VARIABLEATTRIBUTE_HXX */
