/*!
 * \file   include/TFEL/Utilities/ArgumentParserBase.hxx
 * \brief  This file declares the ArgumentParserBase class
 * \author Thomas Helfer
 * \date   17 Nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_UTILITIES_ARGUMENTPARSERBASE_HXX
#define LIB_TFEL_UTILITIES_ARGUMENTPARSERBASE_HXX

#include <map>
#include <vector>
#include <string>

#include "TFEL/Utilities/ArgumentParser.hxx"

namespace tfel {

  namespace utilities {

    /*!
     * \brief an helper class used for command line argument parsing.
     *
     * The CRTP pattern is used to enable ArgumentParserBase to used
     * the method of its derivate class. Such a design was extremly
     * usefull before std::function was introduced.
     *
     * The class maps the command line arguments to method of the
     * derivate class.
     *
     * \param Child
     */
    template <typename Child>
    struct ArgumentParserBase : public ArgumentParser {
      //! a simple alias
      using MemberFuncPtr = void (Child::*)();
      //! default constructor
      ArgumentParserBase();
      /*!
       * \brief constructor
       * \param argc : number of arguments given at command line
       * \param argv : arguments list
       */
      ArgumentParserBase(const int, const char* const* const);
      /*!
       * \brief register a new callback
       * \param key         : command line argument name
       * \param f           : callback
       * \param description : description of the command line argument
       *                      (used for the --help) options
       * \param b           : This command line argument can have an option
       */
      void registerNewCallBack(const std::string&,
                               const MemberFuncPtr&,
                               const std::string& = "",
                               const bool = false);

      /*!
       * \brief register a new callback
       * \param key         : command line argument name
       * \param aliasName   : command line argument alias
       * \param f           : callback
       * \param description : description of the command line argument
       *                      (used for the --help) options
       * \param b           : This command line argument can have an option
       */
      void registerNewCallBack(const std::string&,
                               const std::string&,
                               const MemberFuncPtr&,
                               const std::string& = "",
                               const bool = false);
      //! destructor
      virtual ~ArgumentParserBase();

     private:
      //! copy constructor
      ArgumentParserBase(const ArgumentParserBase&) = delete;
      //! move constructor
      ArgumentParserBase(ArgumentParserBase&&) = delete;
      //! assignement
      ArgumentParserBase& operator=(const ArgumentParserBase&) = delete;
      //! move assignement
      ArgumentParserBase& operator=(ArgumentParserBase&&) = delete;

    };  // end of ArgumentParserBase<Child>

  }  // end of namespace utilities

}  // end of namespace tfel

#include "TFEL/Utilities/ArgumentParserBase.ixx"

#endif /* LIB_TFEL_UTILITIES_ARGUMENTPARSERBASE_HXX */
