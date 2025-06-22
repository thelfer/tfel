/*!
 * \file  mfront/include/MFront/BehaviourAttribute.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURATTRIBUTE_HXX
#define LIB_MFRONT_BEHAVIOURATTRIBUTE_HXX

#include <string>
#include <vector>
#include "TFEL/Metaprogramming/GenerateTypeList.hxx"
#include "TFEL/Utilities/GenTypeBase.hxx"

namespace mfront {

  //! types that can be stored in a behaviour attribute
  using BehaviourAttributeTypes =
      tfel::meta::GenerateTypeList<bool,
                                   unsigned short,
                                   std::string,
                                   std::vector<std::string>>::type;
  //! a class storing mechanical behaviour attribute
  typedef tfel::utilities::GenTypeBase<BehaviourAttributeTypes>
      BehaviourAttribute;

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURATTRIBUTE_HXX */
