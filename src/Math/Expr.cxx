/*!
 * \file   src/Math/Expr.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#include<algorithm>

#include"TFEL/Math/Parser/Expr.hxx"

namespace tfel
{
  namespace math
  {

    namespace parser
    {

      Expr::~Expr()
      {} // end of Expr::~Expr

      void
      mergeVariablesNames(std::vector<std::string>& v,
			  const std::vector<std::string>& nv)
      {
	using namespace std;
	vector<string>::const_iterator p;
	for(p=nv.begin();p!=nv.end();++p){
	  if(find(v.begin(),v.end(),*p)==v.end()){
	    v.push_back(*p);
	  }
	}
      } // end of mergeVariablesNames

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
