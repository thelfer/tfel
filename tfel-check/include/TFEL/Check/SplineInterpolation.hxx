/*!
 * SplineInterpolation.hxx
 *
 *  Created on: 7 mai 2013
 *      Author: rp238441
 *
 *  \class SplineInterpolation
 *  \brief Class that allows to do a spline interpolation
 *
 *  Class that allows to do a spline interpolation using TFEL's CubicSpline
 *
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_SPLINEINTERPOLATION_HXX
#define LIB_TFELCHECK_SPLINEINTERPOLATION_HXX

#include "TFEL/Check/TFELCheckConfig.hxx"
#include "TFEL/Math/CubicSpline.hxx"
#include "TFEL/Check/Interpolation.hxx"

namespace tfel {

  namespace check {

    struct TFELCHECK_VISIBILITY_EXPORT SplineInterpolation
        : public Interpolation {
      SplineInterpolation();
      SplineInterpolation(SplineInterpolation&&);
      SplineInterpolation(const SplineInterpolation&);
      SplineInterpolation& operator=(SplineInterpolation&&);
      SplineInterpolation& operator=(const SplineInterpolation&);
      void interpolate(const std::vector<double>&,
                       const std::vector<double>&) override;
      double getValue(const double x) const override;
      std::string getType() const override;
      bool isConform() const override;
      std::shared_ptr<Interpolation> clone() const override;
      ~SplineInterpolation() override;

     private:
      struct tfel::math::CubicSpline<double> spline;
    };

  }  // end of namespace check

}  // end of namespace tfel

#endif /* LIB_TFELCHECK_SPLINEINTERPOLATION_HXX */
