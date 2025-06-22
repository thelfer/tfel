/*!
 * \file   SingleStructureScheme.hxx
 * \brief
 * \author Thomas Helfer
 * \date   09 déc. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_SINGLESTRUCTURESCHEME_HXX
#define LIB_MTEST_SINGLESTRUCTURESCHEME_HXX

#include "TFEL/Utilities/Data.hxx"
#include "TFEL/Material/OutOfBoundsPolicy.hxx"
#include "MTest/Config.hxx"
#include "MTest/Types.hxx"
#include "MTest/SchemeBase.hxx"

namespace mtest {

  // forward declaration
  struct Behaviour;

  /*!
   * Base class for schemes describing a single structure
   */
  struct MTEST_VISIBILITY_EXPORT SingleStructureScheme : public SchemeBase {
    using Parameters = tfel::utilities::Data;
    //! default constructor
    SingleStructureScheme();
    /*!
     * \brief set the behaviour
     * \param[in] i: interface
     * \param[in] l: library name
     * \param[in] f: function
     * \param[in] d: parameters
     */
    virtual void setBehaviour(const std::string&,
                              const std::string&,
                              const std::string&,
                              const Parameters& = Parameters());
    /*!
     * \brief set the behaviour
     * \param[in] w: behaviour wrapper
     * \param[in] i: interface
     * \param[in] l: library name
     * \param[in] f: function
     * \param[in] d: parameters
     */
    virtual void setBehaviour(const std::string&,
                              const std::string&,
                              const std::string&,
                              const std::string&,
                              const Parameters& = Parameters());
    /*!
     * \brief define a material property
     * \param[in] n     : evolution name
     * \param[in] p     : evolution pointer
     * \param[in] check : check if the evolution already exists.
     * \note if check is true and the evolution already exists, an
     * exeception is thrown.
     */
    virtual void setMaterialProperty(const std::string&,
                                     const EvolutionPtr,
                                     const bool);
    /*!
     * \brief define an external state variable
     * \param[in] n     : evolution name
     * \param[in] p     : evolution pointer
     * \param[in] check : check if the evolution already exists.
     * \note if check is true and the evolution already exists, an
     * exeception is thrown.
     */
    virtual void setExternalStateVariable(const std::string&,
                                          const EvolutionPtr,
                                          const bool);
    //! \return the behaviour type
    virtual BehaviourType getBehaviourType() const;
    //! \return the behaviour kinematic
    virtual Kinematic getBehaviourKinematic() const;
    //! \return the behaviour
    virtual std::shared_ptr<Behaviour> getBehaviour();
    /*!
     * \brief set if mtest shall handle thermal expansion coefficient
     * If true, the thermal expansion will be handled if the thermal
     * expansion coefficients are defined.
     * \param[in] b : boolean
     */
    virtual void setHandleThermalExpansion(const bool);
    /*!
     * \brief perform the computation of the packaging step
     */
    bool doPackagingStep(StudyCurrentState&) const override;
    /*!
     * \brief update current state at the beginning of a new time step:
     * - update the material properties
     * - update the external state variables
     * - compute the thermal expansion if mandatory
     * \param[out] state: current structure state
     * \param[in]  t: current time
     * \param[in] dt: time increment
     */
    void prepare(StudyCurrentState&, const real, const real) const override;
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter value
     */
    virtual void setParameter(const std::string&, const double);
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter value
     */
    virtual void setIntegerParameter(const std::string&, const int);
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter value
     */
    virtual void setUnsignedIntegerParameter(const std::string&,
                                             const unsigned int);
    /*!
     * \brief set the inital value of a scalar variable
     * \param[in] v : value
     */
    virtual void setScalarInternalStateVariableInitialValue(const std::string&,
                                                            const real);
    /*!
     * \brief set the inital values of a symetric tensor variable
     * \param[in] v : values
     */
    virtual void setStensorInternalStateVariableInitialValues(
        const std::string&, const std::vector<real>&);
    /*!
     * \brief set the inital values of a tensor variable
     * \param[in] v : values
     */
    virtual void setTensorInternalStateVariableInitialValues(
        const std::string&, const std::vector<real>&);
    /*!
     * \brief set the out of bounds policy
     * \param[in] a : out of bounds policy
     */
    virtual void setOutOfBoundsPolicy(const tfel::material::OutOfBoundsPolicy);
    void completeInitialisation() override;
    //! destructor
    ~SingleStructureScheme() override;

   protected:
    /*!
     * \brief check that the behaviour is consistent with the
     * modelling hypothesis
     * \param[in] bp : pointer to the behaviour
     */
    virtual void checkBehaviourConsistency(const std::shared_ptr<Behaviour>&);
    /*!
     * \brief set the behaviour
     * \param[in] bp : pointer to the behaviour
     */
    virtual void setBehaviour(const std::shared_ptr<Behaviour>&);
    //! \return the default stiffness matrix type
    StiffnessMatrixType getDefaultStiffnessMatrixType() const override;
    //! list of internal variable names, including their suffixes
    std::vector<std::string> ivfullnames;
    //! the mechanical behaviour
    std::shared_ptr<Behaviour> b;
    //! default values for material properties as given by the behaviour
    std::shared_ptr<EvolutionManager> dmpv;
    // inital values of the internal state variables
    std::vector<real> iv_t0;
    //! handle the computation of thermal expansion
    bool handleThermalExpansion = true;
  };  // end of SingleStructureScheme

}  // end of namespace mtest

#endif /* LIB_MTEST_SINGLESTRUCTURESCHEME_HXX */
