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

#include "MFront/MFrontConfig.hxx"
#include "MFront/AbstractDSL.hxx"

namespace mfront {

  struct MFRONT_VISIBILITY_EXPORT DSLFactory {
    //! \brief a simple alias
    using DSLCreator = std::shared_ptr<AbstractDSL> (*)();
    //! \brief a simple alias
    using ParserCreator = DSLCreator;
    //! \brief a simple alias
    using DescriptionPtr = std::string (*)();
    //!\brief return the uniq instance of the `DSLFactory` class.
    static DSLFactory& getDSLFactory();
    /*!
     * \return the list of all registred DSLs.
     * \param[in] b: add the alias
     */
    std::vector<std::string> getRegistredDSLs(const bool = true) const;

    void registerDSLCreator(const std::string&, DSLCreator, DescriptionPtr);

    void registerDSLAlias(const std::string&, const std::string&);

    std::string getDSLDescription(const std::string&) const;

    std::shared_ptr<AbstractDSL> createNewDSL(const std::string&) const;

    std::vector<std::string> getRegistredParsers(const bool = true) const;

    void registerParserCreator(const std::string&,
                               ParserCreator,
                               DescriptionPtr);

    std::string getParserDescription(const std::string&) const;

    std::shared_ptr<AbstractDSL> createNewParser(const std::string&) const;
    //! destructor
    ~DSLFactory();

   private:
    std::map<std::string, std::vector<std::string>> aliases;
    std::map<std::string, DSLCreator> creators;
    std::map<std::string, DescriptionPtr> descriptions;

    TFEL_VISIBILITY_LOCAL
    DSLFactory();

  };  // end of struct DSLFactory

}  // end of namespace mfront

#endif /* LIB_MFRONTPARSERFACTORY_HXX */
