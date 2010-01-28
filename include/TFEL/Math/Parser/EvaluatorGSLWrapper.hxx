/*!
 * \file   EvaluatorGSLWrapper.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   15 jan 2009
 */

#ifndef _LIB_TFEL_MATH_EVALUATORWRAPPER_HXX_
#define _LIB_TFEL_MATH_EVALUATORWRAPPER_HXX_ 

#include<stdexcept>
#include<gsl/gsl_sf.h>
#include<gsl/gsl_errno.h>

namespace tfel
{

  namespace math
  {

    namespace parser
    {
	
      struct GSLException
	: public std::runtime_error
      {
	GSLException(const std::string&);
      }; // end of struct GSLException

      void
      gslErrorHandler(const char *,
		      const char *,
		      int, int);

      gsl_mode_t&
      getGSLMode(void);
      
      void
      registerGSLFunctions(void);

      extern "C" {
	typedef double (*GSLFunctionPtr)(double,gsl_mode_t);
	typedef double (*GSLFunction2Ptr)(double,double,gsl_mode_t);
	typedef double (*GSLFunction3Ptr)(double,double,double,gsl_mode_t);
	typedef double (*GSLFunction4Ptr)(double,double,double,double,gsl_mode_t);
      }

      template<GSLFunctionPtr f>
      double gsl_wrapper(double);

      template<GSLFunction2Ptr f>
      double gsl_wrapper2(double,double);

      template<GSLFunction3Ptr f>
      double gsl_wrapper3(double,double,double);

      template<GSLFunction4Ptr f>
      double gsl_wrapper4(double,double,double,double);

    } // end of namespace math
    
  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Parser/EvaluatorGSLWrapper.ixx"

#endif /* _LIB_TFEL_MATH_EVALUATORWRAPPER_HXX_ */
