/*!
 * \file   src/Utilities/CxxTokenizer.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   12 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cctype>
#include<sstream>
#include<iterator>
#include<stdexcept>
#include<algorithm>

#include"TFEL/Utilities/CxxKeywords.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"

namespace tfel{

  namespace utilities{

    std::vector<std::string>
    CxxTokenizer::splitStringAtSpaces(const std::string& str)
    {
      using namespace std;
      istringstream s(str);
      vector<string> res;
      copy(istream_iterator<string>(s),istream_iterator<string>(),
	   back_inserter(res));
      return res;
    }  

    static std::string::size_type
    findStringBeginning(const std::string& s,
			const char b)
    {
      using namespace std;
      string::size_type p;
      if(s.empty()){
	return string::npos;
      }
      if(s[0]==b){
	return 0u;
      }
      p = s.find(b);
      while(p!=string::npos){
	if(s[p-1]!='\\'){
	  return p;
	}
	p = s.find(b,p+1);
      }
      return string::npos;
    } // end of findStringBeginning

    static std::string
    stripSpaceAndStarAtBeginningOfCommentLine(const std::string& l)
    {
      using namespace std;
      if(!l.empty()){
	string::size_type pc = 0;
	bool found = false;
	while((pc!=l.size())&&!(found)){
	  if(!(isspace(l[pc]))){
	    found = true;
	  } else {
	    ++pc;
	  }
	}
	if(found){
	  if(l[pc]=='*'){
	    ++pc;
	    found = false;
	    while((pc!=l.size())&&!(found)){
	      if(!(isspace(l[pc]))){
		found = true;
	      } else {
		++pc;
	      }
	    }
	    if(found){
	      return l.substr(pc);
	    } else {
	      return "";
	    }
	  }
	  return l.substr(pc);
	}
      }
      return "";
    }
    
    void
    CxxTokenizer::splitLine(std::string line, const unsigned int lineNumber)
    {
      using namespace std;
      if(this->cStyleCommentOpened){
	string::size_type pos3;
	pos3 = line.find("*/");
	if((this->fileTokens.empty())||
	   ((this->fileTokens.back().flag!=Token::Comment)&&
	    (this->fileTokens.back().flag!=Token::DoxygenComment)&&
	    (this->fileTokens.back().flag!=Token::DoxygenBackwardComment))){
	  string msg("CxxTokenizer::splitLine : internal error (previous token "
		     "is not a comment)");
	  throw(runtime_error(msg));
	}
	if(!this->fileTokens.back().value.empty()){
	  this->fileTokens.back().value += '\n';
	}
	if(pos3==string::npos){
	  this->fileTokens.back().value += 
	    stripSpaceAndStarAtBeginningOfCommentLine(line);
	  return;
	}
	this->fileTokens.back().value +=
	  stripSpaceAndStarAtBeginningOfCommentLine(line.substr(0,pos3));
	line.erase(0,pos3+2);
	this->cStyleCommentOpened=false;
      }

      while(!line.empty()){
	bool treatString=false;
	bool treatCppComment=false;
	bool treatCComment=false;
	bool treatChar=false;
	string::size_type pos[4];
	const string::size_type * min_pos;

	pos[0] = findStringBeginning(line,'"');
	pos[1] = line.find("//");
	pos[2] = line.find("/*");
	pos[3] = findStringBeginning(line,'\'');
      
	min_pos=min_element(pos,pos+4);

	if(*min_pos!=string::npos){
	  ptrdiff_t diff = min_pos-pos;
	  if(diff==0){
	    treatString=true;
	  }
	  if(diff==1){
	    treatCppComment=true;
	  }
	  if(diff==2){
	    treatCComment=true;
	  }
	  if(diff==3){
	    treatChar = true;
	  }
	}

	if(treatString){
	  if(pos[0]!=0){
	    vector<string> tmp=splitStringAtSpaces(line.substr(0,pos[0]));
	    vector<string>::const_iterator p;
	    for(p=tmp.begin();p!=tmp.end();++p){
	      this->fileTokens.push_back(Token(lineNumber,*p));
	    }
	  }
	  line.erase(0,pos[0]);
	  string::iterator ps = line.begin();
	  ++ps;
	  bool found=false;
	  for(;(ps!=line.end())&&(!found);++ps){
	    if(*ps=='\"'){
	      string::const_reverse_iterator ps2(ps);
	      const string::const_reverse_iterator pse(line.rend());
	      found=true;
	      while((ps2!=pse)&&(*ps2=='\\')){
		found = !found;
		++ps2;
	      }
	    }
	  }
	  if(!found){
	    string msg("CxxTokenizer::splitLine : ");
	    msg += "found no matching \" to close string\n";
	    msg += "Error at line : ";
	    msg += to_string(lineNumber);
	    throw(runtime_error(msg));
	  }
	  if(!this->fileTokens.empty()){
	    if(this->fileTokens.back().flag==Token::String){
	      const string old_value = this->fileTokens.back().value.substr(0,this->fileTokens.back().value.size()-1);
	      const string new_value = string(line.begin()+1,ps);
	      this->fileTokens.back().value = old_value+new_value;
	    } else {
	      this->fileTokens.push_back(Token(lineNumber,string(line.begin(),ps),Token::String));
	    }
	  } else {
	    this->fileTokens.push_back(Token(lineNumber,string(line.begin(),ps),Token::String));
	  }
	  line.erase(line.begin(),ps);
	} else if (treatCppComment){
	  if(pos[1]!=0){
	    vector<string> tmp=splitStringAtSpaces(line.substr(0,pos[1]));
	    vector<string>::const_iterator p;
	    for(p=tmp.begin();p!=tmp.end();++p){
	      this->fileTokens.push_back(Token(lineNumber,*p));
	    }
	  }
	  line.erase(0,pos[1]);
	  if((line.size()>=3u)&&((line[2]=='/')||(line[2]=='!'))){
	    // doxygen comment
	    if((line.size()>=4u)&&(line[3]=='<')){
	      // doxygen backward comment
	      if(this->fileTokens.empty()){
		this->fileTokens.push_back(Token(lineNumber,stripSpaceAndStarAtBeginningOfCommentLine(line),
						 Token::Comment));
	      } else if((this->fileTokens.back().line!=lineNumber)||
			((this->fileTokens.back().flag!=Token::Standard)||
			 (*(this->fileTokens.back().value.rbegin())!=';'))){
		this->fileTokens.push_back(Token(lineNumber,stripSpaceAndStarAtBeginningOfCommentLine(line.substr(3)),
						 Token::Comment));
	      } else {
		this->fileTokens.push_back(Token(lineNumber,stripSpaceAndStarAtBeginningOfCommentLine(line.substr(4)),
						 Token::DoxygenBackwardComment));
	      }
	    } else {
	      // standard doxygen command
	      if(this->fileTokens.back().flag==Token::DoxygenComment){
		if(!this->fileTokens.back().value.empty()){
		  this->fileTokens.back().value += '\n';
		}
		this->fileTokens.back().value += stripSpaceAndStarAtBeginningOfCommentLine(line.substr(3));
	      } else {
		this->fileTokens.push_back(Token(lineNumber,stripSpaceAndStarAtBeginningOfCommentLine(line.substr(3)),
						 Token::DoxygenComment));
	      }
	    }
	  } else {
	    // standard C++ comment
	    this->fileTokens.push_back(Token(lineNumber,stripSpaceAndStarAtBeginningOfCommentLine(line),
					     Token::Comment));
	  }
	  line.clear();
	} else if(treatCComment){
	  if(pos[2]!=0){
	    vector<string> tmp = splitStringAtSpaces(line.substr(0,pos[2]));
	    vector<string>::const_iterator p;
	    for(p=tmp.begin();p!=tmp.end();++p){
	      this->fileTokens.push_back(Token(lineNumber,*p));
	    }
	  }
	  line.erase(0,pos[2]);
	  pos[2]=line.find("*/");
	  string comment;
	  if(pos[2]==string::npos){
	    comment = line.substr(2);
	    line.clear();
	    this->cStyleCommentOpened=true;  
	  } else {
	    comment = line.substr(2,pos[2]-2);
	    line.erase(0,pos[2]+2);
	  }
	  if((comment.size()>=1)&&((comment[0]=='*')||(comment[0]=='!'))){
	    // doxygen comment
	    if((comment.size()>=2)&&((comment[1]=='<'))){
	      // backward doxygen comment
	      if(this->fileTokens.empty()){
		this->fileTokens.push_back(Token(lineNumber,
						 stripSpaceAndStarAtBeginningOfCommentLine(comment.substr(2)),
						 Token::Comment));
	      } else if((this->fileTokens.back().line!=lineNumber)||
			((this->fileTokens.back().flag!=Token::Standard)||
			 (*(this->fileTokens.back().value.rbegin())!=';'))){
		this->fileTokens.push_back(Token(lineNumber,stripSpaceAndStarAtBeginningOfCommentLine(comment.substr(2)),
						 Token::Comment));
	      } else {
		this->fileTokens.push_back(Token(lineNumber,stripSpaceAndStarAtBeginningOfCommentLine(comment.substr(2)),
						 Token::DoxygenBackwardComment));
	      }
	    } else {
	      this->fileTokens.push_back(Token(lineNumber,stripSpaceAndStarAtBeginningOfCommentLine(comment.substr(1)),
					       Token::DoxygenComment));
	    }
	  } else {
	    // standard C++ comment
	    this->fileTokens.push_back(Token(lineNumber,stripSpaceAndStarAtBeginningOfCommentLine(comment)
					     ,Token::Comment));
	  }
	} else if(treatChar){
	  if(this->charAsString){
	    if(pos[3]!=0){
	      vector<string> tmp=splitStringAtSpaces(line.substr(0,pos[3]));
	      vector<string>::const_iterator p;
	      for(p=tmp.begin();p!=tmp.end();++p){
		this->fileTokens.push_back(Token(lineNumber,*p));
	      }
	    }
	    line.erase(0,pos[3]);
	    string::iterator ps = line.begin();
	    ++ps;
	    bool found=false;
	    for(;(ps!=line.end())&&(!found);++ps){
	      if((*ps=='\'')&&(*(ps-1)!='\\')){
		found=true;
	      }
	    }
	    if(!found){
	      string msg("CxxTokenizer::splitLine : ");
	      msg += "found no matching \' to close string\n";
	      msg += "Error at line : ";
	      msg += to_string(lineNumber);
	      throw(runtime_error(msg));
	    }
	    this->fileTokens.push_back(Token(lineNumber,string(line.begin(),ps),Token::String));
	    line.erase(line.begin(),ps);
	  } else {
	    if(pos[3]!=0){
	      vector<string> tmp = splitStringAtSpaces(line.substr(0,pos[3]));
	      vector<string>::const_iterator p;
	      for(p=tmp.begin();p!=tmp.end();++p){
		this->fileTokens.push_back(Token(lineNumber,*p));
	      }
	    }
	    line.erase(0,pos[3]);
	    if(line.length()<3){
	      string msg("CxxTokenizer::splitString : ");
	      msg += "error while reading char (1)\n";
	      msg += "Error at line : ";
	      msg += to_string(lineNumber);
	      throw(runtime_error(msg));
	    }
	    if(line[1]=='\\'){
	      if(line.length()<4){
		string msg("CxxTokenizer::splitString : ");
		msg += "error while reading char (2)\n";
		msg += "Error at line : ";
		msg += to_string(lineNumber);
		throw(runtime_error(msg));
	      }
	      if(line[3]!='\''){
		string msg("CxxTokenizer::splitString : ");
		msg += "error while reading char (3)\n";
		msg += "Error at line : ";
		msg += to_string(lineNumber);
		throw(runtime_error(msg));	      
	      }
	      this->fileTokens.push_back(Token(lineNumber,line.substr(0,4),Token::Char));
	      line.erase(0,4);
	    } else {
	      if(line[2]!='\''){
		string msg("CxxTokenizer::splitString : ");
		msg += "error while reading char (expected to read ', read '";
		msg += line[2];
		msg += "')\n";
		msg += "Error at line : ";
		msg += to_string(lineNumber);
		throw(runtime_error(msg));
	      }
	      this->fileTokens.push_back(Token(lineNumber,line.substr(0,3),Token::Char));
	      line.erase(0,3);
	    }
	  }
	} else {
	  vector<string> tmp = splitStringAtSpaces(line);
	  vector<string>::const_iterator p;
	  for(p=tmp.begin();p!=tmp.end();++p){
	    this->fileTokens.push_back(Token(lineNumber,*p));
	  }
	  line.clear();
	}
      }
    }

    std::string
    CxxTokenizer::readNumber(std::string::const_iterator& p,
			     const std::string::const_iterator  pe)
    {
      using namespace std;
      string word;
      string::const_iterator b=p;
      if((*p=='-')||(*p=='+')){
	++p;
      }
      //reading decimal part
      while((isdigit(*p))&&(p!=pe)){
	++p;
      }
      if(p!=pe){
	if(*p=='.'){
	  ++p;
	  while((isdigit(*p))&&(p!=pe)){
	    ++p;
	  }
	}
      }
      if(p!=pe){
	if((*p=='e')||(*p=='E')){
	  ++p;
	  if(p==pe){
	    --p;
	  } else {
	    if((*p=='+')||(*p=='-')){
	      ++p;
	      if(p==pe){
		--(--p);
	      } else if (isdigit(*p)){
		while((isdigit(*p))&&(p!=pe)){
		  ++p;
		}
	      } else {
		--(--p);
	      }
	    } else if (isdigit(*p)){
	      while((isdigit(*p))&&(p!=pe)){
		++p;
	      }
	    } else {
	      --p;
	    }
	  }
	}
      }
      if(p!=pe){
	if(*p=='f'){
	  ++p;
	}
      }
      copy(b,p,back_inserter(word));
      return word;
    } // end of CxxTokenizer::readNumber

    void
    CxxTokenizer::extractNumbers(std::vector<std::string>& res,
				 const std::string& s)
    {
      using namespace std;
      string::const_iterator p = s.begin();
      const string::const_iterator pe = s.end();
      char buf[3];
      buf[1] = '\0';
      buf[2] = '\0';
      while(p!=pe){
	if(*p=='.'){
	  if(p==s.begin()){
	    if(s.size()!=1){
	      ++p;
	      if(isdigit(*p)){
		--p;
		res.push_back(CxxTokenizer::readNumber(p,pe));
	      } else {
		--p;
		buf[0] = *p;
		res.push_back(buf);
		++p;
	      }
	    } else {
	      buf[0] = *p;
	      res.push_back(buf);
	      ++p;
	    }
	  }else{
	    buf[0] = *p;
	    res.push_back(buf);
	    ++p;
	  }
	} else if((*p=='+')||(*p=='-')){
	  if(p==s.begin()){
	    if(s.size()!=1){
	      ++p;
	      if(isdigit(*p)){
		--p;
		res.push_back(CxxTokenizer::readNumber(p,pe));
		
	      } else if (*p=='='){
		--p;
		buf[0] = *p;
		buf[1] = '=';
		res.push_back(buf);
		buf[1] = '\0';
		++(++p);
	      }else {
		--p;
		buf[0] = *p;
		res.push_back(buf);
		++p;
	      }
	    } else {
	      buf[0] = *p;
	      res.push_back(buf);
	      ++p;
	    }
	  }else{
	    buf[0] = *p;
	    res.push_back(buf);
	    ++p;
	  }
	} else if(isdigit(*p)){
	  res.push_back(CxxTokenizer::readNumber(p,pe));
	} else {
	  string::const_iterator pp = find(p,pe,'+');
	  string::const_iterator pm = find(p,pe,'-');
	  string::const_iterator pd = find(p,pe,'.');
	  string word;
	  if((pp==pe)&&(pm==pe)&&(pd==pe)){
	    // this is the end
	    copy(p,pe,back_inserter(word));
	    res.push_back(word);
	    p=pe;	      
	  } else {
	    if((pp-p<pm-p)&&(pp-p<pd-p)){
	      // next one is '+'
	      copy(p,pp,back_inserter(word));
	      p=pp;
	    } else if((pm-p<pp-p)&&(pm-p<pd-p)){
	      // next one is '-'
	      copy(p,pm,back_inserter(word));
	      p=pm;
	    } else {
	      // next one is '.'
	      copy(p,pd,back_inserter(word));
	      p=pd;
	    }
	    res.push_back(word);
	    buf[0] = *p;
	    res.push_back(buf);
	    ++p;
	  }
	}
      }
    } // end of CxxTokenizer::extractNumbers

    void
    CxxTokenizer::splitString(std::vector<std::string>& res,
			      std::string s, const std::string delim)
    {
      using namespace std;
      string tmp;
      string::size_type posb = 0;
      while((!s.empty())&&(posb!=string::npos)){
	posb = s.find(delim);
	if(posb!=0){
	  res.push_back(string(s,0,posb));
	}
	if(posb!=string::npos){
	  res.push_back(delim);
	  s.erase(0,posb+delim.length());
	} else {
	  s.erase(0,posb);
	}
      }
    }

    void
    CxxTokenizer::splitString2(std::vector<std::string>& res,std::string s)
    {
      using namespace std;
      string tmp;
      string::size_type posb = 0;
      while((!s.empty())&&(posb!=string::npos)){
	posb = s.find(">");
	if(posb!=0){
	  res.push_back(string(s,0,posb));
	}
	if(posb!=string::npos){
	  if((posb+1<s.length())&&(s[posb+1]=='>')){
	    res.push_back(">>");
	    s.erase(0,posb+2);
	    ++posb;
	  } else {
	    res.push_back(">");
	    s.erase(0,posb+1);
	  }
	} else {
	  s.erase(0,posb);
	}
      }
    }

    void
    CxxTokenizer::splitString3(std::vector<std::string>& res,std::string s)
    {
      using namespace std;
      string tmp;
      string::size_type posb = 0;
      while((!s.empty())&&(posb!=string::npos)){
	posb = s.find("<");
	if(posb!=0){
	  res.push_back(string(s,0,posb));
	}
	if(posb!=string::npos){
	  if((posb+1<s.length())&&(s[posb+1]=='<')){
	    res.push_back("<<");
	    s.erase(0,posb+2);
	    ++posb;
	  } else {
	    res.push_back("<");
	    s.erase(0,posb+1);
	  }
	} else {
	  s.erase(0,posb);
	}
      }
    }

    void 
    CxxTokenizer::join(std::vector<std::string>& src,
		       const std::string& s1,
		       const std::string& s2)
    {
      using namespace std;
      vector<string> res;
      vector<string>::iterator p;

      if(src.empty()){
	return;
      }
    
      p=src.begin();
      res.push_back(*p);
      ++p;
      for(;p!=src.end();++p){
	if((res.back()==s1)&&(*p==s2)){
	  res.back().append(s2);
	} else {
	  res.push_back(*p);
	}
      }

      src.swap(res);

    } // end of CxxTokenizer::join

    void
    CxxTokenizer::splitAtCxxTokenSperator(std::vector<std::string>& tokens)
    {
      using namespace std;
      // '.' deserves a special treatement.
      // '<' and '>' deserve special treatements.
      constexpr char Separator[20] = {'?',';','/','!','&','*',
				      '|','{','}','[',']','(',
				      ')','%','=','^',',',':',
				      '<','>'};
      vector<string> res(tokens);
      vector<string> init;
      vector<string>::const_iterator p;
      unsigned int i;
      char buf[2] = {'\0','\0'};
      for(i=0;i<sizeof(Separator)/sizeof(char);++i){
	switch(Separator[i]){
	case '<' :
	  // Special care is taken for '<' as "< <" and "<<" are both 
	  // valid keywords 
	  init.swap(res);
	  res.clear();    
	  for(p=init.begin();p!=init.end();++p){
	    if(p->find("<")!=string::npos){
	      vector<string> v;
	      splitString3(v,*p);
	      res.insert(res.end(),v.begin(),v.end());
	    } else {
	      res.push_back(*p);
	    }
	  }
	  break;
	case '>' :
	  init.swap(res);
	  res.clear();    
	  for(p=init.begin();p!=init.end();++p){
	    if(p->find(">")!=string::npos){
	      vector<string> v;
	      splitString2(v,*p);
	      res.insert(res.end(),v.begin(),v.end());
	    } else {
	      res.push_back(*p);
	    }
	  }
	  break;
	default:
	  init.swap(res);
	  res.clear();
	  for(p=init.begin();p!=init.end();++p){
	    if(p->find(Separator[i])!=string::npos){
	      vector<string> v;
	      buf[0] = Separator[i];
	      splitString(v,*p,buf);
	      res.insert(res.end(),v.begin(),v.end());
	    } else {
	      res.push_back(*p);
	    }
	  }
	}

      }

      if(this->bExtractNumbers){
	init.swap(res);
	res.clear();
	for(p=init.begin();p!=init.end();++p){
	  if((p->find("+")!=string::npos)||
	     (p->find("-")!=string::npos)||
	     (p->find(".")!=string::npos)){
	    vector<string> v;
	    CxxTokenizer::extractNumbers(v,*p);
	    res.insert(res.end(),v.begin(),v.end());
	  } else {
	    res.push_back(*p);
	  }
	}
      }

      join(res,"/","/"); 
      join(res,"/","*");    
      join(res,"*","/");
      join(res,":",":");
      join(res,"-",">");
      join(res,"<","<");
      join(res,"<","=");
      join(res,">","=");
      join(res,"+","+");
      join(res,"-","-");
      join(res,"+","=");
      join(res,"-","=");
      join(res,"*","=");
      join(res,"/","=");
      join(res,"%","=");
      join(res,"|","=");
      join(res,"!","=");
      join(res,"&","&");
      join(res,"=","=");
      join(res,".","*");
      join(res,"->","*");    
      join(res,"|","|");

      tokens.swap(res);

    }

    void
    CxxTokenizer::stripComments(void){
      using namespace std;
      TokensContainer::iterator p;    
      for(p=this->fileTokens.begin();p!=this->fileTokens.end();++p){
	if(p->flag==Token::Comment){
	  if(this->comments.find(p->line)!=this->comments.end()){
	    this->comments[p->line] += ' ';
	  }
	  this->comments[p->line] = p->value;
	  p = --(this->fileTokens.erase(p));
	}
	if(p->flag==Token::DoxygenComment){
	  TokensContainer::iterator p2 = p;
	  ++p2;
	  if(p2!=this->fileTokens.end()){
	    if(p2->flag==Token::Standard){
	      if(!p2->comment.empty()){
		p2->comment += '\n';
	      }
	      p2->comment += p->value;
	    }
	    if(p2->flag==Token::DoxygenComment){
	      p2->value = p->value+"\n"+p2->value;
	    }
	  }
	  p = --(this->fileTokens.erase(p));
	}
	if(p->flag==Token::DoxygenBackwardComment){
	  TokensContainer::iterator p2 = p;
	  --p2;
	  if(p2!=this->fileTokens.begin()){
	    if(p2->flag==Token::Standard){
	      p2->comment += p->value;
	    }
	  }
	  p = --(this->fileTokens.erase(p));
	}
      }
    } // end of CxxTokenizer::stripComments

    void 
    CxxTokenizer::splitTokens(void)
    {
      using namespace std;
      TokensContainer::iterator p;
      for(p=this->fileTokens.begin();p!=this->fileTokens.end();++p){
	if(p->flag==Token::Standard){
	  vector<string> splittedTokens;
	  splittedTokens.push_back(p->value);
	  splitAtCxxTokenSperator(splittedTokens);
	  unsigned int line = p->line;
	  vector<string>::iterator p2 = splittedTokens.begin();
	  TokensContainer::iterator current=p;
	  for(;p2!=splittedTokens.end();++p2){
	    p = this->fileTokens.insert(current,Token(line,*p2));
	  }
	  this->fileTokens.erase(current);
	}
      }
    } // end of CxxTokenizer::splitStandardTokens
  
    void
    CxxTokenizer::openFile(const std::string& f)
    {
      using namespace std;

      this->fileTokens.clear();

      string line;
      ifstream file(f.c_str());
      unsigned int lineNumber;

      if(!file){
	string msg("CxxTokenizer::openFile : ");
	msg += "unable to open file '"+f+"'";
	throw(runtime_error(msg));
      }

      lineNumber=0;
      while(!file.eof()){
	if(!file.good()){
	  string msg("CxxTokenizer::openFile : ");
	  msg+="error while reading file '"+f+"'";
	  throw(runtime_error(msg));
	}
	getline(file,line);
	++lineNumber;
	if(!line.empty()){
	  this->splitLine(line,lineNumber);
	}
      }

      this->treatPreprocessorDirectives();
      this->splitTokens();
      file.close();
    }

    void
    CxxTokenizer::parseString(const std::string& s)
    {
      this->fileTokens.clear();
      this->splitLine(s,0u);
      this->treatPreprocessorDirectives();
      this->splitTokens();
    } // end of CxxTokenizer::parseOneString

    CxxTokenizer::CxxTokenizer()
      : cStyleCommentOpened(false),
	bExtractNumbers(true),
	charAsString(false)
    {}
  
    CxxTokenizer::CxxTokenizer(const std::string& f)
      : cStyleCommentOpened(false),
	bExtractNumbers(true),
	charAsString(false)
    {
      this->openFile(f);
    }

    void
    CxxTokenizer::treatCharAsString(const bool b){
      this->charAsString = b;
    } // end of CxxTokenizer::treatCharAsString

    void
    CxxTokenizer::extractNumbers(const bool b){
      this->bExtractNumbers = b;
    } // end of CxxTokenizer::extractNumbers

    void
    CxxTokenizer::treatPreprocessorDirectives(void)
    {
      using namespace std;
      constexpr const char * CppKeywords[11] = {"#define","#undef","#include",
						"#line","#error","#if","#ifdef",
						"#ifndef","elif","#else","#endif"};
      constexpr unsigned short CppKeywordsNumber = 11;

      unsigned short i;
      TokensContainer::iterator p;
      string::size_type pos;
      for(i=0;i<CppKeywordsNumber;++i){
	for(p=this->fileTokens.begin();p!=this->fileTokens.end();++p){
	  if(p->flag==Token::Standard){
	    pos=p->value.find(CppKeywords[i]);
	    if(pos!=string::npos){
	      TokensContainer::iterator b=p;
	      TokensContainer::iterator current;
	      b->flag=Token::Preprocessor;
	      ++p;
	      if(p==this->fileTokens.end()){
		--p;
	      } else if(p->line!=b->line){
		--p;
	      } else {
		while((p->line==b->line)&&
		      (p!=this->fileTokens.end())){
		  current = p;
		  ++p;
		  if((current->flag!=Token::Comment)&&
		     (current->flag!=Token::DoxygenComment)&&
		     (current->flag!=Token::DoxygenBackwardComment)){
		    b->value.append(" ");
		    b->value.append(current->value);
		    this->fileTokens.erase(current);
		  }
		}
		--p;
	      }
	    }
	  }
	}
      }
    } // end of CxxTokenizer::treatPreprocessorDirectives

    void 
    CxxTokenizer::printFileTokens(std::ostream& out) const
    {
      using namespace std;
      const_iterator p;
      p=this->fileTokens.begin();
      unsigned int line = p->line;
      for(p=this->fileTokens.begin();p!=this->fileTokens.end();++p){
	if(p->line!=line){
	  out << endl;
	  line = p->line;
	}
	out << p->value << " ";
      }
      out << endl;
    }

    bool
    CxxTokenizer::isValidIdentifier(const std::string& s, 
				    const bool b)
    {
      using namespace std;
      if(s.empty()){
	return false;
      }
      string::const_iterator p = s.begin();
      if(isdigit(*p)){
	return false;
      }
      for(;p!=s.end();++p){
	if((!isalpha(*p))&&
	   (!(isdigit(*p)))&&
	   (*p!='_')){
	  return false;
	}
	if(isspace(*p)){
	  return false;
	}
      }

      if(b){
	if(isReservedCxxKeywords(s)){
	  return false;
	}
      }
      return true;
    } // end of CxxTokenizer::isValidIdentifier

    bool
    CxxTokenizer::isValidFunctionIdentifier(const std::string& s, 
					    const bool checkCxxKeywords)
    {
      using namespace std;
      if(s.empty()){
	return false;
      }
      string::const_iterator p = s.begin();
      if(isdigit(*p)){
	return false;
      }
      for(;p!=s.end();++p){
	if((!isalpha(*p))&&
	   (!(isdigit(*p)))&&
	   (*p!='_')&&(*p!=':')){
	  return false;
	}
	if(isspace(*p)){
	  return false;
	}
	if(*p==':'){
	  ++p;
	  if(p==s.end()){
	    return false;
	  }
	  if(*p!=':'){
	    return false;
	  }
	  ++p;
	  if(p==s.end()){
	    return false;
	  }
	  if((!isalpha(*p))&&
	     (!(isdigit(*p)))&&
	     (*p!='_')){
	    return false;
	  }
	}
      }

      if(checkCxxKeywords){
	if(isReservedCxxKeywords(s)){
	  return false;
	}
      }

      return true;

    } // end of CxxTokenizer::isValidFunctionIdentifier

    CxxTokenizer::TokensContainer::iterator
    CxxTokenizer::joinPreviousCurrentNext(CxxTokenizer::TokensContainer& res,
					  CxxTokenizer::TokensContainer& src,
					  CxxTokenizer::TokensContainer::iterator p)
    {
      using namespace std;
      TokensContainer::iterator p2;
      TokensContainer::iterator a;
      TokensContainer::iterator b;
      bool previous=false;
      bool next=false;
      if(p==src.end()){
	return p;
      }
      b=p;
      ++b;
      if(p!=src.begin()){
	a=p;
	--a;
	if(a->line==p->line){
	  previous=true;
	}
      }
      if(b!=src.end()){
	if(b->line==p->line){
	  next=true;
	}
      }
      if(res.empty()){
	string msg("CxxTokenizer::joinPreviousCurrentNext : ");
	msg += "internal error \"::\"\n";
	msg += "Error at line : ";
	msg += to_string(p->line);
	throw(runtime_error(msg));
      }
      if(previous&&next){
	res.back().value.append(p->value+b->value);
	return b;
      }
      if(previous){
	res.back().value.append(p->value);
	return p;
      }
      if(next){
	res.push_back(*p);
	res.back().value.append(b->value);
	return b;
      }
      res.push_back(*p);
      return p;    
    } // end of CxxTokenizer::joinPreviousCurrentNext

    void 
    CxxTokenizer::beautifyCode(void)
    {
      using namespace std;
      TokensContainer res;
      unsigned int indent=0;
      unsigned int current_line=0;
      for(auto p=this->fileTokens.begin();p!=this->fileTokens.end();++p){
	if(p->flag==Token::Standard){
	  if(p->value=="}"){
	    if(indent<2){
	      string msg("CxxTokenizer::beautifyCode : ");
	      msg += "Found unmatched closing bracket \"}\"\n";
	      msg += "Error at line : ";
	      msg += to_string(p->line);
	      msg += "\n";
	      msg += "indent = ";
	      msg += to_string(indent);
	      throw(runtime_error(msg));
	    } else{
	      --indent;
	      --indent;
	    }
	  }
	  if(p->value=="{"){
	    ++indent;
	    ++indent;
	  }
	  if((p->line!=current_line)){
	    if(indent!=0){
	      if(p->value=="{"){
		p->value.insert(0u,indent-2,' ');
		current_line = p->line;
	      } else {
		p->value.insert(0u,indent,' ');
		current_line = p->line;
	      }
	    }
	  }
	  if(p->value=="::"){
	    p=joinPreviousCurrentNext(res,this->fileTokens,p);
	  } else if(p->value=="->"){
	    p=joinPreviousCurrentNext(res,this->fileTokens,p);
	  } else if(p->value=="."){
	    p=joinPreviousCurrentNext(res,this->fileTokens,p);
	  } else if(p->value=="<"){
	    p=joinPreviousCurrentNext(res,this->fileTokens,p);
	  } else {
	    res.push_back(*p);
	  }
	} else {
	  res.push_back(*p);
	}
      }
      if(indent!=0){
	string msg("CxxTokenizer::beautifyCode : ");
	msg += "File ended with unmatched closing bracket \"}\"\n";
	msg += "indent : ";
	msg += to_string(indent);
	throw(runtime_error(msg));
      }
    
      this->fileTokens.swap(res);

    } // end of CxxTokenizer::beautifyCode

    CxxTokenizer::const_iterator
    CxxTokenizer::begin(void) const
    {
      return this->fileTokens.begin();
    } // end of CxxTokenizer::begin

    CxxTokenizer::const_iterator
    CxxTokenizer::end(void) const
    {
      return this->fileTokens.end();
    } // end of CxxTokenizer::end

    void
    CxxTokenizer::clear(void)
    {
      this->cStyleCommentOpened = false;
      this->fileTokens.clear();
    } // end of CxxTokenizer::end

    void
    CxxTokenizer::checkNotEndOfLine(const std::string& method,
				    CxxTokenizer::const_iterator& p, 
				    const CxxTokenizer::const_iterator pe)
    {
      using namespace std;
      if(p==pe){
	string msg(method);
	msg += " : unexpected end of line";
	throw(runtime_error(msg));
      }
    } // end of CxxTokenizer::checkNotEndOfLine
    
    void
    CxxTokenizer::checkNotEndOfLine(const std::string& method,
				    const std::string& error,
				    CxxTokenizer::const_iterator& p, 
				    const CxxTokenizer::const_iterator pe)
    {
      using namespace std;
      if(p==pe){
	string msg(method);
	msg += " : unexpected end of line";
	if(!error.empty()){
	  msg += " ("+error+")";
	}
	throw(runtime_error(msg));
      }
    } // end of CxxTokenizer::checkNotEndOfLine
    
    void
    CxxTokenizer::readSpecifiedToken(const std::string& method,
				     const std::string& value,
				     CxxTokenizer::const_iterator& p, 
				     const CxxTokenizer::const_iterator pe)
    {
      using namespace std;
      CxxTokenizer::checkNotEndOfLine(method,"expected '"+value+"'",p,pe);
      if(p->value!=value){
	string msg(method);
	msg += " : unexpected token '"+p->value+"'";
	msg += " (expected '"+value+"')";
	throw(runtime_error(msg));
      }
      ++p;
    } // end of CxxTokenizer::readSpecifiedToken

    std::string
    CxxTokenizer::readString(CxxTokenizer::const_iterator& p, 
			     const CxxTokenizer::const_iterator pe)
    {
      using namespace std;
      using namespace tfel::utilities;
      CxxTokenizer::checkNotEndOfLine("CxxTokenizer::readString","",p,pe);
      if(p->flag!=Token::String){
	string msg("CxxTokenizer::readString : ");
	msg += "expected to read a string (read '"+p->value+"').\n";
	throw(runtime_error(msg));
      }
      if(p->value.size()<2){
	string msg("CxxTokenizer::readString : ");
	msg += "internal error (invalid string size)";
	throw(runtime_error(msg));
      }
      string value = p->value.substr(1,p->value.size()-2);
      ++p;
      return value;
    } // end of CxxTokenizer::readString

    double
    CxxTokenizer::readDouble(CxxTokenizer::const_iterator& p, 
			     const CxxTokenizer::const_iterator pe)
      
    {
      using namespace std;
      double res;
      CxxTokenizer::checkNotEndOfLine("CxxTokenizer::readDouble","expected number",p,pe);
      istringstream is(p->value);
      is >> res;
      if(!is&&(!is.eof())){
	ostringstream msg;
	msg << "CxxTokenizer::readDouble : ";
	msg << "could not read value from token '"+p->value+"'.\n";
	throw(runtime_error(msg.str()));
      }
      ++p;
      return res;
    } // end of CxxTokenizer::readDouble

    int
    CxxTokenizer::readInt(CxxTokenizer::const_iterator& p, 
			     const CxxTokenizer::const_iterator pe)
      
    {
      using namespace std;
      int res;
      CxxTokenizer::checkNotEndOfLine("CxxTokenizer::readInt","expected number",p,pe);
      istringstream is(p->value);
      is >> res;
      if(!is&&(!is.eof())){
	ostringstream msg;
	msg << "CxxTokenizer::readInt : ";
	msg << "could not read value from token '"+p->value+"'.\n";
	throw(runtime_error(msg.str()));
      }
      ++p;
      return res;
    } // end of CxxTokenizer::readInt

    unsigned int
    CxxTokenizer::readUnsignedInt(CxxTokenizer::const_iterator& p, 
				  const CxxTokenizer::const_iterator pe)
    {
      using namespace std;
      unsigned int res;
      CxxTokenizer::checkNotEndOfLine("CxxTokenizer::readUnsignedInt","expected number",p,pe);
      istringstream is(p->value);
      is >> res;
      if(!is&&(!is.eof())){
	ostringstream msg;
	msg << "CxxTokenizer::readUnsignedInt : ";
	msg << "could not read value from token '"+p->value+"'.\n";
	throw(runtime_error(msg.str()));
      }
      ++p;
      return res;
    } // end of CxxTokenizer::readUnsignedInt

    void
    CxxTokenizer::readArray(const std::string& m,
			    std::vector<std::string>& v,
			    const_iterator& p, 
			    const const_iterator pe)
    {
      using namespace std;
      CxxTokenizer::readSpecifiedToken(m,"{",p,pe);
      CxxTokenizer::checkNotEndOfLine(m,p,pe);
      while(p->value!="}"){
	CxxTokenizer::checkNotEndOfLine(m,p,pe);
	if((p->value==",")||p->value=="}"||p->value=="{"){
	  string msg(m+" : unexpected token ',' or '}' or '{'");
	  throw(runtime_error(msg));
	}
	v.push_back(p->value);
	++p;
	CxxTokenizer::checkNotEndOfLine(m,p,pe);
	if(p->value!="}"){
	  CxxTokenizer::readSpecifiedToken(m,",",p,pe);
	  CxxTokenizer::checkNotEndOfLine(m,p,pe);
	  if(p->value==","){
	    string msg(m+" : unexpected token ',' or '}'");
	    throw(runtime_error(msg));
	  }
	}
      }
      CxxTokenizer::readSpecifiedToken(m,"}",p,pe);
    } // end of CxxTokenizer::readArray

    CxxTokenizer::size_type
    CxxTokenizer::size() const
    {
      return this->fileTokens.size();
    } // end of CxxTokenizer::size

    CxxTokenizer::~CxxTokenizer()
    {} // end of CxxTokenizer::~CxxTokenizer

  } // end of namespace utilities

} // end of namespace tfel
