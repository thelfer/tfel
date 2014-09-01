/*! 
 * \file  include/TFEL/Math/Kriging1D.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 mai 2010
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef   _LIB_TFEL_MATH_KRIGING1D_H_
#define   _LIB_TFEL_MATH_KRIGING1D_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/Kriging.hxx"
#include"TFEL/Math/Kriging/KrigingUtilities.hxx"

namespace tfel
{

  namespace math
  {

    struct TFELMATHKRIGING_VISIBILITY_EXPORT Kriging1D
      : private Kriging<1u,double>,
	private KrigingUtilities
    {
      
      Kriging1D(const std::vector<double>&,
		const std::vector<double>&);

      Kriging1D(const tfel::math::vector<double>&,
		const tfel::math::vector<double>&);
  
      double
      operator()(const double) const;
      
      ~Kriging1D();

    private:

      //! normalisation coefficient
      double a;
      //! normalisation coefficient
      double b;

    }; // end of struct Kriging1D

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_KRIGING1D_H */

