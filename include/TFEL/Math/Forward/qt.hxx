/*!
 * \file  include/TFEL/Math/Forward/qt.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 jan. 2012
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FORWARD_QT_H_
#define LIB_TFEL_MATH_FORWARD_QT_H_

#include <type_traits>

namespace tfel {

  namespace math {

    /*
     * \class qt
     * \brief This class describes numbers with unit.
     * qt must be has efficient and simple to use that standard numerical types.
     * \param unit, the unit of the qt.
     * \param T, the underlying numerical type.
     * \pre T must be a fundamental numerical type.
     * \see unit_samples.hxx, IsFundamentalNumericType
     * \author Helfer Thomas
     * \date   06 Jun 2006
     */
    template <typename unit, typename T>
    class qt;

    /*!
     * structure describing an unit
     */
    template <typename N1,
              typename N2,
              typename N3,
              typename N4,
              typename N5,
              typename N6,
              typename N7,
              typename D1,
              typename D2,
              typename D3,
              typename D4,
              typename D5,
              typename D6,
              typename D7>
    struct Unit;

    template <int N1,
              int N2,
              int N3,
              int N4,
              int N5,
              int N6,
              int N7,
              unsigned int D1 = 1,
              unsigned int D2 = 1,
              unsigned int D3 = 1,
              unsigned int D4 = 1,
              unsigned int D5 = 1,
              unsigned int D6 = 1,
              unsigned int D7 = 1>
    struct GenerateUnit {
      typedef Unit<std::integral_constant<int, N1>,
                   std::integral_constant<int, N2>,
                   std::integral_constant<int, N3>,
                   std::integral_constant<int, N4>,
                   std::integral_constant<int, N5>,
                   std::integral_constant<int, N6>,
                   std::integral_constant<int, N7>,
                   std::integral_constant<unsigned int, D1>,
                   std::integral_constant<unsigned int, D2>,
                   std::integral_constant<unsigned int, D3>,
                   std::integral_constant<unsigned int, D4>,
                   std::integral_constant<unsigned int, D5>,
                   std::integral_constant<unsigned int, D6>,
                   std::integral_constant<unsigned int, D7>>
          type;
    };

    // A simple alias
    typedef Unit<std::integral_constant<int, 0>,
                 std::integral_constant<int, 0>,
                 std::integral_constant<int, 0>,
                 std::integral_constant<int, 0>,
                 std::integral_constant<int, 0>,
                 std::integral_constant<int, 0>,
                 std::integral_constant<int, 0>,
                 std::integral_constant<unsigned int, 1u>,
                 std::integral_constant<unsigned int, 1u>,
                 std::integral_constant<unsigned int, 1u>,
                 std::integral_constant<unsigned int, 1u>,
                 std::integral_constant<unsigned int, 1u>,
                 std::integral_constant<unsigned int, 1u>,
                 std::integral_constant<unsigned int, 1u>>
        NoUnit;

    /*!
     * \brief Declares the Mass unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<1, 0, 0, 0, 0, 0, 0>::type Mass;
    /*!
     * \brief Declares the Length unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<0, 1, 0, 0, 0, 0, 0>::type Length;
    /*!
     * \brief Declares the Time unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<0, 0, 1, 0, 0, 0, 0>::type Time;
    /*!
     * \brief Declares the Ampere unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<0, 0, 0, 1, 0, 0, 0>::type Ampere;
    /*!
     * \brief Declares the Temperature unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<0, 0, 0, 0, 1, 0, 0>::type Temperature;
    /*!
     * \brief Declares the Kelvin unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<0, 0, 0, 0, 1, 0, 0>::type Kelvin;
    /*!
     * \brief Declares the Candela unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<0, 0, 0, 0, 0, 1, 0>::type Candela;
    /*!
     * \brief Declares the Mole unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<0, 0, 0, 0, 0, 0, 1>::type Mole;

    // Additional units

    /*!
     * \brief Declares the InvLength unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<0, -1, 0, 0, 0, 0, 0>::type InvLength;  // m-1

    /*!
     * \brief Declares the InvTemperature unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<0, 0, 0, 0, -1, 0, 0>::type InvTemperature;

    /*!
     * \brief Declares the Frequency unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<0, 0, -1, 0, 0, 0, 0>::type Frequency;  // s-1

    /*!
     * \brief Declares the Velocity unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<0, 1, -1, 0, 0, 0, 0>::type Velocity;  // m.s-1

    /*!
     * \brief Declares the Acceleration unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<0, 1, -2, 0, 0, 0, 0>::type Acceleration;  // m.s-2

    /*!
     * \brief Declares the Momentum unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<1, 1, -1, 0, 0, 0, 0>::type Momentum;  // kg.m.s-1

    /*!
     * \brief Declares the Momentum unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<1, 1, -2, 0, 0, 0, 0>::type Force;  // kg.m.s-2

    /*!
     * \brief Declares the Newton unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<1, 1, -2, 0, 0, 0, 0>::type Newton;  // kg.m.s-2

    /*!
     * \brief Declares the Stress unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<1, -1, -2, 0, 0, 0, 0>::type Stress;  // kg.m-1.s-2

    /*!
     * \brief Declares the StressRate unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<1, -1, -3, 0, 0, 0, 0>::type StressRate;  // kg.m-1.s-3

    /*!
     * \brief Declares the Pressure unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<1, -1, -2, 0, 0, 0, 0>::type Pressure;  // kg.m-1.s-2

    /*!
     * \brief Declares the Energy unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<1, 2, -2, 0, 0, 0, 0>::type Energy;  // kg.m2.s-2

    /*!
     * \brief Declares the EnergyDensity unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<1, -1, -2, 0, 0, 0, 0>::type
        EnergyDensity;  // kg.m-1.s-2
    /*!
     * \brief Declares the Density unit
     * \see GenerateUnit
     */
    typedef GenerateUnit<1, -3, 0, 0, 0, 0, 0>::type Density;  // k.m-3

    /*!
     * \brief Declares the ThermalConductivity unit
     */
    typedef GenerateUnit<1, 1, -3, 0, -1, 0, 0>::type
        ThermalConductivity;  // kg.m.s-3.K-1

    /*!
     * \brief Declares the HeatFluxDensity unit
     */
    typedef GenerateUnit<1, 0, -3, 0, 0, 0, 0>::type HeatFluxDensity;  // kg.s-3

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_FORWARD_QT_H_ */
