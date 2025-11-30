/*!
 * \file   include/NUMODIS/GSystem.hxx
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

#ifndef NUMEODIS_GSYSTEM_HXX
#define NUMEODIS_GSYSTEM_HXX

#include <vector>
#include <iosfwd>

#include "NUMODIS/Config.hxx"
#include "NUMODIS/IBurgers.hxx"
#include "NUMODIS/IPlane.hxx"

namespace numodis {

  struct IDirection;
  struct Crystallo;

  //===============================================================
  // Class GSystem
  //---------------------------------------------------------------
  //! GSystem handles all the properties of a glide system
  //===============================================================
  struct TFELNUMODIS_VISIBILITY_EXPORT GSystem {
    void PrintProperties(std::ostream& os, unsigned shift = 0) const;

    //=============================================================
    // GSystem::GSystem
    //-------------------------------------------------------------
    //! Constructor
    //=============================================================
    GSystem(const unsigned size) : _iburgers(size), _iplane(size) {}

    //=============================================================
    // GSystem::GSystem
    //-------------------------------------------------------------
    //! Constructor
    //-------------------------------------------------------------
    /*!
      \param brg Burgers vector
      \param gpl glide plane
    */
    //=============================================================
    GSystem(const IBurgers& iburgers, const IPlane& iplane)
        : _iburgers(iburgers), _iplane(iplane) {}

    //=============================================================
    // GSystem::GSystem
    //-------------------------------------------------------------
    //! Copy constructor
    //-------------------------------------------------------------
    /*! \param gsystem GSystem to be copied                      */
    //=============================================================
    GSystem(const GSystem& gsystem)
        : _iburgers(gsystem._iburgers), _iplane(gsystem._iplane) {}

    //=============================================================
    // GSystem::getBurgers
    //-------------------------------------------------------------
    //! Return the Burgers vector of the system
    //-------------------------------------------------------------
    /*! \return const reference to the Burgers vector            */
    //=============================================================
    const IBurgers& getIBurgers() const { return _iburgers; }

    //=============================================================
    // GSystem::getIPlane
    //-------------------------------------------------------------
    //! Return the glide plane of the system
    //-------------------------------------------------------------
    /*! \return const reference the glide plane                  */
    //=============================================================
    const IPlane& getIPlane() const { return _iplane; }

    //=============================================================
    // GSystem::getNindices
    //-------------------------------------------------------------
    //! Return the number of indices used to store this system
    //-------------------------------------------------------------
    /*! \return number of indices                                */
    //=============================================================
    IBurgers::size_type getNindices() const { return _iburgers.getNindices(); }

    virtual ~GSystem();

   private:
    //
    TFELNUMODIS_VISIBILITY_EXPORT friend bool operator==(const GSystem&,
                                                         const GSystem&);
    TFELNUMODIS_VISIBILITY_EXPORT friend std::ostream& operator<<(
        std::ostream&, const GSystem&);
    //! \brief Burgers's vector
    IBurgers _iburgers;
    //! \brief constraining planes
    IPlane _iplane;
  };

  TFELNUMODIS_VISIBILITY_EXPORT bool operator==(const GSystem&, const GSystem&);

  TFELNUMODIS_VISIBILITY_EXPORT std::ostream& operator<<(std::ostream&, const GSystem&);

  
}  // end of namespace numodis

#endif
