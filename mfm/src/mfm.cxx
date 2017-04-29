/*!
 * \file   mfm.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   13 avril 2017
 */

#include<cstdlib>
#include<iostream>
#include<stdexcept>
#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/Utilities/ArgumentParserBase.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"

struct MFrontMaterialsMain
  : public tfel::utilities::ArgumentParserBase<MFrontMaterialsMain>
{

  MFrontMaterialsMain(const int argc,
		      const char *const *const argv)
    : tfel::utilities::ArgumentParserBase<MFrontMaterialsMain>(argc,argv)
  {
    this->registerCommandLineCallBacks();
    this->parseArguments();
    for(const auto& q: queries){
      q();
    }
  } // end of MFrontMaterialsMain

protected:				 

  using query = std::function<void()>;

  std::vector<query> queries;

  template<typename QueryType>
  void add_query(const std::string& o,
		 const std::string& d,
		 const QueryType& q)
  {
    auto add = [this,q](){
      this->queries.push_back(q);
    };
    this->registerCallBack(o,CallBack(d,add,false));
  } // end of addQuery
  
  void registerCommandLineCallBacks(){
    this->add_query("--list","generated the list of entry points",
		    [this](){
		      for(const auto& e : this->epts){
			std::cout << "- " << e.name << '\n';
		      }
		    });
  }

  const tfel::utilities::Argument& getCurrentCommandLineArgument() const{
    return *(this->currentArgument);
  }
  
  void treatUnknownArgument()
  {
    using namespace tfel::system;
    using tfel::utilities::ends_with;
    auto throw_if = [](const bool c,const std::string& m){
      if(c){throw(std::runtime_error("MFrontMaterialsMain::"
				     "treatUnknownArgument: "+m));}
    };
    auto& elm = ExternalLibraryManager::getExternalLibraryManager();
    const auto& a  = this->getCurrentCommandLineArgument();
    const auto& l = static_cast<const std::string&>(a);
    const auto  enames = elm.getEntryPoints(l);
    for(const auto& en : enames){
      EntryPoint ep;
      ep.library = l;
      ep.name    = en;
      const auto et = elm.getMaterialKnowledgeType(l,en);
      if(et==0u){
	ep.type = EntryPoint::MATERIALPROPERTY;
      } else if(et==1u){
	ep.type = EntryPoint::BEHAVIOUR;
      } else {
	throw_if(et!=2u,"internal error "
		 "(invalid material knowledge type)");
	ep.type = EntryPoint::BEHAVIOUR;
      }
      this->epts.push_back(std::move(ep));
    }
  } // end of treatUnknownArgument
  
  std::string getVersionDescription() const{
    return "1.0";
  }

  std::string getUsageDescription() const
  {
    return "Usage: "+this->programName+" [options] [files]";
  }

private:

  //! \brief an intermediate structure describing an entry point
  struct EntryPoint{
    //! type of material knowledge
    enum MaterialKnowledgeType{
      MATERIALPROPERTY,
      BEHAVIOUR,
      MODEL,
    }; // end of MaterialKnowledgeType
    std::string library;
    std::string name;
    MaterialKnowledgeType type;
  }; // end of EntryPoint
  
  std::vector<EntryPoint> epts;
  
};

int main(const int argc,const char* const* const argv){
  MFrontMaterialsMain m(argc,argv);  
  return EXIT_SUCCESS;
}
