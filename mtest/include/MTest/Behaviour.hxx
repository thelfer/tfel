/*!
 * \file  mtest/include/MTest/Behaviour.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTBEHAVIOUR_H_
#define LIB_MTEST_MTESTBEHAVIOUR_H_

#include <vector>
#include <string>
#include <memory>

#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/matrix.hxx"
#include "TFEL/Math/tmatrix.hxx"

#include "TFEL/Material/OutOfBoundsPolicy.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Material/MechanicalBehaviour.hxx"

#include "MTest/Types.hxx"
#include "MTest/Config.hxx"
#include "MTest/SolverOptions.hxx"

namespace mtest {

  // forward declaration
  struct CurrentState;
  // forward declaration
  struct BehaviourWorkSpace;

  //! A simple wrapper around mechanical behaviours
  struct Behaviour : std::enable_shared_from_this<Behaviour> {
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
    virtual Hypothesis getHypothesis(void) const = 0;
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
    virtual BehaviourType getBehaviourType(void) const = 0;
    //! \return the type of the behaviour
    virtual Kinematic getBehaviourKinematic(void) const = 0;
    //! \return the size of a vector able to contain all the components of the
    //! driving variables
    virtual unsigned short getDrivingVariablesSize(void) const = 0;
    /*!
     * \param[out] v : initial values of the driving variables
     * \note : the vector shall have been correctly allocated
     */
    virtual void getDrivingVariablesDefaultInitialValues(
        tfel::math::vector<real>&) const = 0;
    /*!
     * \return the size of a vector able to contain all the components of the
     * thermodynamic forces
     */
    virtual unsigned short getThermodynamicForcesSize(void) const = 0;
    /*!
     * \return the components suffixes of a symmetric tensor
     */
    virtual std::vector<std::string> getStensorComponentsSuffixes(
        void) const = 0;
    /*!
     * \return the components suffixes of a vector
     */
    virtual std::vector<std::string> getVectorComponentsSuffixes(
        void) const = 0;
    /*!
     * \return the components suffixes of a tensor
     */
    virtual std::vector<std::string> getTensorComponentsSuffixes(
        void) const = 0;
    /*!
     * \return the components of the driving variables
     */
    virtual std::vector<std::string> getDrivingVariablesComponents(
        void) const = 0;
    /*!
     * \return the components of the thermodynamic forces
     */
    virtual std::vector<std::string> getThermodynamicForcesComponents(
        void) const = 0;
    /*!
     * \param[in] c : component
     */
    virtual unsigned short getDrivingVariableComponentPosition(
        const std::string&) const = 0;
    /*!
     * \param[in] c : component
     */
    virtual unsigned short getThermodynamicForceComponentPosition(
        const std::string&) const = 0;
    /*!
     * \return the type of the behaviour
     * 0 means that the behaviour is isotropic.
     * 1 means that the behaviour is orthotropic.
     */
    virtual unsigned short getSymmetryType(void) const = 0;
    /*!
     * \return the number of material properties
     */
    virtual std::vector<std::string> getMaterialPropertiesNames(void) const = 0;
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
    virtual std::vector<std::string> getInternalStateVariablesNames(
        void) const = 0;
    //! \return expand the names of internal variables
    virtual std::vector<std::string> expandInternalStateVariablesNames()
        const = 0;
    /*!
     * \return the size of the array of internal variables
     */
    virtual size_t getInternalStateVariablesSize(void) const = 0;
    /*!
     * \return the descriptions the internal variables
     */
    virtual std::vector<std::string> getInternalStateVariablesDescriptions(
        void) const = 0;
    /*!
     * \return the type of an internal variable
     */
    virtual unsigned short getInternalStateVariableType(
        const std::string&) const = 0;
    /*!
     * \return the position of an internal variable
     * \param[in] n : internal variable name
     */
    virtual unsigned short getInternalStateVariablePosition(
        const std::string&) const = 0;
    /*!
     * \return the number of external variables
     */
    virtual std::vector<std::string> getExternalStateVariablesNames(
        void) const = 0;
    /*!
     * \brief set the out of bounds policy
     * \param[in] p : policy selected
     */
    virtual void setOutOfBoundsPolicy(
        const tfel::material::OutOfBoundsPolicy) const = 0;
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
    virtual void allocate(BehaviourWorkSpace&) const = 0;
    /*!
     * \return the default type of stiffness matrix used by the behaviour
     */
    virtual StiffnessMatrixType getDefaultStiffnessMatrixType(void) const = 0;
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
    //! destructor
    virtual ~Behaviour();
  };  // end of struct Behaviour

  /*!
   * \return an object able to extract a specific value from the
   * current state
   * \param[in] b: behaviour
   * \param[in] n: variable name
   */
  MTEST_VISIBILITY_EXPORT std::function<real(const CurrentState&)>
  buildValueExtractor(const Behaviour&, const std::string&);

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTBEHAVIOUR_H_ */
