/*!
 * \file   ConfigurationManager.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   09/04/2026
 */

#include "MFront/ConfigurationManager.hxx"

namespace mfront {

  ConfigurationManager& ConfigurationManager::get() noexcept {
    static ConfigurationManager m;
    return m;
  }

}