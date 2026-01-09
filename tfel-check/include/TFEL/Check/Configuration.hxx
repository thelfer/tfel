/*!
 * \file   tfel-check/include/TFEL/Check/Configuration.hxx
 * \brief
 * \author Thomas Helfer
 * \date   13/09/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_CHECK_CONFIGURATION_HXX
#define LIB_TFEL_CHECK_CONFIGURATION_HXX

#include <map>
#include <vector>
#include <string>
#include "TFEL/Check/TFELCheckConfig.hxx"
#include "TFEL/Check/PCLogger.hxx"

namespace tfel::check {

  /*!
   * \brief a data structure used to define a test environment.
   */
  struct TFELCHECK_VISIBILITY_EXPORT Configuration {
    //! \brief default constructor
    Configuration();
    //! \brief move constructor
    Configuration(Configuration&&);
    //! \brief copy constructor
    Configuration(const Configuration&);
    //! \brief move assignement
    Configuration& operator=(Configuration&&);
    //! \brief standard assignement
    Configuration& operator=(const Configuration&);
    /*!
     * \brief discard command's failure if comparisons are ok.
     * \note If no comparisons is declared, command's failure is never ignored.
     */
    bool discard_commands_failure = true;
    //! \brief list of components
    std::vector<std::string> available_components;
    /*!
     * \brief substitutions applied to `.check` files.
     *
     * For every {key,value} pair, every occurence of @key@ is
     * replaced by value.
     */
    std::map<std::string, std::string> substitutions;
    //! \brief directory in which the test is executed
    std::string directory;
    //! \brief global logger
    PCLogger log;
  };  // end of struct Configuration

}  // end of namespace tfel::check

#endif /* LIB_TFEL_CHECK_CONFIGURATION_HXX */
