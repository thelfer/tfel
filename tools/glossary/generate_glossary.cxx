/*! 
 * \file  generate_glossary.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 14 mai 2009
 */

// g++ -Iinclude `tfel-config --includes --libs --utilities` generate_glossary.cxx -o generate_glossary
// g++ -DTFELGLOSSARY_VISIBILITY_EXPORT Glossary.cxx -o Glossary.o -I include/ `tfel-config --includes` -c

#include<iostream>
#include<stdexcept>
#include<algorithm>
#include<iterator>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<string>
#include<set>

#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"TFEL/Glossary/GlossaryEntry.hxx"
#include"GlossaryEntry.cxx"

struct GlossaryTokenizer
  : protected tfel::utilities::CxxTokenizer,
    protected std::set<tfel::glossary::GlossaryEntry>
{

  using std::set<tfel::glossary::GlossaryEntry>::begin;

  using std::set<tfel::glossary::GlossaryEntry>::end;

  using std::set<tfel::glossary::GlossaryEntry>::const_iterator;

  void execute(const std::string& f)
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::glossary;
    CxxTokenizer::TokensContainer::const_iterator p;
    CxxTokenizer::TokensContainer::const_iterator p2;
    CxxTokenizer::TokensContainer::const_iterator pe;
    this->treatCharAsString(true);
    this->openFile(f);
    // stripping comment
    this->stripComments();
    p=CxxTokenizer::begin();
    pe = CxxTokenizer::end();
    while(p!=pe){
      string key;
      string name;
      string unit;
      string type;
      string short_description;
      vector<string> description;
      vector<string> latex_description;
      vector<string> note;
      key = CxxTokenizer::readString(p,pe);
      if(!this->isValidIdentifier(key,false)){
	string msg("GlossaryTokenizer : invalid glossary key '"+key+"'");
	throw(runtime_error(msg));
      }
      CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute","{",p,pe);
      CxxTokenizer::checkNotEndOfLine("GlossaryTokenizer::execute",p,pe);
      while(p->value!="}"){
	const string k = p->value;
	++p;
	CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute",":",p,pe);
	try{
	  if(k=="unit"){
	    if(!unit.empty()){
	      string msg("GlossaryTokenizer::execute : "
			 "unit already read for key '"+key+"'");
	    }
	    unit = CxxTokenizer::readString(p,pe);
	  } else if(k=="name"){
	    if(!name.empty()){
	      string msg("GlossaryTokenizer::execute : "
			 "name already read for key '"+key+"'");
	    }
	    name = CxxTokenizer::readString(p,pe);
	  } else if(k=="type"){
	    if(!type.empty()){
	      string msg("GlossaryTokenizer::execute : "
			 "type already read for key '"+key+"'");
	    }
	    type = CxxTokenizer::readString(p,pe);
	  } else if(k=="short_description"){
	    if(!short_description.empty()){
	      string msg("GlossaryTokenizer::execute : "
			 "short description already read for key '"+key+"'");
	    }
	    short_description = CxxTokenizer::readString(p,pe);
	  } else if(k=="description"){
	    if(!description.empty()){
	      string msg("GlossaryTokenizer::execute : "
			 "description already read for key '"+key+"'");
	    }
	    this->readBlock(description,p,pe);
	    CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute","}",p,pe);
	  } else if(k=="latex_description"){
	    if(!latex_description.empty()){
	      string msg("GlossaryTokenizer::execute : "
			 "latex_description already read for key '"+key+"'");
	    }
	    this->readBlock(latex_description,p,pe,true);
	    CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute","}",p,pe);
	  } else if(k=="note"){
	    if(!note.empty()){
	      string msg("GlossaryTokenizer::execute : "
			 "note already read for key '"+key+"'");
	    }
	    this->readBlock(note,p,pe);
	    CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute","}",p,pe);
	  } else {
	    string msg("GlossaryTokenizer::execute : "
		       "unknown key '"+k+"'");
	    throw(runtime_error(msg));
	  }
	  CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute",";",p,pe);
	  CxxTokenizer::checkNotEndOfLine("GlossaryTokenizer::execute",p,pe);
	} catch(exception& ex){
	  string msg("GlossaryTokenizer::execute : ");
	  msg += "error while treating key '"+key+"' (";
	  msg += ex.what();
	  msg += ")";
	  throw(runtime_error(msg));
	}
      }
      CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute","}",p,pe);
      CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute",";",p,pe);
      // postprocessing
      if(short_description.empty()){
	string msg("GlossaryTokenizer::execute : "
		   "no short_description given for glossary entry '"+key+"'");
	throw(runtime_error(msg));
      }
      if(type.empty()){
	string msg("GlossaryTokenizer::execute : "
		   "no type given for glossary entry '"+key+"'");
	throw(runtime_error(msg));
      }
      if(unit.empty()){
	string msg("GlossaryTokenizer::execute : "
		   "no unit given for glossary entry '"+key+"'");
	throw(runtime_error(msg));
      }
      if(name.empty()){
	name = key;
      }
      this->insert(GlossaryEntry(key,name,unit,type,
				 short_description,
				 description,
				 latex_description,
				 note));
    }
  } // end of execute
  
