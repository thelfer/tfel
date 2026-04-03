/*!
 * \file   mfront/include/MFront/Aster/AsterTraits.hxx
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

#ifndef LIB_MFRONT_ASTERTRAITS_HXX
#define LIB_MFRONT_ASTERTRAITS_HXX

namespace aster {

  enum AsterBehaviourType {
    STANDARDSTRAINBASEDBEHAVIOUR,
    STANDARDFINITESTRAINBEHAVIOUR,
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
  /*!
   * \brief list of the finite strain formulation available.
   */
  enum AsterFiniteStrainFormulation {
    //! Formulation introduced by Simo and Miehe (the default)
    SIMO_MIEHE,
    //! Total lagrangian formulation
    GROT_GDEP,
    /*!
     * \brief entry used when the finite strain formulation is
     * meaningless (e.g. small strain behaviour).
     */
    UNDEFINEDFINITESTRAINFORMULATION
  };  // end of enum AsterFiniteStrainFormulation

  /*!
   * \brief The AsterTraits class defines a set of fields used for
   * adapting the AsterInterface.
   */
  template <typename Behaviour>
  struct AsterTraits {
    static constexpr AsterBehaviourType btype =
        aster::STANDARDSTRAINBASEDBEHAVIOUR;
    // size of the driving variable array (STRAN)
    static constexpr unsigned short GradientSize = 0u;
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
    //! \brief type of the elastic behaviour of the material (isotropic or
    //! orthotropic)
    static constexpr AsterSymmetryType etype = aster::ISOTROPIC;
    //! \brief finite strain formulation used
    static constexpr AsterFiniteStrainFormulation afsf =
        aster::UNDEFINEDFINITESTRAINFORMULATION;
  };  // end of class ASTERTraits

}  // end of namespace aster

#endif /* LIB_MFRONT_ASTERTRAITS_HXX */
