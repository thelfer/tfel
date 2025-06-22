/*!
 * \file   include/TFEL/Math/Parser/ExternalCastemFunction.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   02/11/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_PARSER_EXTERNALCASTEMFUNCTION_HXX
#define LIB_TFEL_MATH_PARSER_EXTERNALCASTEMFUNCTION_HXX

#include <vector>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "TFEL/Math/Parser/ExternalFunction.hxx"

namespace tfel::math::parser {

  /*!
   * This class handles a function written with the castem
   * interface.
   */
  struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalCastemFunction final
      : public ExternalFunction {
    /*!
     * \brief main constructor of the class
     * \param[in] f_ : pointer to Cast3M function
     * \param[in] n  : number of variables
     */
    ExternalCastemFunction(const tfel::system::CastemFunctionPtr,
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
    ~ExternalCastemFunction() override;

   private:
    std::vector<double> variables;
    tfel::system::CastemFunctionPtr f;
  };  // end of struct ExternalCastemFunction

}  // end of namespace tfel::math::parser

#endif /* LIB_TFEL_MATH_PARSER_EXTERNALCASTEMFUNCTION_HXX */
