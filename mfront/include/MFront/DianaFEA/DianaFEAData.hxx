/*!
 * \file   DianaFEAData.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   22/03/2016
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_DIANAFEA_DIANAFEADATA_HXX
#define LIB_MFRONT_DIANAFEA_DIANAFEADATA_HXX

#include"MFront/DianaFEA/DianaFEA.hxx"

namespace dianafea{

  /*!
   * \brief a structure used to collect the parameters passed through
   * the usermat interface.
   */
  struct DianaFEAData
  {
    //! stress values
    DianaFEAReal *const STRESS;
    //! tangent operator
    DianaFEAReal *const DDSDDE;
    //! state variables
    DianaFEAReal *const STATEV;
    //! internal energy
    DianaFEAReal *const SEDEL;
    //! dissipated energy (e.g. plastic work)
    DianaFEAReal *const SEDPL;
    //! equivalent plastic strain
    DianaFEAReal *const EPSEQ;
    //! plastic strain
    DianaFEAReal *const EPSPL;
    //! axial strain
    DianaFEAReal *const EPSZZ;
    //! number of components of symmetric tensors
    const DianaFEAInt  NTENS;
    //! number of material properties
    const DianaFEAInt  NPROPS;
    //! number of state variables
    const DianaFEAInt  NSTATV;
    //! time increment
    const DianaFEAReal DTIME;
    //! infinitesimal rotation matrix increment
    const DianaFEAReal *const DROT;
    //! strain
    const DianaFEAReal *const STRAN;
    //! strain increment
    const DianaFEAReal *const DSTRAN;
    //! temperature
    const DianaFEAReal *const TEMP;
    //! temperature increment
    const DianaFEAReal *const DTEMP;
    //! material properties
    const DianaFEAReal *const PROPS;
    //! out of bounds policy
    const tfel::material::OutOfBoundsPolicy op;
    //! stress free expansion handler
    const StressFreeExpansionHandler<DianaFEAReal> sfeh;
  }; // end of struct DianaFEAData
  
} // end of namespace dianafea

#endif /* LIB_MFRONT_DIANAFEA_DIANAFEADATA_HXX */
