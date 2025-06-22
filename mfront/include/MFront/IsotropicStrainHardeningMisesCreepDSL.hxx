/*!
 * \file   mfront/include/MFront/IsotropicStrainHardeningMisesCreepDSL.hxx
 * \brief  This file declares the IsotropicStrainHardeningMisesCreepDSL class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTISOTROPICSTRAINHARDENINGMISESCREEPPARSER_H_
#define LIB_MFRONTISOTROPICSTRAINHARDENINGMISESCREEPPARSER_H_

#include <string>
#include "MFront/IsotropicBehaviourDSLBase.hxx"

namespace mfront {

  struct IsotropicStrainHardeningMisesCreepDSL
      : public IsotropicBehaviourDSLBase {
    static std::string getName(void);

    static std::string getDescription(void);

    virtual void endsInputFileProcessing(void) override;

    virtual void writeBehaviourParserSpecificInitializeMethodPart(
        const Hypothesis) override;

    virtual void writeBehaviourParserSpecificMembers(const Hypothesis) override;

    virtual void writeBehaviourIntegrator(const Hypothesis) override;

    virtual void writeBehaviourComputeTangentOperator(
        const Hypothesis) override;

    IsotropicStrainHardeningMisesCreepDSL();

    virtual ~IsotropicStrainHardeningMisesCreepDSL();
  };

}  // end of namespace mfront

#endif /* LIB_MFRONTISOTROPICSTRAINHARDENINGMISESCREEPPARSER_H_ */
