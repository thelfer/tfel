/*!
 * \file   include/NUMODIS/BCC.hxx
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

#ifndef NUMEODIS_BCC_HXX
#define NUMEODIS_BCC_HXX

#include <iosfwd>

#include "NUMODIS/Config.hxx"
#include "NUMODIS/Cubic.hxx"

namespace numodis {

  //===============================================================
  // Class BCC
  //---------------------------------------------------------------
  //! BCC crystallography handler using the Wigner-Seitz cell.
  //---------------------------------------------------------------
  /*!
    BCC handles Body-Centered Cubic crystallographic
    structures using the Wigner-Seitz cell.

    The unit vectors \f$\mathbf{a}_1\f$,\f$\mathbf{a}_2\f$ and
    \f$\mathbf{a}_3\f$ are defined as follows:

    \image html BCC_WS.png "BCC unit vectors"
    \image latex BCC_WS.eps "BCC unit vectors"

    The discrete position \f$\mathbf{r}\f$ of a node is defined by a
    triplet of integer (i,j,k) such that:
    \f[
    \mathbf{r} = i \mathbf{a}_1 + j \mathbf{a}_2 + k \mathbf{a}_3
    \f]

    Following an idea similar to forbidden reflections used
    in crystallography, a triplet (i,j,k) is admissible if
    and only if i,j and k are all even or all odd.

    \ingroup ClassCrystallo
  */
  //===============================================================
  struct TFELNUMODIS_VISIBILITY_EXPORT BCC : public Cubic {
    //! \brief default constructor
    BCC();
    //! \brief copy constructor
    BCC(const BCC&);
    //! \brief move constructor
    BCC(BCC&&);

    void PrintData(std::ostream& os) const;
    //! \brief destructor
    ~BCC() override;

   private:
    void Init() override;
  };

}  // end of namespace numodis

#endif
