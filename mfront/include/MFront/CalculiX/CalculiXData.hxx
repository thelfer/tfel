/*!
 * \file   CalculiXData.hxx
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

#ifndef LIB_MFRONT_CALCULIX_CALCULIXDATA_HXX
#define LIB_MFRONT_CALCULIX_CALCULIXDATA_HXX

#include "MFront/CalculiX/CalculiX.hxx"

namespace calculix {

  /*!
   * \brief a structure used to collect the parameters passed through
   * the umat interface
   */
  struct CalculiXData {
    CalculiXReal *const STRESS;
    CalculiXReal *const PNEWDT;
    CalculiXReal *const DDSDDE;
    CalculiXReal *const STATEV1;
    //! time increment
    const CalculiXReal DTIME;
    const CalculiXReal *const STATEV0;
    const CalculiXReal *const DV0;
    const CalculiXReal *const DV1;
    //! temperature at the end of the time step
    const CalculiXReal *const TEMP;
    const CalculiXReal *const MPROPS;
    const tfel::material::OutOfBoundsPolicy op;
    const StressFreeExpansionHandler sfeh;
  };  // end of struct CalculiXData

}  // end of namespace calculix

#endif /* LIB_MFRONT_CALCULIX_CALCULIXDATA_HXX */
