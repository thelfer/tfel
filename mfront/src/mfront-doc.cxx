/*! 
 * \file  mfront-doc.cxx
 * \brief
 * \author Helfer Thomas
 * \date   22 mars 2015
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
#include"MFront/BehaviourDocumentationGenerator.hxx"
#include"MFront/AbstractDSL.hxx"
#include"MFront/AbstractBehaviourDSL.hxx"

int main(const int argc, const char *const *const argv)
{
  using namespace std;
  using namespace mfront;
  using BGen = BehaviourDocumentationGenerator;
  initParsers();
  initInterfaces();
  vector<shared_ptr<BGen>> bgens;
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
	    throw(runtime_error("mfront-doc : invalid dsl implementation"));
	  }
	  bgens.push_back(make_shared<BGen>(argc,argv,b,f));
	} else {
	  throw(runtime_error("mfront-doc : unsupported dsl type"));
	}
      } else if ((strcmp(*a,"--help")==0)||(strcmp(*a,"-h")==0)){
	cout << "Usage : " << argv[0] << " [options] [files]" << endl;
	cout << "Available options are :" << endl
	     << "--help,-h : print this message" << endl
	     << "--usage   : show how to use " << argv[0] << endl;
	exit(EXIT_SUCCESS);
      } else if ((strcmp(*a,"--version")==0)||(strcmp(*a,"-v")==0)){
	cout << MFrontHeader::getHeader();
	exit(EXIT_SUCCESS);
      } else if (strcmp(*a,"--usage")==0){
	cout << "Usage : " << argv[0] << " [options] [files]" << endl;
	exit(EXIT_SUCCESS);
      }
    }
    for(auto q : bgens){
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
