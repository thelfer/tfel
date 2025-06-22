/*!
 * SplineLocalInterpolation.hxx
 *
 *  Created on: 3 juin 2013
 *      Author: rp238441
 *
 *  \class SplineLocalInterpolation
 *  \brief Class that allows to do a spline interpolation only locally around
 * the missing points
 *
 *  Class that allows to do a spline local interpolation using TFEL's
 * CubicSpline
 *
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_SPLINELOCALINTERPOLATION_HXX
#define LIB_TFELCHECK_SPLINELOCALINTERPOLATION_HXX

#include <map>
#include "TFELCheck/TFELCheckConfig.hxx"
#include "TFEL/Math/CubicSpline.hxx"
#include "TFELCheck/Interpolation.hxx"

namespace tfel_check {

  struct TFELCHECK_VISIBILITY_EXPORT SplineLocalInterpolation
      : public Interpolation {
    SplineLocalInterpolation();
    virtual ~SplineLocalInterpolation();
    virtual void interpolate(const std::vector<double>&,
                             const std::vector<double>&) override;
    virtual double getValue(const double) const override;
    virtual std::string getType() const override;
    virtual bool isConform() const override;
    virtual std::shared_ptr<Interpolation> clone() const override;

   private:
    std::map<double, struct tfel::math::CubicSpline<double>> splines;
    std::vector<double> timesBefore; /**< vector containing the first times of
                                        each group of 3 times **/
  };

} /* namespace tfel_check */

#endif /* LIB_TFELCHECK_SPLINELOCALINTERPOLATION_HXX */
