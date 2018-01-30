/*!
 * \file   LogarithmicStrain1DBehaviourWrapper.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   28 déc. 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MTEST_LOGARITHMICSTRAIN1DBEHAVIOURWRAPPER_HXX
#define LIB_MTEST_LOGARITHMICSTRAIN1DBEHAVIOURWRAPPER_HXX

#include<memory>
#include"MTest/Behaviour.hxx"

namespace mtest
{
  
  //! A simple wrapper around mechanical behaviours
  struct LogarithmicStrain1DBehaviourWrapper
    : public Behaviour
  {
    /*!
     * \brief This function set a material property to its default value if it not already declared
     * \param[out] mp  : evolution manager where 
     * \param[in]  evm : evolution manager
     * \param[in]  n   : material property name
     * \param[in]  v   : default value
     */
    static void
    setOptionalMaterialPropertyDefaultValue(EvolutionManager&,
					    const EvolutionManager&,
					    const std::string&,
					    const real);
    /*!
     * constructor
     * \param[in] wb : wrapped behaviour
     */
    LogarithmicStrain1DBehaviourWrapper(const std::shared_ptr<Behaviour>&);
    //! \return the modelling hypothesis 
    Hypothesis getHypothesis() const override;
    //! \return the type of the behaviour
    BehaviourType getBehaviourType() const override;
    //! \return the type of the behaviour
    Kinematic getBehaviourKinematic() const override;
    /*!
     * \return the size of a vector able to contain all the components
     * of the driving variables
     */
    unsigned short getDrivingVariablesSize() const override;
    /*!
     * \param[out] v : initial values of the driving variables
     * \note : the vector shall have been correctly allocated
     */
    void getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>&) const override;
    /*!
     * \return the size of a vector able to contain all the components of the thermodynamic forces
     */
    unsigned short getThermodynamicForcesSize() const override;
    /*!
     * \return the components suffixes of a symmetric tensor
     */
    std::vector<std::string> getStensorComponentsSuffixes() const override;
    /*!
     * \return the components suffixes of a vector
     */
    std::vector<std::string> getVectorComponentsSuffixes() const override;
    /*!
     * \return the components suffixes of a tensor
     */
    std::vector<std::string> getTensorComponentsSuffixes() const override;
    /*!
     * \return the components of the driving variables
     */
    std::vector<std::string> getDrivingVariablesComponents() const override;
    /*!
     * \return the components of the thermodynamic forces
     */
    std::vector<std::string> getThermodynamicForcesComponents() const override;
    /*!
     * \param[in] c : component
     */
    unsigned short getDrivingVariableComponentPosition(const std::string&) const override;
    /*!
     * \param[in] c : component
     */
    unsigned short getThermodynamicForceComponentPosition(const std::string&) const override;
    /*!
     * \return the type of the behaviour
     * 0 means that the behaviour is isotropic.
     * 1 means that the behaviour is orthotropic.
     */
    unsigned short getSymmetryType() const override;
    /*!
     * \return the number of material properties
     */
    std::vector<std::string> getMaterialPropertiesNames() const override;
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
    void setOptionalMaterialPropertiesDefaultValues(EvolutionManager&,
						    const EvolutionManager&) const override;
    /*!
     * \return the number of internal variables
     */
    std::vector<std::string> getInternalStateVariablesNames() const  override;
    //! \return expand the names of internal variables
    std::vector<std::string>  expandInternalStateVariablesNames() const override;
    //! \return the size of the array of internal variables
    size_t getInternalStateVariablesSize() const override;
    //! \return the descriptions the internal variables
    std::vector<std::string> getInternalStateVariablesDescriptions() const override;
    /*!
     * \return the type of an internal variable
     * \param[in] n : internal variable name
     */
    unsigned short getInternalStateVariableType(const std::string&) const override;
    /*!
     * \return the position of an internal variable
     * \param[in] n : internal variable name
     */
    unsigned short getInternalStateVariablePosition(const std::string&) const override;
    //! \return the number of external variables
    std::vector<std::string> getExternalStateVariablesNames() const  override;
    //! \return the names of floating point parameters
    std::vector<std::string> getParametersNames() const override;
    //! \return the names of integer parameters
    std::vector<std::string> getIntegerParametersNames() const override;
    //! \return the names of unsigned short parameters
    std::vector<std::string> getUnsignedShortParametersNames() const override;
    /*!
     * \return the default value of a real parameter
     * \param[in] p: parameter name
     */
    double getRealParameterDefaultValue(const std::string&) const override;
    /*!
     * \return the default value of an integer parameter
     * \param[in] p: parameter name
     */
    int getIntegerParameterDefaultValue(const std::string&) const override;
    /*!
     * \return the default value of an integer parameter
     * \param[in] p: parameter name
     */
    unsigned short getUnsignedShortParameterDefaultValue(const std::string&) const override;
    /*!
     * \brief set the out of bounds policy
     * \param[in] p : policy selected
     */
    void setOutOfBoundsPolicy(const tfel::material::OutOfBoundsPolicy) const  override;
    /*!
     * \return true if the given variable has bounds
     * \param[in] v: variable name
     */
    bool hasBounds(const std::string&) const override;
    /*!
     * \return true if the given variable has a lower bound
     * \param[in] v: variable name
     */
    bool hasLowerBound(const std::string&) const override;
    /*!
     * \return true if the given variable has a upper bound
     * \param[in] v: variable name
     */
    bool hasUpperBound(const std::string&) const override;
    /*!
     * \return the lower bound of the given variable
     * \param[in] v: variable name
     */
    long double getLowerBound(const std::string&) const override;
    /*!
     * \return the upper bound of the given variable
     * \param[in] v: variable name
     */
    long double getUpperBound(const std::string&) const override;
    /*!
     * \return true if the given variable has bounds
     * \param[in] v: variable name
     */
    bool hasPhysicalBounds(const std::string&) const override;
    /*!
     * \return true if the given variable has a lower physical bound
     * \param[in] v: variable name
     */
    bool hasLowerPhysicalBound(const std::string&) const override;
    /*!
     * \return true if the given variable has a upper physical bound
     * \param[in] v: variable name
     */
    bool hasUpperPhysicalBound(const std::string&) const override;
    /*!
     * \return the lower bound of the given variable
     * \param[in] v: variable name
     */
    long double getLowerPhysicalBound(const std::string&) const override;
    /*!
     * \return the upper bound of the given variable
     * \param[in] v: variable name
     */
    long double getUpperPhysicalBound(const std::string&) const override;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    void setParameter(const std::string&,
		      const real) const override;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    void setIntegerParameter(const std::string&,
			     const int) const override;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    void setUnsignedIntegerParameter(const std::string&,
				     const unsigned short) const override;
    /*!
     * \brief allocate workspace
     * \param[out] wk : behaviour workspace
     */
    void allocate(BehaviourWorkSpace&) const override;
    /*!
     * \return the default type of stiffness matrix used by the behaviour
     */
    StiffnessMatrixType getDefaultStiffnessMatrixType() const override;
    /*!
     * \brief compute the *real* rotation matrix
     * \param[in] mp : material properties
     * \param[in] r  : rotation matrix defined by the user
     * \note this method is only meaningfull for the umat (Cast3M)
     * interface
     */
    tfel::math::tmatrix<3u,3u,real>
    getRotationMatrix(const tfel::math::vector<real>&,
		      const tfel::math::tmatrix<3u,3u,real>&) const override;
    /*!
     * \brief execute the packaging step. This victious step is done
     * at the beginning of the computation.
     * \return a boolean
     * \param[out] wk : behaviour workspace
     * \param[in] s   : current state
     */
    bool doPackagingStep(CurrentState&,
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
    std::pair<bool,real>
    computePredictionOperator(BehaviourWorkSpace&,
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
    std::pair<bool,real>
    integrate(CurrentState&,
	      BehaviourWorkSpace&,
	      const real,
	      const StiffnessMatrixType) const override;
    //! destructor
    ~LogarithmicStrain1DBehaviourWrapper() override;
  private:
    //! wrapped behaviour
    std::shared_ptr<Behaviour> b;
  }; // end of struct LogarithmicStrain1DBehaviourWrapper
  
} // end of namespace mtest

#endif /* LIB_MTEST_LOGARITHMICSTRAIN1DBEHAVIOURWRAPPER_HXX */
