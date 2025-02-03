/*!
 * \file   mfront/include/MFront/IsotropicBehaviourDSLBase.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   01 jui 2007
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX
#define LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX

#include <string>
#include <string_view>

#include "MFront/MFrontConfig.hxx"
#include "MFront/BehaviourDSLBase.hxx"

namespace mfront {

  struct MFRONT_VISIBILITY_EXPORT IsotropicBehaviourDSLBase
      : public BehaviourDSLBase<IsotropicBehaviourDSLBase> {
    /*!
     * \brief constructor
     *\param[in] opts: options passed to the DSL
     */
    IsotropicBehaviourDSLBase(const DSLOptions&);
    //
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

    virtual double getDefaultThetaValue() const;

    virtual void treatFlowRule();

    virtual void treatTheta();

    virtual void treatEpsilon();

    virtual void treatIterMax();

    virtual std::string flowRuleVariableModifier(const Hypothesis,
                                                 const std::string&,
                                                 const bool);

    /*!
     * \brief performs some check on the given code block describing a flow rule
     * \param[in] n: code block name
     */
    virtual void checkFlowRule(std::string_view) const;

   private:
    friend struct BehaviourDSLBase<IsotropicBehaviourDSLBase>;

  };  // end of class IsotropicBehaviourDSLBase

}  // namespace mfront

#endif /* LIB_MFRONTISOTROPICBEHAVIOURPARSERBASE_HXX */
