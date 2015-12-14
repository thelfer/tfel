/*!
 * \file   CurrentState.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   23 oct. 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MTEST_CURRENTSTATE_HXX_
#define _LIB_MTEST_CURRENTSTATE_HXX_

#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MTest/Config.hxx"
#include"MTest/Types.hxx"

namespace mtest
{

  // forward declaration
  struct Behaviour;
  
  /*!
   * structure containing the state of the material.
   * This structure is used internally and is declared public only
   * for the python bindings. In particular, the description of the
   * variables given here is only valid during the computations.
   */
  struct MTEST_VISIBILITY_EXPORT  CurrentState
  {
    //! default constructor
    CurrentState();
    //! copy constructor
    CurrentState(const CurrentState&);
    //! move constructor
    CurrentState(CurrentState&&);
    //! assignement
    CurrentState& operator=(const CurrentState&);
    //! move assignement
    CurrentState& operator=(CurrentState&&);
    //! destructor
    ~CurrentState() noexcept;
    //! thermodynamic forces at the beginning of the previous of the time step
    tfel::math::vector<real> s_1;
    //! thermodynamic forces at the beginning of the time step
    tfel::math::vector<real> s0;
    //! thermodynamic forces at the end of the time step
    tfel::math::vector<real> s1;
    /*!
     * driving variables at the beginning of the time step
     * \note for small strain behaviour, thermal strain are already substracted.
     */
    tfel::math::vector<real> e0;
    /*!
     * driving variables at the end of the time step
     * \note for small strain behaviour, thermal strain are already substracted.
     */
    tfel::math::vector<real> e1;
    /*!
     * thermal strain at the beginning of the time step
     * (small strain behaviours)
     */
    tfel::math::vector<real> e_th0;
    /*!
     * thermal strain at the end of the time step
     * (small strain behaviours)
     */
    tfel::math::vector<real> e_th1;
    //! material properties at the end of the time step
    tfel::math::vector<real> mprops1;
    //! internal variables at the beginning of the previous time step
    tfel::math::vector<real> iv_1;
    //! internal variables at the beginning of the time step
    tfel::math::vector<real> iv0;
    //! internal variables at the end of the time step
    tfel::math::vector<real> iv1;
    //! external variables at the beginning of the time step
    tfel::math::vector<real> esv0;
    //! external variables increments
    tfel::math::vector<real> desv;
    //! rotation matrix
    tfel::math::tmatrix<3u,3u> r;
    // position of the gauss point in the initial configuration
    real position;
    // reference Temperature
    real Tref  = 293.15;
  }; // end of struct CurrentState

  /*!
   * \brief allocate memory
   * \param[out] s: state
   * \param[in]  b: behaviour
   * \param[in]  h: modelling hypothesis
   */
  MTEST_VISIBILITY_EXPORT void
  allocate(CurrentState&,
	   const Behaviour&,
	   const tfel::material::ModellingHypothesis::Hypothesis);
  
  /*!
   * \brief compute the external state variables for the current time
   * step
   * \param[out] s       : state
   * \param[in]  evm     : evolution manager
   * \param[in]  dvm     : default material properties value
   * \param[in]  mpnames : material properties names
   * \param[in]  t       : time at the beginning of the time step
   * \param[in]  dt      : time increment
   */
  MTEST_VISIBILITY_EXPORT void
  computeMaterialProperties(CurrentState&,
			    const EvolutionManager&,
			    const EvolutionManager&,
			    const std::vector<std::string>&,
			    const real,
			    const real);
  
  /*!
   * \brief compute the external state variables for the current time
   * step
   * \param[out] s        : state
   * \param[in]  evm      : evolution manager
   * \param[in]  esvnames : external state variables names
   * \param[in]  t        : time at the beginning of the time step
   * \param[in]  dt       : time increment
   */
  MTEST_VISIBILITY_EXPORT void
  computeExternalStateVariables(CurrentState&,
				const EvolutionManager&,
				const std::vector<std::string>&,
				const real,
				const real);

  /*!
   * \brief update the state for the next time step
   * \param[out] s: state
   */
  MTEST_VISIBILITY_EXPORT void
  update(CurrentState&);

  /*!
   * \brief revert the state to the beginning of the time step
   * \param[out] s: state
   */
  MTEST_VISIBILITY_EXPORT void
  revert(CurrentState&);
  
} // end of namespace mtest

#endif /* _LIB_MTEST_CURRENTSTATE_HXX_ */
