/*!
 * \file
 * mfront/include/MFront/IsotropicStrainHardeningMisesCreepCodeGenerator.hxx
 * \brief
 * \author Thomas Helfer
 * \date   03/08/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ISOTROPICSTRAINHARDENINGMISESCREEPCODEGENERATOR_HXX
#define LIB_MFRONT_ISOTROPICSTRAINHARDENINGMISESCREEPCODEGENERATOR_HXX

#include "MFront/IsotropicBehaviourCodeGeneratorBase.hxx"

namespace mfront {

  struct IsotropicStrainHardeningMisesCreepCodeGenerator
      : IsotropicBehaviourCodeGeneratorBase {
    /*!
     * \brief constructor
     * \param[in] f: file description
     * \param[in] d: behaviour description
     * \param[in] bim: behaviour interfaces
     */
    IsotropicStrainHardeningMisesCreepCodeGenerator(
        const FileDescription&,
        const BehaviourDescription&,
        const BehaviourInterfaceMap&);
    //! \brief destructor
    ~IsotropicStrainHardeningMisesCreepCodeGenerator() override;

   protected:
    void writeBehaviourParserSpecificMembers(std::ostream&,
                                             const Hypothesis) const override;
    void writeBehaviourIntegrator(std::ostream&,
                                  const Hypothesis) const override;
    void writeBehaviourComputeTangentOperator(std::ostream&,
                                              const Hypothesis) const override;
  };  // end of IsotropicStrainHardeningMisesCreepCodeGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_ISOTROPICSTRAINHARDENINGMISESCREEPCODEGENERATOR_HXX */
