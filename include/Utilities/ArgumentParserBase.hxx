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

    template<typename Child>
    struct ArgumentParserBase
    {

      struct Argument
	: public std::string
      {
	Argument(const std::string& s)
	  : std::string(s)
	{}
	Argument(const char * const s)
	  : std::string(s)
	{}
	void
	setOption(const std::string& o)
	{
	  this->option = o;
	}
	const std::string&
	getOption() const
	{
	  return option;
	}
      private:
	std::string option;
      };
    
      typedef void (Child::* MemberFuncPtr)(void);
      typedef std::map<std::string,
		       std::pair<MemberFuncPtr,
				 std::pair<bool,std::string> > > CallBacksContainer;
      typedef std::map<std::string,std::string> AliasContainer;
      typedef std::vector<Argument> ArgsContainer;

      CallBacksContainer callBacksContainer;
      AliasContainer alias;
      ArgsContainer  args;
      typename ArgsContainer::iterator currentArgument;

      std::string programName;

      virtual ~ArgumentParserBase();

      void registerNewCallBack(const std::string&,
			       const MemberFuncPtr&, 
			       const std::string& = "",
			       const bool = false);

      void registerNewCallBack(const std::string&,
			       const std::string&,
			       const MemberFuncPtr&,
			       const std::string& = "",
			       const bool = false);

      void registerDefaultCallBacks(void);

      void treatUnknownArgument(void);

      void treatHelp(void);

      void treatVersion(void);

      void parseArguments(void);

      ArgumentParserBase(const int,const char* const* const);

    private:

      ArgumentParserBase();
      ArgumentParserBase(const ArgumentParserBase&);
      ArgumentParserBase& operator=(const ArgumentParserBase&);

      void replaceAliases(void);

      void stripArguments(void);
    
      virtual std::string getVersionDescription(void) const = 0;

      virtual std::string getUsageDescription(void) const = 0;

    }; // end of ArgumentParserBase<Child>

  } // end of namespace utilities

} // end of namespace tfel

#include"ArgumentParserBase.ixx"

#endif /* _LIB_TFEL_ARGUMENTPARSERBASE_H */

