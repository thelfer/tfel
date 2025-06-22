/*!
 * \file   mfront/include/MFront/MFrontHeader.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTHEADER_HXX
#define LIB_MFRONTHEADER_HXX

#include <string>

#include "MFront/MFrontConfig.hxx"

namespace mfront {

  struct MFRONT_VISIBILITY_EXPORT MFrontHeader {
    static std::string getVersionName();

    static std::string getVersionNumber();

    static std::string getBugReportAdress();

    static std::string getCompilerName();

    static std::string getBuildOs();

    static std::string getBuildCpu();

    static std::string getHostOs();

    static std::string getHostCpu();

    static std::string getTargetOs();

    static std::string getTargetCpu();

    static std::string getHeader(const std::string& = "");

  };  // end of struct MFrontHeader

}  // end of namespace mfront

#endif /* LIB_MFRONTHEADER_HXX */
