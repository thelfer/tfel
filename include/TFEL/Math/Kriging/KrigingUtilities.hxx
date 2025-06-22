/*!
 * \file  include/TFEL/Math/Kriging/KrigingUtilities.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 14 aoû 2010
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_KRIGINGUTILITIES_HXX
#define LIB_TFEL_MATH_KRIGINGUTILITIES_HXX

#include <vector>
#include <utility>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/vector.hxx"

namespace tfel {

  namespace math {

    struct TFELMATHKRIGING_VISIBILITY_EXPORT KrigingUtilities {
      static std::pair<double, double> normalize(const std::vector<double>&);

      static std::pair<double, double> normalize(
          const tfel::math::vector<double>&);

    };  // end of struct KrigingUtilities

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_KRIGINGUTILITIES_HXX */
