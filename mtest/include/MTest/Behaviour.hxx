/*!
 * \file  mtest/include/MTest/Behaviour.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTBEHAVIOUR_HXX
#define LIB_MTEST_MTESTBEHAVIOUR_HXX

#include <vector>
#include <string>
#include <memory>

#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/matrix.hxx"
#include "TFEL/Math/tmatrix.hxx"

#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Material/OutOfBoundsPolicy.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Material/MechanicalBehaviour.hxx"

#include "MTest/Types.hxx"
#include "MTest/Config.hxx"
#include "MTest/SolverOptions.hxx"

namespace mtest {

  // forward declarations
  struct CurrentState;
  struct BehaviourWorkSpace;

  //! \brief a simple wrapper around mechanical behaviours
  struct MTEST_VISIBILITY_EXPORT Behaviour
      : std::enable_shared_from_this<Behaviour> {
    //! a simple alias
    using Parameters = tfel::utilities::Data;
    //! a simple alias
    using ParametersMap = tfel::utilities::DataMap;
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;
    //! a simple alias
    using BehaviourType =
        tfel::material::MechanicalBehaviourBase::BehaviourType;
    //! a simple alias
    using Kinematic = tfel::material::MechanicalBehaviourBase::Kinematic;
    //! \return the modelling hypothesis
    virtual Hypothesis getHypothesis() const = 0;
    /*!
     * \brief This function set a material property to its default value if it
     * not already declared
     * \param[out] mp  : evolution manager where
     * \param[in]  evm : evolution manager
     * \param[in]  n   : material property name
     * \param[in]  v   : default value
     */
    static void setOptionalMaterialPropertyDefaultValue(EvolutionManager&,
                                                        const EvolutionManager&,
                                                        const std::string&,
                                                        const real);
    /*!
     * \brief build a behaviour
     * \param[in] i: interface
     * \param[in] l: library
     * \param[in] f: function
     * \param[in] d: parameter
     * \param[in] h: modelling hypothesis
     */
    static std::shared_ptr<Behaviour> getBehaviour(const std::string&,
                                                   const std::string&,
                                                   const std::string&,
                                                   const Parameters&,
                                                   const Hypothesis);
    //! \return the name of the behaviour
    virtual std::string getBehaviourName() const = 0;
    //! \return the type of the behaviour
    virtual BehaviourType getBehaviourType() const = 0;
    //! \return the type of the behaviour
    virtual Kinematic getBehaviourKinematic() const = 0;
    //! \return the size of a vector able to contain all the components of the
    //! driving variables
    virtual unsigned short getGradientsSize() const = 0;
    /*!
     * \param[out] v : initial values of the driving variables
     * \note : the vector shall have been correctly allocated
     */
    virtual void getGradientsDefaultInitialValues(
        tfel::math::vector<real>&) const = 0;
    /*!
     * \return the size of a vector able to contain all the components of the
     * thermodynamic forces
     */
    virtual unsigned short getThermodynamicForcesSize() const = 0;
    //! \return the components suffixes of a symmetric tensor
    virtual std::vector<std::string> getStensorComponentsSuffixes() const = 0;
    //! \return the components suffixes of a vector
    virtual std::vector<std::string> getVectorComponentsSuffixes() const = 0;
    //! \return the components suffixes of a tensor
    virtual std::vector<std::string> getTensorComponentsSuffixes() const = 0;
    //! \return the components of the driving variables
    virtual std::vector<std::string> getGradientsComponents() const = 0;
    //! \return the components of the thermodynamic forces
    virtual std::vector<std::string> getThermodynamicForcesComponents()
        const = 0;
    /*!
     * \param[in] c : component
     */
    virtual unsigned short getGradientComponentPosition(
        const std::string&) const = 0;
    /*!
     * \param[in] c : component
     */
    virtual unsigned short getThermodynamicForceComponentPosition(
        const std::string&) const = 0;
    //! \return the size of the tangent operator used by `MTest`
    virtual size_t getTangentOperatorArraySize() const = 0;
    /*!
     * \return the tangent operator blocks defined by the behaviour. Those
     * blocks can be unused by `MTest`.
     */
    virtual std::vector<std::pair<std::string, std::string>>
    getTangentOperatorBlocks() const = 0;
    /*!
     * \return the type of the behaviour
     * 0 means that the behaviour is isotropic.
     * 1 means that the behaviour is orthotropic.
     */
    virtual unsigned short getSymmetryType() const = 0;
    //! \return the names of the material properties
    virtual std::vector<std::string> getMaterialPropertiesNames() const = 0;
    //! \return the number of the material properties
    virtual size_t getMaterialPropertiesSize() const = 0;
    /*!
     * \return the list of optional material properties
     */
    virtual std::vector<std::string> getOptionalMaterialProperties() const = 0;
    /*!
     * \brief This method solves two issues:
     *
     * - Some interface requires dummy material properties to be
     *   declared. For example, the Cast3M finite element solver
     *   requires the mass density and some extra material properties
     *   describing orthotropic axes to be declared.  This method is
     *   meant to automatically declare those if they are not defined
     *   by the user.
     * - Some interface (mistral) uses an external files which gives
     *   the values of some material properties. This method is used
     *   to pass thoses values to MTest.
     *
     * \param[out] mp  : evolution manager where
     * \param[in]  evm : evolution manager
     */
    virtual void setOptionalMaterialPropertiesDefaultValues(
        EvolutionManager&, const EvolutionManager&) const = 0;
    //! \return the names of internal variables
    virtual std::vector<std::string> getInternalStateVariablesNames() const = 0;
    //! \return expand the names of internal variables
    virtual std::vector<std::string> expandInternalStateVariablesNames()
        const = 0;
    //! \return the size of the array of internal variables
    virtual size_t getInternalStateVariablesSize() const = 0;
    //! \return the descriptions the internal variables
    virtual std::vector<std::string> getInternalStateVariablesDescriptions()
        const = 0;
    //! \return the type of an internal variable
    virtual unsigned short getInternalStateVariableType(
        const std::string&) const = 0;
    /*!
     * \return the position of an internal variable
     * \param[in] n : internal variable name
     */
    virtual unsigned short getInternalStateVariablePosition(
        const std::string&) const = 0;
    //! \return the names of external variables
    virtual std::vector<std::string> getExternalStateVariablesNames() const = 0;
    //! \return the size of the array of exernal variables
    virtual size_t getExternalStateVariablesSize() const = 0;
    //! \return expand the names of external variables
    virtual std::vector<std::string> expandExternalStateVariablesNames()
        const = 0;
    //! \return the type of an external variable
    virtual unsigned short getExternalStateVariableType(
        const std::string&) const = 0;
    /*!
     * \return the position of an external variable
     * \param[in] n : external variable name
     */
    virtual unsigned short getExternalStateVariablePosition(
        const std::string&) const = 0;
    //! \return the names of floating point parameters
    virtual std::vector<std::string> getParametersNames() const = 0;
    //! \return the names of integer parameters
    virtual std::vector<std::string> getIntegerParametersNames() const = 0;
    //! \return the names of unsigned short parameters
    virtual std::vector<std::string> getUnsignedShortParametersNames()
        const = 0;
    /*!
     * \return the default value of a parameter
     * \param[in] p: parameter name
     */
    virtual double getRealParameterDefaultValue(const std::string&) const = 0;
    /*!
     * \return the default value of an integer parameter
     * \param[in] p: parameter name
     */
    virtual int getIntegerParameterDefaultValue(const std::string&) const = 0;
    /*!
     * \return the default value of an integer parameter
     * \param[in] p: parameter name
     */
    virtual unsigned short getUnsignedShortParameterDefaultValue(
        const std::string&) const = 0;
    /*!
     * \brief set the out of bounds policy
     * \param[in] p : policy selected
     */
    virtual void setOutOfBoundsPolicy(
        const tfel::material::OutOfBoundsPolicy) const = 0;
    /*!
     * \return true if the given variable has bounds
     * \param[in] v: variable name
     */
    virtual bool hasBounds(const std::string&) const = 0;
    /*!
     * \return true if the given variable has a lower bound
     * \param[in] v: variable name
     */
    virtual bool hasLowerBound(const std::string&) const = 0;
    /*!
     * \return true if the given variable has an upper bound
     * \param[in] v: variable name
     */
    virtual bool hasUpperBound(const std::string&) const = 0;
    /*!
     * \return the lower bound of the given variable
     * \param[in] v: variable name
     */
    virtual long double getLowerBound(const std::string&) const = 0;
    /*!
     * \return the upper bound of the given variable
     * \param[in] v: variable name
     */
    virtual long double getUpperBound(const std::string&) const = 0;
    /*!
     * \return true if the given variable has physical bounds
     * \param[in] v: variable name
     */
    virtual bool hasPhysicalBounds(const std::string&) const = 0;
    /*!
     * \return true if the given variable has a lower physical bound
     * \param[in] v: variable name
     */
    virtual bool hasLowerPhysicalBound(const std::string&) const = 0;
    /*!
     * \return true if the given variable has a upper physical bound
     * \param[in] v: variable name
     */
    virtual bool hasUpperPhysicalBound(const std::string&) const = 0;
    /*!
     * \return the lower bound of the given variable
     * \param[in] v: variable name
     */
    virtual long double getLowerPhysicalBound(const std::string&) const = 0;
    /*!
     * \return the upper bound of the given variable
     * \param[in] v: variable name
     */
    virtual long double getUpperPhysicalBound(const std::string&) const = 0;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    virtual void setParameter(const std::string&, const real) const = 0;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    virtual void setIntegerParameter(const std::string&, const int) const = 0;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    virtual void setUnsignedIntegerParameter(const std::string&,
                                             const unsigned short) const = 0;
    /*!
     * \brief allocate workspace
     * \param[out] wk : behaviour workspace
     */
    virtual void allocateWorkSpace(BehaviourWorkSpace&) const = 0;
    /*!
     * \brief allocate current state
     * \param[out] cs : current state
     */
    virtual void allocateCurrentState(CurrentState&) const = 0;
    /*!
     * \return the default type of stiffness matrix used by the behaviour
     */
    virtual StiffnessMatrixType getDefaultStiffnessMatrixType() const = 0;
    /*!
     * \brief compute the *real* rotation matrix
     * \param[in] mp : material properties
     * \param[in] r  : rotation matrix defined by the user
     * \note this method is only meaningfull for the umat (Cast3M)
     * interface
     */
    virtual tfel::math::tmatrix<3u, 3u, real> getRotationMatrix(
        const tfel::math::vector<real>&,
        const tfel::math::tmatrix<3u, 3u, real>&) const = 0;
    /*!
     * \brief execute the packaging step. This victious step is done
     * at the beginning of the computation.
     * \return a boolean
     * \param[out] wk : behaviour workspace
     * \param[in] s   : current state
     */
    virtual bool doPackagingStep(CurrentState&, BehaviourWorkSpace&) const = 0;
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
        const StiffnessMatrixType) const = 0;
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
        const StiffnessMatrixType) const = 0;
    //! \brief destructor
    virtual ~Behaviour();
  };  // end of struct Behaviour

  /*!
   * \return true if the given variable is either:
   * - a driving variable of the behaviour
   * - a thermodynamic force the behaviour
   * - an internal state variable
   * - an external state variable
   * \param[in] b: behaviour
   * \param[in] n: variable name
   */
  MTEST_VISIBILITY_EXPORT bool isBehaviourVariable(const Behaviour&,
                                                   const std::string&);
  /*!
   * \return an object able to extract a specific value from the
   * current state
   * \param[in] b: behaviour
   * \param[in] n: variable name
   */
  MTEST_VISIBILITY_EXPORT std::function<real(const CurrentState&)>
  buildValueExtractor(const Behaviour&, const std::string&);
  /*!
   * \return if the kind type corresponds to a scalar
   * \param[in] t: variable type
   */
  MTEST_VISIBILITY_EXPORT bool isScalarVariableType(const int);

  /*!
   * \return the size of a variable
   * \param[in] t: variable type
   * \param[in] h: hypothesis
   */
  MTEST_VISIBILITY_EXPORT size_t getVariableSize(const int,
                                                 const Behaviour::Hypothesis);

  /*!
   * \return the size of an array of variables
   * \param[in] types: variable types
   * \param[in] h: hypothesis
   */
  MTEST_VISIBILITY_EXPORT size_t getVariablesSize(const std::vector<int>&,
                                                  const Behaviour::Hypothesis);
  /*!
   * \brief return the components of a variable
   * \param[in] b: behaviour
   * \param[in] n: name of the variable
   * \param[in] t: type of the variable
   */
  MTEST_VISIBILITY_EXPORT std::vector<std::string> getVariableComponents(
      const Behaviour&, const std::string&, const int);

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTBEHAVIOUR_HXX */
