/*!
 * \file  mfront/include/MFront/NonLinearSystemSolverFactory.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 10 juin 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTNONLINEARSYSTEMSOLVERFACTORY_HXX
#define LIB_MFRONT_MFRONTNONLINEARSYSTEMSOLVERFACTORY_HXX

#include <map>
#include <string>
#include <memory>
#include "MFront/MFrontConfig.hxx"
#include "MFront/NonLinearSystemSolver.hxx"

namespace mfront {

  /*!
   * register the non linear solvers used by the implicit parser
   */
  struct MFRONT_VISIBILITY_EXPORT NonLinearSystemSolverFactory {
    //! a simple alias
    typedef std::shared_ptr<NonLinearSystemSolver> (*constructor)();
    /*!
     * \return the uniq instance of the solver factory
     */
    static NonLinearSystemSolverFactory& getNonLinearSystemSolverFactory();
    /*!
     * \return the requested solver
     * \param[in] a : solver name
     */
    std::shared_ptr<NonLinearSystemSolver> getSolver(const std::string&) const;
    /*!
     * \param[in] a : solver name
     * \param[in] c : solver constructor
     */
    void registerSolver(const std::string&, const constructor);
    /*!
     */
    std::vector<std::string> getRegistredSolvers() const;

   private:
    /*!
     * default constructor
     */
    NonLinearSystemSolverFactory();
    /*!
     * copy constructor (disabled)
     */
    NonLinearSystemSolverFactory(const NonLinearSystemSolverFactory&);
    /*!
     * assignement operator (disabled)
     */
    NonLinearSystemSolverFactory& operator=(
        const NonLinearSystemSolverFactory&);
    //! all registred constructors
    std::map<std::string, constructor> constructors;
  };  // end of struct NonLinearSystemSolverFactory

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTNONLINEARSYSTEMSOLVERFACTORY_HXX */
