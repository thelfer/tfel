/*!
 * \file   mfront/src/AbaqusException.cxx
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

#include "MFront/Abaqus/AbaqusException.hxx"

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

namespace abaqus {

  AbaqusException::AbaqusException(const std::string& s)
      : msg(s) {}  // end of AbaqusException::AbaqusException

  AbaqusException::AbaqusException(AbaqusException&&) = default;

  AbaqusException::AbaqusException(const AbaqusException&) = default;

  const char* AbaqusException::what() const noexcept {
    return msg.c_str();
  }  // end of AbaqusException::what

  std::string AbaqusException::getMsg() const noexcept {
    return msg;
  }  // end of AbaqusException::getMsg

  AbaqusException::~AbaqusException() noexcept = default;

  AbaqusInvalidModellingHypothesis::AbaqusInvalidModellingHypothesis(
      AbaqusInvalidModellingHypothesis&&) = default;

  AbaqusInvalidModellingHypothesis::AbaqusInvalidModellingHypothesis(
      const AbaqusInvalidModellingHypothesis&) = default;

  AbaqusInvalidModellingHypothesis::AbaqusInvalidModellingHypothesis(
      const char* b)
      : AbaqusException(
            "current modelling hypothesis is not supported by behaviour '" +
            std::string(b) + "'") {}

  AbaqusInvalidModellingHypothesis::
      ~AbaqusInvalidModellingHypothesis() noexcept = default;

  AbaqusInvalidNTENSValue::AbaqusInvalidNTENSValue(const unsigned short N)
      : AbaqusException("Invalid tensor size declared '" +
                        std::to_string(static_cast<unsigned int>(N)) + "'") {
  }  // end of AbaqusInvalidNTENSValue::AbaqusInvalidNTENSValue

  AbaqusInvalidNTENSValue::AbaqusInvalidNTENSValue(AbaqusInvalidNTENSValue&&) =
      default;

  AbaqusInvalidNTENSValue::AbaqusInvalidNTENSValue(
      const AbaqusInvalidNTENSValue&) = default;

  AbaqusInvalidNTENSValue::~AbaqusInvalidNTENSValue() noexcept = default;

  AbaqusInvalidDimension::AbaqusInvalidDimension(const std::string& b,
                                                 const unsigned short N)
      : AbaqusException("''" + b + "' can't be used in " +
                        std::to_string(static_cast<unsigned int>(N)) + "D") {
  }  // end of AbaqusInvalidDimension::AbaqusInvalidDimension

  AbaqusInvalidDimension::AbaqusInvalidDimension(
      const AbaqusInvalidDimension&) = default;

  AbaqusInvalidDimension::AbaqusInvalidDimension(AbaqusInvalidDimension&&) =
      default;

  AbaqusInvalidDimension::~AbaqusInvalidDimension() noexcept = default;

}  // end of namespace abaqus
