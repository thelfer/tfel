/*!
 * \file   include/TFEL/Config/TFELTypes.hxx
 * \brief  This file implements the Types class
 * \author Thomas Helfer
 * \date   08 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELTYPES_HXX
#define LIB_TFELTYPES_HXX

#include "TFEL/Math/Forward/qt.hxx"
#include "TFEL/Math/Forward/tvector.hxx"
#include "TFEL/Math/Forward/tmatrix.hxx"
#include "TFEL/Math/Forward/tensor.hxx"
#include "TFEL/Math/Forward/stensor.hxx"
#include "TFEL/Math/Forward/t2tost2.hxx"
#include "TFEL/Math/Forward/st2tost2.hxx"
#include "TFEL/Math/Stensor/StensorSizeToDime.hxx"
#include "TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include "TFEL/Config/Internals/TFELTypesInternals.hxx"

namespace tfel::config {

  /*!
   * \brief a class gathering aliases to scalars.
   * \tparam T: numeric type
   * \tparam use_quantities: boolean stating if quantities shall be used
   */
  template <typename T = double, bool use_quantities = true>
  struct ScalarTypes {
    using real = tfel::math::qt<tfel::math::NoUnit, T>;
    typedef tfel::math::qt<tfel::math::Time, T> time;
    typedef tfel::math::qt<tfel::math::Frequency, T> frequency;
    typedef tfel::math::qt<tfel::math::Length, T> length;
    typedef tfel::math::qt<tfel::math::Length, T> displacement;
    typedef tfel::math::qt<tfel::math::InvLength, T> inv_length;
    typedef tfel::math::qt<tfel::math::NoUnit, T> strain;
    typedef tfel::math::qt<tfel::math::Frequency, T> strainrate;
    typedef tfel::math::qt<tfel::math::Force, T> force;
    typedef tfel::math::qt<tfel::math::Stress, T> stress;
    typedef tfel::math::qt<tfel::math::StressRate, T> stressrate;
    typedef tfel::math::qt<tfel::math::Temperature, T> temperature;
    typedef tfel::math::qt<tfel::math::InvTemperature, T> thermalexpansion;
    typedef tfel::math::qt<tfel::math::Density, T> massdensity;
    typedef tfel::math::qt<tfel::math::EnergyDensity, T> energydensity;
    typedef tfel::math::qt<tfel::math::Speed, T> speed;
    using thermalconductivity =
        tfel::math::qt<tfel::math::ThermalConductivity, T>;
  };

  /*!
   * \brief partial specialisation of the `ScalarTypes` class when quantities
   * are not used.
   * \tparam T: numeric type
   */
  template <typename T>
  struct ScalarTypes<T, false> {
    using real = T;
    using time = T;
    using frequency = T;
    using length = T;
    using inv_length = T;
    using displacement = T;
    using strain = T;
    using strainrate = T;
    using force = T;
    using stress = T;
    using stressrate = T;
    using temperature = T;
    using thermalexpansion = T;
    using massdensity = T;
    using energydensity = T;
    using speed = T;
    using thermalconductivity = T;
  };  // end of ScalarTypes<T, false>

  /*!
   * \class Types
   * \brief A set of useful typedef.
   * \param unsigned short N, space dimension.
   * \param typename T, numerical type, double by default.
   * \param bool use_quantities, says if one shall use quantities.
   * \author Thomas Helfer
   * \date   08 Aug 2006
   */
  template <unsigned short N, typename T = double, bool use_quantities = true>
  struct Types : ScalarTypes<T, true> {
    typedef tfel::math::tvector<N, typename ScalarTypes<T, true>::real> TVector;
    typedef tfel::math::stensor<N, typename ScalarTypes<T, true>::real> Stensor;
    typedef tfel::math::stensor<N, typename ScalarTypes<T, true>::frequency>
        FrequencyStensor;
    typedef tfel::math::tvector<N, typename ScalarTypes<T, true>::force>
        ForceTVector;
    typedef tfel::math::stensor<N, typename ScalarTypes<T, true>::stress>
        StressStensor;
    typedef tfel::math::stensor<N, typename ScalarTypes<T, true>::stressrate>
        StressRateStensor;
    typedef tfel::math::tvector<N, typename ScalarTypes<T, true>::length>
        DisplacementTVector;
    typedef tfel::math::stensor<N, tfel::math::qt<tfel::math::NoUnit, T>>
        StrainStensor;
    typedef tfel::math::stensor<N, tfel::math::qt<tfel::math::Frequency, T>>
        StrainRateStensor;
    typedef tfel::math::stensor<N,
                                typename ScalarTypes<T, true>::thermalexpansion>
        ThermalExpansionCoefficientTensor;
    typedef tfel::math::tensor<N, tfel::math::qt<tfel::math::NoUnit, T>> Tensor;
    typedef tfel::math::tensor<N, typename ScalarTypes<T, true>::frequency>
        FrequencyTensor;
    typedef tfel::math::tensor<N, typename ScalarTypes<T, true>::stress>
        StressTensor;
    typedef tfel::math::tensor<N, tfel::math::qt<tfel::math::NoUnit, T>>
        DeformationGradientTensor;
    typedef tfel::math::tensor<N, tfel::math::qt<tfel::math::Frequency, T>>
        DeformationGradientRateTensor;
    typedef tfel::math::st2tost2<N, typename ScalarTypes<T, true>::real>
        Stensor4;
    typedef tfel::math::st2tost2<N, typename ScalarTypes<T, true>::stress>
        StiffnessTensor;
    typedef typename tfel::config::internals::PositionType<N, T, true>::type
        PositionType;
    typedef typename tfel::config::internals::SpatialGradType<N, T, true>::type
        SpatialGradType;
    typedef typename tfel::config::internals::JacobianType<N, T, true>::type
        JacobianType;
    typedef typename tfel::config::internals::InvJacobianType<N, T, true>::type
        InvJacobianType;
    /* Thermal related stuff */
    using TemperatureGradient = typename tfel::config::internals::
        TemperatureGradientType<N, T, true>::type;
    using HeatFluxVector =
        typename tfel::config::internals::HeatFluxVectorType<N, T, true>::type;
    using HeatFlux =
        typename tfel::config::internals::HeatFluxVectorType<N, T, true>::type;
    typedef typename tfel::config::internals::
        ThermalConductivityMatrixType<N, T, true>::type
            ThermalConductivityMatrix;
  };

  /*!
   * \brief partial specialisation when quantities are not used
   */
  template <unsigned short N, typename T>
  struct Types<N, T, false> : ScalarTypes<T, false> {
    typedef tfel::math::tvector<N, T> TVector;
    typedef tfel::math::stensor<N, T> Stensor;
    typedef tfel::math::stensor<N, T> FrequencyStensor;
    typedef tfel::math::tvector<N, T> ForceTVector;
    typedef tfel::math::stensor<N, T> StressStensor;
    typedef tfel::math::stensor<N, T> StressRateStensor;
    typedef tfel::math::tvector<N, T> DisplacementTVector;
    typedef tfel::math::stensor<N, T> StrainStensor;
    typedef tfel::math::stensor<N, T> StrainRateStensor;
    typedef tfel::math::stensor<N, T> ThermalExpansionCoefficientTensor;
    typedef tfel::math::tensor<N, T> Tensor;
    typedef tfel::math::tensor<N, T> FrequencyTensor;
    typedef tfel::math::tensor<N, T> StressTensor;
    typedef tfel::math::tensor<N, T> DeformationGradientTensor;
    typedef tfel::math::tensor<N, T> DeformationGradientRateTensor;
    typedef tfel::math::st2tost2<N, T> Stensor4;
    typedef tfel::math::st2tost2<N, T> StiffnessTensor;
    typedef typename tfel::config::internals::PositionType<N, T, false>::type
        PositionType;
    typedef typename tfel::config::internals::SpatialGradType<N, T, true>::type
        SpatialGradType;
    typedef typename tfel::config::internals::JacobianType<N, T, false>::type
        JacobianType;
    typedef typename tfel::config::internals::InvJacobianType<N, T, false>::type
        InvJacobianType;
    /* Thermal related stuff */
    using TemperatureGradient = typename tfel::config::internals::
        TemperatureGradientType<N, T, false>::type;
    typedef
        typename tfel::config::internals::HeatFluxVectorType<N, T, false>::type
            HeatFluxVector;
    using HeatFlux =
        typename tfel::config::internals::HeatFluxVectorType<N, T, false>::type;
    typedef typename tfel::config::internals::
        ThermalConductivityMatrixType<N, T, false>::type
            ThermalConductivityMatrix;
  };

}  // end of namespace tfel::config

#endif /* LIB_TFELTYPES_HXX */
