/*!
 * \file   AbaqusData.hxx
 * \brief
 * \author Thomas Helfer
 * \date   22/03/2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_ABAQUS_ABAQUSDATA_HXX
#define LIB_MFRONT_ABAQUS_ABAQUSDATA_HXX

#include "MFront/Abaqus/Abaqus.hxx"

namespace abaqus {

  /*!
   * \brief a structure used to collect the parameters passed through
   * the umat interface
   */
  struct AbaqusData {
    AbaqusReal *const STRESS;
    AbaqusReal *const PNEWDT;
    AbaqusReal *const DDSDDE;
    AbaqusReal *const STATEV;
    //! internal energy
    AbaqusReal *const SSE;
    //! plastic dissipation
    AbaqusReal *const SPD;
    //! visoplastic dissipation
    AbaqusReal *const SCD;
    const AbaqusInt NTENS;
    const AbaqusInt NPROPS;
    const AbaqusInt NSTATV;
    const AbaqusReal DTIME;
    const AbaqusReal *const DROT;
    const AbaqusReal *const STRAN;
    const AbaqusReal *const DSTRAN;
    const AbaqusReal *const TEMP;
    const AbaqusReal *const DTEMP;
    const AbaqusReal *const PROPS;
    const AbaqusReal *const PREDEF;
    const AbaqusReal *const DPRED;
    const tfel::material::OutOfBoundsPolicy op;
    const StressFreeExpansionHandler<AbaqusReal> sfeh;
  };  // end of struct AbaqusData

}  // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUS_ABAQUSDATA_HXX */
