/*!
 * \file   mfront/include/MFront/MFrontHeader.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTHEADER_HXX_
#define LIB_MFRONTHEADER_HXX_

#include <string>

#include "MFront/MFrontConfig.hxx"

namespace mfront {

  struct MFRONT_VISIBILITY_EXPORT MFrontHeader {
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

  };  // end of struct MFrontHeader

}  // end of namespace mfront

#endif /* LIB_MFRONTHEADER_HXX_ */
