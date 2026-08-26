/*!
 * \file   MFront/AbstractLinearSystemSolver.hxx
 * \brief
 * \author Thomas Helfer
 * \date 05/07/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABSTRACTLINEARSYSTEMSOLVER_HXX
#define LIB_MFRONT_ABSTRACTLINEARSYSTEMSOLVER_HXX

#include <set>
#include <iosfwd>
#include <optional>
#include <string_view>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  // forward declaration
  struct AbstractNonLinearSystemSolver;
  struct BehaviourDescription;

  /*!
   * \brief Base class for linear system solver used by implicit
   * domain specific languages.
   */
  struct MFRONT_VISIBILITY_EXPORT AbstractLinearSystemSolver {
    //
    struct MatrixDecompositionVariables {
      std::optional<std::string> returned_value;
      std::string matrix;
    };
    //! \brief structure containing results of the
    struct [[nodiscard]] MatrixDecompositionResult {
      struct VariableDescription {
        //
        std::string type;
        std::string name;
        auto operator<=>(const VariableDescription &) const = default;
      };
      //! \brief name of the decomposed matrix
      std::string matrix;
      //! \brief size (number of rows or columns) of the decomposed matrix
      std::string matrix_size;
      /*!
       * \brief list of variables that are generated during the decomposition
       * and that will be used later by the substitution operations
       */
      std::set<VariableDescription> variables;
    };
    //
    struct LinearSystemVariables {
      /*!
       * \brief name of the variable containing a boolean stating the success
       * or the failure of the resolution
       */
      std::optional<std::string> returned_value;
      //! \brief name of the matrix
      std::string matrix;
      //! \brief name of the variable holding the right hand side
      std::string rhs;
    };
    //
    struct LinearSystemSubstitutionVariables {
      /*!
       * \brief name of the variable containing a boolean stating the success
       * or the failure of the resolution
       */
      std::optional<std::string> returned_value;
      //! \brief name of the variable holding the right hand side
      std::string rhs;
      //! \brief number of columns of the rhs
      std::string rhs_column_size;
    };
    //! a simple alias
    using Hypothesis = tfel::material::ModellingHypothesis::Hypothesis;
    //! \return the header to be included
    virtual std::vector<std::string> getSpecificHeaders() const = 0;
    //! \return the reserved names
    virtual std::vector<std::string> getReservedNames() const = 0;
    /*!
     * \brief write the resolution of a linear system
     * \param[in] os: output stream
     * \param[in] solver: nonlinear solver
     * \param[in] h: modelling hypothesis
     * \param[in] s: description of the linear system
     */
    virtual void writeLinearSystemResolution(
        std::ostream &,
        const BehaviourDescription &,
        const AbstractNonLinearSystemSolver &,
        const Hypothesis,
        const LinearSystemVariables &) const = 0;
    /*!
     * \brief write the decomposition of a matrix prior to a resolution
     * of one or several linear systems (see write
     * `writeLinearSystemSubstitution`)
     *
     * \param[in] bd: behaviour description
     * \param[in] solver: nonlinear solver
     * \param[in] h: modelling hypothesis
     * \param[in] s: description of the linear system
     */
    virtual MatrixDecompositionResult getMatrixDecompositionResults(
        const BehaviourDescription &,
        const AbstractNonLinearSystemSolver &,
        const Hypothesis,
        const std::string &) const = 0;
    /*!
     * \brief write the decomposition of a matrix prior to a resolution
     * of one or several linear systems (see write
     * `writeLinearSystemSubstitution`)
     *
     * \param[in] os: output stream
     * \param[in] solver: nonlinear solver
     * \param[in] h: modelling hypothesis
     * \param[in] s: description of the linear system
     */
    virtual MatrixDecompositionResult writeMatrixDecomposition(
        std::ostream &,
        const BehaviourDescription &,
        const AbstractNonLinearSystemSolver &,
        const Hypothesis,
        const MatrixDecompositionVariables &) const = 0;
    /*!
     * \brief write the resolution of a linear system after the matrix
     * decomposition
     *
     * \param[in] os: output stream
     * \param[in] r: result of the matrix decomposition
     * \param[in] rhs: name of the right hand sie
     */
    virtual void writeLinearSystemSubstitution(
        std::ostream &,
        const MatrixDecompositionResult &,
        const LinearSystemSubstitutionVariables &) const = 0;
    /*!
     * \brief write the solver specific members
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    virtual void writeSpecificMembers(std::ostream &,
                                      const BehaviourDescription &,
                                      const Hypothesis) const = 0;
    //! \brief destructor
    virtual ~AbstractLinearSystemSolver();
  };  // end of struct AbstractLinearSystemSolver

}  // end of namespace mfront

#endif /* LIB_MFRONT_ABSTRACTLINEARSYSTEMSOLVER_HXX */
