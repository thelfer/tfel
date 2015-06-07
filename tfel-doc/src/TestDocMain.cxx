/*! 
 * \file  tfel-doc/src/TestDocMain.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<string>
#include<vector>
#include<cstdlib>
#include<fstream>
#include<sstream>
#include<iostream>
#include<iterator>
#include<stdexcept>
#include<algorithm>
#include<stdexcept>
#include<cstdlib>
#include<cerrno>
#include<cstring>

#include <sys/param.h>

#include"TFEL/System/System.hxx"
#include"TFEL/System/RecursiveFind.hxx"
#include"TFEL/Utilities/TerminalColors.hxx"
#include<memory>

#include"TFEL/Utilities/Global.hxx"
#include"TFEL/Utilities/LaTeXConvertion.hxx"
#include"TFEL/Utilities/TestDocumentation.hxx"
#include"TFEL/Utilities/TestDocParser.hxx"
#include"TFEL/Utilities/MTestDocParser.hxx"
#include"TFEL/Utilities/ConfigParser.hxx"
#include"TFEL/Utilities/TestDocMain.hxx"

namespace tfel
{

  namespace utilities
  {

    static std::string
    replace_all(const std::string& c,
		const char c1,
		const char c2)
    {
      using namespace std;
      string s(c);
      string::size_type p  = 0u;
      if(s.size()==0){
	return "";
      }
      while((p=s.find(c1,p))!=string::npos){
	s[p] = c2;
	p+=1u;
      }
      return s;
    } // end of replace_all

    static std::string
    getOutputDirectory(const std::string& f)
    {
      using namespace std;
      string::size_type pos = f.rfind('/');
      if(pos==string::npos){
	return "";
      }
      return f.substr(0,pos);
    } // end of getOutputDirectory
   
    static std::string
    getSectionHeaderTranslation(const std::string& s,
				const std::string& l)
    {
      using namespace std;
      using namespace tfel::utilities;
      if(l=="french"){
	return "Test de la catégorie \\og~"+getCategory(s,l)+"~\\fg{}";
      }
      if(l!="english"){
	cerr << TerminalColors::Red;
	cerr << "getSectionHeaderTranslation : unsupported language'"+l
	     << "', using default" << endl;
	cerr << TerminalColors::Reset;
      }
      return s;
    }

    static std::string
    getBabelPackage(const std::string& l)
    {
      using namespace std;
      using namespace tfel::utilities;
      if(l!="french"){
	cerr << TerminalColors::Red;
	cerr << "getBabelPackage : unsupported language '" << l << 
	  "', using default" << endl;
	cerr << TerminalColors::Reset;
	return "";
      }
      return "\\usepackage[frenchb]{babel}";
    }

    static void
    printLaTeXFile(std::ostream& log,
		   const std::map<std::string,std::vector<TestDocumentation> >& tests,
		   const std::string& d,
		   const std::string& prefix,
		   const std::string& l,
		   const bool fragment,
		   const bool split)
    {
      using namespace std;
      map<string,vector<TestDocumentation> >::const_iterator p;
      vector<TestDocumentation>::const_iterator p2;
      if(!fragment){
	log << "\\documentclass[a4paper,12pt]{article}" << endl;
	log << endl;
	log << "\\usepackage[utf8]{inputenc}" << endl;
	log << "\\usepackage{multind}" << endl;
	log << "\\usepackage{amsmath}" << endl;
	log << "\\usepackage{color}" << endl;
	log << endl;
	log << "\\newcommand{\\Frac}[2]{\\displaystyle\\frac{\\displaystyle #1}{\\displaystyle #2}}" << endl;
	log << "\\newcommand{\\paren}[1]{\\ensuremath\\left(#1\\right)}" << endl;
	log << "\\newcommand{\\tenseur}[1]{\\ensuremath\\underline{#1}}" << endl;
	log << "\\newcommand{\\tenseurq}[1]{\\ensuremath\\underline{\\underline{#1}}}" << endl;
	log << "\\newcommand{\\nom}[1]{\\textsc{#1}}" << endl;
	log << endl;
	log << "% one column index" << endl;
	log << "\\makeatletter" << endl;
	log << "\\def\\printindex#1#2{\\section*{#2}" << endl;
	log << "\\addcontentsline{toc}{section}{#2}" << endl;
	log << "\\@input{#1.ind}}"  << endl;
	log << "\\makeatother" << endl;
	log << endl;
	if(l!="english"){
	  log << getBabelPackage(l) << endl;
	}
	log << "\\makeindex{general}" << endl;
	log << "\\makeindex{models}" << endl;
	log << endl;
	log << "\\begin{document}"<< endl;
	log << endl;
      }
      for(p=tests.begin();p!=tests.end();++p){
	log << "\\clearpage" << endl;
	log << "\\newpage" << endl;
	if(split){
	  const auto& tf = replace_all(p->first,' ','_')+".tex";
	  const auto& file = d+"/"+tf;
	  ofstream f(file.c_str());
	  if(!f){
	    string msg("printLaTeXFile : can't open file '"+file+"'");
	    throw(runtime_error(msg));
	  }
	  f << "\\section{" << getSectionHeaderTranslation(p->first,l) << "}" << endl;
	  f << endl; 
	  for(p2=p->second.begin();p2!=p->second.end();++p2){
	    p2->writeLaTexDescription(f,prefix,l);
	  }
	  log << "\\input{" << tf << "}" << endl << endl;
	} else {
	  log << "\\section{" << getSectionHeaderTranslation(p->first,l) << "}" << endl;
	  log << endl; 
	  for(p2=p->second.begin();p2!=p->second.end();++p2){
	    p2->writeLaTexDescription(log,prefix,l);
	  }
	}
      }
      if(!fragment){
	log << "\\clearpage" << endl;
	log << "\\newpage" << endl;
	log << "\\printindex{general}{" << LaTeXConvertion::capitalize(getTranslation("general index",l)) << "}" << endl  << endl;
	log << "\\clearpage" << endl;
	log << "\\newpage" << endl;
	log << "\\printindex{models}{" << LaTeXConvertion::capitalize(getTranslation("models index",l)) << "}" << endl << endl;
	log << "\\end{document}"<< endl;
      }
    }

    static void
    declareKeys(const std::string& f)
    {
      ConfigParser parser;
      parser.execute(getKeysMap(),f);
    }

    static void
    declareCategories(const std::string& f)
    {
      ConfigParser parser;
      parser.execute(getCategoriesMap(),f);
    }

    static void
    declareTranslations(const std::string& f)
    {
      ConfigParser parser;
      parser.execute(getTranslationsMap(),f);
    }

    TestDocMain::TestDocMain(const int argc,
			     const char*const* argv)
      : ArgumentParserBase<TestDocMain>(argc,argv),
	mtest(false),
	fragment(false),
	split(false)
    {
      using namespace std;
      using namespace tfel::utilities;
      this->registerArgumentCallBacks();
      this->parseArguments();
      if(this->outputFile.empty()){
	cerr << "TestDocMain : no output file specified\n";
	cerr << this->getUsageDescription() << endl;
	exit(EXIT_FAILURE);
      }
      this->outputDirectory = getOutputDirectory(this->outputFile);
      this->output.open(this->outputFile.c_str());
      if(!this->output){
	string msg("TestDocMain : can't open output file '");
	msg += this->outputFile;
	msg += '\'';
	throw(runtime_error(msg));
      }
      if(!this->logFile.empty()){
	this->logptr = shared_ptr<ostream>(new ofstream(argv[2]));
	if(!(*(this->logptr))){
	  string msg("TestDocMain : can't open log file '");
	  msg += this->logFile;
	  msg += '\'';
	  throw(runtime_error(msg));
	}
	this->log = this->logptr.get();
      } else{ 
	this->log = &cerr;
      }
      if(this->lang.empty()){
	lang = "english";
      }
    } // end of TestDocMain::TestDocMain

    void
    TestDocMain::treatUnknownArgument(void)
    {
      using namespace std;
      const auto& s = static_cast<const string&>(*(this->currentArgument));
      if(s[0]=='-'){
	ArgumentParserBase<TestDocMain>::treatUnknownArgument();
	cerr << "TestDocMain : unsupported option '" << s << "'\n";
	cerr << this->getUsageDescription() << endl;
	exit(EXIT_FAILURE);
      }
      if(this->outputFile.empty()){
	this->outputFile = s;
      } else {
	cerr << "TestDocMain : output file already specified" << endl;
	cerr << this->getUsageDescription() << endl;
	exit(EXIT_FAILURE);
      }
    } // end of TestDocMain::treatUnknownArgument

    void 
    TestDocMain::registerArgumentCallBacks(void)
    {
      this->registerNewCallBack("--lang",&TestDocMain::treatLang,
				"specify output language (french,english)",true);
      this->registerNewCallBack("--fragment","-f",&TestDocMain::treatFragment,
				"don't print TeX header",false);
      this->registerNewCallBack("--split","-s",&TestDocMain::treatSplit,
				"split outputs by categories",false);
      this->registerNewCallBack("--src",&TestDocMain::treatSrc,
				"specify root of sources",true);
      this->registerNewCallBack("--log",&TestDocMain::treatLogFile,
				"specify a log file",true);
      this->registerNewCallBack("--keys",&TestDocMain::treatKeyFile,
				"specify a key file",true);
      this->registerNewCallBack("--categories",&TestDocMain::treatCategoryFile,
				"specify a category file",true);
      this->registerNewCallBack("--prefix",&TestDocMain::treatPrefix,
				"specify the application installation directory",true);
      this->registerNewCallBack("--translations",&TestDocMain::treatTranslationFile,
				"specify a translation file",true);
      this->registerNewCallBack("--mtest",&TestDocMain::treatMTest,
				"add mtest files");
    } // end of TestDocMain::registerArgumentCallBacks

    void
    TestDocMain::treatMTest(void)
    {
      this->mtest=true;
    }

    void
    TestDocMain::treatFragment(void)
    {
      this->fragment=true;
    }

    void
    TestDocMain::treatSplit(void)
    {
      this->split=true;
    }

    void
    TestDocMain::treatLogFile(void)
    {
      using namespace std;
      if(!this->logFile.empty()){
	cerr << "TestDocMain : log file already specified" << endl;
	cerr << this->getUsageDescription() << endl;
	exit(EXIT_FAILURE);
      }
      this->logFile = this->currentArgument->getOption();
      if(this->logFile.empty()){
	string msg("TestDocMain::treatlogFile : ");
	msg += "no option given to the --logFile argument";
	throw(runtime_error(msg));
      }
    } // end of TestDocMain::treatLogFile

    void
    TestDocMain::treatPrefix(void)
    {
      using namespace std;
      if(!this->prefix.empty()){
	cerr << "TestDocMain : log file already specified" << endl;
	cerr << this->getUsageDescription() << endl;
	exit(EXIT_FAILURE);
      }
      this->prefix = this->currentArgument->getOption();
      if(this->prefix.empty()){
	string msg("TestDocMain::treatprefix : ");
	msg += "no option given to the --prefix argument";
	throw(runtime_error(msg));
      }
    } // end of TestDocMain::treatPrefix

    void
    TestDocMain::treatSrc(void)
    {
      using namespace std;
      if(!this->srcdir.empty()){
	cerr << "TestDocMain : srcdir file already specified" << endl;
	cerr << this->getUsageDescription() << endl;
	exit(EXIT_FAILURE);
      }
      this->srcdir = this->currentArgument->getOption();
      if(this->srcdir.empty()){
	string msg("TestDocMain::treatSrc : ");
	msg += "no option given to the --src argument";
	throw(runtime_error(msg));
      }
    } // end of TestDocMain::treatSrc

    void
    TestDocMain::treatLang(void)
    {
      using namespace std;
      if(!this->lang.empty()){
	cerr << "TestDocMain : lang file already specified" << endl;
	cerr << this->getUsageDescription() << endl;
	exit(EXIT_FAILURE);
      }
      this->lang = this->currentArgument->getOption();
      if(this->lang.empty()){
	string msg("TestDocMain::treatLang : ");
	msg += "no option given to the --lang argument";
	throw(runtime_error(msg));
      }
    } // end of TestDocMain::treatLang

    void
    TestDocMain::treatKeyFile(void)
    {
      using namespace std;
      const auto& f = this->currentArgument->getOption();
      declareKeys(f);
    } // end of TestDocMain::treatKeyFile

    void
    TestDocMain::treatCategoryFile(void)
    {
      using namespace std;
      const auto& f = this->currentArgument->getOption();
      declareCategories(f);
    } // end of TestDocMain::treatCategoryFile

    void
    TestDocMain::treatTranslationFile(void)
    {
      using namespace std;
      const auto& f = this->currentArgument->getOption();
      declareTranslations(f);
    } // end of TestDocMain::treatTranslationFile

    std::string 
    TestDocMain::getVersionDescription(void) const
    {
      using namespace std;
      return "1.0";
    }
    
    std::string 
    TestDocMain::getUsageDescription(void) const
    {
      using namespace std;
      string usage("Usage : tfel-doc [options] output");
      return usage;
    }

    int
    TestDocMain::execute(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::system;
      char path[MAXPATHLEN];
      char cpath[MAXPATHLEN];
      map<string,vector<string> >::const_iterator p;
      vector<string>::const_iterator p2;
      map<string,vector<TestDocumentation> > tests;
      if(realpath(".",cpath)==nullptr){
	*(this->log) << "main : can't get real path of current directory, aborting\n";
	exit(EXIT_FAILURE);
      }

      if(!this->srcdir.empty()){
	if(chdir(this->srcdir.c_str())==-1){
	  *(this->log) << "can't move to directory " << this->srcdir << endl;
	  exit(EXIT_FAILURE);
	}
      }

      // testdoc files
      map<string,vector<string> > files;
      recursiveFind(files,".*\\.testdoc$",".");
      for(p=files.begin();p!=files.end();++p){
	if(realpath(p->first.c_str(),path)==nullptr){
	  *(this->log) << "entering directory " << p->first << endl;
	} else {
	  *(this->log) << "entering directory " << path << endl;
	} 
	for(p2=p->second.begin();p2!=p->second.end();++p2){
	  string name = p->first+'/'+*p2;
	  try{
	    TestDocParser parser(name);
	    parser.addDocumentation(tests);
	  }
	  catch(std::exception& e){
	    *(this->log) << TerminalColors::Reset;
	    *(this->log) << "treatment of file '"+*p2+"' failed : "
			 << e.what() << endl;
	    *(this->log) << TerminalColors::Reset;
	  }
	}
      }
      
      // mtest files
      recursiveFind(files,".*\\.mtest",".");
      for(p=files.begin();p!=files.end();++p){
	if(realpath(p->first.c_str(),path)==nullptr){
	  *(this->log) << "entering directory " << p->first << endl;
	} else {
	  *(this->log) << "entering directory " << path << endl;
	} 
	for(p2=p->second.begin();p2!=p->second.end();++p2){
	  string name = p->first+'/'+*p2;
	  try{
	    MTestDocParser parser(name);
	    parser.addDocumentation(tests);
	  }
	  catch(std::exception& e){
	    *(this->log) << TerminalColors::Reset;
	    *(this->log) << "treatment of file '"+*p2+"' failed : "
			 << e.what() << endl;
	    *(this->log) << TerminalColors::Reset;
	  }
	}
      }

      if(!this->srcdir.empty()){
	if(chdir(cpath)==-1){
	  *(this->log) << "can't move to directory " << cpath << endl;
	  exit(EXIT_FAILURE);
	}
      }

      printLaTeXFile(this->output,tests,
		     this->outputDirectory,
		     this->prefix,this->lang,
		     this->fragment,this->split);

      map<string,vector<TestDocumentation> >::const_iterator p3;
      map<string,vector<TestDocumentation> >::size_type count = 0u;
      for(p3=tests.begin();p3!=tests.end();++p3){
	count += p3->second.size();
      }
      cout << count << " tests treated in " <<  tests.size() << " categories" << endl;
      return EXIT_SUCCESS;
    }

    TestDocMain::~TestDocMain()
    {} // end of TestDocMain::~TestDocMain()

  } // end of namespace utilities
  
} // end of namespace tfel
