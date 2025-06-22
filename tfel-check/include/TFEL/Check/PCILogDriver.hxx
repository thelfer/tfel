/*!
 * \file PCILogDriver.hxx
 * \brief Interface class of PCLogger Drivers.
 *
 * Interface class of PCLogger Drivers.  Logs are echoed on screen
 * (stdout) unless a filename is given in constructor (of the herited
 * class).
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

#ifndef LIB_TFELCHECK_PCILOGDRIVER_HXX
#define LIB_TFELCHECK_PCILOGDRIVER_HXX

#include <string>
#include <memory>
#include <iosfwd>
#include "TFEL/Check/TFELCheckConfig.hxx"

namespace tfel {

  namespace check {

    struct TFELCHECK_VISIBILITY_EXPORT PCILogDriver {
      //! constructor
      PCILogDriver();
      /*!
       * \brief constructor
       * \param f: string containing the name of the file where logs are
       * to be written. Default : write to stdout (screen).
       */
      PCILogDriver(const std::string&);
      //! move constructor
      PCILogDriver(PCILogDriver&&);
      //! copy constructor
      PCILogDriver(const PCILogDriver&);
      /*!
       * \brief Append a message to log.
       * Append a message to log (to be implemented by inherited class).
       *
       * \param message a message to send to log.
       *
       * \retval <return value> { description }
       */
      virtual void addMessage(const std::string& message) = 0;

      /*!
       * \brief add a test result to log.
       * Append a test result to log (to be implemented by inherited class).
       *
       * \param testname a string message explaining the test
       * \param step a string describing the current step
       * \param command a string describing the test command
       * \param time a float, test time execution
       * \param success a bool test success
       * \param message a string message explaining the test result
       */
      virtual void addTestResult(const std::string& testname,
                                 const std::string& step,
                                 const std::string& command,
                                 float time,
                                 bool success,
                                 const std::string& message = "") = 0;

      /*!
       * \brief terminate driver use.
       *
       * declare end of use of driver to properly close driver (to be overloaded
       * by inherited class if necessary).
       *
       */
      virtual void terminate();
      //! destructor
      virtual ~PCILogDriver();

     protected:
      /*!
       * \brief get current ostream
       *
       * Method to be used by inherited class to obtain an ostream ref that as
       * been redirected to file if necessary.
       *
       * Exemple (for inherited class):
       * this->getStream() << "This is an important message" << endl;
       *
       * \see PCILogDriver()
       * \retval ostream& a reference to current ostream
       */
      std::ostream& getStream();

     private:
      std::shared_ptr<std::ostream> log;
    };

  }  // end of namespace check

}  // end of namespace tfel

#endif /* LIB_TFELCHECK_PCILOGDRIVER_HXX */
