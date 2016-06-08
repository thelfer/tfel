/*!
 * \file   ModelQuery.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   08 juin 2016
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"MFront/ModelDSL.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/ModelQuery.hxx"

namespace mfront{

  
  ModelQuery::ModelQuery(const int argc,
			 const char *const *const argv,
			 std::shared_ptr<ModelDSL> d,
			 const std::string& f)
    : tfel::utilities::ArgumentParserBase<ModelQuery>(argc,argv),
      dsl(d),
      file(f)
  {
    this->registerCommandLineCallBacks();
    this->parseArguments();
    // registring interfaces
    if(!this->interfaces.empty()){
      dsl->setInterfaces(this->interfaces);
    }
  } // end of ModelQuery::ModelQuery

  void
  ModelQuery::registerCommandLineCallBacks(void){
    using Parser = tfel::utilities::ArgumentParserBase<ModelQuery>;
    Parser::registerNewCallBack("--verbose",&ModelQuery::treatVerbose,
				"set verbose output",true);
    Parser::registerNewCallBack("--include","-I",&ModelQuery::treatSearchPath,
				"add a new path at the beginning of the search paths",true);
    Parser::registerNewCallBack("--search-path",&ModelQuery::treatSearchPath,
				"add a new path at the beginning of the search paths",true);
    Parser::registerNewCallBack("--debug",&ModelQuery::treatDebug,
				"set debug mode");
    Parser::registerNewCallBack("--warning","-W",&ModelQuery::treatWarning,"print warnings");
    Parser::registerNewCallBack("--pedantic",&ModelQuery::treatPedantic,
				"print pedantic warning message");
    Parser::registerNewCallBack("--interface",&ModelQuery::treatInterface,
				"define an interface",true);
    // // standard queries
    const std::vector<std::pair<const char*,const char*>> sq = {
      {"--author","show the author name"},
      {"--description","show the file description"},
      {"--date","show the file implementation date"},
      {"--material","show the material name"},
      {"--library","show the library name"}};
    for(const auto& q : sq){
      Parser::registerNewCallBack(q.first,&ModelQuery::treatStandardQuery,q.second);
    }
      // const vector<pair<const char*,const char*>> sq2 = {
    //   {"--attribute-type","display an attribute type"},
    //   {"--attribute-value","display an attribute value"},
    //   {"--parameter-type","display a parameter type"},
    //   {"--parameter-default-value","display a parameter default value"}};
    // for(const auto& q : sq2){
    //   Parser::registerNewCallBack(q.first,&ModelQuery::treatStandardQuery2,q.second,true);
    // }
    Parser::registerNewCallBack("--generated-sources",&ModelQuery::treatGeneratedSources,
				"show all the generated sources");
    Parser::registerNewCallBack("--generated-headers",&ModelQuery::treatGeneratedHeaders,
				"show all the generated headers");
    Parser::registerNewCallBack("--cppflags",&ModelQuery::treatCppFlags,
				"show all the global headers");
    Parser::registerNewCallBack("--libraries-dependencies",&ModelQuery::treatLibrariesDependencies,
				"show all the libraries dependencies");
    Parser::registerNewCallBack("--specific-targets",&ModelQuery::treatSpecificTargets,
				"show all the specific targets");
  } // end of ModelQuery::registerCommandLineCallBacks

  void ModelQuery::treatStandardQuery(void){
    using namespace std;
    const auto& q = this->getCurrentCommandLineArgument();
    const auto& qn = q.as_string();
    if(qn=="--author"){
      this->queries.push_back({"author",[](const FileDescription& fd,
					   const ModelDescription&){
	    const auto& a = fd.authorName;
	    cout << (!a.empty() ? a : "(undefined)") << endl;
	  }});
    } else if(qn=="--description"){
      this->queries.push_back({"description",[](const FileDescription& fd,
						const ModelDescription&){
	    if(!fd.description.empty()){
	      const auto d = tfel::utilities::tokenize(fd.description,'\n');
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
    } else if(qn=="--date"){
      this->queries.push_back({"date",[](const FileDescription& fd,
					 const ModelDescription&){
	    const auto& d = fd.date;
	    cout << (!d.empty() ? d : "(undefined)") << endl;
	  }});
    } else if(qn=="--material"){
      this->queries.push_back({"material",[](const FileDescription&,
					     const ModelDescription& d){
	    const auto& m = d.material;
	    cout << (!m.empty() ? m : "(undefined)") << endl;
	  }});
    } else if(qn=="--library"){
      this->queries.push_back({"library",[](const FileDescription&,
					    const ModelDescription& d){
	    const auto& l = d.library;
	    cout << (!l.empty() ? l : "(undefined)") << endl;
	  }});
    } else {
      throw(runtime_error("Model::treatStandardQuery : "
			  "unsupported query '"+qn+"'"));
    }
  } // end of ModelQuery::treatStandardQuery
  
  void ModelQuery::treatGeneratedSources(void)
  {
    auto ldsl = this->dsl;
    auto q = [ldsl](const FileDescription&,const ModelDescription&){
      for(const auto& l : ldsl->getTargetsDescription()){
	std::cout << l.name << " : "; //< library
	std::copy(std::begin(l.sources),std::end(l.sources),
		  std::ostream_iterator<std::string>(std::cout," "));
	std::cout << std::endl;
      }
    };
    this->queries.push_back({"generated-sources",q});
  } // end of ModelQuery::treatGeneratedSources

  void ModelQuery::treatGeneratedHeaders(void)
  {
    auto ldsl = this->dsl;
    auto q = [ldsl](const FileDescription&,const ModelDescription&){
      const auto headers = ldsl->getTargetsDescription().headers;
      std::copy(std::begin(headers),std::end(headers),
		std::ostream_iterator<std::string>(std::cout," "));
      std::cout << std::endl;
    };
    this->queries.push_back({"generated-headers",q});
  } // end of ModelQuery::treatGeneratedHeaders

  void ModelQuery::treatCppFlags(void)
  {
    auto ldsl = this->dsl;
    auto q = [ldsl](const FileDescription&,const ModelDescription&){
      for(const auto& l : ldsl->getTargetsDescription()){
	std::cout << l.name << " : ";
	std::copy(std::begin(l.cppflags),std::end(l.cppflags),
		  std::ostream_iterator<std::string>(std::cout," "));
	std::cout << std::endl;
      }
    };
    this->queries.push_back({"cppflags",q});
  } // end of ModelQuery::treatCppFlags

  void ModelQuery::treatLibrariesDependencies(void)
  {
    auto ldsl = this->dsl;
    auto q = [ldsl](const FileDescription&,const ModelDescription&){
      for(const auto& l : ldsl->getTargetsDescription()){
	std::cout << l.name << ": ";
	std::copy(std::begin(l.ldflags),std::end(l.ldflags),
		  std::ostream_iterator<std::string>(std::cout," "));
	std::cout << std::endl;
      }
    };
    this->queries.push_back({"libraries-dependencies",q});
  } // end of ModelQuery::treatLibrariesDependencies

  void ModelQuery::treatSpecificTargets(void)
  {
    auto ldsl = this->dsl;
    auto l = [ldsl](const FileDescription&,const ModelDescription&){
      const auto targets = ldsl->getTargetsDescription().specific_targets;
      for(const auto& t : targets){
	std::cout << t.first << " : ";
	std::copy(std::begin(t.second.first),std::end(t.second.first),
		  std::ostream_iterator<std::string>(std::cout," "));
	std::cout << std::endl << "> rule : ";
	std::copy(std::begin(t.second.second),std::end(t.second.second),
		  std::ostream_iterator<std::string>(std::cout,"\n> rule : "));
	std::cout << std::endl;
      }
    };
    this->queries.push_back({"specific-targets",l});
  } // end of ModelQuery::treatSpecificTargets
  
  void ModelQuery::exe(void)
  {
    if(getVerboseMode()>=VERBOSE_LEVEL2){
      getLogStream() << "Treating file '" << this->file << "'" << std::endl;
    }
    // analysing the file
    this->dsl->analyseFile(this->file,this->ecmds,this->substitutions);
    const auto& fd = this->dsl->getFileDescription();
    const auto& d  = this->dsl->getModelDescription();
    // treating the queries
    for(const auto& q : queries){
      if(getVerboseMode()>=VERBOSE_LEVEL2){
	getLogStream() << "Treating query '" << q.first << "'\n";
      }
      q.second(fd,d);
    }
  } // end of ModelQuery::exe

  const tfel::utilities::Argument&
  ModelQuery::getCurrentCommandLineArgument() const{
    return *(this->currentArgument);
  }
  
  void ModelQuery::treatUnknownArgument(void)
  {
    if(!MFrontBase::treatUnknownArgumentBase()){
#if ! (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
      ArgumentParserBase<ModelQuery>::treatUnknownArgument();
#else
      const auto& a = static_cast<const std::string&>(this->getCurrentCommandLineArgument());
      std::cerr << "mfront : unsupported option '" << a << "'\n";
      ::exit(EXIT_FAILURE);
#endif /* __CYGWIN__ */
    }
  }

  std::string
  ModelQuery::getVersionDescription(void) const{
    return MFrontHeader::getHeader();
  }

  std::string 
  ModelQuery::getUsageDescription(void) const
  {
    auto usage = std::string("Usage: ");
    usage += this->programName;
    usage += " [options] [files]";
    return usage;
  }

  ModelQuery::~ModelQuery()
  {} // end of ModelQuery::~ModelQuery
  
}
