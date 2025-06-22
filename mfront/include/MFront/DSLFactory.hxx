/*!
 * \file   mfront/include/MFront/DSLFactory.hxx
 * \brief
 * \author Thomas Helfer
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTPARSERFACTORY_HXX
#define LIB_MFRONTPARSERFACTORY_HXX

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "TFEL/Utilities/Data.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/AbstractDSL.hxx"

namespace mfront {

  /*!
   * \brief an abstract factory for the domain specific languages supported by
   * `MFront` implemented using the singleton pattern.
   */
  struct MFRONT_VISIBILITY_EXPORT DSLFactory {
    //! \brief a simple alias
    using DSLGenerator = std::function<std::shared_ptr<AbstractDSL>(
        const AbstractDSL::DSLOptions&)>;
    //! \brief a simple alias
    using DescriptionGenerator = std::function<std::string()>;
    //!\brief return the unique instance of the `DSLFactory` class.
    static DSLFactory& getDSLFactory();
    /*!
     * \return the list of all registred DSLs.
     * \param[in] b: add the alias
     */
    std::vector<std::string> getRegistredDSLs(const bool = true) const;

    void registerDSLAlias(const std::string&, const std::string&);

    std::string getDSLDescription(const std::string&) const;
    /*!
     * \return an instance of the DSL with the given name
     * \param[in] n: name of the DSL to be generated
     */
    std::shared_ptr<AbstractDSL> createNewDSL(
        const std::string&, const AbstractDSL::DSLOptions& = {}) const;
    /*!
     * \return an instance of the DSL with the given name
     * \param[in] n: name of the DSL to be generated
     */
    [[deprecated]] std::shared_ptr<AbstractDSL> createNewParser(
        const std::string&, const AbstractDSL::DSLOptions& = {}) const;

    std::vector<std::string> getRegistredParsers(const bool = true) const;
    /*!
     * \brief register a new DSL generator
     */
    void registerDSLCreator(const std::string&,
                            DSLGenerator,
                            DescriptionGenerator);

    [[deprecated]] void registerParserCreator(const std::string&,
                                              DSLGenerator,
                                              DescriptionGenerator);

    std::string getParserDescription(const std::string&) const;
    //! \brief destructor
    ~DSLFactory();

   private:
    //! \brief list of generators
    std::map<std::string, DSLGenerator> generators;
    //! \brief list of aliases
    std::map<std::string, std::vector<std::string>> aliases;
    //! \brief list of descriptions
    std::map<std::string, DescriptionGenerator> descriptions;
    //! \brief default constructor
    TFEL_VISIBILITY_LOCAL
    DSLFactory();
  };  // end of struct DSLFactory

}  // end of namespace mfront

#endif /* LIB_MFRONTPARSERFACTORY_HXX */
