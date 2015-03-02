/*!
 * \file   mfront/tests/models/include/MFront/MFrontGerminalModelInterface.hxx
 * \brief  This file declares the MFrontGerminalModelInterface class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTGERMINALMODELINTERFACE_HXX
#define LIB_MFRONTGERMINALMODELINTERFACE_HXX 

#include"MFront/MFrontPleiadesModelInterfaceBase.hxx"

namespace mfront{

  struct MFrontGerminalModelInterface
    : public MFrontPleiadesModelInterfaceBase
  {

    static std::string 
    getName(void);

  private:

    virtual void
    setOutputFileNames(const ModelData&);

    virtual void
    writeGetConstantMaterialProperty(const VarHandler&,
				     const ModelData&);

    virtual std::string
    getApplicationName(void) const;

    virtual void
    writeInitializeMethod(const ModelData&);

    virtual void
    writeSpecificPrivateMethodDeclaration(const GenericData&,
					  const ModelData&);

    virtual void
    generateDomainsList(const ModelData&);

    virtual void
    buildDomainName();

    virtual bool
    initializeDefaultDomainListInConstrutor(const ModelData&) const;

    void
    writeInitializeParametersMethod(const ModelData&);

  }; // end of class MFrontGerminalModelInterface

} // end of namespace mfront  

#endif /* LIB_MFRONTGERMINALMODELINTERFACE_HXX */

