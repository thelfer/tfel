/*!
 * \file   mfront/include/MFront/GenericBehaviour/RotateTangentOperatorBlock.hxx
 * \brief  This file declares the `rotateTangentOperatorBlock` function.
 * \date   20/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICBEHAVIOUR_ROTATETANGENTOPERATORBLOCK_HXX
#define LIB_MFRONT_GENERICBEHAVIOUR_ROTATETANGENTOPERATORBLOCK_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"

namespace mfront::gb {

  /*!
   * \brief rotate a tangent operator block, i.e. the derivative of an object
   * of type `Type1` with respect to an object of type `Type2`, stored by
   * rows. Each object is a vector, a symmetric tensor or a tensor.
   *
   * The rotated block is \f$Q_{1}\,K\,Q_{2}^{T}\f$, where \f$Q_{i}\f$ is the
   * matrix of the rotation of the objects of type `Type_i` by
   * `tfel::math::change_basis`, which is orthogonal in the storage of TFEL:
   * the rows of the block are rotated as objects of type `Type2`, then its
   * columns are rotated as objects of type `Type1`.
   *
   * This function is meant to rotate the blocks for which TFEL has no type
   * with a `change_basis` function (derivative of a vector with respect to a
   * vector, a symmetric tensor or a tensor and conversely, derivative of a
   * tensor with respect to a symmetric tensor).
   *
   * \tparam Type1: type of the derivated object
   * \tparam Type2: type of the object with respect to which the derivative
   * is taken
   * \param[out] dest: rotated block, which may be the source
   * \param[in] src: block
   * \param[in] r: rotation matrix
   */
  template <typename Type1, typename Type2, typename real>
  TFEL_HOST_DEVICE constexpr void rotateTangentOperatorBlock(
      real* const dest,
      const real* const src,
      const tfel::math::rotation_matrix<real>& r) noexcept {
    constexpr auto s1 = static_cast<unsigned short>(Type1{}.size());
    constexpr auto s2 = static_cast<unsigned short>(Type2{}.size());
    // the block is copied, so that the destination may be the source
    auto k = tfel::math::tmatrix<s1, s2, real>{src};
    for (unsigned short i = 0; i != s1; ++i) {
      auto row = Type2{};
      for (unsigned short j = 0; j != s2; ++j) {
        row[j] = k(i, j);
      }
      row = tfel::math::change_basis(row, r);
      for (unsigned short j = 0; j != s2; ++j) {
        k(i, j) = row[j];
      }
    }
    for (unsigned short j = 0; j != s2; ++j) {
      auto column = Type1{};
      for (unsigned short i = 0; i != s1; ++i) {
        column[i] = k(i, j);
      }
      column = tfel::math::change_basis(column, r);
      for (unsigned short i = 0; i != s1; ++i) {
        dest[i * s2 + j] = column[i];
      }
    }
  }  // end of rotateTangentOperatorBlock

}  // end of namespace mfront::gb

#endif /* LIB_MFRONT_GENERICBEHAVIOUR_ROTATETANGENTOPERATORBLOCK_HXX */
