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

namespace tfel {

#if __cplusplus >= 201703L

  template <typename real = double>
  struct PhysicalConstants {
    //! Atomic mass constant (kg)
    static constexpr const real AtomicMassConstant = real(1.66054e-27);
    //! Atomic mass constant (kg)
    static constexpr const real mu = real(1.66054e-27);
    //! Avogadro constant (mol-1)
    static constexpr const real AvogadroConstant = real(6.02214e+23);
    //! Avogadro constant (mol-1)
    static constexpr const real Na = real(6.02214e+23);
    //! Boltzmann constant (J.K-1)
    static constexpr const real BoltzmannConstant = real(1.38065e-23);
    //! Boltzmann constant (J.K-1)
    static constexpr const real kb = real(1.38065e-23);
    //! Conductance quantum (S)
    static constexpr const real ConductanceQuantum = real(7.74809e-05);
    //! Conductance quantum (S)
    static constexpr const real G0 = real(7.74809e-05);
    //! Electric constant (F.m-1)
    static constexpr const real ElectricConstant = real(8.85419e-12);
    //! Electric constant (F.m-1)
    static constexpr const real e0 = real(8.85419e-12);
    //! Electron mass (kg)
    static constexpr const real ElectronMass = real(9.10938e-31);
    //! Electron mass (kg)
    static constexpr const real me = real(9.10938e-31);
    //! Electron volt (J)
    static constexpr const real ElectronVolt = real(1.60218e-19);
    //! Electron volt (J)
    static constexpr const real eV = real(1.60218e-19);
    //! Elementary charge (C)
    static constexpr const real ElementaryCharge = real(1.60218e-19);
    //! Elementary charge (C)
    static constexpr const real e = real(1.60218e-19);
    //! Faraday constant (C.mol-1)
    static constexpr const real FaradayConstant = real(96485.3);
    //! Faraday constant (C.mol-1)
    static constexpr const real F = real(96485.3);
    //! Fine-structure constant
    static constexpr const real FineStructureConstant = real(0.00729735);
    //! Fine-structure constant
    static constexpr const real a = real(0.00729735);
    //! Molar gas constant (J.mol-1.K-1)
    static constexpr const real MolarGasConstant = real(8.31446);
    //! Molar gas constant (J.mol-1.K-1)
    static constexpr const real R = real(8.31446);
    //! Stefan-Boltzmann constant (W.m-2.K-4)
    static constexpr const real StefanBoltzmannConstant = real(5.67037e-08);
    //! Stefan-Boltzmann constant (W.m-2.K-4)
    static constexpr const real s = real(5.67037e-08);
  };  // end of PhysicalConstants

  namespace constants {

