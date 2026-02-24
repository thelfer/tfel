/*!
 * \file   mfront/include/MFront/GlobalDomainSpecificLanguageOptionsManager.hxx
 * \brief
 * \author Thomas Helfer
 * \date   17/01/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GLOBALDOMAINSPECIFICLANGUAGEOPTIONSMANAGER_HXX
#define LIB_MFRONT_GLOBALDOMAINSPECIFICLANGUAGEOPTIONSMANAGER_HXX

#include <string_view>
#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * \brief a class handling global options passed to domain specific languages.
   * Those global options are generally defined by command-line arguments
   */
  struct MFRONT_VISIBILITY_EXPORT GlobalDomainSpecificLanguageOptionsManager {
    //! \return the unique instance of this class
    static GlobalDomainSpecificLanguageOptionsManager& get();
    /*!
     * \brief add an option that will be passed to all DSLs
     * \param[in] n: name of the option
     * \param[in] o: value of the option
     */
    void addDSLOption(const std::string&, const std::string&);
    /*!
     * \brief add an option that will be passed to DSLs related to material
     * properties.
     * \param[in] n: name of the option
     * \param[in] o: value of the option
     */
    void addMaterialPropertyDSLOption(const std::string&, const std::string&);
    /*!
     * \brief add an option that will be passed to DSLs related to behaviours.
     * \param[in] n: name of the option
     * \param[in] o: value of the option
     */
    void addBehaviourDSLOption(const std::string&, const std::string&);
    /*!
     * \brief add an option that will be passed to DSLs related to models.
     * \param[in] n: name of the option
     * \param[in] o: value of the option
     */
    void addModelDSLOption(const std::string&, const std::string&);
    /*!
     * \brief add an option that will be passed to all DSLs
     * \param[in] n: name of the option
     * \param[in] o: value of the option
     */
    void addDSLOption(const std::string&, const tfel::utilities::Data&);
    /*!
     * \brief add an option that will be passed to DSLs related to material
     * properties.
     * \param[in] n: name of the option
     * \param[in] o: value of the option
     */
    void addMaterialPropertyDSLOption(const std::string&,
                                      const tfel::utilities::Data&);
    /*!
     * \brief add an option that will be passed to DSLs related to behaviours.
     * \param[in] n: name of the option
     * \param[in] o: value of the option
     */
    void addBehaviourDSLOption(const std::string&,
                               const tfel::utilities::Data&);
    /*!
     * \brief add an option that will be passed to DSLs related to models.
     * \param[in] n: name of the option
     * \param[in] o: value of the option
     */
    void addModelDSLOption(const std::string&, const tfel::utilities::Data&);
    /*!
     * \return the global options to be passed to DSLs related to material
     * properties.
     */
    tfel::utilities::DataMap getMaterialPropertyDSLOptions() const;
    //! \return the global options to be passed to DSLs related to behaviours.
    tfel::utilities::DataMap getBehaviourDSLOptions() const;
    //! \return the global options to be passed to DSLs related to models.
    tfel::utilities::DataMap getModelDSLOptions() const;

   private:
    //! \brief default constructor
    GlobalDomainSpecificLanguageOptionsManager();
    //! \brief destructor
    ~GlobalDomainSpecificLanguageOptionsManager();
    /*!
     * \brief global options passed to DSLs related to material
     * properties
     */
    tfel::utilities::DataMap material_property_dsl_options;
    //! \brief global options passed to DSLs related to behaviours
    tfel::utilities::DataMap behaviour_dsl_options;
    //! \brief global options passed to DSLs related to models
    tfel::utilities::DataMap model_dsl_options;
  };  // end of struct GlobalDomainSpecificLanguageOptionsManager

}  // end of namespace mfront

#endif /* LIB_MFRONT_GLOBALDOMAINSPECIFICLANGUAGEOPTIONSMANAGER_HXX */
