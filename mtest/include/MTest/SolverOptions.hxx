/*!
 * \file   SolverOptions.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04 nov. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MTEST_SOLVEROPTIONS_HXX
#define LIB_MTEST_SOLVEROPTIONS_HXX

#include <memory>

#include "MTest/Types.hxx"

namespace mtest {

  // forward declaration
  struct AccelerationAlgorithm;

  /*!
   * \brief possible type of stiffness matrix
   */
  enum struct StiffnessMatrixType {
    //! list of possible matrix type
    NOSTIFFNESS,
    ELASTIC,
    SECANTOPERATOR,
    TANGENTOPERATOR,
    CONSISTENTTANGENTOPERATOR,
    // This one is mostly for the umat behaviour
    ELASTICSTIFNESSFROMMATERIALPROPERTIES,
    UNSPECIFIEDSTIFFNESSMATRIXTYPE
  };

  /*!
   * \brief possible algorithms used for global convergence to
   * update the stiffness matrix
   */
  enum struct StiffnessUpdatingPolicy {
    /*!
     * the initial prediction matrix or the initial tangent coherent
     * matrix or the initial secant matrix (normally equal to the
     * elastic stiffness) is used for all iterations
     */
    CONSTANTSTIFFNESS,
    /*!
     * the prediction matrix or the first tangent coherent matrix or
     * the first secant matrix is used for all iterations and
     * updated and factorized at at periods
     */
    CONSTANTSTIFFNESSBYPERIOD,
    /*!
     * the tangent coherent matrix or the secant matrix is computed
     * and factorized at each iteration
     */
    UPDATEDSTIFFNESSMATRIX,
    //! default value
    UNSPECIFIEDSTIFFNESSUPDATINGPOLICY
  };  // end of enum struct StiffnessUpdatingPolicy
      /*!
       * \brief possible prediction policies
       */
  enum struct PredictionPolicy {
    NOPREDICTION,
    LINEARPREDICTION,
    ELASTICPREDICTION,
    ELASTICPREDICTIONFROMMATERIALPROPERTIES,
    SECANTOPERATORPREDICTION,
    TANGENTOPERATORPREDICTION,
    UNSPECIFIEDPREDICTIONPOLICY
  };  // end of enum struct PredictionPolicy

  /*!
   * work space used by the solver
   */
  struct SolverOptions {
    SolverOptions();
    SolverOptions(SolverOptions&&);
    SolverOptions(const SolverOptions&);
    SolverOptions& operator=(SolverOptions&&);
    SolverOptions& operator=(const SolverOptions&);
    ~SolverOptions();
    //! acceleration algorithm
    std::shared_ptr<AccelerationAlgorithm> aa;
    //! use castem acceleration
    bool useCastemAcceleration = false;
    //! stiffness updating policy
    StiffnessUpdatingPolicy ks =
        StiffnessUpdatingPolicy::UNSPECIFIEDSTIFFNESSUPDATINGPOLICY;
    //! type of stifness matrix to use for the resolution
    StiffnessMatrixType ktype =
        StiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE;
    //! use a prediction matrix before beginning the resolution
    PredictionPolicy ppolicy = PredictionPolicy::UNSPECIFIEDPREDICTIONPOLICY;
    //! order of magnitude of the precision expected for the unknows
    real eeps = -real(1);
    //! order of magnitude of the precision expected for the forces
    real seps = -real(1);
    //! maximal time step
    real maximal_time_step = -1;
    //! minimal time step
    real minimal_time_step = -1;
    /*!
     * \brief maximal time scaling factor allowed for increasing the
     * time step. If set, must be greater than 1
     */
    real maximal_time_step_scaling_factor = -1;
    /*!
     * \brief minimal time scaling factor allowed for decreasing the
     * time step. If set, must be lesser than 1
     */
    real minimal_time_step_scaling_factor = -1;
    //! maximum number of substeps allowed
    int mSubSteps = -1;
    //! maximum number of iterations per subtep
    int iterMax = -1;
    //! if true, the time step is adjusted using the values given by
    //! the mechanical behaviour
    bool dynamic_time_step_scaling = false;
  };  // end of struct SolverOptions

}  // namespace mtest

#endif /* LIB_MTEST_SOLVEROPTIONS_HXX */
