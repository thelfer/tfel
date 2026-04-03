/*!
 * \file   mfront/include/MFront/Ansys/AnsysTraits.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   13/02/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ANSYSTRAITS_HXX
#define LIB_MFRONT_ANSYSTRAITS_HXX

namespace ansys {

  enum AnsysBehaviourType {
    STANDARDSTRAINBASEDBEHAVIOUR,
    STANDARDFINITESTRAINBEHAVIOUR
  };  // end of enum ANSYSBehaviourType

  enum AnsysSymmetryType {
    ISOTROPIC,
    ORTHOTROPIC
  };  // end of enum AnsysSymmetryType

  template <typename Behaviour>
  struct AnsysTraits {
    static constexpr AnsysBehaviourType btype =
        ansys::STANDARDSTRAINBASEDBEHAVIOUR;
    // size of the driving variable array (STRAN)
    static constexpr unsigned short GradientSize = 0u;
    // size of the thermodynamic force variable array (STESS)
    static constexpr unsigned short ThermodynamicForceVariableSize = 0u;
    static constexpr bool requiresStiffnessTensor = false;
    static constexpr bool requiresUnAlteredStiffnessTensor = true;
    static constexpr bool requiresThermalExpansionCoefficientTensor = false;
    static constexpr unsigned short elasticPropertiesOffset = 0u;
    static constexpr unsigned short thermalExpansionPropertiesOffset = 0u;
    static constexpr unsigned short orthotropicAxesOffset = 0u;
    // Type of the behaviour of the material (isotropic or orthotropic)
    static constexpr AnsysSymmetryType type = ansys::ISOTROPIC;
    // Type of the elastic behaviour of the material (isotropic or orthotropic)
    static constexpr AnsysSymmetryType etype = ansys::ISOTROPIC;
  };  // end of class ANSYSTraits

}  // end of namespace ansys

#endif /* LIB_MFRONT_ANSYSTRAITS_HXX */
