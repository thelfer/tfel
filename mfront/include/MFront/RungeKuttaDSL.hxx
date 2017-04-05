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
   *
   */
  struct RungeKuttaDSL
    : public RungeKuttaDSLBase
  {
    static std::string 
    getName(void);
    static std::string 
    getDescription(void);
    RungeKuttaDSL();
    ~RungeKuttaDSL();
  }; // end of struct RungeKuttaDSL

} // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTRUNGEKUTTAPARSER_HXX */

