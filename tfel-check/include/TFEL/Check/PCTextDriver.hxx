/*!
 * \file PCTextDriver.hxx
 * \brief ASCII text log formater class
 *
 * Output log in unformatted ASCII text.
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

#ifndef LIB_TFELCHECK_PCTEXTDRIVER_HXX
#define LIB_TFELCHECK_PCTEXTDRIVER_HXX

#include <string>
#include "TFEL/Check/TFELCheckConfig.hxx"
#include "TFEL/Check/PCILogDriver.hxx"

namespace tfel {

  namespace check {

    struct TFELCHECK_VISIBILITY_EXPORT PCTextDriver : public PCILogDriver {
      PCTextDriver();
      PCTextDriver(const std::string&);
      void addMessage(const std::string&) override;
      /*!
       * \param[in] testname:
       * \param[in] step:
       * \param[in] command:
       * \param[in] time:
       * \param[in] success:
       * \param[in] message:
       */
      void addTestResult(const std::string&,
                         const std::string&,
                         const std::string&,
                         const float,
                         bool,
                         const std::string& = "") override;
      //! destructor
      ~PCTextDriver() override;
    };

  }  // end of namespace check

}  // end of namespace tfel

#endif /* LIB_TFELCHECK_PCTEXTDRIVER_HXX */