private:
  
  void
  readBlock(std::vector<std::string>& b,
	    CxxTokenizer::TokensContainer::const_iterator& p,
	    const CxxTokenizer::TokensContainer::const_iterator pe,
	    const bool r = false)
  {
    using namespace std;
    using tfel::utilities::replace_all;
    CxxTokenizer::readSpecifiedToken("GlossaryTokenizer::execute","{",p,pe);
    CxxTokenizer::checkNotEndOfLine("GlossaryTokenizer::execute",p,pe);
    bool first = true;
    b.push_back(string());
    while(p->value!="}"){
      CxxTokenizer::TokensContainer::const_iterator p2 = p;
      const string& s = CxxTokenizer::readString(p,pe);
      if(r){
	b.back()+=replace_all(s,"\\","\\\\")+' ';
      } else {
	b.back()+=s+' ';
      }
      CxxTokenizer::checkNotEndOfLine("GlossaryTokenizer::execute",p,pe);
      if(first){
	first = false;
      }
      if(p->value!="}"){
	if(p->line!=p2->line){
	  b.push_back(string());
	}
      }
    }
  }

}; // end of struct GlossaryTokenizer

std::string
serialize(const std::vector<std::string>& v,
	  const std::string& d)
{
  using namespace std;
  ostringstream out;
  if(v.empty()){
    out <<  "\"\" /* no '" << d << "' defined */";
  } else {
    vector<string>::const_iterator p;
    for(p=v.begin();p!=v.end();++p){
      if(p!=v.begin()){
	out << "\"@^separator^@\"\n";
      }
      out << "\"" << *p << "\"";
    }
  }
  return out.str();
}

