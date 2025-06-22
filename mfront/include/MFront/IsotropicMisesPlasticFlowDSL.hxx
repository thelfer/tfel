/*!
 * \file   mfront/include/MFront/IsotropicMisesPlasticFlowDSL.hxx
 * \brief  This file declares the IsotropicMisesPlasticFlowDSL class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTISOTROPICPLASTICFLOWPARSER_H_
#define LIB_MFRONTISOTROPICPLASTICFLOWPARSER_H_

#include <string>
#include "MFront/IsotropicBehaviourDSLBase.hxx"

namespace mfront {

  /*!
   * \brief a dsl dedicated to isotropic mises plastic flows
   */
  struct IsotropicMisesPlasticFlowDSL : public IsotropicBehaviourDSLBase {
    //! \return the name of the dsl
    static std::string getName(void);
    //! \return a description of the dsl
    static std::string getDescription(void);

    virtual void endsInputFileProcessing(void) override;

    virtual void writeBehaviourParserSpecificInitializeMethodPart(
        const Hypothesis) override;

    virtual void writeBehaviourParserSpecificMembers(const Hypothesis) override;

    virtual void writeBehaviourIntegrator(const Hypothesis) override;

    virtual void writeBehaviourComputeTangentOperator(
        const Hypothesis) override;
    //! constructor
    IsotropicMisesPlasticFlowDSL();
    //! desctructor
    virtual ~IsotropicMisesPlasticFlowDSL();
  };

}  // end of namespace mfront

#endif /* LIB_MFRONTISOTROPICPLASTICFLOWPARSER_H_ */
