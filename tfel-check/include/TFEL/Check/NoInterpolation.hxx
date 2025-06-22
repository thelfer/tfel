/*!
 * \file NoInterpolation.hxx
 *
 *  Created on: 14 mai 2013
 *      Author: rp238441
 *
 *  \class NoInterpolation
 *  \brief Class that prevents to do interpolation
 *
 *  Class that prevents to do interpolation if user doesn't specify the
 * interpolation or if specifies interpolation of type None
 *
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_NOINTERPOLATION_HXX
#define LIB_TFELCHECK_NOINTERPOLATION_HXX

#include "TFEL/Check/TFELCheckConfig.hxx"
#include "TFEL/Check/Interpolation.hxx"

namespace tfel::check {

  struct TFELCHECK_VISIBILITY_EXPORT NoInterpolation : public Interpolation {
    NoInterpolation();
    NoInterpolation(NoInterpolation&&);
    NoInterpolation(const NoInterpolation&);
    NoInterpolation& operator=(NoInterpolation&&);
    NoInterpolation& operator=(const NoInterpolation&);
    void interpolate(const std::vector<double>&,
                     const std::vector<double>&) override;
    double getValue(const double) const override;
    std::string getType() const override;
    bool isConform() const override;
    std::shared_ptr<Interpolation> clone() const override;
    ~NoInterpolation() override;
  };

}  // end of namespace tfel::check

#endif /* LIB_TFELCHECK_NOINTERPOLATION_HXX */
