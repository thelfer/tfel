/*!
 * \file  src/Math/KrigedFunction.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 02 fév 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/Parser/KrigedFunction.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      void
      KrigedFunctionBase::throwUnimplementedDifferentiateFunctionException() {
        raise(
            "KrigedFunctionBase::"
            "throwUnimplementedDifferentiateFunctionException: "
            "unimplemented feature");
      }  // end of
         // KrigedFunctionBase::throwUnimplementedDifferentiateFunctionException

      void KrigedFunctionBase::
          throwInvalidCreateFunctionByChangingParametersIntoVariables() {
        raise(
            "KrigedFunctionException::"
            "throwInvalidCreateFunctionByChangingParametersIntoVariables: "
            "invalid call");
      }  // end of
         // KrigedFunctionBase::throwInvalidCreateFunctionByChangingParametersIntoVariables()

      void KrigedFunctionBase::throwInvalidIndexException(
          const std::vector<double>::size_type i, const unsigned short N) {
        raise(
            "KrigedFunctionBase::"
            "throwInvalidIndexException: "
            "invalid index (" +
            std::to_string(i) + " > " + std::to_string(N) + ")");
      }  // end of KrigedFunctionBase::throwInvalidIndexException

      template <>
      void KrigedFunction<1u>::setVariableValue(
          const std::vector<double>::size_type i, const double v_) {
        if (i > 0) {
          KrigedFunctionBase::throwInvalidIndexException(i, 0);
        }
        this->v = v_;
      }  // end of KrigedFunction<1u>::setVariableValue

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel
