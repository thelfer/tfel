/*!
 * \file   tfel-check/include/TFEL/Check/ConfigurationManager.hxx
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

#ifndef LIB_TFEL_CHECK_CONFIGURATIONMANAGER_HXX
#define LIB_TFEL_CHECK_CONFIGURATIONMANAGER_HXX

#include <map>
#include <string>
#include <functional>
#include "TFEL/Check/TFELCheckConfig.hxx"
#include "TFEL/Check/Configuration.hxx"

namespace tfel {

  namespace check {

    /*!
     * \brief configurations are organized as a tree.
     *
     * All modifications to the configuration of the root of the tree
     * are made to the subtrees, called here the subordinates.
     *
     * A location in the tree is a string where each levels are separated
     * by '::'.
     *
     * The `ConfigurationManager` is build in such a way that each
     * change to the root configuration is propagated to all the
     * subordinates.
     */
    struct TFELCHECK_VISIBILITY_EXPORT ConfigurationManager {
      //! \brief default constructor
      ConfigurationManager();
      /*!
       * \brief constructor from a configuration.
       * \param[in] src: configuration
       */
      ConfigurationManager(const Configuration&);
      //! \brief move constructor
      ConfigurationManager(ConfigurationManager&&);
      //! \brief copy constructor
      ConfigurationManager(const ConfigurationManager&);
      //! \brief move assignement
      ConfigurationManager& operator=(ConfigurationManager&&);
      //! \brief standard assignement
      ConfigurationManager& operator=(const ConfigurationManager&);
      /*!
       * \brief modify the configuration
       * \param[in] f: function applying the modification
       */
      void modify(std::function<void(Configuration&)>);
      /*!
       * \return the configuration associated at the specified
       * location.
       * \param[in] l: level
       */
      ConfigurationManager& getConfigurationManager(const std::string&);
      /*!
       * \return the configuration associated at the specified
       * location.
       * \param[in] l: level
       */
      const ConfigurationManager& getConfigurationManager(
          const std::string&) const;
      //! \return the configuration associated at the root of the tree
      const Configuration& getConfiguration() const;

     private:
      /*!
       * \brief decompose a location in two parts: the first level and the rest.
       * \param[in] l: location
       * \return a pair which contains the first level and the rest of the
       * location
       */
      static std::pair<std::string, std::string> extract(const std::string&);
      //! configuration of the root of the tree
      Configuration c;
      //! \brief list of subordinates
      std::map<std::string, ConfigurationManager> subordinates;
    };  // end of ConfigurationManager

    /*!
     * \brief parse a configuration file.
     * \param[in,out] c: configuration
     * \param[in]     n: file name
     */
    TFELCHECK_VISIBILITY_EXPORT void parse(ConfigurationManager&,
                                           const std::string&);

  }  // end of namespace check

}  // namespace tfel

#endif /* LIB_TFEL_CHECK_CONFIGURATIONMANAGER_HXX */
