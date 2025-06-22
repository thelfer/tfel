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
     * constructor
     * \param[in] wb : wrapped behaviour
     */
    LogarithmicStrain1DBehaviourWrapper(const std::shared_ptr<Behaviour>&);
    //! \return the modelling hypothesis
    virtual Hypothesis getHypothesis() const override;
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
    //! \return the type of the behaviour
    virtual BehaviourType getBehaviourType() const override;
    //! \return the type of the behaviour
    virtual Kinematic getBehaviourKinematic() const override;
    /*!
     * \return the size of a vector able to contain all the components
     * of the driving variables
     */
    virtual unsigned short getDrivingVariablesSize() const override;
    /*!
     * \param[out] v : initial values of the driving variables
     * \note : the vector shall have been correctly allocated
     */
    virtual void getDrivingVariablesDefaultInitialValues(
        tfel::math::vector<real>&) const override;
    /*!
     * \return the size of a vector able to contain all the components of the
     * thermodynamic forces
     */
    virtual unsigned short getThermodynamicForcesSize() const override;
    /*!
     * \return the components suffixes of a symmetric tensor
     */
    virtual std::vector<std::string> getStensorComponentsSuffixes()
        const override;
    /*!
     * \return the components suffixes of a vector
     */
    virtual std::vector<std::string> getVectorComponentsSuffixes()
        const override;
    /*!
     * \return the components suffixes of a tensor
     */
    virtual std::vector<std::string> getTensorComponentsSuffixes()
        const override;
    /*!
     * \return the components of the driving variables
     */
    virtual std::vector<std::string> getDrivingVariablesComponents()
        const override;
    /*!
     * \return the components of the thermodynamic forces
     */
    virtual std::vector<std::string> getThermodynamicForcesComponents()
        const override;
    /*!
     * \param[in] c : component
     */
    virtual unsigned short getDrivingVariableComponentPosition(
        const std::string&) const override;
    /*!
     * \param[in] c : component
     */
    virtual unsigned short getThermodynamicForceComponentPosition(
        const std::string&) const override;
    /*!
     * \return the type of the behaviour
     * 0 means that the behaviour is isotropic.
     * 1 means that the behaviour is orthotropic.
     */
    virtual unsigned short getSymmetryType() const override;
    /*!
     * \return the number of material properties
     */
    virtual std::vector<std::string> getMaterialPropertiesNames()
        const override;
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
    virtual void setOptionalMaterialPropertiesDefaultValues(
        EvolutionManager&, const EvolutionManager&) const override;
    /*!
     * \return the number of internal variables
     */
    virtual std::vector<std::string> getInternalStateVariablesNames()
        const override;
    //! \return expand the names of internal variables
    virtual std::vector<std::string> expandInternalStateVariablesNames()
        const override;
    //! \return the size of the array of internal variables
    virtual size_t getInternalStateVariablesSize() const override;
    //! \return the descriptions the internal variables
    virtual std::vector<std::string> getInternalStateVariablesDescriptions()
        const override;
    /*!
     * \return the type of an internal variable
     * \param[in] n : internal variable name
     */
    virtual unsigned short getInternalStateVariableType(
        const std::string&) const override;
    /*!
     * \return the position of an internal variable
     * \param[in] n : internal variable name
     */
    virtual unsigned short getInternalStateVariablePosition(
        const std::string&) const override;
    //! \return the number of external variables
    virtual std::vector<std::string> getExternalStateVariablesNames()
        const override;
    //! \return the names of floating point parameters
    virtual std::vector<std::string> getParametersNames() const override;
    //! \return the names of integer parameters
    virtual std::vector<std::string> getIntegerParametersNames() const override;
    //! \return the names of unsigned short parameters
    virtual std::vector<std::string> getUnsignedShortParametersNames()
        const override;
    /*!
     * \return the default value of a real parameter
     * \param[in] p: parameter name
     */
    virtual double getRealParameterDefaultValue(
        const std::string&) const override;
    /*!
     * \return the default value of an integer parameter
     * \param[in] p: parameter name
     */
    virtual int getIntegerParameterDefaultValue(
        const std::string&) const override;
    /*!
     * \return the default value of an integer parameter
     * \param[in] p: parameter name
     */
    virtual unsigned short getUnsignedShortParameterDefaultValue(
        const std::string&) const override;
    /*!
     * \brief set the out of bounds policy
     * \param[in] p : policy selected
     */
    virtual void setOutOfBoundsPolicy(
        const tfel::material::OutOfBoundsPolicy) const override;
    /*!
     * \return true if the given variable has bounds
     * \param[in] v: variable name
     */
    virtual bool hasBounds(const std::string&) const override;
    /*!
     * \return true if the given variable has a lower bound
     * \param[in] v: variable name
     */
    virtual bool hasLowerBound(const std::string&) const override;
    /*!
     * \return true if the given variable has a upper bound
     * \param[in] v: variable name
     */
    virtual bool hasUpperBound(const std::string&) const override;
    /*!
     * \return the lower bound of the given variable
     * \param[in] v: variable name
     */
    virtual long double getLowerBound(const std::string&) const override;
    /*!
     * \return the upper bound of the given variable
     * \param[in] v: variable name
     */
    virtual long double getUpperBound(const std::string&) const override;
    /*!
     * \return true if the given variable has bounds
     * \param[in] v: variable name
     */
    virtual bool hasPhysicalBounds(const std::string&) const override;
    /*!
     * \return true if the given variable has a lower physical bound
     * \param[in] v: variable name
     */
    virtual bool hasLowerPhysicalBound(const std::string&) const override;
    /*!
     * \return true if the given variable has a upper physical bound
     * \param[in] v: variable name
     */
    virtual bool hasUpperPhysicalBound(const std::string&) const override;
    /*!
     * \return the lower bound of the given variable
     * \param[in] v: variable name
     */
    virtual long double getLowerPhysicalBound(
        const std::string&) const override;
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
    virtual void setParameter(const std::string&, const real) const override;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    virtual void setIntegerParameter(const std::string&,
                                     const int) const override;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    virtual void setUnsignedIntegerParameter(
        const std::string&, const unsigned short) const override;
    /*!
     * \brief allocate workspace
     * \param[out] wk : behaviour workspace
     */
    virtual void allocate(BehaviourWorkSpace&) const override;
    /*!
     * \return the default type of stiffness matrix used by the behaviour
     */
    virtual StiffnessMatrixType getDefaultStiffnessMatrixType() const override;
    /*!
     * \brief compute the *real* rotation matrix
     * \param[in] mp : material properties
     * \param[in] r  : rotation matrix defined by the user
     * \note this method is only meaningfull for the umat (Cast3M)
     * interface
     */
    virtual tfel::math::tmatrix<3u, 3u, real> getRotationMatrix(
        const tfel::math::vector<real>&,
        const tfel::math::tmatrix<3u, 3u, real>&) const override;
    /*!
     * \brief execute the packaging step. This victious step is done
     * at the beginning of the computation.
     * \return a boolean
     * \param[out] wk : behaviour workspace
     * \param[in] s   : current state
     */
    virtual bool doPackagingStep(CurrentState&,
                                 BehaviourWorkSpace&) const override;
    /*!
     * \brief compute the prediction operator at the beginning of the
     * time step.
     * \return a pair. The first member is true if the integration was
     * successfull, false otherwise. The second member contains a time
     * step scaling factor.
     * \param[out] wk    : behaviour workspace
     * \param[in]  s     : current state
     * \param[in]  ktype : type of the stiffness matrix
     */
    virtual std::pair<bool, real> computePredictionOperator(
        BehaviourWorkSpace&,
        const CurrentState&,
        const StiffnessMatrixType) const override;
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
    virtual std::pair<bool, real> integrate(
        CurrentState&,
        BehaviourWorkSpace&,
        const real,
        const StiffnessMatrixType) const override;
    //! destructor
    virtual ~LogarithmicStrain1DBehaviourWrapper();

   private:
    //! wrapped behaviour
    std::shared_ptr<Behaviour> b;
  };  // end of struct LogarithmicStrain1DBehaviourWrapper

}  // end of namespace mtest

#endif /* LIB_MTEST_LOGARITHMICSTRAIN1DBEHAVIOURWRAPPER_HXX */
