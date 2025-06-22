/*!
 * \file PCLogger.hxx
 * \brief A class designed to manage logs with different format.
 *
 * \author sb152252
 * \date 31 août 2009
 *
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_PCLOGGER_HXX
#define LIB_TFELCHECK_PCLOGGER_HXX

#include <vector>
#include <memory>
#include "TFEL/Check/TFELCheckConfig.hxx"

namespace tfel::check {

  // forward declaration
  struct PCILogDriver;

  struct TFELCHECK_VISIBILITY_EXPORT PCLogger {
    //! a simple alias
    using driver = std::shared_ptr<PCILogDriver>;
    //! \brief default constructor
    PCLogger();
    /*!
     * \param[in] logDriver
     */
    PCLogger(const driver&);
    //! \brief move constructor
    PCLogger(PCLogger&&);
    //! \brief copy constructor
    PCLogger(const PCLogger&);
    //! \brief move assignement
    PCLogger& operator=(PCLogger&&);
    //! \brief standard assignement
    PCLogger& operator=(const PCLogger&);
    //! \brief destructor
    virtual ~PCLogger();

    /*!
     * \brief Add a driver (an output format) to PCLogger
     *
     * \param logDriver a PCILogDriver object.
     *
     * \see PCILogDriver
     * Exemple :
     * PCLogger log;
     * ...
     * log.addDriver(new PCTextDriver())
     */
    void addDriver(driver logDriver);

    /*!
     * \brief add a log message to all log outputs
     *
     * \param message a message to send to log.
     */
    void addMessage(const std::string& message);
    /*!
     * \brief report that a test has been skipped
     * \param[in] d: description of the test
     */
    void reportSkippedTest(const std::string&);
    /*!
     * \brief add a simple test result to all log outputs
     *
     * \param testname a string message explaining the test
     * \param success a bool test success
     * \param message a string message explaining the test result
     */
    void addSimpleTestResult(const std::string& testname,
                             const bool success,
                             const std::string& message = "");

    /*!
     * \brief add a test result to all log outputs
     *
     * \param testname a string message explaining the test
     * \param step a string describing the current step
     * \param command a string describing the test command
     * \param time a float, test time execution
     * \param success a bool test success
     * \param message a string message explaining the test result
     */
    void addTestResult(const std::string& testname,
                       const std::string& step,
                       const std::string& command,
                       const float time,
                       const bool success,
                       const std::string& message = "");

    void terminate();

   private:
    std::vector<driver> drivers;
  };

}  // end of namespace tfel::check

#endif /* LIB_TFELCHECK_PCLOGGER_HXX */
