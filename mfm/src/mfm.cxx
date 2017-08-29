/*!
 * \file   mfm.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   13 avril 2017
 */

#include<functional>
#include<algorithm>
#include<stdexcept>
#include<iostream>
#include<cstdlib>

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
    for(const auto& f: this->filters){
      auto p = std::remove_if(this->epts.begin(),this->epts.end(),
			      [f](const EntryPoint& e){
				return !f.f(e,f.o);
			      });
      this->epts.erase(p,this->epts.end());
    }
    for(const auto& e : this->epts){
      std::cout << "- " << e.name << '\n';
    }
  } // end of MFrontMaterialsMain

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
  
  struct Filter{
    std::function<bool(const EntryPoint&,
		       const std::string&)> f;
    std::string o;
  };

  template<typename FilterType>
  void add_filter(const std::string& o,
		  const std::string& d,
		  const FilterType& f)
  {
    auto add = [this,f](){
      const auto opt = this->currentArgument->getOption();
      this->filters.push_back({f,opt});
    };
    this->registerCallBack(o,CallBack(d,add,true));
  } // end of addFilter
  
  void registerCommandLineCallBacks(){
    auto interface_filter = [](const EntryPoint& e,
			       const std::string& i){
      using tfel::system::ExternalLibraryManager;
      auto& elm = ExternalLibraryManager::getExternalLibraryManager();
      return elm.getInterface(e.library,e.name)==i;
    };
    this->add_filter("--filter-by-interface",
		     "filter entry points by interface",
		     interface_filter);
  } // end of registerCommandLineCallBacks

  const tfel::utilities::Argument&
  getCurrentCommandLineArgument() const{
    return *(this->currentArgument);
  }
  
  void treatUnknownArgument()
  {
    using namespace tfel::system;
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
	ep.type = EntryPoint::MODEL;
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
  
  std::vector<Filter> filters;

  std::vector<EntryPoint> epts;
  
};

int main(const int argc,const char* const* const argv){
  MFrontMaterialsMain m(argc,argv);  
  return EXIT_SUCCESS;
}
