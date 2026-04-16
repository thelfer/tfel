/*!
 * \file   MFront/ConfigurationManager.hxx
 * \brief  This file declares the `ConfigurationManager` class
 * \author Thomas Helfer
 * \date   09/04/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CONFIGURATIONMANAGER_HXX
#define LIB_MFRONT_CONFIGURATIONMANAGER_HXX

#include <map>
#include <set>
#include <string>
#include <vector>
#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/GlobalDomainSpecificLanguageOptionsManager.hxx"

namespace mfront {

  /*!
   * \brief class holding the global configuration of `MFront`
   *
   *
   */
  struct ConfigurationManager : GlobalDomainSpecificLanguageOptionsManager {
    /*!
     * \brief list of supported languages (including extensions) for which
     * options may be defined
     */
    enum Language { CXX, C, FORTRAN, CUDA, SYCL };
    //! \brief list of option categories for the supported languages
    enum LanguageOptionCategory {
      PREPROCESSOR_FLAGS,
      INCLUDES_PATHS,
      COMPILATION_FLAGS,
      LINKER_FLAGS,
      LINK_PATHS,
      LIBRARIES
    };
    //! \brief a simple alias
    using DataMap = tfel::utilities::DataMap;
    //! \brief return the unique instance of this class
    [[nodiscard]] static ConfigurationManager& get() noexcept;
    /*!
     * \brief declare options for the given interface
     *
     * \param[in] i: interface name
     * \param[in] opts: options
     *
     * \note the interface must be declared in the one of interface factories
     * (`MaterialPropertyInterfaceFactory`, `BehaviourInterfaceFactory, or
     * `ModelInterfaceFactory), otherwise an exception is thrown
     */
    void addInterfaceOptions(const std::string&, const DataMap&);
    /*!
     * \brief declare options for the given interface
     *
     * \param[in] i: interface name
     * \param[in] opts: options
     *
     * \note the interface must be declared in the
     * `MaterialPropertyInterfaceFactory` factory, otherwise an exception is
     * thrown
     */
    void addMaterialPropertyInterfaceOptions(const std::string&,
                                             const DataMap&);
    /*!
     * \brief declare options for the given interface
     *
     * \param[in] i: interface name
     * \param[in] opts: options
     *
     * \note the interface must be declared in the
     * `BehaviourInterfaceFactory` factory, otherwise an exception is
     * thrown
     */
    void addBehaviourInterfaceOptions(const std::string&, const DataMap&);
    /*!
     * \brief declare options for the given interface
     *
     * \param[in] i: interface name
     * \param[in] opts: options
     *
     * \note the interface must be declared in the
     * `ModelInterfaceFactory` factory, otherwise an exception is
     * thrown
     */
    void addModelInterfaceOptions(const std::string&, const DataMap&);
    /*!
     * \return the options for the given interface
     *
     * \param[in] i: interface name
     *
     * \note the options returned include the ones declared with
     * `addMaterialPropertyInterfaceOptions` and `addInterfaceOptions`
     *
     * \note the interface must be declared in the
     * `MaterialPropertyInterfaceFactory` factory, otherwise an exception is
     * thrown
     *
     * \note if no option is declared for this interface, an empty data map is
     * returned.
     */
    DataMap getMaterialPropertyInterfaceOptions(const std::string&) const;
    /*!
     * \return the options for the given interface
     *
     * \param[in] i: interface name
     * \note the options returned include the ones declared with
     * `addBehaviourInterfaceOptions` and `addInterfaceOptions`
     *
     * \note the interface must be declared in the
     * `BehaviourInterfaceFactory` factory, otherwise an exception is
     * thrown
     */
    DataMap getBehaviourInterfaceOptions(const std::string&) const;
    /*!
     * \return the options for the given interface
     *
     * \param[in] i: interface name
     * \note the options returned include the ones declared with
     * `addModelInterfaceOptions` and `addInterfaceOptions`
     *
     * \note the interface must be declared in the
     * `ModelInterfaceFactory` factory, otherwise an exception is
     * thrown
     */
    DataMap getModelInterfaceOptions(const std::string&) const;
    /*!
     * \brief add a compilation option of the given language
     *
     * \param[in] l: language
     * \param[in] c: category
     * \param[in] o: option
     */
    void addCompilationOption(const Language,
                              const LanguageOptionCategory,
                              const std::string&);
    /*!
     * \brief add a compilation option of the given language
     *
     * \param[in] l: language
     * \param[in] c: category
     * \param[in] opts: options
     */
    void addCompilationOptions(const Language,
                               const LanguageOptionCategory,
                               const std::vector<std::string>&);
    /*!
     * \return the compilation options for the given language and category
     *
     * \param[in] l: language
     * \param[in] c: category
     *
     * \note if no option is declared, an empty set is returned
     */
    [[nodiscard]] std::set<std::string> getCompilationOptions(
        const Language, const LanguageOptionCategory) const;

   private:
    /*!
     * \brief list of options given to an interface, regardless if it is an
     * interface for material properties, behaviours or models.
     */
    std::map<std::string, DataMap> interfaces_options;
    /*!
     * \brief list of options given to interfaces dedicated to material
     * properties
     */
    std::map<std::string, DataMap> material_property_interfaces_options;
    /*!
     * \brief list of options given to interfaces dedicated to behaviours
     */
    std::map<std::string, DataMap> behaviour_interfaces_options;
    /*!
     * \brief list of options given to interfaces dedicated to models
     */
    std::map<std::string, DataMap> model_interfaces_options;
    /*!
     * \brief list of compilation options
     */
    std::map<Language, std::map<LanguageOptionCategory, std::set<std::string>>>
        compilation_options;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_CONFIGURATIONMANAGER_HXX */
