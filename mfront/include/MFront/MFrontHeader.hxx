/*!
 * \file   MFrontHeader.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 nov 2006
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

