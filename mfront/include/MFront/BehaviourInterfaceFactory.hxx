/*!
 * \file   mfront/include/MFront/BehaviourInterfaceFactory.hxx
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

#ifndef LIB_MFRONBEHAVIOURTINTERFACEFACTORY_HXX_
#define LIB_MFRONBEHAVIOURTINTERFACEFACTORY_HXX_ 

#include<map>
#include<memory>
#include<vector>
#include<string>

#include"MFront/MFrontConfig.hxx"
#include"MFront/AbstractBehaviourInterface.hxx"

namespace mfront{

  struct MFRONT_VISIBILITY_EXPORT BehaviourInterfaceFactory
  {
    typedef std::shared_ptr<AbstractBehaviourInterface> (*InterfaceCreator)(void);
    
    static BehaviourInterfaceFactory&
    getBehaviourInterfaceFactory();

    std::vector<std::string>
    getRegistredInterfaces(void);

    void registerInterfaceCreator(const std::string&,
				  InterfaceCreator);
   
    void
    registerInterfaceAlias(const std::string&,
			   const std::string&);
 
    std::shared_ptr<AbstractBehaviourInterface>
    getInterfacePtr(const std::string&);
    
    void reset(void);

    ~BehaviourInterfaceFactory();
    
  private:

    typedef std::map<std::string,std::string> AliasContainer;
    typedef std::map<std::string,InterfaceCreator> InterfaceCreatorsContainer;
    typedef std::map<std::string,std::shared_ptr<AbstractBehaviourInterface>> InterfaceContainer;

    TFEL_VISIBILITY_LOCAL
    BehaviourInterfaceFactory();

    TFEL_VISIBILITY_LOCAL
    InterfaceCreatorsContainer&
    getInterfaceCreatorsMap(void);

    TFEL_VISIBILITY_LOCAL
    InterfaceContainer&
    getInterfacesMap(void);

    TFEL_VISIBILITY_LOCAL
    AliasContainer&
    getAliasesMap(void);

  };

} // end of namespace mfront  

#endif /* LIB_MFRONBEHAVIOURTINTERFACEFACTORY_HXX_ */

