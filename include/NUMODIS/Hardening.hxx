/*!
 * \file   include/NUMODIS/Hardening.hxx
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

#ifndef NUMEODIS_HARDENING1_HXX
#define NUMEODIS_HARDENING1_HXX

#include <vector>
#include <iosfwd>
#include "NUMODIS/Config.hxx"

namespace numodis {

  struct Crystallo;
  struct GSystem;

  //===============================================================
  // class Hardening
  //---------------------------------------------------------------
  //! Compute all the possibles interactions between glide systems
  //===============================================================
  struct TFELNUMODIS_VISIBILITY_EXPORT Hardening {
    Hardening(const Crystallo&, const std::vector<GSystem>&);

    int getNinteractions() const;

    int getRankInteraction(const GSystem& gsystem1,
                           const GSystem& gsystem2) const;

   private:
    TFELNUMODIS_VISIBILITY_EXPORT friend std::ostream& operator<<(
        std::ostream&, const Hardening&);
    //! pointer to the Crystallo object
    const Crystallo& _pcrystallo;
    //! all glide systems
    const std::vector<GSystem> _gsystems;
    //! hardening matrix
    std::vector<std::vector<int>> _coefficients;
    //! interacting glide systems
    std::vector<std::pair<GSystem, GSystem>> _interactions;
  };

  TFELNUMODIS_VISIBILITY_EXPORT  std::ostream& operator<<(std::ostream&, const Hardening&);
  
}  // end of namespace numodis

#endif
