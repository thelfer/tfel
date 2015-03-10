/*!
 * \file   mfront/include/MFront/ModelInterfaceFactory.hxx
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

#ifndef LIB_MFRONMODELTINTERFACEFACTORY_HXX_
#define LIB_MFRONMODELTINTERFACEFACTORY_HXX_ 

#include<map>
#include<memory>
#include<vector>
#include<string>

#include"MFront/MFrontConfig.hxx"
#include"MFront/AbstractModelInterface.hxx"

namespace mfront{

  struct MFRONT_VISIBILITY_EXPORT ModelInterfaceFactory
  {
    typedef std::shared_ptr<AbstractModelInterface> (*InterfaceCreator)(void);
    
    static ModelInterfaceFactory&
    getModelInterfaceFactory();

    std::vector<std::string>
    getRegistredInterfaces(void);

    void registerInterfaceCreator(const std::string&,
				  InterfaceCreator);
    
    std::shared_ptr<AbstractModelInterface>
    getInterfacePtr(const std::string&);
    
    void reset(void);

    ~ModelInterfaceFactory();
    
  private:

    typedef std::map<std::string,InterfaceCreator> InterfaceCreatorsContainer;
    typedef std::map<std::string,std::shared_ptr<AbstractModelInterface>> InterfaceContainer;

    TFEL_VISIBILITY_LOCAL
    ModelInterfaceFactory();

    TFEL_VISIBILITY_LOCAL
    InterfaceCreatorsContainer&
    getInterfaceCreatorsMap(void);

    TFEL_VISIBILITY_LOCAL
    InterfaceContainer&
    getInterfacesMap(void);

  };

} // end of namespace mfront  

#endif /* LIB_MFRONMODELTINTERFACEFACTORY_HXX_ */