void generateCxxOutput(const GlossaryTokenizer& tokenizer)
{
  using namespace std;
  GlossaryTokenizer::const_iterator p;
  ofstream header("Glossary.hxx");
  ofstream src("Glossary.cxx");
  if(!header){
    string msg("generate_glossary : ");
    msg += "can't open file 'Glossary.hxx'";
    throw(runtime_error(msg));    
  }
  if(!src){
    string msg("generate_glossary : ");
    msg += "can't open file 'Glossary.cxx'";
    throw(runtime_error(msg));    
  }
  // writting headers
  header << "/*!" << endl;
  header << " *\\file   Glossary.hxx" << endl;
  header << " *\\author Helfer Thomas" << endl;
  header << " *\\date   09/06/14" << endl;
  header << "*/" << endl;
  header << endl;
  header << "#ifndef __TFEL_GLOSSARY_GLOSSARY_HH__" << endl;
  header << "#define __TFEL_GLOSSARY_GLOSSARY_HH__" << endl;
  header << endl;
  header << "#include\"TFEL/Config/TFELConfig.hxx\"" << endl;
  header << "#include<set>" << endl;
  header << "#include<string>" << endl;
  header << endl;
  header << "#include\"TFEL/Glossary/GlossaryEntry.hxx\"" << endl;
  header << endl;
  header << "namespace tfel" << endl;
  header << "{" << endl;
  header << "namespace glossary" << endl;
  header << "{" << endl;
  header << endl;
  header << "/*!\n"
	 << " * \\brief Structure in charge of handling the TFEL Glossary\n"
	 << " */" << endl;
  header << "struct TFELGLOSSARY_VISIBILITY_EXPORT Glossary" << endl;
  header << "{" << endl;
  header << endl;
  header << "/*!\n"
	 << " * \\return the uniq instance of this class (singleton pattern)\n"
	 << " */" << endl;
  header << "static Glossary&" << endl;
  header << "getGlossary();" << endl;
  header << endl;
  header << endl;
  header << "/*!\n"
	 << " * \\return true if the glossary contains the given name\n"
	 << " * \\param[in] n : name\n"
	 << " */\n" << endl;
  header << "bool" << endl;
  header << "contains(const std::string&) const;" << endl;
  header << endl;
  for(p=tokenizer.begin();p!=tokenizer.end();++p){
    header << "static const GlossaryEntry " << p->getKey() << ";" << endl;
  }
  header << endl;
  header << "private :" << endl;
  header << "/*!" << endl;
  header << " * \\brief insert a new entry" << endl;
  header << " */" << endl;
  header << "void" << endl;
  header << "insert(const GlossaryEntry&);" << endl;
  header << endl;
  header << "Glossary();" << endl;
  header << endl;
  header << "Glossary(const Glossary&);" << endl;
  header << endl;
  header << "Glossary&" << endl;
  header << "operator=(const Glossary&);" << endl;
  header << endl;
  header << "std::set<GlossaryEntry> entries;" << endl;
  header << endl;
  header << "}; // end of struct Glossary" << endl;
  header << endl;
  header << "} // end of namespace glossary" << endl;
  header << endl;
  header << "} // end of namespace tfel" << endl;
  header << endl;
  header << "#endif /* __TFEL_GLOSSARY_GLOSSARY_HH__ */" << endl;
  /* writting src file */
  src << "/*!" << endl;
  src << " *\\file   Glossary.cxx" << endl;
  src << " *\\author Helfer Thomas" << endl;
  src << " *\\date   09/06/14" << endl;
  src << "*/" << endl;
  src << endl;
  src << "#include<stdexcept>" << endl;
  src << endl;
  src << "#include\"TFEL/Glossary/Glossary.hxx\"" << endl;
  src << endl;
  src << "namespace tfel" << endl;
  src << "{" << endl;
  src << "namespace glossary" << endl;
  src << "{" << endl;
  src << endl;
  for(p=tokenizer.begin();p!=tokenizer.end();++p){
    const string& d  = serialize(p->getDescription(),"description");
    const string& ld = serialize(p->getLaTeXDescription(),"latex description");
    const string& n  = serialize(p->getNotes(),"notes");
    src << "const GlossaryEntry Glossary::" << p->getKey() << "("
	<< "\"" << p->getKey()  << "\","
	<< "\"" << p->getName() << "\",\n"
	<< "\"" << p->getUnit() << "\","
	<< "\"" << p->getType() << "\",\n"
	<< "\"" << p->getShortDescription() << "\",\n"
	<< d << ",\n";
    if(ld!=d){
      src << ld << ",\n";
    } else {
      src << "\"\",\n";
    }
    src << n << ");\n\n";
  }
  src << "Glossary&" << endl;
  src << "Glossary::getGlossary(void)" << endl;
  src << "{" << endl;
  src << "static Glossary glossary;" << endl;
  src << "return glossary;" << endl;
  src << "} // end of Glossary::getGlossary" << endl;
  src << endl;
  src << "Glossary::Glossary()";
  src << "{" << endl;
  for(p=tokenizer.begin();p!=tokenizer.end();++p){
    src << "this->insert(Glossary::" << p->getKey() << ");" << endl;
  }
  src << "} // end of Glossary::Glossary" << endl;
  src << endl;
  src << "void" << endl;
  src << "Glossary::insert(const GlossaryEntry& e)" << endl;
  src << "{" << endl;
  src << "using namespace std;" << endl;
  src << "if(!this->entries.insert(e).second){" << endl;
  src << "string msg(\"Glossary::insert : \");" << endl;
  src << "msg += \"'\"+e.getName()+\"' already declared\";" << endl;
  src << "throw(runtime_error(msg));" << endl;
  src << "}" << endl;
  src << "} // end of Glossary::insert" << endl;
  src << endl;
  src << "bool" << endl;
  src << "Glossary::contains(const std::string& w) const" << endl;
  src << "{" << endl;
  src << "using namespace std;" << endl;
  src << "set<GlossaryEntry>::const_iterator p;" << endl;
  src << "for(p=this->entries.begin();p!=this->entries.end();++p){" << endl;
  src << "if(p->getName()==w){" << endl;
  src << "return true;" << endl;
  src << "}" << endl;
  src << "}" << endl;
  src << "return false;" << endl;
  src << "} // end of Glossary::contains" << endl;
  src << endl;
  src << "} // end of namespace glossary" << endl;
  src << endl;
  src << "} // end of namespace tfel" << endl;
}

