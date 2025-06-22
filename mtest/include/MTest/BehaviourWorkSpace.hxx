/*!
 * \file   BehaviourWorkSpace.hxx
 * \brief
 * \author THOMAS HELFER
 * \date   17 nov. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_BEHAVIOURWORKSPACE_HXX_
#define LIB_MTEST_BEHAVIOURWORKSPACE_HXX_

#include "TFEL/Math/matrix.hxx"
#include "TFEL/Math/vector.hxx"

#include "MTest/Config.hxx"
#include "MTest/Types.hxx"
#include "MTest/CurrentState.hxx"

namespace mtest {

  /*!
   * structure in which local variables used by a behaviour to perform
   * its integration is stored.
   */
  struct MTEST_VISIBILITY_EXPORT BehaviourWorkSpace {
    //! default constructor
    BehaviourWorkSpace();
    // deleted members
    BehaviourWorkSpace(BehaviourWorkSpace&&) = delete;
    BehaviourWorkSpace(const BehaviourWorkSpace&) = delete;
    BehaviourWorkSpace& operator=(BehaviourWorkSpace&&) = delete;
    BehaviourWorkSpace& operator=(const BehaviourWorkSpace&) = delete;
    //! destructor
    ~BehaviourWorkSpace();
    //! current state
    CurrentState cs;
    //! prediction operator (used by mtest to build the global
    //! stiffness matrix)
    tfel::math::matrix<real> kt;
    //! stiffness operator (used by mtest to build the global
    //! stiffness matrix)
    tfel::math::matrix<real> k;
    //! local stifness matrix (result of the behaviour integration)
    tfel::math::matrix<real> D;
    //! temporary vector for material properties
    tfel::math::vector<real> mps;
    //! temporary vector for internal variables
    tfel::math::vector<real> ivs;
    //! temporary vector for external state variables
    tfel::math::vector<real> evs;
    //! numerical stiffness operator
    tfel::math::matrix<real> nk;
    //! temporary vector for internal state variables
    tfel::math::vector<real> nivs;
    //! temporary vector for driving variables
    tfel::math::vector<real> ne;
    //! temporary vector for stresses
    tfel::math::vector<real> ns;
  };  // end of struct BehaviourWorkSpace

}  // end of namespace mtest

#endif /* LIB_MTEST_BEHAVIOURWORKSPACE_HXX_ */
