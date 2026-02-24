/*!
 * \file  include/TFEL/Math/FactorizedKriging1D1D.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 mai 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FACTORIZEDKRIGING1D1D_HXX
#define LIB_TFEL_MATH_FACTORIZEDKRIGING1D1D_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/Kriging/KrigingUtilities.hxx"
#include "TFEL/Math/FactorizedKriging.hxx"
#include "TFEL/Math/Kriging/KrigingPieceWiseLinearModel1D.hxx"

namespace tfel::math {

  struct TFELMATHKRIGING_VISIBILITY_EXPORT FactorizedKriging1D1D
      : private FactorizedKriging<
            1u,
            1u,
            double,
            KrigingPieceWiseLinearModel1D<double>,
            KrigingModelAdaptator<KrigingDefaultModel<1u, double>>>,
        private KrigingUtilities {
    FactorizedKriging1D1D(const std::vector<double>&,
                          const std::vector<double>&,
                          const std::vector<double>&);

    FactorizedKriging1D1D(const tfel::math::vector<double>&,
                          const tfel::math::vector<double>&,
                          const tfel::math::vector<double>&);

    double operator()(const double, const double) const;

    ~FactorizedKriging1D1D();

   private:
    //! a simple alias
    using FK = FactorizedKriging<
        1u,
        1u,
        double,
        KrigingPieceWiseLinearModel1D<double>,
        KrigingModelAdaptator<KrigingDefaultModel<1u, double>>>;
    //! normalisation coefficient
    double a0;
    //! normalisation coefficient
    double b0;
    //! normalisation coefficient
    double a1;
    //! normalisation coefficient
    double b1;

  };  // end of struct FactorizedKriging1D1D

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FACTORIZEDKRIGING1D1D_HXX */
