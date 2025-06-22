/*!
 * \file   mfront/src/CyranoException.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   21 fév 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/Cyrano/CyranoException.hxx"

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

namespace cyrano {

  CyranoException::CyranoException(const std::string& s)
      : msg(s) {}  // end of CyranoException::CyranoException

  CyranoException::CyranoException(const CyranoException& e)
      : std::exception(),
        msg(e.msg) {}  // end of CyranoException::CyranoException

  const char* CyranoException::what() const noexcept {
    return msg.c_str();
  }  // end of CyranoException::what

  std::string CyranoException::getMsg() const noexcept {
    return msg;
  }  // end of CyranoException::getMsg

  CyranoException::~CyranoException() noexcept = default;

  CyranoIntegrationFailed::CyranoIntegrationFailed()
      : CyranoException("behaviour integration failed") {
  }  // end of CyranoIntegrationFailed::CyranoIntegrationFailed

  CyranoIntegrationFailed::CyranoIntegrationFailed(const std::string& m)
      : CyranoException("behaviour integration failed : " + m) {
  }  // end of CyranoIntegrationFailed::CyranoIntegrationFailed

  CyranoIntegrationFailed::CyranoIntegrationFailed(
      const CyranoIntegrationFailed&) = default;

  CyranoIntegrationFailed::~CyranoIntegrationFailed() noexcept = default;

  CyranoInvalidNTENSValue::CyranoInvalidNTENSValue(const unsigned short N)
      : CyranoException("Invalid tensor size declared '" +
                        std::to_string(static_cast<unsigned int>(N)) + "'") {
  }  // end of CyranoInvalidNTENSValue::CyranoInvalidNTENSValue

  CyranoInvalidNTENSValue::CyranoInvalidNTENSValue(
      const CyranoInvalidNTENSValue&) = default;

  CyranoInvalidNTENSValue::~CyranoInvalidNTENSValue() noexcept = default;

  CyranoInvalidDimension::CyranoInvalidDimension(const std::string& b,
                                                 const unsigned short N)
      : CyranoException("''" + b + "' can't be used in " +
                        std::to_string(static_cast<unsigned int>(N)) + "D") {
  }  // end of CyranoInvalidDimension::CyranoInvalidDimension

  CyranoInvalidDimension::CyranoInvalidDimension(
      const CyranoInvalidDimension&) = default;

  CyranoInvalidDimension::~CyranoInvalidDimension() noexcept = default;

}  // end of namespace cyrano
