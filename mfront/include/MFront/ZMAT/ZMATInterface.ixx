/*!
 * \file  mfront/include/MFront/ZMAT/ZMATInterface.ixx
 * \brief
 * \author Thomas Helfer
 * \brief 26 mai 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ZMAT_ZMATINTERFACE_IXX
#define LIB_MFRONT_ZMAT_ZMATINTERFACE_IXX

namespace zmat {

  inline void ZMATInterface::convert(tfel::math::stensor<1u, double>& dest,
                                     const double* const src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
  }  // end of ZMATInterface::convert

  inline void ZMATInterface::convert(tfel::math::stensor<2u, double>& dest,
                                     const double* const src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
  }  // end of ZMATInterface::convert

  inline void ZMATInterface::convert(tfel::math::stensor<3u, double>& dest,
                                     const double* const src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
    dest[4] = src[5];
    dest[5] = src[4];
  }  // end of ZMATInterface::convert

  inline void ZMATInterface::convert(
      double* const dest, const tfel::math::stensor<1u, double>& src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
  }  // end of ZMATInterface::convert

  inline void ZMATInterface::convert(
      double* const dest, const tfel::math::stensor<2u, double>& src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
  }  // end of ZMATInterface::convert

  inline void ZMATInterface::convert(
      double* const dest, const tfel::math::stensor<3u, double>& src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
    dest[4] = src[5];
    dest[5] = src[4];
  }  // end of ZMATInterface::convert

  inline void ZMATInterface::convert(
      ZSET::MATRIX& dest, const tfel::math::st2tost2<1u, double>& src) {
    dest(0, 0) = src(0, 0);
    dest(1, 0) = src(1, 0);
    dest(2, 0) = src(2, 0);
    dest(0, 1) = src(0, 1);
    dest(1, 1) = src(1, 1);
    dest(2, 1) = src(2, 1);
    dest(0, 2) = src(0, 2);
    dest(1, 2) = src(1, 2);
    dest(2, 2) = src(2, 2);
  }

  inline void ZMATInterface::convert(
      ZSET::MATRIX& dest, const tfel::math::st2tost2<2u, double>& src) {
    dest(0, 0) = src(0, 0);
    dest(1, 0) = src(1, 0);
    dest(2, 0) = src(2, 0);
    dest(3, 0) = src(3, 0);
    dest(0, 1) = src(0, 1);
    dest(1, 1) = src(1, 1);
    dest(2, 1) = src(2, 1);
    dest(3, 1) = src(3, 1);
    dest(0, 2) = src(0, 2);
    dest(1, 2) = src(1, 2);
    dest(2, 2) = src(2, 2);
    dest(3, 2) = src(3, 2);
    dest(0, 3) = src(0, 3);
    dest(1, 3) = src(1, 3);
    dest(2, 3) = src(2, 3);
    dest(3, 3) = src(3, 3);
  }  // end of ZMATInterface::convert

  inline void ZMATInterface::convert(
      ZSET::MATRIX& dest, const tfel::math::st2tost2<3u, double>& src) {
    dest(0, 0) = src(0, 0);
    dest(1, 0) = src(1, 0);
    dest(2, 0) = src(2, 0);
    dest(3, 0) = src(3, 0);
    dest(4, 0) = src(5, 0);
    dest(5, 0) = src(4, 0);
    dest(0, 1) = src(0, 1);
    dest(1, 1) = src(1, 1);
    dest(2, 1) = src(2, 1);
    dest(3, 1) = src(3, 1);
    dest(4, 1) = src(5, 1);
    dest(5, 1) = src(4, 1);
    dest(0, 2) = src(0, 2);
    dest(1, 2) = src(1, 2);
    dest(2, 2) = src(2, 2);
    dest(3, 2) = src(3, 2);
    dest(4, 2) = src(5, 2);
    dest(5, 2) = src(4, 2);
    dest(0, 3) = src(0, 3);
    dest(1, 3) = src(1, 3);
    dest(2, 3) = src(2, 3);
    dest(3, 3) = src(3, 3);
    dest(4, 3) = src(5, 3);
    dest(5, 3) = src(4, 3);
    dest(0, 4) = src(0, 5);
    dest(1, 4) = src(1, 5);
    dest(2, 4) = src(2, 5);
    dest(3, 4) = src(3, 5);
    dest(4, 4) = src(5, 5);
    dest(5, 4) = src(4, 5);
    dest(0, 5) = src(0, 4);
    dest(1, 5) = src(1, 4);
    dest(2, 5) = src(2, 4);
    dest(3, 5) = src(3, 4);
    dest(4, 5) = src(5, 4);
    dest(5, 5) = src(4, 4);
  }  // end of ZMATInterface::convert

  inline void ZMATInterface::convert(tfel::math::st2tost2<1u, double>& dest,
                                     const ZSET::MATRIX& src) {
    dest(0, 0) = src(0, 0);
    dest(1, 0) = src(1, 0);
    dest(2, 0) = src(2, 0);
    dest(0, 1) = src(0, 1);
    dest(1, 1) = src(1, 1);
    dest(2, 1) = src(2, 1);
    dest(0, 2) = src(0, 2);
    dest(1, 2) = src(1, 2);
    dest(2, 2) = src(2, 2);
  }

  inline void ZMATInterface::convert(tfel::math::st2tost2<2u, double>& dest,
                                     const ZSET::MATRIX& src) {
    dest(0, 0) = src(0, 0);
    dest(1, 0) = src(1, 0);
    dest(2, 0) = src(2, 0);
    dest(3, 0) = src(3, 0);
    dest(0, 1) = src(0, 1);
    dest(1, 1) = src(1, 1);
    dest(2, 1) = src(2, 1);
    dest(3, 1) = src(3, 1);
    dest(0, 2) = src(0, 2);
    dest(1, 2) = src(1, 2);
    dest(2, 2) = src(2, 2);
    dest(3, 2) = src(3, 2);
    dest(0, 3) = src(0, 3);
    dest(1, 3) = src(1, 3);
    dest(2, 3) = src(2, 3);
    dest(3, 3) = src(3, 3);
  }  // end of ZMATInterface::convert

  inline void ZMATInterface::convert(tfel::math::st2tost2<3u, double>& dest,
                                     const ZSET::MATRIX& src) {
    dest(0, 0) = src(0, 0);
    dest(1, 0) = src(1, 0);
    dest(2, 0) = src(2, 0);
    dest(3, 0) = src(3, 0);
    dest(4, 0) = src(5, 0);
    dest(5, 0) = src(4, 0);
    dest(0, 1) = src(0, 1);
    dest(1, 1) = src(1, 1);
    dest(2, 1) = src(2, 1);
    dest(3, 1) = src(3, 1);
    dest(4, 1) = src(5, 1);
    dest(5, 1) = src(4, 1);
    dest(0, 2) = src(0, 2);
    dest(1, 2) = src(1, 2);
    dest(2, 2) = src(2, 2);
    dest(3, 2) = src(3, 2);
    dest(4, 2) = src(5, 2);
    dest(5, 2) = src(4, 2);
    dest(0, 3) = src(0, 3);
    dest(1, 3) = src(1, 3);
    dest(2, 3) = src(2, 3);
    dest(3, 3) = src(3, 3);
    dest(4, 3) = src(5, 3);
    dest(5, 3) = src(4, 3);
    dest(0, 4) = src(0, 5);
    dest(1, 4) = src(1, 5);
    dest(2, 4) = src(2, 5);
    dest(3, 4) = src(3, 5);
    dest(4, 4) = src(5, 5);
    dest(5, 4) = src(4, 5);
    dest(0, 5) = src(0, 4);
    dest(1, 5) = src(1, 4);
    dest(2, 5) = src(2, 4);
    dest(3, 5) = src(3, 4);
    dest(4, 5) = src(5, 4);
    dest(5, 5) = src(4, 4);
  }  // end of ZMATInterface::convert

  inline void ZMATInterface::convert(tfel::math::tensor<1u, double>& dest,
                                     const double* const src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
  }  // end of ZMATInterface::convert

  inline void ZMATInterface::convert(tfel::math::tensor<2u, double>& dest,
                                     const double* const src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
    dest[4] = src[4];
  }  // end of ZMATInterface::convert

  inline void ZMATInterface::convert(tfel::math::tensor<3u, double>& dest,
                                     const double* const src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
    dest[7] = src[4];
    dest[6] = src[5];
    dest[4] = src[6];
    dest[8] = src[7];
    dest[5] = src[8];
  }  // end of ZMATInterface::convert

  inline void ZMATInterface::convert(
      double* const dest, const tfel::math::tensor<1u, double>& src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
  }  // end of ZMATInterface::convert

  inline void ZMATInterface::convert(
      double* const dest, const tfel::math::tensor<2u, double>& src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
    dest[4] = src[4];
  }  // end of ZMATInterface::convert

  inline void ZMATInterface::convert(
      double* const dest, const tfel::math::tensor<3u, double>& src) {
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = src[3];
    dest[4] = src[7];
    dest[5] = src[6];
    dest[6] = src[4];
    dest[7] = src[8];
    dest[8] = src[5];
  }  // end of ZMATInterface::convert

}  // end of namespace zmat

#endif /* LIB_MFRONT_ZMAT_ZMATINTERFACE_IXX */
