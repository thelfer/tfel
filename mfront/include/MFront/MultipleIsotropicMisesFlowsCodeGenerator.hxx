/*!
 * \file   mfront/include/MFront/MultipleIsotropicMisesFlowsCodeGenerator.hxx
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

#ifndef LIB_MFRONT_MULTIPLEISOTROPICMISESFLOWSCODEGENERATOR_HXX
#define LIB_MFRONT_MULTIPLEISOTROPICMISESFLOWSCODEGENERATOR_HXX

#include "MFront/MultipleIsotropicMisesFlowsDSL.hxx"
#include "MFront/IsotropicBehaviourCodeGeneratorBase.hxx"

namespace mfront {

  struct MultipleIsotropicMisesFlowsCodeGenerator
      : IsotropicBehaviourCodeGeneratorBase {
    //! \brief a simple alias
    using FlowHandler = MultipleIsotropicMisesFlowsDSL::FlowHandler;
    /*!
     * \brief constructor
     * \param[in] f: file description
     * \param[in] d: behaviour description
     * \param[in] bim: behaviour interfaces
     * \param[in] fhs: flow handlers
     */
    MultipleIsotropicMisesFlowsCodeGenerator(const FileDescription&,
                                             const BehaviourDescription&,
                                             const BehaviourInterfaceMap&,
                                             const std::vector<FlowHandler>&);
    //! \brief destructor
    ~MultipleIsotropicMisesFlowsCodeGenerator() override;

   protected:
    void writeBehaviourParserSpecificInitializeMethodPart(
        std::ostream&, const Hypothesis) const override;
    void writeBehaviourParserSpecificMembers(std::ostream&,
                                             const Hypothesis) const override;
    void writeBehaviourParserSpecificIncludes(std::ostream&) const override;
    void writeBehaviourIntegrator(std::ostream&,
                                  const Hypothesis) const override;
    void writeBehaviourComputeTangentOperator(std::ostream&,
                                              const Hypothesis) const override;
#pragma message("shall be removed, this is ugly !")
    const std::vector<MultipleIsotropicMisesFlowsDSL::FlowHandler>& flows;
  };  // end of MultipleIsotropicMisesFlowsCodeGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_MULTIPLEISOTROPICMISESFLOWSCODEGENERATOR_HXX */
