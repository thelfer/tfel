/*!
 * \file   mfront/include/MFront/BehaviourDSLBase.ixx
 * \brief
 * \author Thomas Helfer
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURDSLBASE_IXX
#define LIB_MFRONT_BEHAVIOURDSLBASE_IXX

#include <sstream>
#include <stdexcept>
#include <algorithm>

#include "MFront/AbstractBehaviourInterface.hxx"
#include "MFront/MFrontLogStream.hxx"

namespace mfront {

  template <typename Child>
  BehaviourDSLBase<Child>::BehaviourDSLBase() = default;

  template <typename Child>
  void BehaviourDSLBase<Child>::registerNewCallBack(const std::string& k,
                                                    const MemberFuncPtr f) {
    this->addCallBack(k, [this, f] {
      auto& c = *(static_cast<Child*>(this));
      (c.*f)();
    });
  }  // end of registerNewCallBack

  template <typename Child>
  BehaviourDSLBase<Child>::~BehaviourDSLBase() = default;

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURDSLBASE_IXX */
