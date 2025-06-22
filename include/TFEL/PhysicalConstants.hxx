/*!
 * \file   PhysicalConstants.hxx
 * \brief
 * \author HELFER Thomas 202608
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

namespace tfel {

  template <typename real = double, bool use_qt = false>
  struct PhysicalConstants {
    //! Atomic mass constant (kg)
    static constexpr const real mu = real(1.66054e-27);
    //! Avogadro constant (mol-1)
    static constexpr const real Na = real(6.02214e+23);
    //! Boltzmann constant (J.K-1)
    static constexpr const real kb = real(1.38065e-23);
    //! Conductance quantum (S)
    static constexpr const real G0 = real(7.74809e-05);
    //! Electric constant (F.m-1)
    static constexpr const real e0 = real(8.85419e-12);
    //! Electron mass (kg)
    static constexpr const real me = real(9.10938e-31);
    //! Electron volt (J)
    static constexpr const real eV = real(1.60218e-19);
    //! Elementary charge (C)
    static constexpr const real e = real(1.60218e-19);
    //! Faraday constant (C.mol-1)
    static constexpr const real F = real(96485.3);
    //! Fine-structure constant
    static constexpr const real a = real(0.00729735);
    //! Molar gas constant (J.mol-1.K-1)
    static constexpr const real R = real(8.31446);
    //! Stefan-Boltzmann constant (W.m-2.K-4)
    static constexpr const real s = real(5.67037e-08);
  };  // end of PhysicalConstants

}  // end of namespace tfel

#endif /* LIB_TFEL_PHYSICALCONSTANTS_HXX */
