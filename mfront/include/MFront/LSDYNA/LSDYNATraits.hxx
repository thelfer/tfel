/*!
 * \file   mfront/include/MFront/LSDYNA/LSDYNATraits.hxx
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

#ifndef LIB_MFRONT_LSDYNATRAITS_HXX
#define LIB_MFRONT_LSDYNATRAITS_HXX

namespace lsdyna {

  enum LSDYNABehaviourType {
    STANDARDSTRAINBASEDBEHAVIOUR,
    STANDARDFINITESTRAINBEHAVIOUR
  };  // end of enum LSDYNABehaviourType

  enum LSDYNASymmetryType {
    ISOTROPIC,
    ORTHOTROPIC
  };  // end of enum LSDYNASymmetryType

  template <typename Behaviour>
  struct LSDYNATraits {
    static constexpr LSDYNABehaviourType btype =
        lsdyna::STANDARDSTRAINBASEDBEHAVIOUR;
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
    static constexpr LSDYNASymmetryType type = lsdyna::ISOTROPIC;
    // Type of the elastic behaviour of the material (isotropic or orthotropic)
    static constexpr LSDYNASymmetryType etype = lsdyna::ISOTROPIC;
  };  // end of class LSDYNATraits

}  // end of namespace lsdyna

#endif /* LIB_MFRONT_LSDYNATRAITS_HXX */
