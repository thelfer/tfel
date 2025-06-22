/*!
 * \file  mtest/include/MTest/UmatBehaviourBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_MTESTUMATBEHAVIOURBASE_H_
#define LIB_MTEST_MTESTUMATBEHAVIOURBASE_H_

#include "TFEL/Math/matrix.hxx"
#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "TFEL/System/ExternalBehaviourDescription.hxx"
#include "MTest/Behaviour.hxx"

namespace mtest {

  /*!
   * a class gathering all the relevant information about an Umat
   * behaviour.
   */
  struct UmatBehaviourDescription : tfel::system::ExternalBehaviourData {
    /*!
     * \param[in] l: library name
     * \param[in] b: behaviour name
     * \param[in] h: modelling hypothesis
     */
    UmatBehaviourDescription(const std::string&,
                             const std::string&,
                             const std::string&);
    UmatBehaviourDescription();
    UmatBehaviourDescription(UmatBehaviourDescription&&);
    UmatBehaviourDescription(const UmatBehaviourDescription&);
    UmatBehaviourDescription& operator=(UmatBehaviourDescription&&);
    UmatBehaviourDescription& operator=(const UmatBehaviourDescription&);
    //! library
    std::string library;
    //! function
    std::string behaviour;
  };

  /*!
   * A class to handle mechanical beheaviours written using the umat
   * interface
   */
  struct UmatBehaviourBase : public Behaviour, public UmatBehaviourDescription {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    UmatBehaviourBase(const Hypothesis, const std::string&, const std::string&);
    /*!
     * \param[in] umb: behaviour description
     * \param[in] h:   modelling hypothesis
     */
    UmatBehaviourBase(const UmatBehaviourDescription&, const Hypothesis);
    //! \return the modelling hypothesis
    virtual Hypothesis getHypothesis(void) const override;
    //! \return the type of the behaviour
    virtual BehaviourType getBehaviourType(void) const override;
    //! \return the type of the behaviour
    virtual Kinematic getBehaviourKinematic(void) const override;
    /*!
     * \return the size of a vector able to contain all the components
     * of the driving variables
     */
    virtual unsigned short getDrivingVariablesSize(void) const override;
    /*!
     * \return the size of a vector able to contain all the components
     * of the thermodynamic forces
     */
    virtual unsigned short getThermodynamicForcesSize(void) const override;
    /*!
     * \return the components suffixes of a symmetric tensor
     */
    virtual std::vector<std::string> getStensorComponentsSuffixes(
        void) const override;
    /*!
     * \return the components suffixes of a vector
     */
    virtual std::vector<std::string> getVectorComponentsSuffixes(
        void) const override;
    /*!
     * \return the components suffixes of a tensor
     */
    virtual std::vector<std::string> getTensorComponentsSuffixes(
        void) const override;
    /*!
     * \return the components of the driving variables
     */
    virtual std::vector<std::string> getDrivingVariablesComponents(
        void) const override;
    /*!
     * \return the components of the thermodynamic forces
     */
    virtual std::vector<std::string> getThermodynamicForcesComponents(
        void) const override;
    /*!
     * \param[in] cname : component name
     */
    virtual unsigned short getDrivingVariableComponentPosition(
        const std::string&) const override;
    /*!
     * \param[in] cname : component name
     */
    virtual unsigned short getThermodynamicForceComponentPosition(
        const std::string&) const override;
    /*!
     * \return the type of the behaviour
     * 0 means that the behaviour is isotropic.
     * 1 means that the behaviour is orthotropic.
     */
    virtual unsigned short getSymmetryType(void) const override;
    /*!
     * \return the names of material properties
     */
    virtual std::vector<std::string> getMaterialPropertiesNames(
        void) const override;
    //! \return the names of internal variables
    virtual std::vector<std::string> getInternalStateVariablesNames(
        void) const override;
    //! \return expand the names of internal variables
    virtual std::vector<std::string> expandInternalStateVariablesNames()
        const override;
    //! \return the size of the array of internal variables
    virtual size_t getInternalStateVariablesSize(void) const override;
    //! \return the descriptions the internal variables
    virtual std::vector<std::string> getInternalStateVariablesDescriptions(
        void) const override;
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
    /*!
     * \return the names of external variables
     */
    virtual std::vector<std::string> getExternalStateVariablesNames(
        void) const override;
    /*!
     * \brief set the out of bounds policy
     * \param[in] p : policy selected
     */
    virtual void setOutOfBoundsPolicy(
        const tfel::material::OutOfBoundsPolicy) const override;
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
     * \brief execute the packaging step. This victious step is done
     * at the beginning of the computation.
     * \return a boolean
     * \param[out] wk : behaviour workspace
     * \param[in] s   : current state
     */
    virtual bool doPackagingStep(CurrentState&,
                                 BehaviourWorkSpace&) const override;
    //! destructor
    virtual ~UmatBehaviourBase();

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
    //! hypothesis
    const std::string hypothesis;
  };  // end of struct Behaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTUMATBEHAVIOURBASE_H_ */
