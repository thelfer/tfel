/*!
 * \file   MFrontCelaenoModelInterface_v1_3.hxx
 * \brief  This file declares the MFrontCelaenoModelInterface_v1_3 class
 * \author Helfer Thomas
 * \date   10 Nov 2006
 */

#ifndef _LIB_MFRONTCELAENOMODELINTERFACE_V1_3_H_
#define _LIB_MFRONTCELAENOMODELINTERFACE_V1_3_H_ 

#include"MFront/MFrontModelVirtualInterface.hxx"
#include"MFront/MFrontPleiadesModelInterfaceBase.hxx"

namespace mfront{

  struct MFrontCelaenoModelInterface_v1_3
    : public MFrontPleiadesModelInterfaceBase
  {

    static std::string 
    getName(void);

    virtual void
    setOutputFileNames(const MFrontGenericData&);

  }; // end of class MFrontCelaenoModelInterface_v1_3

} // end of namespace mfront  

#endif /* _LIB_MFRONTCELAENOMODELINTERFACE_V1_3_H */

