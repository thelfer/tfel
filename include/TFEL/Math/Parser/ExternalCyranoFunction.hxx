/*!
 * \file   include/TFEL/Math/Parser/ExternalCyranoFunction.hxx
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

#ifndef LIB_TFEL_MATH_PARSER_EXTERNALCYRANOFUNCTION_HXX
#define LIB_TFEL_MATH_PARSER_EXTERNALCYRANOFUNCTION_HXX

#include <vector>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "TFEL/Math/Parser/ExternalFunction.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      /*!
       * This class handles a function following the cyrano
       * interface for material properties.
       */
      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCyranoFunction final
          : public ExternalFunction {
        /*!
         * \brief main constructor of the class
         * \param[in] f_ : pointer to Cast3M function
         * \param[in] n  : number of variables
         */
        ExternalCyranoFunction(const CyranoMaterialPropertyPtr,
                               const unsigned short);
        void setVariableValue(const std::vector<double>::size_type,
                              const double) override;
        std::vector<double>::size_type getNumberOfVariables() const override;
        double getValue() const override;
        void checkCyclicDependency(const std::string&) const override;
        void checkCyclicDependency(std::vector<std::string>&) const override;
        std::shared_ptr<ExternalFunction> differentiate(
            const std::vector<double>::size_type) const override;
        std::shared_ptr<ExternalFunction> differentiate(
            const std::string&) const override;
        std::shared_ptr<ExternalFunction> resolveDependencies() const override;
        std::shared_ptr<ExternalFunction>
        createFunctionByChangingParametersIntoVariables(
            const std::vector<std::string>&) const override;
        std::shared_ptr<ExternalFunction>
        createFunctionByChangingParametersIntoVariables(
            std::vector<std::string>&,
            const std::vector<double>&,
            const std::vector<std::string>&,
            const std::map<std::string, std::vector<double>::size_type>&)
            const override;
        void getParametersNames(std::set<std::string>&) const override;
        //! \brief destructor
        ~ExternalCyranoFunction() override;

       private:
        std::vector<double> variables;
        CyranoMaterialPropertyPtr f;
      };  // end of struct ExternalCyranoFunction

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_PARSER_EXTERNALCYRANOFUNCTION_HXX */
