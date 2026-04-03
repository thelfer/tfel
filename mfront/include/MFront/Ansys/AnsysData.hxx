/*!
 * \file   AnsysData.hxx
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

#ifndef LIB_MFRONT_ANSYS_ANSYSDATA_HXX
#define LIB_MFRONT_ANSYS_ANSYSDATA_HXX

#include "MFront/Ansys/Ansys.hxx"

namespace ansys {

  /*!
   * \brief a structure used to collect the parameters passed through
   * the usermat interface.
   */
  struct AnsysData {
    //! stress values
    AnsysReal *const STRESS;
    //! tangent operator
    AnsysReal *const DDSDDE;
    //! state variables
    AnsysReal *const STATEV;
    //! internal energy
    AnsysReal *const SEDEL;
    //! dissipated energy (e.g. plastic work)
    AnsysReal *const SEDPL;
    //! equivalent plastic strain
    AnsysReal *const EPSEQ;
    //! plastic strain
    AnsysReal *const EPSPL;
    //! axial strain
    AnsysReal *const EPSZZ;
    //! number of components of symmetric tensors
    const AnsysInt NTENS;
    //! number of material properties
    const AnsysInt NPROPS;
    //! number of state variables
    const AnsysInt NSTATV;
    //! time increment
    const AnsysReal DTIME;
    //! infinitesimal rotation matrix increment
    const AnsysReal *const DROT;
    //! strain
    const AnsysReal *const STRAN;
    //! strain increment
    const AnsysReal *const DSTRAN;
    //! temperature
    const AnsysReal *const TEMP;
    //! temperature increment
    const AnsysReal *const DTEMP;
    //! material properties
    const AnsysReal *const PROPS;
    //! out of bounds policy
    const tfel::material::OutOfBoundsPolicy op;
    //! stress free expansion handler
    const StressFreeExpansionHandler<AnsysReal> sfeh;
  };  // end of struct AnsysData

}  // end of namespace ansys

#endif /* LIB_MFRONT_ANSYS_ANSYSDATA_HXX */
