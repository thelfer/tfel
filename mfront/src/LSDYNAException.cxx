/*!
 * \file   mfront/src/LSDYNAException.cxx
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

#include "MFront/LSDYNA/LSDYNAException.hxx"

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

namespace lsdyna {

  LSDYNAException::LSDYNAException(const std::string& s)
      : msg(s) {}  // end of LSDYNAException::LSDYNAException

  LSDYNAException::LSDYNAException(LSDYNAException&&) = default;

  LSDYNAException::LSDYNAException(const LSDYNAException&) = default;

  const char* LSDYNAException::what() const noexcept {
    return msg.c_str();
  }  // end of LSDYNAException::what

  std::string LSDYNAException::getMsg() const noexcept {
    return msg;
  }  // end of LSDYNAException::getMsg

  LSDYNAException::~LSDYNAException() noexcept = default;

  LSDYNAInvalidModellingHypothesis::LSDYNAInvalidModellingHypothesis(
      LSDYNAInvalidModellingHypothesis&&) = default;

  LSDYNAInvalidModellingHypothesis::LSDYNAInvalidModellingHypothesis(
      const LSDYNAInvalidModellingHypothesis&) = default;

  LSDYNAInvalidModellingHypothesis::LSDYNAInvalidModellingHypothesis(
      const char* b)
      : LSDYNAException(
            "current modelling hypothesis is not supported by behaviour '" +
            std::string(b) + "'") {}

  LSDYNAInvalidModellingHypothesis::
      ~LSDYNAInvalidModellingHypothesis() noexcept = default;

  LSDYNAInvalidNTENSValue::LSDYNAInvalidNTENSValue(const unsigned short N)
      : LSDYNAException("Invalid tensor size declared '" +
                        std::to_string(static_cast<unsigned int>(N)) + "'") {
  }  // end of LSDYNAInvalidNTENSValue::LSDYNAInvalidNTENSValue

  LSDYNAInvalidNTENSValue::LSDYNAInvalidNTENSValue(LSDYNAInvalidNTENSValue&&) =
      default;

  LSDYNAInvalidNTENSValue::LSDYNAInvalidNTENSValue(
      const LSDYNAInvalidNTENSValue&) = default;

  LSDYNAInvalidNTENSValue::~LSDYNAInvalidNTENSValue() noexcept = default;

  LSDYNAInvalidDimension::LSDYNAInvalidDimension(const std::string& b,
                                                 const unsigned short N)
      : LSDYNAException("''" + b + "' can't be used in " +
                        std::to_string(static_cast<unsigned int>(N)) + "D") {
  }  // end of LSDYNAInvalidDimension::LSDYNAInvalidDimension

  LSDYNAInvalidDimension::LSDYNAInvalidDimension(
      const LSDYNAInvalidDimension&) = default;

  LSDYNAInvalidDimension::LSDYNAInvalidDimension(LSDYNAInvalidDimension&&) =
      default;

  LSDYNAInvalidDimension::~LSDYNAInvalidDimension() noexcept = default;

}  // end of namespace lsdyna
