/*!
 * \file  include/TFEL/Math/Kriging3D.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 mai 3010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_KRIGING3D_HXX
#define LIB_TFEL_MATH_KRIGING3D_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/Kriging.hxx"
#include "TFEL/Math/Kriging/KrigingUtilities.hxx"

namespace tfel {

  namespace math {

    struct TFELMATHKRIGING_VISIBILITY_EXPORT Kriging3D
        : private Kriging<3u, double>,
          private KrigingUtilities {
      Kriging3D(const std::vector<double>&,
                const std::vector<double>&,
                const std::vector<double>&,
                const std::vector<double>&);

      Kriging3D(const tfel::math::vector<double>&,
                const tfel::math::vector<double>&,
                const tfel::math::vector<double>&,
                const tfel::math::vector<double>&);

      double operator()(const double, const double, const double) const;

      ~Kriging3D();

     private:
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

    };  // end of struct Kriging3D

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_KRIGING3D_HXX */
