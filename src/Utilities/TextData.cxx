/*!
 * \file   src/Utilities/TextData.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   13 Nov 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cassert>
#include<stdexcept>
#include<sstream>
#include<fstream>
#include<algorithm>
#include<iterator>
#include"TFEL/Utilities/TextData.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"

namespace tfel
{

  namespace utilities
  {

    TextData::TextData(const std::string& file,
		       const std::string& format)
    {
      using namespace std;
      auto get_legends = [](std::vector<std::string>& r,
			    const std::string& l){
	istringstream tokenizer(l);
	copy(istream_iterator<string>(tokenizer),
	     istream_iterator<string>(),back_inserter(r));
      };
      auto add_line = [](std::vector<Line>& vl,
			 const std::string& l,
			 const unsigned short n){
	Line nl;
	nl.nbr = n;
	vl.push_back(nl);
	istringstream tokenizer(l);
	copy(istream_iterator<string>(tokenizer),
	     istream_iterator<string>(),back_inserter(vl.back().tokens));
	if(vl.back().tokens.empty()){
	  vl.pop_back();
	}
      };
      bool firstLine;
      bool firstComments;
      unsigned short nbr;
      std::ifstream f{file};
      if(!f){
	throw(std::runtime_error("TextData::TextData: "
				 "can't open '"+file+'\''));
      }
      nbr=1;
      firstLine=true;
      firstComments = true;
      while(!f.eof()){
	string line;
	getline(f,line);
	if(line.empty()){
	  continue;
	}
	if(line[0]=='#'){
	  if(!firstComments){
	    continue;
	  }
	  if(format.empty()){
	    line.erase(line.begin());
	    if(firstLine){
	      get_legends(this->legends,line);
	    }
	    this->preamble.push_back(line);
	  }
	} else {
	  if(((format=="gnuplot")||(format=="alcyone"))&&(firstLine)){
	    get_legends(this->legends,line);
	    bool all_numbers = true;
	    for(const auto& l:this->legends){
	      try{
		convert<double>(l);
	      } catch(exception&){
		all_numbers = false;
	      }
	      if(!all_numbers){
		break;
	      }
	    }
	    if(all_numbers){
	      this->legends.clear();
	      add_line(this->lines,line,nbr);
	    }
	  } else {
	    add_line(this->lines,line,nbr);
	    firstComments = false;
	  }
	}
	firstLine = false;
	++nbr;
      }
    } // end of TextData::TextData
      
    const std::vector<std::string>&
    TextData::getLegends() const
    {
      return this->legends;
    } // end of TextData::getLegends

    const std::vector<std::string>&
    TextData::getPreamble() const
    {
      return this->preamble;
    } // end of TextData::getPreamble

    std::string
    TextData::getLegend(const unsigned short c) const
    {
      using namespace std;
      if(c==0){
	string msg("TextData::getLegend : ");
	msg += "invalid column index";
	throw(runtime_error(msg));
      }
      if(c>=this->legends.size()+1){
	return "";
      }
      auto c2 = c;
      return this->legends[--c2];
    } // end of TextData::getLegend

    unsigned short
    TextData::findColumn(const std::string& name) const
    {
      using namespace std;
      vector<string>::const_iterator p;
      p = find(this->legends.begin(),this->legends.end(),name);
      if(p==this->legends.end()){
	string msg("TextData::findColumn : ");
	msg += "no column named '"+name+"' found'.";
	throw(runtime_error(msg));
      }
      return static_cast<unsigned short>(p-this->legends.begin()+1);
    } // end of TextData::findColumn

    std::vector<double>
    TextData::getColumn(const unsigned short i) const
    {
      std::vector<double> tab;
      this->getColumn(tab,i);
      return tab;
    } // end of TextData::getColumn

    void
    TextData::getColumn(std::vector<double>& tab,
			const unsigned short i) const
    {
      tab.clear();
      tab.reserve(this->lines.size());
      // sanity check
      if(i==0u){
	throw(std::runtime_error("TextData::getColumn: "
				 "column '0' requested (column numbers begins at '1')."));
      }
      // treatment
      auto line=this->lines.begin();
      for(int j=0;line!=this->lines.end();++line,++j){
	assert(line->tokens.begin()!=line->tokens.end());
	if(line->tokens.size()<i){
	  std::ostringstream msg;
	  msg << "TextData::getColumn : line '" 
	      << line->nbr << "' "
	      << "does not have '" << i << "' columns.";
	  throw(std::runtime_error(msg.str()));
	}
	tab.push_back(this->readDouble(line->tokens.begin()+i-1u,line->nbr));
      }
    } // end of TextData::getColumn

    std::vector<TextData::Line>::const_iterator
    TextData::begin() const
    {
      return this->lines.begin();
    } // end of TextData::begin()

    std::vector<TextData::Line>::const_iterator
    TextData::end() const
    {
      return this->lines.end();
    } // end of TextData::end()
      
    double
    TextData::readDouble(const std::vector<std::string>::const_iterator current,
			 const unsigned short l) const
    {
      using namespace std;
      double res;
      istringstream is(*current);
      is >> res;
      if(!is&&(!is.eof())){
	ostringstream msg;
	msg << "TextData::readDouble : ";
	msg << "could not read value from token '"+*current+"'.\n";
	msg << "Error at line : " << l;
	throw(runtime_error(msg.str()));
      }
      return res;
    } // end of TextData::readDouble

    void
    TextData::skipLines(const unsigned short n)
    {
      using namespace std;
      auto p = this->lines. begin();
      while((p->nbr<=n+1)&&(p!=this->lines.end())){
	++p;
      }
      lines.erase(lines.begin(),p);
    }

  } // end of namespace utilities

} // end of namespace tfel

