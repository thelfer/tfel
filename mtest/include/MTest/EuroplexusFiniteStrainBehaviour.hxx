/*!
 * \file  mtest/include/MTest/EuroplexusFiniteStrainBehaviour.hxx
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

#ifndef LIB_MTEST_MTESTEUROPLEXUSFINITESTRAINBEHAVIOUR_H_
#define LIB_MTEST_MTESTEUROPLEXUSFINITESTRAINBEHAVIOUR_H_

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/System/ExternalFunctionsPrototypes.hxx"
#include "MTest/EuroplexusStandardBehaviour.hxx"

namespace mtest {

  /*!
   * A class to handle mechanical beheaviours written using the Europlexus
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL EuroplexusFiniteStrainBehaviour
      : public EuroplexusStandardBehaviour {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    EuroplexusFiniteStrainBehaviour(const Hypothesis,
                                    const std::string&,
                                    const std::string&);
    /*!
     * \param[out] v : initial values of the driving variables
     */
    virtual void getDrivingVariablesDefaultInitialValues(
        tfel::math::vector<real>&) const override;
    /*!
     * \brief allocate internal workspace
     * \param[out] wk : work space
     */
    virtual void allocate(BehaviourWorkSpace&) const override;
    //! destructor
    virtual ~EuroplexusFiniteStrainBehaviour();

   protected:
    /*!
     * \brief call the mechanical behaviour
     * \return a pair. The first member is true if the integration was
     * successfull, false otherwise. The second member contains a time
     * step scaling factor.
     * \param[out]    Kt    : tangent operator
     * \param[in,out] s     : current state
     * \param[out]    wk    : workspace
     * \param[in]     dt    : time increment
     * \param[in]     ktype : type of the stiffness matrix
     * \param[in]     b     : if true, integrate the behaviour over the time
     * step, if false compute a prediction of the stiffness matrix
     */
    virtual std::pair<bool, real> call_behaviour(tfel::math::matrix<real>&,
                                                 CurrentState&,
                                                 BehaviourWorkSpace&,
                                                 const real,
                                                 const StiffnessMatrixType,
                                                 const bool) const override;
  };  // end of struct Behaviour

}  // end of namespace mtest

#endif /* LIB_MTEST_MTESTEUROPLEXUSFINITESTRAINBEHAVIOUR_H_ */
