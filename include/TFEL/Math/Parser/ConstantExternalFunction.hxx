/*!
 * \file  include/TFEL/Math/Parser/ConstantExternalFunction.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 mars 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_PARSER_CONSTANTEXTERNALFUNCTION_HXX
#define LIB_TFEL_MATH_PARSER_CONSTANTEXTERNALFUNCTION_HXX

#include <vector>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Parser/ExternalFunction.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      struct TFELMATHPARSER_VISIBILITY_EXPORT ConstantExternalFunction final
          : public ExternalFunction {
        ConstantExternalFunction(const double);
        virtual double getValue() const override;
        virtual void setVariableValue(const std::vector<double>::size_type,
                                      const double) override;
        virtual std::vector<double>::size_type getNumberOfVariables()
            const override;
        virtual void checkCyclicDependency(const std::string&) const override;
        virtual void checkCyclicDependency(
            std::vector<std::string>&) const override;
        virtual std::shared_ptr<ExternalFunction> differentiate(
            const std::vector<double>::size_type) const override;
        virtual std::shared_ptr<ExternalFunction> differentiate(
            const std::string&) const override;
        virtual std::shared_ptr<ExternalFunction> resolveDependencies()
            const override;
        virtual std::shared_ptr<ExternalFunction>
        createFunctionByChangingParametersIntoVariables(
            const std::vector<std::string>&) const override;
        virtual std::shared_ptr<ExternalFunction>
        createFunctionByChangingParametersIntoVariables(
            std::vector<std::string>&,
            const std::vector<double>&,
            const std::vector<std::string>&,
            const std::map<std::string, std::vector<double>::size_type>&)
            const override;
        virtual void getParametersNames(std::set<std::string>&) const override;
        virtual ~ConstantExternalFunction();

       protected:
        ConstantExternalFunction& operator=(const ConstantExternalFunction&) =
            delete;
        ConstantExternalFunction& operator=(ConstantExternalFunction&&) =
            delete;
        const double value;
      };  // end of struct ConstantExternalFunction

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_PARSER_CONSTANTEXTERNALFUNCTION_HXX */
