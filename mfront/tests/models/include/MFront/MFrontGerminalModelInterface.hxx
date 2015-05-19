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

  struct MFrontGerminalModelInterface final
    : public MFrontPleiadesModelInterfaceBase
  {

    static std::string 
    getName(void);

  private:

    virtual void
    setOutputFileNames(const ModelData&) override;

    virtual void
    writeGetConstantMaterialProperty(const VarHandler&,
				     const ModelData&) override;

    virtual std::string
    getApplicationName(void) const override;

    virtual void
    writeInitializeMethod(const ModelData&) override;

    virtual void
    writeSpecificPrivateMethodDeclaration(const FileDescription&,
					  const ModelData&) override;

    virtual void
    generateDomainsList(const ModelData&) override;

    virtual void
    buildDomainName() override;

    virtual bool
    initializeDefaultDomainListInConstrutor(const ModelData&) const override;

    virtual void
    writeInitializeParametersMethod(const ModelData&) override;

  }; // end of class MFrontGerminalModelInterface

} // end of namespace mfront  

#endif /* LIB_MFRONTGERMINALMODELINTERFACE_HXX */

