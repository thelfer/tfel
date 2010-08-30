/*! 
 * \file  KrigingUtilities.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 aoû 2010
 */

#include<algorithm>
#include<stdexcept>

#include"TFEL/Math/Kriging/KrigingUtilities.hxx"

namespace tfel
{

  namespace math
  {

    static bool
    compareFloatingPointValues(const double a,
			       const double b)
    {
      using namespace std;
      if((abs(a)<10*numeric_limits<double>::min())&&
	 (abs(b)<10*numeric_limits<double>::min())){
	// a and b are almost 0
	return true;
      }
      if((abs(a-b)<abs(a)*10*numeric_limits<double>::epsilon())||
	(abs(a-b)<abs(b)*10*numeric_limits<double>::epsilon())){
	return true;
      }
      return false;
    } // end of compareFloatingPointValues
    
    std::pair<double,double>
    KrigingUtilities::normalize(const std::vector<double>& v)
    {
      using namespace std;
      pair<double,double> r;
      double max = *(max_element(v.begin(),v.end()));
      double min = *(min_element(v.begin(),v.end()));
      if(compareFloatingPointValues(max-min,0.)){
	string msg("KrigingUtilities::normalize : ");
	msg += "values are almost identical";
	throw(runtime_error(msg));
      }
      r.first  = 1./(max-min);
      r.second = -min/r.first;
      return r;
    }

    std::pair<double,double>
    KrigingUtilities::normalize(const tfel::math::vector<double>& v)
    {
      using namespace std;
      pair<double,double> r;
      double max = *(max_element(v.begin(),v.end()));
      double min = *(min_element(v.begin(),v.end()));
      if(compareFloatingPointValues(max-min,0.)){
	string msg("KrigingUtilities::normalize : ");
	msg += "values are almost identical";
	throw(runtime_error(msg));
      }
      r.first  = 1./(max-min);
      r.second = -min/r.first;
      return r;
    } // end of KrigingUtilities::normalize
    
  } // end of namespace math
  
} // end of namespace tfel
