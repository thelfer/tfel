/*!
 * \file   mfront/include/MFront/BehaviourDescription.ixx
 * \brief
 * \author Thomas Helfer
 * \brief  20 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURDESCRIPTION_IXX
#define LIB_MFRONT_BEHAVIOURDESCRIPTION_IXX

namespace mfront {

  template <typename T>
  const T& BehaviourDescription::getAttribute(const Hypothesis h,
                                              const std::string& n) const {
    const auto& bdata = this->getBehaviourData(h);
    return bdata.template getAttribute<T>(n);
  }  // end of BehaviourDescription::getAttribute

  template <typename T>
  T BehaviourDescription::getAttribute(const Hypothesis h,
                                       const std::string& n,
                                       const T& v) const {
    const auto& bdata = this->getBehaviourData(h);
    if (!bdata.hasAttribute(n)) {
      return v;
    }
    return bdata.template getAttribute<T>(n);
  }

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURDESCRIPTION_IXX */
