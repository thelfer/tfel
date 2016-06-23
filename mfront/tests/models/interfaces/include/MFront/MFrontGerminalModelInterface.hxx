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
    setOutputFileNames(const ModelDescription&) override;

    virtual void
    writeGetConstantMaterialProperty(const VarHandler&,
				     const ModelDescription&) override;

    virtual std::string
    getApplicationName(void) const override;

    virtual void
    writeInitializeMethod(const ModelDescription&) override;

    virtual void
    writeSpecificPrivateMethodDeclaration(const FileDescription&,
					  const ModelDescription&) override;

    virtual void
    generateDomainsList(const ModelDescription&) override;

    virtual void
    buildDomainName() override;

    virtual bool
    initializeDefaultDomainListInConstrutor(const ModelDescription&) const override;

    virtual void
    writeInitializeParametersMethod(const ModelDescription&) override;

  }; // end of class MFrontGerminalModelInterface

} // end of namespace mfront  

#endif /* LIB_MFRONTGERMINALMODELINTERFACE_HXX */

