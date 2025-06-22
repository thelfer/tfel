/*!
 * \brief JUnit log formater class
 *
 * \file PCJUnitDriver.hxx
 *
 * Output log according to JUnit format (to be used with Hudson)
 *
 * \author sb152252
 * \date 1 sept. 2009
 *
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_PCJUNITDRIVER_HXX
#define LIB_TFELCHECK_PCJUNITDRIVER_HXX

#include <string>
#include "TFELCheck/TFELCheckConfig.hxx"
#include "TFELCheck/PCILogDriver.hxx"

namespace tfel_check {

  struct TFELCHECK_VISIBILITY_EXPORT PCJUnitDriver : public PCILogDriver {
    PCJUnitDriver(const std::string& f = "");
    //! destructor
    virtual ~PCJUnitDriver();

    virtual void addMessage(const std::string&) override;
    /*!
     * \param[in] testname:
     * \param[in] step:
     * \param[in] command:
     * \param[in] time:
     * \param[in] success:
     * \param[in] message:
     */
    virtual void addTestResult(const std::string&,
                               const std::string&,
                               const std::string&,
                               const float,
                               const bool,
                               const std::string& = "") override;
    virtual void terminate() override;
  };

}  // namespace tfel_check

#endif /* LIB_TFELCHECK_PCJUNITDRIVER_HXX */
