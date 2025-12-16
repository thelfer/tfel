/*!
 * \file   include/NUMODIS/IBurgers.hxx
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

#ifndef NUMEODIS_IBURGERS_HXX
#define NUMEODIS_IBURGERS_HXX

#include <vector>
#include <iosfwd>

#include "NUMODIS/Config.hxx"

namespace numodis {

  //===============================================================
  // Class IBurgers
  //---------------------------------------------------------------
  //! IBurgers handles the index of a burgers vector
  //===============================================================
  struct TFELNUMODIS_VISIBILITY_EXPORT IBurgers {
    IBurgers(IBurgers&&);

    IBurgers(const IBurgers&);

    using size_type = std::vector<int>::size_type;

    IBurgers operator+(const IBurgers& rhs) const;

    IBurgers operator-(const IBurgers& rhs) const;

    IBurgers& operator=(const IBurgers& rhs);

    IBurgers& operator+=(const IBurgers& rhs);

    IBurgers& operator-=(const IBurgers& rhs);

    IBurgers& operator/=(const int scalar);

    virtual ~IBurgers();

    //===========================================================
    // IBurgers::IBurgers
    //-----------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------
    /*! \param nindices number of indices used to store b      */
    //===========================================================
    IBurgers(const unsigned nindices = 3) : _index(nindices, 0) {}

    //===========================================================
    // IBurgers::IBurgers
    //-----------------------------------------------------------
    //! Constructor
    //-----------------------------------------------------------
    /*! \param iburgers index of the burgers vector             */
    //===========================================================
    explicit IBurgers(const std::vector<int>& iburgers) : _index(iburgers) {}

    //===========================================================
    // IBurgers::setIBurgers
    //-----------------------------------------------------------
    //! set the index of the Burgers vector
    //-----------------------------------------------------------
    /*! \param index index of the Burgers vector               */
    //===========================================================
    void setIBurgers(const std::vector<int>& index) { _index = index; }

    //===========================================================
    // IBurgers::IsNull
    //-----------------------------------------------------------
    //! Determines whether the burgers vector is null or not
    //-----------------------------------------------------------
    /*! \return true if burgers=0, false otherwise             */
    //===========================================================
    bool IsNull() const {
      for (unsigned i = 0; i < _index.size(); i++)
        if (_index[i] != 0) return false;
      return true;
    }

    //=============================================================
    // IBurgers::getIndex
    //-------------------------------------------------------------
    //! Return the index of the Burgers vector
    //-------------------------------------------------------------
    /*! \return index of the Burgers vector                      */
    //=============================================================
    void getIndex(std::vector<int>& b) const { b = _index; }

    //=============================================================
    // IBurgers::getIndex
    //-------------------------------------------------------------
    //! Return the index of the Burgers vector
    //-------------------------------------------------------------
    /*! \return reference on the Burgers vector's index          */
    //=============================================================
    const std::vector<int>& getIndex() const { return _index; }

    //=============================================================
    // IBurgers::getNindices
    //-------------------------------------------------------------
    //! Return the number of indices of the Burgers vector
    //-------------------------------------------------------------
    /*! \return number of indices                                */
    //=============================================================
    size_type getNindices() const { return _index.size(); }

    //=============================================================
    // IBurgers::Operator[]
    //-------------------------------------------------------------
    //! Return iburgers[i]
    //-------------------------------------------------------------
    /*!
      \param i index rank
      \return iburgers[i]
    */
    //=============================================================
    int operator[](const size_type i) const { return _index[i]; }

    int& operator[](const size_type i) { return _index[i]; }

   protected:
    //! index of the burgers vector
    std::vector<int> _index;

   private:
    TFELNUMODIS_VISIBILITY_FRIEND_EXPORT friend int Coincide(const IBurgers&,
                                                             const IBurgers&);
    TFELNUMODIS_VISIBILITY_FRIEND_EXPORT
    friend std::ostream& operator<<(std::ostream&, const IBurgers&);
  };

  TFELNUMODIS_VISIBILITY_EXPORT
  int Coincide(const IBurgers&, const IBurgers&);

  TFELNUMODIS_VISIBILITY_EXPORT std::ostream& operator<<(std::ostream&,
                                                         const IBurgers&);

}  // end of namespace numodis

#endif
