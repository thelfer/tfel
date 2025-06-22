/*!
 * \file   mfront/src/AnsysException.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   09 nov 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/Ansys/AnsysException.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ && (!defined _GLIBCXX_USE_C99)
#include <sstream>
namespace std {
  template <typename T>
  std::string to_string(const T& v) {
    std::ostringstream s;
    s << v;
    return s.str();
  }
}  // namespace std
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

namespace ansys {

  AnsysException::AnsysException(const std::string& s)
      : msg(s) {}  // end of AnsysException::AnsysException

  AnsysException::AnsysException(AnsysException&&) = default;

  AnsysException::AnsysException(const AnsysException&) = default;

  const char* AnsysException::what() const noexcept {
    return msg.c_str();
  }  // end of AnsysException::what

  std::string AnsysException::getMsg() const noexcept {
    return msg;
  }  // end of AnsysException::getMsg

  AnsysException::~AnsysException() noexcept = default;

  AnsysInvalidModellingHypothesis::AnsysInvalidModellingHypothesis(
      AnsysInvalidModellingHypothesis&&) = default;

  AnsysInvalidModellingHypothesis::AnsysInvalidModellingHypothesis(
      const AnsysInvalidModellingHypothesis&) = default;

  AnsysInvalidModellingHypothesis::AnsysInvalidModellingHypothesis(
      const char* b)
      : AnsysException(
            "current modelling hypothesis is not supported by behaviour '" +
            std::string(b) + "'") {}

  AnsysInvalidModellingHypothesis::~AnsysInvalidModellingHypothesis() noexcept =
      default;

  AnsysInvalidNTENSValue::AnsysInvalidNTENSValue(const unsigned short N)
      : AnsysException("Invalid tensor size declared '" +
                       std::to_string(static_cast<unsigned int>(N)) + "'") {
  }  // end of AnsysInvalidNTENSValue::AnsysInvalidNTENSValue

  AnsysInvalidNTENSValue::AnsysInvalidNTENSValue(AnsysInvalidNTENSValue&&) =
      default;

  AnsysInvalidNTENSValue::AnsysInvalidNTENSValue(
      const AnsysInvalidNTENSValue&) = default;

  AnsysInvalidNTENSValue::~AnsysInvalidNTENSValue() noexcept = default;

  AnsysInvalidDimension::AnsysInvalidDimension(const std::string& b,
                                               const unsigned short N)
      : AnsysException("''" + b + "' can't be used in " +
                       std::to_string(static_cast<unsigned int>(N)) + "D") {
  }  // end of AnsysInvalidDimension::AnsysInvalidDimension

  AnsysInvalidDimension::AnsysInvalidDimension(const AnsysInvalidDimension&) =
      default;

  AnsysInvalidDimension::AnsysInvalidDimension(AnsysInvalidDimension&&) =
      default;

  AnsysInvalidDimension::~AnsysInvalidDimension() noexcept = default;

}  // end of namespace ansys
