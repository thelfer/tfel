/*!
 * \file   include/TFEL/PhysicalConstants.hxx
 * \brief
 * \author Thomas Helfer
 * \date   18 août 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_PHYSICALCONSTANTS_HXX
#define LIB_TFEL_PHYSICALCONSTANTS_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/qt.hxx"

namespace tfel {

  template <typename NumericType = double, bool use_quantities = false>
  struct PhysicalConstants;

  /*!
   * \brief partial specialisation when quantities are used */
  template <typename NumericType>
  struct PhysicalConstants<NumericType, true> {
    //! a simple alias
    template <int N1, int N2, int N3, int N4, int N5, int N6, int N7>
    using quantity = tfel::math::qt<
        typename tfel::math::GenerateUnit<N1, N2, N3, N4, N5, N6, N7>::type,
        NumericType>;
    //! Atomic mass constant (kg)
    static constexpr auto AtomicMassConstant =
        quantity<1, 0, 0, 0, 0, 0, 0>(1.66054e-27);
    //! Atomic mass constant (kg)
    static constexpr auto mu = quantity<1, 0, 0, 0, 0, 0, 0>(1.66054e-27);
    //! Avogadro constant (mol-1)
    static constexpr auto AvogadroConstant =
        quantity<0, 0, 0, 0, 0, 0, -1>(6.02214e+23);
    //! Avogadro constant (mol-1)
    static constexpr auto Na = quantity<0, 0, 0, 0, 0, 0, -1>(6.02214e+23);
    //! Boltzmann constant (J.K-1)
    static constexpr auto BoltzmannConstant =
        quantity<1, 2, -2, 0, -1, 0, 0>(1.38065e-23);
    //! Boltzmann constant (J.K-1)
    static constexpr auto kb = quantity<1, 2, -2, 0, -1, 0, 0>(1.38065e-23);
    //! Conductance quantum (S)
    static constexpr auto ConductanceQuantum =
        quantity<-1, -2, 3, 2, 0, 0, 0>(7.74809e-05);
    //! Conductance quantum (S)
    static constexpr auto G0 = quantity<-1, -2, 3, 2, 0, 0, 0>(7.74809e-05);
    //! Electric constant (F.m-1)
    static constexpr auto ElectricConstant =
        quantity<-1, -3, 4, 2, 0, 0, 0>(8.85419e-12);
    //! Electric constant (F.m-1)
    static constexpr auto e0 = quantity<-1, -3, 4, 2, 0, 0, 0>(8.85419e-12);
    //! Electron mass (kg)
    static constexpr auto ElectronMass =
        quantity<1, 0, 0, 0, 0, 0, 0>(9.10938e-31);
    //! Electron mass (kg)
    static constexpr auto me = quantity<1, 0, 0, 0, 0, 0, 0>(9.10938e-31);
    //! Electron volt (J)
    static constexpr auto ElectronVolt =
        quantity<1, 2, -2, 0, 0, 0, 0>(1.60218e-19);
    //! Electron volt (J)
    static constexpr auto eV = quantity<1, 2, -2, 0, 0, 0, 0>(1.60218e-19);
    //! Elementary charge (C)
    static constexpr auto ElementaryCharge =
        quantity<0, 0, -1, 1, 0, 0, 0>(1.60218e-19);
    //! Elementary charge (C)
    static constexpr auto e = quantity<0, 0, -1, 1, 0, 0, 0>(1.60218e-19);
    //! Faraday constant (C.mol-1)
    static constexpr auto FaradayConstant =
        quantity<0, 0, -1, 1, 0, 0, -1>(96485.3);
    //! Faraday constant (C.mol-1)
    static constexpr auto F = quantity<0, 0, -1, 1, 0, 0, -1>(96485.3);
    //! Fine-structure constant
    static constexpr auto FineStructureConstant = NumericType(0.00729735);
    //! Fine-structure constant
    static constexpr auto a = NumericType(0.00729735);
    //! Molar gas constant (J.mol-1.K-1)
    static constexpr auto MolarGasConstant =
        quantity<1, 2, -2, 0, -1, 0, -1>(8.31446);
    //! Molar gas constant (J.mol-1.K-1)
    static constexpr auto R = quantity<1, 2, -2, 0, -1, 0, -1>(8.31446);
    //! Stefan-Boltzmann constant (W.m-2.K-4)
    static constexpr auto StefanBoltzmannConstant =
        quantity<1, 0, -3, 0, -4, 0, 0>(5.67037e-08);
    //! Stefan-Boltzmann constant (W.m-2.K-4)
    static constexpr auto s = quantity<1, 0, -3, 0, -4, 0, 0>(5.67037e-08);
  };  // end of PhysicalConstants
  /*!
   * \brief partial specialisation when quantities are not used */
  template <typename NumericType>
  struct PhysicalConstants<NumericType, false> {
    //! Atomic mass constant (kg)
    static constexpr auto AtomicMassConstant = NumericType(1.66054e-27);
    //! Atomic mass constant (kg)
    static constexpr auto mu = NumericType(1.66054e-27);
    //! Avogadro constant (mol-1)
    static constexpr auto AvogadroConstant = NumericType(6.02214e+23);
    //! Avogadro constant (mol-1)
    static constexpr auto Na = NumericType(6.02214e+23);
    //! Boltzmann constant (J.K-1)
    static constexpr auto BoltzmannConstant = NumericType(1.38065e-23);
    //! Boltzmann constant (J.K-1)
    static constexpr auto kb = NumericType(1.38065e-23);
    //! Conductance quantum (S)
    static constexpr auto ConductanceQuantum = NumericType(7.74809e-05);
    //! Conductance quantum (S)
    static constexpr auto G0 = NumericType(7.74809e-05);
    //! Electric constant (F.m-1)
    static constexpr auto ElectricConstant = NumericType(8.85419e-12);
    //! Electric constant (F.m-1)
    static constexpr auto e0 = NumericType(8.85419e-12);
    //! Electron mass (kg)
    static constexpr auto ElectronMass = NumericType(9.10938e-31);
    //! Electron mass (kg)
    static constexpr auto me = NumericType(9.10938e-31);
    //! Electron volt (J)
    static constexpr auto ElectronVolt = NumericType(1.60218e-19);
    //! Electron volt (J)
    static constexpr auto eV = NumericType(1.60218e-19);
    //! Elementary charge (C)
    static constexpr auto ElementaryCharge = NumericType(1.60218e-19);
    //! Elementary charge (C)
    static constexpr auto e = NumericType(1.60218e-19);
    //! Faraday constant (C.mol-1)
    static constexpr auto FaradayConstant = NumericType(96485.3);
    //! Faraday constant (C.mol-1)
    static constexpr auto F = NumericType(96485.3);
    //! Fine-structure constant
    static constexpr auto FineStructureConstant = NumericType(0.00729735);
    //! Fine-structure constant
    static constexpr auto a = NumericType(0.00729735);
    //! Molar gas constant (J.mol-1.K-1)
    static constexpr auto MolarGasConstant = NumericType(8.31446);
    //! Molar gas constant (J.mol-1.K-1)
    static constexpr auto R = NumericType(8.31446);
    //! Stefan-Boltzmann constant (W.m-2.K-4)
    static constexpr auto StefanBoltzmannConstant = NumericType(5.67037e-08);
    //! Stefan-Boltzmann constant (W.m-2.K-4)
    static constexpr auto s = NumericType(5.67037e-08);
  };  // end of PhysicalConstants

}  // end of namespace tfel

