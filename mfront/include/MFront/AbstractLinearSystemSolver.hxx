/*!
 * \file   MFront/AbstractLinearSystemSolver.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   05/07/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABSTRACTLINEARSYSTEMSOLVER_HXX
#define LIB_MFRONT_ABSTRACTLINEARSYSTEMSOLVER_HXX

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
    //! a simple alias
    using Hypothesis = tfel::material::ModellingHypothesis::Hypothesis;
    //! \return the header to be included
    virtual std::vector<std::string> getSpecificHeaders() const = 0;
    //! \return the reserved names
    virtual std::vector<std::string> getReservedNames() const = 0;
    /*!
     * \brief write the solveLinearSystem method
     */

    /*!
     * \brief write the solver specific members
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    virtual void writeSpecificMembers(std::ostream&,
                                      const BehaviourDescription&,
                                      const Hypothesis) const = 0;
    //! \brief destructor
    virtual ~AbstractLinearSystemSolver();
  };  // end of struct AbstractLinearSystemSolver

} // end of namespace mfront

#endif /* LIB_MFRONT_ABSTRACTLINEARSYSTEMSOLVER_HXX */
