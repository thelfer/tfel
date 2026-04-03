/*!
 * \file   mfront/include/MFront/DianaFEA/DianaFEATraits.hxx
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

#ifndef LIB_MFRONT_DIANAFEATRAITS_HXX
#define LIB_MFRONT_DIANAFEATRAITS_HXX

namespace dianafea {

  enum DianaFEABehaviourType {
    STANDARDSTRAINBASEDBEHAVIOUR,
    STANDARDFINITESTRAINBEHAVIOUR
  };  // end of enum DIANAFEABehaviourType

  enum DianaFEASymmetryType {
    ISOTROPIC,
    ORTHOTROPIC
  };  // end of enum DianaFEASymmetryType

  template <typename Behaviour>
  struct DianaFEATraits {
    static constexpr DianaFEABehaviourType btype =
        dianafea::STANDARDSTRAINBASEDBEHAVIOUR;
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
    static constexpr DianaFEASymmetryType type = dianafea::ISOTROPIC;
    // Type of the elastic behaviour of the material (isotropic or orthotropic)
    static constexpr DianaFEASymmetryType etype = dianafea::ISOTROPIC;
  };  // end of class DIANAFEATraits

}  // end of namespace dianafea

#endif /* LIB_MFRONT_DIANAFEATRAITS_HXX */
