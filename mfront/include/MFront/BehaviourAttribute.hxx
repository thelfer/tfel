/*!
 * \file  mfront/include/MFront/BehaviourAttribute.hxx
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

#ifndef LIB_MFRONT_BEHAVIOURATTRIBUTE_H_
#define LIB_MFRONT_BEHAVIOURATTRIBUTE_H_

#include <string>
#include "TFEL/Metaprogramming/GenerateTypeList.hxx"
#include "TFEL/Utilities/GenTypeBase.hxx"

namespace mfront {

  //! types that can be stored in a behaviour attribute
  typedef tfel::meta::GenerateTypeList<bool, unsigned short, std::string>::type
      BehaviourAttributeTypes;
  //! a class storing mechanical behaviour attribute
  typedef tfel::utilities::GenTypeBase<BehaviourAttributeTypes>
      BehaviourAttribute;

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURATTRIBUTE_H_ */
