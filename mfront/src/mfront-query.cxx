/*! 
 * \file  mfront-query.cxx
 * \brief
 * \author Helfer Thomas
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<windows.h>
#endif /* __CYGWIN__ */

#include<cstdlib>
#include<cstring>
#include<iostream>

#include"MFront/InitDSLs.hxx"
#include"MFront/InitInterfaces.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontBase.hxx"
#include"MFront/BehaviourQuery.hxx"
#include"MFront/AbstractDSL.hxx"
#include"MFront/AbstractBehaviourDSL.hxx"

int main(const int argc, const char *const *const argv)
{
  using namespace std;
  using namespace mfront;
  initParsers();
  initInterfaces();
  vector<shared_ptr<BehaviourQuery>> bqueries;
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
  try{
#endif /* __CYGWIN__ */
    for(auto a=argv+1;a!=argv+argc;++a){
      if((*a)[0]!='-'){
	const string f{*a};
	auto dsl = MFrontBase::getDSL(f);
	if(dsl->getTargetType()==AbstractDSL::BEHAVIOURDSL){
	  auto b = dynamic_pointer_cast<AbstractBehaviourDSL>(dsl);
	  if(!b){
	    throw(runtime_error("mfront-query : invalid dsl implementation"));
	  }
	  bqueries.push_back(make_shared<BehaviourQuery>(argc,argv,b,f));
	} else {
	  throw(runtime_error("mfront-query : unsupported dsl type"));
	}
      } else if ((strcmp(*a,"--help")==0)||(strcmp(*a,"-h")==0)){
	cout << "Usage : " << argv[0] << " [options] [files]" << endl;
	cout << "Available options are :" << endl
	     << "--help,-h                     : print this message" << endl
	     << "--usage                       : show how to use " << argv[0] << endl
	     << "--help-behaviour-queries-list : list all queries avaiable for mfront behaviour files" << endl;
	exit(EXIT_SUCCESS);
      } else if ((strcmp(*a,"--version")==0)||(strcmp(*a,"-v")==0)){
	cout << MFrontHeader::getHeader();
	exit(EXIT_SUCCESS);
      } else if (strcmp(*a,"--usage")==0){
	cout << "Usage : " << argv[0] << " [options] [files]" << endl;
	exit(EXIT_SUCCESS);
      } else if (strcmp(*a,"--help-behaviour-queries-list")==0){
	const char* args[2] = {argv[0],"--help"};
	shared_ptr<AbstractBehaviourDSL> b;
	auto bq = make_shared<BehaviourQuery>(2,args,b,"");
	exit(EXIT_SUCCESS);
      }
    }
    for(auto q : bqueries){
      q->exe();
    }
#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
  }
  catch(exception& e){
    MessageBox(0,e.what(),
	       "mfront",0);
    return EXIT_FAILURE;
  }
  catch(...){
    return EXIT_FAILURE;
  }
#endif /* __CYGWIN__ */
  return EXIT_SUCCESS;
}
