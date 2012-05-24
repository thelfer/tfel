/*!
 * \file   ArgumentParserBase.ixx
 * \brief  This file implements methods of class ArgumentParserBase.
 * \author Helfer Thomas
 * \date   17 Nov 2006
 */

#ifndef _LIB_TFEL_ARGUMENTPARSERBASE_I_
#define _LIB_TFEL_ARGUMENTPARSERBASE_I_ 

#include<iostream>
#include<utility>
#include<algorithm>
#include<iterator>
#include<stdexcept>
#include<cstdlib>

namespace tfel
{

  namespace utilities
  {

    template<typename Child>
    void ArgumentParserBase<Child>::registerNewCallBack(const std::string& key,
							const MemberFuncPtr& f,
							const std::string& description,
							const bool b)
    {
      using namespace std;
      this->callBacksContainer.insert(make_pair(key,make_pair(f,make_pair(b,description))));
    }

    template<typename Child>
    void ArgumentParserBase<Child>::registerNewCallBack(const std::string& key,
							const std::string& aliasName,
							const MemberFuncPtr& f,
							const std::string& description,
							const bool b)
    {
      using namespace std;
      this->alias.insert(make_pair(aliasName,key));
      this->registerNewCallBack(key,f,description,b);
    }

    template<typename Child>
    void ArgumentParserBase<Child>::treatHelp(void)
    {
      using namespace std;
      typename CallBacksContainer::const_iterator p;
      typename AliasContainer::const_iterator pa;
      bool aliasFound;
      string aliasName;
      string tmp;
      cout << this->getUsageDescription() << endl << endl;
      cout << "Available options are : \n";
      aliasFound = false;
      for(p  = this->callBacksContainer.begin();
	  p != this->callBacksContainer.end(); ++p){ 
	for(pa=this->alias.begin();(pa!=this->alias.end())&&(!aliasFound);++pa){
	  if(pa->second==p->first){
	    aliasName = pa->first;
	    aliasFound = true;
	  }
	}
	if(aliasFound){
	  tmp = p->first + ", " + aliasName;
	} else {
	  tmp = p->first;
	}
	if(tmp.size()<=32){
	  tmp.insert(tmp.size(),32-tmp.size(),' ');
	}
	aliasFound=false;
      }
      std::exit(EXIT_SUCCESS);
    }

    template<typename Child>
    void ArgumentParserBase<Child>::treatVersion(void)
    {
      using namespace std;
      cout << this->getVersionDescription() << endl;
      std::exit(EXIT_SUCCESS);
    }

    template<typename Child>
    void
    ArgumentParserBase<Child>::registerDefaultCallBacks(void)
    {
      this->registerNewCallBack("--help","-h",&Child::treatHelp,"Display this message");
      this->registerNewCallBack("--version","-v",&Child::treatVersion,
				"Display version information");
    }

    template<typename Child>
    ArgumentParserBase<Child>::ArgumentParserBase(const int argc,
						  const char * const * const argv)
    {
      using namespace std;

      this->registerDefaultCallBacks();

      if(argc<1){
	throw(runtime_error("argc value not valid"));
      }

      this->programName = argv[0];
      copy(argv+1,argv+argc,back_inserter(args));
    }

    template<typename Child>
    void ArgumentParserBase<Child>::replaceAliases(void)
    {
      using namespace std;
      typename ArgsContainer::iterator p;
      typename AliasContainer::const_iterator pa;
      typename CallBacksContainer::iterator pf;
      for(p=args.begin();p!=args.end();){
	if((pa=alias.find(*p))!=alias.end()){
	  pf = this->callBacksContainer.find(pa->second);
	  if(pf==this->callBacksContainer.end()){
	    string msg("ArgumentParserBase<Child>::replaceAliases : '");
	    msg += *p;
	    msg += "' is not a known argument";
	    throw(runtime_error(msg));
	  }
	  if(pf->second.second.first){
	    typename ArgsContainer::iterator p2 = p+1;
	    if(p2==args.end()){
	      string msg("ArgumentParserBase<Child>::replaceAliases : '");
	      msg += "no argument given to option '"+*p+"'";
	      throw(runtime_error(msg));
	    }
	    if((*p2)[0]=='-'){
	      string msg("ArgumentParserBase<Child>::replaceAliases : '");
	      msg += "no argument given to option '"+*p+"'";
	      throw(runtime_error(msg));
	    }
	    *p = pa->second;
	    p->setOption(*p2);
	    p = this->args.erase(p2);
	  } else {
	    *p = pa->second;
	    ++p;
	  }
	} else {
	  ++p;
	}
      }
    }

    template<typename Child>
    void
    ArgumentParserBase<Child>::stripArguments(void)
    {
      using namespace std;
      typename ArgsContainer::iterator p;
      typename CallBacksContainer::iterator pf;
      typename string::size_type pos;
      for(p=this->args.begin();p!=this->args.end();++p){
	if((pos=p->find("="))!=string::npos){
	  if(pos!=std::string::npos){
	    string option(*p,pos+1,string::npos);
	    p->erase(pos,string::npos);
	    pf = this->callBacksContainer.find(*p);
	    if(pf==this->callBacksContainer.end()){
	      string msg("ArgumentParserBase<Child>::stripArguments : '");
	      msg += *p;
	      msg += "' is not a valid argument";
	      throw(runtime_error(msg));
	    }
	    if(!(pf->second.second.first)){
	      string msg("ArgumentParserBase<Child>::stripArguments : argument '");
	      msg += *p;
	      msg += "' does not have any option";
	      throw(runtime_error(msg));
	    }
	    p->setOption(option);
	  }
	}
      }
    } // end of ArgumentParserBase<Child>::stripArguments

    template<typename Child>
    void
    ArgumentParserBase<Child>::treatUnknownArgument(void)
    {
      using namespace std;
      string msg("ArgumentParserBase<Child>::treatUnknownArg : ");
      msg += *(this->currentArgument);
      msg += " is not a valid argument";
      throw(runtime_error(msg));
    } // end of ArgumentParserBase<Child>::treatUnknownArgument
  
    template<typename Child>
    void ArgumentParserBase<Child>::parseArguments(void)
    {
      this->stripArguments();
      this->replaceAliases();

      this->currentArgument=this->args.begin();
      typename CallBacksContainer::iterator pf;
      typename ArgsContainer::iterator pa;

      if((pa=find(args.begin(),args.end(),"--help"))!=args.end()){
	static_cast<Child *>(this)->treatHelp();
	this->args.erase(pa);
      }

      if((pa=find(args.begin(),args.end(),"--version"))!=args.end()){
	static_cast<Child *>(this)->treatVersion();
	this->args.erase(pa);
      }

      while(this->currentArgument!=this->args.end()){
	pf = this->callBacksContainer.find(*(this->currentArgument));
	if(pf!=this->callBacksContainer.end()){
	  (static_cast<Child *>(this)->*(pf->second.first))();
	} else {
	  static_cast<Child *>(this)->treatUnknownArgument();
	}
	++(this->currentArgument);
      }
    }

    template<typename Child>
    ArgumentParserBase<Child>::~ArgumentParserBase(void)
    {}

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_ARGUMENTPARSERBASE_I_ */

