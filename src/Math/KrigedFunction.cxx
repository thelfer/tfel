/*!
 * \file  src/Math/KrigedFunction.cxx
 * \brief
 * \author Helfer Thomas
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

#include "TFEL/Math/Parser/KrigedFunction.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      void
      KrigedFunctionBase::throwUnimplementedDifferentiateFunctionException() {
        using namespace std;
        string msg("KrigedFunctionBase::");
        msg += "throwUnimplementedDifferentiateFunctionException : ";
        msg += "unimplemented feature";
        throw(runtime_error(msg));
      }  // end of
         // KrigedFunctionBase::throwUnimplementedDifferentiateFunctionException

      void KrigedFunctionBase::
          throwInvalidCreateFunctionByChangingParametersIntoVariables() {
        using namespace std;
        string msg("KrigedFunctionException::");
        msg += "throwInvalidCreateFunctionByChangingParametersIntoVariables : ";
        msg += "invalid call";
        throw(runtime_error(msg));
      }  // end of
         // KrigedFunctionBase::throwInvalidCreateFunctionByChangingParametersIntoVariables(void)

      void KrigedFunctionBase::throwInvalidIndexException(
          const std::vector<double>::size_type i, const unsigned short N) {
        using namespace std;
        ostringstream msg;
        msg << "KrigedFunctionBase::throwInvalidIndexException : invalid index "
               "("
            << i << " >  " << N << ")";
        throw(runtime_error(msg.str()));
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
