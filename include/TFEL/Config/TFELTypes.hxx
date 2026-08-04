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
#include "TFEL/Config/Internals/TFELTypesInternals.hxx"

namespace tfel::config {

  /*!
   * \brief a class gathering aliases to scalars.
   * \tparam T: numeric type
   * \tparam use_quantities: boolean stating if quantities shall be used
   */
  template <typename T = double, bool use_quantities = true>
  struct ScalarTypes {
    using numeric_type = T;
    using real = tfel::math::qt<tfel::math::unit::NoUnit, T>;
    using time = tfel::math::qt<tfel::math::unit::Time, T>;
    using frequency = tfel::math::qt<tfel::math::unit::Frequency, T>;
    using length = tfel::math::qt<tfel::math::unit::Length, T>;
    using displacement = tfel::math::qt<tfel::math::unit::Length, T>;
    using inv_length = tfel::math::qt<tfel::math::unit::InvLength, T>;
    using strain = tfel::math::qt<tfel::math::unit::NoUnit, T>;
    using strainrate = tfel::math::qt<tfel::math::unit::Frequency, T>;
    using force = tfel::math::qt<tfel::math::unit::Force, T>;
    using stress = tfel::math::qt<tfel::math::unit::Stress, T>;
    using compliance =
        tfel::math::qt<tfel::math::unit::StandardUnit<-1, 1, 2, 0, 0, 0, 0>, T>;
    using viscosity =
        tfel::math::qt<tfel::math::unit::StandardUnit<1, -1, -1, 0, 0, 0, 0>,
                       T>;
    using viscouscompliance =
        tfel::math::qt<tfel::math::unit::StandardUnit<-1, 1, 1, 0, 0, 0, 0>, T>;
    using stressrate = tfel::math::qt<tfel::math::unit::StressRate, T>;
    using temperature = tfel::math::qt<tfel::math::unit::Temperature, T>;
    using thermalexpansion =
        tfel::math::qt<tfel::math::unit::InvTemperature, T>;
    using massdensity = tfel::math::qt<tfel::math::unit::Density, T>;
    using energydensity = tfel::math::qt<tfel::math::unit::EnergyDensity, T>;
    using speed = tfel::math::qt<tfel::math::unit::Speed, T>;
    using thermalconductivity =
        tfel::math::qt<tfel::math::unit::ThermalConductivity, T>;
  };

  /*!
   * \brief partial specialisation of the `ScalarTypes` class when quantities
   * are not used.
   * \tparam T: numeric type
   */
  template <typename T>
  struct ScalarTypes<T, false> {
    using numeric_type = T;
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
    using compliance = T;
    using viscosity = T;
    using viscouscompliance = T;
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
   * \brief A set of useful using  = .
   * \param unsigned short N, space dimension.
   * \param typename T, numerical type, double by default.
   * \param bool use_quantities, says if one shall use quantities.
   * \author Thomas Helfer
   * \date   08 Aug 2006
   */
  template <unsigned short N, typename T = double, bool use_quantities = true>
  struct Types : ScalarTypes<T, true> {
    using TVector = tfel::math::tvector<N, typename ScalarTypes<T, true>::real>;
    using Stensor = tfel::math::stensor<N, typename ScalarTypes<T, true>::real>;
    using FrequencyStensor =
        tfel::math::stensor<N, typename ScalarTypes<T, true>::frequency>;
    using ForceTVector =
        tfel::math::tvector<N, typename ScalarTypes<T, true>::force>;
    using StressStensor =
        tfel::math::stensor<N, typename ScalarTypes<T, true>::stress>;
    using StressRateStensor =
        tfel::math::stensor<N, typename ScalarTypes<T, true>::stressrate>;
    using DisplacementTVector =
        tfel::math::tvector<N, typename ScalarTypes<T, true>::length>;
    using StrainStensor =
        tfel::math::stensor<N, tfel::math::qt<tfel::math::unit::NoUnit, T>>;
    using StrainRateStensor =
        tfel::math::stensor<N, tfel::math::qt<tfel::math::unit::Frequency, T>>;
    using ThermalExpansionCoefficientTensor =
        tfel::math::stensor<N, typename ScalarTypes<T, true>::thermalexpansion>;
    using Tensor =
        tfel::math::tensor<N, tfel::math::qt<tfel::math::unit::NoUnit, T>>;
    using FrequencyTensor =
        tfel::math::tensor<N, typename ScalarTypes<T, true>::frequency>;
    using StressTensor =
        tfel::math::tensor<N, typename ScalarTypes<T, true>::stress>;
    using DeformationGradientTensor =
        tfel::math::tensor<N, tfel::math::qt<tfel::math::unit::NoUnit, T>>;
    using DeformationGradientRateTensor =
        tfel::math::tensor<N, tfel::math::qt<tfel::math::unit::Frequency, T>>;
    using Stensor4 =
        tfel::math::st2tost2<N, typename ScalarTypes<T, true>::real>;
    using ComplianceTensor =
        tfel::math::st2tost2<N, typename ScalarTypes<T, true>::compliance>;
    using ViscosityTensor =
        tfel::math::st2tost2<N, typename ScalarTypes<T, true>::viscosity>;
    using ViscousComplianceTensor =
        tfel::math::st2tost2<N,
                             typename ScalarTypes<T, true>::viscouscompliance>;
    using StiffnessTensor =
        tfel::math::st2tost2<N, typename ScalarTypes<T, true>::stress>;
    using PositionType =
        typename tfel::config::internals::PositionType<N, T, true>::type;
    using SpatialGradType =
        typename tfel::config::internals::SpatialGradType<N, T, true>::type;
    using JacobianType =
        typename tfel::config::internals::JacobianType<N, T, true>::type;
    using InvJacobianType =
        typename tfel::config::internals::InvJacobianType<N, T, true>::type;
    /* Thermal related stuff */
    using TemperatureGradient = typename tfel::config::internals::
        TemperatureGradientType<N, T, true>::type;
    using HeatFluxVector =
        typename tfel::config::internals::HeatFluxVectorType<N, T, true>::type;
    using HeatFlux =
        typename tfel::config::internals::HeatFluxVectorType<N, T, true>::type;
    using ThermalConductivityMatrix = typename tfel::config::internals::
        ThermalConductivityMatrixType<N, T, true>::type;
  };

