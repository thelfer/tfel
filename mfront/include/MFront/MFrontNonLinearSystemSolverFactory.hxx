/*! 
 * \file  mfront/include/MFront/MFrontNonLinearSystemSolverFactory.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_MFRONTNONLINEARSYSTEMSOLVERFACTORY_H_
#define _LIB_MFRONT_MFRONTNONLINEARSYSTEMSOLVERFACTORY_H_ 

#include<map>
#include<string>

#include"TFEL/Utilities/SmartPtr.hxx"
#include"MFront/MFrontNonLinearSystemSolver.hxx"

namespace mfront
{
  
  /*!
   * register the non linear solvers used by the implicit parser
   */
  struct MFrontNonLinearSystemSolverFactory
  {
    //! a simple alias
    typedef tfel::utilities::shared_ptr<MFrontNonLinearSystemSolver> (* constructor)(void);
    /*!
     * \return the uniq instance of the solver factory
     */
    static MFrontNonLinearSystemSolverFactory&
    getMFrontNonLinearSystemSolverFactory();
    /*!
     * \return the requested solver
     * \param[in] a : solver name
     */
    tfel::utilities::shared_ptr<MFrontNonLinearSystemSolver>
    getSolver(const std::string&) const;
    /*!
     * \param[in] a : solver name 
     * \param[in] c : solver constructor 
     */
    void
    registerSolver(const std::string&,
		   const constructor);
  private:
    /*!
     * default constructor
     */
    MFrontNonLinearSystemSolverFactory();
    /*!
     * copy constructor (disabled)
     */
    MFrontNonLinearSystemSolverFactory(const MFrontNonLinearSystemSolverFactory&);
    /*!
     * assignement operator (disabled)
     */
    MFrontNonLinearSystemSolverFactory&
    operator=(const MFrontNonLinearSystemSolverFactory&);
    //! all registred constructors
    std::map<std::string,constructor> constructors;
  }; // end of struct MFrontNonLinearSystemSolverFactory

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTNONLINEARSYSTEMSOLVERFACTORY_H */

