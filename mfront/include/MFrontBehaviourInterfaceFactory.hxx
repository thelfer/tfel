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
#include"MFrontBehaviourVirtualInterface.hxx"

namespace mfront{

  struct MFrontBehaviourInterfaceFactory
  {
    typedef MFrontBehaviourVirtualInterface* (*InterfaceCreator)(void);
    
    static MFrontBehaviourInterfaceFactory&
    getMFrontBehaviourInterfaceFactory();

    void setVerboseMode(void);

    void setDebugMode(void);

    void setWarningMode(void);

    std::vector<std::string>
    getRegistredInterfaces(void);

    void registerInterfaceCreator(const std::string&,
				  InterfaceCreator);
    
    MFrontBehaviourVirtualInterface* 
    getInterfacePtr(const std::string&);
    
    void
    reset(void);

    void
    clear(void);

    ~MFrontBehaviourInterfaceFactory();
    
  private:

    typedef std::map<std::string,InterfaceCreator> InterfaceCreatorsContainer;
    typedef std::map<std::string,MFrontBehaviourVirtualInterface *> InterfaceContainer;

    MFrontBehaviourInterfaceFactory();

    InterfaceCreatorsContainer&
    getInterfaceCreatorsMap(void);

    InterfaceContainer&
    getInterfacesMap(void);

    bool verboseMode;
    bool debugMode;
    bool warningMode;

  };

} // end of namespace mfront  

#endif /* _LIB_MFRONTBEHAVIOURINTERFACEFACTORY_HXX */

