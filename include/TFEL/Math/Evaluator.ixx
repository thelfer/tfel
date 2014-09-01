/*!
 * \file   include/TFEL/Math/Evaluator.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 jan 2009
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_EVALUATOR_IXX_
#define _LIB_TFEL_MATH_EVALUATOR_IXX_ 

#include"TFEL/Math/Parser/Function.hxx"
#include"TFEL/Math/Parser/BinaryFunction.hxx"

namespace tfel
{

  namespace math
  {

    template<double (*f)(const double)>
    tfel::utilities::shared_ptr<tfel::math::parser::Expr>
    StandardFctGenerator(tfel::utilities::shared_ptr<tfel::math::parser::Expr> e)
    {
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      return shared_ptr<Expr>(new StandardFunction<f>(e));
    } // end of Evaluator::FctGenerator::StandardFctGenerator

    template<double (*f)(const double,const double)>
    tfel::utilities::shared_ptr<tfel::math::parser::Expr>
    StandardBinaryFctGenerator(tfel::utilities::shared_ptr<tfel::math::parser::Expr> e1,
			       tfel::utilities::shared_ptr<tfel::math::parser::Expr> e2)
    {
      using namespace tfel::utilities;
      using namespace tfel::math::parser;
      return shared_ptr<Expr>(new StandardBinaryFunction<f>(e1,e2));
    } // end of Evaluator::FctGenerator::StandardBinaryFctGenerator
  
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_EVALUATOR_IXX */
