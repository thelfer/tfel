/*! 
 * \file  LevenbergMarquardtExternalFunctionWrapper.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 24 fév 2010
 */

#ifndef _LIB_LEVENBERGMARQUARDTEXTERNALFUNCTIONWRAPPER_H_
#define _LIB_LEVENBERGMARQUARDTEXTERNALFUNCTIONWRAPPER_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/Parser/ExternalFunction.hxx"

namespace tfel
{

  namespace math
  {

    struct TFELMATHPARSER_VISIBILITY_EXPORT LevenbergMarquardtExternalFunctionWrapper
    {
      typedef tfel::math::vector<double>::size_type size_type;
      typedef double NumericType;
      
      LevenbergMarquardtExternalFunctionWrapper(tfel::utilities::shared_ptr<tfel::math::parser::ExternalFunction>,
						const size_type,
						const size_type);
      
      size_type
      getNumberOfVariables(void) const;
      
      size_type
      getNumberOfParameters(void) const;
      
      void
      operator()(double&,tfel::math::vector<double>&,
		 const tfel::math::vector<double>&,
		 const tfel::math::vector<double>&);
      
    private:
      
      tfel::utilities::shared_ptr<tfel::math::parser::ExternalFunction> ev;
      tfel::math::vector<tfel::utilities::shared_ptr<tfel::math::parser::ExternalFunction> > dev;
      size_type nv;
      size_type np;
    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_LEVENBERGMARQUARDTEXTERNALFUNCTIONWRAPPER_H */

