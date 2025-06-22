/*!
 * \file   ArgumentParser.hxx
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

#ifndef LIB_TFEL_UTILITIES_ARGUMENTPARSER_HXX
#define LIB_TFEL_UTILITIES_ARGUMENTPARSER_HXX

#include <map>
#include <vector>
#include <functional>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Utilities/Argument.hxx"

namespace tfel {

  namespace utilities {

    //! \brief class used to parser command line arguments
    struct TFELUTILITIES_VISIBILITY_EXPORT ArgumentParser {
      /*!
       * \brief structure describing an action to be performed when a
       * argument is treated
       */
      struct TFELUTILITIES_VISIBILITY_EXPORT CallBack {
        //! default constructor (deleted)
        CallBack() = delete;
        /*!
         * \brief constructor
         * \param[in] d_: description
         * \param[in] c_: action
         * \param[in] b : true if the callback requires an option
         */
        CallBack(const std::string&, const std::function<void()>&, const bool);
        //! \brief move constructor
        CallBack(CallBack&&);
        //! \brief copy constructor (deleted)
        CallBack(const CallBack&);
        //! \brief move assignement
        CallBack& operator=(CallBack&&) = delete;
        //! \brief assignement
        CallBack& operator=(const CallBack&) = delete;
        //! \brief description
        const std::string d;
        //! \brief action performed
        std::function<void()> c;
        //! \brief flag, true if the callback has an option
        const bool hasOption = false;
      };
      //! \brief a simple alias
      using CallBacksContainer = std::map<std::string, CallBack>;
      //! \brief default constructor
      ArgumentParser();
      /*!
       * \brief constructor
       * \param argc : number of arguments given at command line
       * \param argv : arguments list
       */
      ArgumentParser(const int, const char* const* const);
      /*!
       * \brief set the arguments to be parsed
       * \param argc : number of arguments given at command line
       * \param argv : arguments list
       */
      virtual void setArguments(const int, const char* const* const);
      /*!
       * \brief register a new callback
       * \param key         : command line argument name
       * \param f           : callback
       */
      virtual void registerCallBack(const std::string&, const CallBack&);
      /*!
       * \brief register a new callback
       * \param key         : command line argument name
       * \param aliasName   : command line argument alias
       * \param f           : callback
       */
      virtual void registerCallBack(const std::string&,
                                    const std::string&,
                                    const CallBack&);
      //! \brief parse arguments using registred methods.
      virtual void parseArguments();
      //! \return the list of registred callbacks
      const CallBacksContainer& getRegistredCallBacks() const;
      //! destructor
      virtual ~ArgumentParser();

     protected:
      //! \brief a simple alias
      using AliasContainer = std::map<std::string, std::string>;
      //! \brief a simple alias
      using ArgsContainer = std::vector<Argument>;
      /*!
       * \brief register a default callbacks
       *
       * The default callbacks are :
       * - '--help', '-h' which displays the list of the command line
       *    arguments (if the treatHelp method is not overriden if the
       *    derivate class)
       * - '--version', '-v' which displays the current code version
       *    using the getVersionDescription() method of the derivate
       *    class (if the 'treatVersion' method is not overriden if the
       *    derivate class)
       */
      virtual void registerDefaultCallBacks();
      /*!
       * \brief method called while parsing unregistred command line
       * arguments.
       *
       * \throw runtime_error
       */
      virtual void treatUnknownArgument();
      /*!
       * \brief method associated with the '--help' command line
       * argument.
       *
       * This method uses the getUsageDescription() method to get a
       * more helpfull message.
       *
       * This method stops the execution by calling the exit
       * method.
       */
      virtual void treatHelp();
      /*!
       * \brief method associated with the '--version' command line
       * argument
       */
      virtual void treatVersion();
      //! \brief container of all the call-backs
      CallBacksContainer callBacksContainer;
      //! \brief container of all the alias
      AliasContainer alias;
      //! \brief container of all the command line arguments
      ArgsContainer args;
      //! \brief an iterator to the argument being treated
      ArgsContainer::iterator currentArgument;
      //! \brief program name
      std::string programName;

     private:
      //! \brief copy constructor
      ArgumentParser(const ArgumentParser&) = delete;
      //! \brief move constructor
      ArgumentParser(ArgumentParser&&) = delete;
      //! \brief standard assignement
      ArgumentParser& operator=(const ArgumentParser&) = delete;
      //! \brief move assignement
      ArgumentParser& operator=(ArgumentParser&&) = delete;
      //! \brief replaces aliases by their usual names
      virtual void replaceAliases();
      //! \brief slip arguments and options
      virtual void stripArguments();

     protected:
      //! \return the version of the program being used
      virtual std::string getVersionDescription() const = 0;
      //! \return the usage of the program being used
      virtual std::string getUsageDescription() const = 0;
    };

  }  // namespace utilities

}  // namespace tfel

#endif /* LIB_TFEL_UTILITIES_ARGUMENTPARSER_HXX */
