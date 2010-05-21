/*! 
 * \file  Kriging1D.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 mai 2010
 */

#ifndef   _LIB_TFEL_MATH_KRIGING1D_H_
#define   _LIB_TFEL_MATH_KRIGING1D_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/Kriging.hxx"

namespace tfel
{

  namespace math
  {

    struct TFEL_VISIBILITY_EXPORT Kriging1D
      : private Kriging<1u,double>
    {
      
      Kriging1D(const std::vector<double>&,
		const std::vector<double>&);

      Kriging1D(const tfel::math::vector<double>&,
		const tfel::math::vector<double>&);
  
      double
      operator()(const double) const;
      
      ~Kriging1D();

    }; // end of struct Kriging1D

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_KRIGING1D_H */

