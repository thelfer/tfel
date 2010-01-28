/*!
 * \file   MFrontLawInterfaceFactory.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
 */

#ifndef _LIB_MFRONTLAWINTERFACEFACTORY_HXX_
#define _LIB_MFRONTLAWINTERFACEFACTORY_HXX_ 

#include<vector>
#include<string>
#include<map>

#include"MFront/MFrontLawVirtualInterface.hxx"

namespace mfront{

  struct MFrontLawInterfaceFactory
  {

    typedef MFrontLawVirtualInterface* (*InterfaceCreator)(void);
    
    static MFrontLawInterfaceFactory&
    getMFrontLawInterfaceFactory();

    void setVerboseMode(void);

    void setDebugMode(void);

    void setWarningMode(void);

    std::vector<std::string>
    getRegistredInterfaces(void);

    void
    registerInterfaceCreator(const std::string&,
			     InterfaceCreator);

    void
    registerInterfaceAlias(const std::string&,
			   const std::string&);

    void
    registerInterfaceDependency(const std::string&,
				const std::string&);
    
    std::vector<std::string>
    getInterfaceDependencies(const std::string&);

    MFrontLawVirtualInterface* 
    getInterfacePtr(const std::string&);
    
    void
    reset(void);

    void
    clear(void);

    ~MFrontLawInterfaceFactory();

  private:

    typedef std::map<std::string,std::string> AliasContainer;
    typedef std::map<std::string,InterfaceCreator> InterfaceCreatorsContainer;
    typedef std::map<std::string,MFrontLawVirtualInterface *> InterfaceContainer;
    typedef std::map<std::string,std::vector<std::string> > InterfaceDependencyContainer;

    MFrontLawInterfaceFactory();

    InterfaceDependencyContainer&
    getDependenciesMap(void);

    InterfaceCreatorsContainer&
    getInterfaceCreatorsMap(void);

    InterfaceContainer&
    getInterfacesMap(void);

    AliasContainer&
    getAliasesMap(void);

    bool verboseMode;
    bool debugMode;
    bool warningMode;

  };

} // end of namespace mfront  

#endif /* _LIB_MFRONTLAWINTERFACEFACTORY_HXX */

