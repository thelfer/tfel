/*!
 * \file  mfront/include/MFront/ThermodynamicForce.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 mai 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_THERMODYNAMICFORCE_HXX
#define LIB_MFRONT_THERMODYNAMICFORCE_HXX

#include <string>
#include "MFront/MFrontConfig.hxx"
#include "MFront/VariableDescription.hxx"

namespace mfront {

  /*!
   * \brief structure describes the thermodynamic force associated
   * with a driving variable.
   */
  struct MFRONT_VISIBILITY_EXPORT ThermodynamicForce : VariableDescription {
    /*!
     * Constructor
     * \param[in] t : variable type
     * \param[in] n : variable name
     */
    ThermodynamicForce(const std::string&, const std::string&);
    /*!
     * Constructor
     * \param[in] t : variable type
     * \param[in] s: symbol name
     * \param[in] n : variable name
     */
    ThermodynamicForce(const std::string&,
                       const std::string&,
                       const std::string&);
    /*!
     * \brief constructor from a variable description
     * \param[in] v: variable description
     */
    ThermodynamicForce(const VariableDescription&);
    //! \brief move constructor
    ThermodynamicForce(ThermodynamicForce&&);
    //! \brief copy constructor
    ThermodynamicForce(const ThermodynamicForce&);
    //! \brief move assignement
    ThermodynamicForce& operator=(ThermodynamicForce&&);
    //! \brief standard assignement
    ThermodynamicForce& operator=(const ThermodynamicForce&);
    //! destructor
    ~ThermodynamicForce() noexcept;
  };  // end of struct ThermodynamicForce

}  // end of namespace mfront

#endif /* LIB_MFRONT_THERMODYNAMICFORCE_HXX */
