/*!
 * \file   src/Math/Expr.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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
