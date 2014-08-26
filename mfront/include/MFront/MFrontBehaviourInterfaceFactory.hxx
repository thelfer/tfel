/*!
 * \file   MFrontBehaviourInterfaceFactory.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
 */

#ifndef _LIB_MFRONBEHAVIOURTINTERFACEFACTORY_HXX_
#define _LIB_MFRONBEHAVIOURTINTERFACEFACTORY_HXX_ 

#include<vector>
#include<string>
#include<map>

#include"TFEL/Config/TFELConfig.hxx"
#include"MFront/MFrontBehaviourVirtualInterface.hxx"

namespace mfront{

  struct TFEL_VISIBILITY_EXPORT MFrontBehaviourInterfaceFactory
  {
    typedef MFrontBehaviourVirtualInterface* (*InterfaceCreator)(void);
    
    static MFrontBehaviourInterfaceFactory&
    getMFrontBehaviourInterfaceFactory();

    std::vector<std::string>
    getRegistredInterfaces(void);

    void registerInterfaceCreator(const std::string&,
				  InterfaceCreator);
   
    void
    registerInterfaceAlias(const std::string&,
			   const std::string&);
 
    MFrontBehaviourVirtualInterface* 
    getInterfacePtr(const std::string&);
    
    void
    reset(void);

    void
    clear(void);

    ~MFrontBehaviourInterfaceFactory();
    
  private:

    typedef std::map<std::string,std::string> AliasContainer;
    typedef std::map<std::string,InterfaceCreator> InterfaceCreatorsContainer;
    typedef std::map<std::string,MFrontBehaviourVirtualInterface *> InterfaceContainer;

    TFEL_VISIBILITY_LOCAL
    MFrontBehaviourInterfaceFactory();

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

#endif /* _LIB_MFRONTBEHAVIOURINTERFACEFACTORY_HXX */