  /*!
   * \brief partial specialisation when quantities are not used
   */
  template <unsigned short N, typename T>
  struct Types<N, T, false> : ScalarTypes<T, false> {
    using TVector = tfel::math::tvector<N, T>;
    using Stensor = tfel::math::stensor<N, T>;
    using FrequencyStensor = tfel::math::stensor<N, T>;
    using ForceTVector = tfel::math::tvector<N, T>;
    using StressStensor = tfel::math::stensor<N, T>;
    using StressRateStensor = tfel::math::stensor<N, T>;
    using DisplacementTVector = tfel::math::tvector<N, T>;
    using StrainStensor = tfel::math::stensor<N, T>;
    using StrainRateStensor = tfel::math::stensor<N, T>;
    using ThermalExpansionCoefficientTensor = tfel::math::stensor<N, T>;
    using Tensor = tfel::math::tensor<N, T>;
    using FrequencyTensor = tfel::math::tensor<N, T>;
    using StressTensor = tfel::math::tensor<N, T>;
    using DeformationGradientTensor = tfel::math::tensor<N, T>;
    using DeformationGradientRateTensor = tfel::math::tensor<N, T>;
    using Stensor4 = tfel::math::st2tost2<N, T>;
    using StiffnessTensor = tfel::math::st2tost2<N, T>;
    using ComplianceTensor = tfel::math::st2tost2<N, T>;
    using ViscosityTensor = tfel::math::st2tost2<N, T>;
    using ViscousComplianceTensor = tfel::math::st2tost2<N, T>;
    using PositionType =
        typename tfel::config::internals::PositionType<N, T, false>::type;
    using SpatialGradType =
        typename tfel::config::internals::SpatialGradType<N, T, true>::type;
    using JacobianType =
        typename tfel::config::internals::JacobianType<N, T, false>::type;
    using InvJacobianType =
        typename tfel::config::internals::InvJacobianType<N, T, false>::type;
    /* Thermal related stuff */
    using TemperatureGradient = typename tfel::config::internals::
        TemperatureGradientType<N, T, false>::type;
    using HeatFluxVector =
        typename tfel::config::internals::HeatFluxVectorType<N, T, false>::type;
    using HeatFlux =
        typename tfel::config::internals::HeatFluxVectorType<N, T, false>::type;
    using ThermalConductivityMatrix = typename tfel::config::internals::
        ThermalConductivityMatrixType<N, T, false>::type;
  };

}  // end of namespace tfel::config

#endif /* LIB_TFELTYPES_HXX */
