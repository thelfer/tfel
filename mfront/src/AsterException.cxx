/*!
 * \file   mfront/src/AsterException.cxx
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

#include "MFront/Aster/AsterException.hxx"

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

namespace aster {

  AsterException::AsterException(const std::string& s)
      : msg(s) {}  // end of AsterException::AsterException

  AsterException::AsterException(const AsterException& e)
      : msg(e.msg) {}  // end of AsterException::AsterException

  const char* AsterException::what() const noexcept {
    return msg.c_str();
  }  // end of AsterException::what

  std::string AsterException::getMsg() const noexcept {
    return msg;
  }  // end of AsterException::getMsg

  AsterException::~AsterException() noexcept = default;

  AsterInvalidNTENSValue::AsterInvalidNTENSValue(const unsigned short N)
      : AsterException("Invalid tensor size declared '" +
                       std::to_string(static_cast<unsigned int>(N)) + "'") {
  }  // end of AsterInvalidNTENSValue::AsterInvalidNTENSValue

  AsterInvalidNTENSValue::AsterInvalidNTENSValue(
      const AsterInvalidNTENSValue& e)
      : AsterException(e) {
  }  // end of AsterInvalidNTENSValue::AsterInvalidNTENSValue

  AsterInvalidNTENSValue::~AsterInvalidNTENSValue() noexcept = default;

  AsterInvalidDimension::AsterInvalidDimension(const std::string& b,
                                               const unsigned short N)
      : AsterException("''" + b + "' can't be used in " +
                       std::to_string(static_cast<unsigned int>(N)) + "D") {
  }  // end of AsterInvalidDimension::AsterInvalidDimension

  AsterInvalidDimension::AsterInvalidDimension(const AsterInvalidDimension&) =
      default;
  AsterInvalidDimension::~AsterInvalidDimension() noexcept = default;

  AsterInvalidModellingHypothesis::AsterInvalidModellingHypothesis()
      : AsterException("unsupported modelling hypothesis") {}

  AsterInvalidModellingHypothesis::AsterInvalidModellingHypothesis(
      const AsterInvalidModellingHypothesis&) = default;

  AsterInvalidModellingHypothesis::~AsterInvalidModellingHypothesis() noexcept =
      default;

}  // end of namespace aster
