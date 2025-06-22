/*!
 * \file   src/NUMODIS/Hardening.cxx
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

#include <vector>
#include <ostream>

#include "NUMODIS/Crystallo.hxx"
#include "NUMODIS/GSystem.hxx"
#include "NUMODIS/Hardening.hxx"

namespace numodis {

  //===============================================================
  // Hardening::Hardening
  //---------------------------------------------------------------
  //! Constructor
  //---------------------------------------------------------------
  /*! Compute all the possible interactions between glide systems
   */
  //===============================================================
  Hardening::Hardening(const Crystallo& pcrystallo,
                       const std::vector<GSystem>& gsystems)
      : _pcrystallo(pcrystallo),
        _gsystems(gsystems),
        _coefficients(std::vector<std::vector<int>>(
            gsystems.size(), std::vector<int>(gsystems.size()))) {
    using size_type = decltype(gsystems.size());
    // consider all possible interactions
    for (size_type i = 0; i != gsystems.size(); i++) {
      for (size_type j = 0; j != gsystems.size(); j++) {
        // has this interaction already been considered?
        const auto rankInteraction =
            this->getRankInteraction(gsystems[i], gsystems[j]);
        // store new interaction
        if (rankInteraction == _interactions.size())
          _interactions.push_back(std::make_pair(gsystems[i], gsystems[j]));
        // store corresponding hardening coefficient
        _coefficients[i][j] = rankInteraction;
      }
    }
  }

  int Hardening::getNinteractions() const { return _interactions.size(); }

  //===============================================================
  // Hardening::getRankInteraction
  //---------------------------------------------------------------
  //! return the rank of and interaction
  //---------------------------------------------------------------
  /*!
    This method compares a new interaction (defined by the
    glide system of the two dislocations) to already-stored
    dislocations.

    \param gsystem1 first interacting glide system
    \param gsystem2 second interacting glide system
    \return rank of the interaction if found, number of stored interactions
    otherwise

  */
  //===============================================================
  int Hardening::getRankInteraction(const GSystem& gsystem1,
                                    const GSystem& gsystem2) const {
    using size_type = decltype(this->_interactions.size());
    for (int k = 0; k != _pcrystallo.getNsymmetries(); k++) {
      GSystem g1(_pcrystallo.Symmetry(k, gsystem1));
      GSystem g2(_pcrystallo.Symmetry(k, gsystem2));
      for (size_type i = 0; i != _interactions.size(); i++)
        if (g1 == _interactions[i].first && g2 == _interactions[i].second)
          return i;
    }
    // not found: return the number of already stored interactions
    return _interactions.size();
  }

  //===============================================================
  // operator <<
  //---------------------------------------------------------------
  //! display an Hardening object
  //===============================================================
  std::ostream& operator<<(std::ostream& os, const Hardening& hardening1) {
    using size_type = decltype(hardening1._gsystems.size());
    os << "  => number of glide systems under consideration: "
       << hardening1._gsystems.size() << std::endl;

    for (size_type i = 0; i != hardening1._gsystems.size(); i++)
      os << "      #" << i << ": " << hardening1._gsystems[i] << std::endl;

    os << "  => number of independant coefficients: "
       << hardening1.getNinteractions() << std::endl;

    os << "  => interactions: " << std::endl;
    for (int i = 0; i != hardening1.getNinteractions(); i++) {
      os << "     #" << i
         << ": gliding system: " << hardening1._interactions[i].first
         << " with forest: " << hardening1._interactions[i].second;

      try {
        GSystem junction(hardening1._pcrystallo.ComputeJunctionGSystem(
            hardening1._interactions[i].first,
            hardening1._interactions[i].second));

        os << " => junction: " << junction << std::endl;
      } catch (int msg) {
        if (msg == -1)
          os << " => self interaction " << std::endl;
        else
          os << " => colinear interaction " << std::endl;
      }
    }

    os << "  => interaction matrix " << std::endl;
    for (size_type i = 0; i != hardening1._coefficients.size(); i++) {
      os << "    | ";
      for (size_type j = 0; j != hardening1._coefficients.size(); j++)
        os << hardening1._coefficients[i][j] << " ";
      os << "|" << std::endl;
    }

    return os;
  }

}  // end of namespace numodis
