/*!
 * \file   include/NUMODIS/IPlane.hxx
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

#ifndef NUMEODIS_IPLANE_HXX
#define NUMEODIS_IPLANE_HXX

#include "NUMODIS/Config.hxx"
#include "NUMODIS/Math/Utilities.hxx"

namespace numodis {

  //===============================================================
  // Class IPlane
  //---------------------------------------------------------------
  //! IPlane handles the indices of a critallographic plane
  //===============================================================
  struct TFELNUMODIS_VISIBILITY_EXPORT IPlane {
    IPlane(IPlane&&);

    IPlane(const IPlane&);

    using size_type = std::vector<int>::size_type;

    IPlane& operator=(const IPlane& rhs);

    //===========================================================
    // IPlane::IPlane
    //-----------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------
    /*! \param nindices number of indices used to store n      */
    //===========================================================
    IPlane(const unsigned nindices = 3) : _index(nindices, 0) {}

    //===========================================================
    // IPlane::IPlane
    //-----------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------
    /*! \param iplane index of the glide plane                */
    //===========================================================
    explicit IPlane(const std::vector<int>& iplane) : _index(iplane) {
      int gcd = numodis::math::GCD(_index);
      if (gcd != 0)
        for (unsigned i = 0; i != _index.size(); i++) _index[i] /= gcd;
    }

    //===========================================================
    // IPlane::setIPlane
    //-----------------------------------------------------------
    //! set the index of the glide plane
    //-----------------------------------------------------------
    /*! \param iplane index of the glide plane                */
    //===========================================================
    void setIPlane(const std::vector<int>& iplane) {
      int gcd = numodis::math::GCD(iplane);
      if (gcd != 0)
        for (unsigned i = 0; i != iplane.size(); i++)
          _index[i] = iplane[i] / gcd;
      else
        for (unsigned i = 0; i != iplane.size(); i++) _index[i] = iplane[i];
    }

    //=============================================================
    // XPlane::getIndex
    //-------------------------------------------------------------
    //! Return the index of the plane
    //-------------------------------------------------------------
    /*! \param pln normal to the plane                           */
    //=============================================================
    void getIndex(std::vector<int>& pln) const { pln = _index; }

    //=============================================================
    // IPlane::getIndex
    //-------------------------------------------------------------
    //! Return the index of the plane
    //-------------------------------------------------------------
    /*! \return reference on the index of the plane              */
    //=============================================================
    const std::vector<int>& getIndex() const { return _index; }

    //=============================================================
    // IPlane::Operator[]
    //-------------------------------------------------------------
    //! Return iplane[i]
    //-------------------------------------------------------------
    /*!
      \param i index rank
      \return iplane[i]
    */
    //=============================================================
    int operator[](const size_type i) const { return this->_index[i]; }

    int& operator[](const size_type i) { return this->_index[i]; }

    //=============================================================
    // IPlane::getNindices
    //-------------------------------------------------------------
    //! Return the number of indices of the plane
    //-------------------------------------------------------------
    /*! \return number of indices                                */
    //=============================================================
    long unsigned int getNindices() const { return _index.size(); }

    //=============================================================
    // IPlane::IsNull
    //-------------------------------------------------------------
    //! Are the indices of the plane all zero?
    //-------------------------------------------------------------
    /*! \return true if only zero                                */
    //=============================================================
    bool IsNull() const {
      for (unsigned i = 0; i < _index.size(); i++)
        if (_index[i] != 0) return false;
      return true;
    }

   protected:
    //! index of the glide plane
    std::vector<int> _index;

   private:
    TFELNUMODIS_VISIBILITY_FRIEND_EXPORT friend std::ostream& operator<<(
        std::ostream&, const IPlane&);
    TFELNUMODIS_VISIBILITY_FRIEND_EXPORT friend bool operator==(const IPlane&,
                                                                const IPlane&);
    TFELNUMODIS_VISIBILITY_FRIEND_EXPORT friend bool operator!=(const IPlane&,
                                                                const IPlane&);
    TFELNUMODIS_VISIBILITY_FRIEND_EXPORT friend bool operator<(const IPlane&,
                                                               const IPlane&);
    TFELNUMODIS_VISIBILITY_FRIEND_EXPORT friend int PlaneCoincide(
        const IPlane&, const IPlane&);
  };

  TFELNUMODIS_VISIBILITY_EXPORT std::ostream& operator<<(std::ostream& os,
                                                         const IPlane& iplane);

  TFELNUMODIS_VISIBILITY_EXPORT
  bool operator==(const IPlane& lhs, const IPlane& rhs);

  TFELNUMODIS_VISIBILITY_EXPORT
  bool operator!=(const IPlane& lhs, const IPlane& rhs);

  TFELNUMODIS_VISIBILITY_EXPORT
  bool operator<(const IPlane& lhs, const IPlane& rhs);

  TFELNUMODIS_VISIBILITY_EXPORT
  int PlaneCoincide(const IPlane& lhs, const IPlane& rhs);

}  // end of namespace numodis

#endif
