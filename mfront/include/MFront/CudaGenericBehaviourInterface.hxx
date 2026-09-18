/*!
 * \file   mfront/include/CudaGenericBehaviourInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \date   30/06/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CUDAGENERICBEHAVIOURINTERFACE_HXX
#define LIB_MFRONT_CUDAGENERICBEHAVIOURINTERFACE_HXX

#include "MFront/GenericBehaviourInterface.hxx"

namespace mfront {

  /*!
   * \brief a generic behaviour interface
   * See https://github.com/thelfer/MFrontGenericInterfaceSupport
   */
  struct MFRONT_VISIBILITY_EXPORT CudaGenericBehaviourInterface
      : public GenericBehaviourInterface {
    //! \return the interface name
    static std::string getName();
    //! \brief destructor
    ~CudaGenericBehaviourInterface() override;
  };  // end of struct GenericBehaviourInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_CUDAGENERICBEHAVIOURINTERFACE_HXX */
