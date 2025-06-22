/*!
 * \file   mfront/include/MFront/Aster/AsterTraits.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   13 fév 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ASTERTRAITS_HXX_
#define LIB_MFRONT_ASTERTRAITS_HXX_

namespace aster {

  enum AsterBehaviourType {
    SMALLSTRAINSTANDARDBEHAVIOUR,
    FINITESTRAINSTANDARDBEHAVIOUR,
    COHESIVEZONEMODEL
  };  // end of enum ASTERBehaviourType

  enum AsterSymmetryType {
    ISOTROPIC,
    ORTHOTROPIC
  };  // end of enum AsterSymmetryType

  enum AsterErrorReportPolicy {
    ASTER_WRITEONSTDOUT,
    ASTER_NOERRORREPORT
  };  // end of enum AsterErrorReportPolicy

  template <typename Behaviour>
  struct AsterTraits {
    static constexpr AsterBehaviourType btype =
        aster::SMALLSTRAINSTANDARDBEHAVIOUR;
    // size of the driving variable array (STRAN)
    static constexpr unsigned short DrivingVariableSize = 0u;
    // size of the thermodynamic force variable array (STESS)
    static constexpr unsigned short ThermodynamicForceVariableSize = 0u;
    static constexpr AsterErrorReportPolicy errorReportPolicy =
        ASTER_WRITEONSTDOUT;
    static constexpr bool requiresStiffnessTensor = false;
    static constexpr bool requiresThermalExpansionCoefficientTensor = false;
    static constexpr unsigned short propertiesOffset = 0u;
    static constexpr unsigned short material_properties_nb = 0u;
    static constexpr unsigned short elasticPropertiesOffset = 0u;
    static constexpr unsigned short thermalExpansionPropertiesOffset = 0u;
    //! \brief type of the behaviour of the material (isotropic or orthotropic)
    static constexpr AsterSymmetryType type = aster::ISOTROPIC;
    // Type of the elastic behaviour of the material (isotropic or orthotropic)
    static constexpr AsterSymmetryType etype = aster::ISOTROPIC;
  };  // end of class ASTERTraits

}  // end of namespace aster

#endif /* LIB_MFRONT_ASTERTRAITS_HXX_ */
