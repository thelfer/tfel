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

#ifndef LIB_TFELCHECK_LINEARIZATION_HXX
#define LIB_TFELCHECK_LINEARIZATION_HXX

#include <map>
#include <vector>
#include "TFEL/Check/TFELCheckConfig.hxx"

namespace tfel {

  namespace check {

    struct TFELCHECK_VISIBILITY_EXPORT Linearization {
      //! constructor
      Linearization();
      //! move constructor
      Linearization(Linearization&&);
      //! copy constructor
      Linearization(const Linearization&);
      //! move assignement
      Linearization& operator=(Linearization&&);
      //! copy assignement
      Linearization& operator=(const Linearization&);
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
      bool isConstant() const;

     private:
      std::map<double, double> values; /*< the times and values */
    };

  }  // end of namespace check

}  // end of namespace tfel

#endif /* LIB_TFELCHECK_LINEARIZATION_HXX */
