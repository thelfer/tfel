/*!
 * \file  mfront/include/MFront/MFrontWarningMode.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 17/01/2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTWARNINGMODE_HXX
#define LIB_MFRONT_MFRONTWARNINGMODE_HXX

#include <string>
#include <vector>
#include <string_view>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  //! \return true if the warning mode is enabled
  MFRONT_VISIBILITY_EXPORT bool getWarningMode() noexcept;
  /*!
   * \brief enable or diable the warning mode
   * \param[in] b : if true, set the warning mode
   */
  MFRONT_VISIBILITY_EXPORT void setWarningMode(const bool) noexcept;

  //! \return true if the warning error mode is enabled
  MFRONT_VISIBILITY_EXPORT bool getWarningErrorMode() noexcept;
  /*!
   * \brief enable or diable the warning error mode
   * \param[in] b : if true, set the warning mode mode
   */
  MFRONT_VISIBILITY_EXPORT void setWarningErrorMode(const bool) noexcept;

  /*!
   * \brief report a warning if warning mode is enabled
   * \param[in] msg: warning message
   * \note if the warning error mode is enabled, reporting a warning
   * throws an exception. Otherwise, the warning is displayed on the standard
   * log stream
   */
  MFRONT_VISIBILITY_EXPORT void reportWarning(std::string_view);
  /*!
   * \brief report a list of warnings if warning mode is enabled
   * \param[in] warnings: warning messages
   * \note if the warning error mode is enabled, reporting warnings
   * throws an exception. Otherwise, the warnings are displayed on the standard
   * log stream.
   */
  MFRONT_VISIBILITY_EXPORT void reportWarning(const std::vector<std::string>&);
  //! \return if the safe option for keywords or code blocks shall be ignored
  MFRONT_VISIBILITY_EXPORT bool ignoreSafeOptionForWarnings() noexcept;
  //! \brief set if the safe option for keywords or code blocks shall be ignored
  MFRONT_VISIBILITY_EXPORT void setIgnoreSafeOptionForWarnings(
      const bool) noexcept;

}  // namespace mfront

#endif /* LIB_MFRONT_MFRONTWARNINGMODE_HXX */
