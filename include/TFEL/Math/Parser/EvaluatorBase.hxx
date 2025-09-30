/*!
 * \file   include/TFEL/Math/Parser/EvaluatorBase.hxx
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

#ifndef LIB_TFEL_MATH_PARSER_EVALUATORBASE_HXX
#define LIB_TFEL_MATH_PARSER_EVALUATORBASE_HXX

#include <vector>
#include <string>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::math::parser {

  struct TFELMATHPARSER_VISIBILITY_EXPORT EvaluatorBase {
   protected:
    EvaluatorBase() = default;
    EvaluatorBase(const EvaluatorBase&) = default;
    EvaluatorBase(EvaluatorBase&&) = default;
    EvaluatorBase& operator=(const EvaluatorBase&) = default;
    EvaluatorBase& operator=(EvaluatorBase&&) = default;
    void analyse(const std::string&);
    ~EvaluatorBase();
    std::vector<std::string> tokens;

   private:
    static TFEL_VISIBILITY_LOCAL void splitAtTokenSeperator(
        std::vector<std::string>&);
    static TFEL_VISIBILITY_LOCAL std::string readNumber(
        std::string::const_iterator&, const std::string::const_iterator);
  };  // end of struct EvaluatorBase

}  // end of namespace tfel::math::parser

#endif /* LIB_TFEL_MATH_PARSER_EVALUATORBASE_HXX */
