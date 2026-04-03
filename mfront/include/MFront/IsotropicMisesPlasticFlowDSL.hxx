/*!
 * \file   mfront/include/MFront/IsotropicMisesPlasticFlowDSL.hxx
 * \brief  This file declares the IsotropicMisesPlasticFlowDSL class
 * \author Thomas Helfer
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTISOTROPICPLASTICFLOWPARSER_HXX
#define LIB_MFRONTISOTROPICPLASTICFLOWPARSER_HXX

#include <string>
#include "MFront/IsotropicBehaviourDSLBase.hxx"

namespace mfront {

  /*!
   * \brief a dsl dedicated to isotropic mises plastic flows
   */
  struct IsotropicMisesPlasticFlowDSL : public IsotropicBehaviourDSLBase {
    //! \return the name of the dsl
    static std::string getName();
    //! \return a description of the dsl
    static std::string getDescription();
    /*!
     * \brief constructor
     *\param[in] opts: options passed to the DSL
     */
    IsotropicMisesPlasticFlowDSL(const DSLOptions&);
    //
    std::string getCodeBlockTemplate(
        const std::string&,
        const MFrontTemplateGenerationOptions&) const override;
    void endsInputFileProcessing() override;
    //! \brief destructor
    ~IsotropicMisesPlasticFlowDSL() override;

   protected:
    std::unique_ptr<AbstractBehaviourCodeGenerator> getCodeGenerator()
        const override;
    double getDefaultThetaValue() const override;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONTISOTROPICPLASTICFLOWPARSER_HXX */
