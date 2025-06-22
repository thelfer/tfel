/*!
 * \file   include/MFront/BehaviourBrick/PorosityNucleationModel.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_POROSITYNUCLEATIONMODEL_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_POROSITYNUCLEATIONMODEL_HXX

#include <memory>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/BehaviourDescription.hxx"

namespace tfel::utilities {
  // forward declaration
  struct Data;
  //! \brief a simple alias
  using DataMap = std::map<std::string, Data, std::less<>>;
}  // end of namespace tfel::utilities

namespace mfront {
  // forward declaration
  struct AbstractBehaviourDSL;
}  // end of namespace mfront

namespace mfront::bbrick {

  // forward declaration
  struct StressPotential;
  // forward declaration
  struct InelasticFlow;
  // forward declaration
  struct OptionDescription;

  //! \brief class describing an inelastic flow.
  struct MFRONT_VISIBILITY_EXPORT PorosityNucleationModel {
    //! \brief a simple alias
    using Data = tfel::utilities::Data;
    //! \brief a simple alias
    using DataMap = tfel::utilities::DataMap;
    //! \brief a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    /*!
     * \return the name of a variable from a base name and the nucleation
     * model id.
     * \param[in] n: base name
     * \param[in] id: nucleation model id
     */
    static std::string getVariableId(const std::string&, const std::string&);
    /*!
     * \param[in,out] bd: behaviour description
     * \param[in,out] dsl: abstract behaviour dsl
     * \param[in] id: porosity nucleation model id
     * \param[in] d: options
     */
    virtual void initialize(BehaviourDescription&,
                            AbstractBehaviourDSL&,
                            const std::string&,
                            const DataMap&) = 0;
    //! \return the flow options
    virtual std::vector<OptionDescription> getOptions() const = 0;
    /*!
     * \brief complete the variable description
     * \param[in/out] bd: behaviour description
     * \param[in] dsl: abstract behaviour dsl
     * \param[in] iflows: list of inelastic flows and their associated ids
     * \param[in] id: porosity nucleation model id
     */
    virtual void completeVariableDeclaration(
        BehaviourDescription&,
        const AbstractBehaviourDSL&,
        const std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>&,
        const std::string&) const = 0;
    /*!
     * \brief method called at the end of the input file processing
     * \param[in] dsl: abstract behaviour dsl
     * \param[in] bd: behaviour description
     * \param[in] sp: stress potential
     * \param[in] iflows: list of inelastic flows and their associated ids
     * \param[in] id: porosity nucleation model id
     */
    virtual void endTreatment(
        BehaviourDescription&,
        const AbstractBehaviourDSL&,
        const StressPotential&,
        const std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>&,
        const std::string&) const = 0;
    /*!
     * \return the code updating the next estimate of the porosity
     * increment with the contribution of this nucleation model.
     * \param[in] bd: behaviour description
     * \param[in] iflows: list of inelastic flows and their associated ids
     * \param[in] id: porosity nucleation model id
     */
    virtual std::string updateNextEstimateOfThePorosityIncrement(
        const BehaviourDescription&,
        const std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>&,
        const std::string&) const = 0;
    //! destructor
    virtual ~PorosityNucleationModel();

  };  // end of struct PorosityNucleationModel

}  // end of namespace mfront::bbrick

#endif /* LIB_MFRONT_BEHAVIOURBRICK_POROSITYNUCLEATIONMODEL_HXX */
