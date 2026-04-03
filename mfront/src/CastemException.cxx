/*!
 * \file   mfront/src/CastemException.cxx
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

#include "MFront/Castem/CastemException.hxx"

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

namespace castem {

  CastemException::CastemException(const std::string& s)
      : msg(s) {}  // end of CastemException::CastemException

  const char* CastemException::what() const noexcept {
    return msg.c_str();
  }  // end of CastemException::what

  CastemException::~CastemException() noexcept = default;

  CastemIntegrationFailed::CastemIntegrationFailed()
      : CastemException("behaviour integration failed") {
  }  // end of CastemIntegrationFailed::CastemIntegrationFailed

  CastemIntegrationFailed::CastemIntegrationFailed(const std::string& m)
      : CastemException("behaviour integration failed : " + m) {
  }  // end of CastemIntegrationFailed::CastemIntegrationFailed

  CastemIntegrationFailed::CastemIntegrationFailed(
      const CastemIntegrationFailed&) = default;

  CastemIntegrationFailed::~CastemIntegrationFailed() noexcept = default;

  CastemInvalidNTENSValue::CastemInvalidNTENSValue(const unsigned short N)
      : CastemException("Invalid tensor size declared '" +
                        std::to_string(static_cast<unsigned int>(N)) + "'") {
  }  // end of CastemInvalidNTENSValue::CastemInvalidNTENSValue

  CastemInvalidNTENSValue::CastemInvalidNTENSValue(
      const CastemInvalidNTENSValue&) = default;

  CastemInvalidNTENSValue::~CastemInvalidNTENSValue() noexcept {
  }  // end of CastemInvalidNTENSValue::~CastemInvalidNTENSValue()

  CastemInvalidDimension::CastemInvalidDimension(const std::string& b,
                                                 const unsigned short N)
      : CastemException("''" + b + "' can't be used in " +
                        std::to_string(static_cast<unsigned int>(N)) + "D") {
  }  // end of CastemInvalidDimension::CastemInvalidDimension

  CastemInvalidDimension::CastemInvalidDimension(
      const CastemInvalidDimension&) = default;

  CastemInvalidDimension::~CastemInvalidDimension() noexcept = default;

}  // end of namespace castem
