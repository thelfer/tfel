/*!
 * \file   SolverOptions.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   04 nov. 2015
 */

#ifndef _LIB_MTEST_SOLVEROPTIONS_HXX_
#define _LIB_MTEST_SOLVEROPTIONS_HXX_

#include<memory>

#include"MTest/Types.hxx"

namespace mtest{

  // forward declaration
  struct AccelerationAlgorithm;

  /*!
   * \brief possible type of stiffness matrix
   */
  enum struct StiffnessMatrixType
  {
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
  enum struct StiffnessUpdatingPolicy{
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
  }; // end of enum struct StiffnessUpdatingPolicy
  /*!
   * \brief possible prediction policies
   */
  enum struct PredictionPolicy{
    NOPREDICTION,
    LINEARPREDICTION,
    ELASTICPREDICTION,
    ELASTICPREDICTIONFROMMATERIALPROPERTIES,
    SECANTOPERATORPREDICTION,
    TANGENTOPERATORPREDICTION,
    UNSPECIFIEDPREDICTIONPOLICY
  }; // end of enum struct PredictionPolicy
  
  /*!
   * work space used by the solver
   */
  struct SolverOptions
  {
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
    PredictionPolicy ppolicy =
      PredictionPolicy::UNSPECIFIEDPREDICTIONPOLICY;
    //! order of magnitude of the precision expected for the unknows
    real eeps = -real(1);
    //! order of magnitude of the precision expected for the forces
    real seps = -real(1);
    //! maximum number of substeps allowed
    int mSubSteps = -1;
    //! maximum number of iterations per subtep
    int iterMax = -1;
  }; // end of struct SolverOptions
  
} // end of namespace namespace mtest

#endif /* _LIB_MTEST_SOLVEROPTIONS_HXX_ */
