/*!
 * \file   mfront/src/GnuplotMaterialPropertyInterface.cxx
 * \brief    
 * \author P. GOLDBRONN
 * \date   14 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<algorithm>
#include<stdexcept>

#include"MFront/MFrontHeader.hxx"
#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontUtilities.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/GnuplotMaterialPropertyInterface.hxx"

// fixing a bug on current glibc++ cygwin versions (19/08/2015)
#if defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99)
#include<sstream>
namespace std{
  template<typename T>
  std::string to_string(const T& v){
    std::ostringstream s;
    s << v;
    return s.str();
  }
}
#endif /* defined __CYGWIN__ &&  (!defined _GLIBCXX_USE_C99) */

namespace mfront
{

  GnuplotMaterialPropertyInterface::dataFile::dataFile() = default;
  GnuplotMaterialPropertyInterface::dataFile::dataFile(const GnuplotMaterialPropertyInterface::dataFile&) = default;
  GnuplotMaterialPropertyInterface::dataFile::dataFile(GnuplotMaterialPropertyInterface::dataFile&&) = default;
  GnuplotMaterialPropertyInterface::dataFile&
  GnuplotMaterialPropertyInterface::dataFile::operator=(const GnuplotMaterialPropertyInterface::dataFile&) = default;
  GnuplotMaterialPropertyInterface::dataFile&
  GnuplotMaterialPropertyInterface::dataFile::operator=(GnuplotMaterialPropertyInterface::dataFile&&) = default;
  GnuplotMaterialPropertyInterface::dataFile::~dataFile() = default;
  
  GnuplotMaterialPropertyInterface::tokens_iterator
  GnuplotMaterialPropertyInterface::nextToken(tokens_iterator current,
				       const tokens_iterator endTokens,
				       const std::string &msg) {
    using namespace std;
    string what(msg) ;
    if(++current==endTokens){
      what+="unexpected end of file.\n";
      what+="Error at line : ";
      what+=to_string((--current)->line);
      throw(std::runtime_error(what));
    }
    return current;
  }

  std::string GnuplotMaterialPropertyInterface::getName()
  {
    return "gnuplot";
  }

  GnuplotMaterialPropertyInterface::GnuplotMaterialPropertyInterface() = default;

  GnuplotMaterialPropertyInterface::~GnuplotMaterialPropertyInterface() = default;
  
  std::string
  GnuplotMaterialPropertyInterface::eraseQuote(const std::string& in)
  {
    using namespace std;
    string res(in);
    if ((res[0]=='"')&&(res[res.size()-1]=='"')) {
      res.erase(res.begin()) ;
      res.erase(res.end()-1) ;
    }
    return res;
  }

  std::pair<bool,GnuplotMaterialPropertyInterface::tokens_iterator>
  GnuplotMaterialPropertyInterface::treatKeyword(const std::string& key,
						 const std::vector<std::string>& i,
						 tokens_iterator current,
						 const tokens_iterator endTokens)
  {
    if(std::find(i.begin(),i.end(),"gnuplot")!=i.end()){
      if((key!="@TestBounds")&&(key!="@Graph")){
	throw(std::runtime_error("GnuplotMaterialPropertyInterface::treatKeyword: "
				 "unsupported key '"+key+"'"));
      }
    }
    if ( key == "@TestBounds" ){
      return registerTestBounds(current,endTokens);
    } else if ( key == "@Graph"){
      return registerGraph(current,endTokens);
    }
    return {false,current};
  } // end of treatKeyword

  std::pair<bool,GnuplotMaterialPropertyInterface::tokens_iterator>
  GnuplotMaterialPropertyInterface::registerTestBounds(tokens_iterator p,
						       const tokens_iterator pe)
  {
    using tfel::utilities::CxxTokenizer;
    const std::string m = "GnuplotMaterialPropertyInterface::registerTestBounds";
    auto throw_if = [&m](const bool b,const std::string& msg){
      if(b){throw(std::runtime_error(m+": "+msg));}
    };
    const auto b = mfront::readVariableBounds(p,pe);
    CxxTokenizer::readSpecifiedToken(m,";",p,pe);
    throw_if(b.second.boundsType!=VariableBoundsDescription::LOWERANDUPPER,
	     "invalid bounds type");
    throw_if(!testBounds.insert(b).second,
	     "test bounds for variable '"+b.first+"' already registred");
    return {true,++p};
  } // end of registerTestBounds

