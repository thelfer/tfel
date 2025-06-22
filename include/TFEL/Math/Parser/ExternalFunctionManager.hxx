/*!
 * \file   include/TFEL/Math/Parser/ExternalFunctionManager.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_PARSER_EXTERNALFUNCTIONMANAGER_HXX_
#define LIB_TFEL_MATH_PARSER_EXTERNALFUNCTIONMANAGER_HXX_

#include <map>
#include <memory>
#include <string>

#include "TFEL/Math/Parser/ExternalFunction.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      struct ExternalFunctionManager
          : public std::map<std::string, std::shared_ptr<ExternalFunction>> {
      };  // end of struct ExternalFunctionManager

      typedef std::shared_ptr<ExternalFunctionManager>
          ExternalFunctionManagerPtr;

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_PARSER_EXTERNALFUNCTIONMANAGER_HXX_ */
