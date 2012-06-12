/*!
 * \file   TextData.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   13 Nov 2007
 */

#include<iostream>
#include<cassert>
#include<stdexcept>
#include<sstream>
#include<fstream>
#include<algorithm>
#include<iterator>
#include"TFEL/Utilities/TextData.hxx"

namespace tfel
{

  namespace utilities
  {

    TextData::TextData(const std::string& file)
    {
      using namespace std;
      ifstream f(file.c_str());
      bool firstLine;
      unsigned short nbr;
      if(!f){
	string msg("TextData::TextData : ");
	msg += "can't open "+file+".";
	throw(runtime_error(msg));
      }
      nbr=1;
      firstLine=true;
      while(!f.eof()){
	string line;
	getline(f,line);
	if(!line.empty()){
	  if(line[0]=='#'){
	    if(firstLine){
	      line.erase(line.begin());
	      istringstream tokenizer(line);
	      copy(istream_iterator<string>(tokenizer),
		   istream_iterator<string>(),back_inserter(this->legends));
	    }
	  } else {
	    Line l;
	    l.nbr = nbr;
	    this->lines.push_back(l);
	    istringstream tokenizer(line);
	    copy(istream_iterator<string>(tokenizer),
		 istream_iterator<string>(),back_inserter(this->lines.back().tokens));
	    if(this->lines.back().tokens.empty()){
	      this->lines.pop_back();
	    }
	  }
	  firstLine = false;
	}
	++nbr;
      }
    } // end of TextData::TextData
      
    const std::vector<std::string>&
    TextData::getLegends() const
    {
      return this->legends;
    } // end of TextData::getLegends

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
      return this->legends[c-1];
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
      using namespace std;
      vector<double> tab;
      this->getColumn(tab,i);
      return tab;
    } // end of TextData::getColumn

    void
    TextData::getColumn(std::vector<double>& tab,
			const unsigned short i) const
    {
      using namespace std;
      tab.reserve(this->lines.size());
      int j;
      // current line
      vector<Line>::const_iterator line;
      // sanity check
      if(i==0u){
	string msg("TextData::getColumn : ");
	msg += "column '0' requested (column numbers begins at '1').";
	throw(runtime_error(msg));
      }
      // treatment
      for(line=this->lines.begin(),j=0;line!=this->lines.end();++line,++j){
	assert(line->tokens.begin()!=line->tokens.end());
	if(line->tokens.size()<i){
	  ostringstream msg;
	  msg << "TextData::getColumn : line '" 
	      << line->nbr << "' "
	      << "does not have '" << i << "' columns.";
	  throw(runtime_error(msg.str()));
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

  } // end of namespace utilities

} // end of namespace tfel

