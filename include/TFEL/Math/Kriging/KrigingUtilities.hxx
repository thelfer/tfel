/*! 
 * \file  KrigingUtilities.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 aoû 2010
 */

#ifndef   _LIB_TFEL_MATH_KRIGINGUTILITIES_H_
#define   _LIB_TFEL_MATH_KRIGINGUTILITIES_H_ 

#include<vector>
#include<utility>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/vector.hxx"

namespace tfel
{

  namespace math
  {

    struct TFELMATHKRIGING_VISIBILITY_EXPORT KrigingUtilities
    {
      
      static std::pair<double,double>
      normalize(const std::vector<double>&);

      static std::pair<double,double>
      normalize(const tfel::math::vector<double>&);

    }; // end of struct KrigingUtilities

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_KRIGINGUTILITIES_H */

