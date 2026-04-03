/*!
 * \file   mfront/include/MFront/Gradient.hxx
 * \brief
 * \author Thomas Helfer
 * \brief  11 mai 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_DRIVINGVARIABLE_HXX
#define LIB_MFRONT_DRIVINGVARIABLE_HXX

#include <string>
#include "MFront/MFrontConfig.hxx"
#include "MFront/VariableDescription.hxx"

namespace mfront {

  /*!
   * structure in charge of describing the main inputs of a behaviour
   * and their associated thermodynamic forces.  For example, small
   * strain behaviours have the total strain as driving variable.
   */
  struct MFRONT_VISIBILITY_EXPORT Gradient : public VariableDescription {
    /*!
     * \brief a simple helper function to set an attribute stating if the
     * gradient increment is known.
     * \param[in] v: variable
     */
    static void setIsIncrementKnownAttribute(VariableDescription&, const bool);
    //! \return if the increment of a variable is known
    static bool isIncrementKnown(const VariableDescription&);
    /*!
     * \brief Constructor
     * \param[in] t : variable type
     * \param[in] n : variable name
     */
    Gradient(const std::string&, const std::string&);
    /*!
     * \brief Constructor
     * \param[in] t: variable type
     * \param[in] s: symbol name
     * \param[in] n: variable name
     */
    Gradient(const std::string&, const std::string&, const std::string&);
    /*!
     * \brief constructor from a variable description
     * \param[in] v: variable description
     */
    Gradient(const VariableDescription&);
    //! \brief move constructor
    Gradient(Gradient&&);
    //! \brief copy constructor
    Gradient(const Gradient&);
    //! \brief move assignement
    Gradient& operator=(Gradient&&);
    //! \brief standard assignement
    Gradient& operator=(const Gradient&);
    //! destructor
    ~Gradient() noexcept;
  };  // end of struct Gradient

}  // end of namespace mfront

#endif /* LIB_MFRONT_DRIVINGVARIABLE_HXX */
