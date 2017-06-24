/*!
 * \file   CalculiXData.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   22/03/2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_CALCULIX_CALCULIXDATA_HXX
#define LIB_MFRONT_CALCULIX_CALCULIXDATA_HXX

#include"MFront/CalculiX/CalculiX.hxx"

namespace calculix{

  /*!
   * \brief a structure used to collect the parameters passed through
   * the umat interface
   */
  struct CalculiXData
  {
    CalculiXReal *const STRESS;
    CalculiXReal *const PNEWDT;
    CalculiXReal *const DDSDDE;
    CalculiXReal *const STATEV;
    const CalculiXInt  NTENS;
    const CalculiXInt  NPROPS;
    const CalculiXInt  NSTATV;
    const CalculiXReal DTIME;
    const CalculiXReal *const DROT;
    const CalculiXReal *const STRAN;
    const CalculiXReal *const DSTRAN;
    const CalculiXReal *const TEMP;
    const CalculiXReal *const DTEMP;
    const CalculiXReal *const PROPS;
    const CalculiXReal *const PREDEF;
    const CalculiXReal *const DPRED;
    const tfel::material::OutOfBoundsPolicy op;
    const StressFreeExpansionHandler<CalculiXReal> sfeh;
  }; // end of struct CalculiXData
  
} // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIX_CALCULIXDATA_HXX */
