/*!
 * \file   mtest/include/MTest/AsterLogarithmicStrainBehaviourWrapper.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04/01/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_ASTERLOGARITHMICSTRAINBEHAVIOURWRAPPER_HXX
#define LIB_MTEST_ASTERLOGARITHMICSTRAINBEHAVIOURWRAPPER_HXX

#include "MTest/BehaviourWrapperBase.hxx"

namespace mtest {

  // forward declaration
  struct AsterSmallStrainBehaviour;

  /*!
   * \brief behaviour wrapper used to handle behaviours using the logarithmic
   * strain framework. The wrapped behaviour appears as a standard finite strain
   * behaviour to `MTest`.
   */
  struct AsterLogarithmicStrainBehaviourWrapper : public BehaviourWrapperBase {
    /*!
     * \brief constructor
     * \param[in] wb : wrapped behaviour
     */
    AsterLogarithmicStrainBehaviourWrapper(
        const std::shared_ptr<AsterSmallStrainBehaviour>&);
    Hypothesis getHypothesis() const override;
    BehaviourType getBehaviourType() const override;
    Kinematic getBehaviourKinematic() const override;
    unsigned short getGradientsSize() const override;
    void getGradientsDefaultInitialValues(
        tfel::math::vector<real>&) const override;
    unsigned short getThermodynamicForcesSize() const override;
    std::vector<std::string> getStensorComponentsSuffixes() const override;
    std::vector<std::string> getVectorComponentsSuffixes() const override;
    std::vector<std::string> getTensorComponentsSuffixes() const override;
    std::vector<std::string> getGradientsComponents() const override;
    std::vector<std::string> getThermodynamicForcesComponents() const override;
    unsigned short getGradientComponentPosition(
        const std::string&) const override;
    unsigned short getThermodynamicForceComponentPosition(
        const std::string&) const override;
    size_t getTangentOperatorArraySize() const override;
    std::vector<std::pair<std::string, std::string>> getTangentOperatorBlocks()
        const override;
    std::vector<std::string> getInternalStateVariablesNames() const override;
    std::vector<std::string> expandInternalStateVariablesNames() const override;
    size_t getInternalStateVariablesSize() const override;
    std::vector<std::string> getInternalStateVariablesDescriptions()
        const override;
    unsigned short getInternalStateVariableType(
        const std::string&) const override;
    unsigned short getInternalStateVariablePosition(
        const std::string&) const override;
    //
    std::vector<std::string> getExternalStateVariablesNames() const override;
    std::vector<std::string> expandExternalStateVariablesNames() const override;
    size_t getExternalStateVariablesSize() const override;
    unsigned short getExternalStateVariableType(
        const std::string&) const override;
    unsigned short getExternalStateVariablePosition(
        const std::string&) const override;
    void allocateCurrentState(CurrentState& s) const override;
    void allocateWorkSpace(BehaviourWorkSpace&) const override;
    tfel::math::tmatrix<3u, 3u, real> getRotationMatrix(
        const tfel::math::vector<real>&,
        const tfel::math::tmatrix<3u, 3u, real>&) const override;
    bool doPackagingStep(CurrentState&, BehaviourWorkSpace&) const override;
    std::pair<bool, real> computePredictionOperator(
        BehaviourWorkSpace&,
        const CurrentState&,
        const StiffnessMatrixType) const override;
    std::pair<bool, real> integrate(CurrentState&,
                                    BehaviourWorkSpace&,
                                    const real,
                                    const StiffnessMatrixType) const override;
    //! \brief destructor
    virtual ~AsterLogarithmicStrainBehaviourWrapper() override;

   private:
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return a pair. The first member is true if the integration was
     * successfull, false otherwise. The second member contains a time
     * step scaling factor.
     * \param[out/in] s     : current state
     * \param[out]    wk    : behaviour workspace
     * \param[in]     dt    : time increment
     * \param[in]     ktype : type of the stiffness matrix
     */
    template <unsigned N>
    TFEL_VISIBILITY_LOCAL std::pair<bool, real> do_integrate(
        CurrentState&,
        BehaviourWorkSpace&,
        const real,
        const StiffnessMatrixType) const;

  };  // end of struct AsterLogarithmicStrainBehaviourWrapper

}  // end of namespace mtest

#endif /* LIB_MTEST_ASTERLOGARITHMICSTRAINBEHAVIOURWRAPPER_HXX */
