/*! 
 * \file  Kriging3D.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 mai 3010
 */

#ifndef   _LIB_TFEL_MATH_KRIGING3D_H_
#define   _LIB_TFEL_MATH_KRIGING3D_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/Kriging.hxx"

namespace tfel
{

  namespace math
  {

    struct TFEL_VISIBILITY_EXPORT Kriging3D
      : private Kriging<3u,double>
    {
      
      Kriging3D(const std::vector<double>&,
		const std::vector<double>&,
		const std::vector<double>&,
		const std::vector<double>&);

      Kriging3D(const tfel::math::vector<double>&,
		const tfel::math::vector<double>&,
		const tfel::math::vector<double>&,
		const tfel::math::vector<double>&);
  
      double
      operator()(const double,
		 const double,
		 const double) const;
      
      ~Kriging3D();

    }; // end of struct Kriging3D

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_KRIGING3D_H */

