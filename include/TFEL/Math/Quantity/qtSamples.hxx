/*!
 * \file   include/TFEL/Math/Quantity/qtSamples.hxx
 * \brief  This file declares some useful typedefs for usual quantities.
 * \author Thomas Helfer
 * \date   29 jui 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_QTSAMPLES_HXX
#define LIB_TFEL_MATH_QTSAMPLES_HXX

#include "TFEL/Math/Forward/qt.hxx"

namespace tfel {

  namespace math {

    /*!
     * \brief Defines a mass.
     * \see qt, Mass
     */
    typedef qt<Mass> mass;
    /*!
     * \brief Defines a length.
     * \see qt, Length
     */
    typedef qt<Length> length;
    /*!
     * \brief Defines a invlength.
     * \see qt, InvLength
     */
    typedef qt<Length> invlength;
    /*!
     * \brief Defines a time.
     * \see qt, Time
     */
    typedef qt<Time> time;
    /*!
     * \brief Defines an ampere.
     * \see qt, Ampere
     */
    typedef qt<Ampere> ampere;
    /*!
     * \brief Defines a temperature.
     * \see qt, Temperature
     */
    typedef qt<Temperature> temperature;
    /*!
     * \brief Defines a candela.
     * \see qt, Candela
     */
    typedef qt<Candela> candela;
    /*!
     * \brief Defines a mole.
     * \see qt, Mole
     */
    typedef qt<Mole> mole;
    /*!
     * \brief Defines a speed.
     * \see qt, Speed
     */
    typedef qt<Speed> velocity;
    /*!
     * \brief Defines an acceleration.
     * \see qt, Acceleration
     */
    typedef qt<Acceleration> acceleration;
    /*!
     * \brief Defines a momentum.
     * \see qt, Momentum
     */
    typedef qt<Momentum> momentum;
    /*!
     * \brief Defines a force.
     * \see qt, Force
     */
    typedef qt<Force> force;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_QTSAMPLES_HXX */
