/*!
 * \file   Argument.hxx
 * \brief
 * \author Thomas Helfer
 * \date 09/06/2016
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
    Argument(std::string);  // NOLINT(google-explicit-constructor)
    //! \param s : argument name
    Argument(const char* const s);  // NOLINT(google-explicit-constructor)
    //
    Argument(Argument&&) noexcept;  // NOLINT(google-explicit-constructor)
    Argument(const Argument&);
    Argument& operator=(Argument&&) noexcept;
    Argument& operator=(const Argument&);
    //! \return true if an option was given for this argument
    [[nodiscard]] bool hasOption() const noexcept;
    //! \brief convertion to string
    [[nodiscard]] std::string& as_string() noexcept;
    //! \brief convertion to string
    [[nodiscard]] const std::string& as_string() const noexcept;
    //! \brief convertion to string (return argument's name)
    explicit operator const std::string&() const noexcept;
    /*!
     * \brief set argument option
     * \param o : option
     */
    void setOption(const std::string& o);
    //! \return argument option
    [[nodiscard]] const std::string& getOption() const noexcept;
    //! \brief destructor
    ~Argument() noexcept;

   private:
    //! \brief argument name
    std::string name;
    //! \brief argument option
    std::string option;
    //! \brief flag telling if an option has been defined
    bool isOptionSet = false;
  };  // end of struct Argument

}  // end of namespace tfel::utilities

#endif /* LIB_TFEL_UTILITIES_ARGUMENT_HXX */
