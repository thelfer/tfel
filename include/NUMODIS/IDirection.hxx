/*!
 * \file   include/NUMODIS/IDirection.hxx
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

#ifndef NUMEODIS_IDIRECTION_HXX
#define NUMEODIS_IDIRECTION_HXX

#include <vector>

#include "NUMODIS/Math/Utilities.hxx"

namespace numodis {

  //===============================================================
  // Class IDirection
  //---------------------------------------------------------------
  //! Handles the indices of a discrete direction
  //===============================================================
  struct IDirection {
    using size_type = std::vector<int>::size_type;

    //===========================================================
    // IDirection::IDirection
    //-----------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------
    /*! \param nindices number of indices                      */
    //===========================================================
    IDirection(size_type nindices) : _index(std::vector<int>(nindices, 0)) {}

    //===========================================================
    // IDirection::IDirection
    //-----------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------
    /*! \param ivector indices of the vector                   */
    //===========================================================
    explicit IDirection(const std::vector<int>& ivector) : _index(ivector) {
      int gcd = numodis::math::GCD(_index);
      if (gcd != 0 && gcd != 1)
        for (unsigned i = 0; i != _index.size(); i++) _index[i] /= gcd;
    }

    //===========================================================
    // IDirection::Reset
    //-----------------------------------------------------------
    //! Reset the direction to 0
    //==========================================================
    void Reset() {
      for (unsigned i = 0; i != _index.size(); i++) _index[i] = 0;
    }

    //===========================================================
    // IDirection::IsNull
    //-----------------------------------------------------------
    //! Determines whether the vector is null or not
    //-----------------------------------------------------------
    /*! \return true if vector=0, false otherwise              */
    //===========================================================
    bool IsNull() const {
      for (unsigned i = 0; i < _index.size(); i++)
        if (_index[i] != 0) return false;
      return true;
    }

    //=============================================================
    // IDirection::setIDirection
    //-------------------------------------------------------------
    //! set the indices of this discrete direction
    //-------------------------------------------------------------
    /*! \param idirection indices of the direction               */
    //=============================================================
    void setIDirection(const std::vector<int>& idirection) {
      _index = idirection;
      int gcd = numodis::math::GCD(_index);
      if (gcd != 0 && gcd != 1)
        for (unsigned i = 0; i != _index.size(); i++) _index[i] /= gcd;
    }

    //=============================================================
    // IDirection::getIndex
    //-------------------------------------------------------------
    //! Return the index of the vector
    //-------------------------------------------------------------
    /*! \return reference on the vector's indices                */
    //=============================================================
    const std::vector<int>& getIndex() const { return _index; }

    //=============================================================
    // IDirection::getNindices
    //-------------------------------------------------------------
    //! Return the number of indices of the vector
    //-------------------------------------------------------------
    /*! \return number of indices                                */
    //=============================================================
    long unsigned int getNindices() const { return _index.size(); }

   protected:
    //! index of the vector
    std::vector<int> _index;

  private:

    friend std::ostream& operator<<(std::ostream&, const IDirection&);
    friend bool operator==(const IDirection&, const IDirection&);
    friend bool operator!=(const IDirection&, const IDirection&);
  };

  TFELNUMODIS_VISIBILITY_EXPORT std::ostream& operator<<(
        std::ostream&, const IDirection&);
  
  TFELNUMODIS_VISIBILITY_EXPORT
  bool operator==(const IDirection& lhs, const IDirection& rhs);

  TFELNUMODIS_VISIBILITY_EXPORT
  bool operator!=(const IDirection& lhs, const IDirection& rhs);

}  // end of namespace numodis

#endif
