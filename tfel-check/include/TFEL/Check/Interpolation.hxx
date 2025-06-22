/*!
 * \file Interpolation.hxx
 *
 *  \date 7 mai 2013
 *  \author Remy Petkantchin
 *
 *  \class Interpolation
 *  \brief Abstract mother class for Spline-, Linear- or No- Interpolation
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_INTERPOLATION_HXX
#define LIB_TFELCHECK_INTERPOLATION_HXX

#include <vector>
#include <string>
#include <memory>
#include "TFEL/Check/TFELCheckConfig.hxx"

namespace tfel::check {

  struct TFELCHECK_VISIBILITY_EXPORT Interpolation {
    Interpolation();
    Interpolation(Interpolation&&);
    Interpolation(const Interpolation&);
    Interpolation& operator=(Interpolation&&);
    Interpolation& operator=(const Interpolation&);
    virtual ~Interpolation();
    /*!
     * \brief stores the values of abscissa and ordinate and does the
     * interpolation
     *
     * \param[in] times
     * \param[in] values
     */
    virtual void interpolate(const std::vector<double>&,
                             const std::vector<double>&) = 0;

    /*!
     * \brief returns the value at time in parameter
     *
     * \param[in] x
     * \return value at time in parameter
     */
    virtual double getValue(const double) const = 0;

    /*!
     * \brief returns the type of interpolation
     *
     * \return string : type of interpolation
     */
    virtual std::string getType() const = 0;

    /*!
     * \brief check if interpolation is usable or not
     *
     * \return true if interpolation type is other than None, otherwise
     * returns false
     */
    virtual bool isConform() const = 0;

    virtual std::shared_ptr<Interpolation> clone() const = 0;
  };

}  // end of namespace tfel::check

#endif /* LIB_TFELCHECK_INTERPOLATION_HXX */
