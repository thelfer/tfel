/*!
 * \file   MFrontGerminalModelInterface.hxx
 * \brief  This file declares the MFrontGerminalModelInterface class
 * \author Helfer Thomas
 * \date   10 Nov 2006
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
    setOutputFileNames(const MFrontGenericData&);

    virtual void
    writeGetConstantMaterialProperty(const VarHandler&,
				     const MFrontGenericData&,
				     const MFrontModelData&);

    virtual std::string
    getApplicationName(void) const;

    virtual void
    writeInitializeMethod(const MFrontGenericData&,
			  const MFrontModelData&);

    virtual void
    writeSpecificPrivateMethodDeclaration(const MFrontGenericData&,
					  const MFrontModelData&);

    virtual void
    generateDomainsList(const MFrontModelData&);

    virtual bool
    initializeDefaultDomainListInConstrutor(const MFrontModelData&) const;

    void
    writeInitializeParametersMethod(const MFrontGenericData&,
				    const MFrontModelData&);

  }; // end of class MFrontGerminalModelInterface

} // end of namespace mfront  

#endif /* _LIB_MFRONTGERMINALMODELINTERFACE_H */