  std::pair<bool,GnuplotMaterialPropertyInterface::tokens_iterator>
  GnuplotMaterialPropertyInterface::registerGraph(tokens_iterator current,
						  const tokens_iterator pe)
  {
    using namespace std;
    string msg("GnuplotMaterialPropertyInterface::registerGraph : ");
    current=nextToken(--current,pe,msg);
    if(current->value!="{"){
      msg+="expected '{'.\n";
      msg+="Error at line : ";
      msg+=to_string(current->line);
      throw(runtime_error(msg));
    }
    current=nextToken(current,pe,msg);
    while((current->value!="}")&&
	  (current!=pe)){
      if(current->value=="{"){
	msg+="'{' is not allowed in graph.\n";
	msg+="Error at line : ";
	msg+=to_string(current->line);
	throw(runtime_error(msg));
      }
      // on lit les mots clefs
      if (current->value == "Label") {
	//	current=readUntilEndOfInstruction(yLabel,++current,pe);
	// suppress " at begin and end of string if any
	
	
	current=nextToken(current,pe,msg);
	yLabel=eraseQuote(current->value);
	current=nextToken(current,pe,msg);
	if (current->value != ";") {
	  msg+="Label : you must give only a string.\n";
	  msg+="Error at line : ";
	  msg+=to_string((--current)->line);
	  throw(runtime_error(msg));
	}
	current=nextToken(current,pe,msg);
      } else if (current->value == "ExperimentalData") {
	current=nextToken(current,pe,msg);
	string input = eraseQuote(current->value) ;
	current=nextToken(current,pe,msg);
	dataFile mydataFile;
	mydataFile.name = eraseQuote(current->value) ;
	current=nextToken(current,pe,msg);
	mydataFile.legend = eraseQuote(current->value) ;
	experimentalData.insert({input,mydataFile}) ;
	current=nextToken(current,pe,msg);
	if (current->value != ";") {
	  msg+="ExperimentalData : you provide more than three arguments.\n";
	  msg+="Error at line : ";
	  msg+=to_string((--current)->line);
	  throw(runtime_error(msg));
	}
	current=nextToken(current,pe,msg);
	//      } else if (current->value == "xxx") {
      } else {
	msg+="Keyword ";
	msg+=current->value ;
	msg+=" is not allowed in graph.\n";
	msg+="Error at line : ";
	msg+=to_string(current->line);
	throw(runtime_error(msg));
      }
      //current=nextToken(current,pe,msg);
    }
    if(current==pe){
      msg+="File ended before the end of graph.\n";
      msg+="Error at line : ";
      msg+=to_string(current->line);
      throw(runtime_error(msg));
    }
    return make_pair(true,++current);
  } // end of registerGraph

  void
  GnuplotMaterialPropertyInterface::getTargetsDescription(TargetsDescription& d,
				       const MaterialPropertyDescription& mpd)
  {
    using std::string;
    const auto name = (mpd.material.empty()) ? mpd.className : mpd.material+"_"+mpd.className;
    const auto target = name+"GnuplotGraph";
    d.specific_targets[target].first.push_back(name+"CppTest");
    d.specific_targets[target].second.push_back("@./"+name+"CppTest");
    d.specific_targets[target].second.push_back(string("@")+string(GNUPLOT_PATH)+" "+name+".gp");
    d.specific_targets["graph"].first.push_back(target);
    d.specific_targets["check"].first.push_back(target);
  }

