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

#ifndef _LIB_MFRONTGERMINALMODELINTERFACE_H_
#define _LIB_MFRONTGERMINALMODELINTERFACE_H_ 

#include"MFront/MFrontModelVirtualInterface.hxx"
#include"MFront/MFrontPleiadesModelInterfaceBase.hxx"

namespace mfront{

  struct MFrontGerminalModelInterface
    : public MFrontPleiadesModelInterfaceBase
  {

    static std::string 
    getName(void);

  private:

    virtual void
    setOutputFileNames(const MFrontModelData&);

    virtual void
    writeGetConstantMaterialProperty(const VarHandler&,
				     const MFrontModelData&);

    virtual std::string
    getApplicationName(void) const;

    virtual void
    writeInitializeMethod(const MFrontModelData&);

    virtual void
    writeSpecificPrivateMethodDeclaration(const MFrontGenericData&,
					  const MFrontModelData&);

    virtual void
    generateDomainsList(const MFrontModelData&);

    virtual void
    buildDomainName();

    virtual bool
    initializeDefaultDomainListInConstrutor(const MFrontModelData&) const;

    void
    writeInitializeParametersMethod(const MFrontModelData&);

  }; // end of class MFrontGerminalModelInterface

} // end of namespace mfront  

#endif /* _LIB_MFRONTGERMINALMODELINTERFACE_H */

