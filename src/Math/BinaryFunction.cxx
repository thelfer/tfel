/*!
 * \file   src/Math/BinaryFunction.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cerrno>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/Parser/BinaryFunction.hxx"

namespace tfel {
  namespace math {

    namespace parser {

      void StandardBinaryFunctionBase::
          throwUnimplementedDifferentiateFunctionException() {
        raise(
            "StandardBinaryFunctionBase::"
            "throwUnimplementedDifferentiateFunctionException: "
            "unimplemented feature");
      }  // end of
         // StandardBinaryFunctionBase::throwUnimplementedDifferentiateFunctionException

      void StandardBinaryFunctionBase::throwInvalidCallException(const int e) {
        raise(
            "StandardBinaryFunctionBase::"
            "throwInvalidCallException: "
            "call to function failed "
            "(" +
            std::string(strerror(e)) + ")");
      }  // end of struct StandardBinaryFunctionBase::throwInvalidCallException

      std::string StandardBinaryFunctionBase::getCxxFormula(
          const char* const n, const std::string& e1, const std::string& e2) {
        return std::string(n) + '(' + e1 + ',' + e2 + ')';
      }  // end of StandardBinaryFunctionBase::getCxxFormula

      BinaryFunction::~BinaryFunction() = default;

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel
