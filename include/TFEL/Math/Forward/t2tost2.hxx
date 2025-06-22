/*!
 * \file  include/TFEL/Math/Forward/t2tost2.hxx
 * \brief
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FORWARD_T2TOST2_HXX
#define LIB_TFEL_MATH_FORWARD_T2TOST2_HXX

#include "TFEL/Math/General/MathObjectTraits.hxx"

namespace tfel::math {

  /*!
   * \class t2tost2
   * \brief linear operator which turns an unsymmetric tensor to symmetric
   * tensor.
   * \tparam N: the spatial dimension, see StensorDimeToSize for
   * details.
   * \tparam T: numerical type used.
   * \pre This class is only defined for N=1u,2u and 3u.
   * \see StensorDimeToSize and StensorSizeToDime.
   */
  template <unsigned short N, typename T = double>
  struct t2tost2;

  /*
   * \brief partial specialisation of the `MathObjectTraits` class for
   * `t2tost2`.
   * \tparam N: space dimension
   * \tparam T: numeric type
   */
  template <unsigned short N, typename T>
  struct MathObjectTraits<t2tost2<N, T>>
      : MathObjectTraitsBase<T, unsigned short> {
    //! \brief space dimension
    static constexpr unsigned short dime = N;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FORWARD_T2TOST2_HXX */
