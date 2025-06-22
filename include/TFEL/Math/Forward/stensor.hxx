/*!
 * \file  include/TFEL/Math/Forward/stensor.hxx
 * \brief This file introduces some forward declaration relative to
 * the stensor class.
 * \author Thomas Helfer
 * \brief 19 sept. 2011
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FORWARD_STENSOR_HXX
#define LIB_TFEL_MATH_FORWARD_STENSOR_HXX

namespace tfel {

  namespace math {

    template <class T>
    struct StensorConcept;

    template <class T>
    struct StensorTraits;

    /*
     * \class stensor
     * \brief finite size symmetric tensor.
     * \tparam N, the spatial dimension, see StensorDimeToSize for details.
     * \tparam T, numerical type used, by default, double
     * \pre   This class is only defined for N=1u,2u and 3u.
     * \see   StensorDimeToSize and StensorSizeToDime.
     */
    template <unsigned short N, typename T = double>
    struct stensor;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_FORWARD_STENSOR_HXX */
