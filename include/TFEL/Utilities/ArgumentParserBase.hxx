/*!
 * \file   include/TFEL/Utilities/ArgumentParserBase.hxx
 * \brief  This file declares the ArgumentParserBase class
 * \author Helfer Thomas
 * \date   17 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_ARGUMENTPARSERBASE_H_
#define LIB_TFEL_ARGUMENTPARSERBASE_H_ 

#include<map>
#include<vector>
#include<string>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{

  namespace utilities
  {

    /*!
     * Class holding a command line argument
     *
     * This class is used internally.
     * \note inheriting from std::string shall be forbidden in most
     *cases. Here its does not harm.
     */
    struct TFELUTILITIES_VISIBILITY_EXPORT Argument
      : public std::string
    {
      //! \param s : argument name
      Argument(std::string s);
      //! \param s : argument name
      Argument(const char* const s);
      Argument(Argument&&) = default;
      Argument(const Argument&) = default;
      Argument& operator=(Argument&&) = default;
      Argument& operator=(const Argument&) = default;
      //! \return true if an option was given for this argument
      bool hasOption(void) const noexcept;
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
      //! argument option
      std::string option;
      bool isOptionSet;
    }; // end of struct Argument

    /*!
     * \brief Class used for command line argument parsing.
     *
     * The CRTP pattern is used to enable ArgumentParserBase to used
     * the method of its derivate class.
     * 
     * The class maps the command line arguments to method of the
     * derivate class.
     *
     * \param Child
     */
    template<typename Child>
    struct ArgumentParserBase
    {

    protected:
    
      //! a simple alias
      typedef void (Child::* MemberFuncPtr)(void);
      //! a simple alias
      typedef std::map<std::string,
		       std::pair<MemberFuncPtr,
				 std::pair<bool,std::string> > > CallBacksContainer;
      //! a simple alias
      typedef std::map<std::string,std::string> AliasContainer;
      //! a simple alias
      typedef std::vector<Argument> ArgsContainer;

      //! container of all the call-backs
      CallBacksContainer callBacksContainer;
      //! container of all the alias
      AliasContainer alias;
      //! container of all the command line arguments
      ArgsContainer  args;
      //! an iterator to the argument being treated
      typename ArgsContainer::iterator currentArgument;
      //! program name
      std::string programName;
      
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

      /*!
       * \brief register a default callbacks
       *
       * The default callbacks are :
       * - '--help', '-h' which displays the list of the command line
           arguments (if the treatHelp method is not overriden if the
           derivate class)
       * - '--version', '-v' which displays the current code version
           using the getVersionDescription() method of the derivate
           class (if the 'treatVersion' method is not overriden if the
           derivate class)
       */
      void registerDefaultCallBacks(void);

      /*!
       * \brief method called while parsing unregistred command line
       * arguments.
       *
       * \throw runtime_error
       */
      void treatUnknownArgument(void);

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
      void treatHelp(void);
      /*!
       * \brief method associated with the '--version' command line
       * argument
       */
      void treatVersion(void);

      /*!
       * \brief parse arguments using registred methods.
       */
      void parseArguments(void);
      //! default constructor
      ArgumentParserBase();
      /*!
       * \brief constructor
       * \param argc : number of arguments given at command line
       * \param argv : arguments list
       */
      ArgumentParserBase(const int,const char* const* const);
      /*!
       * \brief set the arguments to be parsed
       * \param argc : number of arguments given at command line
       * \param argv : arguments list
       */
      void setArguments(const int,const char* const* const);
      /*!
       * Destructor
       */
      virtual ~ArgumentParserBase();

    private:

      ArgumentParserBase(const ArgumentParserBase&) = delete;
      ArgumentParserBase(ArgumentParserBase&&) = delete;
      ArgumentParserBase& operator=(const ArgumentParserBase&) = delete;
      ArgumentParserBase& operator=(ArgumentParserBase&&) = delete;
      /*!
       * \brief replaces aliases by their usual names
       */
      void replaceAliases(void);
      /*!
       * \brief slip arguments and options
       */
      void stripArguments(void);

    protected:
    
      /*!
       * \return the version of the program being used
       */
      virtual std::string getVersionDescription(void) const = 0;

      /*!
       * \return the usage of the program being used
       */
      virtual std::string getUsageDescription(void) const = 0;

    }; // end of ArgumentParserBase<Child>

  } // end of namespace utilities

} // end of namespace tfel

#include"ArgumentParserBase.ixx"

#endif /* LIB_TFEL_ARGUMENTPARSERBASE_H_ */

