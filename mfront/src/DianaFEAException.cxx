/*!
 * \file   mfront/src/DianaFEAException.cxx
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

#include "MFront/DianaFEA/DianaFEAException.hxx"

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

namespace dianafea {

  DianaFEAException::DianaFEAException(const std::string& s)
      : msg(s) {}  // end of DianaFEAException::DianaFEAException

  DianaFEAException::DianaFEAException(DianaFEAException&&) = default;

  DianaFEAException::DianaFEAException(const DianaFEAException&) = default;

  const char* DianaFEAException::what() const noexcept {
    return msg.c_str();
  }  // end of DianaFEAException::what

  std::string DianaFEAException::getMsg() const noexcept {
    return msg;
  }  // end of DianaFEAException::getMsg

  DianaFEAException::~DianaFEAException() noexcept = default;

  DianaFEAInvalidModellingHypothesis::DianaFEAInvalidModellingHypothesis(
      DianaFEAInvalidModellingHypothesis&&) = default;

  DianaFEAInvalidModellingHypothesis::DianaFEAInvalidModellingHypothesis(
      const DianaFEAInvalidModellingHypothesis&) = default;

  DianaFEAInvalidModellingHypothesis::DianaFEAInvalidModellingHypothesis(
      const char* b)
      : DianaFEAException(
            "current modelling hypothesis is not supported by behaviour '" +
            std::string(b) + "'") {}

  DianaFEAInvalidModellingHypothesis::
      ~DianaFEAInvalidModellingHypothesis() noexcept = default;

  DianaFEAInvalidNTENSValue::DianaFEAInvalidNTENSValue(const unsigned short N)
      : DianaFEAException("Invalid tensor size declared '" +
                          std::to_string(static_cast<unsigned int>(N)) + "'") {
  }  // end of DianaFEAInvalidNTENSValue::DianaFEAInvalidNTENSValue

  DianaFEAInvalidNTENSValue::DianaFEAInvalidNTENSValue(
      DianaFEAInvalidNTENSValue&&) = default;

  DianaFEAInvalidNTENSValue::DianaFEAInvalidNTENSValue(
      const DianaFEAInvalidNTENSValue&) = default;

  DianaFEAInvalidNTENSValue::~DianaFEAInvalidNTENSValue() noexcept = default;

  DianaFEAInvalidDimension::DianaFEAInvalidDimension(const std::string& b,
                                                     const unsigned short N)
      : DianaFEAException("''" + b + "' can't be used in " +
                          std::to_string(static_cast<unsigned int>(N)) + "D") {
  }  // end of DianaFEAInvalidDimension::DianaFEAInvalidDimension

  DianaFEAInvalidDimension::DianaFEAInvalidDimension(
      const DianaFEAInvalidDimension&) = default;

  DianaFEAInvalidDimension::DianaFEAInvalidDimension(
      DianaFEAInvalidDimension&&) = default;

  DianaFEAInvalidDimension::~DianaFEAInvalidDimension() noexcept = default;

}  // end of namespace dianafea
