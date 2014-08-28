/*!
 * \file   src/Math/power.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 jan 2007
 */

#include<cmath>

#include"TFEL/Math/power.hxx"

namespace tfel{

  namespace math{

    namespace stdfunctions{

      float 
      pow(const float a,const float b)
      {
	using namespace std;
	return std::pow(a,b);
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
	return std::pow(a,b);
      }

    } // end of namespace stdfunctions

  }  // end of namespace math

}  // end of namespace tfel
