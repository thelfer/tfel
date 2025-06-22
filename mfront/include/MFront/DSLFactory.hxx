/*!
 * \file   mfront/include/MFront/DSLFactory.hxx
 * \brief
 * \author Helfer Thomas
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTPARSERFACTORY_HXX_
#define LIB_MFRONTPARSERFACTORY_HXX_

#include <map>
#include <vector>
#include <string>
#include <memory>

#include "MFront/MFrontConfig.hxx"
#include "MFront/AbstractDSL.hxx"

namespace mfront {

  struct MFRONT_VISIBILITY_EXPORT DSLFactory {
    typedef std::shared_ptr<AbstractDSL> (*ParserCreator)(void);
    typedef std::string (*DescriptionPtr)(void);

    static DSLFactory& getDSLFactory();

    std::vector<std::string> getRegistredParsers(void);

    void registerParserCreator(const std::string&,
                               ParserCreator,
                               DescriptionPtr);

    std::shared_ptr<AbstractDSL> createNewParser(const std::string&);

    std::shared_ptr<AbstractDSL> createNewDSL(const std::string&);

    std::string getParserDescription(const std::string&);

    ~DSLFactory();

   private:
    typedef std::map<std::string, ParserCreator> ParserCreatorsContainer;
    typedef std::map<std::string, DescriptionPtr> DescriptionPtrContainer;

    TFEL_VISIBILITY_LOCAL
    DSLFactory();

    TFEL_VISIBILITY_LOCAL
    DescriptionPtrContainer& getDescriptionMap(void);

    TFEL_VISIBILITY_LOCAL
    ParserCreatorsContainer& getMap(void);

  };  // end of struct DSLFactory

}  // end of namespace mfront

#endif /* LIB_MFRONTPARSERFACTORY_HXX_ */