    //! Atomic mass constant (kg)
    template <typename real>
    inline constexpr const real AtomicMassConstant = real(1.66054e-27);
    //! Atomic mass constant (kg)
    template <typename real = double>
    inline constexpr const real mu = real(1.66054e-27);
    //! Avogadro constant (mol-1)
    template <typename real>
    inline constexpr const real AvogadroConstant = real(6.02214e+23);
    //! Avogadro constant (mol-1)
    template <typename real = double>
    inline constexpr const real Na = real(6.02214e+23);
    //! Boltzmann constant (J.K-1)
    template <typename real>
    inline constexpr const real BoltzmannConstant = real(1.38065e-23);
    //! Boltzmann constant (J.K-1)
    template <typename real = double>
    inline constexpr const real kb = real(1.38065e-23);
    //! Conductance quantum (S)
    template <typename real>
    inline constexpr const real ConductanceQuantum = real(7.74809e-05);
    //! Conductance quantum (S)
    template <typename real = double>
    inline constexpr const real G0 = real(7.74809e-05);
    //! Electric constant (F.m-1)
    template <typename real>
    inline constexpr const real ElectricConstant = real(8.85419e-12);
    //! Electric constant (F.m-1)
    template <typename real = double>
    inline constexpr const real e0 = real(8.85419e-12);
    //! Electron mass (kg)
    template <typename real>
    inline constexpr const real ElectronMass = real(9.10938e-31);
    //! Electron mass (kg)
    template <typename real = double>
    inline constexpr const real me = real(9.10938e-31);
    //! Electron volt (J)
    template <typename real>
    inline constexpr const real ElectronVolt = real(1.60218e-19);
    //! Electron volt (J)
    template <typename real = double>
    inline constexpr const real eV = real(1.60218e-19);
    //! Elementary charge (C)
    template <typename real>
    inline constexpr const real ElementaryCharge = real(1.60218e-19);
    //! Elementary charge (C)
    template <typename real = double>
    inline constexpr const real e = real(1.60218e-19);
    //! Faraday constant (C.mol-1)
    template <typename real>
    inline constexpr const real FaradayConstant = real(96485.3);
    //! Faraday constant (C.mol-1)
    template <typename real = double>
    inline constexpr const real F = real(96485.3);
    //! Fine-structure constant
    template <typename real>
    inline constexpr const real FineStructureConstant = real(0.00729735);
    //! Fine-structure constant
    template <typename real = double>
    inline constexpr const real a = real(0.00729735);
    //! Molar gas constant (J.mol-1.K-1)
    template <typename real>
    inline constexpr const real MolarGasConstant = real(8.31446);
    //! Molar gas constant (J.mol-1.K-1)
    template <typename real = double>
    inline constexpr const real R = real(8.31446);
    //! Stefan-Boltzmann constant (W.m-2.K-4)
    template <typename real>
    inline constexpr const real StefanBoltzmannConstant = real(5.67037e-08);
    //! Stefan-Boltzmann constant (W.m-2.K-4)
    template <typename real = double>
    inline constexpr const real s = real(5.67037e-08);

  }  // end of namespace constants

#else  /* __cplusplus >= 201703L */

  template <typename real = double>
  struct TFELPHYSICALCONSTANTS_VISIBILITY_EXPORT PhysicalConstants {
    //! Atomic mass constant (kg)
    static const real AtomicMassConstant;
    //! Atomic mass constant (kg)
    static const real mu;
    //! Avogadro constant (mol-1)
    static const real AvogadroConstant;
    //! Avogadro constant (mol-1)
    static const real Na;
    //! Boltzmann constant (J.K-1)
    static const real BoltzmannConstant;
    //! Boltzmann constant (J.K-1)
    static const real kb;
    //! Conductance quantum (S)
    static const real ConductanceQuantum;
    //! Conductance quantum (S)
    static const real G0;
    //! Electric constant (F.m-1)
    static const real ElectricConstant;
    //! Electric constant (F.m-1)
    static const real e0;
    //! Electron mass (kg)
    static const real ElectronMass;
    //! Electron mass (kg)
    static const real me;
    //! Electron volt (J)
    static const real ElectronVolt;
    //! Electron volt (J)
    static const real eV;
    //! Elementary charge (C)
    static const real ElementaryCharge;
    //! Elementary charge (C)
    static const real e;
    //! Faraday constant (C.mol-1)
    static const real FaradayConstant;
    //! Faraday constant (C.mol-1)
    static const real F;
    //! Fine-structure constant
    static const real FineStructureConstant;
    //! Fine-structure constant
    static const real a;
    //! Molar gas constant (J.mol-1.K-1)
    static const real MolarGasConstant;
    //! Molar gas constant (J.mol-1.K-1)
    static const real R;
    //! Stefan-Boltzmann constant (W.m-2.K-4)
    static const real StefanBoltzmannConstant;
    //! Stefan-Boltzmann constant (W.m-2.K-4)
    static const real s;
  };  // end of PhysicalConstants
#endif /* __cplusplus >= 201703L */

}  // end of namespace tfel

#endif /* LIB_TFEL_PHYSICALCONSTANTS_HXX */
