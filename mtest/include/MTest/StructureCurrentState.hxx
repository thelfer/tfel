/*!
 * \file   StructureCurrentState.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   28 oct. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MTEST_STRUCTURECURRENTSTATE_HXX_
#define _LIB_MTEST_STRUCTURECURRENTSTATE_HXX_

#include"TFEL/Math/vector.hxx"
#include"MTest/Config.hxx"
#include"MTest/Types.hxx"

namespace mtest{

  /*!
   * Data structure containing the state of a mechanical structure.
   */
  struct MTEST_VISIBILITY_EXPORT StructureCurrentState
  {
    //! default constructor
    StructureCurrentState();
    //! copy constructor
    StructureCurrentState(const StructureCurrentState&);
    StructureCurrentState(StructureCurrentState&&);
    //! destructor
    ~StructureCurrentState() noexcept;
    // vector of unknows at 
    // the beginning of the
    // previous time step.
    tfel::math::vector<real> u_1;
    // vector of unknows at the beginning of the
    // time step.
    tfel::math::vector<real> u0;
    // vector of unknows at the end of the
    // time step
    tfel::math::vector<real> u1;
    // vector of unknows at the preivous iteration (end of the
    // time step)
    tfel::math::vector<real> u10;
    // current period number
    unsigned int period;
    // previous time step
    real dt_1;
    // reference Temperature
    real Tref;
    //! current state of each integration points
    tfel::math::vector<CurrentState> istates;
  private:
    StructureCurrentState&
    operator=(const StructureCurrentState&) = delete;
    StructureCurrentState&
    operator=(StructureCurrentState&&) = delete;
  };

} // end of namespace mtest

#endif /* _LIB_MTEST_STRUCTURECURRENTSTATE_HXX_ */
