/*!
 * \file   mfront/include/MFront/LawFunction.hxx
 * \brief  This file declares the LawFunction class
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_LAWFUNCTION_H_
#define _LIB_LAWFUNCTION_H_ 

#include<string>
#include"TFEL/Config/TFELConfig.hxx"

namespace mfront
{

  struct TFEL_VISIBILITY_EXPORT LawFunction
  {
    LawFunction() = default;
    LawFunction(LawFunction&&) = default;
    LawFunction(const LawFunction&) = default;
    LawFunction& operator=(LawFunction&&) = default;
    LawFunction& operator=(const LawFunction&) = default;
    ~LawFunction();
    std::string body;
    unsigned int line;
    bool modified;
  }; // end of struct LawFunction

} // end of namespace mfront

#endif /* _LIB_LAWFUNCTION_H */

