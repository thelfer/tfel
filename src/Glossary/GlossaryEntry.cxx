/*! 
 * \file  GlossaryEntry.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 31 mars 2014
 */

#include<stdexcept>

#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/Glossary/GlossaryEntry.hxx"

namespace tfel
{

  namespace glossary
  {
    
    GlossaryEntry::GlossaryEntry(const std::string& k,
				 const std::string& n,
				 const std::string& u,
				 const std::string& t,
				 const std::string& sd,
				 const std::vector<std::string>& d,
				 const std::vector<std::string>& l,
				 const std::vector<std::string>& no)
      : key(k),
	name(n),
	unit(u),
	type(t),
	short_description(sd),
	description(d),
	latex_description( (!l.empty()) ? l : d),
	notes(no)
    {
      using namespace std;
      if(!(t=="scalar")&&(t=="vector")&&(t=="symmetric tensor")){
	string msg("GlossaryTokenizer::execute : "
		   "unsupported type '"+t+"'");
	throw(runtime_error(msg));
      }
    }

    GlossaryEntry::GlossaryEntry(const std::string& k,
				 const std::string& n,
				 const std::string& u,
				 const std::string& t,
				 const std::string& sd,
				 const std::string& d,
				 const std::string& l,
				 const std::string& no)
      : key(k),
	name(n),
	unit(u),
	type(t),
	short_description(sd),
	description(tfel::utilities::tokenize(d,"@^separator^@")),
	latex_description(tfel::utilities::tokenize(l,"@^separator^@")),
	notes(tfel::utilities::tokenize(no,"@^separator^@"))
    {
      using namespace std;
      if(!(t=="scalar")&&(t=="vector")&&(t=="symmetric tensor")){
	string msg("GlossaryTokenizer::execute : "
		   "unsupported type '"+t+"'");
	throw(runtime_error(msg));
      }
    }

    const std::string&
    GlossaryEntry::getKey(void) const
    {
      return this->key;
    }

    const std::string&
    GlossaryEntry::getName(void) const
    {
      return this->name;
    }

    const std::string&
    GlossaryEntry::getUnit(void) const
    {
      return this->unit;
    }

    const std::string&
    GlossaryEntry::getType(void) const
    {
      return this->type;
    }

    const std::string&
    GlossaryEntry::getShortDescription(void) const
    {
      return this->short_description;
    }

    const std::vector<std::string>&
    GlossaryEntry::getDescription(void) const
    {
      return this->description;
    }

    const std::vector<std::string>&
    GlossaryEntry::getLaTeXDescription(void) const
    {
      if(!this->latex_description.empty()){
	return this->latex_description;
      }
      return this->description;
    }

    const std::vector<std::string>&
    GlossaryEntry::getNotes(void) const
    {
      return this->notes;
    }

    GlossaryEntry::operator const std::string& () const
    {
      return this->name;
    } // end of operator std::string

    bool
    GlossaryEntry::operator<(const GlossaryEntry& e) const
    {
      return this->name < e.name;
    }

  } // end of namespace glossary

} // end of namespace tfel

