/*!
 * \file   mfront/src/CudaGenericBehaviourInterface.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   11/07/2023
 */

#include "MFront/CudaGenericBehaviourInterface.hxx"

namespace mfront{

  std::string CudaGenericBehaviourInterface::getName(){
    return "generic-cuda";
  } // end of getName

  CudaGenericBehaviourInterface::~CudaGenericBehaviourInterface() = default;

} // end of namespace mfront