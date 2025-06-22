/*!
 * \file   LogarithmicStrain1DBehaviourWrapper.hxx
 * \brief
 * \author Thomas Helfer
 * \date   28 déc. 2015
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
#include "MTest/Behaviour.hxx"

namespace mtest {

  //! A simple wrapper around mechanical behaviours
  struct LogarithmicStrain1DBehaviourWrapper : public Behaviour {
    /*!
     * \brief This function set a material property to its default value if it
     * not already declared \param[out] mp  : evolution manager where \param[in]
     * evm : evolution manager \param[in]  n   : material property name
     * \param[in]  v   : default value
     */
    static void setOptionalMaterialPropertyDefaultValue(EvolutionManager&,
                                                        const EvolutionManager&,
                                                        const std::string&,
                                                        const real);
    /*!
     * constructor
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
    unsigned short getSymmetryType() const override;
    std::vector<std::string> getMaterialPropertiesNames() const override;
    size_t getMaterialPropertiesSize() const override;
    void setOptionalMaterialPropertiesDefaultValues(
        EvolutionManager&, const EvolutionManager&) const override;
    std::vector<std::string> getInternalStateVariablesNames() const override;
    std::vector<std::string> expandInternalStateVariablesNames() const override;
    size_t getInternalStateVariablesSize() const override;
    std::vector<std::string> getInternalStateVariablesDescriptions()
        const override;
    unsigned short getInternalStateVariableType(
        const std::string&) const override;
    unsigned short getInternalStateVariablePosition(
        const std::string&) const override;
    std::vector<std::string> getExternalStateVariablesNames() const override;
    size_t getExternalStateVariablesSize() const override;
    std::vector<std::string> getParametersNames() const override;
    std::vector<std::string> getIntegerParametersNames() const override;
    std::vector<std::string> getUnsignedShortParametersNames() const override;
    double getRealParameterDefaultValue(const std::string&) const override;
    int getIntegerParameterDefaultValue(const std::string&) const override;
    unsigned short getUnsignedShortParameterDefaultValue(
        const std::string&) const override;
    void setOutOfBoundsPolicy(
        const tfel::material::OutOfBoundsPolicy) const override;
    bool hasBounds(const std::string&) const override;
    bool hasLowerBound(const std::string&) const override;
    bool hasUpperBound(const std::string&) const override;
    long double getLowerBound(const std::string&) const override;
    long double getUpperBound(const std::string&) const override;
    bool hasPhysicalBounds(const std::string&) const override;
    bool hasLowerPhysicalBound(const std::string&) const override;
    bool hasUpperPhysicalBound(const std::string&) const override;
    long double getLowerPhysicalBound(const std::string&) const override;
    long double getUpperPhysicalBound(const std::string&) const override;
    void setParameter(const std::string&, const real) const override;
    void setIntegerParameter(const std::string&, const int) const override;
    void setUnsignedIntegerParameter(const std::string&,
                                     const unsigned short) const override;
    void allocate(BehaviourWorkSpace&) const override;
    StiffnessMatrixType getDefaultStiffnessMatrixType() const override;
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
    //! destructor
    ~LogarithmicStrain1DBehaviourWrapper() override;

   private:
    //! wrapped behaviour
    std::shared_ptr<Behaviour> b;
  };  // end of struct LogarithmicStrain1DBehaviourWrapper

}  // end of namespace mtest

#endif /* LIB_MTEST_LOGARITHMICSTRAIN1DBEHAVIOURWRAPPER_HXX */
