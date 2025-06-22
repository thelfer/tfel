/*!
 * \file  mtest/include/MTest/StandardBehaviourBase.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTUMATBEHAVIOURBASE_HXX
#define LIB_MTEST_MTESTUMATBEHAVIOURBASE_HXX

#include "TFEL/Math/matrix.hxx"
#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "TFEL/System/ExternalBehaviourDescription.hxx"
#include "MTest/Behaviour.hxx"

namespace mtest {

  /*!
   * a class gathering all the relevant information about an Umat
   * behaviour.
   */
  struct StandardBehaviourDescription : tfel::system::ExternalBehaviourData {
    /*!
     * \param[in] l: library name
     * \param[in] b: behaviour name
     * \param[in] h: modelling hypothesis
     */
    StandardBehaviourDescription(const std::string&,
                                 const std::string&,
                                 const std::string&);
    StandardBehaviourDescription();
    StandardBehaviourDescription(StandardBehaviourDescription&&);
    StandardBehaviourDescription(const StandardBehaviourDescription&);
    StandardBehaviourDescription& operator=(StandardBehaviourDescription&&);
    StandardBehaviourDescription& operator=(
        const StandardBehaviourDescription&);
  };

  /*!
   * A class to handle mechanical beheaviours written using the umat
   * interface
   */
  struct StandardBehaviourBase : public Behaviour,
                                 public StandardBehaviourDescription {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    StandardBehaviourBase(const Hypothesis,
                          const std::string&,
                          const std::string&);
    //! \param[in] umb: behaviour description
    StandardBehaviourBase(const StandardBehaviourDescription&);
    Hypothesis getHypothesis() const override;
    BehaviourType getBehaviourType() const override;
    Kinematic getBehaviourKinematic() const override;
    unsigned short getGradientsSize() const override;
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
    /*!
     * \return the upper bound of the given variable
     * \param[in] v: variable name
     */
    virtual long double getUpperPhysicalBound(
        const std::string&) const override;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    void setParameter(const std::string&, const real) const override;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    void setIntegerParameter(const std::string&, const int) const override;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    void setUnsignedIntegerParameter(const std::string&,
                                     const unsigned short) const override;
    /*!
     * \brief some interfaces requires dummy material properties to be
     * declared. For example, the Cast3M finite element solver
     * requires the mass density and some extra material properties
     * describing orthotropic axes to be declared.  This method is
     * meant to automatically declare those if they are not defined by
     * the user.
     * \param[out] mp  : evolution manager where
     * \param[in]  evm : evolution manager
     */
    void setOptionalMaterialPropertiesDefaultValues(
        EvolutionManager&, const EvolutionManager&) const override;
    /*!
     * \brief execute the packaging step. This victious step is done
     * at the beginning of the computation.
     * \return a boolean
     * \param[out] wk : behaviour workspace
     * \param[in] s   : current state
     */
    bool doPackagingStep(CurrentState&, BehaviourWorkSpace&) const override;
    //! destructor
    ~StandardBehaviourBase() override;

   protected:
    /*!
     * \brief initialize the first value of the tangent operator
     * \param[out] wk    : behaviour workspace
     * \param[in]  ktype : requested tangent operator type
     * \param[in]  b     : if false, a prediction operator is requested
     */
    void initializeTangentOperator(tfel::math::matrix<real>&,
                                   const StiffnessMatrixType,
                                   const bool) const;
  };  // end of struct Behaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTUMATBEHAVIOURBASE_HXX */
