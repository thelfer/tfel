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
#include <string>
#include "TFEL/Check/TFELCheckConfig.hxx"

namespace tfel {

  namespace check {

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
       * \brief substitutions applied to `.check` files.
       *
       * For every {key,value} pair, every occurence of @key@ is
       * replaced by value.
       */
      std::map<std::string, std::string> substitutions;
    };  // end of struct Configuration

  }  // end of namespace check

}  // namespace tfel

#endif /* LIB_TFEL_CHECK_CONFIGURATION_HXX */
