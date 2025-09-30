/*!
 * \file   Argument.hxx
 * \brief
 * \author Thomas Helfer
 * \date   09 juin 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_ARGUMENT_HXX
#define LIB_TFEL_UTILITIES_ARGUMENT_HXX

#include <string>
#include "TFEL/Config/TFELConfig.hxx"

namespace tfel::utilities {

  /*!
   * \brief class holding a command line argument
   *
   * This class is used internally.
   * \note inheriting from std::string shall be forbidden in most
   *cases. Here its does not harm.
   */
  struct TFELUTILITIES_VISIBILITY_EXPORT Argument {
    //! \param s : argument name
    Argument(std::string);
    //! \param s : argument name
    Argument(const char* const s);
    Argument(Argument&&);
    Argument(const Argument&);
    Argument& operator=(Argument&&);
    Argument& operator=(const Argument&);
    //! \return true if an option was given for this argument
    bool hasOption() const noexcept;
    //! convertion to string
    std::string& as_string() noexcept;
    //! convertion to string
    const std::string& as_string() const noexcept;
    //! convertion to string (return argument's name)
    explicit operator const std::string&() const noexcept;
    /*!
     * \brief set argument option
     * \param o : option
     */
    void setOption(const std::string& o);
    //! \return argument option
    const std::string& getOption() const noexcept;
    //! destructor
    ~Argument() noexcept;

   private:
    //! argument name
    std::string name;
    //! argument option
    std::string option;
    //! flag telling if an option has been defined
    bool isOptionSet = false;
  };  // end of struct Argument

}  // end of namespace tfel::utilities

#endif /* LIB_TFEL_UTILITIES_ARGUMENT_HXX */
