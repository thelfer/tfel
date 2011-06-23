/*!
 * \file   MFrontAlcyoneModelInterface.hxx
 * \brief  This file declares the MFrontAlcyoneModelInterface class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTALCYONEMODELINTERFACE_H_
#define _LIB_MFRONTALCYONEMODELINTERFACE_H_ 

#include"MFront/MFrontModelVirtualInterface.hxx"
#include"MFront/MFrontPleiadesModelInterfaceBase.hxx"

namespace mfront{

  struct MFrontAlcyoneModelInterface
    : public MFrontPleiadesModelInterfaceBase
  {

    static std::string 
    getName(void);

  private:

    virtual void
    setOutputFileNames(const MFrontGenericData&);

    virtual void
    buildDomainName();

    virtual std::string
    getApplicationName(void) const;

  }; // end of class MFrontAlcyoneModelInterface

} // end of namespace mfront  

#endif /* _LIB_MFRONTALCYONEMODELINTERFACE_H */

