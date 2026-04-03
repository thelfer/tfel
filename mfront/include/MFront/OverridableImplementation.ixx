/*!
 * \file   include/MFront/OverridableImplementation.ixx
 * \brief
 * \author Thomas Helfer
 * \date   01/10/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_OVERRIDABLEIMPLEMENTATION_IXX
#define LIB_MFRONT_OVERRIDABLEIMPLEMENTATION_IXX

namespace mfront {

  template <OverridableStringInformationTags::Tags Tag>
  void OverridableStringInformation<Tag>::setOverridenValue(
      const std::string& v) {
    if (!this->overriden_value.empty()) {
      throwOverridenValueAlreadySet();
    }
    this->overriden_value = v;
  }  // end of OverridableStringInformation<Tag>::setOverridenValue

  template <OverridableStringInformationTags::Tags Tag>
  const std::string& OverridableStringInformation<Tag>::getOverridenValue()
      const {
    return this->overriden_value;
  }  // end of OverridableStringInformation<Tag>::setOverridenValue

  template <OverridableImplementation::Tags Tag>
  void OverridableImplementation::setOverridenValue(const std::string& v) {
    static_cast<OverridableStringInformation<Tag>&>(*this).setOverridenValue(v);
  }  // end of OverridableImplementation::setOverridenValue

  template <OverridableImplementation::Tags Tag>
  const std::string& OverridableImplementation::getOverridenValue() const {
    return static_cast<const OverridableStringInformation<Tag>&>(*this)
        .getOverridenValue();
  }  // end of OverridableImplementation::getOverridenValue

}  // end of namespace mfront

#endif /* LIB_MFRONT_OVERRIDABLEIMPLEMENTATION_IXX */
