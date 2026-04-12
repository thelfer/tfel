/*!
 * \file   MFront/ConfigurationManager.hxx
 * \brief  This file declares the `ConfigurationManager` class
 * \author Thomas Helfer
 * \date   09/04/2026
 */

#ifndef LIB_MFRONT_CONFIGURATIONMANAGER_HXX
#define LIB_MFRONT_CONFIGURATIONMANAGER_HXX

#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/GlobalDomainSpecificLanguageOptionsManager.hxx"

namespace mfront {

  /*!
   * \brief class holding the global configuration of `MFront`
   *
   * 
   */
  struct ConfigurationManager : GlobalDomainSpecificLanguageOptionsManager {
    //! \brief return the unique instance of this class
    [[nodiscard]] static ConfigurationManager& get() noexcept;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_CONFIGURATIONMANAGER_HXX */
