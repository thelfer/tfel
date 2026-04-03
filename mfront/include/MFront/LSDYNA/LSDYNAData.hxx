/*!
 * \file   LSDYNAData.hxx
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

#ifndef LIB_MFRONT_LSDYNA_LSDYNADATA_HXX
#define LIB_MFRONT_LSDYNA_LSDYNADATA_HXX

#include "MFront/LSDYNA/LSDYNA.hxx"

namespace lsdyna {

  /*!
   * \brief a structure used to collect the parameters passed through
   * the umat interface
   */
  struct LSDYNAData {
    LSDYNAReal *const STRESS;
    LSDYNAReal *const PNEWDT;
    LSDYNAReal *const DDSDDE;
    LSDYNAReal *const STATEV;
    const LSDYNAInt NTENS;
    const LSDYNAInt NPROPS;
    const LSDYNAInt NSTATV;
    const LSDYNAReal DTIME;
    const LSDYNAReal *const DROT;
    const LSDYNAReal *const STRAN;
    const LSDYNAReal *const DSTRAN;
    const LSDYNAReal *const TEMP;
    const LSDYNAReal *const DTEMP;
    const LSDYNAReal *const PROPS;
    const LSDYNAReal *const PREDEF;
    const LSDYNAReal *const DPRED;
    const tfel::material::OutOfBoundsPolicy op;
    const StressFreeExpansionHandler<LSDYNAReal> sfeh;
  };  // end of struct LSDYNAData

}  // end of namespace lsdyna

#endif /* LIB_MFRONT_LSDYNA_LSDYNADATA_HXX */
