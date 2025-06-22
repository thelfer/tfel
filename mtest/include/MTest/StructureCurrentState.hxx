/*!
 * \file   StructureCurrentState.hxx
 * \brief
 * \author Thomas Helfer
 * \date   28 oct. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_STRUCTURECURRENTSTATE_HXX
#define LIB_MTEST_STRUCTURECURRENTSTATE_HXX

#include <memory>
#include <vector>
#include "TFEL/Math/vector.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MTest/Config.hxx"
#include "MTest/Types.hxx"

namespace mtest {

  // forward declaration
  struct CurrentState;
  // forward declaration
  struct Behaviour;
  // forward declaration
  struct BehaviourWorkSpace;

  /*!
   * \brief data structure containing the state of a mechanical structure.
   */
  struct MTEST_VISIBILITY_EXPORT StructureCurrentState {
    //! \brief a simple alias
    using Hypothesis = tfel::material::ModellingHypothesis::Hypothesis;
    //! \brief a simple alias
    using Model = Behaviour;
    /*!
     * \brief default constructor
     * \param[in] p: behaviour
     * \param[in] h: modelling hypotesis
     */
    StructureCurrentState();
    //! \brief move constructor
    StructureCurrentState(StructureCurrentState &&);
    //! \brief copy constructor
    StructureCurrentState(const StructureCurrentState &);
    //! \brief move assignment operator
    StructureCurrentState &operator=(StructureCurrentState &&);
    //! \brief assignment operator
    StructureCurrentState &operator=(const StructureCurrentState &);
    /*!
     * \set the behaviour associated to the structure
     * \param[in] p : pointer to the behaviour
     */
    void setBehaviour(const std::shared_ptr<Behaviour> &);
    /*!
     * \set the behaviour associated to the structure
     * \param[in] mh : modelling hypothesis
     */
    void setModellingHypothesis(const Hypothesis);
    //! \return the behaviour workspace associated to the current thread.
    BehaviourWorkSpace &getBehaviourWorkSpace() const;
    //! \return the behaviour associated to the structure
    const Behaviour &getBehaviour() const;
    /*!
     * \return the current state associated with an auxiliary model
     * \param[in] m: model
     */
    CurrentState &getModelCurrentState(const Model &);
    /*!
     * \return the current state associated with an auxiliary model
     * \param[in] m: model
     */
    const CurrentState &getModelCurrentState(const Model &) const;
    /*!
     * \return the workspace associated with the oxidation model
     * \param[in] m: model
     */
    BehaviourWorkSpace &getModelWorkSpace(const Model &);
    /*!
     * \brief update the state at each integration points and the state of
     * auxiliary models
     */
    void update();
    /*!
     * \brief rever the state at each integration points and the state of
     * auxiliary models.
     */
    void revert();
    //! \brief destructor
    ~StructureCurrentState();
    //! \brief current state of each integration points
    tfel::math::vector<CurrentState> istates;

   private:
    //! \brief behaviour
    std::shared_ptr<Behaviour> b;
    //! \brief modelling hypothesis
    Hypothesis h = tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    //! \brief behaviour workspaces
    mutable std::vector<std::shared_ptr<BehaviourWorkSpace>> bwks;
    //! \brief model states
    std::map<const Model *, std::shared_ptr<CurrentState>> model_states;
    //! \brief model workspace
    std::map<const Model *, std::shared_ptr<BehaviourWorkSpace>> model_wks;
  };

  /*!
   * \brief update the state: values at the end of the time step
   * becomes the values at the beginning of the next time step
   * \param[out] s: state
   */
  MTEST_VISIBILITY_EXPORT void update(StructureCurrentState &);
  /*!
   * \brief revert the state: values at the end of the time step are
   * set equal to the values at the beginning of the next time step
   * \param[out] s: state
   */
  MTEST_VISIBILITY_EXPORT void revert(StructureCurrentState &);

}  // end of namespace mtest

#endif /* LIB_MTEST_STRUCTURECURRENTSTATE_HXX */
