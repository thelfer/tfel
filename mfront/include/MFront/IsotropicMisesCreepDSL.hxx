/*!
 * \file   mfront/include/MFront/IsotropicMisesCreepDSL.hxx
 * \brief  This file declares the IsotropicMisesCreepDSL class
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTISOTROPICMISESCREEPPARSER_HXX
#define LIB_MFRONTISOTROPICMISESCREEPPARSER_HXX

#include <string>
#include "MFront/IsotropicBehaviourDSLBase.hxx"

namespace mfront {

  struct IsotropicMisesCreepDSL : public IsotropicBehaviourDSLBase {
    static std::string getName();

    static std::string getDescription();

    IsotropicMisesCreepDSL();

    virtual void endsInputFileProcessing() override;

    virtual ~IsotropicMisesCreepDSL();

   protected:
    virtual void writeBehaviourParserSpecificInitializeMethodPart(
        std::ostream&, const Hypothesis) const override;

    virtual void writeBehaviourParserSpecificMembers(
        std::ostream&, const Hypothesis) const override;

    virtual void writeBehaviourIntegrator(std::ostream&,
                                          const Hypothesis) const override;

    virtual void writeBehaviourComputeTangentOperator(
        std::ostream&, const Hypothesis) const override;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONTISOTROPICMISESCREEPPARSER_HXX */