namespace tfel::constants {

  //! Atomic mass constant (kg)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto AtomicMassConstant =
      tfel::PhysicalConstants<NumericType, use_quantities>::AtomicMassConstant;
  //! Atomic mass constant (kg)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto mu =
      tfel::PhysicalConstants<NumericType, use_quantities>::mu;
  //! Avogadro constant (mol-1)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto AvogadroConstant =
      tfel::PhysicalConstants<NumericType, use_quantities>::AvogadroConstant;
  //! Avogadro constant (mol-1)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto Na =
      tfel::PhysicalConstants<NumericType, use_quantities>::Na;
  //! Boltzmann constant (J.K-1)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto BoltzmannConstant =
      tfel::PhysicalConstants<NumericType, use_quantities>::BoltzmannConstant;
  //! Boltzmann constant (J.K-1)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto kb =
      tfel::PhysicalConstants<NumericType, use_quantities>::kb;
  //! Conductance quantum (S)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto ConductanceQuantum =
      tfel::PhysicalConstants<NumericType, use_quantities>::ConductanceQuantum;
  //! Conductance quantum (S)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto G0 =
      tfel::PhysicalConstants<NumericType, use_quantities>::G0;
  //! Electric constant (F.m-1)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto ElectricConstant =
      tfel::PhysicalConstants<NumericType, use_quantities>::ElectricConstant;
  //! Electric constant (F.m-1)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto e0 =
      tfel::PhysicalConstants<NumericType, use_quantities>::e0;
  //! Electron mass (kg)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto ElectronMass =
      tfel::PhysicalConstants<NumericType, use_quantities>::ElectronMass;
  //! Electron mass (kg)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto me =
      tfel::PhysicalConstants<NumericType, use_quantities>::me;
  //! Electron volt (J)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto ElectronVolt =
      tfel::PhysicalConstants<NumericType, use_quantities>::ElectronVolt;
  //! Electron volt (J)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto eV =
      tfel::PhysicalConstants<NumericType, use_quantities>::eV;
  //! Elementary charge (C)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto ElementaryCharge =
      tfel::PhysicalConstants<NumericType, use_quantities>::ElementaryCharge;
  //! Elementary charge (C)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto e =
      tfel::PhysicalConstants<NumericType, use_quantities>::e;
  //! Faraday constant (C.mol-1)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto FaradayConstant =
      tfel::PhysicalConstants<NumericType, use_quantities>::FaradayConstant;
  //! Faraday constant (C.mol-1)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto F =
      tfel::PhysicalConstants<NumericType, use_quantities>::F;
  //! Fine-structure constant
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto FineStructureConstant =
      tfel::PhysicalConstants<NumericType,
                              use_quantities>::FineStructureConstant;
  //! Fine-structure constant
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto a =
      tfel::PhysicalConstants<NumericType, use_quantities>::a;
  //! Molar gas constant (J.mol-1.K-1)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto MolarGasConstant =
      tfel::PhysicalConstants<NumericType, use_quantities>::MolarGasConstant;
  //! Molar gas constant (J.mol-1.K-1)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto R =
      tfel::PhysicalConstants<NumericType, use_quantities>::R;
  //! Stefan-Boltzmann constant (W.m-2.K-4)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto StefanBoltzmannConstant =
      tfel::PhysicalConstants<NumericType,
                              use_quantities>::StefanBoltzmannConstant;
  //! Stefan-Boltzmann constant (W.m-2.K-4)
  template <typename NumericType = double, bool use_quantities = false>
  inline constexpr auto s =
      tfel::PhysicalConstants<NumericType, use_quantities>::s;

}  // end of namespace tfel::constants

#endif /* LIB_TFEL_PHYSICALCONSTANTS_HXX */
