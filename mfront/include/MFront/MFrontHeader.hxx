/*!
 * \file   mfront/include/MFront/MFrontHeader.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONTHEADER_HXX_
#define _LIB_MFRONTHEADER_HXX_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"

namespace mfront{

  struct TFEL_VISIBILITY_EXPORT MFrontHeader{

    static std::string getVersionName(void);

    static std::string getVersionNumber(void);

    static std::string getBugReportAdress(void);

    static std::string getCompilerName(void);

    static std::string getBuildOs(void);

    static std::string getBuildCpu(void);

    static std::string getHostOs(void);

    static std::string getHostCpu(void);

    static std::string getTargetOs(void);

    static std::string getTargetCpu(void);

    static std::string getHeader(const std::string& = "");      
    
  }; // end of struct MFrontHeader

} // end of namespace mfront

#endif /* _LIB_MFRONTHEADER_HXX */