  void
  GnuplotMaterialPropertyInterface::writeOutputFiles(const MaterialPropertyDescription& mpd,
						     const FileDescription& fd)
  {
    using namespace std;
    const auto& file=fd.fileName;
    const auto& className=mpd.className;
    const auto& material=mpd.material;
    const auto& inputs=mpd.inputs;
    const auto name = (material.empty()) ? className : material+"_"+className;
    std::ofstream out("src/"+name+".gp");
    if(!out){
      throw(runtime_error("MaterialPropertyDSL::writeOutputFiles: "
			  "unable to open 'src/"+name+".gp' "
			  "for writing output file."));
    }
    out.exceptions(ios::badbit|ios::failbit);
    // writing src file
    string base;
    if(file.find('/')!=string::npos){
      base = file.substr(0,file.rfind('/'))+'/';
    }
    // en attendant qu'on puisse passer testbounds
    if (testBounds.empty()){
      for(const auto& i:mpd.inputs){
	if(hasBounds(i)){
	  if(testBounds.count(i.name)==0){
	    testBounds.insert({i.name,i.getBounds()});
	  }
	}
      }
    }
    out<<"# generated by mfront from " << file << endl << endl;
    out<< "set term postscript eps color enhanced" << endl
		 << "set encoding iso_8859_1" << endl
		 << "set grid" << endl << endl
		 << "set key under" << endl << endl;
    // On doit demander a l'utilisateur ce que l'on veut pour le placement de la legende !!!!
    if (!yLabel.empty())
      out<< "set ylabel '" << yLabel << "'" << endl;
    
    // on boucle sur toutes les variables pour generer les plot
    const auto csvFileName = name + "_test.csv";
    if(!inputs.empty()){
      if (inputs.size() == 1) {
	out<< "set xlabel '"<< inputs.begin()->name <<"'"<<endl;
	out<< "set output '"<< name << "_" << inputs.begin()->name << ".eps'" << endl;
	out<< "plot '" << csvFileName << "' u 1:2 notitle" ;
	//adding experimental data if any
	for(multimap< string, dataFile >::const_iterator it=experimentalData.begin();it!=experimentalData.end();++it){
	  if (inputs[0].name == (*it).first){
	    out<<",\\\n'../"<< base + it->second.name <<"' u 1:2 t '"<<it->second.legend<<"'" ;
	  }
	}
	out<<endl;
      } else {
	int column = 0 ;
	for(auto p3=inputs.begin();p3!=inputs.end();p3++){
	  out<< "set xlabel '"<< p3->name <<"'"<<endl;
	  out<< "set output '"<< name << "_" << p3->name << ".eps'" << endl ;
	  int columnIni=++column ;
	  out<< "plot '" << csvFileName << "' u " <<columnIni<<":"<<++column<< " t '" ;
	  for(auto p4=inputs.begin();p4!=inputs.end();++p4){
	    if (p4 != p3) {
	      for(auto p6=testBounds.begin();p6!=testBounds.end();++p6){
		if (p6->first == p4->name) {
		  out<<p4->name<<"="<<p6->second.lowerBound ;
		  if (p3+1==inputs.end()) {
		    if (p4+2!=inputs.end())
		      out<<",";
		  } else {
		    if (p4+1!=inputs.end())
		      out<<",";
		  }
		}
	      }
	    } 
	  }
	  out<< "'" ;
	  out<<" w l lw 2 ";
	  out<< ", '" << csvFileName << "' u " <<columnIni<<":"<<++column<< " t '" ;
	  for(auto p4=inputs.begin();p4!=inputs.end();++p4){
	    if (p4 != p3) {
	      // pb si vide !
	      for(auto p6=testBounds.begin();p6!=testBounds.end();++p6){
		if (p6->first == p4->name) {
		  out<<p4->name<<"="<<(p6->second.upperBound-p6->second.lowerBound)/2.;
		  if (p3+1==inputs.end()) {
		    if (p4+2!=inputs.end())
		      out<<",";
		  } else {
		    if (p4+1!=inputs.end())
		      out<<",";
		  }
		}
	      }
	    }	
	  }
	  out<< "'" ;
	  out<<" w l lw 2 ";
	  out<< ", '" << csvFileName << "' u " <<columnIni<<":"<<++column<< " t '" ;
	  for(auto p4=inputs.begin();p4!=inputs.end();++p4){
	    if (p4 != p3) {
	      for(auto p6=testBounds.begin();p6!=testBounds.end();++p6){
		if (p6->first == p4->name){
		  out<<p4->name<<"="<<p6->second.upperBound;
		  if (p3+1==inputs.end()) {
		    if (p4+2!=inputs.end())
		      out<<",";
		  } else {
		    if (p4+1!=inputs.end())
		      out<<",";
		  }
		}
	      }
	    }
	  }
	  out<< "'" ;
	  out<<" w l lw 2 ";
	  //adding experimental data if any
	  for(auto it=experimentalData.begin();it!=experimentalData.end();++it){
	    if (p3->name == (*it).first){
	      out << "\\"<<endl<<",'../"<< base + (*it).second.name
			    << "' u 1:2 t '"<<(it->second).legend<<"' w p lw 2";
	    }
	  }
	  out<<endl;
	}
      }
      out.close();
    }

  } // end of GnuplotMaterialPropertyInterface::writeSrcFile(void)

} // end of namespace mfront
