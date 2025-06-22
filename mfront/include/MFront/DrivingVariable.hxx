/*!
 * \file   mfront/include/MFront/DrivingVariable.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  11 mai 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_DRIVINGVARIABLE_H_
#define LIB_MFRONT_DRIVINGVARIABLE_H_

#include <string>
#include "MFront/MFrontConfig.hxx"

namespace mfront {

  /*!
   * structure in charge of describing the main inputs of a behaviour
   * and their associated thermodynamic forces.  For example, small
   * strain behaviours have the total strain as driving variable.
   */
  struct MFRONT_VISIBILITY_EXPORT DrivingVariable {
    DrivingVariable() = default;
    DrivingVariable(DrivingVariable&&) = default;
    DrivingVariable(const DrivingVariable&) = default;
    DrivingVariable& operator=(DrivingVariable&&) = default;
    DrivingVariable& operator=(const DrivingVariable&) = default;
    ~DrivingVariable() noexcept;
    //! name of the driving variable
    std::string name;
    //! type of the driving variable
    std::string type;
    //! This flags tells which of the driving variable increment or of
    //! the value of driving variable at the end of the time step is
    //! given.
    bool increment_known = false;
    /*!
     * comparison operator
     * \param[in] o : the other driving variable
     */
    bool operator<(const DrivingVariable&) const;
  };  // end of struct DrivingVariable

}  // end of namespace mfront

#endif /* LIB_MFRONT_DRIVINGVARIABLE_H_ */
