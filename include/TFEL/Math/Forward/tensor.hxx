/*!
 * \file  include/TFEL/Math/Forward/tensor.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 04 juil. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FORWARD_TENSOR_HXX
#define LIB_TFEL_MATH_FORWARD_TENSOR_HXX

#include "TFEL/Math/General/MathObjectTraits.hxx"

namespace tfel::math {

  /*
   * \class tensor
   * \brief non symmetric tensor.
   * \param N: the spatial dimension, see TensorDimeToSize for details.
   * \param T: numerical type used, by default, double
   * \pre   This class is only defined for N=1u,2u and 3u.
   * \see   TensorDimeToSize and TensorSizeToDime.
   */
  template <unsigned short N, typename T = double>
  struct tensor;

  /*!
   * \brief partial specialisation of the `MathObjectTraits` class.
   * \tparam N: space dimension
   * \tparam T: numeric type
   */
  template <unsigned short N, typename T>
  struct MathObjectTraits<tensor<N, T>>
      : MathObjectTraitsBase<T, unsigned short> {
    //! \brief space dimension
    static constexpr unsigned short dime = N;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FORWARD_TENSOR_HXX */
