/*!
 * \file   mfront/include/MFront/DSLProxy.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTPARSERPROXY_IXX
#define LIB_MFRONTPARSERPROXY_IXX

namespace mfront {

  template <typename DSLType>
  DSLProxy<DSLType>::DSLProxy() {
    auto& parserFactory = DSLFactory::getDSLFactory();
    parserFactory.registerParserCreator(DSLType::getName(),
                                        &DSLProxy<DSLType>::createParser,
                                        &DSLType::getDescription);
  }

  template <typename DSLType>
  DSLProxy<DSLType>::DSLProxy(const std::string& n) {
    auto& parserFactory = DSLFactory::getDSLFactory();
    parserFactory.registerParserCreator(DSLType::getName(),
                                        &DSLProxy<DSLType>::createParser,
                                        &DSLType::getDescription);
    parserFactory.registerParserCreator(n, &DSLProxy<DSLType>::createParser,
                                        &DSLType::getDescription);
  }

  template <typename DSLType>
  DSLProxy<DSLType>::DSLProxy(const std::vector<std::string>& names) {
    auto& parserFactory = DSLFactory::getDSLFactory();
    parserFactory.registerParserCreator(DSLType::getName(),
                                        &DSLProxy<DSLType>::createParser,
                                        &DSLType::getDescription);
    for (const auto& n : names) {
      parserFactory.registerParserCreator(n, &DSLProxy<DSLType>::createParser,
                                          &DSLType::getDescription);
    }
  }

  template <typename DSLType>
  std::shared_ptr<AbstractDSL> DSLProxy<DSLType>::createParser() {
    return std::make_shared<DSLType>();
  }

}  // end of namespace mfront

#endif /* LIB_MFRONTPARSERPROXY_IXX */