void generateBoostPythonBindings(const GlossaryTokenizer& tokenizer)
{
  using namespace std;
  ofstream psrc("PythonGlossary.cxx");
  if(!psrc){
    string msg("generate_glossary : ");
    msg += "can't open file 'PythonGlossary.cxx'";
    throw(runtime_error(msg));    
  }
  GlossaryTokenizer::const_iterator p;
  // python bindings
  psrc << "/*!" << endl;
  psrc << " *\\file   PythonGlossary.cxx" << endl;
  psrc << " *\\author Helfer Thomas" << endl;
  psrc << " *\\date   09/06/14" << endl;
  psrc << "*/" << endl;
  psrc << endl;
  psrc << "#include<boost/python.hpp>" << endl;
  psrc << endl;
  psrc << "#include\"TFEL/Glossary/Glossary.hxx\"" << endl;
  psrc << endl;
  psrc << "BOOST_PYTHON_MODULE(glossary)" << endl;
  psrc << "{" << endl;
  psrc << "using namespace boost;" << endl;
  psrc << "using namespace boost::python;" << endl;
  psrc << "using namespace tfel::glossary;" << endl;
  psrc << "class_<Glossary,noncopyable>(\"Glossary\",no_init)" << endl;
  psrc << ".def(\"getGlossary\",Glossary::getGlossary," << endl
       << "return_value_policy<reference_existing_object>())" << endl
       << ".staticmethod(\"getGlossary\")" << endl
       << ".def(\"contains\",&Glossary::contains)" << endl;
  for(p=tokenizer.begin();p!=tokenizer.end();++p){
    psrc << ".def_readonly(\"" << p->getKey() << "\",&Glossary::" << p->getKey();
    const string& d = p->getShortDescription();
    if(!d.empty()){
      psrc << ",\n\"" << d << "\"";
    }
    psrc << ")" << endl;
  }
  psrc << ";" << endl << endl;
  psrc << "}" << endl;
} // end of generateBoostPythonBindings

void generateXMLOutput(const GlossaryTokenizer& tokenizer)
{
  using namespace std;
  ofstream xml("glossary.xml");
  if(!xml){
    string msg("generate_glossary : ");
    msg += "can't open file 'PythonGlossary.cxx'";
    throw(runtime_error(msg));    
  }
  GlossaryTokenizer::const_iterator p;
  xml << "<?xml version=\"1.0\"?>" << endl;
  xml << "<glossary>" << endl;
  for(p=tokenizer.begin();p!=tokenizer.end();++p){
    xml << "<glossary_entry key=\"" << p->getKey() << "\">" << endl;
    xml << "<name>" << p->getName() << "</name>" << endl;
    xml << "<unit>" << p->getUnit() << "</unit>" << endl;
    xml << "<type>" << p->getType() << "</type>" << endl;
    xml << "<short_description>" << p->getType() << "</short_description>" << endl;
    xml << "<description>\n" 
	<< "<![CDATA[\n";
    const vector<string>& d =  p->getDescription();
    for(vector<string>::const_iterator p2=d.begin();p2!=d.end();++p2){
      xml << *p2 << endl;
    }
    xml << "]]>"
	<< "\n</description>" << endl;
    xml << "</glossary_entry>" << endl;;
  }
  xml << "</glossary>" << endl;

}

int main(void)
{
  using namespace std;
  GlossaryTokenizer tokenizer;
  tokenizer.execute("glossary.txt");
  generateCxxOutput(tokenizer);  
  generateBoostPythonBindings(tokenizer);  
  generateXMLOutput(tokenizer);  
} // end of main
