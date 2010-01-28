/*!
 * \file   EvaluatorGSLWrapper.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   15 jan 2009
 */

#ifndef _LIB_TFEL_MATH_EVALUATORWRAPPER_IXX_
#define _LIB_TFEL_MATH_EVALUATORWRAPPER_IXX_ 

#include<stdexcept>
#include<gsl/gsl_sf.h>

namespace tfel
{

  namespace math
  {

      namespace parser
      {

	template<GSLFunctionPtr f>
	double gsl_wrapper(double x)
	{
	  return f(x,getGSLMode());
	} // end of gsl_wrapper

	template<GSLFunction2Ptr f>
	double gsl_wrapper2(double x,double y)
	{
	  return f(x,y,getGSLMode());
	} // end of gsl_wrapper2
	
	template<GSLFunction3Ptr f>
	double gsl_wrapper3(double x,double y,double z)
	{
	  return f(x,y,z,getGSLMode());
	} // end of gsl_wrapper3
	
	template<GSLFunction4Ptr f>
	double gsl_wrapper4(double x,double y,double z,double u)
	{
	  return f(x,y,z,u,getGSLMode());
	} // end of gsl_wrapper4

      } // end of namespace parser
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_EVALUATORWRAPPER_IXX_ */
