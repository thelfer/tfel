/*!
 * \file mfront/include/MFront/include/MaterialKnowledgeDescription.ixx
 * \brief
 * \author Thomas Helfer
 * \date   10/01/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MATERIALKNOWLEDGEDESCRIPTION_IXX
#define LIB_MFRONT_MATERIALKNOWLEDGEDESCRIPTION_IXX

namespace mfront {

  template <typename T>
  T& MaterialKnowledgeDescription::getAttribute(const std::string_view n)  //
      requires(isMaterialKnowledgeAttributeType<T>()) {
    auto p = this->attributes.find(n);
    if (p == this->attributes.end()) {
      p = this->attributes
              .insert({std::string{n}, MaterialKnowledgeAttribute(T())})
              .first;
    }
    return p->second.template get<T>();
  }  // end of getAttribute

  template <typename T>
  const T& MaterialKnowledgeDescription::getAttribute(
      const std::string_view n) const  //
      requires(isMaterialKnowledgeAttributeType<T>()) {
    const auto p = this->attributes.find(n);
    if (p == this->attributes.end()) {
      MaterialKnowledgeDescription::throwUndefinedAttribute(n);
    }
    return p->second.template get<T>();
  }  // end of getAttribute

  template <typename T>
  T MaterialKnowledgeDescription::getAttribute(const std::string_view n,
                                               const T& v) const  //
      requires(isMaterialKnowledgeAttributeType<T>()) {
    const auto p = this->attributes.find(n);
    if (p == this->attributes.end()) {
      return v;
    }
    return p->second.template get<T>();
  }  // end of getAttribute

}  // end of namespace mfront

#endif /* LIB_MFRONT_MATERIALKNOWLEDGEDESCRIPTION_IXX */
