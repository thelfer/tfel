/*!
 * \file LinearInterpolation.hxx
 *
 *  Created on: 7 mai 2013
 *      Author: rp238441
 *
 *  \class LinearInterpolation
 *  \brief Class that allows to do a linear interpolation
 *
 *  Class that allows to do a linear interpolation using a Linearization object
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_LINEARINTERPOLATION_HXX
#define LIB_TFELCHECK_LINEARINTERPOLATION_HXX

#include "TFEL/Check/TFELCheckConfig.hxx"
#include "TFEL/Check/Interpolation.hxx"
#include "TFEL/Check/Linearization.hxx"

namespace tfel::check {

  struct TFELCHECK_VISIBILITY_EXPORT LinearInterpolation
      : public Interpolation {
    LinearInterpolation();
    LinearInterpolation(LinearInterpolation&&);
    LinearInterpolation(const LinearInterpolation&);
    LinearInterpolation& operator=(LinearInterpolation&&);
    LinearInterpolation& operator=(const LinearInterpolation&);
    void interpolate(const std::vector<double>&,
                     const std::vector<double>&) override;
    double getValue(const double) const override;
    std::string getType() const override;
    bool isConform() const override;
    std::shared_ptr<Interpolation> clone() const override;
    ~LinearInterpolation() override;

   private:
    Linearization linear;
  };

}  // end of namespace tfel::check

#endif /* LIB_TFELCHECK_LINEARINTERPOLATION_HXX */
