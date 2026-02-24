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

namespace tfel::check {

  /*!
   * \brief configurations are organized as a tree.
   *
   * All modifications to the configuration of the root of the tree
   * are made to the subtrees, called here the subordinates.
   *
   * The `ConfigurationManaoger` is build in such a way that each
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
     * \return the configuration manager assocatied with the given
     * directory.
     * \param[in] n: directory name
     */
    ConfigurationManager& getConfigurationManager(const std::string&);
    /*!
     * \return the configuration associated at the specified
     * location.
     * \param[in] d: relative directory
     */
    const Configuration& getConfiguration(const std::string&) const;
    //! \return the configuration associated at the root of the tree
    const Configuration& getConfiguration() const;
    /*!
     * \brief add a new substitution
     * \param[in] s1: string to be substituted
     * \param[in] s2: substitution string
     * \param[in] b: if true, throws if the substitution already exists.
     * Otherwise, nothing is done.
     */
    void addSubstitution(const std::string&,
                         const std::string&,
                         const bool = true);
    /*!
     * \brief add a new component
     * \param[in] c: component
     */
    void addComponent(const std::string&);
    /*!
     * \brief set discard commands failure
     * \param[in] b: boolean
     */
    void setDiscardCommandsFailure(const bool);

   private:
    /*!
     * \brief decompose a path in two parts: the first level and the rest.
     * \param[in] l: location
     * \return a pair which contains the first level and the rest of the
     * location
     */
    static std::pair<std::string, std::string> extract(const std::string&);
    /*!
     * \brief modify the current configuration and those of the subdirectories
     * \param[in] f: function applying the modification
     */
    void modify(std::function<void(Configuration&)>);
    //! \brief current configuration
    Configuration configuration;
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

}  // end of namespace tfel::check

#endif /* LIB_TFEL_CHECK_CONFIGURATIONMANAGER_HXX */
