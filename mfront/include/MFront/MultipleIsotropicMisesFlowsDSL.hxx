/*!
 * \file   mfront/include/MFront/MultipleIsotropicMisesFlowsDSL.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   31 jan 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTMULTIPLEISOTROPICMISESFLOWSPARSER_HXX
#define LIB_MFRONTMULTIPLEISOTROPICMISESFLOWSPARSER_HXX

#include <vector>
#include <string>

#include "MFront/IsotropicBehaviourDSLBase.hxx"

namespace mfront {

  struct MultipleIsotropicMisesFlowsDSL : public IsotropicBehaviourDSLBase {
    //! \brief return the name of the DSL
    static std::string getName();
    //! \brief return a short description of the DSL
    static std::string getDescription();
    //! \brief default constructor
    MultipleIsotropicMisesFlowsDSL();

    BehaviourDSLDescription getBehaviourDSLDescription() const override;

    void getSymbols(std::map<std::string, std::string>&,
                    const Hypothesis,
                    const std::string&) override;

    std::string getCodeBlockTemplate(
        const std::string&,
        const MFrontTemplateGenerationOptions&) const override;

    //! \brief destructor
    ~MultipleIsotropicMisesFlowsDSL() override;

   private:
    void writeBehaviourParserSpecificInitializeMethodPart(
        std::ostream&, const Hypothesis) const override;

    void writeBehaviourParserSpecificMembers(std::ostream&,
                                             const Hypothesis) const override;

    void writeBehaviourParserSpecificIncludes(std::ostream&) const override;

    void writeBehaviourIntegrator(std::ostream&,
                                  const Hypothesis) const override;

    void writeBehaviourComputeTangentOperator(std::ostream&,
                                              const Hypothesis) const override;

    void treatFlowRule() override;

    struct FlowHandler {
      enum FlowType {
        PlasticFlow,
        CreepFlow,
        StrainHardeningCreepFlow
      };  // end of enum FlowType
      FlowHandler() = default;
      FlowHandler(FlowHandler&&) = default;
      FlowHandler(const FlowHandler&) = default;
      FlowHandler& operator=(FlowHandler&&) = default;
      FlowHandler& operator=(const FlowHandler&) = default;
      ~FlowHandler() noexcept;
      std::string flowRule;
      FlowType flow = PlasticFlow;
      double theta = 1.;
      bool hasSpecificTheta = false;
    };  // end of struct LawHandler

    std::vector<FlowHandler> flows;

  };  // end of struct MultipleIsotropicMisesFlowsDSL

}  // end of namespace mfront

#endif /* LIB_MFRONTMULTIPLEISOTROPICMISESFLOWSPARSER_HXX */
