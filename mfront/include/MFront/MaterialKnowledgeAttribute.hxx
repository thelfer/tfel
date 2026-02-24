/*!
 * \file  mfront/include/MFront/MaterialKnowledgeAttribute.hxx
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

#ifndef LIB_MFRONT_MATERIALKNOWLEDGEATTRIBUTE_HXX
#define LIB_MFRONT_MATERIALKNOWLEDGEATTRIBUTE_HXX

#include <string>
#include <vector>
#include "TFEL/Metaprogramming/GenerateTypeList.hxx"
#include "TFEL/Utilities/GenTypeBase.hxx"

namespace mfront {

  //! \brief types that can be stored in a behaviour attribute
  using MaterialKnowledgeAttributeTypes =
      tfel::meta::GenerateTypeList<bool,
                                   unsigned short,
                                   std::string,
                                   std::vector<std::string>>::type;
  //! \brief a class storing mechanical behaviour attribute
  using MaterialKnowledgeAttribute =
      tfel::utilities::GenTypeBase<MaterialKnowledgeAttributeTypes>;

  template <typename T>
  constexpr bool isMaterialKnowledgeAttributeType() {
    return tfel::meta::TLCountNbrOfT<T,
                                     MaterialKnowledgeAttributeTypes>::value ==
           1;
  }  // end of isMaterialKnowledgeAttributeType

}  // end of namespace mfront

#endif /* LIB_MFRONT_MATERIALKNOWLEDGEATTRIBUTE_HXX */
