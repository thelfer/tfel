/*!
 * \file   src/PhysicalConstants/PhysicalConstants.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28/12/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/PhysicalConstants.hxx"

#if __cplusplus < 201703L

namespace tfel {

  template <>
  const float PhysicalConstants<float>::AtomicMassConstant =
      static_cast<float>(1.66054e-27);

  template <>
  const float PhysicalConstants<float>::mu = static_cast<float>(1.66054e-27);

  template <>
  const float PhysicalConstants<float>::AvogadroConstant =
      static_cast<float>(6.02214e+23);

  template <>
  const float PhysicalConstants<float>::Na = static_cast<float>(6.02214e+23);

  template <>
  const float PhysicalConstants<float>::BoltzmannConstant =
      static_cast<float>(1.38065e-23);

  template <>
  const float PhysicalConstants<float>::kb = static_cast<float>(1.38065e-23);

  template <>
  const float PhysicalConstants<float>::ConductanceQuantum =
      static_cast<float>(7.74809e-05);

  template <>
  const float PhysicalConstants<float>::G0 = static_cast<float>(7.74809e-05);

  template <>
  const float PhysicalConstants<float>::ElectricConstant =
      static_cast<float>(8.85419e-12);

  template <>
  const float PhysicalConstants<float>::e0 = static_cast<float>(8.85419e-12);

  template <>
  const float PhysicalConstants<float>::ElectronMass =
      static_cast<float>(9.10938e-31);

  template <>
  const float PhysicalConstants<float>::me = static_cast<float>(9.10938e-31);

  template <>
  const float PhysicalConstants<float>::ElectronVolt =
      static_cast<float>(1.60218e-19);

  template <>
  const float PhysicalConstants<float>::eV = static_cast<float>(1.60218e-19);

  template <>
  const float PhysicalConstants<float>::ElementaryCharge =
      static_cast<float>(1.60218e-19);

  template <>
  const float PhysicalConstants<float>::e = static_cast<float>(1.60218e-19);

  template <>
  const float PhysicalConstants<float>::FaradayConstant =
      static_cast<float>(96485.3);

  template <>
  const float PhysicalConstants<float>::F = static_cast<float>(96485.3);

  template <>
  const float PhysicalConstants<float>::FineStructureConstant =
      static_cast<float>(0.00729735);

  template <>
  const float PhysicalConstants<float>::a = static_cast<float>(0.00729735);

  template <>
  const float PhysicalConstants<float>::MolarGasConstant =
      static_cast<float>(8.31446);

  template <>
  const float PhysicalConstants<float>::R = static_cast<float>(8.31446);

  template <>
  const float PhysicalConstants<float>::StefanBoltzmannConstant =
      static_cast<float>(5.67037e-08);

  template <>
  const float PhysicalConstants<float>::s = static_cast<float>(5.67037e-08);

  template <>
  const double PhysicalConstants<double>::AtomicMassConstant =
      static_cast<double>(1.66054e-27);

  template <>
  const double PhysicalConstants<double>::mu = static_cast<double>(1.66054e-27);

  template <>
  const double PhysicalConstants<double>::AvogadroConstant =
      static_cast<double>(6.02214e+23);

  template <>
  const double PhysicalConstants<double>::Na = static_cast<double>(6.02214e+23);

  template <>
  const double PhysicalConstants<double>::BoltzmannConstant =
      static_cast<double>(1.38065e-23);

  template <>
  const double PhysicalConstants<double>::kb = static_cast<double>(1.38065e-23);

  template <>
  const double PhysicalConstants<double>::ConductanceQuantum =
      static_cast<double>(7.74809e-05);

  template <>
  const double PhysicalConstants<double>::G0 = static_cast<double>(7.74809e-05);

  template <>
  const double PhysicalConstants<double>::ElectricConstant =
      static_cast<double>(8.85419e-12);

  template <>
  const double PhysicalConstants<double>::e0 = static_cast<double>(8.85419e-12);

  template <>
  const double PhysicalConstants<double>::ElectronMass =
      static_cast<double>(9.10938e-31);

  template <>
  const double PhysicalConstants<double>::me = static_cast<double>(9.10938e-31);

  template <>
  const double PhysicalConstants<double>::ElectronVolt =
      static_cast<double>(1.60218e-19);

  template <>
  const double PhysicalConstants<double>::eV = static_cast<double>(1.60218e-19);

  template <>
  const double PhysicalConstants<double>::ElementaryCharge =
      static_cast<double>(1.60218e-19);

  template <>
  const double PhysicalConstants<double>::e = static_cast<double>(1.60218e-19);

  template <>
  const double PhysicalConstants<double>::FaradayConstant =
      static_cast<double>(96485.3);

  template <>
  const double PhysicalConstants<double>::F = static_cast<double>(96485.3);

  template <>
  const double PhysicalConstants<double>::FineStructureConstant =
      static_cast<double>(0.00729735);

  template <>
  const double PhysicalConstants<double>::a = static_cast<double>(0.00729735);

  template <>
  const double PhysicalConstants<double>::MolarGasConstant =
      static_cast<double>(8.31446);

  template <>
  const double PhysicalConstants<double>::R = static_cast<double>(8.31446);

  template <>
  const double PhysicalConstants<double>::StefanBoltzmannConstant =
      static_cast<double>(5.67037e-08);

  template <>
  const double PhysicalConstants<double>::s = static_cast<double>(5.67037e-08);

  template <>
  const long double PhysicalConstants<long double>::AtomicMassConstant =
      static_cast<long double>(1.66054e-27);

  template <>
  const long double PhysicalConstants<long double>::mu =
      static_cast<long double>(1.66054e-27);

  template <>
  const long double PhysicalConstants<long double>::AvogadroConstant =
      static_cast<long double>(6.02214e+23);

  template <>
  const long double PhysicalConstants<long double>::Na =
      static_cast<long double>(6.02214e+23);

  template <>
  const long double PhysicalConstants<long double>::BoltzmannConstant =
      static_cast<long double>(1.38065e-23);

  template <>
  const long double PhysicalConstants<long double>::kb =
      static_cast<long double>(1.38065e-23);

  template <>
  const long double PhysicalConstants<long double>::ConductanceQuantum =
      static_cast<long double>(7.74809e-05);

  template <>
  const long double PhysicalConstants<long double>::G0 =
      static_cast<long double>(7.74809e-05);

  template <>
  const long double PhysicalConstants<long double>::ElectricConstant =
      static_cast<long double>(8.85419e-12);

  template <>
  const long double PhysicalConstants<long double>::e0 =
      static_cast<long double>(8.85419e-12);

  template <>
  const long double PhysicalConstants<long double>::ElectronMass =
      static_cast<long double>(9.10938e-31);

  template <>
  const long double PhysicalConstants<long double>::me =
      static_cast<long double>(9.10938e-31);

  template <>
  const long double PhysicalConstants<long double>::ElectronVolt =
      static_cast<long double>(1.60218e-19);

  template <>
  const long double PhysicalConstants<long double>::eV =
      static_cast<long double>(1.60218e-19);

  template <>
  const long double PhysicalConstants<long double>::ElementaryCharge =
      static_cast<long double>(1.60218e-19);

  template <>
  const long double PhysicalConstants<long double>::e =
      static_cast<long double>(1.60218e-19);

  template <>
  const long double PhysicalConstants<long double>::FaradayConstant =
      static_cast<long double>(96485.3);

  template <>
  const long double PhysicalConstants<long double>::F =
      static_cast<long double>(96485.3);

  template <>
  const long double PhysicalConstants<long double>::FineStructureConstant =
      static_cast<long double>(0.00729735);

  template <>
  const long double PhysicalConstants<long double>::a =
      static_cast<long double>(0.00729735);

  template <>
  const long double PhysicalConstants<long double>::MolarGasConstant =
      static_cast<long double>(8.31446);

  template <>
  const long double PhysicalConstants<long double>::R =
      static_cast<long double>(8.31446);

  template <>
  const long double PhysicalConstants<long double>::StefanBoltzmannConstant =
      static_cast<long double>(5.67037e-08);

  template <>
  const long double PhysicalConstants<long double>::s =
      static_cast<long double>(5.67037e-08);

}  // end of namespace tfel

#endif /* __cplusplus < 201703L */
