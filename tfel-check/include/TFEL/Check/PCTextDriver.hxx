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

namespace tfel::check {

  struct TFELCHECK_VISIBILITY_EXPORT PCTextDriver : public PCILogDriver {
    //! \brief default constructor
    PCTextDriver();
    /*!
     * \brief default constructor
     * \param[in] b: use terminal colors
     */
    PCTextDriver(const bool);
    /*!
     * \brief default constructor
     * \param[in] os: external stream
     * \param[in] b: use terminal colors
     *
     * \note user is responsible of keeping the stream alive
     */
    PCTextDriver(std::ostream&, const bool = false);
    /*!
     * \brief default constructor
     * \param[in] f: output file
     * \param[in] b: use terminal colors
     */
    PCTextDriver(const std::string&, const bool = false);
    //
    void addMessage(const std::string&) override;
    void reportSkippedTest(const std::string&) override;
    void addTestResult(const std::string&,
                       const std::string&,
                       const std::string&,
                       const float,
                       bool,
                       const std::string& = "") override;
    //! \brief destructor
    ~PCTextDriver() override;

   private:
    const bool use_terminal_colors = false;
  };

}  // end of namespace tfel::check

#endif /* LIB_TFELCHECK_PCTEXTDRIVER_HXX */
