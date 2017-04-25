/*!
 * \file   mfm.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   13 avril 2017
 */

#include<cstdlib>
#include<iostream>
#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/Utilities/ArgumentParserBase.hxx"
#include"TFEL/System/LibraryInformation.hxx"

struct MFrontMaterialsMain
  : public tfel::utilities::ArgumentParserBase<MFrontMaterialsMain>
{

  MFrontMaterialsMain(const int argc,
		      const char *const *const argv)
    : tfel::utilities::ArgumentParserBase<MFrontMaterialsMain>(argc,argv)
  {
    this->registerCommandLineCallBacks();
    this->parseArguments();
  } // end of MFrontMaterialsMain

protected:				 

  void registerCommandLineCallBacks(){
  }

  const tfel::utilities::Argument& getCurrentCommandLineArgument() const{
    return *(this->currentArgument);
  }
  
  void treatUnknownArgument()
  {
    using tfel::utilities::ends_with;
    using tfel::system::LibraryInformation;
    const auto& a  = this->getCurrentCommandLineArgument();
    const auto& as = static_cast<const std::string&>(a);
    for(const auto& s : LibraryInformation(as).symbols()){
      if(ends_with(s,"_mfront_ept")){
	std::cout << s.substr(0,s.size()-11) << std::endl;
      }
    }
  }
  
  std::string getVersionDescription() const{
    return "1.0";
  }

  std::string getUsageDescription() const
  {
    return "Usage: "+this->programName+" [options] [files]";
  }

private:

  std::vector<std::string> libraries;
  
};

int main(const int argc,const char* const* const argv){
  MFrontMaterialsMain m(argc,argv);  
  return EXIT_SUCCESS;
}
