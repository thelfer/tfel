/*!
 * \file   GeneratorOptions.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   16 août 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_GENERATOROPTIONS_HXX_
#define _LIB_MFRONT_GENERATOROPTIONS_HXX_

#include<string>

namespace mfront{

  /*!
   * a data structure holding options passed to generators
   */
  struct GeneratorOptions{
    std::string sys;
    bool oflags0 = false;
    bool oflags  = false;
    bool oflags2 = false;
    bool silentBuild = true;
#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    bool nodeps = true;
#else  /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
    bool nodeps = false;
#endif /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
    bool melt = true;
  }; // end of struct GeneratorOptions

} // end of namespace mfront

#endif /* _LIB_MFRONT_GENERATOROPTIONS_HXX_ */

