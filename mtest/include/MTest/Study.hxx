/*!
 * \file   Study.hxx
 * \brief
 * \author Thomas Helfer
 * \date   03 nov. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_STUDY_HXX
#define LIB_MTEST_STUDY_HXX

#include <utility>

#include "TFEL/Math/vector.hxx"
#include "TFEL/Math/matrix.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

#include "MTest/Config.hxx"
#include "MTest/Types.hxx"
#include "MTest/SolverOptions.hxx"

namespace mtest {

  // forward declaration
  struct StudyCurrentState;
  // forward declaration
  struct CurrentState;
  // forward declaration
  struct SolverWorkSpace;
  // forward declaration
  struct Structure;

  /*!
   * \brief abstract class describing a physical system
   */
  struct MTEST_VISIBILITY_EXPORT Study {
    //! a simple alias
    using size_type = tfel::math::vector<real>::size_type;
    /*!
     * \return the total number of unknowns (including the Lagrange
     * multipliers)
     */
    virtual size_type getNumberOfUnknowns() const = 0;
    /*!
     * \brief initialize the current state
     * \param[in] s : current state
     */
    virtual void initializeCurrentState(StudyCurrentState&) const = 0;
    /*!
     * \brief initialize the workspace
     * \param[in] wk : workspace
     */
    virtual void initializeWorkSpace(SolverWorkSpace&) const = 0;
    /*!
     * \brief update current state at the beginning of a new time step:
     * - update the material properties
     * - update the external state variables
     * - compute the thermal expansion if mandatory
     * \param[out] s: current structure state
     * \param[in]  t: current time
     * \param[in] dt: time increment
     */
    [[nodiscard]] virtual std::pair<bool, real> prepare(StudyCurrentState&,
                                                        const real,
                                                        const real) const = 0;
    /*!
     * \brief make a linear prediction of the unknows and state
     * \param[out] s: current structure state
     * \param[in] dt: time increment
     */
    virtual void makeLinearPrediction(StudyCurrentState&, const real) const = 0;
    /*!
     * \brief perform the computation of the packaging step
     */
    virtual bool doPackagingStep(StudyCurrentState&) const = 0;
    /*!
     * \brief compute the stiffness matrix and the residual
     * \return a pair containing:
     * - a boolean syaing if the behaviour integration shall be
     *   performed
     * - a scaling factor that can be used to:
     *     - increase the time step if the integration was successfull
     *     - decrease the time step if the integration failed or if the
     *       results were not reliable (time step too large).
     * \param[out] s: current structure state
     * \param[out] K:   tangent operator
     * \param[out] r:   residual
     * \param[in]  t:   current time
     * \param[in]  dt:  time increment
     * \param[in]  smt: type of tangent operator
     * \note the memory has already been allocated
     */
    [[nodiscard]] virtual std::pair<bool, real>
    computePredictionStiffnessAndResidual(StudyCurrentState&,
                                          tfel::math::matrix<real>&,
                                          tfel::math::vector<real>&,
                                          const real&,
                                          const real&,
                                          const StiffnessMatrixType) const = 0;
    /*!
     * \brief compute the stiffness matrix and the residual
     * \return a pair containing:
     * - a boolean syaing if the behaviour integration shall be
     *   performed
     * - a scaling factor that can be used to:
     *     - increase the time step if the integration was successfull
     *     - decrease the time step if the integration failed or if the
     *       results were not reliable (time step too large).
     * \param[out] s: current structure state
     * \param[out] K:   tangent operator
     * \param[out] r:   residual
     * \param[in]  t:   current time
     * \param[in]  dt:  time increment
     * \param[in]  smt: type of tangent operator
     * \note the memory has already been allocated
     */
    [[nodiscard]] virtual std::pair<bool, real>
    computeStiffnessMatrixAndResidual(StudyCurrentState&,
                                      tfel::math::matrix<real>&,
                                      tfel::math::vector<real>&,
                                      const real,
                                      const real,
                                      const StiffnessMatrixType) const = 0;
    /*!
     * \param[in] : du unknows increment difference between two iterations
     */
    virtual real getErrorNorm(const tfel::math::vector<real>&) const = 0;
    /*!
     * \param[in]  s: current structure state
     * \param[in] du: unknows increment estimation
     * \param[in] r:  residual
     * \param[in] o:  solver options
     * \param[in] i:  iteration number
     * \param[in] t:  current time
     * \param[in] dt: time increment
     * \return a boolean saying if all convergence criteria are met
     */
    virtual bool checkConvergence(StudyCurrentState&,
                                  const tfel::math::vector<real>&,
                                  const tfel::math::vector<real>&,
                                  const SolverOptions&,
                                  const unsigned int,
                                  const real,
                                  const real) const = 0;
    /*!
     * \param[in]  s: current structure state
     * \param[in] du: unknows increment estimation
     * \param[in] r:  residual
     * \param[in] o:  solver options
     * \param[in] t:  current time
     * \param[in] dt: time increment
     * \return a description of all the criteria that were not met.
     */
    virtual std::vector<std::string> getFailedCriteriaDiagnostic(
        const StudyCurrentState&,
        const tfel::math::vector<real>&,
        const tfel::math::vector<real>&,
        const SolverOptions&,
        const real,
        const real) const = 0;
    /*!
     * \param[out] s: current structure state
     * \param[in] wk: solver workspace
     * \param[in]  o:  solver options
     * \param[in]  t:  current time
     * \param[in]  dt: time increment
     */
    virtual void computeLoadingCorrection(StudyCurrentState&,
                                          SolverWorkSpace&,
                                          const SolverOptions&,
                                          const real,
                                          const real) const = 0;
    /*!
     * \param[out] s: current structure state
     * \param[in]  t:  current time
     * \param[in]  dt: time increment
     * \param[in]  p:  period
     */
    [[nodiscard]] virtual bool postConvergence(StudyCurrentState&,
                                               const real,
                                               const real,
                                               const unsigned int) const = 0;
    /*!
     * \param[in] h : modelling hypothesis
     */
    virtual void setModellingHypothesis(const std::string&) = 0;
    /*!
     * \brief print usefull information in the output file
     * \param[in] t: time
     * \param[in] s: current state
     * \param[in] o: if true, this time has been specified by the
     * user. Otherwise, it has been reached due to sub-stepping.
     */
    virtual void printOutput(const real,
                             const StudyCurrentState&,
                             const bool) const = 0;
    //! \brief set the modelling hypothesis to the default one
    virtual void setDefaultModellingHypothesis() = 0;
    //! \brief destructor
    virtual ~Study();

   protected:
    /*!
     * \brief set the position of the Gauss point in the evolution
     * manager
     * \param[in] s: current state
     */
    virtual void setGaussPointPositionForEvolutionsEvaluation(
        const CurrentState&) const = 0;
  };  // end of struct Study

}  // end of namespace mtest

#endif /* LIB_MTEST_STUDY_HXX */
