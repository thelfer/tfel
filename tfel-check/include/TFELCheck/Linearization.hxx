/*!
 * \file Linearization.hxx
 *
 *  Created on: 13 mai 2013
 *      Author: rp238441
 *
 *  \class Linearization
 *  \brief Class that makes that stores values for a linear interpolation
 *
 *  Class that makes that stores values for a linear interpolation based on
 * TFEL's MLPITestEvolution \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D.
 * All rights reserved. This project is publicly released under either the GNU
 * GPL Licence or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LINEARIZATION_HH_
#define LINEARIZATION_HH_

#include <map>
#include <vector>
#include "TFELCheck/TFELCheckConfig.hxx"

namespace tfel_check {

  struct TFELCHECK_VISIBILITY_EXPORT Linearization {
    //! constructor
    Linearization();
    /*!
     * \param[in] times
     * \param[in] values
     */
    Linearization(const std::vector<double>&, const std::vector<double>&);
    //! destructor
    ~Linearization();
    /*!
     * \return the value at time in parameter
     */
    double operator()(const double x) const;

    /*!
     * \return true if the evolution
     * is constant
     */
    bool isConstant(void) const;

   private:
    std::map<double, double> values; /*< the times and values */
  };

} /* namespace tfel_check */
#endif /* LINEARIZATION_HH_ */
