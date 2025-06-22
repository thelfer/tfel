/*!
 * \file  mfront/include/MFront/VariableAttribute.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_VARIABLEATTRIBUTE_H_
#define LIB_MFRONT_VARIABLEATTRIBUTE_H_

#include <string>
#include <vector>
#include "TFEL/Metaprogramming/GenerateTypeList.hxx"
#include "TFEL/Utilities/GenTypeBase.hxx"
#include "MFront/VariableBoundsDescription.hxx"

namespace mfront {

  //! \brief types that can be stored in a variable attribute
  using VariableAttributeTypes =
      tfel::meta::GenerateTypeList<bool,
                                   unsigned short,
                                   double,
                                   std::string,
                                   std::vector<double>,
                                   std::vector<std::string>,
                                   VariableBoundsDescription>::type;
  //! \brief a class used to store a variable attribute
  using VariableAttribute =
      tfel::utilities::GenTypeBase<VariableAttributeTypes>;

}  // end of namespace mfront

#endif /* LIB_MFRONT_VARIABLEATTRIBUTE_H_ */
