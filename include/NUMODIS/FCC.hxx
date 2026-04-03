/*!
 * \file   include/NUMODIS/FCC.hxx
 * \brief
 * \author Laurent Dupuy
 * \date   9/06/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef NUMEODIS_FCC_HXX
#define NUMEODIS_FCC_HXX

#include <iosfwd>
#include "NUMODIS/Config.hxx"
#include "NUMODIS/Cubic.hxx"

namespace numodis {

  //===============================================================
  // Class FCC
  //---------------------------------------------------------------
  //! FCC crystallography handler using the Wigner-Seitz cell.
  //---------------------------------------------------------------
  /*!
    FCC handles Face-Centered Cubic crystallographic
    structures using the Wigner-Seitz cell.

    The unit vectors \f$\mathbf{a}_1\f$,\f$\mathbf{a}_2\f$ and
    \f$\mathbf{a}_3\f$ are defined as follows:

    \image html FCC_WS.png "FCC unit vectors"
    \image latex FCC_WS.eps "FCC unit vectors"

    The discrete position \f$\mathbf{r}\f$ of a node is defined by a
    triplet of integer \f$(i,j,k)\f$ such that:
    \f[
    \mathbf{r} = i \mathbf{a}_1 + j \mathbf{a}_2 + k \mathbf{a}_3
    \f]

    Following an idea similar to forbidden reflections used
    in crystallography, a triplet \f$(i,j,k)\f$ is admissible if
    and only if \f$ i+j+k=0\bmod{2} \f$.

    \ingroup ClassCrystallo
  */
  //===============================================================
  struct TFELNUMODIS_VISIBILITY_EXPORT FCC : public Cubic {
    FCC();

    FCC(FCC&&);

    FCC(const FCC&);

   private:
    virtual void Init() override;
  };

}  // end of namespace numodis

#endif
