/*!
 * \file   Power.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 jan 2007
 */

#include<cmath>
#include"Math/Function/Power.hxx"

namespace tfel{

  namespace math{

    namespace stdfunctions{

      tfel::math::FctSqrt sqrt;

      float 
      pow(const float a,const float b)
      {
	using namespace std;
	return pow(a,b);
      }
      
      double
      pow(const double a,const double b)
      {
	return ::pow(a,b);
      }
      
      long double 
      pow(const long double a,const long double b)
      {
	using namespace std;
	return pow(a,b);
      }

    } // end of namespace stdfunctions

  }  // end of namespace math

}  // end of namespace tfel
