/*!
 * \file   StudyCurrentState.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   11 nov. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MTEST_STUDYCURRENTSTATE_HXX_
#define _LIB_MTEST_STUDYCURRENTSTATE_HXX_

#include<map>
#include<string>
#include<memory>

#include"TFEL/Math/vector.hxx"
#include"MTest/Config.hxx"
#include"MTest/Types.hxx"

namespace mtest{

  // forward declaration
  struct StructureCurrentState;
  
  /*!
   * structure describing the current state of a study
   */
  struct MTEST_VISIBILITY_EXPORT StudyCurrentState
  {
    //! a simple alias
    using size_type = tfel::math::vector<real>::size_type;
    //! default constructor
    StudyCurrentState();
    StudyCurrentState(const StudyCurrentState&);
    StudyCurrentState(StudyCurrentState&&);
    StudyCurrentState& operator=(const StudyCurrentState&);
    StudyCurrentState& operator=(StudyCurrentState&&);
    ~StudyCurrentState();
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
    // vector of unknows at the previous iteration (end of the
    // time step)
    tfel::math::vector<real> u10;
    // current period number
    unsigned int period = 1u;
    // previous time step
    real dt_1 = real{};    
    /*!
     * \brief allocate memory
     * \param[out] psz: problem size
     */
    void initialize(const StudyCurrentState::size_type);
    /*!
     * \brief update the state: values at the end of the time step
     * becomes the values at the beginning of the next time step
     * \param[in] dt: time increment
     */
    void update(const real);
    /*!
     * \brief revert the state: values at the end of the time step are
     * set equal to the values at the beginning of the next time step
     */
    void revert(void);
    /*!
     * \return the state associated with a structure
     * \param[in] n: structure name
     */ 
    StructureCurrentState&
    getStructureCurrentState(const std::string&);
    /*!
     * \return the state associated with a structure
     * \param[in] n: structure name
     */ 
    const StructureCurrentState&
    getStructureCurrentState(const std::string&) const;
  protected:
    /*!
     * current state of the structures indexed by the structure's
     * names
     */
    std::map<std::string,
	     std::shared_ptr<StructureCurrentState>> s;
  }; // end of struct StudyCurrentState

    
} // end of namespace mtest

#endif /* _LIB_MTEST_STUDYCURRENTSTATE_HXX_ */


