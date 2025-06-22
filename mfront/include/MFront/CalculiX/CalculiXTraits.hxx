/*!
 * \file   mfront/include/MFront/CalculiX/CalculiXTraits.hxx
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

#ifndef LIB_MFRONT_CALCULIXTRAITS_HXX
#define LIB_MFRONT_CALCULIXTRAITS_HXX

namespace calculix {

  enum CalculiXBehaviourType {
    STANDARDSTRAINBASEDBEHAVIOUR,
    STANDARDFINITESTRAINBEHAVIOUR
  };  // end of enum CALCULIXBehaviourType

  enum CalculiXSymmetryType {
    ISOTROPIC,
    ORTHOTROPIC
  };  // end of enum CalculiXSymmetryType

  template <typename Behaviour>
  struct CalculiXTraits {
    static constexpr CalculiXBehaviourType btype =
        calculix::STANDARDSTRAINBASEDBEHAVIOUR;
    // size of the driving variable array (STRAN)
    static constexpr unsigned short GradientSize = 0u;
    // size of the thermodynamic force variable array (STESS)
    static constexpr unsigned short ThermodynamicForceVariableSize = 0u;
    static constexpr bool requiresStiffnessTensor = false;
    static constexpr bool requiresUnAlteredStiffnessTensor = true;
    static constexpr bool requiresThermalExpansionCoefficientTensor = false;
    static constexpr unsigned short elasticPropertiesOffset = 0u;
    static constexpr unsigned short thermalExpansionPropertiesOffset = 0u;
    // Type of the behaviour of the material (isotropic or orthotropic)
    static constexpr CalculiXSymmetryType type = calculix::ISOTROPIC;
    // Type of the elastic behaviour of the material (isotropic or orthotropic)
    static constexpr CalculiXSymmetryType etype = calculix::ISOTROPIC;
  };  // end of class CALCULIXTraits

}  // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIXTRAITS_HXX */
