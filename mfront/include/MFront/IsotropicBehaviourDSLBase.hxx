/*!
 * \file   mfront/include/MFront/IsotropicBehaviourDSLBase.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   01 jui 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX
#define LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX

#include <string>

#include "MFront/MFrontConfig.hxx"
#include "MFront/BehaviourDSLBase.hxx"

namespace mfront {

  struct MFRONT_VISIBILITY_EXPORT IsotropicBehaviourDSLBase
      : public BehaviourDSLBase<IsotropicBehaviourDSLBase> {
    //! \brief default constructor
    IsotropicBehaviourDSLBase();

    BehaviourDSLDescription getBehaviourDSLDescription() const override;

    void getSymbols(std::map<std::string, std::string>&,
                    const Hypothesis,
                    const std::string&) override;

    //! \brief destructor
    ~IsotropicBehaviourDSLBase() override;

   protected:
    void treatExternalStateVariable() override;

    void completeVariableDeclaration() override;

    void endsInputFileProcessing() override;

    void writeBehaviourParserSpecificIncludes(std::ostream&) const override;

    void writeBehaviourParserSpecificTypedefs(std::ostream&) const override;

    void writeBehaviourLocalVariablesInitialisation(
        std::ostream&, const Hypothesis) const override;

    void writeBehaviourComputePredictionOperator(
        std::ostream&, const Hypothesis) const override;

    void writeBehaviourComputeTangentOperator(std::ostream&,
                                              const Hypothesis) const override;

    virtual double getDefaultThetaValue() const;

    virtual void treatFlowRule();

    virtual void treatTheta();

    virtual void treatEpsilon();

    virtual void treatIterMax();

    virtual std::string flowRuleVariableModifier(const Hypothesis,
                                                 const std::string&,
                                                 const bool);

   private:
    friend struct BehaviourDSLBase<IsotropicBehaviourDSLBase>;

  };  // end of class IsotropicBehaviourDSLBase

}  // namespace mfront

#endif /* LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX */
