/*!
 * \file   src/Math/power.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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
