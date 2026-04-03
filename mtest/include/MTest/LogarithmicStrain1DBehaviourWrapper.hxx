/*!
 * \file   LogarithmicStrain1DBehaviourWrapper.hxx
 * \brief
 * \author Thomas Helfer
 * \date   28/12/2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_LOGARITHMICSTRAIN1DBEHAVIOURWRAPPER_HXX
#define LIB_MTEST_LOGARITHMICSTRAIN1DBEHAVIOURWRAPPER_HXX

#include <memory>
#include "MTest/Config.hxx"
#include "MTest/BehaviourWrapperBase.hxx"

namespace mtest {

  /*!
   * \brief a simple wrapper around a small strain behaviour which turns a small
   * strain behaviour into a finite strain behaviour in the logarithmic strain
   * framework using the ETO-PK1 kinematic assumption under the axisymmetrical
   * generalised plane strain modelling hypothesis.
   */
  struct MTEST_VISIBILITY_EXPORT LogarithmicStrain1DBehaviourWrapper
      : public BehaviourWrapperBase {
    /*!
     * \brief constructor
     * \param[in] wb : wrapped behaviour
     */
    LogarithmicStrain1DBehaviourWrapper(const std::shared_ptr<Behaviour>&);
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
    ~LogarithmicStrain1DBehaviourWrapper() override;
  };  // end of struct LogarithmicStrain1DBehaviourWrapper

}  // end of namespace mtest

#endif /* LIB_MTEST_LOGARITHMICSTRAIN1DBEHAVIOURWRAPPER_HXX */
