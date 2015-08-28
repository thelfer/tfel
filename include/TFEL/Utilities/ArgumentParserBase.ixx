/*!
 * \file   include/TFEL/Utilities/ArgumentParserBase.ixx
 * \brief  This file implements methods of class ArgumentParserBase.
 * \author Helfer Thomas
 * \date   17 Nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_ARGUMENTPARSERBASE_I_
#define LIB_TFEL_ARGUMENTPARSERBASE_I_ 

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
	cout << tmp << " : " <<  p->second.second.second << endl;
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
    ArgumentParserBase<Child>::ArgumentParserBase()
    {
      this->registerDefaultCallBacks();
    } // end of ArgumentParserBase<Child>::ArgumentParserBase

    template<typename Child>
    ArgumentParserBase<Child>::ArgumentParserBase(const int argc,
						  const char * const * const argv)
      : ArgumentParserBase()
    {
      this->setArguments(argc,argv);;
    }

    template<typename Child>
    void
    ArgumentParserBase<Child>::setArguments(const int argc,
					    const char * const * const argv)
    {
      if(argc<1){
	throw(std::runtime_error("argc value not valid"));
      }
      this->programName = argv[0];
      this->args.clear();
      copy(argv+1,argv+argc,back_inserter(this->args));
    } // end of ArgumentParserBase<Child>::setArguments

    template<typename Child>
    void ArgumentParserBase<Child>::replaceAliases(void)
    {
      for(auto p=args.begin();p!=args.end();){
	const auto& pn = p->as_string();
	const auto pa=alias.find(pn);
	if(pa!=alias.end()){
	  const auto pf = this->callBacksContainer.find(pa->second);
	  if(pf==this->callBacksContainer.end()){
	    throw(std::runtime_error("ArgumentParserBase<Child>::replaceAliases: '"
				     +pn+"' is not a known argument"));
	  }
	  if(pf->second.second.first){
	    const auto p2 = p+1;
	    if(p2==args.end()){
	      throw(std::runtime_error("ArgumentParserBase<Child>::replaceAliases : '"
				       "no argument given to option '"+pn+"'"));
	    }
	    const auto& p2n = p2->as_string();
	    if(p2n[0]=='-'){
	      throw(std::runtime_error("ArgumentParserBase<Child>::replaceAliases : '"
				       "no argument given to option '"+pn+"'"));
	    }
	    *p = pa->second;
	    p->setOption(p2n);
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
      for(auto& a : this->args){
	auto& an = a.as_string();
	const auto pos = an.find("=");
	if(pos!=std::string::npos){
	  if(pos!=std::string::npos){
	    const auto option = an.substr(pos+1,std::string::npos);
	    an.erase(pos,std::string::npos);
	    const auto pf = this->callBacksContainer.find(an);
	    if(pf!=this->callBacksContainer.end()){
	      if(!(pf->second.second.first)){
		throw(std::runtime_error("ArgumentParserBase<Child>::stripArguments: "
					 "argument '"+an+"' does not have any option"));
	      }
	    }
	    a.setOption(option);
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
      msg += this->currentArgument->as_string();
      msg += " is not a valid argument";
      throw(runtime_error(msg));
    } // end of ArgumentParserBase<Child>::treatUnknownArgument
  
    template<typename Child>
    void ArgumentParserBase<Child>::parseArguments(void)
    {
      auto comp = [](const Argument& a,
		     const char* const s){
	return a.as_string()==s;
      };
      this->stripArguments();
      this->replaceAliases();
      for(auto pa = args.begin();pa!=args.end();){
	if(comp(*pa,"--help")){
	  static_cast<Child *>(this)->treatHelp();
	  pa = this->args.erase(pa);
	} else {
	  ++pa;
	}
      }
      for(auto pa = args.begin();pa!=args.end();){
	if(comp(*pa,"--version")){
	  static_cast<Child *>(this)->treatVersion();
	  pa = this->args.erase(pa);
	} else {
	  ++pa;
	}
      }
      this->currentArgument=this->args.begin();
      while(this->currentArgument!=this->args.end()){
	  auto a  = this->currentArgument->as_string();
	  auto pf = this->callBacksContainer.find(a);
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

#endif /* LIB_TFEL_ARGUMENTPARSERBASE_I_ */

