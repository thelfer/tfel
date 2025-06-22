/*!
 * \file   mfront/include/MFront/Europlexus/EuroplexusTraits.hxx
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

#ifndef LIB_MFRONT_EPXTRAITS_HXX
#define LIB_MFRONT_EPXTRAITS_HXX

namespace epx {

  enum EuroplexusBehaviourType {
    STANDARDSTRAINBASEDBEHAVIOUR,
    STANDARDFINITESTRAINBEHAVIOUR
  };  // end of enum EPXBehaviourType

  enum EuroplexusSymmetryType {
    ISOTROPIC,
    ORTHOTROPIC
  };  // end of enum EuroplexusSymmetryType

  template <typename Behaviour>
  struct EuroplexusTraits {
    static constexpr EuroplexusBehaviourType btype =
        epx::STANDARDSTRAINBASEDBEHAVIOUR;
    // size of the driving variable array (STRAN)
    static constexpr unsigned short GradientSize = 0u;
    // size of the thermodynamic force variable array (STESS)
    static constexpr unsigned short ThermodynamicForceVariableSize = 0u;
    static constexpr bool requiresStiffnessTensor = false;
    static constexpr bool requiresThermalExpansionCoefficientTensor = false;
    static constexpr unsigned short propertiesOffset = 0u;
    // Type of the behaviour of the material (isotropic or orthotropic)
    static constexpr EuroplexusSymmetryType type = epx::ISOTROPIC;
    // Type of the elastic behaviour of the material (isotropic or orthotropic)
    static constexpr EuroplexusSymmetryType etype = epx::ISOTROPIC;
  };  // end of class EPXTraits

}  // end of namespace epx

#endif /* LIB_MFRONT_EPXTRAITS_HXX */
