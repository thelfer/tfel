/*!
 * \file   mfront/src/CalculiXException.cxx
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

#include "MFront/CalculiX/CalculiXException.hxx"

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

namespace calculix {

  CalculiXException::CalculiXException(const std::string& s)
      : msg(s) {}  // end of CalculiXException::CalculiXException

  CalculiXException::CalculiXException(CalculiXException&&) = default;

  CalculiXException::CalculiXException(const CalculiXException&) = default;

  const char* CalculiXException::what() const noexcept {
    return msg.c_str();
  }  // end of CalculiXException::what

  std::string CalculiXException::getMsg() const noexcept {
    return msg;
  }  // end of CalculiXException::getMsg

  CalculiXException::~CalculiXException() noexcept = default;

  CalculiXInvalidModellingHypothesis::CalculiXInvalidModellingHypothesis(
      CalculiXInvalidModellingHypothesis&&) = default;

  CalculiXInvalidModellingHypothesis::CalculiXInvalidModellingHypothesis(
      const CalculiXInvalidModellingHypothesis&) = default;

  CalculiXInvalidModellingHypothesis::CalculiXInvalidModellingHypothesis(
      const char* b)
      : CalculiXException(
            "current modelling hypothesis is not supported by behaviour '" +
            std::string(b) + "'") {}

  CalculiXInvalidModellingHypothesis::
      ~CalculiXInvalidModellingHypothesis() noexcept = default;

  CalculiXInvalidNTENSValue::CalculiXInvalidNTENSValue(const unsigned short N)
      : CalculiXException("Invalid tensor size declared '" +
                          std::to_string(static_cast<unsigned int>(N)) + "'") {
  }  // end of CalculiXInvalidNTENSValue::CalculiXInvalidNTENSValue

  CalculiXInvalidNTENSValue::CalculiXInvalidNTENSValue(
      CalculiXInvalidNTENSValue&&) = default;

  CalculiXInvalidNTENSValue::CalculiXInvalidNTENSValue(
      const CalculiXInvalidNTENSValue&) = default;

  CalculiXInvalidNTENSValue::~CalculiXInvalidNTENSValue() noexcept = default;

  CalculiXInvalidDimension::CalculiXInvalidDimension(const std::string& b,
                                                     const unsigned short N)
      : CalculiXException("''" + b + "' can't be used in " +
                          std::to_string(static_cast<unsigned int>(N)) + "D") {
  }  // end of CalculiXInvalidDimension::CalculiXInvalidDimension

  CalculiXInvalidDimension::CalculiXInvalidDimension(
      const CalculiXInvalidDimension&) = default;

  CalculiXInvalidDimension::CalculiXInvalidDimension(
      CalculiXInvalidDimension&&) = default;

  CalculiXInvalidDimension::~CalculiXInvalidDimension() noexcept = default;

}  // end of namespace calculix
