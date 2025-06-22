/*!
 * \file   include/TFEL/Math/Parser/ExternalFunction.hxx
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

#ifndef LIB_TFEL_MATH_PARSER_EXTERNALFUNCTION_HXX
#define LIB_TFEL_MATH_PARSER_EXTERNALFUNCTION_HXX

#include <set>
#include <map>
#include <vector>
#include <string>
#include <memory>

#include "TFEL/Config/TFELConfig.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalFunction {
        virtual double getValue() const = 0;
        virtual void setVariableValue(const std::vector<double>::size_type,
                                      const double) = 0;
        virtual std::vector<double>::size_type getNumberOfVariables() const = 0;
        virtual void checkCyclicDependency(const std::string&) const = 0;
        virtual void checkCyclicDependency(std::vector<std::string>&) const = 0;
        virtual std::shared_ptr<ExternalFunction> differentiate(
            const std::vector<double>::size_type) const = 0;
        virtual std::shared_ptr<ExternalFunction> differentiate(
            const std::string&) const = 0;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const = 0;
        virtual std::shared_ptr<ExternalFunction>
        createFunctionByChangingParametersIntoVariables(
            const std::vector<std::string>&) const = 0;
        virtual std::shared_ptr<ExternalFunction>
        createFunctionByChangingParametersIntoVariables(
            std::vector<std::string>&,
            const std::vector<double>&,
            const std::vector<std::string>&,
            const std::map<std::string, std::vector<double>::size_type>&)
            const = 0;
        virtual void getParametersNames(std::set<std::string>&) const = 0;
        virtual ~ExternalFunction();
      };  // end of struct ExternalFunction

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_PARSER_EXTERNALFUNCTION_HXX */
