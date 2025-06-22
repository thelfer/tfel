/*!
 * \file  include/TFEL/Math/FactorizedKriging1D3D.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 mai 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FACTORIZEDKRIGING1D3D_H_
#define LIB_TFEL_MATH_FACTORIZEDKRIGING1D3D_H_

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/Kriging/KrigingUtilities.hxx"
#include "TFEL/Math/FactorizedKriging.hxx"
#include "TFEL/Math/Kriging/KrigingPieceWiseLinearModel1D.hxx"

namespace tfel {

  namespace math {

    struct TFELMATHKRIGING_VISIBILITY_EXPORT FactorizedKriging1D3D
        : private FactorizedKriging<
              1u,
              3u,
              double,
              KrigingPieceWiseLinearModel1D<double>,
              KrigingModelAdaptator<KrigingDefaultModel<3u, double>>>,
          private KrigingUtilities {
      FactorizedKriging1D3D(const std::vector<double>&,
                            const std::vector<double>&,
                            const std::vector<double>&,
                            const std::vector<double>&,
                            const std::vector<double>&);

      FactorizedKriging1D3D(const tfel::math::vector<double>&,
                            const tfel::math::vector<double>&,
                            const tfel::math::vector<double>&,
                            const tfel::math::vector<double>&,
                            const tfel::math::vector<double>&);

      double operator()(const double,
                        const double,
                        const double,
                        const double) const;

      ~FactorizedKriging1D3D();

     private:
      //! a simple alias
      using FK = FactorizedKriging<
          1u,
          3u,
          double,
          KrigingPieceWiseLinearModel1D<double>,
          KrigingModelAdaptator<KrigingDefaultModel<3u, double>>>;
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
      //! normalisation coefficient
      double a3;
      //! normalisation coefficient
      double b3;

    };  // end of struct FactorizedKriging1D3D

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_FACTORIZEDKRIGING1D3D_H */
