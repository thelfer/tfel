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
#include "MTest/Config.hxx"
#include "MTest/Types.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

namespace mtest {

  // forward declaration
  struct CurrentState;
  // forward declaration
  struct Behaviour;
  // forward declaration
  struct BehaviourWorkSpace;

  /*!
   * Data structure containing the state of a mechanical structure.
   */
  struct MTEST_VISIBILITY_EXPORT StructureCurrentState {
    //! a simple alias
    using Hypothesis = tfel::material::ModellingHypothesis::Hypothesis;
    /*!
     * default constructor
     * \param[in] p: behaviour
     * \param[in] h: modelling hypotesis
     */
    StructureCurrentState();
    //! move constructor
    StructureCurrentState(StructureCurrentState&&);
    //! copy constructor
    StructureCurrentState(const StructureCurrentState&);
    //! move assignment operator
    StructureCurrentState& operator=(StructureCurrentState&&);
    //! assignment operator
    StructureCurrentState& operator=(const StructureCurrentState&);
    /*!
     * \set the behaviour associated to the structure
     * \param[in] p : pointer to the behaviour
     */
    void setBehaviour(const std::shared_ptr<Behaviour>&);
    /*!
     * \set the behaviour associated to the structure
     * \param[in] mh : modelling hypothesis
     */
    void setModellingHypothesis(const Hypothesis);
    /*!
     * \return the behaviour workspace associated to the current thread.
     */
    BehaviourWorkSpace& getBehaviourWorkSpace() const;
    /*!
     * \return the behaviour associated to the structure
     */
    const Behaviour& getBehaviour() const;
    //! destructor
    ~StructureCurrentState();
    //! current state of each integration points
    tfel::math::vector<CurrentState> istates;

   private:
    //! behaviour
    std::shared_ptr<Behaviour> b;
    //! modelling hypothesis
    Hypothesis h = tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    //! behaviour workspaces
    mutable std::vector<std::shared_ptr<BehaviourWorkSpace>> bwks;
  };

  /*!
   * \brief update the state: values at the end of the time step
   * becomes the values at the beginning of the next time step
   * \param[out] s: state
   */
  MTEST_VISIBILITY_EXPORT void update(StructureCurrentState&);
  /*!
   * \brief revert the state: values at the end of the time step are
   * set equal to the values at the beginning of the next time step
   * \param[out] s: state
   */
  MTEST_VISIBILITY_EXPORT void revert(StructureCurrentState&);

}  // end of namespace mtest

#endif /* LIB_MTEST_STRUCTURECURRENTSTATE_HXX */
