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

#include "TFELCheck/TFELCheckConfig.hxx"
#include "TFELCheck/Interpolation.hxx"

namespace tfel_check {

  struct TFELCHECK_VISIBILITY_EXPORT NoInterpolation : public Interpolation {
    NoInterpolation();
    NoInterpolation(NoInterpolation&&);
    NoInterpolation(const NoInterpolation&);
    NoInterpolation& operator=(NoInterpolation&&);
    NoInterpolation& operator=(const NoInterpolation&);
    virtual ~NoInterpolation();
    virtual void interpolate(const std::vector<double>&,
                             const std::vector<double>&) override;
    virtual double getValue(const double) const override;
    virtual std::string getType() const override;
    virtual bool isConform() const override;
    virtual std::shared_ptr<Interpolation> clone() const override;
  };

} /* namespace tfel_check */

#endif /* LIB_TFELCHECK_NOINTERPOLATION_H_ */
