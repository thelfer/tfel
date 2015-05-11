/*! 
 * \file  BehaviourQuery.cxx
 * \brief
 * \author Helfer Thomas
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
#include<iostream>
#endif
#include<sstream>
#include<algorithm>

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/Glossary/Glossary.hxx"
#include"TFEL/Glossary/GlossaryEntry.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/AbstractBehaviourDSL.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/BehaviourDescription.hxx"
#include"MFront/BehaviourQuery.hxx"

namespace mfront{

  static const BehaviourAttribute&
  getAttribute(const std::string& n,
	       const BehaviourDescription& d,
	       const tfel::material::ModellingHypothesis::Hypothesis h){
    const auto& as1 = d.getAttributes();
    const auto& as2 = d.getBehaviourData(h).getAttributes();
    auto p = as2.find(n);
    if(p!=as2.end()){
      return p->second;
    }
    p = as1.find(n);
    if(p!=as2.end()){
      return p->second;
    }
    return p->second;
  }
  
  BehaviourQuery::BehaviourQuery(const int argc,
				 const char *const *const argv,
				 std::shared_ptr<AbstractBehaviourDSL> d,
				 const std::string& f)
    : tfel::utilities::ArgumentParserBase<BehaviourQuery>(argc,argv),
      dsl(d),
      file(f)
  {
    this->registerCommandLineCallBacks();
    this->parseArguments();
    // registring interfaces
    if(!this->interfaces.empty()){
      dsl->setInterfaces(this->interfaces);
    }
  } // end of BehaviourQuery::BehaviourQuery

  void
  BehaviourQuery::registerCommandLineCallBacks(void){
    using namespace std;
    using Parser = tfel::utilities::ArgumentParserBase<BehaviourQuery>;
    Parser::registerNewCallBack("--verbose",&BehaviourQuery::treatVerbose,
				"set verbose output",true);
    Parser::registerNewCallBack("--include","-I",&BehaviourQuery::treatSearchPath,
				"add a new path at the beginning of the search paths",true);
    Parser::registerNewCallBack("--search-path",&BehaviourQuery::treatSearchPath,
				"add a new path at the beginning of the search paths",true);
    Parser::registerNewCallBack("--debug",&BehaviourQuery::treatDebug,
				"set debug mode");
    Parser::registerNewCallBack("--warning","-W",&BehaviourQuery::treatWarning,"print warnings");
    Parser::registerNewCallBack("--pedantic",&BehaviourQuery::treatPedantic,
				"print pedantic warning message");
    Parser::registerNewCallBack("--interface",&BehaviourQuery::treatInterface,
				"define an interface",true);
    Parser::registerNewCallBack("--modelling-hypothesis",&BehaviourQuery::treatModellingHypothesis,
				"select a modelling hypothesis",true);
    // standard queries
    const vector<pair<const char*,const char*>> sq = {
      {"--author","show the author name"},
      {"--description","show the file description"},
      {"--date","show the file implementation date"},
      {"--material","show the material name"},
      {"--library","show the library name"},
      {"--supported-modelling-hypotheses","show the list of supported modelling hypothesis"},
      {"--material-properties","show the list of material properties for the selected modelling hypothesis"},
      {"--state-variables","show the list of state variables for the selected modelling hypothesis"},
      {"--auxiliary-state-variables","show the auxiliary state variables properties for the selected modelling hypothesis"},
      {"--external-state-variables","show the list of external state variables for the selected modelling hypothesis"},
      {"--integration-variables","show the list of integration variables for the selected modelling hypothesis"},
      {"--persistent-variables","show the list of persistent variables for the selected modelling hypothesis"},
      {"--local-variables","show the list of local variables for the selected modelling hypothesis"},
      {"--parameters","show the list of parameters for the selected modelling hypothesis"},
      {"--attributes","show the list of attributes of the behaviour description"},
      {"--code-blocks","show the list of code-blocks of the behaviour description for "
	  " the selected modelling hypothesis"}};
    for(const auto& q : sq){
      Parser::registerNewCallBack(q.first,&BehaviourQuery::treatStandardQuery,q.second);
    }
    const vector<pair<const char*,const char*>> sq2 = {
      {"--attribute-type","display an attribute type"},
      {"--attribute-value","display an attribute value"},
      {"--parameter-type","display a parameter type"},
      {"--parameter-default-value","display a parameter default value"}};
    for(const auto& q : sq2){
      Parser::registerNewCallBack(q.first,&BehaviourQuery::treatStandardQuery2,q.second,true);
    }
    Parser::registerNewCallBack("--generated-sources",&BehaviourQuery::treatGeneratedSources,
				"show all the generated sources");
    Parser::registerNewCallBack("--generated-headers",&BehaviourQuery::treatGeneratedHeaders,
				"show all the generated headers");
    Parser::registerNewCallBack("--cppflags",&BehaviourQuery::treatCppFlags,
				"show all the global headers");
    Parser::registerNewCallBack("--libraries-dependencies",&BehaviourQuery::treatLibrariesDependencies,
				"show all the libraries dependencies");
    Parser::registerNewCallBack("--specific-targets",&BehaviourQuery::treatSpecificTargets,
				"show all the specific targets");
  } // end of BehaviourQuery::registerCommandLineCallBacks

  void
  BehaviourQuery::treatStandardQuery(void){
    using namespace std;
    using namespace tfel::utilities;
    using tfel::material::ModellingHypothesis;
    const auto& q = this->getCurrentCommandLineArgument();
    if(q=="--author"){
      this->queries.push_back({"author",[](const FileDescription& fd,
					   const BehaviourDescription&,
					   const Hypothesis){
	    const auto& a = fd.authorName;
	    cout << (!a.empty() ? a : "(undefined)") << endl;
	  }});
    } else if(q=="--description"){
      this->queries.push_back({"description",[](const FileDescription& fd,
						const BehaviourDescription&,
						const Hypothesis){
	    if(!fd.description.empty()){
	      const auto d = tokenize(fd.description,'\n');
	      for(const auto& l : d){
		if((l.size()>=2)&&((l)[0]=='*')&&((l)[1]==' ')){
		  cout << l.substr(2) << endl;
		} else {
		  cout << l << endl;
		}
	      }
	    } else {
	      cout << "(undefined)" << endl;
	    }
	  }});
    } else if(q=="--date"){
      this->queries.push_back({"date",[](const FileDescription& fd,
					 const BehaviourDescription&,
					 const Hypothesis){
	    const auto& d = fd.date;
	    cout << (!d.empty() ? d : "(undefined)") << endl;
	  }});
    } else if(q=="--material"){
      this->queries.push_back({"material",[](const FileDescription&,
					     const BehaviourDescription& d,
const Hypothesis){
	    const auto& m = d.getMaterialName();
	    cout << (!m.empty() ? m : "(undefined)") << endl;
	  }});
    } else if(q=="--library"){
      this->queries.push_back({"library",[](const FileDescription&,
					    const BehaviourDescription& d,
					    const Hypothesis){
	    const auto& l = d.getLibrary();
	    cout << (!l.empty() ? l : "(undefined)") << endl;
	  }});
    } else if(q=="--supported-modelling-hypotheses"){
      this->queries.push_back({"supported-modelling-hypotheses",[](const FileDescription&,
								   const BehaviourDescription& d,
								   const Hypothesis){
	    const auto& mh = d.getModellingHypotheses();
	    for(const auto& h : mh){
	      cout << ModellingHypothesis::toString(h) << " ";
	    }
	    cout << endl;
	  }});
    } else if (q=="--material-properties"){
      this->queries.emplace_back("material-properties",
				 this->generateVariablesListQuery<&BehaviourData::getMaterialProperties>());
    } else if (q=="--state-variables"){
      this->queries.emplace_back("state-variables",
				 this->generateVariablesListQuery<&BehaviourData::getStateVariables>());
    } else if (q=="--auxiliary-state-variables"){
      this->queries.emplace_back("auxiliary-state-variables",
				 this->generateVariablesListQuery<&BehaviourData::getAuxiliaryStateVariables>());
    } else if (q=="--external-state-variables"){
      this->queries.emplace_back("external-state-variables",
				 this->generateVariablesListQuery<&BehaviourData::getExternalStateVariables>());
    } else if (q=="--integration-variables"){
      this->queries.emplace_back("integration-state-variables",
				 this->generateVariablesListQuery<&BehaviourData::getIntegrationVariables>());
    } else if (q=="--persistent-variables"){
      this->queries.emplace_back("persistent-state-variables",
				 this->generateVariablesListQuery<&BehaviourData::getPersistentVariables>());
    } else if (q=="--local-variables"){
      this->queries.emplace_back("local-state-variables",
				 this->generateVariablesListQuery<&BehaviourData::getLocalVariables>());
    } else if (q=="--parameters"){
      this->queries.emplace_back("parameters",
				 this->generateVariablesListQuery<&BehaviourData::getParameters>());
    } else if (q=="--attributes"){
      this->queries.push_back({"attributes",[](const FileDescription&,
					       const BehaviourDescription& d,
					       const Hypothesis h){
	    const auto& as1 = d.getAttributes();
	    if(!as1.empty()){
	      for(const auto& a:as1){
		cout << a.first << " ";
	      }
	    }
	    const auto& as2 = d.getBehaviourData(h).getAttributes();
	    for(const auto& a:as2){
	      cout << a.first << " ";
	    }
	    cout << endl;
	  }});
    } else if (q=="--code-blocks"){
      this->queries.push_back({"attributes",[](const FileDescription&,
					       const BehaviourDescription& d,
					       const Hypothesis h){
	    const auto& names = d.getBehaviourData(h).getCodeBlockNames();
	    for(const auto& n:names){
	      cout << "- "  << n << endl;
	    }
	  }});
    } else {
      throw(runtime_error("Behaviour::treatStandardQuery : "
			  "unsupported query '"+q+"'"));
    }
  }

  void
  BehaviourQuery::treatStandardQuery2(void){
    using namespace std;
    using namespace tfel::utilities;
    using tfel::material::ModellingHypothesis;
    const auto& q = this->getCurrentCommandLineArgument();
    const auto& o = q.getOption();
    if(o.empty()){
      throw(runtime_error("Behaviour::treatStandardQuery2 : "
			  "no option given to the '"+q+"' query"));
    } else if (q=="--attribute-type"){
      auto l = [o](const FileDescription&,
		   const BehaviourDescription& d,
		   const Hypothesis h){
	const auto& a = getAttribute(o,d,h);
	if(a.is<bool>()){
	  cout << "bool" << endl;
	} else if(a.is<unsigned short>()){
	  cout << "unsigned short" << endl;
	} else if(a.is<string>()){
	  cout << "string" << endl;
	} else {
	  throw(runtime_error("Behaviour::treatStandardQuery2 : "
			      "unsupported attribute type"));
	}
      };
      this->queries.push_back({"attribute-type",l});
    } else if (q=="--attribute-value"){
      auto l = [o](const FileDescription&,
		   const BehaviourDescription& d,
		   const Hypothesis h){
	const auto& a = getAttribute(o,d,h);
	if(a.is<bool>()){
	  if(a.get<bool>()){
	    cout << "true" << endl;
	  } else {
	    cout << "false" << endl;
	  }
	} else if(a.is<unsigned short>()){
	  cout << a.get<unsigned short>() << endl;
	} else if(a.is<string>()){
	  cout << a.get<string>() << endl;
	} else {
	  throw(runtime_error("Behaviour::treatStandardQuery2 : "
			      "unsupported attribute type"));
	}
      };
      this->queries.push_back({"attributes-value",l});
    } else if (q=="--parameter-type"){
      auto l = [o](const FileDescription&,
		   const BehaviourDescription& d,
		   const Hypothesis h){
	const auto& bd = d.getBehaviourData(h);
	const auto& p  = bd.getVariableHandler(bd.getParameters(),o);
	cout << p.type << endl;
      };
      this->queries.push_back({"parameter-type",l});
    } else if (q=="--parameter-default-value"){
      auto l = [o](const FileDescription&,
		   const BehaviourDescription& d,
		   const Hypothesis h){
	const auto& bd = d.getBehaviourData(h);
	const auto& p  = bd.getVariableHandler(bd.getParameters(),o);
	if(p.type=="real"){
	  cout << bd.getFloattingPointParameterDefaultValue(o) << endl;
	} else if(p.type=="int"){
	  cout << bd.getIntegerParameterDefaultValue(o) << endl;
	} else if(p.type=="ushort"){
	  cout << bd.getUnsignedShortParameterDefaultValue(o) << endl;
	} else {
	  throw(runtime_error("Behaviour::treatStandardQuery2 : "
			      "unsupported parameter type"));
	}
      };
      this->queries.push_back({"parameter-default-value",l});
    } else {
      throw(runtime_error("Behaviour::treatStandardQuery : "
			  "unsupported query '"+q+"'"));
    }
  }

  template<const VariableDescriptionContainer& (BehaviourData::* m)(void) const>
  typename BehaviourQuery::query
  BehaviourQuery::generateVariablesListQuery(void){
    return [](const FileDescription&,
	      const BehaviourDescription& bd,
	      const Hypothesis h){
      using namespace std;
      using namespace tfel::glossary;
      const auto& d        = bd.getBehaviourData(h);
      const auto& vars     = (d.*m)();
      for(const auto& v:vars){
	const auto& n = d.getExternalName(v.name);
	cout << "- " << n << " (" << v.name << ")";
	if(!v.description.empty()){
	  cout << " :  " << v.description;
	} else {
	  const auto& glossary = Glossary::getGlossary();
	  if(glossary.contains(n)){
	    cout << " :  " << glossary.getGlossaryEntry(n).getShortDescription();
	  }
	}
	cout << endl;
      }
    };
  }

  void
  BehaviourQuery::treatGeneratedSources(void)
  {
    using namespace std;
    auto ldsl = this->dsl;
    auto l = [ldsl](const FileDescription&,
		    const BehaviourDescription&,
		    const Hypothesis){
      const auto& sources = ldsl->getTargetsDescription().sources;
      for(const auto& src : sources){
	cout << src.first << " : "; //< library
	copy(src.second.begin(),src.second.end(),
	     ostream_iterator<string>(cout," "));
	cout << endl;
      }
    };
    this->queries.push_back({"generated-sources",l});
  } // end of BehaviourQuery::treatGeneratedSources

  void
  BehaviourQuery::treatGeneratedHeaders(void)
  {
    using namespace std;
    auto ldsl = this->dsl;
    auto l = [ldsl](const FileDescription&,
		    const BehaviourDescription&,
		    const Hypothesis){
      const auto headers = ldsl->getTargetsDescription().headers;
      copy(headers.begin(),headers.end(),
	   ostream_iterator<string>(cout," "));
      cout << endl;
    };
    this->queries.push_back({"generated-headers",l});
  } // end of BehaviourQuery::treatGeneratedHeaders

  void
  BehaviourQuery::treatCppFlags(void)
  {
    using namespace std;
    auto ldsl = this->dsl;
    auto l = [ldsl](const FileDescription&,
		    const BehaviourDescription&,
		    const Hypothesis){
      const auto flags = ldsl->getTargetsDescription().cppflags;
      for(const auto& f : flags){
	cout << f.first << " : "; //< library
	copy(f.second.begin(),f.second.end(),
	     ostream_iterator<string>(cout," "));
	cout << endl;
      }
    };
    this->queries.push_back({"cppflags",l});
  } // end of BehaviourQuery::treatCppFlags

  void
  BehaviourQuery::treatLibrariesDependencies(void)
  {
    using namespace std;
    auto ldsl = this->dsl;
    auto l = [ldsl](const FileDescription&,
		    const BehaviourDescription&,
		    const Hypothesis){
      const auto deps = ldsl->getTargetsDescription().dependencies;
      for(const auto& d : deps){
	cout << d.first << " : "; //< library
	copy(d.second.begin(),d.second.end(),
	     ostream_iterator<string>(cout," "));
	cout << endl;
      }
    };
    this->queries.push_back({"libraries-dependencies",l});
  } // end of BehaviourQuery::treatLibrariesDependencies

  void
  BehaviourQuery::treatSpecificTargets(void)
  {
    using namespace std;
    auto ldsl = this->dsl;
    auto l = [ldsl](const FileDescription&,
		    const BehaviourDescription&,
		    const Hypothesis){
      const auto targets = ldsl->getTargetsDescription().specific_targets;
      for(const auto t : targets){
	cout << t.first << " : ";
	copy(t.second.first.begin(),t.second.first.end(),
	     ostream_iterator<string>(cout," "));
	cout << endl << "> rule : ";
	copy(t.second.second.begin(),t.second.second.end(),
	     ostream_iterator<string>(cout,"\n> rule : "));
	cout << endl;
      }
    };
    this->queries.push_back({"specific-targets",l});
  } // end of BehaviourQuery::treatSpecificTargets

  void
  BehaviourQuery::treatModellingHypothesis(void)
  {
    using namespace std;
    const auto& o = this->getCurrentCommandLineArgument().getOption();
    if(o.empty()){
      throw(runtime_error("BehaviourQuery::treatModellingHypothesis : "
			  "no option given to --modelling-hypothesis"
			  "command line option"));
    }
    if(this->hypothesis!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      throw(runtime_error("BehaviourQuery::treatModellingHypothesis : "
			  "modelling hypothesis already defined"));
    }
    this->hypothesis = ModellingHypothesis::fromString(o);
  } // end of BehaviourQuery::treatModellingHypothesis

  void
  BehaviourQuery::exe(void)
  {
    using namespace std;
    if(getVerboseMode()>=VERBOSE_LEVEL2){
      getLogStream() << "Treating file '" << this->file << "'" << endl;
    }
    // analysing the file
    this->dsl->analyseFile(this->file,this->ecmds);
    const auto& fd = this->dsl->getFileDescription();
    const auto& d  = this->dsl->getBehaviourDescription();
    // checks
    if(this->hypothesis==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(d.areAllMechanicalDataSpecialised()){
	ostringstream msg;
	const auto& mh = d.getModellingHypotheses();
	if(mh.size()==1u){
	  this->hypothesis = *(mh.begin());
	} else {
	  msg << "BehaviourQuery::exe : all mechanical data are specialised." << endl
	      << "A modelling hypothesis must be specified using "
	      << "the '--modelling-hypothesis' command line option." << endl
	      << "The supported modelling hypothesis are : " << endl;
	  for(const auto h : mh){
	    msg << "- " << ModellingHypothesis::toString(h) << endl;
	  }
	  throw(runtime_error(msg.str()));
	}
      }
    } else  {
      const auto& mh = d.getDistinctModellingHypotheses();
      if(mh.find(this->hypothesis)==mh.end()){
	string msg("BehaviourQuery::exe : the specified modelling hypothesis ('"+
		   ModellingHypothesis::toString(this->hypothesis)+"') is not supported by the behaviour");
	throw(runtime_error(msg));
      }
    }
    // treating the queries
    for(const auto& q : queries){
      if(getVerboseMode()>=VERBOSE_LEVEL2){
	getLogStream() << "Treating query '" << q.first << "'" << endl;
      }
      q.second(fd,d,this->hypothesis);
    }
  } // end of BehaviourQuery::exe

  const tfel::utilities::Argument&
  BehaviourQuery::getCurrentCommandLineArgument() const{
    return *(this->currentArgument);
  }
  
  void BehaviourQuery::treatUnknownArgument(void)
  {
    if(!MFrontBase::treatUnknownArgumentBase()){
#if not (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
      ArgumentParserBase<BehaviourQuery>::treatUnknownArgument();
#else
      cerr << "mfront : unsupported option '" << a << "'\n";
      exit(EXIT_FAILURE);
#endif /* __CYGWIN__ */
    }
  }

  std::string
  BehaviourQuery::getVersionDescription(void) const{
    return MFrontHeader::getHeader();
  }

  std::string 
  BehaviourQuery::getUsageDescription(void) const
  {
    using namespace std;
    string usage("Usage : ");
    usage += this->programName;
    usage += " [options] [files]";
    return usage;
  }

  BehaviourQuery::~BehaviourQuery()
  {} // end of BehaviourQuery::~BehaviourQuery

} // end of namespace mfront
