/*! 
 * \file   mfront/include/MFront/RungeKuttaDSL.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  17/10/2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_MFRONTRUNGEKUTTAPARSER_HXX
#define LIB_MFRONT_MFRONTRUNGEKUTTAPARSER_HXX 

#include"MFront/RungeKuttaDSLBase.hxx"

namespace mfront
{

  /*!
   * \brief DSL for integrating a mechanical behaviour using
   * Runge-Kutta algorithms.
   */
  struct RungeKuttaDSL
    : public RungeKuttaDSLBase
  {
    //! \return the name of the DSL
    static std::string getName(void);
    //! \return the description of the DSL
    static std::string getDescription(void);
    //! constructor
    RungeKuttaDSL();
    //! destructor
    ~RungeKuttaDSL();
  }; // end of struct RungeKuttaDSL

} // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTRUNGEKUTTAPARSER_HXX */

