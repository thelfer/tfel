/*! 
 * \file  FactorizedKriging1D1D.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 mai 2010
 */

#ifndef   _LIB_TFEL_MATH_FACTORIZEDKRIGING1D1D_H_
#define   _LIB_TFEL_MATH_FACTORIZEDKRIGING1D1D_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/Kriging/KrigingUtilities.hxx"
#include"TFEL/Math/FactorizedKriging.hxx"
#include"TFEL/Math/Kriging/KrigingPieceWiseLinearModel1D.hxx"

namespace tfel
{

  namespace math
  {

    struct TFEL_VISIBILITY_EXPORT FactorizedKriging1D1D
      : private FactorizedKriging<1u,1u,double,
				  KrigingPieceWiseLinearModel1D<double>,
				  KrigingModelAdaptator<KrigingDefaultModel<1u,double> > >,
	private KrigingUtilities
    {
      
      FactorizedKriging1D1D(const std::vector<double>&,
			    const std::vector<double>&,
			    const std::vector<double>&);
      
      FactorizedKriging1D1D(const tfel::math::vector<double>&,
			    const tfel::math::vector<double>&,
			    const tfel::math::vector<double>&);
      
      double
      operator()(const double,
		 const double) const;
      
      ~FactorizedKriging1D1D();

    private:

      //! normalisation coefficient
      double a0;
      //! normalisation coefficient
      double b0;
      //! normalisation coefficient
      double a1;
      //! normalisation coefficient
      double b1;

    }; // end of struct FactorizedKriging1D1D

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_FACTORIZEDKRIGING1D1D_H */

