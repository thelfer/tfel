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

      template<double (*f)(double,gsl_mode_t)>
      double gsl_wrapper(double);

      template<double (*f)(double,double,gsl_mode_t)>
      double gsl_wrapper2(double,double);

      template<double (*f)(double,double,double,gsl_mode_t)>
      double gsl_wrapper3(double,double,double);

      template<double (*f)(double,double,double,
			   double,gsl_mode_t)>
      double gsl_wrapper4(double,double,double,double);

    } // end of namespace math
    
  } // end of namespace math

} // end of namespace tfel

#include"Math/Parser/EvaluatorGSLWrapper.ixx"

#endif /* _LIB_TFEL_MATH_EVALUATORWRAPPER_HXX_ */
