/*!
 * \file  include/TFEL/Math/Kriging2D.hxx
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

#ifndef LIB_TFEL_MATH_KRIGING2D_HXX
#define LIB_TFEL_MATH_KRIGING2D_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/Kriging.hxx"
#include "TFEL/Math/Kriging/KrigingUtilities.hxx"

namespace tfel {

  namespace math {

    struct TFELMATHKRIGING_VISIBILITY_EXPORT Kriging2D
        : private Kriging<2u, double>,
          private KrigingUtilities {
      Kriging2D(const std::vector<double>&,
                const std::vector<double>&,
                const std::vector<double>&);

      Kriging2D(const tfel::math::vector<double>&,
                const tfel::math::vector<double>&,
                const tfel::math::vector<double>&);

      double operator()(const double, const double) const;

      ~Kriging2D();

     private:
      //! normalisation coefficient
      double a1;
      //! normalisation coefficient
      double b1;
      //! normalisation coefficient
      double a2;
      //! normalisation coefficient
      double b2;

    };  // end of struct Kriging2D

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_KRIGING2D_HXX */
