/*!
 * \file   ArgumentParserBase.hxx
 * \brief  This file declares the ArgumentParserBase class
 * \author Helfer Thomas
 * \date   17 Nov 2006
 */

#ifndef _LIB_TFEL_ARGUMENTPARSERBASE_H_
#define _LIB_TFEL_ARGUMENTPARSERBASE_H_ 

#include<map>
#include<vector>
#include<string>

namespace tfel
{

  namespace utilities
  {

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

      /*!
       * Class holding a command line argument
       *
       * This class is used internally.
       *
       * \note inheriting from std::string shall be forbidden in most
       *cases. Here its does not harm.
       */
      struct Argument
	: public std::string
      {
	/*!
	 * Default constructor
	 *
	 * \param s : argument name
	 */
	Argument(const std::string& s)
	  : std::string(s)
	{}
	/*!
	 * Default constructor
	 *
	 * \param s : argument name
	 */
	Argument(const char * const s)
	  : std::string(s),
	    isOptionSet(false)
	{}
	bool hasOption(void) const
	{
	  return this->isOptionSet;
	}
	/*!
	 * set argument option
	 *
	 * \param o : option
	 */
	void
	setOption(const std::string& o)
	{
	  this->isOptionSet = true;
	  this->option = o;
	}
	/*!
	 * get argument option
	 */
	const std::string&
	getOption() const
	{
	  return option;
	}
      private:
	//! argument option
	std::string option;
	bool isOptionSet;
      }; // end of struct Argument
    
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

      /*!
       * \brief constructor
       * \param argc : number of arguments given at command line
       * \param argv : arguments list
       */
      ArgumentParserBase(const int,const char* const* const);

      /*!
       * Destructor
       */
      virtual ~ArgumentParserBase();

    private:

      /*!
       * \brief default constructor (disabled)
       */
      ArgumentParserBase();

      /*!
       * \brief copy constructor (disabled)
       */
      ArgumentParserBase(const ArgumentParserBase&);

      /*!
       * \brief assignement operator (disabled)
       */
      ArgumentParserBase& operator=(const ArgumentParserBase&);

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

#endif /* _LIB_TFEL_ARGUMENTPARSERBASE_H */

