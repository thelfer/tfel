/*!
 * \file   include/TFEL/Config/Internals/TFELTypesInternals.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15 Sep 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELTYPESINTERNALS_HXX
#define LIB_TFELTYPESINTERNALS_HXX

#include "TFEL/Math/Forward/qt.hxx"
#include "TFEL/Math/Forward/tvector.hxx"
#include "TFEL/Math/Forward/tmatrix.hxx"
#include "TFEL/Math/Forward/stensor.hxx"

namespace tfel::config::internals {

  template <unsigned short N, typename T, bool use_qt>
  struct PositionType {
    typedef tfel::math::tvector<N, tfel::math::qt<tfel::math::unit::Length, T>>
        type;
  };

  template <unsigned short N, typename T>
  struct PositionType<N, T, false> {
    typedef tfel::math::tvector<N, T> type;
  };

  template <typename T, bool use_qt>
  struct PositionType<1u, T, use_qt> {
    typedef tfel::math::qt<tfel::math::unit::Length,
                           tfel::math::qt<tfel::math::unit::NoUnit, T>>
        type;
  };

  template <typename T>
  struct PositionType<1u, T, false> {
    typedef T type;
  };

  template <unsigned short N, typename T, bool use_qt>
  struct SpatialGradType {
    typedef tfel::math::tvector<N,
                                tfel::math::qt<tfel::math::unit::InvLength, T>>
        type;
  };

  template <unsigned short N, typename T>
  struct SpatialGradType<N, T, false> {
    typedef tfel::math::tvector<N, T> type;
  };

  template <typename T, bool use_qt>
  struct SpatialGradType<1u, T, use_qt> {
    typedef tfel::math::qt<tfel::math::unit::InvLength, T> type;
  };

  template <typename T>
  struct SpatialGradType<1u, T, false> {
    typedef T type;
  };

  template <unsigned short N, typename T, bool use_qt>
  struct JacobianType {
    typedef tfel::math::
        tmatrix<N, N, tfel::math::qt<tfel::math::unit::NoUnit, T>>
            type;
  };

  template <unsigned short N, typename T>
  struct JacobianType<N, T, false> {
    typedef tfel::math::tmatrix<N, N, T> type;
  };

  template <typename T, bool use_qt>
  struct JacobianType<1u, T, use_qt> {
    typedef tfel::math::qt<tfel::math::unit::NoUnit, T> type;
  };

  template <typename T>
  struct JacobianType<1u, T, false> {
    typedef T type;
  };

  template <unsigned short N, typename T, bool use_qt>
  struct InvJacobianType {
    typedef tfel::math::
        tmatrix<N, N, tfel::math::qt<tfel::math::unit::NoUnit, T>>
            type;
  };

  template <unsigned short N, typename T>
  struct InvJacobianType<N, T, false> {
    typedef tfel::math::tmatrix<N, N, T> type;
  };

  template <typename T, bool use_qt>
  struct InvJacobianType<1u, T, use_qt> {
    typedef tfel::math::qt<tfel::math::unit::NoUnit, T> type;
  };

  template <typename T>
  struct InvJacobianType<1u, T, false> {
    typedef T type;
  };

  template <unsigned short N, typename T, bool use_qt>
  struct TemperatureGradientType {
    typedef tfel::math::
        tvector<N, tfel::math::qt<tfel::math::unit::TemperatureGradient, T>>
            type;
  };  // end of struct TemperatureGradientVectorType

  template <unsigned short N, typename T>
  struct TemperatureGradientType<N, T, false> {
    typedef tfel::math::tvector<N, T> type;
  };  // end of struct TemperatureGradientVectorType

  template <unsigned short N, typename T, bool use_qt>
  struct HeatFluxVectorType {
    typedef tfel::math::
        tvector<N, tfel::math::qt<tfel::math::unit::HeatFluxDensity, T>>
            type;
  };

  template <unsigned short N, typename T>
  struct HeatFluxVectorType<N, T, false> {
    typedef tfel::math::tvector<N, T> type;
  };

  template <unsigned short N, typename T, bool use_qt>
  struct ThermalConductivityMatrixType {
    typedef tfel::math::
        tmatrix<N, N, tfel::math::qt<tfel::math::unit::ThermalConductivity, T>>
            type;
  };

  template <unsigned short N, typename T>
  struct ThermalConductivityMatrixType<N, T, false> {
    typedef tfel::math::tmatrix<N, N, T> type;
  };

  template <typename T, bool use_qt>
  struct ThermalConductivityMatrixType<1u, T, use_qt> {
    typedef tfel::math::qt<tfel::math::unit::ThermalConductivity, T> type;
  };

  template <typename T>
  struct ThermalConductivityMatrixType<1u, T, false> {
    typedef T type;
  };

}  // end of namespace tfel::config::internals

#endif /* LIB_TFELTYPESINTERNALS_HXX */
