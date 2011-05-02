/*! 
 * \file  StringAlgorithms.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mar 2011
 */

#include"TFEL/Utilities/StringAlgorithms.hxx"

namespace tfel
{

  namespace utilities
  {

    std::vector<std::string>
    tokenize(const std::string& s,
	     const char c)
    {
      using namespace std;
      vector<string> res;
      string::size_type b = 0u;
      string::size_type e = s.find_first_of(c, b);
      while (string::npos != e || string::npos != b){
	// Found a token, add it to the vector.
	res.push_back(s.substr(b, e - b));
	b = s.find_first_not_of(c, e);
	e = s.find_first_of(c, b);
      }
      return res;
    } // end of tokenize

    std::vector<std::string>
    tokenize(const std::string& s,
	     const std::string& delim)
    {
      using namespace std;
      vector<string> res;
      string::size_type b = 0u;
      string::size_type e = s.find_first_of(delim, b);
      while (string::npos != e || string::npos != b){
	// Found a token, add it to the vector.
	res.push_back(s.substr(b, e - b));
	b = s.find_first_not_of(delim, e);
	e = s.find_first_of(delim, b);
      }
      return res;
    } // end of tokenize

    std::string
    replace_all(const std::string& s,
		const std::string& s1,
		const std::string& s2,
		const std::string::size_type ps)
    {
      using namespace std;
      string r;
      replace_all(r,s,s1,s2,ps);
      return r;
    }
    
    void
    replace_all(std::string& r,
		const std::string& s,
		const std::string& s1,
		const std::string& s2,
		const std::string::size_type ps){
      using namespace std;
      string::size_type rs;
      string::size_type pos = ps;
      string::size_type p;
      r.clear();
      if(!s1.empty()){
	p=s.find(s1,pos);
	while(p!=string::npos){
	  rs = r.size();
	  r.resize(rs+p-pos+s2.size());
	  //	  copy(s.begin()+pos,s.begin()+p,r.begin()+rs);
	  copy(&s[0]+pos,&s[0]+p,&r[0]+rs);
	  copy(s2.begin(),s2.end(),&r[0]+rs+p-pos);
	  pos=p+s1.size();
	  p=s.find(s1,pos);
	}
      }
      rs = r.size();
      r.resize(rs+s.size()-pos);
      copy(&s[0]+pos,&s[0]+s.size(),
	   &r[0]+rs);
    }

  } // end of namespace utilities

} // end of namespace tfel
