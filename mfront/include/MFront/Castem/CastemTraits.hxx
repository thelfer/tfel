/*!
 * \file   mfront/include/MFront/Castem/CastemTraits.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   13 fév 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEMTRAITS_HXX
#define LIB_MFRONT_CASTEMTRAITS_HXX

namespace castem {

  enum CastemBehaviourType {
    STANDARDSTRAINBASEDBEHAVIOUR,
    STANDARDFINITESTRAINBEHAVIOUR,
    COHESIVEZONEMODEL
  };  // end of enum CastemBehaviourType

  enum CastemSymmetryType {
    ISOTROPIC,
    ORTHOTROPIC
  };  // end of enum CastemSymmetryType

  template <typename Behaviour>
  struct CastemTraits {
    static constexpr CastemBehaviourType btype =
        castem::STANDARDSTRAINBASEDBEHAVIOUR;
    // size of the driving variable array (STRAN)
    static constexpr unsigned short DrivingVariableSize = 0u;
    // size of the thermodynamic force variable array (STESS)
    static constexpr unsigned short ThermodynamicForceVariableSize = 0u;
    static constexpr bool useTimeSubStepping = false;
    static constexpr bool doSubSteppingOnInvalidResults = false;
    static constexpr unsigned short maximumSubStepping = 0u;
    static constexpr bool requiresStiffnessTensor = false;
    static constexpr bool requiresThermalExpansionCoefficientTensor = false;
    static constexpr unsigned short propertiesOffset = 0u;
    static constexpr unsigned short material_properties_nb = 0u;
    static constexpr unsigned short internal_variables_nb = 0u;
    static constexpr CastemSymmetryType stype = castem::ISOTROPIC;
  };  // end of class CastemTraits

}  // end of namespace castem

#endif /* LIB_MFRONT_CASTEMTRAITS_HXX */
