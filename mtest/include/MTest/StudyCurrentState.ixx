/*!
 * \file   StudyCurrentState.ixx
 * \brief
 * \author THOMAS HELFER
 * \date   23 nov. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_STUDYCURRENTSTATE_IXX_
#define LIB_MTEST_STUDYCURRENTSTATE_IXX_

namespace mtest {

  template <typename T>
  bool StudyCurrentState::checkParameterType(const std::string& n) const {
    const auto p = this->parameters.find(n);
    if (p == this->parameters.end()) {
      StudyCurrentState::throwUnknownParameterException(n);
    }
    return p->second.template is<T>();
  }  // end of StudyCurrentState::checkParameterType

  template <typename T>
  T& StudyCurrentState::getParameter(const std::string& n, const bool b) {
    auto p = this->parameters.find(n);
    if (p == this->parameters.end()) {
      if (b) {
        return this->setParameter(n, T());
      } else {
        StudyCurrentState::throwUnknownParameterException(n);
      }
    }
    return p->second.template get<T>();
  }  // end of StudyCurrentState::getParameter

  template <typename T>
  const T& StudyCurrentState::getParameter(const std::string& n) const {
    const auto p = this->parameters.find(n);
    if (p == this->parameters.end()) {
      StudyCurrentState::throwUnknownParameterException(n);
    }
    return p->second.template get<T>();
  }  // end of StudyCurrentState::getParameter

  template <typename T>
  T& StudyCurrentState::setParameter(const std::string& n, T&& v) {
    const auto p = this->parameters.find(n);
    if (p != this->parameters.end()) {
      StudyCurrentState::throwAlreadyDeclaredParameterException(n);
    }
    return this->parameters.insert({n, StudyParameter{v}}).first->second;
  }  // end of StudyCurrentState::getParameter

}  // end of namespace mtest

#endif /* LIB_MTEST_STUDYCURRENTSTATE_IXX_ */
