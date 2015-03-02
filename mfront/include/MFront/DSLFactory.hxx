/*!
 * \file   mfront/include/MFront/DSLFactory.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTPARSERFACTORY_HXX_
#define LIB_MFRONTPARSERFACTORY_HXX_ 

#include<vector>
#include<string>
#include<map>

#include"TFEL/Config/TFELConfig.hxx"

#include"MFront/AbstractDSL.hxx"
#include"MFront/DefaultDSL.hxx"

namespace mfront{

  struct TFEL_VISIBILITY_EXPORT DSLFactory
  {
    typedef AbstractDSL* (*ParserCreator)(void);
    typedef std::string (*DescriptionPtr)(void);
    
    static DSLFactory&
    getDSLFactory();

    std::vector<std::string>
    getRegistredParsers(void);

    void registerParserCreator(const std::string&,
			       ParserCreator,
			       DescriptionPtr);
    
    AbstractDSL*
    createNewParser(const std::string& = DefaultDSL::getName());

    std::string
    getParserDescription(const std::string&);
    
    ~DSLFactory();

  private:

    typedef std::map<std::string,ParserCreator>  ParserCreatorsContainer;
    typedef std::map<std::string,DescriptionPtr> DescriptionPtrContainer;

    TFEL_VISIBILITY_LOCAL
    DSLFactory();

    TFEL_VISIBILITY_LOCAL
    DescriptionPtrContainer&
    getDescriptionMap(void);

    TFEL_VISIBILITY_LOCAL
    ParserCreatorsContainer&
    getMap(void);

  }; // end of struct DSLFactory

} // end of namespace mfront  

#endif /* LIB_MFRONTPARSERFACTORY_HXX_ */

