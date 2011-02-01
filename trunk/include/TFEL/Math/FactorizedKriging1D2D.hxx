/*! 
 * \file  FactorizedKriging1D2D.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 mai 2010
 */

#ifndef   _LIB_TFEL_MATH_FACTORIZEDKRIGING1D2D_H_
#define   _LIB_TFEL_MATH_FACTORIZEDKRIGING1D2D_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/Kriging/KrigingUtilities.hxx"
#include"TFEL/Math/FactorizedKriging.hxx"
#include"TFEL/Math/Kriging/KrigingPieceWiseLinearModel1D.hxx"

namespace tfel
{

  namespace math
  {

    struct TFEL_VISIBILITY_EXPORT FactorizedKriging1D2D
      : private FactorizedKriging<1u,2u,double,
				  KrigingPieceWiseLinearModel1D<double>,
				  KrigingModelAdaptator<KrigingDefaultModel<2u,double> > >,
      	private KrigingUtilities
    {
      
      FactorizedKriging1D2D(const std::vector<double>&,
			    const std::vector<double>&,
			    const std::vector<double>&,
			    const std::vector<double>&);
      
      FactorizedKriging1D2D(const tfel::math::vector<double>&,
			    const tfel::math::vector<double>&,
			    const tfel::math::vector<double>&,
			    const tfel::math::vector<double>&);
      
      double
      operator()(const double,
		 const double,
		 const double) const;
      
      ~FactorizedKriging1D2D();

    private:

      //! normalisation coefficient
      double a0;
      //! normalisation coefficient
      double b0;
      //! normalisation coefficient
      double a1;
      //! normalisation coefficient
      double b1;
      //! normalisation coefficient
      double a2;
      //! normalisation coefficient
      double b2;

    }; // end of struct FactorizedKriging1D2D

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FACTORIZEDKRIGING1D2D_H */

